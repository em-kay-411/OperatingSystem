#ifndef STRING_H
#define STRING_H

#include <stdbool.h>

int strlen(const char * ptr);
int strnlen(const char * ptr, int max);
int to_digit(char c);
bool isDigit(char c);

#endif