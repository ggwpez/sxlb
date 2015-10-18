#pragma once

#include "../types.hpp"
#include "../task/task.hpp"
#include "../system/system.hpp"

namespace idt
{
    //all methods return true, if a user task that caused this fault, should be killed, aka #GPF
    //if no handler is available, the task is also killed.

    bool nmi_handler(task::cpu_state_t* state, char* kill_msg);
    bool zerodiv_handler(task::cpu_state_t* state, char* kill_msg);
    bool gpf_handler(task::cpu_state_t* state, char* kill_msg);
    bool pf_handler(task::cpu_state_t* state, char *kill_msg);
}
