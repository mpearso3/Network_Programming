#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main(int argc, char * argv[])
{
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
  if ((status = getaddrinfo(argv[1], NULL, &hints, &results)) != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
    return 2;
  }

  printf("IP Address for %s:\n\n", argv[1]);

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
