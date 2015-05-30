#include "task.hpp"

namespace task
{
	//cc
	static uint32_t pid = 1;
	static int current_task = -1;
    static int num_tasks = 0, capacity = 6;
    static struct task_t* tasks[6];
	static bool multitasking_enabled = false;
	extern "C" tss_entry tss;

	void init()
	{
		cli
		
		
		
		sti
	};

	void multitasking_set_enabled(bool value)
	{
		multitasking_enabled = value;
	};

    uint32_t find_by_pid(uint32_t pid)
	{
        for (uint32_t i = 0; i < capacity; i++)
		{
            if (tasks[i] && tasks[i]->pid == pid)
				return i;
		}

		return uint32_max;
	};
	
	uint32_t get_pid()
	{
        if (tasks[current_task])
            return tasks[current_task]->pid;
        else
            return 0;
	};

    uint32_t get_active_tasks()
    {
        uint32_t c = 0;
        for (int i = 0; i < capacity; ++i)
            if (tasks[i])
                c++;
        if (c != num_tasks)
            syshlt("internal error");

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
        if (num_tasks >= capacity)
            return false;

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
            memory::k_free(task->kernel_stack);
            memory::k_free(task->user_stack_original);
            memory::k_free(task);

            return false;
        }

        tasks[num_tasks++] = task;

		return true;
	};

    void end()  //terminats the actual working task
    {
        tasks[current_task]->to_dispose = true;
        //__asm__ __volatile__("int $32");
        while(1);
    } __attribute__((noreturn));    //end means end
	
    bool kill(uint32_t pid)
	{
        uint32_t index;

        if (!pid || (index = find_by_pid(pid)) == uint32_max)
            return false;
        else
            return kill_at(index);
	};

    bool kill_at(uint32_t index)
    {
        if (index +1 >= capacity || num_tasks <= 1)
            return false;

        task_t* tmp = tasks[index];
        cli
        tasks[index] = nullptr;
        num_tasks--;
        sti

        if (!memory::k_free(tmp))
            syshlt("Task::kill error");

        printf("killed @%u", index);
        return true;
    };

	struct task::cpu_state_t* schedule(struct task::cpu_state_t* cpu)
	{
        if (!multitasking_enabled)
            return cpu;

        if (num_tasks == 0) syshlt("no tasks available");

        uint32_t i = current_task +1;
        if (i == capacity) i = 0;                 //always normalize
        while (!tasks[i])
        {
           if (tasks[i]->to_dispose)
           {
               kill_at(i);
               continue;
           }

           i++;

           if (i == current_task)
               return cpu;

           if (i == capacity) i = 0;
        }

        if (tasks[current_task])
            tasks[current_task]->cpu_state = cpu;

        current_task = i;
        cpu = tasks[i]->cpu_state;
        //tss.esp0 = tasks[i]->kernel_stack;

		return cpu;
	};
	//kk
}
