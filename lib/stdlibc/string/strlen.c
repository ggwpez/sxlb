#include "../string.h"

#ifdef __cplusplus
extern "C" {
#endif

size_t strlen(char const* str)
{
    unsigned int i = 0;

    while (str[i]) i++;
    return i;
}

#ifdef __cplusplus
}
#endif