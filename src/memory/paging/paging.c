#include "paging.h"
#include "memory/heap/kheap.h"
#include "status.h"

static uint32_t * current_directory = 0;
void load_directory(uint32_t * directory);
struct paging_4gb_chunk * paging_new_4gb(uint8_t flags){

    // Creaeting the page table directory
    uint32_t * directory = kzalloc(sizeof(uint32_t) * TOTAL_ENTRIES_PER_TABLE); // Directory containing 1024 entries for 1024 page tables
    int offset = 0;
    for(int i=0; i<TOTAL_ENTRIES_PER_TABLE; i++){
        uint32_t * entry  = kzalloc(sizeof(uint32_t) * TOTAL_ENTRIES_PER_TABLE); // Page Table initialisation as an entry to directory

        // Loop to map every entry to the address in the physical memory
        for(int j=0; j<TOTAL_ENTRIES_PER_TABLE; j++){
            entry[j] = (offset + j*(PAGE_SIZE)) | flags;                            // To map virtual address to the physical address x:x
        }

        // Increment offset to the new address
        offset = offset + (TOTAL_ENTRIES_PER_TABLE * PAGE_SIZE);
        directory[i] = (uint32_t)entry | flags | IS_WRITABLE;
    }

    //Pointing the 4gb paging chunk to the directory and returning it
    struct paging_4gb_chunk * chunk = kzalloc(sizeof(struct paging_4gb_chunk));
    chunk->directory_entry = directory;
    return chunk;
}

void paging_switch(uint32_t * directory){
    load_directory(directory);
    current_directory = directory;
}

uint32_t * get_page_directory(struct paging_4gb_chunk * chunk){
    return chunk->directory_entry;
}

bool paging_is_aligned(void * addr){
    return ((uint32_t)(addr) % PAGE_SIZE) == 0;
}

// Function to get indexes
int get_indexes(void* virtual_address, uint32_t * directory_index_out, uint32_t* table_index_out){
    int res = 0;
    if(!paging_is_aligned(virtual_address)){
        res = -EINVARG;
        goto out;
    }   

    *directory_index_out = ((uint32_t)virtual_address / (TOTAL_ENTRIES_PER_TABLE * PAGE_SIZE));     // Directory index for the virtual address
    *table_index_out = ((uint32_t)virtual_address % (TOTAL_ENTRIES_PER_TABLE * PAGE_SIZE) / PAGE_SIZE);         // Table index for the virtual address

out:
    return res;
}

// Function to set a virtual address to a physical address
int paging_set(uint32_t * directory, void * virtual, uint32_t val){
    if(!paging_is_aligned(virtual)){
        return -EINVARG;
    }

    uint32_t directory_index = 0;
    uint32_t table_index = 0;

    int res = get_indexes(virtual, &directory_index, &table_index);

    if(res < 0){
        return res;
    }

    uint32_t entry = directory[directory_index];
    uint32_t* table = (uint32_t*)(entry & 0xfffff000);
    table[table_index] = val;

    return 0;
}