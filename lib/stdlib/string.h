#ifndef _STDSTRING_H
#define _STDSTRING_H

#include "stddef.h"

void* memcpy(void* dest, void const* src, size_t length);
void* memset(void* dest, int value, size_t length);
int   memcmp(void const* mem1, void const* mem2, size_t length);

char* strcpy(char* dest, char const* src);
int   strcmp(char const* str1, char const* str2);

#endif