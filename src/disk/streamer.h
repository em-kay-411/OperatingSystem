#ifndef DISKSTREAMER_H
#define DISKSTREAMER_H

#include "disk.h"

struct disk_stream{
    int pos;
    struct disk* disk;
};

struct disk_stream* new_stream(int disk_id);
int diskstreamer_seek(struct disk_stream* streamer, int pos);
int diskstreamer_read(struct disk_stream * streamer, void * out, int total);
void diskstreamer_close(struct disk_stream* streamer);

#endif