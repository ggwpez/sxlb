#ifndef _STDBOOL_H
#define _STDBOOL_H

#ifdef __cplusplus
extern "C" {
#endif

#if !defined(__bool_true_false_are_defined)// && !defined(__cplusplus)
	#define true 1
	#define false 0
#endif

#ifdef __cplusplus
}
#endif
#endif
