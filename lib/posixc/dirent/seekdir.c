#include "../dirent.h"

#ifdef __cplusplus
extern "C" {
#endif

void seekdir(DIR* dirp, long int loc)
{
    dirp->i = loc;
};

#ifdef __cplusplus
}
#endif
