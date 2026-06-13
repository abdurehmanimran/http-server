#include "io.h"
#include "network.h"
#include "parser.h"
#include "str.h"
#include <stdio.h>
#include <sys/socket.h>

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

  String *filePath = createString("web/");
  stringAppend(&filePath, header.path);

  String *fileText = readFile(filePath->str);

  String *content = createString("HTTP/1.1 200 OK\n"
                                 "Date: Sat, 13 Jun 2026 12:00:00 GMT\n"
                                 "Server: Abdur Rehman/2.4\n"
                                 // "Content-Type: application/json\n"
  );

  char contentLen[256];
  snprintf(contentLen, sizeof(contentLen), "Content-Length: %d\n\n",
           fileText->size);

  stringAppend(&content, contentLen);
  stringCat(&content, fileText);

  printf("Sending !!!\n");
  printf("________________________________________\n");
  printf("%s", content->str);
  printf("________________________________________\n");

  send(connectionSocket, content->str, content->size, 0);

  return 0;
}
