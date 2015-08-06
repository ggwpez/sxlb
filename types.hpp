#pragma once

#define __CHECKS 3	//enter check level, lower = unsafer = faster, higher = safer = slower

#if __CHECKS > 0
    #define __CHECKS_NLPTR true
#endif
#if __CHECKS > 1
    #define __CHECKS_ADVNCD true
#endif
#if __CHECKS > 2
    #define __CHECKS_DBG true
#endif

#ifndef __BITS
#define __BITS 32		//enter bits
#endif

#define nullptr 0
#define NULL 0;
#define sgn(x) ((x < 0) ? -1 : ((x > 0) ? 1 : 0))
#define abs(x) ((x < 0) ? -x: x)
#define TASK_SWITCH __asm__ __volatile__("int $32");
#define hlt asm("hlt");
#define hlt_cli asm("cli"); asm("hlt");
#define stop asm("jmp .");
#define finit asm("finit"); \
	asm_set_register_ctrl(0, asm_get_register_ctrl(0) | (1 << 5));

typedef signed char byte_t, char_t, int8_t;
typedef unsigned char ubyte_t, uchar_t, uint8_t;

typedef signed short int word_t, short_t, int16_t;
typedef unsigned short int uword_t, ushort_t, uint16_t;

typedef signed int dword_t, int_t, int32_t;
typedef unsigned int udword_t, uint_t, uint32_t, size_t, LPTR;
typedef char*  va_list;

#define cc ;
#define kk ;

#define break_point __asm__ __volatile__("xchgw %bx, %bx");

#ifdef __cplusplus
#define _ADDRESSOF(v)   ( &reinterpret_cast<const char &>(v) )
#else  /* __cplusplus */
#define _ADDRESSOF(v)   ( &(v) )
#endif  /* __cplusplus */

#define _INTSIZEOF(n)   ( (sizeof(n) + sizeof(int) - 1) & ~(sizeof(int) - 1) )

#define va_start(ap,v)  ( ap = (va_list)_ADDRESSOF(v) + _INTSIZEOF(v) )
#define va_arg(ap,t)    ( *(t *)((ap += _INTSIZEOF(t)) - _INTSIZEOF(t)) )
#define va_end(ap)      ( ap = (va_list)0 )

#define int32_max  0x7fffffff
#define uint32_max 0xffffffff

#define internal static

#define B(x) ( \
	0##x >> 0 & 0001 | \
	0##x >> 2 & 0002 | \
	0##x >> 4 & 0004 | \
	0##x >> 6 & 0010 | \
	0##x >> 8 & 0020 | \
	0##x >> 10 & 0040 | \
	0##x >> 12 & 0100 | \
	0##x >> 14 & 0200)

#ifndef __cplusplus
enum { false = 0, true = 1 };
typedef ubyte_t bool;
#endif
