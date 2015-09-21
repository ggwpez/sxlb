#ifndef _STDINT_H
#define _STDINT_H

#ifdef __cplusplus
extern "C" {
#endif

typedef signed char int8_t;
typedef unsigned char uint8_t, uchar_t;

typedef signed short int16_t;
typedef unsigned short uint16_t;

typedef signed int int32_t;
typedef unsigned int uint32_t, LPTR;

typedef int intptr_t;
typedef unsigned int uintptr_t;

_Static_assert(sizeof(uint8_t)  == 1, "Better compile it in 32bit, pls.");
_Static_assert(sizeof(uint16_t) == 2, "Better compile it in 32bit, pls.");
_Static_assert(sizeof(uint32_t) == 4, "Better compile it in 32bit, pls.");

#ifdef __cplusplus
}
#endif
#endif
