/* note these headers are all provided by newlib - you don't need to provide them */
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/fcntl.h>
#include <sys/times.h>
#include <sys/errno.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>

#include "../../../../../../../src/system/syscall_defs.hpp"
#include "stdio/file_ptr.h"
#include "stdio/open.c"
#include "stdio/read.c"

void _exit(int ret)
{
	SYSCALL1(SYSCNUM_TASK_EXIT, ret);	//normal way to exit

	int i;
	while (1)							//hardcore way, if the syscall fails
		i = 1 / 0;
}

int close(int file)
{
	SYSCALL1(SYSCNUM_VFS_CLOSE, file_ptrs[file].fs_node);

    file_ptrs[file].fs_node = 0;
    file_ptrs[file].off = 0;
    return 0;
}

extern char** environ; /* pointer to array of char * strings that define the current environment variables */
int execve(const char *path, char * const argv[], char * const envp[] );
int fork();
int fstat(int file, struct stat *st);
int getpid()
{
	int pid;
	SYSCALL_RET0(SYSCNUM_TASK_GET_PID, pid);
	return pid;
}

int isatty(int file);
int kill(int pid, int sig)
{
	int ret = -1;
	SYSCALL_RET2(SYSCNUM_TASK_SIG, ret, pid, sig);
	return ret;
}

int link(const char *path1, const char *path2 );
off_t lseek(int fildes, off_t offset, int whence )
{
	if (whence == SEEK_CUR)
		return ((struct __file_ptr*)fildes)->off += offset;
	if (whence == SEEK_SET)
		return ((struct __file_ptr*)fildes)->off = offset;
		
	//SEEK_END
	return (off_t)-1;
};

int __getchar()
{
    int c = 0;
    while (!c)
        SYSCALL_RET0(SYSCNUM_GETC, c);

    return c;
}

unsigned int i = 0;

register char *stack_ptr asm ("esp");
void* sbrk(ptrdiff_t incr)
{
	extern char _end;		//see linker script
	static char *heap_end;
	char *prev_heap_end;

	if (heap_end == 0)
		heap_end = &_end;

	prev_heap_end = heap_end;
	if (heap_end + incr > stack_ptr) {
		write (1, "Heap and stack collision\n", 25);
		abort ();
	}

	heap_end += incr;
  return (caddr_t) prev_heap_end;
}

int stat(const char *file, struct stat *st);
clock_t times(struct tms *buf);
int unlink(const char *path );
int wait(int *status);

//###
int write(int fd, const void *buf, size_t nbyte )
{
	unsigned int l = 0;
	
	while (nbyte--)
		SYSCALL1(SYSCNUM_PUTC, ((char*)buf)[l++]);

	return l;
};

int __i = 0;
int _EXFUN(gettimeofday, (struct timeval *__restrict __p,
			  void *__restrict __tz))
{ return __i++; }