#include "stdio.h"
#include "../../src/system/syscall_defs.hpp"

int putchar(int c)
{
	SYSCALL1(SYSCNUM_PUTC, (char)c);
}

int puts(const char* str)
{
	SYSCALL1(SYSCNUM_PRINTL, str);
}

int printf(const char* format, ...)
{
	va_list ap;
	va_start(ap, format);
	return vprintf(format, ap);
	//va_end(ap);

	//return ret;
}

int vprintf(const char* format, va_list arg)
{

}