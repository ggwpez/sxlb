#pragma once

#include "../types.hpp"
#include "../convert.hpp"
#include "keyboard.hpp"

#define CMOS_ADDRESS    0x70
#define CMOS_DATA       0x71

//offsets for reading specific values
#define CMOS_OFFSET_SECONDS     0x00
#define CMOS_OFFSET_SECONDS_ARL 0x01
#define CMOS_OFFSET_MINUTES     0x02
#define CMOS_OFFSET_MINUTES_ALR 0x03
#define CMOS_OFFSET_HOURS       0x04
#define CMOS_OFFSET_HOURS_ALR   0x05
#define CMOS_OFFSET_DAY_OF_WEAK 0x06
#define CMOS_OFFSET_DAY_OF_MONTH 0x07
#define CMOS_OFFSET_MONTH       0x08
#define CMOS_OFFSET_YEAR        0x09

#define CMOS_OFFSET_CENTURY     0x48
#define CMOS_OFFSET_DATE_ALARM  0x49
#define CMOS_OFFSET_MODELL_NR   0x40
#define CMOS_OFFSET_SERIAL_NR_1 0x41
#define CMOS_OFFSET_SERIAL_NR_2 CMOS_OFFSET_SERIAL_NR_1 +1
#define CMOS_OFFSET_SERIAL_NR_3 CMOS_OFFSET_SERIAL_NR_2 +1
#define CMOS_OFFSET_SERIAL_NR_4 CMOS_OFFSET_SERIAL_NR_3 +1
#define CMOS_OFFSET_SERIAL_NR_5 CMOS_OFFSET_SERIAL_NR_4 +1
#define CMOS_OFFSET_SERIAL_NR_6 CMOS_OFFSET_SERIAL_NR_5 +1


namespace io
{
    /*ASM 'in'. Reads a byte from the given port.*/
    ubyte_t asm_inb(ushort_t port);
    /*ASM 'in'. Reads a word from the given port.*/
    uword_t asm_inw(ushort_t port);
    /*ASM 'in'. Reads a dword from the given port.*/
    udword_t asm_ind(ushort_t port);

    /*ASM 'out'. Writes the given byte to the given port.*/
    void asm_outb(ushort_t port, ubyte_t value);
    /*ASM 'out'. Writes the given word to the given port.*/
    void asm_outw(ushort_t port, uword_t value);
    /*ASM 'out'. Writes the given word to the given port.*/
    void asm_outd(ushort_t port, udword_t value);

    /*ASM 'mov'. Reads a value from the selected controll register {0,2,3,4}, 1 is reserved.*/
    int32_t asm_get_register_ctrl(uint8_t number);
    /*Sets the values for: cr0, cr2, cr3, cr4*/
    void asm_set_register_ctrl(uint8_t number, uint32_t value);

    /*ASM 'mov'. Reads a value from eax.*/
    int32_t asm_get_register_eax();
    /*ASM 'mov'. Reads a value from ebx.*/
    int32_t asm_get_register_ebx();
    /*ASM 'mov'. Reads a value from ecx.*/
    int32_t asm_get_register_ecx();
    /*ASM 'mov'. Reads a value from edx.*/
    int32_t asm_get_register_edx();

    /*ASM 'mov'. Reads a value from esi.*/
    int32_t asm_get_register_esi();
    /*ASM 'mov'. Reads a value from edi.*/
    int32_t asm_get_register_edi();
    /*ASM 'mov'. Reads a value from esp.*/
    int32_t asm_get_register_esp();
    /*ASM 'mov'. Reads a value from ebp.*/
    int32_t asm_get_register_ebp();
    /*ASM 'mov'. Reads a value from eip.*/
    int32_t asm_get_register_eip();

    /* these two version wait some time after register selection, so CMOS should
     * have enougth time before reading/writing. */
    uint8_t cmos_read(uint8_t offset);
    void    cmos_write(uint8_t offset, uint8_t calue);
}
