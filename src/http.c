#include "header.h"
#include "io.h"
#include "network.h"
#include "parser.h"
#include "str.h"
#include <bits/pthreadtypes.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

String *fileType(String *path) {
  String *fileType;

  if (strstr(path->str, "html"))
    fileType = createString("text/html");
  else if (strstr(path->str, "png"))
    fileType = createString("image/png");
  else if (strstr(path->str, "js"))
    fileType = createString("text/javascript");
  else
    fileType = createString("text/html");

  return fileType;
}

void *sendResponse(void *connectionSocket) {
  char buffer[2056];
  printf("Getting Request Data!!\n");
  getRequestData(*((int *)connectionSocket), buffer, sizeof(buffer));

  RequestHeader header;
  parseHeader(buffer, &header);

  String *filePath = createString("web/");
  stringAppend(&filePath, header.path);

  String *fileText = readFile(filePath->str);
  String *type = fileType(filePath);
  String *resHeader =
      createResponseHeader(200, "OK", fileText->size, type->str);

  stringCat(&resHeader, fileText);

  int bytes =
      send(*((int *)connectionSocket), resHeader->str, resHeader->size, 0);

  printf("Sent: %d bytes!!\n", bytes);

  return NULL;
}

void responceFork(int *fd) {
  pthread_t thread;
  pthread_create(&thread, NULL, sendResponse, fd);
}

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
  listen(sock, 100);

  while (1) {
    struct sockaddr_storage incomingAddr;
    printf("Accepting !!\n");
    int connectionSocket = acceptConnection(sock, &incomingAddr);
    printf("Accepted !!\n");

    if (connectionSocket != -1)
      sendResponse(&connectionSocket);
    else {
      printf("Error: Something happened!!\n");
    }
    close(connectionSocket);
    // responceFork(&connectionSocket);
  }

  return 0;
}
