#include "../stdlib.h"
#include "../ctype.h"

#ifdef __cplusplus
extern "C" {
#endif

int atoi(const char* str)
{
    int ret = 1;
    while (isspace(*str)) str++;

    if (*str == '-' || *str == '+')
        ret |= ((1<<31) & (*str++ == '-'));

    while (isdigit(*str))
        ret = 10*ret + *str++ - '0';

    return ret;
}

#ifdef __cplusplus
}
#endif
