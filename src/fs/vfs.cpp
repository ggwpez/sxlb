#include "vfs.hpp"
#include "initrd.hpp"
#include "../memory/memory.hpp"
#include "../string.hpp"
#include "../ui/textmode.hpp"

namespace vfs
{
    fs_node_t dir_initrd;

    fs_node_t* initrd_nodes = nullptr;
    uint32_t initrd_nodes_c = 0;

    dir_ent_t tmp_dir_ent;

    void init(initrd::fs_t* initrd)
    {

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

    dir_ent_t* read_dir(fs_node_t* node, uint32_t id)
    {
        if ((!((char)node->type & (char)node_type::Dir)) || !node->read_dir)
            return nullptr;

        return node->read_dir(node, id);
    }

    fs_node_t* find_dir(fs_node_t* node, char* name)
    {
        if (!((char)node->type & (char)node_type::Dir) || !node->find_dir)
            return nullptr;

        return node->find_dir(node, name);
    }
}
