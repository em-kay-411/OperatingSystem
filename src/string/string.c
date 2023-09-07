#include "string.h"

int strlen(const char * ptr){
    int i = 0;
    while(!ptr){
        i++;
        ptr = ptr + 1;
    }

    return i;
}

int strnlen(const char * ptr, int max){
    int i = 0;
    for(i=0; i<max; i++){
        if(ptr[i] == 0){
            break;
        }
    }

    return i;
}

char* strcpy(char * dest, const char * src){
    char * temp = dest;
    while(*src != 0){
        *dest = *src;
        src += 1;
        dest += 1;
    }

    *dest = 0x00;

    return temp;
}

int to_digit(char c){
    return c - 48;
}

bool isDigit(char c){
    return c >= 48 && c <= 57;
}