#ifndef HEADER_H
#define HEADER_H

#include "str.h"

String *getDateTime();
String *createResponseHeader(int status, char *reason, int contentLen,
                             char *contentType);

#endif // !DEBUG
