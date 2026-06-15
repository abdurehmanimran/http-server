#ifndef PARSER_H
#define PARSER_H

#define GET 2555
#define POST 2556
#include "str.h"

typedef struct {
  int type;
  char *path;
  unsigned int pathLen;
} RequestHeader;

int parseHeader(char *buffer, RequestHeader *header);
void removeURLParms(String *str);

#endif // !PARSER_H
