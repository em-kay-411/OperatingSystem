#include "parser.h"
#include "string/string.h"
#include "kernel.h"
#include "memory/heap/kheap.h"
#include "memory/memory.h"
#include "status.h"

static int path_valid_format(const char * filename){
    int len = strnlen(filename, MAX_PATH);
    return (len >= 3 && isDigit(filename[0]) && memcmp((void *) & filename[1], ":/", 2) == 0);
}

static int get_drive_by_path(const char ** path){
    if(!path_valid_format(*path)){
        return -EBADPATH;
    }

    int drive_no = to_digit(*path[0]);
    *path += 3;                         // Repositions the path variable 3 bytes ahead
    return drive_no;
}

static struct path_root* create_root(int drive_number){
    struct path_root * pathr = kzalloc(sizeof(struct path_root));
    pathr->drive_no = drive_number;
    pathr->first = 0;

    return pathr;
}

// To get path from current index to the next '/' or end
static const char * get_path_part(const char ** path){
    char * res = kzalloc(MAX_PATH);
    int i = 0;

    while(**path != '/' && **path != 0x00){
        res[i] = **path;
        *path += 1;
        i++;
    }

    if(**path == '/'){
        *path += 1;
    }

    if(i == 0){
        kfree(res);
        res = 0;
    }

    return res;
}

struct path_part* parse_path_part(struct path_part * last, const char ** path){

    const char * part_str = get_path_part(path);
    if(!part_str){
        return 0;
    }

    struct path_part * curr = kzalloc(sizeof(struct path_part));
    curr->part = part_str;
    curr->next = 0x00;

    if(last){
        last->next = curr;
    }

    return curr;
}

void path_free(struct path_root * root){
    struct path_part* curr = root->first;

    while(curr){
        struct path_part * next_part = curr->next;
        kfree((void*) curr->part);
        kfree(curr);
        curr = next_part;
    }

    kfree(root);
}

struct path_root* parse_path(const char * path, const char * current_directory_path){
    int res = 0;
    const char * temp = path;
    struct path_root * root = 0;

    if(strlen(path) > MAX_PATH){
        goto out;
    }

    // Get the drive
    res = get_drive_by_path(&temp);
    if(res < 0){
        goto out;
    }

    // Create root
    root = create_root(res);
    if(!root){
        goto out;
    }

    // Enter the first of the root
    struct path_part* first = parse_path_part(NULL, &temp);
    if(!first){
        goto out;
    }

    root->first = first;

    // Enter the next parse
    struct path_part* part = parse_path_part(first, &temp);
    // Keep parsing till the end
    while(part){
        part = parse_path_part(part, &temp);
    }

out:
    return root;
}