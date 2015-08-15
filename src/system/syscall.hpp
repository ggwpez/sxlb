#pragma once
#include "../types.hpp"
#include "../ui/textmode.hpp"
#include "../idt/idt.hpp"
#include "../user/test.hpp"
#include "../task/task.hpp"

namespace system
{    
	#define CALL_NUM_MAX 2
    enum class CALL: dword_t
	{
		PUTC = 0,
		TEST = 1
	};
	
    dword_t test_ret(dword_t arg0, dword_t arg1, dword_t arg2, dword_t arg3, dword_t arg4);

	static void* calls[CALL_NUM_MAX]
	{
		(void*)(void(*)(const char))ui::text::put_char,
        (void*)&test_ret
	};
	
    void init();
    void syscall_event_handler(struct task::cpu_state_t* state);
    extern "C"
    {
        extern dword_t syscall(CALL num, dword_t arg0, dword_t arg1, dword_t arg2, dword_t arg3, dword_t arg4);
    }
}
