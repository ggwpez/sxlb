#include "../stdlib.h"

#ifdef __cplusplus
extern "C" {
#endif

void abort()
{
	raise(SIGABRT);
}

#ifdef __cplusplus
}
#endif
