#include "kheap.h"
#include "heap.h"
#include "config.h"
#include "kernel.h"

struct heap kernel_heap;
struct heap_table kernel_heap_table;

void kheap_init(){
    int total_table_entries = HEAP_SIZE_BYTES / HEAP_BLOCK_SIZE;                // Find the total number of table entries
    kernel_heap_table.entries = (HEAP_BLOCK_TABLE_ENTRY*) (HEAP_TABLE_ADDRESS); // Each entry is going to be an unsigned char which was typedefed and pointed to heap table address
    kernel_heap_table.total = total_table_entries;

    void * start = (void*)(HEAP_ADDRESS);
    void * end = (void*)(HEAP_ADDRESS + HEAP_SIZE_BYTES);

    int res = create_heap(&kernel_heap, start, end, &kernel_heap_table);

    if(res < 0){
        print("Failed to create heap\n");
    }
}

void * kmalloc(size_t size){
    return heap_malloc(&kernel_heap, size);                      
}

void kfree(void * ptr){
    heap_free(&kernel_heap, ptr);
}