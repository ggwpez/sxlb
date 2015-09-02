#include "task.hpp"

namespace task
{
    uint32_t pid = 1;
    int num_tasks = 0, capacity = 20;
    task_t* start_task = nullptr,* actual_task = nullptr;
    task_t* idle_task;
    bool multitasking_enabled = false;
    extern "C" struct tss_entry tss;

    void idle_swap_in();
    void idle_create();
    bool remove_from_list(task_t* target);

	void init()
	{
        idle_create();
        actual_task = start_task = idle_task;

        num_tasks++;
    }

    void idle_swap_in()
    {
        if (num_tasks >= capacity)
            return;

        idle_task->running = true;
    }

    void idle_swap_out()
    {
        if (num_tasks == 1)
            return;

        idle_task->running = false;
    }

    void idle_create()
    {
        idle_task = (task_t*)memory::k_malloc(sizeof(task_t), 0, 0);
        idle_task->pid = pid++;
        idle_task->cpu_state = actual_task->ebp = idle_task->eip = idle_task->rpl = 0;
        idle_task->directory = kernel_directory;
        idle_task->next = nullptr;
        idle_task->kernel_stack = memory::k_malloc(KERNEL_STACK_SIZE, 0, 0) + KERNEL_STACK_SIZE;
        idle_task->running = true;
    }

    void multitasking_set(bool value)
    {
        multitasking_enabled = value;
    }

    bool multitasking_get()
    {
        return multitasking_enabled;
    }

    task_t* find_by_pid(uint32_t pid)
	{
        syshlt("find_by_pid not implemented");
        return nullptr;
    }
	
	uint32_t get_pid()
	{
        return actual_task->pid;
    }

    uint32_t get_rpl()
    {
        return actual_task->rpl;
    }

    task_t* get_task()
    {
        return actual_task;
    }

    uint32_t get_task_count()
    {
        return num_tasks;
    }

    void dump_tss(tss_entry* tssEntry)
    {
        printf("eax: %x ", tssEntry->eax);
        printf("ecx: %x ", tssEntry->ecx);
        printf("edx: %x ", tssEntry->edx);
        printf("ebx: %x ", tssEntry->ebx);
        printf("esp: %x ", tssEntry->esp);
        printf("esi: %x ", tssEntry->esi);
        printfl("edi: %x ", tssEntry->edi);

        printf("es: %x ", tssEntry->es);
        printf("cs: %x ", tssEntry->cs);
        printf("ss: %x ", tssEntry->ss);
        printf("ds: %x ", tssEntry->ds);
        printf("fs: %x ", tssEntry->fs);
        printfl("gs: %x", tssEntry->gs);

        printf("esp0: %x ", tssEntry->esp0);
        printf("ss0: %x ", tssEntry->ss0);
        printf("cr3: %x ", tssEntry->cr3);
        printf("eip: %x ", tssEntry->eip);
        printfl("eflags: %x ", tssEntry->eflags);
    }

