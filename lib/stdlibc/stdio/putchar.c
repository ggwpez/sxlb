#include "stdio.h"
#include "../../../src/system/syscall_defs.hpp"

#ifdef __cplusplus
extern "C" {
#endif

int putchar(int c)
{
    SYSCALL1(SYSCNUM_PUTC, c & 0xff);
}

#ifdef __cplusplus
}
#endif
