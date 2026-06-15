#include "colors.h"
#include "header.h"
#include "io.h"
#include "network.h"
#include "parser.h"
#include "str.h"
#include <bits/pthreadtypes.h>
#include <netdb.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

// Not complete but enough for now
String *fileType(String *path) {
  String *fileType;

  if (strstr(path->str, "html"))
    fileType = createString("text/html");
  else if (strstr(path->str, "png"))
    fileType = createString("image/png");
  else if (strstr(path->str, "js"))
    fileType = createString("text/javascript");
  else if (strstr(path->str, "css"))
    fileType = createString("text/css");
  else if (strstr(path->str, "jpg"))
    fileType = createString("image/jpeg");
  else if (strstr(path->str, "webp"))
    fileType = createString("image/webp");
  else if (strstr(path->str, "ttf"))
    fileType = createString("font/ttf");
  else
    fileType = createString("text/html");

  return fileType;
}

void *sendResponse(void *connectionSocket) {
  char buffer[2056];
  printf(RED ":: " GREEN "Getting " WHITE "Request Data!!\n");
  getRequestData(*((int *)connectionSocket), buffer, sizeof(buffer));

  RequestHeader header;
  parseHeader(buffer, &header);

  String *filePath = createString("web/");
  String *halfPath = createString(header.path);

  replaceInString(&halfPath, "%20", " ");
  stringCat(&filePath, halfPath);
  freeString(halfPath);

  String *fileText = readFile(filePath->str);
  String *type = fileType(filePath);
  String *resHeader =
      createResponseHeader(200, "OK", fileText->size, type->str);

  stringCat(&resHeader, fileText);

  int bytes =
      send(*((int *)connectionSocket), resHeader->str, resHeader->size, 0);

  printf(RED "::" GREEN " Sent:" WHITE " %d bytes!!\n", bytes);

  // Freeing up memory
  freeString(filePath);
  freeString(fileText);
  freeString(resHeader);
  freeString(type);
  free(header.path);

  // Also, close the socket
  close(*((int *)connectionSocket));

  return NULL;
}

// Unused function waiting for me
void responceFork(int *fd) {
  pthread_t thread;
  pthread_create(&thread, NULL, sendResponse, fd);
}

int main() {
  struct addrinfo *results;

  printf(RED ":: " WHITE "Creating " GREEN "address structures!!" WHITE "\n");
  initAddrInfo(&results);

  int sock = createINETSock();
  printf(RED ":: " WHITE "Created " GREEN "socket " WHITE "-> %d\n", sock);

  resetPort(sock);
  printf(RED ":: " GREEN "Binding " WHITE "!!\n");
  bindSock(sock, results);

  printf(RED ":: " GREEN "Listening" WHITE " !!\n");
  listen(sock, 100);

  while (1) {
    struct sockaddr_storage incomingAddr;

    printf("\n");
    printf(RED ":: " GREEN "Accepting Connection " WHITE "...... !!\n");
    int connectionSocket = acceptConnection(sock, &incomingAddr);
    printf(RED "::" GREEN " Connection Accepted " WHITE ".... .. !!\n");

    if (connectionSocket != -1)
      sendResponse(&connectionSocket);
    else {
      printf("Error: Something happened!!\n");
    }
  }

  // Clean up the stuff
  close(sock);
  freeaddrinfo(results);

  return 0;
}
