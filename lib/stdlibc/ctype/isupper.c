#include "../ctype.h"

#ifdef __cplusplus
extern "C" {
#endif

int isupper(int c)
{
    return (c >= 'A' && c <= 'Z');
}

#ifdef __cplusplus
}
#endif
