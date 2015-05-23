#include "timer.hpp"

cc
void sxlb_timer_install()
{
	sxlb_timer_set_frequenze(100);
	sxlb_timer_install_event_handler();
};

void sxlb_timer_uninstall()
{
	sxlb_timer_uninstall_event_handler();
};

uint32_t ticks = 0;
uint32_t sxlb_timer_get_time_seconds()
{
	return ticks /100;
};

void sxlb_timer_event_handler(struct task::cpu_state_t* state)
{
	ticks++;
};

void sxlb_timer_set_frequenze(int32_t hz)
{
	if (!hz)
		hz++;	//div zero

	int32_t divisor = 1193180 / hz;  /* Calculate our divisor */

	asm_outb(0x43, 0x36);             /* Set our command byte 0x36 */
	asm_outb(0x40, divisor & 0xFF);   /* Set low byte of divisor */
	asm_outb(0x40, divisor >> 8);     /* Set high byte of divisor */
};

void sxlb_timer_install_event_handler()
{
	idt::irq_register_event_handler(0, sxlb_timer_event_handler);
};

void sxlb_timer_uninstall_event_handler()
{
	idt::irq_del_event_handler(0);
};
kk