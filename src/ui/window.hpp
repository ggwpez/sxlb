#pragma once

#include "../types.hpp"
#include "../memory/memory.hpp"
#include "../ui/textmode.hpp"

namespace2 (ui, window)
{
    #define MAX_WINDOWS 6

    void init();
    bool switch_window(ubyte_t window_index);

    struct window_t
    {
        ubyte_t* data;
        uint16_t cursor_x;
        uint16_t cursor_y;
    }__attribute__((packed));
}}
