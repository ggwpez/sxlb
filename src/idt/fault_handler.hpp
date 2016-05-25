#pragma once

#include "../types.hpp"
#include "../task/task.hpp"
#include "../system/system.hpp"

namespace idt
{
    //all methods return true, if a user task that caused this fault, should be killed, aka #GPF
    //if no handler is available, the task is also killed.

    task::cpu_state_t* nmi_handler(task::cpu_state_t* state);
    task::cpu_state_t* zerodiv_handler(task::cpu_state_t* state);
    task::cpu_state_t* gpf_handler(task::cpu_state_t* state);
    task::cpu_state_t* pf_handler(task::cpu_state_t* state);
    task::cpu_state_t* df_handler(task::cpu_state_t* state);
}
