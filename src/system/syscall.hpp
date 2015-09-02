#pragma once
#include "../types.hpp"
#include "../ui/textmode.hpp"
#include "../idt/idt.hpp"
#include "../user/test.hpp"
#include "../task/task.hpp"
#include "../io/keyboard.hpp"

namespace system
{    
    //syscalls are actually exactly 10 times slower than normal calls
    enum class CALL : dword_t
	{
        NOP = 0,
        PUTC,
        PRINT,
        PRINTL,
        PRINTF,
        PRINTFL,                //5
        UI_TEXT_CLEAR_SCREEN,
        UI_TEXT_SET_COLOR,
        UI_TEXT_SET_COLOR_ALL,
        UI_TEXT_SET_BC,
        UI_TEXT_SET_BC_ALL,     //10
        UI_TEXT_SET_FC,
        UI_TEXT_SET_FC_ALL,
        GET,
        GETC,
        TASK_EXIT,              //15
        TASK_GET_RPL,
        TASK_GET_PID,
        CALL_ENTRYS
	};
	
    dword_t test_ret(dword_t arg0, dword_t arg1, dword_t arg2, dword_t arg3, dword_t arg4);

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
        &io::keyboard::getc,
        &task::end,
        &task::get_rpl,
        &task::get_pid
	};
	
    void init();
    bool syscall_event_handler(task::cpu_state_t* state, char* kill_msg);

    #define SYSCALL_RET0(num, ret) asm volatile("int $0x7F" : "=a" (ret) : "0" (num))
    #define SYSCALL_RET1(num, ret, arg0) asm volatile("int $0x7F" : "=a" (ret) : "0" (num), "b" (arg0))
    #define SYSCALL_RET2(num, ret, arg0, arg1) asm volatile("int $0x7F" : "=a" (ret) : "0" (num), "b" (arg0), "c" (arg1))
    #define SYSCALL_RET3(num, ret, arg0, arg1, arg2) asm volatile("int $0x7F" : "=a" (ret) : "0" (num), "b" (arg0), "c" (arg1), "d" (arg2))
    #define SYSCALL_RET4(num, ret, arg0, arg1, arg2, arg3) asm volatile("int $0x7F" : "=a" (ret) : "0" (num), "b" (arg0), "c" (arg1), "d" (arg2), "S" (arg3))
    #define SYSCALL_RET5(num, ret, arg0, arg1, arg2, arg3, arg4) asm volatile("int $0x7F": "=a" (ret) : "0" (num), "b" (arg0), "c" (arg1), "d" (arg2), "S" (arg3), "D" (arg4))

    #define SYSCALL0(num) asm volatile("int $0x7F" : : "a" (num))
    #define SYSCALL1(num, arg0) asm volatile("int $0x7F" : : "a" (num), "b" (arg0))
    #define SYSCALL2(num, arg0, arg1) asm volatile("int $0x7F" : : "a" (num), "b" (arg0), "c" (arg1))
    #define SYSCALL3(num, arg0, arg1, arg2) asm volatile("int $0x7F" : : "a" (num), "b" (arg0), "c" (arg1), "d" (arg2))
    #define SYSCALL4(num, arg0, arg1, arg2, arg3) asm volatile("int $0x7F" : : "a" (num), "b" (arg0), "c" (arg1), "d" (arg2), "S" (arg3))
    #define SYSCALL5(num, arg0, arg1, arg2, arg3, arg4) asm volatile("int $0x7F": : "a" (num), "b" (arg0), "c" (arg1), "d" (arg2), "S" (arg3), "D" (arg4))

    #define EXIT SYSCALL0(::system::CALL::TASK_EXIT);
}
