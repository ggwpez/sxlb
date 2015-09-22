#include "../string.h"

#ifdef __cplusplus
extern "C" {
#endif

int strcmp(char const* str1, char const* str2)
{
	while (*str1 && *str1 == *str2)
		str1++, str2++;

	return (unsigned char)*str1 - (unsigned char)*str2;
}

#ifdef __cplusplus
}
#endif