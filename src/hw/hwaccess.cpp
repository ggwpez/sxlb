#include "hwaccess.hpp"

namespace hw
{
/*ASM 'in'. Reads a byte from the given port.*/
ubyte_t asm_inb(ushort_t port)
{
	unsigned char ret;
	asm volatile("inb %%dx,%%al":"=a" (ret) : "d" (port));
	return ret;
};
/*ASM 'in'. Reads a word from the given port.*/
uword_t asm_inw(ushort_t port)
{
	unsigned char ret;
	asm volatile("inw %%dx,%%ax":"=a" (ret) : "d" (port));
	return ret;
};
/*ASM 'in'. Reads a dword from the given port.*/
udword_t asm_ind(ushort_t port)
{
	unsigned char ret;
	asm volatile("in %%dx,%%eax":"=a" (ret) : "d" (port));
	return ret;
};
/*ASM 'out'. Writes the given byte to the given port.*/
void asm_outb(ushort_t port, ubyte_t value)
{
	asm volatile("outb %%al,%%dx": : "d" (port), "a" (value));
};
/*ASM 'out'. Writes the given word to the given port.*/
void asm_outw(ushort_t port, uword_t value)
{
	asm volatile("outw %%ax,%%dx": : "d" (port), "a" (value));
};
/*ASM 'out'. Writes the given dword to the given port.*/
void asm_outd(ushort_t port, udword_t value)
{
	asm volatile("out %%eax,%%dx": : "d" (port), "a" (value));
};
/*Gets the value from: cr0, cr2, cr3, cr4*/
int32_t asm_get_register_ctrl(uint8_t number)
{
	int32_t out = 0;

	switch (number)
	{
		case 0:
			__asm__ __volatile__("mov %%cr0, %0": "=r"(out));
			break;
		//case 1:	//CPU reserved
		//	__asm__ __volatile__("mov %%cr1, %0": "=r"(out));
		//	break;
		case 2:
			__asm__ __volatile__("mov %%cr2, %0": "=r"(out));
			break;
		case 3:
			__asm__ __volatile__("mov %%cr3, %0": "=r"(out));
			break;
		case 4:
			__asm__ __volatile__("mov %%cr4, %0": "=r"(out));
			break;
	}

	return out;
};
/*Sets the values for: cr0, cr2, cr3, cr4*/
void asm_set_register_ctrl(uint8_t number, uint32_t value)
{
	switch (number)
	{
		case 0:
			__asm__ __volatile__("mov %0, %%cr0":: "r"(value)); // write cr0
			break;
		/*case 1:
			__asm__ __volatile__("mov %0, %%cr1":: "r"(value)); // write cr1
			break;*/
		case 2:
			__asm__ __volatile__("mov %0, %%cr2":: "r"(value)); // write cr2
			break;
		case 3:
			__asm__ __volatile__("mov %0, %%cr3":: "r"(value)); // write cr3
			break;
		case 4:
			__asm__ __volatile__("mov %0, %%cr4":: "r"(value)); // write cr4
			break;
	}
};


int32_t asm_get_register_eax()
{ };    //eax returns itself

int32_t asm_get_register_ebx()
{
	int32_t out;
	asm volatile("mov %%ebx, %0": "=r"(out));
	return out;
};

int32_t asm_get_register_ecx()
{
	int32_t out;
	asm volatile("mov %%ecx, %0": "=r"(out));
	return out;
};

int32_t asm_get_register_edx()
{
	int32_t out;
	asm volatile("mov %%edx, %0": "=r"(out));
	return out;
};

int32_t asm_get_register_esi()
{
	int32_t out;
	asm volatile("mov %%esi, %0": "=r"(out));
	return out;
};

int32_t asm_get_register_edi()
{
	int32_t out;
	asm volatile("mov %%edi, %0": "=r"(out));
	return out;
};

int32_t asm_get_register_esp()
{
	int32_t out;
	asm volatile("mov %%esp, %0": "=r"(out));
	return out;
};

int32_t asm_get_register_ebp()
{
	int32_t out;
	asm volatile("mov %%ebp, %0": "=r"(out));
	return out;
};

int32_t asm_get_register_eip()
{
	int32_t out;
	__asm__ volatile ("call 1f \n\t"	//cant be accessed directly, call smth (eip gets pushd), and then just pop
		"1: pop %0" : "=r"(out));
	return out;
};
}
