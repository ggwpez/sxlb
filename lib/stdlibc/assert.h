#ifndef _STDASSERT_H
#define _STDASSERT_H

#ifdef __cplusplus
extern "C" {
#endif

void assertion(char const* const expr, char const* const file, unsigned int line);

#ifndef NDEBUG
	#define assert(expr) ((expr) ? (void)0 : (assertion(#expr, __FILE__, __LINE__)))
#else
	#define assert(expr)
#endif

#ifdef __cplusplus
}
#endif

#endif