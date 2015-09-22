#include "../string.h"

#ifdef __cplusplus
extern "C" {
#endif

void* memset(void* dest, int value, size_t length)
{
	while (length--)
		*(int*)dest++ = value;

	return dest;
}

#ifdef __cplusplus
}
#endif