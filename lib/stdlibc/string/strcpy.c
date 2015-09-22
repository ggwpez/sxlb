#include "../string.h"

#ifdef __cplusplus
extern "C" {
#endif

char* strcpy(char* dest, char const* src)
{
	char* ret = dest;
	while (*dest++ = *src++);

	return ret;
}

#ifdef __cplusplus
}
#endif