#ifndef STR_H
#define STR_H

typedef struct {
  unsigned int size;
  unsigned int capacity;
  char *str;
} String;

#define EXPAND_CAP 512

void allocString(String **str, unsigned int cap);
void expandStringCap(String **str);
void freeString(String *ptr);

String *createString(char *str);
String *createStringData(char *str, unsigned int bytes);

void stringAppend(String **dest, char *srcStr);
void stringAppendData(String **dest, char *srcStr, int bytes);

void stringCat(String **dest, String *src);
void replaceInString(String **str, char *find, char *replace);

#endif // !STR_H
