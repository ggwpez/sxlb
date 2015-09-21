#include "../dirent.h"

#ifdef __cplusplus
extern "C" {
#endif

int closedir(DIR* dirp)
{
    dirp->fs_node = 0;
    dirp->i = -1;
}

#ifdef __cplusplus
}
#endif
