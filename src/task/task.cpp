#include "task.hpp"

namespace task
{
    uint32_t pid = 1;
    int num_tasks = 0, capacity = 20;
    struct task_t* start_task = nullptr,* actual_task = nullptr;
    bool multitasking_enabled = false;
	extern "C" tss_entry tss;

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

    #define PRIVILEG 3
    struct task::task_t* init_task(void* entry, LPTR kernel_stack, LPTR user_stack)
	{
        dword_t code_segment = 0x08, data_segment = 0x10;

        cli
        task::cpu_state_t new_state;
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
            new_state.ss = 0x23;    //TODO: missong privileg == 3 check for kernel_stack
            code_segment = 0x1b;
        }
        new_state.cs = code_segment;

        new_state.ds = data_segment;
        new_state.es = data_segment;
        new_state.fs = data_segment;
        new_state.gs = data_segment;

        tss.ss0 = 0x10;
        tss.esp0 = kernel_stack;
        tss.ss = data_segment;

        new_state.eflags = 0x202;
        //new_state.return_address = task::end;


        struct task::cpu_state_t* state = kernel_stack +4096;// -sizeof(task::cpu_state_t);
		*state = new_state;

        task_t* task = (task_t*)memory::k_malloc(sizeof(task_t), 0, nullptr);
        if (!task)
            return 0;

        task->pid = pid++;
        task->kernel_stack_original = kernel_stack;
        task->user_stack_original = user_stack;
        task->directory = clone_directory(kernel_directory);
        task->cpu_state = state;
        task->to_dispose = 0;

        task->esp = (uint32_t)kernel_stack; //TODO: look here
        task->ss  = data_segment;

        sti
        return task;
	};

    /*extern "C"
    {
        extern void irq_tail();
    }
    task::task_t* init_task2(void* entry, LPTR kernel_stack, LPTR user_stack)
    {
        cli
        page_directory* directory = clone_directory(kernel_directory);
        task_t* new_task            = (task_t*)memory::k_malloc(sizeof(task_t),0,0);

        new_task->pid  = pid++;
        new_task->directory = directory;
        new_task->kernel_stack_original   = kernel_stack; //align
        new_task->to_dispose = 0;

        uint32_t* esp = kernel_stack;
        uint32_t code_segment=0x08, data_segment=0x10;

        //*(--esp) = 0x0;  // return address dummy

        if(PRIVILEG == 3)
        {
            // general information: Intel 3A Chapter 5.12
            *(--esp) = new_task->ss = 0x23;    // ss
            *(--esp) = new_task->kernel_stack_original; // esp0
            code_segment = 0x1B;                        // 0x18|0x3=0x1B
        }

        *(--esp) = 0x0202; // eflags = interrupts activated and iopl = 0
        *(--esp) = code_segment; // cs
        *(--esp) = (uint32_t)entry; // eip
        *(--esp) = 0; // error code

        *(--esp) = 0; // interrupt nummer

        // general purpose registers w/o esp
        *(--esp) = 0;
        *(--esp) = 0;
        *(--esp) = 0;
        *(--esp) = 0;
        *(--esp) = 0;
        *(--esp) = 0;
        *(--esp) = 0;

        if(PRIVILEG == 3) data_segment = 0x23; // 0x20|0x3=0x23

        *(--esp) = data_segment;
        *(--esp) = data_segment;
        *(--esp) = data_segment;
        *(--esp) = data_segment;

        //setup TSS
        tss.ss0   = 0x10;
        tss.esp0  = new_task->kernel_stack_original;
        tss.ss    = data_segment;


        //setup task_t
        new_task->ebp = 0xd00fc0de; // test value
        new_task->esp = (uint32_t)esp;
        new_task->eip = (uint32_t)irq_tail;
        new_task->ss  = data_segment;

        sti
        return new_task;
    }*/

    bool create(uint32_t entry_point)
    {
        return create(entry_point, nullptr, nullptr);
	}

    bool create(uint32_t entry_point, LPTR kernel_stack, LPTR user_stack)
    {
        if (num_tasks >= capacity)
        {
            syshlt("no capacity");
            return false;
        }

        if (!kernel_stack && !(kernel_stack = memory::k_malloc(4096, 0, nullptr)))	//stack could not be allocated
           { syshlt("1"); memory::k_free(kernel_stack); return false; }
        if (!user_stack && !(user_stack = memory::k_malloc(4096, 0, nullptr)))	//stack could not be allocated
        {
            syshlt("2");
            memory::k_free(user_stack);
            return false;
        }

        task_t* task = init_task(entry_point, kernel_stack, user_stack);
        if (!task)
        {
            syshlt("3");
            memory::k_free(task);

            printfl("internal allocation error");
            return false;
        }
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

    void tss_switch(uint32_t esp0, uint32_t esp, uint32_t ss)
    {
        tss.esp0 = esp0;
        tss.esp = esp;
        tss.ss = ss;
    }

    bool activated = false;
	struct task::cpu_state_t* schedule(struct task::cpu_state_t* cpu)
	{
        if (!multitasking_enabled)
            return cpu;
        if (!actual_task)
            syshlt("no task available");

        if (!activated)   //first switch from kernel to task, where the kernel cpu should not be copied to the task
        {
            activated = true;

            if (!actual_task) syshlt("wat");
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
        //tss.esp0 = actual_task->kernel_stack_original+4096;
        tss_switch(actual_task->kernel_stack_original, actual_task->esp, actual_task->ss); // esp0, esp, ss

		return cpu;
	};

    task_t::~task_t()
    {
        this->to_dispose = 0;
        memory::k_free(this);
        memory::k_free(user_stack_original);
        memory::k_free(kernel_stack_original);
    };
}
