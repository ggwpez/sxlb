#pragma once

#include "../types.hpp"

namespace vfs
{
    #define VFS_NODE_NAME_L 64

    typedef struct fs_node fs_node_t;
    typedef struct dir_ent dir_ent_t;

    //define all delegates
    typedef uint32_t (*read_delegate_t)(fs_node_t*, uint32_t, uint32_t, LPTR);
    typedef uint32_t (*write_delegate_t)(fs_node_t*, uint32_t, uint32_t, LPTR);
    typedef void (*open_delegate_t)(fs_node_t*);
    typedef void (*close_delegate_t)(fs_node_t*);
    typedef dir_ent_t* (*read_dir_delegate_t)(fs_node_t*, uint32_t);
    typedef fs_node_t* (*find_dir_delegate_t)(fs_node_t*, char*);

    enum class node_type : char
    {
        File        = 1,
        Dir         = 2,
        Char_Dev    = 3,
        Block_Dev   = 4,
        Pipe        = 5,
        Symlink     = 6,
        Mount_P     = 8     //for | it
    };

    struct fs_node
    {
        char name[VFS_NODE_NAME_L];
        node_type type;
        uint32_t length;
        uint32_t reserved;
        uint32_t inode;
        fs_node_t* ptr;

        read_delegate_t read;
        write_delegate_t write;
        open_delegate_t open;
        close_delegate_t close;
        read_dir_delegate_t read_dir;
        find_dir_delegate_t find_dir;
    };

    typedef struct dir_ent
    {
        char name[VFS_NODE_NAME_L];
        uint32_t inode;
        node_type type;
    } dir_ent_t;

    uint32_t read(fs_node_t* node, uint32_t off, uint32_t size, LPTR buffer);
    uint32_t write(fs_node_t* node, uint32_t off, uint32_t size, LPTR buffer);
    void open(fs_node_t* node);
    void close(fs_node_t* node);
    dir_ent_t* read_dir(fs_node_t* node, uint32_t id);
    fs_node_t* find_dir(fs_node_t* node, char* name);
}
