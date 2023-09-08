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

char toLower(char a){
    if(a >= 65 && a <= 90){
        a += 32;
    }

    return a;
}

int istrncmp(const char *s1, const char *s2, int n){
    unsigned char u1, u2;

    while(n--){
        u1 = (unsigned char)*s1++;
        u2 = (unsigned char)*s2++;

        if(u1 != u2 && toLower(u1) != toLower(u2)){
            return u1 - u2;
        }

        if(u1 == '\0'){
            return 0;
        }
    }

    return 0;
}

int strncmp(const char * s1, const char * s2, int n){
    unsigned char u1, u2;

    while(n--){
        u1 = (unsigned char)*s1++;
        u2 = (unsigned char)*s2++;

        if(u1 != u2){
            return u1 - u2;
        }

        if(u1 == '\0'){
            return 0;
        }
    }

    return 0;
}

// Function to find the terminator or null temrinator
int strnlen_terminator(const char * str, int max, char terminator){
    int i = 0;
    for(i = 0; i<max; i++){
        if(str[i] == '\0' || str[i] == terminator){
            break;
        }
    }

    return i;
}

int to_digit(char c){
    return c - 48;
}

bool isDigit(char c){
    return c >= 48 && c <= 57;
}