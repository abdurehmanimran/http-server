#include "network.h"
#include "parser.h"
#include <stdio.h>

int main() {
  struct addrinfo *results;

  printf("Creating address structures!!\n");
  initAddrInfo(&results);

  int sock = createINETSock();
  printf("Created socket -> %d\n", sock);

  resetPort(sock);
  printf("Binding !!\n");
  bindSock(sock, results);

  printf("Listening!!\n");
  listen(sock, 20);

  struct sockaddr_storage incomingAddr;
  printf("Accepting !!\n");
  int connectionSocket = acceptConnection(sock, &incomingAddr);
  printf("Accepted !!\n");

  char buffer[2056];
  getRequestData(connectionSocket, buffer, sizeof(buffer));

  RequestHeader header;
  parseHeader(buffer, &header);

  printf("Type -> ");
  switch (header.type) {
  case GET:
    printf("GET");
    break;
  case POST:
    printf("POST");
    break;
  default:
    printf("NA");
    break;
  }
  printf("\n");

  printf("Path -> %s\n", header.path);

  return 0;
}
