#pragma once

#include "../types.hpp"

namespace vfs
{
    #define VFS_NODE_NAME_L 255

    typedef struct fs_node fs_node_t;
    typedef struct dir_ent dir_ent_t;
    extern fs_node_t root_node;
    extern fs_node_t* root_nodes[];

    //define all delegates
    typedef uint32_t (*read_delegate_t)(fs_node_t*, uint32_t, uint32_t, LPTR);
    typedef uint32_t (*write_delegate_t)(fs_node_t*, uint32_t, uint32_t, LPTR);
    typedef void (*open_delegate_t)(fs_node_t*);
    typedef void (*close_delegate_t)(fs_node_t*);
    typedef dir_ent_t* (*read_dir_delegate_t)(fs_node_t*, uint32_t);
    typedef fs_node_t* (*find_dir_delegate_t)(fs_node_t*, char*);

    enum class node_type : uint32_t
    {
        Blk         = 1,
        Chr         = 2,
        Dir         = 3,
        Fifo        = 4,
        Lnk         = 5,
        Reg         = 6,    //regulare file
        Sock        = 7,
        Unknown     = 0,
        Mount_P     = 8,    //for | it
        Virtual     = 16
    };

    struct fs_node
    {
        char name[NAME_MAX];
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

        fs_node(char const* Name, node_type Type, uint32_t Length, uint32_t Reserved, uint32_t Inode, fs_node_t* Ptr,
                read_delegate_t Read,
                write_delegate_t Write,
                open_delegate_t Open,
                close_delegate_t Close,
                read_dir_delegate_t Read_dir,
                find_dir_delegate_t Find_dir);
        fs_node();
    };

    typedef struct dir_ent
    {
        uint32_t inode;
        node_type type;
        char name[VFS_NODE_NAME_L];
    } dir_ent_t;

    void init(fs_node_t* initrd);

    fs_node_t* get_root();
    fs_node_t* resolve_path(fs_node_t* node, char* path);
    LPTR render_path(fs_node_t* node, char* buffer, size_t size);
    uint32_t read(fs_node_t* node, uint32_t off, uint32_t size, LPTR buffer);
    uint32_t write(fs_node_t* node, uint32_t off, uint32_t size, LPTR buffer);
    void open(fs_node_t* node);
    void close(fs_node_t* node);
    dir_ent_t* read_dir(fs_node_t* node, uint32_t id);
    fs_node_t* find_dir(fs_node_t* node, char* name);
}
