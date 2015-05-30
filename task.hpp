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

    bool            create(uint32_t entry_point);
    bool            create(uint32_t entry_point, LPTR kernel_stack, LPTR user_stack);
    cpu_state_t*    schedule(struct cpu_state_t* cpu);
    void            end() __attribute__((noreturn));
    bool            kill(uint32_t pid);
    bool            kill_at(uint32_t index);

    uint32_t        get_pid();
    uint32_t        get_active_tasks();

	struct task_t
	{
        bool to_dispose = false;
		uint32_t pid;
        char_t* kernel_stack;
        LPTR user_stack_original;
        page_directory* directory;
        cpu_state_t* cpu_state;
        //~task_t();
	}__attribute__((packed));
	//kk
}
