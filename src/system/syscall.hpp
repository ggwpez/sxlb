#pragma once
#include "../types.hpp"
#include "../ui/textmode.hpp"
#include "../user/test.hpp"

namespace system
{
	#define CALL_NUM_MAX 2
	enum call_number : uint32_t
	{
		PUTC = 0,
		TEST = 1
	};
	
	static void* calls[CALL_NUM_MAX]
	{
		(void*)(void(*)(const char))ui::text::put_char,
		(void*)&user::test
	};
	
	dword_t syscall(call_number num, dword_t arg0, dword_t arg1, dword_t arg2, dword_t arg3, dword_t arg4)  __attribute__((fastcall));
}
