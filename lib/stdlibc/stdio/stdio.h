#ifndef _STDIO_H
#define _STDIO_H

#include "../stdarg.h"
#include "../stdint.h"
#include "../macro.h"

#ifdef __cplusplus
extern "C" {
#endif

#define NULL ((void*)0)
typedef unsigned int size_t;

#define NAME_MAX 64
#define FILENAME_MAX NAME_MAX
#define EOF ((int32_t)'EOF '*-1)
#define _FILE_C 16                  //simultaneously open file streams

typedef enum FILE_acces             //& 00000011 access mode, & 00000100 bit for update, & 00001000 bit for binary, & 00010000 for unbuffered stream
{
	AC_NONE		= B(00000000),
	AC_READ		= B(00000001),
	AC_WRITE	= B(00000010),
	AC_APPEND	= B(00000100),
	AC_UPDATE	= B(00001000),
	AC_BINARY	= B(00010000),
	AC_UNBUF	= B(00100000)
} FILE_acces_t;

typedef enum FILE_state
{
	ST_CLOSED	= B(00000000),
    ST_EOF		= B(00000001),
    ST_ERR		= B(11111110)
} FILE_state_t;

typedef struct _iobuf
{
    int left;           //remaining chars
    char* ptr;          //next one
	char* base;         //buffer
    char access;        //access moce
	char state;         //file state
	LPTR fd;            //file descriptor
} FILE;

extern FILE _iobufs[_FILE_C];

#define stdin  (&_iobufs[0])
#define stdout (&_iobufs[1])
#define stderr (&_iobufs[2])

#define feof(fp) 	((fp)->state & ST_EOF)	//this should be actually a function, not a macro, but nah…
#define ferr(fp) 	((fp)->state & ST_ERR)	// "
#define fileno(fp)	((fp)->fd)				// "

//#define getchar()   fgetc(stdin)
//#define putchar(v)  fputc(v, stdout)

FILE* fopen (char const* const filename, char const* const mode);

int fgetc(FILE* fp);
int fputc(int v, FILE* fp);

int _fillbuff(FILE* fp);
int _flushbuff(FILE* fp, size_t l);

int putchar(int c);
int getchar();

int puts(const char* str);
int printf(char* const format, ...);
int vprintf(const char* format, va_list arg);

#ifdef __cplusplus
}
#endif
#endif
