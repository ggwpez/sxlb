#include "task.hpp"

namespace task
{
    uint32_t pid = 1;
    int current_task = -1;
    int num_tasks = 0, capacity = 6;
    struct task_t* actual_task = nullptr;
    bool multitasking_enabled = false;
	extern "C" tss_entry tss;

	void init()
	{
        if (!actual_task)
            printlf("fail");
        else
            printlf("oko");
	};

	void multitasking_set_enabled(bool value)
	{
		multitasking_enabled = value;
	};

    task_t* find_by_pid(uint32_t pid)
	{
        task_t* start = actual_task;
        task_t* iterator = actual_task;

        while ((iterator = actual_task->next) != start && iterator->pid != pid);

        if (iterator->pid == pid)
            return iterator;
        else
            return nullptr;
	};
	
	uint32_t get_pid()
	{
        return actual_task->pid;
	};
    uint32_t        get_active_tasks()
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

		struct task::cpu_state_t* state = kernel_stack +4096 -sizeof(new_state);
		*state = new_state;

        task_t* task = (task_t*)memory::k_malloc(sizeof(task_t), 0, 0);
        if (!task)
            return 0;
        task->pid = pid++;
        task->kernel_stack = kernel_stack;
        task->user_stack_original = user_stack;
        task->cpu_state = state;
        task->to_dispose = 0;
        //task.directory =

		sti
        return task;
	};

    bool create(uint32_t entry_point)
    {
        return create(entry_point, nullptr, nullptr);
	}

    bool create(uint32_t entry_point, LPTR kernel_stack, LPTR user_stack)
    {
        if (!kernel_stack && !(kernel_stack = memory::k_malloc(4096, 0, nullptr)))	//stack could not be allocated
            return false;
        if (!user_stack && !(user_stack = memory::k_malloc(4096, 0, nullptr)))	//stack could not be allocated
        {
            memory::k_free(kernel_stack);
            return false;
        }

        task_t* task = init_task(entry_point, kernel_stack, user_stack);
        if (!task)
        {
            memory::k_free(task);

            //printlf("internal allocation error");
            return false;
        }

        if (actual_task == nullptr)     //is it the first task to be created?
        {
            actual_task = task;

            actual_task->next = actual_task;
            actual_task->prev = actual_task;
            //printlf("   only task now: %i", actual_task == nullptr);
        }
        else                            //no, there are already other tasks here
        {
            task_t* prev = actual_task->prev;

            prev->next = task;
            task->prev = prev;
            task->next = actual_task;
            actual_task->prev = task;
            //printlf("not the same!");
        }

        //printlf("task created");
        num_tasks++;
        return true;
	};

    void end()  //terminats the actual working task
    {
        actual_task->to_dispose = 0xff;

        //__asm__ __volatile__("int $32");
        while(1);            //end means end
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
        if (target->next == target)   //1 task
            return false;

        task_t* next = target->next,
              * prev = target->prev;

        prev->next = next;
        next->prev = prev;
        actual_task = next;

        target->~task_t();
        //memory::k_free(target);

        num_tasks--;
        return true;
    };

	struct task::cpu_state_t* schedule(struct task::cpu_state_t* cpu)
	{
        if (!multitasking_enabled)
            return cpu;
        if (!actual_task)
            syshlt("no task available");

        if (actual_task->next == actual_task)    //only one task here
        {
            if (actual_task->cpu_state->eip == cpu->eip)
                return cpu;
            else
            {
                return actual_task->cpu_state;
            }
        }

        if (actual_task->to_dispose)
            kill_at(actual_task);
        else
        {
            actual_task->cpu_state = cpu;
            actual_task = actual_task->next;
        }
        cpu = actual_task->cpu_state;
        //tss.esp0 = tasks[i]->kernel_stack;

		return cpu;
	};

    task_t::~task_t()
    {
        to_dispose = 0;

        memory::k_free(user_stack_original);
        memory::k_free(kernel_stack);
        //memory::k_free(&to_dispose -1);
    };
}
