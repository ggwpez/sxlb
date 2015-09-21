#include "syscall.hpp"

namespace system
{
    void init()
    {
        logINF("registering syscall isr...");
        idt::isr_register_event_handler(127, syscall_event_handler);
        logDONE;
    };

    void dump(task::cpu_state_t* state)
    {
        printfl("eax=%u \n\
                ebx=%u \n\
                ecx=%u \n\
                edx=%u \n\
                esi=%u \n\
                edi=%u", state->eax, state->ebx, state->ecx, state->edx, state->esi, state->edi);
    }

    bool syscall_event_handler(task::cpu_state_t* state, char* kill_msg)
    {
        if (state->eax > SYSCNUM_CALL_ENTRYS)
        {
            sprintf_s(kill_msg, 64, "Invalid SYSCALL number: '%u'", state->eax);
            return true;
        }

        void* address = calls[state->eax];
        dword_t ret;
        asm volatile (" \
             push %1; \
             push %2; \
             push %3; \
             push %4; \
             push %5; \
             call *%6; \
             add $20, %%esp; \
            " : "=a" (ret) : "r" (state->edi), "r" (state->esi), "r" (state->edx), "r" (state->ecx), "r" (state->ebx), "r" (address));
        //TODO: use fastcall
        state->eax = ret;       /*dont return the value, you must write it in the state of the task, otherwiese the old
                                 *eax value would be poped in _irp_tail and would end up without return value.*/

        return false;           //tell the idt that this interrupt is not deadly for a task
    }
}
