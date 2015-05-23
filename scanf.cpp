#include "scanf.hpp"
int32_t sscanf(const char_t* s, const char_t* f, ...)
{
#if __CHECKS_NLPTR
	if (!s || !f)
		return -1;
#endif

	va_list ap;
	va_start(ap, f);
	uint32_t matched = 0;

	while (*f && *s)
	{
		if (*f == '%')
			switch (*(++f))
			{
				case 'u':

					break;
				default:
					break;
			}

		f++; s++;
	}
	va_end(ap);
};