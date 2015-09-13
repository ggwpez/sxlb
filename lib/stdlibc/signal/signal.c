#include "signal.h"
#include "../../../src/system/syscall_defs.hpp"

#ifdef __cplusplus
extern "C" {
#endif

void default_handler(int sig);
void ignore_handler (int sig);

const void (*SIG_DFL)(int sig) = &default_handler;
const void (*SIG_IGN)(int sig) = &ignore_handler ;
void (*sig_handlers[_SIG_C])(int) = { &default_handler, &default_handler, &default_handler, &default_handler, &default_handler, &default_handler };


void (*signal(int sig, void (*func)(int)))(int)
{
	if (sig >= _SIG_C)
		return -1;

	void (*old_hander)(int) = sig_handlers[sig];
	sig_handlers[sig] = func;

	return old_hander;
}

int raise(int sig)
{
	if (sig >= _SIG_C)
		return -1;

	sig_handlers[sig](sig);
	return 0;
}

void default_handler(int sig)
{
	switch(sig)
	{
		case SIGTERM:
			break;
		case SIGSEGV:
			break;
		case SIGINT:
			break;
		case SIGILL:
			break;
		case SIGABRT:
			SYSCALL0(SYSCNUM_TASK_EXIT);
			break;
		case SIGFPE:
			break;
	}
}

void ignore_handler(int sig) { }

#ifdef __cplusplus
}
#endif
