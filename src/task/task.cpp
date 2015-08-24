#include "task.hpp"

namespace task
{
    uint32_t pid = 1;
    int num_tasks = 0, capacity = 20;
    task_t* start_task = nullptr,* actual_task = nullptr;
    bool multitasking_enabled = false;
    extern "C" struct tss_entry tss;

	void init()
	{
	};

	void multitasking_set_enabled(bool value)
	{
		multitasking_enabled = value;
	};

    task_t* find_by_pid(uint32_t pid)
	{
        syshlt("not implemented find_by_pid");
        return nullptr;
	};
	
	uint32_t get_pid()
	{
        return actual_task->pid;
	};

    task_t* get_task()
    {
        return actual_task;
    };

    uint32_t get_task_count()
    {
        return num_tasks;
    };

    void dump_tss(tss_entry* tssEntry)
    {
        printf("esp0: %x ", tssEntry->esp0);
        printf("ss0: %x ", tssEntry->ss0);
        printf("cr3: %x ", tssEntry->cr3);
        printf("eip: %x ", tssEntry->eip);
        printf("eflags: %x ", tssEntry->eflags);
        printf("eax: %x ", tssEntry->eax);
        printf("ecx: %x ", tssEntry->ecx);
        printf("edx: %x ", tssEntry->edx);
        printf("ebx: %x ", tssEntry->ebx);
        printf("esp: %x ", tssEntry->esp);
        printf("esi: %x ", tssEntry->esi);
        printf("edi: %x ", tssEntry->edi);
        printf("es: %x ", tssEntry->es);
        printf("cs: %x ", tssEntry->cs);
        printf("ss: %x ", tssEntry->ss);
        printf("ds: %x ", tssEntry->ds);
        printf("fs: %x ", tssEntry->fs);
        printf("gs: %x\n", tssEntry->gs);
    }

    #define PRIVILEG 3
    task_t* init_task(task_t* task, void* entry, LPTR const kernel_stack, LPTR const user_stack)
	{
        dword_t code_segment = 0x08, data_segment = 0x10;

        cli
        cpu_state_t new_state;
        new_state.eax = 0;
        new_state.ebx = 0;
		new_state.ecx = 0;
		new_state.edx = 0;
        new_state.esi = 0;
		new_state.edi = 0;
		new_state.ebp = 0;
		
		new_state.error = 0;
		new_state.int_no = 0;

        new_state.user_esp = user_stack +4096;
		new_state.eip = entry;

        if (PRIVILEG == 3)
        {
            code_segment = 0x1b;
            data_segment = 0x23;
        }

        new_state.cs = code_segment;        
        new_state.ss = data_segment;
        new_state.ds = data_segment;
        new_state.es = data_segment;
        new_state.fs = data_segment;
        new_state.gs = data_segment;

        tss.ss0 = 0x10;
        tss.esp0 = kernel_stack;
        tss.ss = data_segment;

        new_state.eflags = 0x202;
        //new_state.return_address = task::end;


        cpu_state_t* state = kernel_stack +4096 -sizeof(task::cpu_state_t);    //you better dont forgett the -!
		*state = new_state;

        if (!task)
            return 0;

        task->pid = pid++;
        task->kernel_stack = kernel_stack;
        task->user_stack = user_stack;
        task->directory = clone_directory(kernel_directory, &task->dir_offset);
        //task->directory = kernel_directory;
        task->cpu_state = state;
        task->to_dispose = 0;

        task->esp = (uint32_t)user_stack; //TODO: look here
        task->ss  = data_segment;

        sti
        return task;
	};  

