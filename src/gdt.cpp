#include "gdt.hpp"
#include "memory/memory.hpp"
#include "ui/textmode.hpp"

#define GDT_ENTRYS 6

namespace gdt
{
    static gdt_entry gdt_entrys[GDT_ENTRYS];
    static gdt_ptr gdt;

    extern "C"
    {
        extern void gdt_flush(uint32_t gdt);
        extern void tss_flush();
    }
    tss_entry tss;

    void init()
    {
        logINF("writing all %u gdt entrys...", GDT_ENTRYS -1);
        gdt.limit = (sizeof(gdt_entry) * GDT_ENTRYS) - 1;
        gdt.base = (uint32_t)&gdt_entrys;

        gate_set_data(0, 0, 0, 0, 0);	//null desc
        gate_set_data(1, 0, 0xffffffff, 0x9a, 0xCF);
        gate_set_data(2, 0, 0xffffffff, 0x92, 0xCF);
        gate_set_data(3, 0, 0xffffffff, 0xFA, 0xCF); // usermode code segment
        gate_set_data(4, 0, 0xffffffff, 0xF2, 0xCF); // usermode data segment
        logDONE; logINF("writing tss...");

        write_tss(5, 0x10, 0);
        logDONE; logINF("flusing gdt...");
        gdt_flush(&gdt);
        logDONE; logINF("loading tss...");
        asm volatile("ltr %%ax" : : "a" (0x2b));
        logDONE;
    };

    void reload()
    {
        asm volatile("lgdt %0" : "=m" (gdt));
    };

    void write_tss(int num, uint16_t ss0, uint32_t esp0)
    {
        uint32_t base = (uint32_t)&tss;
        uint32_t limit = sizeof(tss_entry);  //TODO: look it the +base is right

        gate_set_data(num, base, limit, 0xE9, 0x00); //access = 0xE9, granularity = 0x0
        memory::memset(&tss, 0, sizeof(tss_entry));

        tss.ss0 = ss0;      // kernel stack segment
        tss.esp0 = esp0;    // kernel stack pointer

        tss.cs = 0x08;
        tss.ss = tss.ds = tss.es = tss.fs = tss.gs = 0x10;
    };

    void gate_set_data(uint32_t index, uint32_t base_address, uint32_t limit, uint8_t access, uint8_t granularity)
    {
        gdt_entrys[index].base_low = base_address & 0xffff;
        gdt_entrys[index].base_mid = (base_address >> 16) & 0xff;
        gdt_entrys[index].base_height = (base_address >> 24) & 0xff;

        gdt_entrys[index].limit_low = limit & 0x0000ffff;
        gdt_entrys[index].granularity = (limit >> 16) & 0xf;

        gdt_entrys[index].granularity |= (granularity & 0xf0);
        gdt_entrys[index].access = access;
    };
}
