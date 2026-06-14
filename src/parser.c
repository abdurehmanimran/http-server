#include "parser.h"
#include <string.h>

int parseHeader(char *buffer, RequestHeader *header) {
  char *line_p;
  char *firstLine = strtok_r(buffer, "\n", &line_p);

  while (!strstr(firstLine, "GET")) {
    firstLine = strtok_r(NULL, "\n", &line_p);

    if (firstLine == NULL)
      return -1;
  }

  if (firstLine != NULL) {
    char *part_p;
    char *part = strtok_r(firstLine, " ", &part_p);

    if (strcmp(part, "GET") == 0)
      header->type = GET;
    else if (strcmp(part, "POST") == 0)
      header->type = POST;

    part = strtok_r(NULL, " ", &part_p);

    header->path = strdup(part);

    return 1;
  } else
    return -1;
}
