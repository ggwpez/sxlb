#include "string.h"
#include "stdbool.h"

#ifdef __cplusplus
extern "C" {
#endif

void* memcpy(void* dest, void const* src, size_t length)
{
	while (length--)
		*(char*)dest++ = *(char*)src++;

	return dest;
}

void* memset(void* dest, int value, size_t length)
{
	while (length--)
		*(int*)dest++ = value;

	return dest;
}

int memcmp(void const* mem1, void const* mem2, size_t length)
{
	while (length--)
		if (*(char*)mem1++ != *(char*)mem2++)
			return 1;

	return 0;
}

char* strcpy(char* dest, char const* src)
{
	while (*src)
		*dest++ = *src++;

	return dest;
}

int strcmp(char const* str1, char const* str2)
{
	while (*str1 && *str1 == *str2)
		str1++, str2++;

	return (unsigned char)*str1 - (unsigned char)*str2;
}

size_t strlen(char const* str)
{
    unsigned int i = 0;

    while (str[i]) i++;
    return i;
}

#ifdef __cplusplus
}
#endif
