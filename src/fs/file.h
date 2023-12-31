#ifndef FILE_H
#define FILE_H

#include "parser.h"
#include <stdint.h>

typedef unsigned int FILE_SEEK_MODE;
enum{
    SEEK_SET,
    SEEK_CUR,
    SEEK_END
};

typedef unsigned int FILE_MODE;
enum{
    FILE_MODE_READ,
    FILE_MODE_WRITE,
    FILE_MODE_APPEND,
    FILE_MODE_INVALID
};

struct disk;
typedef void*(*FS_OPEN_FUNCTION) (struct disk* disk, struct path_part* path, FILE_MODE mode);
typedef int (*FS_RESOLVE_FUNCTION) (struct disk* disk);             // Will return 0 if the filesystem with which a disk is being resolved has the same filesystem

// Structure defining different filesystems. Each filesystem will have a resolve function of itself and an open function
// These filesystems can be introduced by the user or developer
struct filesystem{
    // If the provided disk is using its filesystem, it must resolve 0
    FS_RESOLVE_FUNCTION resolve;
    FS_OPEN_FUNCTION open;

    char name[20];
};

struct file_descriptor{
    int index;
    struct filesystem * filesystem;
    void * private;
    struct disk* disk;
};

void fs_init();
int fopen(const char * filename, const char * mode_str);
void fs_insert_filesystem(struct filesystem* filesystem);
struct filesystem * fs_resolve(struct disk * disk);

#endif