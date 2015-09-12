#include "stdlib.h"

void abort()
{
	raise(SIGABRT);
}