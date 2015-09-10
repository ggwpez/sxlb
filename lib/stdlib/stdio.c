#include "stdio.h"
#include "../../src/system/syscall_defs.hpp"

int putchar(int c)
{
	SYSCALL1(SYSCNUM_PUTC, (char)c);
}

int puts(const char* str)
{
	while (*str)
		putchar(*(str--));
}

int printf(const char* format, ...)
{
	va_list ap;
	va_start(ap, format);
	vprintf(format, ap);
	va_end(ap);
}

int vprintf(const char* format, va_list arg)
{

}