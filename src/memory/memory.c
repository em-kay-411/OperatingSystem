#include "memory.h"

void* memset(void * ptr, int c, size_t size){
    char * temp = (char*) ptr;

    for(int i=0; i<size; i++){
        temp[i] = (char) c;
    }

    return ptr;
}

int memcmp(void * s1, void *s2, int count){
    char * ptr1 = s1;
    char * ptr2 = s2;

    while(count--){
        if(*ptr1++ != *ptr2++){
            return ptr1[-1] < ptr2[-1] ? -1 : 1;
        }
    }

    return 0;
}