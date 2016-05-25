#include "fault_handler.hpp"
#include "idt.hpp"
#include "../system/cpu.hpp"

#define PROTECTED_NOT_PRESENT   B(00000001)
#define WRITE_NOT_READ          B(00000010)
#define US_NOT_SV               B(00000100)
#define RESERVED                B(00001000)
#define EXECUTION_PROHIBITED    B(00010000)

namespace idt
{
    //no interrupt equivalent
    task::cpu_state_t* nullptr_handler(task::cpu_state_t* state)
    {
        char buffer[64];

        sprintf_s(buffer, sizeof(buffer), "Nullptr access");
        idt::critical(state, buffer);

        return state;
    }

    task::cpu_state_t* zerodiv_handler(task::cpu_state_t* state)
    {
        char buffer[64];

        sprintf_s(buffer, 64, "Division by zero");
        idt::critical(state, buffer);

        return state;
    }

    //14
    task::cpu_state_t* pf_handler(task::cpu_state_t* state)
    {
        char buffer[64];
        uint32_t err = state->error;
        uint32_t faulting_address = system::cpu_get_register_ctrl(2);

        if (!faulting_address)
            return nullptr_handler(state);

        faulting_address = faulting_address -(faulting_address %PAGE_SIZE);
        alloc_frame(get_page(faulting_address, 1, kernel_directory), 1, 1);
        //map_id(faulting_address, faulting_address +PAGE_SIZE);

        //logtINF("PF @0x%x\n", faulting_address);

        sprintf_s(buffer, sizeof(buffer), "Page fault 0x%x (%s|%s|%s|%s|%s)",
                  faulting_address,
                  err & PROTECTED_NOT_PRESENT ? "Protected" : "Not Present",
                  err & WRITE_NOT_READ ? "Write" : "Read",
                  err & US_NOT_SV ? "User" : "Su User",
                  err & RESERVED ? "Reserved" : "0",
                  err & EXECUTION_PROHIBITED ? "Exec illegal" : "0");

        //idt::critical(state, buffer);

        return state;
    }

    //13
    task::cpu_state_t* gpf_handler(task::cpu_state_t* state)
    {
        char buffer[64];

        sprintf_s(buffer, sizeof(buffer), "General Protection fault");
        idt::critical(state, buffer);

        return state;
    }

    //2
    task::cpu_state_t* nmi_handler(task::cpu_state_t* state)
    {
        char buffer[64];

        sprintf_s(buffer, sizeof(buffer), "NMI: Severe hardware failure.");
        idt::critical(state, buffer);

        return state;
    }

    //8
    task::cpu_state_t* df_handler(task::cpu_state_t* state)
    {
        char buffer[64];

        sprintf_s(buffer, sizeof(buffer), "Double fault");
        idt::critical(state, buffer);

        return state;
    }
}
