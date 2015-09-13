#ifndef _MACRO_H
#define _MACRO_H

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned int LPTR;

#define B(x) ( \
	0##x >> 0 & 0001 | \
	0##x >> 2 & 0002 | \
	0##x >> 4 & 0004 | \
	0##x >> 6 & 0010 | \
	0##x >> 8 & 0020 | \
	0##x >> 10 & 0040 | \
	0##x >> 12 & 0100 | \
	0##x >> 14 & 0200)

#ifdef __cplusplus
}
#endif
#endif
