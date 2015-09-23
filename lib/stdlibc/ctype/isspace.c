#include "../ctype.h"

#ifdef __cplusplus
extern "C" {
#endif

int isspace(int c)
{
    return (c == ' '  ||
            c == '\t' ||
            c == '\n' ||
            c == '\v' ||
            c == '\f' ||
            c == '\r');
}

#ifdef __cplusplus
}
#endif
