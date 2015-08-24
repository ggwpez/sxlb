#pragma once

#include "types.hpp"

struct gdt_entry
{
	uint16_t limit_low;
	uint16_t base_low;		//0-15 bit
	uint8_t base_mid;		//16-23 bit
	uint8_t access;
	uint8_t granularity;	//0-3 segment_length, 4-7 Flags
	uint8_t base_height;	//24-32 bit
}__attribute__((packed));

struct gdt_ptr
{
	uint16_t limit;
	uint32_t base;
}__attribute__((packed));

struct tss_entry
{
    uint16_t prev_tss;
    uint16_t reserved10;

    uint32_t  esp0;

    uint16_t ss0;
    uint16_t reserved09;

    uint32_t  esp1;

    uint16_t ss1;
    uint16_t reserved08;

    uint32_t  esp2;

    uint16_t ss2;
    uint16_t reserved07;

    uint32_t  cr3;
    uint32_t  eip;
    uint32_t  eflags;
    uint32_t  eax;
    uint32_t  ecx;
    uint32_t  edx;
    uint32_t  ebx;
    uint32_t  esp;
    uint32_t  ebp;
    uint32_t  esi;
    uint32_t  edi;

    uint16_t es;
    uint16_t reserved06;

    uint16_t cs;
    uint16_t reserved05;

    uint16_t ss;
    uint16_t reserved04;

    uint16_t ds;
    uint16_t reserved03;

    uint16_t fs;
    uint16_t reserved02;

    uint16_t gs;
    uint16_t reserved01;

    uint16_t ldt;
    uint16_t reserved00;

    uint16_t trap; //bit0 only! bit1...15 reserved
    uint16_t iomap_base;
}__attribute__((packed));

void sxlb_gdt_load();
void sxlb_gdt_reload();
void write_tss(int num, uint16_t ss0, uint32_t esp0);
void sxlb_gdt_gate_set_data(uint32_t index, uint32_t base_address, uint32_t limit, uint8_t access, uint8_t granularity);
