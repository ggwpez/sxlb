#pragma once
#include "../types.hpp"
#include "../memory/memory.hpp"
#include "../string.hpp"

#define FS_MAGIC 0xdeadbeef

enum class fs_type
{
    FS_DIR,
    FS_FILE
};

struct fs_node_t
{
    unsigned int magic;
    char name[64];
    unsigned int data_length;
    unsigned int data;   //after init() its absolute

    char* read(char* buffer);
};

struct fs_t
{
    uint32_t files_c;
    fs_node_t* files;

    fs_node_t* find_file(char* name);
};

fs_t* fs_install(LPTR address);
