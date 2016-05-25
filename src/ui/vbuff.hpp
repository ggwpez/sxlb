#ifndef VBUFF_HPP
#define VBUFF_HPP

#include "../types.hpp"
#include "../interproc/mutex.hpp"

namespace2(ui, video)
{
    class vbuff_t
    {
    public:
        vbuff_t();

        LPTR data;
        uint32_t size;
        mutex_t* mutex;
    private:
        bool changed;
    };
}}
#endif // VBUFF_HPP
