#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT "3490" // the port client will be connecting to

#define MAXDATASIZE 100 // max number of bytes we can get at once

// Error codes for socket commands
#define RECEIVE_ERROR -1

void * get_in_addr(struct sockaddr *sa)
{
  if (sa->sa_family == AF_INET) {
    return &(((struct sockaddr_in*)sa)->sin_addr);
  }
  return &(((struct sockaddr_in6*)sa)->sin6_addr);
}
int main(int argc, char *argv[])
{
  if (argc != 2) {
    fprintf(stderr, "usage: client hostname\n");
    exit(1);
  }

  struct addrinfo hints;
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  struct addrinfo *results;
  int status = getaddrinfo(argv[1], PORT, &hints, &results);
  if (status != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
    return 1;
  }

  // loop through all the results and connect to the first we can
  struct addrinfo *p;
  int socket_file_descriptor;
  for (p = results; p != NULL; p = p->ai_next) {
    socket_file_descriptor = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
    if (socket_file_descriptor == -1) {
      perror("client: socket");
      continue;
    }

    if (connect(socket_file_descriptor, p->ai_addr, p->ai_addrlen) == -1) {
      close(socket_file_descriptor);
      perror("client: connect");
      continue;
    }

    break;
  }

  if ( p == NULL) {
    fprintf(stderr, "client: failed to connect\n");
    return 2;
  }

  char s[INET6_ADDRSTRLEN];
  inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr), s, sizeof s);
  printf("client: connecting to %s\n", s);

  freeaddrinfo(results);

  //char buf[MAXDATASIZE];

  printf("client: sending to server\n");
  int num_sent_bytes = send(socket_file_descriptor, "HEY", 3, 0);
  printf("client: num_sent_bytes %d\n", num_sent_bytes);

  char receive_buffer[MAXDATASIZE];
  int num_received_bytes = recv(socket_file_descriptor, receive_buffer, MAXDATASIZE-1, 0);
  if (num_received_bytes == RECEIVE_ERROR) {
    perror("client: recv");
    exit(1);
  }
  printf("client: num_received_bytes %d\n", num_received_bytes);
  receive_buffer[num_received_bytes] = '\0';
  printf("client: recieved %s\n", receive_buffer);

  //// TODO: recv should be a loop that checks to make sure there isn't more data to grab
  //int num_bytes = recv(socket_file_descriptor, buf, MAXDATASIZE-1, 0);
  //if (num_bytes == -1) {
  //  perror("recv");
  //  exit(1);
  //}

  //buf[num_bytes] = '\0';

  //printf("client: received '%s'\n", buf);

  close(socket_file_descriptor);

  return 0;
}
