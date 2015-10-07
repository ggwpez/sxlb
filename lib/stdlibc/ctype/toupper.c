#include "../ctype.h"

#ifdef __cplusplus
extern "C" {
#endif

int toupper(int c)
{
    return (islower(c) ? c - 32 : c);
}

#ifdef __cplusplus
}
#endif
