#include "../stdlib/stdlib.h"
#include "../../../src/system/syscall_defs.hpp"

#ifdef __cplusplus
extern "C" {
#endif

#define _DTORS_C 64

void (*dtors[_DTORS_C])() = { 0 };
int i = 0;

int atexit(void (*func)(void))
{
    if (i >= _DTORS_C)
		return -1;

	dtors[i++] = func;
	return 0;
}

void exit(int status)
{
    while (i--)
        dtors[i]();

    SYSCALL1(SYSCNUM_TASK_EXIT, status);
}

#ifdef __cplusplus
}
#endif
