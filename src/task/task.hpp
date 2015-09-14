#pragma once

#include "../types.hpp"
#include "../ui/textmode.hpp"
#include "../system/system.hpp"
#include "../memory/memory.hpp"
#include "../gdt.hpp"

#define ELF_OFFSET 0x110

namespace task
{
    struct task_t;
	struct cpu_state_t
	{
        // Von Hand gesicherte Register
        uint32_t   gs;
        uint32_t   fs;
        uint32_t   es;
        uint32_t   ds;

        uint32_t   edi;
        uint32_t   esi;
        uint32_t   ebp;
        uint32_t   edx;
        uint32_t   ecx;
        uint32_t   ebx;
        uint32_t   eax;

		uint32_t   int_no;
		uint32_t   error;

        // Von der CPU gesichert
		uint32_t   eip;
        uint32_t   cs;
		uint32_t   eflags;
		uint32_t   user_esp;
		uint32_t   ss;
        //uint32_t   return_address;
	}__attribute__((packed));

    void multitasking_set(bool value);
    bool multitasking_get();
	void init();

    void dump_tss(tss_entry* tssEntry);
    extern "C" { extern struct tss_entry tss; }

    bool            create(uint32_t entry_point, ubyte_t privileg);
    cpu_state_t*    schedule(cpu_state_t* cpu);
    void            end(uint32_t status) __attribute__((noreturn));
    bool            kill(uint32_t pid);
    bool            kill_at(task_t* target);

    uint32_t        get_pid();
    uint32_t        get_rpl();
    uint32_t        get_spawn_time();
    task_t*         get_task();
    uint32_t        get_task_count();

    #define USER_STACK_SIZE 4096
    #define KERNEL_STACK_SIZE 4096
	struct task_t
	{
        uint8_t running;
		uint32_t pid;
        uint32_t ebp, eip, ss;
        LPTR kernel_stack;
        page_directory* directory;
        uint32_t dir_offset;
        cpu_state_t* cpu_state;
        uchar_t rpl;
        uint32_t spawn_time;


        task_t* next;
        ~task_t();
	}__attribute__((packed));
}
