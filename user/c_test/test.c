#include "../../lib/stdlib/stdio.h"
#include "../../lib/stdlib/stdlib.h"

void dtor1()
{
	puts("dtor1 did died.");
}

void dtor2()
{
	puts("dtor2 did died.");
}

int main()
{
	puts("this gonb gud");
	atexit(&dtor1);
	atexit(&dtor2);

	return 1337;
}