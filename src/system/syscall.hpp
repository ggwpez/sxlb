#pragma once
#include "../types.hpp"
#include "../ui/textmode.hpp"
#include "../idt/idt.hpp"
#include "../user/test.hpp"
#include "../task/task.hpp"
#include "../io/keyboard.hpp"
#include "../fs/vfs.hpp"
#include "syscall_defs.hpp"
#include "../utils/version.hpp"
#include "../user/executable.hpp"

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
        &ui::text::set_bc,
        &ui::text::set_fc,
        &task::poll_key,
        &task::poll_char,
        &task::end,                     //15
        &task::get_rpl,
        &task::get_pid,
        &task::get_spawn_time,
        &task::get_working_dir,
        &task::set_working_dir,         //20
        &task::sig,
        &vfs::get_root,
        &vfs::read,
        &vfs::write,
        &vfs::open,
        &vfs::close,                    //26
        &vfs::read_dir,
        &vfs::find_dir,
        &vfs::resolve_path,
        &vfs::render_path,
        &execve,                        //31
        &utils::get_version,
        &system::reboot                 //33
    };

    static_assert(CALL::count == COUNTOF(calls), "Syscall def enum out of sync.");

    void init();
    bool syscall_event_handler(task::cpu_state_t* state, char* kill_msg);
}
