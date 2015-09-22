#include "../string.h"

#ifdef __cplusplus
extern "C" {
#endif

int memcmp(void const* mem1, void const* mem2, size_t length)
{
	while (length--)
		if (*(char*)mem1++ != *(char*)mem2++)
			return 1;

	return 0;
}

#ifdef __cplusplus
}
#endif