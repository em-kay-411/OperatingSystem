#ifndef PAGING_H
#define PAGING_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define CACHE_DISABLED  0b00010000
#define WRITE_THROUGH   0b00001000
#define ACCESS_FROM_ALL 0b00000100
#define IS_WRITABLE     0b00000010
#define IS_PRESENT      0b00000001

#define TOTAL_ENTRIES_PER_TABLE 1024
#define PAGE_SIZE 4096

struct paging_4gb_chunk{
    uint32_t* directory_entry;
};

void paging_switch(uint32_t * directory);
struct paging_4gb_chunk * paging_new_4gb(uint8_t flags);
void paging_switch(uint32_t * directory);
uint32_t * get_page_directory(struct paging_4gb_chunk * chunk);
int paging_set(uint32_t * directory, void * virtual, uint32_t val);
bool paging_is_aligned(void * addr);
void enable_paging();

#endif