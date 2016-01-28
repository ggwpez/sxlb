#include "task.hpp"
#include "../time/timer.hpp"
#include "../string.hpp"

namespace task
{
    uint32_t pid = 1;
    int num_tasks = 0, capacity = 20;
    task_t* start_task = nullptr,* actual_task = nullptr,* focused_task = nullptr;
    task_t* idle_task;
    bool multitasking_enabled = false;
    struct gdt::tss_entry tss;
    io::keyboard::key_queue_t* key_queue = nullptr;

    void idle_swap_in();
    void idle_create();
    bool remove_from_list(task_t* target);

    void init()
    {
        idle_create();
        logINF("finalizing tasking...");
        actual_task = start_task = idle_task;

        num_tasks++;
        logDONE;
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
        logINF("creating idle task...");
        idle_task = (task_t*)memory::k_malloc(sizeof(task_t), 0, 0);
        idle_task->pid = pid++;
        idle_task->cpu_state = actual_task->ebp = idle_task->eip = idle_task->rpl = 0;
        idle_task->directory = kernel_directory;
        idle_task->next = nullptr;
        idle_task->kernel_stack = memory::k_malloc(KERNEL_STACK_SIZE, 0, 0) + KERNEL_STACK_SIZE;
        idle_task->running = true;
        idle_task->key_queue = nullptr;
        logINF("(pid: %u)", idle_task->pid); logDONE;
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
        task_t* ret = start_task;

        while (ret->pid != pid)
            if (!ret->next)
                return nullptr;
            else
                ret = ret->next;

        return ret;
    }

    bool change_focus(uint32_t pid)
    {
        change_focus(find_by_pid(pid));
    }

    bool change_focus(task_t* task)
    {
        focused_task = task;
        key_queue = focused_task->key_queue;
    }

    uint32_t get_pid()
    {
        return actual_task->pid;
    }

    uint32_t get_rpl()
    {
        return actual_task->rpl;
    }

    uint32_t get_spawn_time()
    {
        return get_ticks() - actual_task->spawn_time;
    }

    task_t* get_task()
    {
        return actual_task;
    }

    uint32_t get_task_count()
    {
        return num_tasks;
    }

    vfs::fs_node_t* get_working_dir()
    {
        return actual_task->working_dir;
    }

    int set_working_dir(vfs::fs_node_t* dir)
    {
        NOT_NULL(dir);

        actual_task->working_dir = dir;
        return 0;
    }

    void force_execute(task_t* task, uint32_t entry, uint32_t ret, uint32_t sig)
    {
        *((uint32_t*)--task->cpu_state->user_esp) = sig;
        *((uint32_t*)--task->cpu_state->user_esp) = ret;
        task->cpu_state->eip = entry;
    }

    uint32_t poll_key()
    {
        io::keyboard::key_queue_t* queue = actual_task->key_queue;

        if (!queue || queue->empty())
            return 0;

        return queue->pop_front();
    }

    uint32_t poll_key_buffer_s(io::keyboard::key_state_t* buffer, size_t s)
    {
        io::keyboard::key_queue_t* queue = actual_task->key_queue;

        if (!queue || queue->empty())
            return 0;

        while (!queue->empty() && s--)
            *buffer++ = queue->pop_front();
    }

    uint32_t poll_char()
    {
        io::keyboard::key_queue_t* queue = actual_task->key_queue;

        if (!queue || queue->empty())
        {
            if (!queue)
            { logWAR("kq null !!"); }
            return 0;
        }

        char c = io::keyboard::state_to_char(actual_task->key_queue->pop_front());
       // logINF("polling %c", c);
        return c;
    }

    uint32_t poll_char_buffer_s(char *buffer, size_t s)
    {
        io::keyboard::key_queue_t* queue = actual_task->key_queue;

        if (!queue || queue->empty())
            return 0;

        while (!queue->empty() && s--)
            *buffer++ = io::keyboard::state_to_char(queue->pop_front());
    }

    void dump_tss(gdt::tss_entry* tssEntry)
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
    bool create(uint32_t entry_point, uint32_t argc, LPTR argv, ubyte_t privileg)
    {
        cli
        if (num_tasks >= capacity)
        {
            syshlt("no capacity");
            return false;
        }

        LPTR mem = memory::k_malloc(sizeof(task_t) + KERNEL_STACK_SIZE, 0, 0);

        task_t* task = mem;
        //task_t* task = memory::k_malloc(sizeof(task_t), 0, 0);
        task->next = nullptr;
        task->directory = clone_directory(current_directory, &task->dir_offset);

        task->pid = pid++;
        task->kernel_stack = mem + sizeof(task_t) + KERNEL_STACK_SIZE;
        //task->kernel_stack = memory::k_malloc(KERNEL_STACK_SIZE,0,0) + KERNEL_STACK_SIZE;
        uint32_t* kernel_stack = task->kernel_stack;

        task->key_queue = memory::k_malloc(sizeof(io::keyboard::key_queue_t),0,0);
        //task->key_queue = task->kernel_stack + sizeof(io::keyboard::key_queue_t);       //TODO look here
        task->key_queue->create();
        task->working_dir = &vfs::root_node;

        uint32_t code_segment=0x08, data_segment=0x10;
        *(--kernel_stack) = &task::end_noret;           //implicit 0 return

        if(privileg != 0)
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
        *(--kernel_stack) = argc;
        *(--kernel_stack) = argv;
        *(--kernel_stack) = 0;
        *(--kernel_stack) = 0;
        *(--kernel_stack) = 0;
        *(--kernel_stack) = 0;
        *(--kernel_stack) = 0;

        if(privileg != 0) data_segment = 0x23; // 0x20|0x3=0x23

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
        task->spawn_time = get_ticks();

        task->next = actual_task->next;
        actual_task->next = task;

        if (num_tasks++ == 1)   //first user task to be craeted
            change_focus(task);
        idle_swap_out();
        sti
        return true;
    };

    void end(uint32_t status)           //terminates the actual working task
    {
        if (!multitasking_enabled)
            return;

        if (num_tasks == 1)
            syshlt("Last task ended.");

        actual_task->cpu_state->eax = status;   //return code
        kill_at(actual_task);
        idle_swap_in();
        TASK_SWITCH
        while(1);
    }

    void end_noret()
    {
        end(0);
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
        num_tasks--;
        printfl("Killing task: id=%u, return=%i", target->pid, target->cpu_state->eax);
        target->~task_t();

        return true;
    };

    uint32_t sig(uint32_t id, uint32_t sig)
    {
        task_t* task = find_by_pid(id);
        logINF("SIG pid: %u  sig: %u task eip: 0x%x ", id, sig, task->cpu_state->eip);
        logINF((task ? "yes\n" : "no\n")); logDONE;

        if (!task)
            return -1;

        force_execute(task, 0x40c160, task->cpu_state->eip, sig);  //0x40c10d == &task_sig_trap    //highly experimental
        return 0;
    }

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
        change_focus(actual_task);

        return actual_task->cpu_state;
    };

    task_t::~task_t()
    {
        memory::k_free(this);
        memory::k_free((uint32_t)directory - this->dir_offset);   //###
    };
}
