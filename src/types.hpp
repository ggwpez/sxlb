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

#define nullptr 0
#define NULL 0

#define sgn(x) ((x < 0) ? -1 : ((x > 0) ? 1 : 0))
#define abs(x) ((x < 0) ? -x: x)

//#define assert(expr) ((expr) ? (void)0 : (syshlt("Assertion failed: " #expr ", file " __FILE__ ", line " __LINE__);))
#define assert(e)
#define NAME_MAX 255
#define TASK_SWITCH __asm__ __volatile__("int $32");
#define hlt asm("hlt");
extern "C" { extern void nop(); }
#define cli_hlt asm("cli"); asm("hlt");
#define stop asm("jmp .");
#define finit asm("finit"); \
    io::asm_set_register_ctrl(0, io::asm_get_register_ctrl(0) | (1 << 5));

#define USER_RPL 0

typedef signed char byte_t, char_t, int8_t;
typedef unsigned char ubyte_t, uchar_t, uint8_t;

typedef signed short int word_t, short_t, int16_t;
typedef unsigned short int uword_t, ushort_t, uint16_t;

typedef signed int dword_t, int_t, int32_t, sint32_t;
typedef unsigned int udword_t, uint_t, uint32_t, size_t, LPTR;
typedef char*  va_list;

typedef signed long long qword_t, int64_t;
typedef unsigned long long uqword_t, uint64_t, LLPTR;

static_assert(sizeof(int8_t ) == 1, "Typesize wrong. (int8_t )");
static_assert(sizeof(int16_t) == 2, "Typesize wrong. (int16_t)");
static_assert(sizeof(int32_t) == 4, "Typesize wrong. (int32_t)");
static_assert(sizeof(int64_t) == 8, "Typesize wrong. (int64_t)");

#define break_point __asm__ __volatile__("xchgw %bx, %bx");

#define _2NSIZEOF(n, s) ((sizeof(n) + s -1) & ~(s - 1))
#define _INTSIZEOF(n)   ((sizeof(n) + sizeof(int) - 1) & ~(sizeof(int) - 1))
#define COUNTOF(arr)     (sizeof(arr) / sizeof(arr[0]))

#define va_start(ap,v)  ( ap = (va_list)_ADDRESSOF(v) + _INTSIZEOF(v) )
#define va_arg(ap,t)    ( *(t *)((ap += _INTSIZEOF(t)) - _INTSIZEOF(t)) )
#define va_end(ap)      ( ap = (va_list)0 )

#define int32_max  0x7fffffff
#define uint32_max 0xffffffff

#define internal static
#define namespace2(a, b) namespace a { namespace b

#define B(x) ( \
	0##x >> 0 & 0001 | \
	0##x >> 2 & 0002 | \
	0##x >> 4 & 0004 | \
	0##x >> 6 & 0010 | \
	0##x >> 8 & 0020 | \
	0##x >> 10 & 0040 | \
	0##x >> 12 & 0100 | \
	0##x >> 14 & 0200)

#define PAGE_SIZE	0x1000

#define _512KiB	  128*PAGE_SIZE
#define _1MiB	  256*PAGE_SIZE
#define _2MiB	  512*PAGE_SIZE
#define _4MiB	 1024*PAGE_SIZE
#define _8MiB	 4096*PAGE_SIZE
#define _16MiB 	 8192*PAGE_SIZE
#define _32MiB  16384*PAGE_SIZE
#define _64MiB       2*_32MiB
#define _128MiB      4*_32MiB

#define _1GiB        1073741824
#define _2GiB        2*_1GiB
#define _4GiB        0xffffffff

#ifndef __cplusplus
enum { false = 0, true = 1 };
typedef ubyte_t bool;
#define _ADDRESSOF(v)   (&(v))
#else
#define _ADDRESSOF(v)   (&reinterpret_cast<const char &>(v))
#endif
