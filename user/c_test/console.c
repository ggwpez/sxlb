#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "../../lib/posixc/dirent.h"
#include "../../lib/posixc/sys/utsname.h"

#include "console.h"

char bang[6];
uchar_t buffer[s];
uchar_t cmd_buffer[s];

uchar_t path[NAME_MAX];

int main()
{
    strcpy(path, "/");
    strcpy(bang, ":~$ ");
    char* user = "root";

    while (1)
    {
        printf("%s@%s%s", user, path, bang);
        uint32_t l = get_line();
        putchar('\n');

        buffer[l] = 0;

        if (l)
            interpret_cmd();
    }
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

uint32_t b_off = 0;
char* get_next_arg()
{
    uint32_t ret = NULL;
    if (!buffer[b_off] || b_off >= s)
        return NULL;

    if (buffer[b_off] != ' ')
    {
        ret = b_off;
        while (buffer[b_off] != ' ' && buffer[b_off]) b_off++;
        uint32_t i;
        for (i = 0; i +ret < b_off; ++i)
            cmd_buffer[i] = buffer[i +ret];
        cmd_buffer[i] = 0;

        while (buffer[b_off] == ' ' && buffer[b_off]) b_off++;
        return cmd_buffer;
    }
    else                //malformatted command, like ' cd /'
        return NULL;
}

char* cmds[] =
{
    "help", "clear", "cd", "ls", "uname", "pag_info", "tss_info", "mem_info", "con_info", "reboot", "quit"
};

void interpret_cmd()
{
    b_off = 0;
    uint32_t err = 0;
    char* cmd = get_next_arg();

    if(!strcmp(cmd, cmds[0]))
        err = cmd_help();
    else if(!strcmp(cmd, cmds[1]))
        err = cmd_clear();
    else if(!strcmp(cmd, cmds[2]))
        err = cmd_cd();
    else if(!strcmp(cmd, cmds[3]))
        err = cmd_ls();
    else if(!strcmp(cmd, cmds[4]))
        err = cmd_uname();
    else if(!strcmp(cmd, cmds[5]))
        err = cmd_pag_info();
    else if(!strcmp(cmd, cmds[6]))
        err = cmd_tss_info();
    else if(!strcmp(cmd, cmds[7]))
        err = cmd_mem_info();
    else if(!strcmp(cmd, cmds[8]))
        err = cmd_con_info();
    else if(!strcmp(cmd, cmds[9]))
        err = cmd_reboot();
    else if(!strcmp(cmd, cmds[10]))
        err = cmd_quit();
    else
    {
        printf("Command '%s' unknown.\n", cmd);
        err = 1;
    }
    if (err) printf("ERROR\n");
}

//cmd functions
uint32_t cmd_help()
{
    uint32_t c = sizeof(cmds) / sizeof(cmds[0]);

    printf("Commands(%u):\n", c);
    for (int i = 0; i < c; ++i)
    {
        putchar('\t');
        printf(cmds[i]);
        putchar('\n');
    }
    return 0;
}

uint32_t cmd_ls()
{
    DIR* dir = opendir(".");

    dirent_t* ent;
    dir->i = 1;                 //skip the name entry in a directory
    while (ent = readdir(dir))
        printf("%s\n", ent->d_name);

    closedir(dir);
    return 0;
}

uint32_t cmd_cd()
{
    char* target_dir = get_next_arg();
    if (!target_dir)
        return (printf("cd needs a target directory\n") | 1);  //| 1 if printf fails too

    if (chdir(target_dir))
        return (printf("could not open %s\n", target_dir) | 1);

    //worked
    strcpy(path, target_dir);
    return 0;
}

uint32_t cmd_clear()
{
    /*SYSCALL1(SYSCNUM_UI_TEXT_SET_COLOR, fc | bc);
    SYSCALL0(SYSCNUM_UI_TEXT_CLEAR_SCREEN);*/
    return 0;
}

uint32_t cmd_uname()
{
    utsname_t buf;
    uname(&buf);
    printf("%s %s%s (%s)\n", buf.sysname, buf.version, buf.release, buf.machine);
    
    return 0;
}

uint32_t cmd_pag_info()
{
    //analyze_physical_addresses();
    return 0;
}

uint32_t cmd_tss_info()
{
    //task::dump_tss(&task::tss);
    return 0;
}

uint32_t cmd_mem_info()
{
    //memory::dump_info(nullptr);
    return 0;
}

uint32_t cmd_con_info()
{
    //printf("This console runns in %s mode.", this->rpl ? "user" : "kernel");)
    return 0;
}

uint32_t cmd_reboot()
{
    //io::reboot();
    return 0;
}

uint32_t cmd_quit()
{
    exit(0);
}