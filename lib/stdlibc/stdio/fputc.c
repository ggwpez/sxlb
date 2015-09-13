#include "stdio.h"

#ifdef __cplusplus
extern "C" {
#endif

int fputc(int v, FILE* fp)
{
    if (--fp->left >= 0)
        return *fp->ptr++ = v;

    return _flushbuff(fp, v);
}

#ifdef __cplusplus
}
#endif
