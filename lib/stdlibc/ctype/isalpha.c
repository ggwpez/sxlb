#include "../ctype.h"

#ifdef __cplusplus
extern "C" {
#endif

int isalpha(int c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

#ifdef __cplusplus
}
#endif
