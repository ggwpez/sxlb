#ifndef _POSIXFCNTL_H
#define _POSIXFCNTL_H

#ifdef __cplusplus
extern "C" {
#endif

int creat(const char *, mode_t);
int open(const char *, int, ...);

#ifdef __cplusplus
}
#endif
#endif
