#include "timer.hpp"

namespace time
{
    uint32_t ticks = 0;

    void init()
    {
        //date_init();
        install_event_handler();
        set_frequenze(HZ);
    };

    void uninstall()
    {
        uninstall_event_handler();
    };

    uint32_t get_ms()
    {
        return ticks;
    }

    uint32_t get_seconds()
    {
        return ticks /HZ;///HZ;
    };

    void event_handler(task::cpu_state_t* state)
    {
        ticks++;
    };

    void set_frequenze(int32_t hz)
    {
        logtINF("setting pic frequenze...");
        if (!hz)
           return;

        int32_t divisor = 1193182 / hz;                 /* Calculate our divisor */

        system::outb(0x43, 0x36);                       /* Set our command byte 0x36 */
        system::outb(0x40, divisor & 0xFF);             /* Set low byte of divisor */
        system::outb(0x40, (divisor >> 8) & 0xff);      /* Set high byte of divisor */
        logINF("(%u hz)", hz); logDONE;
    };

    void install_event_handler()
    {
        logtINF("registering pic irq...");
        idt::irq_register_event_handler(0, event_handler);
        //idt::irq_register_event_handler(7, nop);            //APIC timer, actually not used
        logDONE;
    };

    void uninstall_event_handler()
    {
        idt::irq_del_event_handler(0);
        //idt::irq_del_event_handler(7);
    };
}

