#include "stdio.h"
#include "../../../src/system/syscall_defs.hpp"

#ifdef __cplusplus
extern "C" {
#endif

int puts(const char* str)
{
	SYSCALL1(SYSCNUM_PRINTL, str);
}

#ifdef __cplusplus
}
#endif
