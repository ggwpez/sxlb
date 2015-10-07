#include "../ctype.h"

#ifdef __cplusplus
extern "C" {
#endif

int isblank(int c)
{
    return 	(c == '\t') ||
    		(c == ' ');
}

#ifdef __cplusplus
}
#endif
