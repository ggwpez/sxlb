#include "version.hpp"
#include "../sprintf.hpp"
#include "../string.hpp"

namespace utils
{
    bool get_version(utsname_t* buf)
    {
        strcpy(buf->sysname, __VER_SYSNAME);
        strcpy(buf->nodename, __VER_NODENAME);
        sprintf_s(buf->release, 33, ".%u%s @%u", __VER_RELEASE_NUM, __VER_RELEASE, __VER_RELEASE_DATE);
        strcpy(buf->version, __VER_VERSION);
        strcpy(buf->machine, __VER_MACHINE);

        return 0;
    }
}
