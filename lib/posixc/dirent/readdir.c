#include "../dirent.h"
#include "../../../src/system/syscall_defs.hpp"

#ifdef __cplusplus
extern "C" {
#endif

struct dirent* readdir(DIR* dirp)
{
    if (!dirp->fs_node || dirp->i == -1)
        return 0;

    dirent_t* ent;
    SYSCALL_RET2(SYSCNUM_VFS_READ_DIR, ent, dirp->fs_node, dirp->i++);

    return ent;
}

#ifdef __cplusplus
}
#endif
