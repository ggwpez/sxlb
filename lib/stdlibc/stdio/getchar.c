#include "stdio.h"
#include "../../../src/system/syscall_defs.hpp"

#ifdef __cplusplus
extern "C" {
#endif

int getchar()
{
    int c = 0;
    while (!c)
        SYSCALL_RET0(SYSCNUM_GETC, c);

    return c & 0xff;
}

#ifdef __cplusplus
}
#endif
