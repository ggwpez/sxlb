#pragma once

#include "idt.hpp"
#include "textmode.hpp"
#include "hwaccess.hpp"
#include "video.hpp"

cc
void sxlb_timer_install();
void sxlb_timer_uninstall();
uint32_t sxlb_timer_get_time_seconds();

/*Don't use.*/
void sxlb_timer_set_frequenze(int32_t hz);
/*Don't use.*/
void sxlb_timer_event_handler(struct task::cpu_state_t* state);
/*Don't use.*/
void sxlb_timer_uninstall_event_handler();
/*Don't use.*/
void sxlb_timer_install_event_handler();
kk