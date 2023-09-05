#ifndef HEAP_H
#define HEAP_H
#include "config.h"
#include <stdint.h>
#include <stddef.h>

// Defining the lower  bytes for entry type.
// We will have only two entry types for the heap
#define HEAP_BLOCK_TABLE_ENTRY_TAKEN 0x01   
#define HEAP_BLOCK_TABLE_ENTRY_FREE 0x00

// HAS_N is the highest bit and next is the is free bit.
#define HEAP_BLOCK_HAS_N    0b10000000          // If the next block is the part of allocation
#define HEAP_BLOCK_IS_FIRST  0b01000000          // If the current block is the first entry of allocation

typedef unsigned char HEAP_BLOCK_TABLE_ENTRY;

struct heap_table{
    HEAP_BLOCK_TABLE_ENTRY * entries;
    size_t total;
};

struct heap{
    struct heap_table* table;

    // Start address of the heap data pool
    void *saddr;
};

int create_heap(struct heap* heap, void *ptr, void *end, struct heap_table* table);
void * heap_malloc(struct heap* heap, size_t size);
void heap_free(struct heap * heap, void * ptr);

#endif