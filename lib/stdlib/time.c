#include "time.h"
#include "../../src/system/syscall_defs.hpp"

clock_t clock()
{
	clock_t t;
	SYSCALL_RET0(SYSCNUM_TASK_GET_SPAWNE_TIME, t);
	return t;
}

double difftime(time_t t1, time_t t2)
{
	return t2 - t1;
}