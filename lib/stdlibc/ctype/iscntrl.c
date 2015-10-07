#include "../ctype.h"

#ifdef __cplusplus
extern "C" {
#endif

int iscntrl(int c)
{
	return 	(c >= 0 && c <= 0x1f) ||
			(c == 0x7f);
}

#ifdef __cplusplus
}
#endif
