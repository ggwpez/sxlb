#ifndef VBUFF_HPP
#define VBUFF_HPP

#include "../types.hpp"
#include "../interproc/mutex.hpp"

namespace2(ui, video)
{
    /*typedef struct
    {
        uint16_t w, h;
        uint8_t bpp, bypp;
        uint32_t pitch;
        LPTR fb;
        uint32_t len;
        uint8_t type;
    } vbuff_info_t;

    typedef struct
    {
        vbuff_t(vbuff_info_t& data);

        vbuff_info_t data;
        mutex_t* mutex;
        bool changed;
    } vbuff_t;*/
}}
#endif // VBUFF_HPP
