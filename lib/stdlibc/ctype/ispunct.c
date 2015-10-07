#include "../ctype.h"

#ifdef __cplusplus
extern "C" {
#endif

int ispunct(int c)
{
    return 	(c >= '!' && c <= '/') ||
    		(c >= ':' && c <= '@') ||
    		(c >= '[' && c <= '`') ||
    		(c >= '{' && c <= '~') ||
    		(c > 0x7f);
}

#ifdef __cplusplus
}
#endif
