#include "syscall.hpp"

namespace system
{
    void init()
    {
        idt::irq_register_event_handler(5, syscall_event_handler);
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

    dword_t test_ret(dword_t arg0, dword_t arg1, dword_t arg2, dword_t arg3, dword_t arg4)
    {
        return 999;
    }

    void syscall_event_handler(struct task::cpu_state_t* state)
    {
        if (state->eax >= CALL_NUM_MAX)
        {
            printf("out of bounds, eax=%i ", state->eax);
            return;
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
    }
}
