#include "idt.hpp"
#include "fault_handler.hpp"

#ifdef __LP64__
#pragma error This is 32bit asm code, but you assume its 64bit.
#endif

using namespace system;
namespace idt
{
    const uchar_t* idt_isr_messages[32] =
    {
        "Division By Zero", "Debug", "Non Maskable Interrupt", "Breakpoint",
        "Into Detected Overflow", "Out of Bounds", "Invalid Opcode", "No Coprocessor",
        "Double Fault", "Coprocessor Segment Overrun", "Bad TSS", "Segment Not Present",
        "Stack Fault", "General Protection Fault", "Page Fault", "Unknown Interrupt",
        "Coprocessor Fault", "Alignment Check", "Machine Check", "SIMD FPU Fault",
        "Virtualization Fault", "Reserved", "Security Fault", "Nullptr Exception",
        "Triple Fault", "Reserved", "Reserved", "Reserved",
        "Reserved", "Reserved", "Reserved", "Reserved"
    };

    void*  irq_functions[128] = { 0 };
    void*  isr_functions[128] = { 0 };
    idt_entry idt[256];	//always 256 needed
    idt_ptr idt_r;		//Interrupt register
    gdt::tss_entry tss;


    task::cpu_state_t* isr_handler(task::cpu_state_t* state)
    {
        isr_delegate handler = isr_functions[state->int_no];

        char buffer[128] = { 0 };
        if (handler)
        {
            return handler(state);
        }
        else
        {
            if (task::multitasking_get())
            {
                printfl("Task '%u' killed for unhandled '%s'.", task::get_pid(), idt_isr_messages[state->int_no]);
                task::end(-1);                    //no handler, and it dies too
            }
            else
            {
                sprintf_s(buffer, COUNTOF(buffer), "unhandled ISR #%u: %s", state->int_no, idt_isr_messages[state->int_no]);
                syshlt(buffer);	//halts the system
            }
        }
    };

    task::cpu_state_t* irq_handler(task::cpu_state_t* state)
    {
        task::cpu_state_t* state_new = state;

        if (state->int_no == 32 && task::multitasking_get())
        {
            state_new = task::schedule(state);
        }

        irq_delegate handler = irq_functions[state->int_no - 32];

        if (handler)
            handler(state);

        return state_new;
    };

    //Decides if its an ISR or IRQ
    extern "C" struct task::cpu_state_t* ir_event_handler(struct task::cpu_state_t* state)
    {
        if (state->int_no >= 32 && state->int_no < 48)
            state = irq_handler(state);
        else
            state = isr_handler(state);

        if (state->int_no >= 40) { outb(0xA0, 0x20); }      //send all ok too master and slave PIC
        outb(0x20, 0x20);

        return state;
    };

    int load()
    {
        idt_r.limit = (sizeof(idt_entry) * 256) - 1;
        idt_r.base = (uint32_t)&idt;
        flush();
        irq_install();
        logtINF("loading idt...");
        asm volatile("lidt %0" : "=m" (idt_r));

        logDONE; logtINF("registering default isrs...(2,13,14)");
        isr_register_event_handler( 0, zerodiv_handler);
        isr_register_event_handler( 2, nmi_handler);
        isr_register_event_handler(13, gpf_handler);
        isr_register_event_handler(14, pf_handler);
        isr_register_event_handler( 8, df_handler);
        logDONE;
        return 1;
    };


    void critical(task::cpu_state_t *state, char* msg)
    {
        if (task::multitasking_get())//&& task::get_rpl() != 0)
        {
            printfl("Task '%u' killed for '%s'.", task::get_pid(), msg);
            task::end(-1);
        }
        else
        {
            char buffer[128];
            sprintf_s(buffer, COUNTOF(buffer), "ISR #%u: %s", state->int_no, msg);
            syshlt(buffer);	//halts the system
        }
    }

    int unload()
    {
        asm("cli");
        idt_r.limit = NULL;
        idt_r.base = NULL;
        flush();
        asm("lidt 0x00");
        return 1;
    };

    /*Clears the memory block of the complete IDT.*/
    void flush()
    {
        memory::memset(&idt, 0, sizeof(idt_entry) * 256);
    };

    /*Remaps the IRQ from offset=0 to offset=31, so the IRS wont overide user IRQ.*/
    void irq_remap()
    {
        logtINF("remapping pic1...(to 40)");
        outb(0x20, 0x11);	//master PIC command-port
        outb(0xA0, 0x11);	//slave  PIC command-port

        //Set data Vectors
        outb(0x21, 0x20);	//master PIC data-port
        outb(0xA1, 0x28);	//slave PIC  data-port

        outb(0x21, 0x04);
        outb(0xA1, 0x02);
        outb(0x21, 0x01);
        outb(0xA1, 0x01);
        outb(0x21, 0x00);
        outb(0xA1, 0x00);
        logDONE;
    };

