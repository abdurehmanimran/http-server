#include "network.h"
#include <arpa/inet.h>
#include <netdb.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

void initAddrInfo(struct addrinfo **resultList) {
  struct addrinfo hints;
  memset(&hints, 0, sizeof(hints));

  hints.ai_family = AF_UNSPEC;
  hints.ai_flags = AI_PASSIVE;
  hints.ai_socktype = SOCK_STREAM;

  if (getaddrinfo(NULL, "8080", &hints, resultList) != -1) {
    return;
  }

  printf("Error: Failed to setup addrinfo structures!!\n");
  exit(1);
}

int createINETSock() { return socket(PF_INET, SOCK_STREAM, 0); }

void resetPort(int sock) {
  int yes = 1;
  setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
}

int bindSock(int sock, struct addrinfo *addrStruct) {
  return bind(sock, addrStruct->ai_addr, addrStruct->ai_addrlen);
}

void createConnection(int sock, struct addrinfo *addrStruct) {
  if (connect(sock, addrStruct->ai_addr, addrStruct->ai_addrlen) == -1) {
    printf("Error: Failed to make a connection!!\n");
    exit(1);
  }
}

int acceptConnection(int sock, struct sockaddr_storage *incomingAddr) {
  socklen_t size = sizeof(*incomingAddr);
  int new_fd = accept(sock, (struct sockaddr *)incomingAddr, &size);

  if (new_fd != -1)
    return new_fd;
  else {
    printf("Error: Failed to accept!!\n");
    exit(1);
  };
}

void getRequestData(int fd, char *buffer, size_t size) {
  memset(buffer, 0, size);

  if (recv(fd, buffer, size, 0) != -1) {
    printf("_______________________________\n");
    printf("%s", buffer);
    printf("_______________________________\n");
  } else {
    printf("Error: Failed to recieve the request data !!\n");
  }
}

#ifdef NETWORK_DEBUG
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

  return 0;
}
#endif
