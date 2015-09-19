#pragma once

#include "keyboard.hpp"
#include "../ui/window.hpp"
#include "../user/console.hpp"
#include "../task/task.hpp"

namespace2(io, keyboard)
{
    void syskey_init();
    void syskey_handler(key_state_t key);
}}
