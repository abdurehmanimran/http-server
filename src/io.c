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
    printf("Error: %s couldn't be accessed !!", path);
    // exit(1);
    dBuffer = createString("");
    return dBuffer;
  }

  char tempBuffer[513];
  int bytes = fread(tempBuffer, sizeof(char), 512, file);

  dBuffer = createStringData(tempBuffer, bytes);

  while (1) {
    memset(tempBuffer, 0, sizeof(tempBuffer));
    size_t bytesRead = fread(tempBuffer, sizeof(char), 513, file);
    stringAppendData(&dBuffer, tempBuffer, bytesRead);

    if (bytesRead < 51) {
      if (feof(file))
        break;
    }
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
