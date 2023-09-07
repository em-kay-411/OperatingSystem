#include "file.h"
#include "config.h"
#include "memory/memory.h"
#include "memory/heap/kheap.h"
#include "status.h"
#include "kernel.h"
#include "fat/fat16.h"

struct filesystem* filesystems[MAX_FILESYSTEMS];
struct file_descriptor* file_descriptors[MAX_FILE_DESCRIPTORS];

// Function to get free filesystem
static struct filesystem** get_free_filesystem(){
    int i = 0;
    for(i = 0; i < MAX_FILESYSTEMS; i++){
        if(filesystems[i] == 0){
            return &filesystems[i];
        }
    }

    return 0;
}

// Function to insert new filesystem
void fs_insert_filesystem(struct filesystem* filesystem){
    struct filesystem ** fs;
    fs = get_free_filesystem();

    if(!fs){
        print("Problem inserting filesystem.The system needs to panic here");
        while(1){}
    }

    *fs = filesystem;
}

// Function to load the statically implemented filesystems
static void fs_static_load(){
    fs_insert_filesystem(fat16_init());
}

void fs_load(){
    memset(filesystems, 0, sizeof(filesystems));
    fs_static_load();
}

void fs_init(){
    memset(file_descriptors, 0, sizeof(file_descriptors));
    fs_load();
}

static int new_file_descriptor(struct file_descriptor ** desc_out){
    int res = -ENOMEM;

    for(int i=0; i<MAX_FILE_DESCRIPTORS; i++){
        if(file_descriptors[i] == 0){
            struct file_descriptor * desc = kzalloc(sizeof(struct file_descriptor));
            desc->index = i+1;          //Index to be started with 1
            file_descriptors[i] = desc;
            *desc_out = desc;
            res = 0;
            break;
        }
    }

    return res;
}

static struct file_descriptor* get_file_descriptor(int fd){
    if(fd <= 0 || fd >= MAX_FILE_DESCRIPTORS){
        return 0;
    }

    int index = fd - 1;
    return file_descriptors[index];
}

struct filesystem* fs_resolve(struct disk * disk){
    struct filesystem * fs = 0;

    for(int i=0; i<MAX_FILESYSTEMS; i++){
        if(filesystems[i] != 0 && filesystems[i]->resolve(disk) == 0){
            fs = filesystems[i];
            break;
        }
    }

    return fs;
}

int fopen(const char * filename, const char * mode){
    return -EIO;
}
