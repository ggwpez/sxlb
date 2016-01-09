#include "../dirent.h"

#ifdef __cplusplus
extern "C" {
#endif

int closedir(DIR* dirp)
{
    if (!dirp)              //invalid
        return -1;

    if (!dirp->fs_node)     //already closed
        return 0;

    SYSCALL1(SYSCNUM_VFS_CLOSE, dirp->fs_node);

    dirp->fs_node = 0;
    return 0;
}

#ifdef __cplusplus
}
#endif
