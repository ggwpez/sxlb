#ifndef _STDBOOL_H
#define _STDBOOL_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef NDEBUG
	#define assert(expr) ()
#else
	#define assert(expr)
#endif

#ifdef __cplusplus
}
#endif
#endif
