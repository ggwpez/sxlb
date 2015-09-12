#include "stdlib.h"
#include "../../src/system/syscall_defs.hpp"

#define _DTOR_C 64

void (*dtors[_DTOR_C])() = { 0 };
int i = 0;

int atexit (void (*func)(void))
{
	if (i >= _DTOR_C)
		return -1;

	dtors[i++] = func;
	return 0;
}

void exit (int status)
{
	i--;
	while (i >= 0)
		dtors[i--]();

	SYSCALL0(SYSCNUM_TASK_EXIT);
}