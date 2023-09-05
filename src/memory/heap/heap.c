#include "heap.h"
#include "kernel.h"
#include "status.h"
#include "memory/memory.h"

static int validate_heap_table(void *ptr, void *end, struct heap_table* table){
    int res = 0;

    size_t table_size = (size_t)(end - ptr);
    size_t total_blocks = table_size / HEAP_BLOCK_SIZE;
    if(total_blocks != table->total){
        res = -EINVARG;
        goto out;
    }

out:
    return res;

}

static int validate_alignment(void * ptr){
    return (((unsigned int)ptr % HEAP_BLOCK_SIZE) == 0);
}

int create_heap(struct heap* heap, void *ptr, void *end, struct heap_table* table){
    int res = 0;

    if(!validate_alignment(ptr) || !validate_alignment(end)){
        res = -EINVARG;
        goto out;
    }

    //Set heap to 0 and then initialise it
    memset(heap, 0, sizeof(struct heap));
    heap->saddr = ptr;
    heap->table = table;

    // Check if the pointer start and end and the table are valid
    res = validate_heap_table(ptr, end, table);

    if(res < 0){
        goto out;
    }

    // Set table entries to EntryFree value
    size_t table_size = sizeof(HEAP_BLOCK_TABLE_ENTRY) * table->total;
    memset(table->entries, HEAP_BLOCK_TABLE_ENTRY_FREE, table_size);

out:
    return res;
}

// To find the aligned size for any value of bytes entered
// Ex 5000 bytes is not aligned. We need 2 blocks that is 8192 bytes to align it. The function will do this
static uint32_t align_value_to_upper(uint32_t val){
    if(val % HEAP_BLOCK_SIZE == 0){
        return val;
    }

    val = (val - (val % HEAP_BLOCK_SIZE));  // Will give the lower value
    val = val + HEAP_BLOCK_SIZE;            // Add one block to the lower value

    return val;
}

static int get_entry_type(HEAP_BLOCK_TABLE_ENTRY entry){
    return entry & 0x0f;                    // To return the last four bits
}

int get_start_block(struct heap * heap, uint32_t total_blocks){
    struct heap_table * table = heap->table;

    int current_block = 0;
    int start_block = -1;

    for(size_t i=0; i<table->total; i++){
        if(get_entry_type(table->entries[i]) != HEAP_BLOCK_TABLE_ENTRY_FREE){
            // Reset if the block is not free
            current_block = 0;
            start_block = -1;
            continue;
        }

        if(start_block == -1){
            start_block = i;
        }
        current_block++;
        if(current_block == total_blocks){
            break;
        }
    }

    if(start_block == -1){
        return -ENOMEM;
    }

    return start_block;
}

void * block_to_address(struct heap * heap, uint32_t start_block){
    return heap->saddr + (start_block * HEAP_BLOCK_SIZE);
}

void mark_blocks_taken(struct heap * heap, uint32_t start_block, uint32_t total_blocks){
    uint32_t end_block = start_block + total_blocks - 1;
    HEAP_BLOCK_TABLE_ENTRY entry = HEAP_BLOCK_TABLE_ENTRY_TAKEN | HEAP_BLOCK_IS_FIRST;

    if(total_blocks > 1){
        entry = entry | HEAP_BLOCK_HAS_N;
    }

    for(uint32_t i=start_block; i<=end_block; i++){
        heap->table->entries[i] = entry;
        entry = HEAP_BLOCK_TABLE_ENTRY_TAKEN;
        if(i != end_block - 1){
            entry = entry | HEAP_BLOCK_HAS_N;
        }
    }
}

void * malloc_blocks(struct heap * heap, uint32_t total_blocks){
    void * address = 0;

    int start_block = get_start_block(heap, total_blocks);

    if(start_block < 0){
        goto out;
    }

    address = block_to_address(heap, start_block);

    mark_blocks_taken(heap, start_block, total_blocks);

out:
    return address;
}

void * heap_malloc(struct heap* heap, size_t size){
    size_t aligned_size = align_value_to_upper(size);
    uint32_t total_blocks = aligned_size / HEAP_BLOCK_SIZE; 

    return malloc_blocks(heap, total_blocks);
}

void mark_blocks_free(struct heap * heap, uint32_t start_block){
    struct heap_table * table= heap->table;

    for(int i=start_block; i<(int)table->total; i++){
        HEAP_BLOCK_TABLE_ENTRY entry = table->entries[i];
        table->entries[i] = HEAP_BLOCK_TABLE_ENTRY_FREE;

        if(!(entry && HEAP_BLOCK_HAS_N)){
            break;
        }
    }
}

int address_to_block(struct heap* heap, void* address){
    return ((int)(address - heap->saddr)/HEAP_BLOCK_SIZE);
}

void heap_free(struct heap* heap, void * ptr){
    mark_blocks_free(heap, address_to_block(heap, ptr));
}