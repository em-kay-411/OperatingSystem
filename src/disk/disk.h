#ifndef DISK_H
#define DISK_H

typedef unsigned int DISK_TYPE;
#define DISK_TYPE_REAL 0        // Represents the real physical hard disk
struct disk{
    DISK_TYPE type;
    int sector_size;
};

int disk_read_sector(int lba, int total, void *buf);
void disk_search_and_init();
struct disk * get_disk(int index);
int disk_read_block(struct disk * idisk, unsigned int lba, int total, void * buf);

# endif