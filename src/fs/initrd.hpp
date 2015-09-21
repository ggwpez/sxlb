#pragma once
#include "../types.hpp"
#include "../memory/memory.hpp"
#include "../string.hpp"
#include "vfs.hpp"

#define FS_MAGIC 0xdeadbeef

namespace initrd
{
    enum class node_type : char
    {
        FS_DIR,
        FS_FILE
    };

    typedef struct fs_node
    {
        unsigned int magic;
        char name[64];
        unsigned int data_length;
        unsigned int data;   //after init() its absolute
        unsigned int inode;
    } fs_node_t;

    struct fs_t
    {
        uint32_t files_c;
        fs_node_t* files;
    };

    vfs::fs_node_t *fs_install(LPTR address);
    uint32_t read(vfs::fs_node_t* node, uint32_t off, uint32_t size, LPTR buffer);
    void open(fs_node_t* node);
    void close(fs_node_t* node);
    vfs::fs_node_t* find_dir(vfs::fs_node_t* node, char* name);
    vfs::dir_ent_t* read_dir(vfs::fs_node_t* node, uint32_t i);
}
