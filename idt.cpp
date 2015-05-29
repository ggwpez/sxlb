#include "idt.hpp"
#include "memory.hpp"

namespace idt
{
	const uchar_t* idt_isr_messages[32] =
	{
		"Division By Zero", "Debug", "Non Maskable Interrupt", "Breakpoint",
		"Into Detected Overflow", "Out of Bounds", "Invalid Opcode", "No Coprocessor",
		"Double Fault", "Coprocessor Segment Overrun", "Bad TSS", "Segment Not Present",
		"Stack Fault", "General Protection Fault - check your privileg", "Page Fault", "Unknown Interrupt",
		"Coprocessor Fault", "Alignment Check", "Machine Check", "Reserved",
		"Reserved", "Reserved", "Reserved", "Reserved",
		"Reserved", "Reserved", "Reserved", "Reserved",
		"Reserved", "Reserved", "Reserved", "Reserved"
	};

	void*  irq_functions[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	struct idt_entry idt[256];	//always 256 needed
	struct idt_ptr idt_r;		//Interrupt register
	extern "C" tss_entry tss;
	

	//If a exception_messages exists, it will be printed out.
	extern "C" void isr_fault_handler(struct task::cpu_state_t* state)
	{
		if (state->int_no < 32)	//ensure that the fired ISR is valid
			syshlt((char_t*)idt_isr_messages[state->int_no]);	//halts the system
	};

	//If a delegate exists for the called IRQ, it gets called by this handler.
	extern "C" struct task::cpu_state_t* irq_event_handler(struct task::cpu_state_t* state)
    {
		struct task::cpu_state_t* state_new = state;

		if (task::multitasking_set_enabled && state->int_no == 32)
		{
			state_new = task::schedule(state);
			//tss.esp0 = (uint32_t)(state_new + 1);
		}
		//thats a delegate with one argument (struct task::cpu_state_t*)
		void(*handler)(struct task::cpu_state_t* state);

		//Look if functionhandler for this IRQ exists
		handler = irq_functions[state->int_no - 32];
		if (handler) { handler(state); }	//### <- dont comment this
		//If the IRQ was raised by the SLAVE PIC, send EOI to SLAVE controllers
		if (state->int_no >= 40) { asm_outb(0xA0, 0x20); }

		//In both cases the MASTER PIC needs a EOI, otherwise no IRQ anymore
		asm_outb(0x20, 0x20);
		return state_new;
	};

	int load()
	{
		idt_r.limit = (sizeof (struct idt_entry) * 256) - 1;
		idt_r.base = (uint32_t)&idt;
		flush();
		irq_install();
		asm volatile("lidt %0" : "=m" (idt_r));
		return 1;
	};

	int unload()
	{
		asm("cli");
		idt_r.limit = NULL;
		idt_r.base = NULL;
		flush();
		asm("lidt 0x00");
	};

	/*Clears the memory block of the complete IDT.*/
	void flush()
	{
        memory::memset(&idt, 0, sizeof(struct idt_entry) * 256);
	};

	/*Remaps the IRQ from offset=0 to offset=31, so the IRS wont overide user IRQ.*/
	void irq_remap()
	{
		asm_outb(0x20, 0x11);	//master PIC command-port
		asm_outb(0xA0, 0x11);	//slave  PIC command-port

		//Set data Vectors
		asm_outb(0x21, 0x20);	//master PIC data-port
		asm_outb(0xA1, 0x28);	//slave PIC  data-port

		asm_outb(0x21, 0x04);
		asm_outb(0xA1, 0x02);
		asm_outb(0x21, 0x01);
		asm_outb(0xA1, 0x01);
		asm_outb(0x21, 0x00);
		asm_outb(0xA1, 0x00);
	};

	/*Calls the constructor for all IRS and IRQ*/
	void irq_install()
	{
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

		irq_remap();

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
	};

	/*Constructs an idt_entry, at the given index.*/
	void entry_set_data(uchar_t index, uint32_t base, ushort_t selector, uchar_t flags)
	{
		idt[index].base_lo = base & 0x0000ffff;
		idt[index].base_hi = (base >> 16) & 0xffff;
		//idt[index].base_hi = base & 0xffff0000;
		idt[index].selector = selector;
		idt[index].zero = NULL;
		idt[index].flags = flags | 0x60;
	};

	void irq_register_event_handler(uchar_t event_number, void(*event_handler)(struct task::cpu_state_t* state))
	{
#if __CHECKS_NLPTR
		if (!event_handler)
			syshlt("IDT event register!");
#endif
		irq_functions[event_number] = event_handler;
	};

	void irq_del_event_handler(uchar_t event_number)
	{
		irq_functions[event_number] = NULL;
	};
}
