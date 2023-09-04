#include "memory.h"

void* memset(void * ptr, int c, size_t size){
    char * temp = (char*) ptr;

    for(int i=0; i<size; i++){
        temp[i] = (char) c;
    }

    return ptr;
}