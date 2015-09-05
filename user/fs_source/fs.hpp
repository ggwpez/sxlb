#pragma once
#include <string.h>

#define FS_MAGIC 0xdeadbeef

enum fs_type
{
    FS_DIR,
    FS_FILE
};

struct fs_t
{
    unsigned int files_c;

    void init();
};

struct fs_node_t
{
    unsigned int magic;
    char name[64];
    unsigned int data_length;
    unsigned int data;   //after init() its absolute
};
