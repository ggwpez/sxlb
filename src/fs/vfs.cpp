#include "vfs.hpp"
#include "initrd.hpp"
#include "../memory/memory.hpp"
#include "../string.hpp"
#include "../ui/textmode.hpp"
#include "../task/task.hpp"

#define _VFS_ROOT_ENTRIES_C 2
namespace vfs
{
    fs_node_t* root_nodes[_VFS_ROOT_ENTRIES_C];       //i dont like malloc, you know
    fs_node_t root_node;

    dir_ent_t tmp_dir_ent;
    fs_node_t tmp_node;

    void init(fs_node_t* initrd)
    {
        logtINF("installing vfs...(root items_c: %u)", _VFS_ROOT_ENTRIES_C);
        root_node = fs_node("/", node_type::Dir, _VFS_ROOT_ENTRIES_C, 0, 0, &root_node, 0, 0, 0, 0, &read_dir, &find_dir);
        root_nodes[0] = &root_node;
        root_nodes[1] = initrd;
        initrd->ptr = &root_node;       //set parent pointer
        logDONE;
    }

    fs_node_t* resolve_path(fs_node_t* node, char* path)
    {
#ifdef __CHECKS_NLPTR
        if (!path || (!path && !node))
            return nullptr;
#endif
        fs_node_t* found;
        char buffer[NAME_MAX];

        uint32_t i = 0, j = 0;
        if (path[i] == '/')
        {
            found = &root_node;
            i++;
        }
        else if (path[i] == '.' && path[i+1] == '/')
        {
            found = node;
            i += 2;
        }
        else
            found = node;

        while (1)
        {
            while (path[i] && path[i] != '/')
               buffer[j++] = path[i++];
            buffer[j] = 0;

            if (j)
            {
                found = find_dir(found, buffer);
                if (!found)
                    break;
            }

            if (!path[i] || !path[++i])
                break;
            j = 0;
        }

        return found;
    }

    LPTR render_path(fs_node_t* node, char* buffer, size_t size)
    {
        uint32_t i = 0;
        if (!buffer)
        {
            ERRNO(EFAULT);
            return nullptr;
        }
        else if (!size)     //yep that must be else, see posix
        {
            ERRNO(EINVAL);
            return nullptr;
        }

        while (1)
        {
            if (i >= size)
            {
                ERRNO(ERAGNE);
                buffer = nullptr;
                break;
            }

            uint32_t l = strlen(node->name);
            if (l + i +1 >= size)
            {
                ERRNO(ERANGE);
                buffer = nullptr;
                break;
            }
            else
            {
                strcpy(buffer +i, node->name);
                i += l;
            }

            if (node == &root_node)
                break;

            node = node->ptr;
        }

        return buffer;
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

    //returns the entry with the index id
    dir_ent_t* read_dir(fs_node_t* node, uint32_t id)
    {
        if (!node || !((char)node->type & (char)node_type::Dir) || !node->read_dir)
            return nullptr;

        if (id == node->length)        // return link to parent directory
        {
            memory::memset(&tmp_dir_ent, 0, sizeof(dir_ent_t));
            strcpy(tmp_dir_ent.name, "..");
            tmp_dir_ent.inode = node->ptr->inode;
            tmp_dir_ent.type = node_type::Lnk;//(node_type)((uint32_t)node_type::Lnk | (uint32_t)node_type::Virtual);

            return &tmp_dir_ent;
        }

        if (node == &root_node)
        {
            if (id >= root_node.length)
                return nullptr;

            strcpy(tmp_dir_ent.name, root_nodes[id]->name);
            tmp_dir_ent.inode = root_nodes[id]->inode;
            tmp_dir_ent.type = root_nodes[id]->type;

            return &tmp_dir_ent;
        }

        return node->read_dir(node, id);
    }

    fs_node_t* find_dir(fs_node_t* node, char* name)
    {
        if (!node || !((char)node->type & (char)node_type::Dir) || !node->find_dir)
            return nullptr;

        if (!strcmp(name, ".."))        // return link to parent directory
        {
            memory::memset(&tmp_node, 0, sizeof(fs_node_t));
            tmp_node.type = node_type::Lnk;//(node_type)((uint32_t)node_type::Lnk | (uint32_t)node_type::Virtual);
            strcmp(tmp_node.name, "..");
            tmp_node.ptr = node->ptr;

            return &tmp_node;
        }

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
