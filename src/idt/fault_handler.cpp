#include "fault_handler.hpp"

#define PROTECTED_NOT_PRESENT   B(00000001)
#define WRITE_NOT_READ          B(00000010)
#define US_NOT_SV               B(00000100)
#define RESERVED                B(00001000)
#define EXECUTION_PROHIBITED    B(00010000)

namespace idt
{
    bool page_fault_handler(task::cpu_state_t* state, char* kill_msg)
    {
        uint32_t err = state->error;

        sprintf_s(kill_msg, 64, "Page fault @%x (%s|%s|%s|%s|%s)",
                  io::asm_get_register_ctrl(2),
                  err & PROTECTED_NOT_PRESENT ? "Protected" : "Not Present",
                  err & WRITE_NOT_READ ? "Write" : "Read",
                  err & US_NOT_SV ? "User" : "Su User",
                  err & RESERVED ? "Reserved" : "0",
                  err & EXECUTION_PROHIBITED ? "Exec illegal" : "0");

        return true;
    }

    //13
    bool gpf_handler(task::cpu_state_t* state, char* kill_msg)
    {
        sprintf_s(kill_msg, 64, "General Protection fault");
        return true;
    }
}
