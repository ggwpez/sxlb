#include "stdio.h"

#ifdef __cplusplus
extern "C" {
#endif

int printf(const char* format, ...)
{
	va_list ap;
	va_start(ap, format);
	return vprintf(format, ap);
	//va_end(ap);

	//return ret;
}

#ifdef __cplusplus
}
#endif
