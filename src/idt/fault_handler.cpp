#include "fault_handler.hpp"

namespace idt
{
    void page_fault_handler(task::cpu_state_t* state)
    {
        syshlt("PAGE FAULT");
    }
}
