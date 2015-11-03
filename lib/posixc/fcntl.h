/*#ifndef _POSIXFCNTL_H
#define _POSIXFCNTL_H

#ifdef __cplusplus
extern "C" {
#endif

int creat(const char *, mode_t);
int open(const char* path, int flags, ...);
//int openat(int fd, const char* path, int flags, ...);

#ifdef __cplusplus
}
#endif
#endif*/

#ifndef _POSIXDIRENT_H
#define _POSIXDIRENT_H

#include "limits.h"
#include "sys/types.h"
#include "stdint.h"

//#include "../stdlibc/stdint.h"
typedef unsigned int LPTR;

#ifdef __cplusplus
extern "C" {
#endif

#define _BSD_SOURCE 1
#define _DIRENT_HAVE_D_TYPE 1
enum
{
	DT_BLK = 1,
	DT_CHR = 2,
    DT_DIR = 3,         //directory
    DT_FIFO = 4,
	DT_LNK = 5,
    DT_REG = 6,         //regular file
	DT_SOCK = 7,
	DT_UNKNOWN = 0
};

typedef struct dirent
{
	ino_t d_ino;
	uint32_t d_type;
	char d_name[NAME_MAX];
} dirent_t;

typedef struct __dirstr
{
    LPTR fs_node;
	int32_t i;
} DIR;

int             chdir(const char* path);
int 			closedir(DIR*dirp);
DIR* 			opendir(const char*);
struct dirent* 	readdir(DIR*dirp);
int 			readdir_r(DIR*, struct dirent* , struct dirent**);
void 			rewinddir(DIR*);
void 			seekdir(DIR*, long int);
long int 		telldir(DIR*);

#ifdef __cplusplus
}
#endif
#endif
