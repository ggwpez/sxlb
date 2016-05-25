#pragma once

#include "../types.hpp"
#include "../convert.hpp"
#include "../io/keyboard.hpp"

namespace system
{
    /*ASM 'in'. Reads a byte from the given port.*/
    uint8_t inb(uint16_t port);
    /*ASM 'in'. Reads a word from the given port.*/
    uint16_t inw(uint16_t port);
    /*ASM 'in'. Reads a dword from the given port.*/
    uint32_t ind(uint16_t port);

    /*ASM 'out'. Writes the given byte to the given port.*/
    void outb(uint16_t port, uint8_t value);
    /*ASM 'out'. Writes the given word to the given port.*/
    void outw(uint16_t port, uint16_t value);
    /*ASM 'out'. Writes the given word to the given port.*/
    void outd(uint16_t port, uint32_t value);
}