    extern "C" { extern void ir_tail(); }
    bool create(uint32_t entry_point, ubyte_t privileg)
    {
        cli
        if (num_tasks >= capacity)
        {
            syshlt("no capacity");
            return false;
        }

        //LPTR mem = memory::k_malloc(sizeof(task_t) + KERNEL_STACK_SIZE, 0, 0);

        task_t* task = (task_t*)memory::k_malloc(sizeof(task_t), 0, 0);
        task->next = nullptr;
        task->directory = clone_directory(current_directory, &task->dir_offset);
        //task->directory = current_directory;

        task->pid = pid++;
        task->kernel_stack = memory::k_malloc(KERNEL_STACK_SIZE,0,0) + KERNEL_STACK_SIZE;
        uint32_t* kernel_stack = task->kernel_stack;
        printfl("end: %x", task->kernel_stack+KERNEL_STACK_SIZE);

        uint32_t code_segment=0x08, data_segment=0x10;
        *(--kernel_stack) = 0;  // return address dummy

        if(privileg == 3)
        {
            // general information: Intel 3A Chapter 5.12
            *(--kernel_stack) = task->ss = 0x23;    // ss
            *(--kernel_stack) = task->kernel_stack; // esp0
            code_segment = 0x1B;                        // 0x18|0x3=0x1B
        }

        *(--kernel_stack) = 0x0202; // eflags = interrupts activated and iopl = 0
        *(--kernel_stack) = code_segment; // cs
        *(--kernel_stack) = entry_point; // eip
        *(--kernel_stack) = 0; // error code

        *(--kernel_stack) = 0; // interrupt nummer

        // general purpose registers w/o esp
        *(--kernel_stack) = 0;
        *(--kernel_stack) = 0;
        *(--kernel_stack) = 0;
        *(--kernel_stack) = 0;
        *(--kernel_stack) = 0;
        *(--kernel_stack) = 0;
        *(--kernel_stack) = 0;

        if(privileg == 3) data_segment = 0x23; // 0x20|0x3=0x23

        *(--kernel_stack) = data_segment;
        *(--kernel_stack) = data_segment;
        *(--kernel_stack) = data_segment;
        *(--kernel_stack) = data_segment;

        tss.ss0 = 0x10;
        tss.esp0 = task->kernel_stack;
        tss.ss = data_segment;

        task->ebp = 0;
        task->cpu_state = (cpu_state_t*)kernel_stack;
        task->eip = (uint32_t)ir_tail;
        task->ss = data_segment;
        task->rpl = privileg;
        task->running = true;

        task->next = actual_task->next;
        actual_task->next = task;

        num_tasks++;
        idle_swap_out();
        //printfl("Task created, id=%u, count=%u", task->pid, num_tasks);
        sti
        return true;
    };

    void end()  //terminates the actual working task
    {
        if (num_tasks == 1)
            syshlt("Last task ended.");

        kill_at(actual_task);
        idle_swap_in();
        TASK_SWITCH
        while(1);
    }
	
    bool kill(uint32_t pid)
	{
        task_t* target;
        if (actual_task->next == actual_task)   //1 task
            return false;

        if (!pid || (target = find_by_pid(pid)) == nullptr)
            return false;
        else
            return kill_at(target);
	};

    bool kill_at(task_t* target)
    {
        target->running = false;

        task_t* prev = start_task;
        while (prev->next != target)
            if (!prev->next)
                break;
            else
                prev = prev->next;

        prev->next = target->next;
        target->~task_t();

        num_tasks--;
        printfl("Task killed, id=%u, count=%u", target->pid, num_tasks);
        return true;
    };

    void tss_switch(uint32_t esp, uint32_t esp0, uint32_t ebp, uint32_t cr3, uint32_t ss)
    {
        tss.esp  = esp;
        tss.esp0 = esp0;
        tss.ebp  = ebp;
        tss.cr3  = cr3;
        tss.ss   = ss;
    }

    task_t* next_task(task_t* t)
    {
        if (!t->next)
            return start_task;
        else
            return t->next;
    }

    bool activated = false;
    cpu_state_t* schedule(cpu_state_t* cpu)
    {
        if (!actual_task || !multitasking_enabled)
            return cpu;

        actual_task->cpu_state = cpu;

        task_t* started_at = actual_task;
        while (1)                           //this searches for the next runnable task
        {
            if (!actual_task->next)
                actual_task = start_task;
            else
                actual_task = actual_task->next;

            if (actual_task->running)
                break;

            if (actual_task == started_at)  //all tasks are paused, resume the idle task
            {
                idle_swap_in();
                actual_task = idle_task;
                break;
            }
        }

        switch_paging(actual_task->directory);
        tss_switch(actual_task->cpu_state, actual_task->kernel_stack +KERNEL_STACK_SIZE, actual_task->ebp, actual_task->directory->physical_address, actual_task->ss); // esp0, esp, ss        switch_paging(actual_task->directory);

        return actual_task->cpu_state;
	};

    task_t::~task_t()
    {
        memory::k_free(this);
        memory::k_free(this->kernel_stack - KERNEL_STACK_SIZE);
        //memory::k_free((uint32_t)directory - this->dir_offset);
    };
}
