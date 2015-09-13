#ifndef _STDLIB_H
#define _STDLIB_H

#include "../signal/signal.h"
#include "../stddef.h"
#include "../stdint.h"

#ifdef __cplusplus
extern "C" {
#endif

#define NULL ((void*)0)
typedef unsigned int size_t;

void abort();

//dtor.c
int atexit(void (*func)(void));
void exit(int status);

#ifdef __cplusplus
}
#endif
#endif
