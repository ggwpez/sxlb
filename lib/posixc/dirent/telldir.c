#include "../dirent.h"

#ifdef __cplusplus
extern "C" {
#endif

long int telldir(DIR* dirp)
{
    return dirp->i;
};

#ifdef __cplusplus
}
#endif
