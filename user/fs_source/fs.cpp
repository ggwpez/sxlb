#include "fs.h"

void fs_t::init()
{
    unsigned int base = (unsigned int)this + sizeof(fs_t);
    unsigned int c = this->files_c;

    while (c--)
    {
        fs_node_t* node = (fs_node_t*)base;

        node->data = base + sizeof(fs_node_t);
        base += node->data_length + sizeof(fs_node_t);
    }
}

void read(fs_node_t* node, char* buffer)
{
    memcpy(buffer, node->data, node->data_length);
}
