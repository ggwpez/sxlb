#ifndef _STDBOOL_H
#define _STDBOOL_H

#ifndef NDEBUG
	#define assert(expr) ()
#else
	#define assert(expr)
#endif

#endif