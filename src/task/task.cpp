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
	
    struct task::task_t* init_task(void* entry, LPTR kernel_stack, LPTR user_stack)
	{
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

		new_state.cs = 0x08;
		new_state.ds = 0x10;
		new_state.es = 0x10;
        new_state.fs = 0x10;
		new_state.gs = 0x10;
        //new_state.ss = 0x23;

		/*tss.ss0 = 0x10;
		tss.esp0 = kernel_stack + 4096 - sizeof(new_state);
		tss.ss = 0x23;*/

        new_state.eflags = 0x200;

        struct task::cpu_state_t* state = kernel_stack +4096 -sizeof(task::cpu_state_t);
		*state = new_state;

        task_t* task = (task_t*)memory::k_malloc(sizeof(task_t), 0, nullptr);
        if (!task)
            return 0;

        task->pid = pid++;
        task->kernel_stack_original = kernel_stack;
        task->user_stack_original = user_stack;
        task->cpu_state = state;
        task->to_dispose = 0;

        sti
        return task;
	};

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
        tss.esp0 = actual_task->kernel_stack_original+4096;

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
