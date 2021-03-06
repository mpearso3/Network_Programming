#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>

int struct_ip_example(int argc, char *url);
int socket_bind_example();
int socket_connect_example();
int socket_listen_example();
int socket_accept_example();

int main(int argc, char * argv[])
{
  int status = struct_ip_example(argc, argv[1]);
  if (status) {
    printf("failed socket_ip_example\n");
  }

  status = socket_bind_example();
  if (status) {
    printf("failed socket_bind_example\n");
  }

  //status = socket_connect_example(); // hangs
  if (status) {
    printf("failed socket_connect_example\n");
  }

  status = socket_listen_example();
  if (status) {
    printf("failed socket_listen_example\n");
  }

  //status = socket_accept_example(); // hangs
  if (status) {
    printf("failed socket_accept_example\n");
  }

  return status;
}

int socket_connect_example()
{
  printf("\n");
  printf(__FUNCTION__);
  printf("\n");

  struct addrinfo hints;
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  struct addrinfo *results;
  int status = getaddrinfo("www.example.com", "3490", &hints, &results);
  if (status != 0) {
    fprintf(stderr, "Failed getaddrinfo %s\n", gai_strerror(status));
    return 5;
  }

  int socket_file_descriptor = socket(results->ai_family, results->ai_socktype, results->ai_protocol);
  if (socket_file_descriptor == -1) {
    fprintf(stderr, "Failed socket %d\n", socket_file_descriptor);
    return 6;
  }

  printf("connecting...\n");
  status = connect(socket_file_descriptor, results->ai_addr, results->ai_addrlen);
  if (status == -1) {
    fprintf(stderr, "Failed connect %d\n", status);
    return 7;
  }
  printf("done connecting...\n");

  freeaddrinfo(results);
  return 0;
}

int socket_accept_example()
{
  printf("\n");
  printf(__FUNCTION__);
  printf("\n");

  struct addrinfo hints;
  memset(&hints, 0, sizeof hints);
  hints.ai_flags = AI_PASSIVE;
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  struct addrinfo *results;
  int status = getaddrinfo(NULL, "3491", &hints, &results);
  if (status != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
    return 8;
  }

  int socket_file_descriptor = socket(results->ai_family, results->ai_socktype, results->ai_protocol);
  if (socket_file_descriptor == -1) {
    fprintf(stderr, "Failed socket %d\n", socket_file_descriptor);
    return 9;
  }

  int yes = 1;
  if (setsockopt(socket_file_descriptor, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes) == -1) {
    perror("setsockopt");
    return 10;
  }

  int bind_status = bind(socket_file_descriptor, results->ai_addr, results->ai_addrlen);
  printf("bind %d\n", bind_status);

  const int BACKLOG = 10;
  int listen_file_descriptor = listen(socket_file_descriptor, BACKLOG);
  printf("listen %d\n", listen_file_descriptor);

  struct sockaddr_storage their_addr;
  socklen_t addr_size;
  int new_file_descriptor = accept(socket_file_descriptor, (struct sockaddr *)&their_addr, &addr_size);
  printf("accept %d\n", new_file_descriptor);
  printf("errno %d\n", errno);

  freeaddrinfo(results);
  return 0;
}

int socket_listen_example()
{
  printf("\n");
  printf(__FUNCTION__);
  printf("\n");

  struct addrinfo hints;
  memset(&hints, 0, sizeof hints);
  hints.ai_flags = AI_PASSIVE;
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  struct addrinfo *results;
  int status = getaddrinfo(NULL, "3490", &hints, &results);
  if (status != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
    return 8;
  }

  int socket_file_descriptor = socket(results->ai_family, results->ai_socktype, results->ai_protocol);
  if (socket_file_descriptor == -1) {
    fprintf(stderr, "Failed socket %d\n", socket_file_descriptor);
    return 9;
  }

  int yes = 1;
  if (setsockopt(socket_file_descriptor, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes) == -1) {
    perror("setsockopt");
    return 10;
  }

  int bind_status = bind(socket_file_descriptor, results->ai_addr, results->ai_addrlen);
  printf("bind %d\n", bind_status);

  const int BACKLOG = 10;
  int listen_status = listen(socket_file_descriptor, BACKLOG);
  printf("listen %d\n", listen_status);

  freeaddrinfo(results);
  return 0;
}

int socket_bind_example()
{
  printf("\n");
  printf(__FUNCTION__);
  printf("\n");

  struct addrinfo hints;
  memset(&hints, 0, sizeof hints);
  hints.ai_flags = AI_PASSIVE;
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  struct addrinfo *results;
  int status = getaddrinfo(NULL, "3490", &hints, &results);
  if (status != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
    return 3;
  }

  // TODO:
  //  You should do error-checking on getaddrinfo() [done above] and walk the results linked list
  //  looking for valid entries instead of just assuming the first one is good (like
  //  many of these examples do).  See the section on client/server for real examples.

  int socket_file_descriptor = socket(results->ai_family, results->ai_socktype, results->ai_protocol);
  printf("socket_file_descriptor %d\n", socket_file_descriptor);

  // This will fix the bind error: "Address already in use" by allowing the port to be reused
  int yes = 1;
  if (setsockopt(socket_file_descriptor, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes) == -1) {
    perror("setsockopt");
    return 4;
  }

  // bind it to the port we passed in to getaddrinfo();
  int bind_status = bind(socket_file_descriptor, results->ai_addr, results->ai_addrlen);
  printf("bind %d\n", bind_status);

  freeaddrinfo(results);
  return 0;
}

int struct_ip_example(int argc, char * url)
{
  printf("\n");
  printf(__FUNCTION__);
  printf("\n");

  struct addrinfo hints;
  struct addrinfo *results;

  if (argc != 2) {
    fprintf(stderr, "usage: showip hostname\n");
    return 1;
  }

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_INET6;
  hints.ai_socktype = SOCK_STREAM;

  int status;
  if ((status = getaddrinfo(url, NULL, &hints, &results)) != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
    return 2;
  }

  printf("IP Address for %s:\n\n", url);

  char ipstr[INET6_ADDRSTRLEN];
  struct addrinfo *p;
  for ( p = results; p != NULL; p = p->ai_next) {
    void *addr;
    char *ipver;

    if (p->ai_family == AF_INET) {
      struct sockaddr_in *ipv4 = (struct sockaddr_in *) p->ai_addr;
      addr = &(ipv4->sin_addr);
      ipver = "IPv4";
    }
    else if (p->ai_family == AF_INET6) {
      struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *) p->ai_addr;
      addr = &(ipv6->sin6_addr);
      ipver = "IPv6";
    }
    else {
      fprintf(stderr, "ai_family does not exist\n");
    }

    // convert the IP address to a string and print it
    inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr);
    printf("IP Address %s, ver: %s\n", ipstr, ipver);
  }

  freeaddrinfo(results);
  return 0;
}
