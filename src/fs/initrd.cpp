#include "initrd.hpp"

namespace initrd
{
    vfs::fs_node_t root_node;
    fs_t* fs;
    vfs::dir_ent_t tmp_dir_ent;
    vfs::fs_node_t tmp_node_t;

    vfs::fs_node_t* fs_install(LPTR address)
    {
        uint32_t headers_c = *(uint32_t*)address;
        logINF("installing initrd...(@0x%x, in: '/initrd/')", address);
        for (int i = 0; i < headers_c; ++i)
        {
            fs_node_t* node = address + 4 + i*sizeof(fs_node_t);
            node->inode = i;
            node->data += address;
        }
        fs = (fs_t*)address;
        fs->files = address + 4;

        root_node.length = fs->files_c+1;   //[0] is the directory name
        strcpy(root_node.name, "initrd");
        root_node.read_dir = &read_dir;
        root_node.find_dir = &find_dir;
        root_node.read = 0;
        root_node.ptr = nullptr;
        root_node.type = vfs::node_type::Dir;

        tmp_node_t.read = &read;
        tmp_node_t.read_dir = nullptr;
        tmp_node_t.find_dir = nullptr;
        logDONE;

        return &root_node;
    }

    uint32_t read(vfs::fs_node_t* node, uint32_t off, uint32_t size, LPTR buffer)
    {
        fs_node_t header = fs->files[node->inode];
        if (off >= header.data_length)
            return 0;

        size_t to_read = header.data_length -off;
        if (to_read > size)
            to_read = size;

        memory::memcpy(buffer, header.data+off, to_read);
        /*logINF("read: %i", size);*/
        return size;
    }

    void open(fs_node_t* node)
    {

    }

    void close(fs_node_t* node)
    {

    }

    vfs::fs_node_t* find_dir(vfs::fs_node_t* node, char* name)
    {
        if (node != &root_node)
            return nullptr;

        if (!strcmp(name, root_node.name))
        {
            tmp_node_t.type = vfs::node_type::Dir;
            tmp_node_t.length = fs->files_c;
            tmp_node_t.inode = &root_node;
            strcpy(tmp_node_t.name, root_node.name);
            return &tmp_node_t;
        }

        for (int i = 0; i < fs->files_c; ++i)
        {
            if (!strcmp(name, fs->files[i].name))
            {
                tmp_node_t.type = vfs::node_type::Reg;
                tmp_node_t.length = fs->files[i].data_length;
                tmp_node_t.inode = fs->files[i].inode;
                strcpy(tmp_node_t.name, fs->files[i].name);
                return &tmp_node_t;
            }
        }

        return nullptr;
    }

    vfs::dir_ent_t* read_dir(vfs::fs_node_t* node, uint32_t i)
    {
        if (node != &root_node)
            return nullptr;

        if (i == 0)
        {
            strcpy(tmp_dir_ent.name, root_node.name);
            tmp_dir_ent.inode = &root_node;
            tmp_dir_ent.type = vfs::node_type::Dir;
            return &tmp_dir_ent;
        }
        if (i-1 >= fs->files_c)
            return nullptr;

        i--;
        strcpy(tmp_dir_ent.name, fs->files[i].name);
        tmp_dir_ent.inode = fs->files[i].inode;
        tmp_dir_ent.type = vfs::node_type::Reg;

        return &tmp_dir_ent;
    }
}
