#pragma once

#include "../idt/idt.hpp"
#include "../ui/textmode.hpp"
#include "../hw/hwaccess.hpp"
#include "../ui/video.hpp"

namespace time
{
void install();
void uninstall();

uint32_t get_seconds();
uint32_t get_ticks();

/*Don't use.*/
void set_frequenze(int32_t hz);
/*Don't use.*/
void event_handler(struct task::cpu_state_t* state);
/*Don't use.*/
void uninstall_event_handler();
/*Don't use.*/
void install_event_handler();
}