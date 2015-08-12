#include "syscall.hpp"

namespace system
{
    void init()
    {
        idt::irq_register_event_handler(5, syscall_event_handler);
    };

    dword_t test_ret()
    { return 1337; }

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
        state->eax = ret;
    }
}
