#include "timer.hpp"

namespace time
{
void install()
{
    set_frequenze(100);
	install_event_handler();
};

void uninstall()
{
	uninstall_event_handler();
};

uint32_t ticks = 0;
uint32_t get_ticks()
{
	return ticks;
};

uint32_t get_seconds()
{
	return ticks /1000;
};

void event_handler(struct task::cpu_state_t* state)
{
	ticks++;
};

void set_frequenze(int32_t hz)
{
	if (!hz)
		hz++;	//div zero

	int32_t divisor = 1193180 / hz;  /* Calculate our divisor */

    hw::asm_outb(0x43, 0x36);             /* Set our command byte 0x36 */
    hw::asm_outb(0x40, divisor & 0xFF);   /* Set low byte of divisor */
    hw::asm_outb(0x40, divisor >> 8);     /* Set high byte of divisor */
};

void install_event_handler()
{
	idt::irq_register_event_handler(0, event_handler);
    idt::irq_register_event_handler(7, nop);            //APIC timer, actually not used
};

void uninstall_event_handler()
{
	idt::irq_del_event_handler(0);
    idt::irq_del_event_handler(7);
};
}
