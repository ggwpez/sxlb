#include "../utsname.h"
#include "../../../stdlibc/string.h"
#include "../../../../src/system/syscall_defs.hpp"

#ifdef __cplusplus
extern "C" {
#endif

int uname(utsname_t* buf)	//TODO: set errno
{
    int ret;
    SYSCALL_RET1(SYSCNUM_UTILS_GET_VERSION, ret, buf);
    return ret;
}

#ifdef __cplusplus
}
#endif
