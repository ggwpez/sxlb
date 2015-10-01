#include "../assert.h"
#include "../stdio.h"
#include "../stdlib.h"

#ifdef __cplusplus
extern "C" {
#endif

void assertion(char const* const expr, char const* const file, unsigned int line)
{
	printf("Assertion failed: %s, file %s, line %u", expr, file, line);
	exit(EXIT_FAILURE);
}

#ifdef __cplusplus
}
#endif