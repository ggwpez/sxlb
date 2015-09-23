#include "../ctype.h"

#ifdef __cplusplus
extern "C" {
#endif

int isdigit(int c)
{
    return (c >= '0' && c <= '9');
}

#ifdef __cplusplus
}
#endif
