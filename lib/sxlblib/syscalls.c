/* note these headers are all provided by newlib - you don't need to provide them */
#include "sys/stat.h"
#include "sys/types.h"
#include "sys/fcntl.h"
#include "sys/times.h"
#include "sys/time.h"
#include "sys/errno.h"
#include "sys/time.h"
#include "sys/stat.h"
#include "reent.h"
#include "stdio.h"
#include "../../src/system/syscall_defs.hpp"
#include "errno.h"
#undef errno
extern int errno;

void _exit()
{
  EXIT(-1);
}

int _close(int file)
{
  return -1;
}

char*  __env[1] = { 0 };
char** environ = __env;

int execve(char *name, char **argv, char **env)
{
  errno = ENOMEM;
    return -1;
}

int fork(void)
{
    errno = EAGAIN;
    return -1;
}

int fstat(int file, struct stat *st)
{
    st->st_mode = S_IFCHR;
    return 0;
}

int getpid()
{
  int pid;
  SYSCALL_RET0(SYSCNUM_TASK_GET_PID, pid);
  return pid;
}

int isatty(int file)
{
  return 1;
}

int kill(int pid, int sig)
{
  errno = EINVAL;
    return -1;
}

int link(char *old, char *new)
{
  errno = EMLINK;
    return -1;
}

int lseek(int file, int ptr, int dir)
{
  return 0;
}

int _open(const char *name, int flags, ...)
{
  return 0;
  FILE* fp = NULL;
    /*LPTR f_desc;

    if (*mode != 'r' && *mode != 'w' && *mode != 'a')
        return NULL;

    for (int i = 0; i < _FILE_C; ++i)                           //find free stream
        if (fp->state == ST_CLOSED && fp->access == AC_NONE)
            break;
    if (!fp)
        return NULL;                                            //none found

  */
}

int read(int file, char *ptr, int len)
{
  return 0;
}

register char *stack_ptr asm ("esp");
caddr_t sbrk(int incr)
{
  extern char _end;   /* Defined by the linker */
  static char *heap_end;
  char *prev_heap_end;

  if (heap_end == 0)
    heap_end = &_end;

  prev_heap_end = heap_end;
  if (heap_end + incr > stack_ptr)
  {
    write (1, "Heap and stack collision\n", 25);
    abort ();
  }

  heap_end += incr;
  return (caddr_t) prev_heap_end;
}

int stat(const char *file, struct stat *st)
{
  st->st_mode = S_IFCHR;
  return 0;
}

clock_t times(struct tms *buf)
{
  return -1;
}

int unlink(char *name)
{
  errno = ENOENT;
    return -1; 
}

int wait(int *status)
{
  errno = ECHILD;
    return -1;
}

int _write(int __fd, const void *__buf, size_t __nbyte )
{
  char* buf = __buf;
  while (__nbyte--)
  {
    SYSCALL1(1, *buf++);
  }
}

unsigned int usec = 0;

int gettimeofday(struct timeval *__restrict __p, void *__restrict __tz)
{
  __p->tv_usec = usec++ % 1000;
  __p->tv_sec = usec / 1000;
}