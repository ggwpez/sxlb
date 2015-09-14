#ifndef _STDARG_H
#define _STDARG_H

#ifdef __cplusplus
extern "C" {
#endif

#define _VA_LIST 1

typedef __builtin_va_list va_list;
#define _INTSIZEOF(n)   ((sizeof(n) + sizeof(int) - 1) & ~(sizeof(int) - 1))

#ifndef __cplusplus
#define _ADDRESSOF(v)   (&(v))
#else
#define _ADDRESSOF(v)   (&reinterpret_cast<const char &>(v))
#endif

#define va_start(ap,v)  ( ap = (va_list)_ADDRESSOF(v) + _INTSIZEOF(v) )
#define va_arg(ap,t)    ( *(t *)((ap += _INTSIZEOF(t)) - _INTSIZEOF(t)) )
#define va_end(ap)      ( ap = (va_list)0 )
#define __GNUC_VA_LIST 1

#ifdef __cplusplus
}
#endif
#endif
