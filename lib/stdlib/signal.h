#ifndef _STDSIGNAL_H
#define _STDSIGNAL_H

#define SIGTERM	0
#define SIGSEGV	SIGTERM	+1
#define SIGINT	SIGSEGV	+1
#define SIGILL	SIGINT	+1
#define SIGABRT SIGILL	+1
#define SIGFPE	SIGABRT	+1
#define _SIG_C	SIGFPE	+1

typedef volatile int sig_atomic_t;

extern const void (*SIG_DFL)(int sig);
extern const void (*SIG_IGN)(int sig);

void (*signal(int sig, void (*func)(int)))(int);
int raise(int sig);

#endif