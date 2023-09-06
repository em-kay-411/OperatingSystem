#include "streamer.h"
#include "memory/heap/kheap.h"
#include "config.h"

struct disk_stream* new_stream(int disk_id){
    struct disk* disk = get_disk(disk_id);
    if(!disk){
        return 0;
    }

    struct disk_stream* streamer = kzalloc(sizeof(struct disk_stream));
    streamer->pos = 0;
    streamer->disk = disk;
    return streamer;
}

int diskstreamer_seek(struct disk_stream* streamer, int pos){
    streamer->pos = pos;
    return 0;
}

int diskstreamer_read(struct disk_stream * streamer, void * out, int total){
    // Find the sector and offset
    int sector = streamer->pos / SECTOR_SIZE;
    int offset = streamer->pos % SECTOR_SIZE;

    char buf[SECTOR_SIZE]; //Declaring the vuffer

    int res = disk_read_block(streamer->disk, sector, 1, buf);    // Read the block and put in buf
    if(res < 0){
        goto out;
    }

    int total_to_read = total > SECTOR_SIZE ? SECTOR_SIZE : total;  // Find total bytes to read
    for(int i=0; i<total_to_read; i++){                             // Put into out
        *(char*) out++ = buf[offset+i];
    }

    // Adjust the stream
    streamer->pos = streamer->pos + total_to_read;

    // Recursive call if total to read is greater than sector size
    if(total_to_read > SECTOR_SIZE){
        res = diskstreamer_read(streamer, out, total-SECTOR_SIZE);
    }

out:
    return res;
}

void diskstreamer_close(struct disk_stream* streamer){
    kfree(streamer);
}