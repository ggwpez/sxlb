#include "syskey_handler.hpp"

namespace2(io, keyboard)
{
    uint8_t focused, w_count;

    bool inited = false;
    void syskey_init()
    {
        logINF("initializing syskey hander...");
        if (inited)
        {
            logWAR("already done?!");
            return;
        }
        else inited = true;

        focused = 0;
        w_count = 1;
        logDONE;
    }

    void syskey_handler(key_state_t key)
    {
        if (!inited) return;
        char c = state_to_char(key | (META_SHIFT << 16));
        printf("^%c", c);

        if (c == 't')
        {
            if (w_count < MAX_WINDOWS)
            {
                focused = w_count++;

                ui::window::switch_window(focused);
                printfl("Created: %u", focused);
            }
            else
                printfl("Max windows count of: %u reached.", MAX_WINDOWS);
        }
        else if (c == 'w')
        {
            if (w_count > 1)
            {
                task::end(0);
                focused--; w_count--;

                ui::window::switch_window(focused);
                printfl("Killed: %u", focused);
            }
            else
                printfl("Last window cant be destroyed.");
        }
        else if (c >= '1' && c <= '9')
        {
            uint8_t sel = c -'1';

            if (sel < w_count)
            {
                focused = sel;
                ui::window::switch_window(focused);
                printfl("Switched to: %u", focused);
            }
            else
                printfl("Window unknown.");
        }
    }
}}
