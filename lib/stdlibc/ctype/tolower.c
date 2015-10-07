#include "../ctype.h"

#ifdef __cplusplus
extern "C" {
#endif

int tolower(int c)
{
    return (isupper(c) ? c + 32 : c);
}

#ifdef __cplusplus
}
#endif
