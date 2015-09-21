#include "../dirent.h"
#include "../../../src/system/syscall_defs.hpp"

#ifdef __cplusplus
extern "C" {
#endif

int chdir(const char* path)
{
    DIR* new_dir = opendir(path);
    if (!new_dir) return -1;                  //TODO return error in errno

    int ret;
    SYSCALL_RET1(SYSCNUM_TASK_SET_WORKING_DIR, ret, path);
    return ret;
}

#ifdef __cplusplus
}
#endif
