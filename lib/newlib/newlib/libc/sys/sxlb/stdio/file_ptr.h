#ifndef _STDIO_FILE_PTR_H
#define _STDIO_FILE_PTR_H

struct __file_ptr
{
    void* fs_node;
    size_t off;
};

#endif
