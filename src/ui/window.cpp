#include "window.hpp"
#include "../user/executable.hpp"

namespace2 (ui, window)
{
    uint16_t window_size;

    window_t* windows;      //höhö
    ubyte_t actual_windows;
    ubyte_t actual_window;

    bool inited = false;
    void init()
    {
        if (!ui::text::initialized || inited)
            return;
        else inited = true;

        uint16_t x, y;
        ui::text::get_size(x, y);
        window_size = x*y;

        LPTR mem = memory::k_malloc(MAX_WINDOWS * window_size, 0, 0);
        memory::memset(mem, 0, MAX_WINDOWS * window_size);

        for (int i = 0; i < MAX_WINDOWS; ++i)
        {
            windows[i].data = mem + i * window_size;
            windows[i].cursor_x = windows[i].cursor_y = 0;
        }

        actual_windows = MAX_WINDOWS;
        actual_window = 0;
        logDONE;
    }

    void fork_bash()
    {
        char* argv[] = { "/initrd/bash.dat", nullptr };
        execve(nullptr, argv[0], argv, nullptr);
    }

    bool switch_window(ubyte_t window_index)
    {
        if (window_index == actual_window || window_index >= actual_windows)
            return false;

        window_t focus = windows[actual_window];
        memory::memcpy(focus.data, ui::text::vram, window_size);    //save old window state
        ui::text::get_cursor(&focus.cursor_x, &focus.cursor_y);
        //ui::text::clear_screen();

        actual_window = window_index;
        focus = windows[actual_window];
        memory::memcpy(ui::text::vram, focus.data, window_size);
        ui::text::set_cursor(focus.cursor_x, focus.cursor_y);
        //ui::text::update();   //already in set_cursor
        return true;
    }
}}
