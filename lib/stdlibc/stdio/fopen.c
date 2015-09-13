#include "stdio.h"
#include "../../../src/system/syscall_defs.hpp"

#ifdef __cplusplus
extern "C" {
#endif

FILE* fopen(char const* const filename, char const* const mode)
{
    FILE* fp = NULL;
    LPTR f_desc;

    if (*mode != 'r' && *mode != 'w' && *mode != 'a')
        return NULL;

    for (int i = 0; i < _FILE_C; ++i)                           //find free stream
        if (fp->state == ST_CLOSED && fp->access == AC_NONE)
            break;
    if (!fp)
        return NULL;                                            //none found



}

#ifdef __cplusplus
}
#endif
