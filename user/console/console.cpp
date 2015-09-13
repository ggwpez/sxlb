#include "console.hpp"
#include "stdio/stdio.h"
#include "stdlib/stdlib.h"
#include "string/string.h"

int main()
{
    return 9;
    printf("cpp world #1 !");
    console con;
    con.main();
}

console::console()
{
    bang = "$ ";
    this->s = 128;
    buffer[s -1] = 0;

    /*usr_fc = FC_TURQUOISE; usr_bc = BC_BLACK;
    fc = FC_GREEN; bc = BC_BLACK;

    SYSCALL1(SYSCNUM_UI_TEXT_SET_COLOR, fc | bc);*/
}

int console::main()
{
    printf("main HERE!");
    //SYSCALL0(SYSCNUM_UI_TEXT_CLEAR_SCREEN);
    return 0;
    while (1)
    {
        //SYSCALL1(SYSCNUM_PRINT, bang);
        printf("%s", bang);
        uint32_t l = this->get_line();
        putchar('\n');

        buffer[l] = 0;

        if (l)
            this->interpret_cmd();
    }
}

uint32_t console::get_line()
{
    uint32_t i = 0;
    while (i < this->s-1)
    {
        uchar_t in;
        //SYSCALL_RET0(SYSCNUM_GETC, in);

        if (in == '\b')
        {
            if (i != 0)
            {
                i--;
                //SYSCALL1(SYSCNUM_PUTC, '\b');
            }
        }
        else if (in == '\n')
            return i;
        //else
         //   SYSCALL1(SYSCNUM_PUTC, this->buffer[i++] = in);
    }

    return i;
}

char* cmds[] =
{
    "help", "clear", "sys_info", "pag_info", "tss_info", "mem_info", "con_info", "reboot", "quit"
};

void console::interpret_cmd()
{
    //SYSCALL1(SYSCNUM_UI_TEXT_SET_COLOR, usr_fc | usr_bc);

    if(!strcmp(buffer, cmds[0]))
        cmd_help();
    else if(!strcmp(buffer, cmds[1]))
        cmd_clear();
    else if(!strcmp(buffer, cmds[2]))
        cmd_sys_info();
    else if(!strcmp(buffer, cmds[3]))
        cmd_pag_info();
    else if(!strcmp(buffer, cmds[4]))
        cmd_tss_info();
    else if(!strcmp(buffer, cmds[5]))
        cmd_mem_info();
    else if(!strcmp(buffer, cmds[6]))
        cmd_con_info();
    else if(!strcmp(buffer, cmds[7]))
        cmd_reboot();
    else if (!strcmp(buffer, cmds[8]))
        cmd_quit();
    /*else
        SYSCALL2(SYSCNUM_PRINTFL, "command '%s' not found", buffer);

    SYSCALL1(SYSCNUM_UI_TEXT_SET_COLOR, fc | bc);*/
}

//cmd functions
void console::cmd_help()
{
    /*uint32_t c = COUNTOF(cmds);

    SYSCALL2(SYSCNUM_PRINTFL, "Commands(%u):", c);
    for (int i = 0; i < c; ++i)
    {
        SYSCALL1(SYSCNUM_PUTC, '\t');
        SYSCALL1(SYSCNUM_PRINTL, cmds[i]);
    }*/
}

void console::cmd_clear()
{
    //SYSCALL1(SYSCNUM_UI_TEXT_SET_COLOR, fc | bc);
    //SYSCALL0(SYSCNUM_UI_TEXT_CLEAR_SCREEN);
}

void console::cmd_sys_info()
{
    //uint32_t start = system::kernel_start_address(), end = system::kernel_end_address();

    //printfl("Version: %s (%s)\nSize of the OS: %m\nSize of the Kernel: %m",
    //        __VERSION__, __VERSION_STABLE__,  end, end - start);
}

void console::cmd_pag_info()
{
   // analyze_physical_addresses();
}

void console::cmd_tss_info()
{
   // task::dump_tss(&task::tss);
}

void console::cmd_mem_info()
{
   // memory::dump_info(nullptr);
}

void console::cmd_con_info()
{
   // SYSCALL2(SYSCNUM_PRINTFL, "This console runns in %s mode.", this->rpl ? "user" : "kernel");
}

void console::cmd_reboot()
{
    //io::reboot();
}

void console::cmd_quit()
{
    //SYSCALL0(SYSCNUM_TASK_EXIT);
}
