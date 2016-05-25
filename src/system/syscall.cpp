#include "syscall.hpp"

namespace system
{
    void init()
    {
        logtINF("registering syscall isr...");
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

    task::cpu_state_t* syscall_event_handler(task::cpu_state_t* state)
    {
        if (state->eax > CALL::count)
        {
            char buffer[64];

            sprintf_s(buffer, sizeof(buffer), "Invalid SYSCALL number: '%u'", state->eax);
            idt::critical(state, buffer);

            return state;
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

        return state;           //tell the idt that this interrupt is not deadly for a task
    }
}
