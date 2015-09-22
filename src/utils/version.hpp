#pragma once

#include "../types.hpp"
#include "../../lib/posixc/sys/utsname.h"

extern char _BUILD_DATE;
extern char _BUILD_NUMBER;

namespace utils
{
    #define __VER_SYSNAME   "Unix (SUS)"
    #define __VER_NODENAME  "0.0.0.0"
    #define __VER_RELEASE   "-unstable"
    #define __VER_RELEASE_NUM &_BUILD_NUMBER
    #define __VER_RELEASE_DATE &_BUILD_DATE
    #define __VER_VERSION   "SXLB v0.12"
    #define __VER_MACHINE   "x86"

    bool get_version(utsname_t* buf);
}
