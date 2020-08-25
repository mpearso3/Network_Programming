#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include "helper.h"

#define PORT "3490" // the port users will be connecting to

#define BACKLOG 10 // how many pending connections queue will hold

void signal_child_handler(int s) // TODO: What is the s for?
{
  // waitpid() might overwrite errno, save and restore it
  int saved_errno = errno;

  while (waitpid(-1, NULL, WNOHANG) > 0) {
  }

  errno = saved_errno;
}

int main(void)
{
  printf("Starting server...\n\n");

  struct addrinfo hints;
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE; // use my IP

  struct addrinfo *results;
  int status = getaddrinfo(NULL, PORT, &hints, &results);
  if (status != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
    return 1;
  }

  // loop through all results and bind to the first we can
  struct addrinfo *p;
  int socket_file_descriptor;
  int yes = 1;
  for (p = results; p != NULL; p = p->ai_next) {
    socket_file_descriptor = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
    if (socket_file_descriptor == -1) {
      perror("server: socket");
      continue;
    }

    if (setsockopt(socket_file_descriptor, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
      perror("setsockopt");
      exit(1);
    }

    if (bind(socket_file_descriptor, p->ai_addr, p->ai_addrlen) == -1) {
      close(socket_file_descriptor);
      perror("server: bind");
      continue;
    }

    break;
  }

  freeaddrinfo(results);

  if (p == NULL) {
    fprintf(stderr, "server: failed to bind\n");
    exit(1);
  }

  if (listen(socket_file_descriptor, BACKLOG) == -1) {
    perror("listen");
    exit(1);
  }

  struct sigaction signal_action;
  signal_action.sa_handler = signal_child_handler; // read all dead processes
  sigemptyset(&signal_action.sa_mask);
  signal_action.sa_flags = SA_RESTART;
  if (sigaction(SIGCHLD, &signal_action, NULL) == -1) {
    perror("sigaction");
    exit(1);
  }

  printf("server: waiting for connection...\n");

  char s[INET6_ADDRSTRLEN];
  socklen_t sin_size;
  struct sockaddr_storage their_addr;
  int new_file_descriptor;

  while(1) { // main accept loop
    sin_size = sizeof their_addr;
    new_file_descriptor = accept(socket_file_descriptor, (struct sockaddr *)&their_addr, &sin_size);
    if (new_file_descriptor == -1) {
      perror("accept");
      continue;
    }

    inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr), s, sizeof s);

    printf("server: got connection from %s\n", s);

    if (fork() == 0) { //  this is the child process (fork() returns 0 for child)
      close(socket_file_descriptor); // child does not need the listener, only parent does
      if (send(new_file_descriptor, "Hello, World", 12, 0) == -1) {
        perror("send");
      }
      close(new_file_descriptor);
      exit(0);
    }
    close (new_file_descriptor); // parent does not need this, only chld does
  }

  return 0;
}
