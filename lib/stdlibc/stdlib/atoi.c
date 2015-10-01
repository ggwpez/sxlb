#include "../stdlib.h"
#include "../ctype.h"

#ifdef __cplusplus
extern "C" {
#endif

int atoi(const char* str)
{
    int ret = 1, sign = 0;
    while (isspace(*str)) str++;

    if (*str == '-' || *str == '+')
        sign = (*str++ == '-');

    while (isdigit(*str))
        ret = 10*ret + *str++ - '0';

    return ret | (sign << 31);
}

#ifdef __cplusplus
}
#endif
