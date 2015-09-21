#include "../dirent.h"
#include "../../../src/system/syscall_defs.hpp"
#include "../../stdlibc/stdio.h"

#ifdef __cplusplus
extern "C" {
#endif

int chdir(const char* path)
{
    DIR* new_dir = opendir(path);
    printf("got open with: %s return: %u\n", path, new_dir);
    if (!new_dir)
        return -1;                  //TODO return error in errno

    int ret;
    SYSCALL_RET1(SYSCNUM_TASK_SET_WORKING_DIR, ret, new_dir->fs_node);
    return ret;
}

#ifdef __cplusplus
}
#endif
