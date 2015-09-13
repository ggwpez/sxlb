#include "stdio/stdio.h"
#include "stdlib/stdlib.h"
#include "time/time.h"

sig_atomic_t signaled = 0;

void my_handler (int param)
{
	signaled = 1;
}

int main ()
{
	void (*prev_handler)(int);

	prev_handler = signal (SIGINT, my_handler);

	/* ... */
	raise(SIGINT);
	/* ... */

	printf ("signaled is %u.\n",signaled);
	
	return 0;
}