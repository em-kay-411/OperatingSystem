#ifndef PARSER_H
#define PARSER_H

struct path_root{
    int drive_no;
    struct path_part * first;
};

struct path_part{
    const char * part;
    struct path_part * next;
};

void path_free(struct path_root * root);
struct path_root* parse_path(const char * path, const char * current_directory_path);

#endif