#pragma once
#include "../types.hpp"
#include "../ui/textmode.hpp"
#include "../idt/idt.hpp"
#include "../user/test.hpp"
#include "../task/task.hpp"
#include "../io/keyboard.hpp"
#include "syscall_defs.hpp"

namespace system
{    
    //syscalls are actually exactly 10 times slower than normal calls
    static void* calls[]
	{
        &nop,
        (void*)(void (*) (char))ui::text::put_char,
        &ui::text::write,
        &ui::text::write_line,
        &ui::text::write_f,
        &ui::text::write_line_f,        //5
        &ui::text::clear_screen,
        &ui::text::set_color,
        &ui::text::set_color_all,
        &ui::text::set_bc,
        &ui::text::set_bc_all,
        &ui::text::set_fc,
        &ui::text::set_fc_all,
        &io::keyboard::get_key,
        //&io::keyboard::getc,
        &io::keyboard::getc,     //TODO
        &task::end,
        &task::get_rpl,
        &task::get_pid,
        &task::get_spawn_time
	};
	
    void init();
    bool syscall_event_handler(task::cpu_state_t* state, char* kill_msg);
}
