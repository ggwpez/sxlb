#include "stdio.h"

#ifdef __cplusplus
extern "C" {
#endif

int fgetc(FILE* fp)
{
    if (--fp->left >= 0)
        return *fp->ptr++;

    return _fillbuff(fp);
}

#ifdef __cplusplus
}
#endif
