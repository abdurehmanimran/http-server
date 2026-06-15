#include "header.h"
#include "str.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

String *getDateTime() {
  String *dateTime = createString("Date: ");
  char date[128];
  char *day;
  char *month;

  time_t raw_time;
  time(&raw_time);

  struct tm *time;
  time = gmtime(&raw_time);

  switch (time->tm_wday) {
  case 0:
    day = "Sun";
    break;
  case 1:
    day = "Mon";
    break;
  case 2:
    day = "Tue";
    break;
  case 3:
    day = "Wed";
    break;
  case 4:
    day = "Thu";
    break;
  case 5:
    day = "Fri";
    break;
  case 6:
    day = "Sat";
    break;
  }

  switch (time->tm_mon) {
  case 0:
    month = "Jan";
    break;
  case 1:
    month = "Feb";
    break;
  case 2:
    month = "Mar";
    break;
  case 3:
    month = "Apr";
    break;
  case 4:
    month = "May";
    break;
  case 5:
    month = "Jun";
    break;
  case 6:
    month = "Jul";
    break;
  case 7:
    month = "Aug";
    break;
  case 8:
    month = "Sep";
    break;
  case 9:
    month = "Oct";
    break;
  case 10:
    month = "Nov";
    break;
  case 11:
    month = "Dec";
    break;
  }

  snprintf(date, sizeof(date), "%s, %2d %s %d %.2d:%.2d:%.2d GMT\r\n", day,
           time->tm_mday, month, time->tm_year + 1900, time->tm_hour,
           time->tm_min, time->tm_sec);

  stringAppend(&dateTime, date);
  return dateTime;
}

String *createResponseHeader(int status, char *reason, int contentLen,
                             char *contentType) {
  char *httpVer = "HTTP/1.1";
  char *server = "Server: http-server/0.1\r\n";
  char tempBuff[256];

  snprintf(tempBuff, sizeof(tempBuff), "%s %d %s\r\n", httpVer, status, reason);
  String *responseHeader = createString(tempBuff);
  String *datetime = getDateTime();
  stringCat(&responseHeader, datetime);
  stringAppend(&responseHeader, server);

  memset(tempBuff, 0, sizeof(tempBuff));
  snprintf(tempBuff, sizeof(tempBuff), "Content-Type: %s\r\n", contentType);
  stringAppend(&responseHeader, tempBuff);

  memset(tempBuff, 0, sizeof(tempBuff));
  snprintf(tempBuff, sizeof(tempBuff), "Content-Length: %d\r\n", contentLen);
  stringAppend(&responseHeader, tempBuff);
  stringAppend(&responseHeader, "\r\n");

  // Freeing up memory
  freeString(datetime);

  return responseHeader;
}