    bool create2(uint32_t entry_point)
    {
        if (num_tasks >= capacity)
        {
            syshlt("no capacity");
            return false;
        }

        task_t* task = memory::k_malloc(sizeof(task_t), 0, 0);
        task->next = nullptr;
        task->directory = clone_directory(kernel_directory, &task->dir_offset);

        task->pid = pid++;
        task->kernel_stack = memory::k_malloc(KERNEL_STACK_SIZE, 0, 0) + KERNEL_STACK_SIZE;
        uint32_t* kernel_stack = task->kernel_stack;

        uint32_t code_segment=0x08, data_segment=0x10;
        *(--kernel_stack) = 0x0;  // return address dummy

        if(PRIVILEG == 3)
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

        if(PRIVILEG == 3) data_segment = 0x23; // 0x20|0x3=0x23

        *(--kernel_stack) = data_segment;
        *(--kernel_stack) = data_segment;
        *(--kernel_stack) = data_segment;
        *(--kernel_stack) = data_segment;

        tss.ss0 = 0x10;
        tss.esp0 = task->kernel_stack;
        tss.ss = data_segment;

        task->esp = kernel_stack;
        task->ebp = 0xdeadbeef; // test value
        task->ss = data_segment;

        if (!start_task)    //first task to be created
        {
            start_task = task;
            actual_task = start_task;
        }
        else
        {
            task->next = actual_task->next;
            actual_task->next = task;
        }
        //printl("task created");
        num_tasks++;
        return true;
    };

    bool create(uint32_t entry_point)
    {
        if (num_tasks >= capacity)
        {
            syshlt("no capacity");
            return false;
        }

        LPTR task_mem = memory::k_malloc(sizeof(task_t) + USER_STACK_SIZE + KERNEL_STACK_SIZE, 0, nullptr);
        LPTR kernel_stack   = task_mem     + sizeof(task_t) + KERNEL_STACK_SIZE;
        LPTR user_stack     = kernel_stack + USER_STACK_SIZE;
        if (!task_mem)
            return false;

        task_t* task = init_task((task_t*)task_mem, entry_point, kernel_stack, user_stack);
        task->next = nullptr;

        if (!start_task)    //first task to be created
        {
            start_task = task;
            actual_task = start_task;
        }
        else
        {
            task->next = actual_task->next;
            actual_task->next = task;
        }
        //printl("task created");
        num_tasks++;
        return true;
	};

    void end()  //terminats the actual working task
    {
        actual_task->to_dispose = 0xff;

        if (num_tasks == 1)
            syshlt("Last task ended.");

        TASK_SWITCH
        stop                                //end means end
    };
	
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
        if (start_task == target)
        {
            start_task = target->next;
            target->~task_t();

            num_tasks--;

            return true;
        }

        task_t* task = start_task;
        while (task->next != target)
            if (!task->next)
                break;
            else
                task = task->next;

        task->next = target->next;
        target->~task_t();

        num_tasks--;
        return true;
    };

    void tss_switch(uint32_t esp, uint32_t esp0, uint32_t ebp, uint32_t cr3, uint32_t ss)
    {
        tss.esp0 = esp0;
        tss.esp  = esp;
        tss.cr3  = cr3;
        tss.ebp  = ebp;
        tss.ss   = ss;
    }
    bool activated = false;
    cpu_state_t* schedule(cpu_state_t* cpu)
    {
        if (!multitasking_enabled)
            return cpu;
        if (!actual_task)
            syshlt("no task available");

        if (!activated)   //first switch from kernel to task, where the kernel cpu should not be copied to the task
        {
            activated = true;

            if (!actual_task) syshlt("wat");
            tss_switch(actual_task->esp, actual_task->kernel_stack +KERNEL_STACK_SIZE, actual_task->ebp, actual_task->directory->physical_address, actual_task->ss); // esp0, esp, ss
            switch_paging(actual_task->directory->physical_address);
            return actual_task->cpu_state;
        }

        actual_task->cpu_state = cpu;

        if (actual_task->to_dispose)    //disposion flag set
            kill_at(actual_task);

        if (actual_task->next)
            actual_task = actual_task->next;
        else
            actual_task = start_task;

        cpu = actual_task->cpu_state;
        tss_switch(actual_task->esp, actual_task->kernel_stack +KERNEL_STACK_SIZE, actual_task->ebp, actual_task->directory->physical_address, actual_task->ss); // esp0, esp, ss
        switch_paging(actual_task->directory->physical_address);

		return cpu;
	};

    task_t::~task_t()
    {
        memory::k_free(this);
        memory::k_free((uint32_t)directory - this->dir_offset);
        this->to_dispose = 0;
    };
}
