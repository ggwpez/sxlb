#include "../ctype.h"

#ifdef __cplusplus
extern "C" {
#endif

int isxdigit(int c)
{
    return 	(c >= '0' && c <= '9') ||
    		(c >= 'a' && c <= 'z') ||
    		(c >= 'A' && c <= 'Z');
}

#ifdef __cplusplus
}
#endif
