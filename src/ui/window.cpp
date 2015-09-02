#include "window.hpp"

namespace2 (ui, window)
{
    uint16_t window_size;

    window_t* windows;      //höhö
    ubyte_t actual_windows;
    ubyte_t actual_window;

    void init()
    {
        if (!ui::text::initialized)
        {
            break_point
            return;
        }

        uint16_t x, y;
        ui::text::get_size(x, y);
        window_size = x*y;

        LPTR mem = memory::k_malloc(START_WINDOWS * window_size, 0, 0);
        memory::memset(mem, 0, START_WINDOWS * window_size);

        for (int i = 0; i < START_WINDOWS; ++i)
        {
            windows[i].data = mem + i * window_size;
            windows[i].cursor_x = windows[i].cursor_y = 0;
        }

        actual_windows = START_WINDOWS;
        actual_window = 0;
    }

    void switch_window(ubyte_t window_index)
    {
        if (window_index == actual_window || window_index >= actual_windows)
            return;

        window_t focus = windows[actual_window];
        memory::memcpy(focus.data, ui::text::vram, window_size);    //save old window state
        ui::text::get_cursor(&focus.cursor_x, &focus.cursor_y);
        ui::text::clear_screen();

        actual_window = window_index;
        focus = windows[actual_window];
        memory::memcpy(ui::text::vram, focus.data, window_size);
        ui::text::set_cursor(focus.cursor_x, focus.cursor_y);
        ui::text::update();
    }
}}
