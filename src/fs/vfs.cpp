#include "vfs.hpp"
#include "initrd.hpp"
#include "../memory/memory.hpp"
#include "../string.hpp"
#include "../ui/textmode.hpp"

namespace vfs
{
    fs_node_t* root_nodes[8];       //i dont like malloc, you know
    fs_node_t root_node;

    dir_ent_t tmp_dir_ent;

    void init(fs_node_t* initrd)
    {
        root_node = fs_node("/", node_type::Dir, 0, 0, 0, 0, 0, 0, 0, 0, &read_dir, &find_dir);
        root_nodes[0] = &root_node;
        root_nodes[1] = initrd;
        root_node.length = 2;
    }

    uint32_t read(fs_node_t* node, uint32_t off, uint32_t size, LPTR buffer)
    {
        return (node->read) ? node->read(node, off, size, buffer) : 0;
    }

    uint32_t write(fs_node_t* node, uint32_t off, uint32_t size, LPTR buffer)
    {
        return (node->write) ? node->write(node, off, size, buffer) : 0;
    }

    void open(fs_node_t* node)
    {
        if (node->open)
            node->open(node);
    }

    void close(fs_node_t* node)
    {
        if (node->close)
            node->close(node);
    }

    fs_node_t* get_root()
    {
        return &root_node;
    }

    dir_ent_t* read_dir(fs_node_t* node, uint32_t id)
    {
        if (node == &root_node)
        {
            if (id >= root_node.length)
                return nullptr;

            strcpy(tmp_dir_ent.name, root_nodes[id]->name);
            tmp_dir_ent.inode = root_nodes[id]->inode;
            tmp_dir_ent.type = root_nodes[id]->type;
            return &tmp_dir_ent;
        }

        if ((!((char)node->type & (char)node_type::Dir)) || !node->read_dir)
            return nullptr;

        return node->read_dir(node, id);
    }

    fs_node_t* find_dir(fs_node_t* node, char* name)
    {
        if (node == &root_node)    //shorten this
        {
            if (!strcmp(name, root_node.name))
                return &root_node;

            for (int i = 0; i < root_node.length; ++i)
            {
                if (!strcmp(name, root_nodes[i]->name))
                    return root_nodes[i];
            }
        }

        if (!((char)node->type & (char)node_type::Dir) || !node->find_dir)
            return nullptr;

        return node->find_dir(node, name);
    }

    fs_node::fs_node()
    {
        memory::memset(this, 0, sizeof(fs_node_t));
    }

    fs_node::fs_node(char const* Name, node_type Type, uint32_t Length, uint32_t Reserved, uint32_t Inode, fs_node_t* Ptr,
                     read_delegate_t Read,
                     write_delegate_t Write,
                     open_delegate_t Open,
                     close_delegate_t Close,
                     read_dir_delegate_t Read_dir,
                     find_dir_delegate_t Find_dir)
    {
        strcpy(name, Name);
        type = Type;
        length = Length;
        reserved = reserved;
        inode = Inode;
        ptr = Ptr;
        read = Read;
        write = Write;
        open = Open;
        close = Close;
        read_dir = Read_dir;
        find_dir = Find_dir;
    }
}
