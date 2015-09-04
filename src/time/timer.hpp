#pragma once

#include "../idt/idt.hpp"
#include "../ui/textmode.hpp"
#include "../io/hwaccess.hpp"
#include "../ui/video.hpp"
#include "date.hpp"

namespace time
{
    void init();
    void uninstall();

    uint32_t get_seconds();
    uint32_t get_ticks();

    /*Don't use.*/
    void set_frequenze(int32_t hz);
    /*Don't use.*/
    void event_handler(task::cpu_state_t* state);
    /*Don't use.*/
    void uninstall_event_handler();
    /*Don't use.*/
    void install_event_handler();
}
