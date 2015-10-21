#include "../dirent.h"
#include "string.h"
#include "../../../src/system/syscall_defs.hpp"

#ifdef __cplusplus
extern "C" {
#endif

DIR tmp_dir;

DIR* opendir(const char* name)
{
    LPTR start_dir = 0;

    SYSCALL_RET0(SYSCNUM_TASK_GET_WORKING_DIR, start_dir);

    if (!strcmp(name, "."))
    {
        tmp_dir.fs_node = start_dir;
        tmp_dir.i = 0;
        return &tmp_dir;
    }

    SYSCALL_RET2(SYSCNUM_VFS_RESOLVE_PATH, tmp_dir.fs_node, start_dir, name);
    if (*(uint32_t*)(tmp_dir.fs_node+NAME_MAX+1) != 3)  //its not a direcotry
        return 0;

    tmp_dir.i = 0;
    return &tmp_dir;

    /*LPTR dir;
    char buffer[NAME_MAX];
    uint32_t buffer_i;

    uint32_t name_l = strlen(name);
    if (!strcmp(name, "."))
    {
        SYSCALL_RET0(SYSCNUM_TASK_GET_WORKING_DIR, tmp_dir.fs_node);

        tmp_dir.i = 0;
        return &tmp_dir;
    }
    if (name[0] != '/')
        return 0;

    SYSCALL_RET0(SYSCNUM_VFS_GET_ROOT, dir);
    if (!strcmp(name, "/"))
    {
        tmp_dir.fs_node = dir;
        tmp_dir.i = 0;
        return &tmp_dir;
    }

    buffer_i = 1;
    while (1)
    {
        if (buffer_i >= name_l -1)
            break;

        while (name[buffer_i] != '/')
            buffer[buffer_i-1] = name[buffer_i++];
        buffer[buffer_i-1] = 0;
        buffer_i++;
        if (!strcmp(buffer, "."))
            continue;

        SYSCALL_RET2(SYSCNUM_VFS_FIND_DIR, dir, dir, buffer);
        if (!dir)
            return NULL;
    }

    tmp_dir.fs_node = dir;
    tmp_dir.i = 0;
    return &tmp_dir;*/
}

#ifdef __cplusplus
}
#endif
