#pragma once

#include "types.hpp"
#include "textmode.hpp"
#include "system.hpp"
#include "memory.hpp"
#include "gdt.hpp"

namespace task
{
	struct cpu_state_t
	{
		uint32_t   ds;
		uint32_t   es;
		uint32_t   fs;
		uint32_t   gs;
		
		// Von Hand gesicherte Register
		uint32_t   eax;
		uint32_t   ebx;
		uint32_t   ecx;
		uint32_t   edx;
		uint32_t   ebp;
		uint32_t   esi;
		uint32_t   edi;

		uint32_t   int_no;
		uint32_t   error;

		// Von der CPU gesichert
		uint32_t   eip;
		uint32_t   cs;
		uint32_t   eflags;
		uint32_t   user_esp;
		uint32_t   ss;
	}__attribute__((packed));
/*
	unsigned int gs, fs, es, ds;
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;
    unsigned int int_no, err_code;
    unsigned int eip, cs, eflags, useresp, ss;*/

	//cc
	void multitasking_set_enabled(bool value);
	void init();

	bool task_create(uint32_t entry_point);
	bool task_create(uint32_t entry_point, LPTR kernel_stack, LPTR user_stack);
	struct cpu_state_t* schedule(struct cpu_state_t* cpu);
	bool task_kill(uint32_t pid);
	uint32_t get_pid();
	
	struct task_t
	{
		uint32_t pid;
		char_t* stack;
		struct cpu_state_t* cpu_state;
	}__attribute__((packed));
	//kk
}
