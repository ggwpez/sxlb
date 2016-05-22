#include "fault_handler.hpp"

#define PROTECTED_NOT_PRESENT   B(00000001)
#define WRITE_NOT_READ          B(00000010)
#define US_NOT_SV               B(00000100)
#define RESERVED                B(00001000)
#define EXECUTION_PROHIBITED    B(00010000)

namespace idt
{
    //no interrupt equivalent
    bool nullptr_handler(task::cpu_state_t* state, char* kill_msg)
    {
        sprintf_s(kill_msg, 64, "Nullptr access");
        return true;
    }

    bool zerodiv_handler(task::cpu_state_t* state, char* kill_msg)
    {
        sprintf_s(kill_msg, 64, "Division by zero");
        return true;
    }

    //14
    bool pf_handler(task::cpu_state_t* state, char* kill_msg)
    {
        uint32_t err = state->error;
        uint32_t faulting_address = io::asm_get_register_ctrl(2);

        if (!faulting_address)
            return nullptr_handler(state, kill_msg);

        faulting_address = faulting_address -(faulting_address %PAGE_SIZE);
        alloc_frame(get_page(faulting_address, 1, kernel_directory), 1, 1);

        //logtINF("PF @0x%x\n", faulting_address);

        sprintf_s(kill_msg, 64, "Page fault 0x%x (%s|%s|%s|%s|%s)",
                  faulting_address,
                  err & PROTECTED_NOT_PRESENT ? "Protected" : "Not Present",
                  err & WRITE_NOT_READ ? "Write" : "Read",
                  err & US_NOT_SV ? "User" : "Su User",
                  err & RESERVED ? "Reserved" : "0",
                  err & EXECUTION_PROHIBITED ? "Exec illegal" : "0");

        return false;
    }

    //13
    bool gpf_handler(task::cpu_state_t* state, char* kill_msg)
    {
        sprintf_s(kill_msg, 25, "General Protection fault");
        return true;
    }

    //2
    bool nmi_handler(task::cpu_state_t* state, char* kill_msg)
    {
        sprintf_s(kill_msg, 64, "NMI: Severe hardware failure.");
        return true;
    }

    //8
    bool df_handler(task::cpu_state_t* state, char* kill_msg)
    {
        sprintf_s(kill_msg, 64, "Double fault");
        return true;
    }
}
