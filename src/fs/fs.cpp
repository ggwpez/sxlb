#include "fs.hpp"

fs_t* fs_install(LPTR address)
{
    uint32_t headers_c = *(uint32_t*)address;
    for (int i = 0; i < headers_c; ++i)
    {
        fs_node_t* node = address + 4 + i*sizeof(fs_node_t);
        node->data += address;
    }
    fs_t* fs = (fs_t*)address;
    fs->files = address + 4;

    return fs;
}

char* fs_node_t::read(char* buffer)
{
    memory::memcpy(buffer, this->data, this->data_length);
    return buffer;
}

fs_node_t* fs_t::find_file(char* name)
{
    fs_node_t* files = (uint32_t)this +4;

    for (int i = 0; i < this->files_c; ++i)
        if (!strcmp(files[i].name, name))
            return &files[i];

    return nullptr;
}
