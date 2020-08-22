#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int struct_ip_example(int argc, char *url);
int socket_bind_example();
int socket_connect_example();

int main(int argc, char * argv[])
{
  int status = struct_ip_example(argc, argv[1]);
  if (status) {
    printf("failed struct_ip_example\n");
  }

  status = socket_bind_example();
  if (status) {
    printf("failed struct_bind_example\n");
  }

  socket_connect_example();
  if (status) {
    printf("failed struct_connect_example\n");
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

  return 0;
}
