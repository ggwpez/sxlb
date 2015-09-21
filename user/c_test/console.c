#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "../../lib/posixc/dirent.h"

#include "console.h"

char* bang;
uchar_t buffer[s];

uchar_t path[NAME_MAX];
uint32_t path_l = 0;

int main()
{
    int rpl = 0;
    char* user = rpl ? "user" : "root";
    strcpy(path, ".");
    strcpy(bang, "$ ");

    while (1)
    {
        printf("%s@%s%s", user, path, bang);
        uint32_t l = get_line();
        putchar('\n');

        buffer[l] = 0;

        if (l)
            interpret_cmd();
    }

    printf("end\n");
    return 0;
}

uint32_t get_line()
{
    uint32_t i = 0;
    while (i < s-1)
    {
        uchar_t in = getchar();

        if (in == '\b')
        {
            if (i != 0)
            {
                i--;
                putchar('\b');
            }
        }
        else if (in == '\n')
            return i;
        else
            putchar(buffer[i++] = in);
    }

    return i;
}

char* cmds[] =
{
    "help", "clear", "uname", "pag_info", "tss_info", "mem_info", "con_info", "reboot", "quit"
};

void interpret_cmd()
{
    cmd_ls();
    /*if(!strcmp(buffer, cmds[0]))
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
    else if(!strcmp(buffer, cmds[8]))
        cmd_quit();
    else
        printf("command '%s' not found\n", buffer);*/
}

//cmd functions
void cmd_help()
{
    uint32_t c = sizeof(cmds) / sizeof(cmds[0]);

    printf("Commands(%u):\n", c);
    for (int i = 0; i < c; ++i)
    {
        putchar('\t');
        printf(cmds[i]);
        putchar('\n');
    }
}

void cmd_ls()
{
    DIR* dir = opendir(".");

    dirent_t* ent;
    while (ent = readdir(dir))
        printf("%s\n", ent->d_name);

    closedir(dir);
}

void cmd_clear()
{
    /*SYSCALL1(SYSCNUM_UI_TEXT_SET_COLOR, fc | bc);
    SYSCALL0(SYSCNUM_UI_TEXT_CLEAR_SCREEN);*/
}

void cmd_uname()
{
    /*uint32_t start = system::kernel_start_address(), end = system::kernel_end_address();

    printfl("Version: %s (%s)\nSize of the OS: %m\nSize of the Kernel: %m",
            __VERSION__, __VERSION_STABLE__,  end, end - start);*/
}

void cmd_pag_info()
{
    //analyze_physical_addresses();
}

void cmd_tss_info()
{
    //task::dump_tss(&task::tss);
}

void cmd_mem_info()
{
    //memory::dump_info(nullptr);
}

void cmd_con_info()
{
    //printf("This console runns in %s mode.", this->rpl ? "user" : "kernel");)
}

void cmd_reboot()
{
    //io::reboot();
}

void cmd_quit()
{
    exit(0);
}