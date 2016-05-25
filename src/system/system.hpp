#pragma once

#include "../types.hpp"
#include "../ui/textmode.hpp"

#define sti __asm__ __volatile__("sti");
#define cli __asm__ __volatile__("cli");
#define syshlt(x) (system::sxlb_system_halt(x))

namespace system
{
    void reboot();
    /*Dumps all register values.*/
    void sxlb_system_dumb();
    /*Calls dump, and halts the CPU.*/
    void sxlb_system_halt(char_t* error_msg) __attribute__((noreturn));

    /*Returns the address, where the kernel ends.*/
    uint32_t kernel_start_address();
    uint32_t kernel_end_address();
}
