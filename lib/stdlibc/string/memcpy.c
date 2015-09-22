#include "../string.h"

#ifdef __cplusplus
extern "C" {
#endif

void* memcpy(void* dest, void const* src, size_t length)
{
	while (length--)
		*(char*)dest++ = *(char*)src++;

	return dest;
}

#ifdef __cplusplus
}
#endif