#include "../dirent.h"

#ifdef __cplusplus
extern "C" {
#endif

void rewinddir(DIR* dirp)
{
    dirp->i = 0;
};

#ifdef __cplusplus
}
#endif
