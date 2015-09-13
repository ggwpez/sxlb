#include "time.h"
#include "../../../src/system/syscall_defs.hpp"

#ifdef __cplusplus
extern "C" {
#endif

clock_t clock()
{
	clock_t t;
    SYSCALL_RET0(SYSCNUM_TASK_GET_SPAWN_TIME, t);
	return t;
}

double difftime(time_t t1, time_t t2)
{
	return t2 - t1;
}

#ifdef __cplusplus
}
#endif
