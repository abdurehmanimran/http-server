#ifndef STR_H
#define STR_H

typedef struct {
  unsigned int size;
  unsigned int capacity;
  char *str;
} String;

#define EXPAND_CAP 512

void allocString(String **str, unsigned int cap);
String *createString(char *str);
void expandStringCap(String **str);
void stringAppend(String **dest, char *srcStr);
void stringCat(String **dest, String *src);
void stringAppendData(String **dest, char *srcStr, int bytes);
String *createStringData(char *str, unsigned int bytes);

#endif // !STR_H