    /*Calls the constructor for all IRS and IRQ*/
    void irq_install()
    {
        logtINF("setting isrs vectors...(0-31,126,127)");
        entry_set_data(0, (uint32_t)isr0, 0x08, 0x8E);
        entry_set_data(1, (uint32_t)isr1, 0x08, 0x8E);
        entry_set_data(2, (uint32_t)isr2, 0x08, 0x8E);
        entry_set_data(3, (uint32_t)isr3, 0x08, 0x8E);
        entry_set_data(4, (uint32_t)isr4, 0x08, 0x8E);
        entry_set_data(5, (uint32_t)isr5, 0x08, 0x8E);
        entry_set_data(6, (uint32_t)isr6, 0x08, 0x8E);
        entry_set_data(7, (uint32_t)isr7, 0x08, 0x8E);
        entry_set_data(8, (uint32_t)isr8, 0x08, 0x8E);
        entry_set_data(9, (uint32_t)isr9, 0x08, 0x8E);
        entry_set_data(10, (uint32_t)isr10, 0x08, 0x8E);
        entry_set_data(11, (uint32_t)isr11, 0x08, 0x8E);
        entry_set_data(12, (uint32_t)isr12, 0x08, 0x8E);
        entry_set_data(13, (uint32_t)isr13, 0x08, 0x8E);
        entry_set_data(14, (uint32_t)isr14, 0x08, 0x8E);
        entry_set_data(15, (uint32_t)isr15, 0x08, 0x8E);
        entry_set_data(16, (uint32_t)isr16, 0x08, 0x8E);
        entry_set_data(17, (uint32_t)isr17, 0x08, 0x8E);
        entry_set_data(18, (uint32_t)isr18, 0x08, 0x8E);
        entry_set_data(19, (uint32_t)isr19, 0x08, 0x8E);
        entry_set_data(20, (uint32_t)isr20, 0x08, 0x8E);
        entry_set_data(21, (uint32_t)isr21, 0x08, 0x8E);
        entry_set_data(22, (uint32_t)isr22, 0x08, 0x8E);
        entry_set_data(23, (uint32_t)isr23, 0x08, 0x8E);
        entry_set_data(24, (uint32_t)isr24, 0x08, 0x8E);
        entry_set_data(25, (uint32_t)isr25, 0x08, 0x8E);
        entry_set_data(26, (uint32_t)isr26, 0x08, 0x8E);
        entry_set_data(27, (uint32_t)isr27, 0x08, 0x8E);
        entry_set_data(28, (uint32_t)isr28, 0x08, 0x8E);
        entry_set_data(29, (uint32_t)isr29, 0x08, 0x8E);
        entry_set_data(30, (uint32_t)isr30, 0x08, 0x8E);
        entry_set_data(31, (uint32_t)isr31, 0x08, 0x8E);

        entry_set_data(126,(uint32_t)isr126,0x08, 0x8E);    //will be rescheduling interrupt
        entry_set_data(127,(uint32_t)isr127,0x08, 0x8E);    //hopefully syscalls
        logDONE;
        irq_remap();

        logtINF("setting irqs vectors...(0,15)");
        entry_set_data(32, (uint32_t)irq0, 0x08, 0x8E);
        entry_set_data(33, (uint32_t)irq1, 0x08, 0x8E);
        entry_set_data(34, (uint32_t)irq2, 0x08, 0x8E);
        entry_set_data(35, (uint32_t)irq3, 0x08, 0x8E);
        entry_set_data(36, (uint32_t)irq4, 0x08, 0x8E);
        entry_set_data(37, (uint32_t)irq5, 0x08, 0x8E);
        entry_set_data(38, (uint32_t)irq6, 0x08, 0x8E);
        entry_set_data(39, (uint32_t)irq7, 0x08, 0x8E);
        entry_set_data(40, (uint32_t)irq8, 0x08, 0x8E);
        entry_set_data(41, (uint32_t)irq9, 0x08, 0x8E);
        entry_set_data(42, (uint32_t)irq10, 0x08, 0x8E);
        entry_set_data(43, (uint32_t)irq11, 0x08, 0x8E);
        entry_set_data(44, (uint32_t)irq12, 0x08, 0x8E);
        entry_set_data(45, (uint32_t)irq13, 0x08, 0x8E);
        entry_set_data(46, (uint32_t)irq14, 0x08, 0x8E);
        entry_set_data(47, (uint32_t)irq15, 0x08, 0x8E);
        logDONE;
    };

    /*Constructs an idt_entry, at the given index.*/
    void entry_set_data(uchar_t index, uint32_t base, ushort_t selector, uchar_t flags)
    {
        idt[index].base_lo = base & 0xffff;
        idt[index].base_hi = (base >> 16) & 0xffff;
        idt[index].selector = selector;
        idt[index].zero = 0;
        idt[index].flags =  flags | 0x60;
    };

    void isr_register_event_handler(uchar_t event_number, isr_delegate handler)
    {
#if __CHECKS_NLPTR
        if (!handler)
            syshlt("IDT event register!");
#endif
        if (event_number > 127)
            return;

        isr_functions[event_number] = handler;
    };

    void isr_del_event_handler(uchar_t event_number)
    {
        if (event_number > 127)
            return;

        isr_functions[event_number] = NULL;
    };

    void irq_register_event_handler(uchar_t event_number, irq_delegate handler)
    {
#if __CHECKS_NLPTR
        if (!handler)
            syshlt("IDT event register!");
#endif
        if (event_number > 127)
        {
            syshlt("registered irq number too hight");
            return;
        }

        irq_functions[event_number] = handler;
    };

    void irq_del_event_handler(uchar_t event_number)
    {
        if (event_number > 127)
            return;

        irq_functions[event_number] = NULL;
    };
}
