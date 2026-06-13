#include "str.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_CAP 1024

String *readFile(char *path) {
  String *dBuffer;

  FILE *file = fopen(path, "r");
  if (file == NULL) {
    printf("Error: File couldn't be accessed !!");
    exit(1);
  }

  char tempBuffer[512];
  fread(tempBuffer, sizeof(char), 512, file);

  dBuffer = createString(tempBuffer);

  while (1) {
    memset(tempBuffer, 0, sizeof(tempBuffer));
    size_t bytesRead = fread(tempBuffer, sizeof(char), 512, file);
    stringAppend(&dBuffer, tempBuffer);

    if (bytesRead < 512)
      break;
  }

  return dBuffer;
}

#ifdef IO_DEBUG
int main(int argc, char **argv) {
  if (argc <= 1) {
    printf("Error: Arguments not enough !!\n");
    return 1;
  }

  String *fileContent = readFile(argv[1]);

  printf("File Content Size -> %lu\n", fileContent->size);
  printf("File Content Cap-> %lu\n", fileContent->capacity);
  printf("__________________________________________\n");
  printf("%s", fileContent->str);

  return 0;
}
#endif
