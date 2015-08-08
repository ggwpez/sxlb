#pragma once

#include "../types.hpp"
#include "../convert.hpp"

namespace hw
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
}
