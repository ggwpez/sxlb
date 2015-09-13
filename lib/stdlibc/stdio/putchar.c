#include "stdio.h"
#include "../../../src/system/syscall_defs.hpp"

#ifdef __cplusplus
extern "C" {
#endif

int putchar(int c)
{
	SYSCALL1(SYSCNUM_PUTC, (char)c);
}

#ifdef __cplusplus
}
#endif
