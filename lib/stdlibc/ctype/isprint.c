#include "../ctype.h"

#ifdef __cplusplus
extern "C" {
#endif

int isprint(int c)
{
    return 	(c >= ' ' && c <= '~') ||
    		(c > 0x7f);
}

#ifdef __cplusplus
}
#endif
