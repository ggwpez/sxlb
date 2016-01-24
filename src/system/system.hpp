#pragma once

#include "../types.hpp"
#include "../ui/textmode.hpp"
#include "../io/hwaccess.hpp"

#define sti __asm__ __volatile__("sti");
#define cli __asm__ __volatile__("cli");
#define syshlt(x) (system::sxlb_system_halt(x))

typedef enum
{
    CPUID_CPU_COMPANY_NAME	= 0x00000000,
    CPUID_CPU_INFO			= 0x00000001,
} CPUID_REQUEST_TYPE;

/*ASM call to test, if the CPU supports CPUID.*/
extern uint32_t CPU_CPUID_test();
/*ASM call to perform a CPUID request.*/
extern void CPU_CPUID_request(CPUID_REQUEST_TYPE request_type, char_t* string);

namespace system
{
    void reboot();
    /*Dumps all register values.*/
    void sxlb_system_dumb();
    /*Calls dump, and halts the CPU.*/
    void sxlb_system_halt(char_t* error_msg) __attribute__((noreturn));

    /*Call to get information about the CPU*/
    void sxlb_system_cpu_info(CPUID_REQUEST_TYPE request_test, char_t* buffer);
    /*Returns the address, where the kernel ends.*/

    uint32_t kernel_start_address();
    uint32_t kernel_end_address();
}
