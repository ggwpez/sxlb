#include "../ctype.h"

#ifdef __cplusplus
extern "C" {
#endif

int islower(int c)
{
    return (c >= 'a' && c <= 'z');
}

#ifdef __cplusplus
}
#endif
