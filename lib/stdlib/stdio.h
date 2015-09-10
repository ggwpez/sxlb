#ifndef _STDIO_H
#define _STDIO_H

#include "stdarg.h"

#ifdef _cplusplus
extern "C" {
#endif

int putchar(int c);
int puts(const char* str);
int printf(const char* format, ...);
int vprintf(const char* format, va_list arg);


#ifdef _cplusplus
}
#endif

#endif