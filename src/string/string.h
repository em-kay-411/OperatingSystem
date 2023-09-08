#ifndef STRING_H
#define STRING_H

#include <stdbool.h>

int strlen(const char * ptr);
int strnlen(const char * ptr, int max);
int to_digit(char c);
bool isDigit(char c);
char * strcpy(char * dest, const char * src);
int strncmp(const char * s1, const char * s2, int n);
int istrncmp(const char *s1, const char *s2, int n);
int strnlen_terminator(const char * str, int max, char terminator);
char toLower(char a);

#endif