#include "gdt.hpp"
#include "memory/memory.hpp"

#define GDT_FLAG_DATASEG 0x02
#define GDT_FLAG_CODESEG 0x0a
#define GDT_FLAG_TSS     0x09
#define GDT_FLAG_SEGMENT 0x10
#define GDT_FLAG_RING0   0x00
#define GDT_FLAG_RING3   0x60
#define GDT_FLAG_PRESENT 0x80
#define GDT_FLAG_4K      0x800
#define GDT_FLAG_32_BIT  0x400

#define GDT_ENTRYS 6
cc
static struct gdt_entry gdt_entrys[GDT_ENTRYS];
static struct gdt_ptr gdt;

extern "C"
{
	extern void gdt_flush(uint32_t gdt);
	extern void tss_flush();
}
tss_entry tss;

void sxlb_gdt_load()
{
	gdt.limit = (sizeof(struct gdt_entry) * GDT_ENTRYS) - 1;
	gdt.base = (uint32_t)&gdt_entrys;

	sxlb_gdt_gate_set_data(0, 0, 0, 0, 0);	//null desc
	sxlb_gdt_gate_set_data(1, 0, 0xffffffff, 0x9a, 0xcf);
	sxlb_gdt_gate_set_data(2, 0, 0xffffffff, 0x92, 0xcf);
	//sxlb_gdt_gate_set_data(3, 0, 0xffffffff, 0xfa, 0xcf);
	//sxlb_gdt_gate_set_data(4, 0, 0xffffffff, 0xf2, 0xcf);
	//sxlb_gdt_gate_set_data(5, tss, 4, 0xf2, 0xcf);
    //write_tss(3, 0x10, 0x0);

	gdt_flush(&gdt);
    //tss_flush();
	//sxlb_gdt_reload();
	//asm volatile("ltr %%ax" : : "a" (0x2B));
};

void sxlb_gdt_reload()
{
	asm volatile("lgdt %0" : "=m" (gdt));
};

void write_tss(int num, uint16_t ss0, uint32_t esp0)
{
	uint32_t base = (uint32_t)&tss;
	uint32_t limit = sizeof(tss_entry);

	sxlb_gdt_gate_set_data(num, base, limit, 0xE9, 0x00); //access = 0xE9, granularity = 0x0
    memory::memset(&tss, 0, sizeof(tss_entry));

	tss.ss0 = ss0;  // Set the kernel stack segment.
	tss.esp0 = esp0; // Set the kernel stack pointer.

	tss.cs = 0x08;
	tss.ss = tss.ds = tss.es = tss.fs = tss.gs = 0x10;
};

void sxlb_gdt_gate_set_data(uint32_t index, uint32_t base_address, uint32_t limit, uint8_t access, uint8_t granularity)
{
	gdt_entrys[index].base_low = base_address & 0x0000ffff;
	gdt_entrys[index].base_mid = base_address & 0x00ff0000;
	gdt_entrys[index].base_height = base_address & 0xff000000;

	gdt_entrys[index].limit_low = limit & 0x0000ffff;
	gdt_entrys[index].granularity = limit & 0x000f0000;

	gdt_entrys[index].granularity |= (granularity & 0xf0);
	gdt_entrys[index].access = access;
};
kk
