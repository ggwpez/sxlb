#pragma once

#include "../types.hpp"
#include "../system/system.hpp"
#include "../ui/textmode.hpp"
#include "../task/task.hpp"
#include "../gdt.hpp"
#include "../memory/memory.hpp"

namespace idt
{
    struct idt_entry		//IDT entry, 256 in all
    {
        ushort_t base_lo;	//base adress 0-15 bit
        ushort_t selector;  //code segment selector in GDT or LDT
        uchar_t zero;
        uchar_t flags;		//permission flags
        ushort_t base_hi;	//base adress 16-32 bit
    }__attribute__((packed));

    struct idt_ptr
    {
        ushort_t limit;
        uint32_t base;
    }__attribute__((packed));

    typedef task::cpu_state_t*(*isr_delegate)(task::cpu_state_t*);
    typedef void(*irq_delegate)(task::cpu_state_t*);

    /*Call in kernel load up, to set up IDT (IRSs+IRQs)*/
    int load();
    /*Call in kernel shut down, to unload IDT (IRSs+IRQs). Not necessary*/
    int unload();
    /*Call to register a Hardware Interrupt Handler*/
    void isr_register_event_handler(uchar_t event_number, isr_delegate handler);
    /*Call to remove a Hardware Interrupt Handler*/
    void isr_del_event_handler(uchar_t index);
    /*Call to register a Software Interrupt Handler*/
    void irq_register_event_handler(uchar_t index, irq_delegate handler);
    /*Call to remove a Software Interrupt Handler*/
    void irq_del_event_handler(uchar_t index);
    /*If multitasking is enabled, the task dies. Otherwise syshlt.*/
    void critical(task::cpu_state_t* state, char* msg);

    /*Dont call manually, unless you know what you do. Docu in .cpp*/
    void irq_install();
    extern "C"
    {
        /*Dont call manually, unless you know what you do. Docu in .cpp*/
        struct cpu_state_t* sxlb_irq_event_handler(struct task::cpu_state_t* state);
        /*Dont call manually, unless you know what you do. Docu in .cpp*/
        void isr_fault_handler(struct task::cpu_state_t* state);
    }
    /*Dont call manually, unless you know what you do. Docu in .cpp*/
    void entry_set_data(uchar_t index, uint32_t base, ushort_t selector, uchar_t flags);
    /*Dont call manually, unless you know what you do. Docu in .cpp*/
    void irq_remap();
    /*Dont call manually, unless you know what you do. Docu in .cpp*/
    void flush();
    /*Dont call manually, unless you know what you do. Functionality selfexplaining.*/
    void isr_write_error(uint32_t error_number);
    extern "C"
    {
        extern void isr0();
        extern void isr1();
        extern void isr2();
        extern void isr3();
        extern void isr4();
        extern void isr5();
        extern void isr6();
        extern void isr7();
        extern void isr8();
        extern void isr9();
        extern void isr10();
        extern void isr11();
        extern void isr12();
        extern void isr13();
        extern void isr14();
        extern void isr15();
        extern void isr16();
        extern void isr17();
        extern void isr18();
        extern void isr19();
        extern void isr20();
        extern void isr21();
        extern void isr22();
        extern void isr23();
        extern void isr24();
        extern void isr25();
        extern void isr26();
        extern void isr27();
        extern void isr28();
        extern void isr29();
        extern void isr30();
        extern void isr31();

        extern void isr126();
        extern void isr127();

        extern void irq0();
        extern void irq1();
        extern void irq2();
        extern void irq3();
        extern void irq4();
        extern void irq5();
        extern void irq6();
        extern void irq7();
        extern void irq8();
        extern void irq9();
        extern void irq10();
        extern void irq11();
        extern void irq12();
        extern void irq13();
        extern void irq14();
        extern void irq15();
    }
}
