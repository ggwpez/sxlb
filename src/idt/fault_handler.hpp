#pragma once

#include "../types.hpp"
#include "../task/task.hpp"
#include "../system/system.hpp"

namespace idt
{
    void page_fault_handler(task::cpu_state_t* state);
}
