#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "../../lib/posixc/dirent.h"
//#include "utsname.h"

#include "console.h"

char bang[6];
uchar_t buffer[s];
uchar_t cmd_buffer[s];

uchar_t path[NAME_MAX];
extern int errno;

int main(uint32_t argc, char** argv)
{
    strcpy(path, "/");
    strcpy(bang, "$ ");
    char* user = "root";
    printf("started from: %s\n", argv[0]);

    while (1)
    {
        printf("%s:%s%s", user, path, bang);
        fflush(stdout);
        uint32_t l = get_line();
        putchar('\n');

        buffer[l] = 0;

        if (l)
            interpret_cmd();
    }
    
    exit(0);
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
        {
            putchar(buffer[i++] = in);
            fflush(stdout);
        }
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

typedef struct cmd
{
    char const* const name;
    uint32_t(*ptr)();
};

#define _CMDS_C (sizeof(cmds) / sizeof(cmds[0]))
struct cmd cmds[] =
{
    { "help", &cmd_help },
    { "clear", &cmd_clear },
    { "cd", &cmd_cd },
    { "ls", &cmd_ls },
    { "uname", &cmd_uname },
    { "pag_info", &cmd_pag_info },
    { "tss_info", &cmd_tss_info },
    { "mem_info", &cmd_mem_info },
    { "con_info", &cmd_con_info },
    { "reboot", &cmd_reboot },
    { "quit", &cmd_quit },
    { "atoi", &cmd_atoi }
 };

void interpret_cmd()
{
    b_off = 0;
    uint32_t err = 0;
    uint32_t(*ptr)() = NULL;
    char* cmd = get_next_arg();

    for (int i = 0; i < _CMDS_C; ++i)
        if (!strcmp(cmd, cmds[i].name))
        {
            ptr = cmds[i].ptr;
            break;
        }

    if (!ptr)
        printf("command '%s' unknown\n", cmd);
    else if (ptr())
        printf("ERROR\n");
}
//cmd functions
uint32_t cmd_help()
{
    uint32_t c = sizeof(cmds) / sizeof(cmds[0]);

    printf("Commands(%u):\n", c);
    for (int i = 0; i < c; ++i)
    {
        putchar('\t');
        printf(cmds[i].name);
        putchar('\n');
    }
    return 0;
}

uint32_t cmd_ls()
{
    DIR* dir = opendir(".");

    dirent_t* ent;
    dir->i = 1;                 //skip the name entry in a directory
    uint32_t t;
    while (ent = readdir(dir))
    {
        t = ent->d_type;
        printf("%s\t\t%s\n", ent->d_name, (t == DT_REG ? "FILE" : t == DT_DIR ? "DIR" : "???"));
    }

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

    return 0;
}

uint32_t cmd_clear()
{
    putchar('\f');
    return 0;
}

uint32_t cmd_uname()
{
    /*utsname_t buf;
    uname(&buf);
    printf("%s %s%s (%s)\n", buf.sysname, buf.version, buf.release, buf.machine);*/
    
    return -1;
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

uint32_t cmd_atoi()
{
    int v = atoi(get_next_arg());
    printf("got number: %i\n", v);

    return 0;
}