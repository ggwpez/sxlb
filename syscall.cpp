#include "syscall.hpp"

namespace system
{
	dword_t syscall(call_number num, dword_t arg0, dword_t arg1, dword_t arg2, dword_t arg3, dword_t arg4)
	{
		if (num >= CALL_NUM_MAX) return 0;
		void* f = calls[num];
		dword_t ret;
		
		asm volatile ("   \
		  push %1;        \
		  push %2;        \
		  push %3;        \
		  push %4;        \
		  push %5;        \
		  call *%6;       \
		  add $20, %%esp; \
		" : "=a" (ret) : "r" (arg4), "r" (arg3), "r" (arg2), "r" (arg1), "r" (arg0), "r" (f));
		//### use fastcall advantages
		
		return ret;
	}  __attribute__((fastcall));
}
