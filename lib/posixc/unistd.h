#ifndef _POSIXUNISTD_H
#define _POSIXUNISTD_H

#include "sys/types.h"

#ifdef __cplusplus
extern "C" {
#endif

char* getcwd(char* buf, uint32_t size);

#ifdef __cplusplus
}
#endif
#endif
