#include "io/io.h"
#include "disk.h"
#include "memory/memory.h"
#include "config.h"
#include "status.h"

struct disk disk;

// Function to read from the primary hard disk
int disk_read_sector(int lba, int total, void * buf){

    // Sending bits to the port to instruct it to allow us to read from the disk
    outb(0x1F6, (lba > 24) | 0xE0);
    outb(0x1F2, total);
    outb(0x1F3, (unsigned char)(lba & 0xff));
    outb(0x1F4, (unsigned char)(lba >> 8));
    outb(0x1F4, (unsigned char)(lba >> 16));
    outb(0x1F7, 0x20);

    unsigned short * ptr = (unsigned short *) buf;
    for(int i=0; i<total; i++){
        
        // Waiting for the buffer to be ready
        char c = insb(0x1F7);
        while(!(c & 0x08)){
            c = insb(0x1F7);
        }

        // Copy from hard disk to buffer memory;
        for(int j=0; j<256; j++){
            *ptr = insw(0x1F0);         // To read two bytes
            ptr++;
        }
    }
    return 0;
}

void disk_search_and_init(){
    memset(&disk, 0, sizeof(disk));
    disk.type = DISK_TYPE_REAL;
    disk.sector_size = SECTOR_SIZE;
}

// Only for disk 0, which is the primary disk
struct disk * get_disk(int index){
    if(index != 0){
        return 0;
    }

    return &disk;
}

int disk_read_block(struct disk * idisk, unsigned int lba, int total, void * buf){
    if(idisk != &disk){
        return -EIO;
    }

    return disk_read_sector(lba, total, buf);
}