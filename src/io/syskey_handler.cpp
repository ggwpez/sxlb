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
        char c = state_to_char(key);

        if (c == 'c')
        {
            task::end(0);
            printfl("\nKilled: %u\n", focused);
        }
        else if (c >= '1' && c <= '9')
            ui::window::switch_window(c -'1');
        else
            printf("^%c", c);
    }
}}
