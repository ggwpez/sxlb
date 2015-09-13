#ifndef _STDSIGNAL_H
#define _STDSIGNAL_H

#ifdef __cplusplus
extern "C" {
#endif

enum 
{
	SIGTERM = 0,
	SIGSEGV,
	SIGINT,
 	SIGILL,
	SIGABRT,
	SIGFPE,
	_SIG_C
};

typedef volatile int sig_atomic_t;

extern const void (*SIG_DFL)(int sig);
extern const void (*SIG_IGN)(int sig);

void (*signal(int sig, void (*func)(int)))(int);
int raise(int sig);

#ifdef __cplusplus
}
#endif
#endif
