#include "stdio.h"

#ifdef __cplusplus
extern "C" {
#endif

int printf(char* const format, ...)
{
	va_list ap;
	va_start(ap, format);
    int ret =  vprintf(format, ap);
    va_end(ap);

    return ret;
}

#ifdef __cplusplus
}
#endif
