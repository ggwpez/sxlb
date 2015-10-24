#include "../unistd.h"
#include "../dirent.h"
#include "../../../src/system/syscall_defs.hpp"

#ifdef __cplusplus
extern "C" {
#endif

char* getcwd(char* buf, uint32_t size)
{
    LPTR dir = 0;
    SYSCALL_RET0(SYSCNUM_TASK_GET_WORKING_DIR, dir);

    uint32_t ret;
    SYSCALL_RET3(SYSCNUM_VFS_RENDER_PATH, ret, dir, buf, size);
    return ret;
}

#ifdef __cplusplus
}
#endif
