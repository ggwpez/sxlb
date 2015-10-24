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
    tmp_dir.i = 0;
    uint32_t t = *(uint32_t*)(tmp_dir.fs_node+NAME_MAX+1);

    if (t == 3)             //its a direcotry
        return &tmp_dir;
    else if (t == 5)        //its a link
    {
        uint32_t new_node = *(uint32_t*)(tmp_dir.fs_node+NAME_MAX+1+16);
        tmp_dir.fs_node = new_node;
        return &tmp_dir;
    }
    return NULL;
}

#ifdef __cplusplus
}
#endif
