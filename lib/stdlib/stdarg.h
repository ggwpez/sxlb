#ifndef _STDARG_H
#define _STDARG_H

#define _VA_LIST 1

typedef __builtin_va_list va_list;
#define va_start(ap, v)  __builtin_va_start(ap, v)
#define va_arg(ap, t)    __builtin_va_arg(ap, t)
#define va_copy(d, s)    __builtin_va_copy(d, s)
#define va_end(ap)      __builtin_va_end(ap)
#define __GNUC_VA_LIST 1

#endif