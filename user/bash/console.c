#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "../../lib/posixc/dirent.h"
#include "sys/fcntl.h"
#include "user.h"
#include "signal.h"

#include "console.h"

char bang[6];
uchar_t buffer[BUF_LEN];
uchar_t cmd_buffer[BUF_LEN];

uchar_t path[NAME_MAX];
extern int errno;
extern void _task_sig_trap();

char const* const user = "vados";

void sig_h(int sig_num)
{
    printf("Got sig 0x%x\n", sig_num);
    fflush(stdout);
}

void init(uint32_t argc, char** argv)
{
    //signal(SIGUSR1, sig_h);
    //signal(SIGUSR2, sig_h);

    strcpy(path, "/");
    strcpy(bang, "$ ");
    int pid = getpid();

    //printf("started from: %s @0x%x pid: %u\n", argv[0], &main, pid);
    for (int i = 1; i < argc; i++)
    {
        getcwd(path, NAME_MAX);
        printf("%s:%s%s", user, path, bang);
        
        strcpy(buffer, argv[i]);
        interpret_cmd();
    }
    //printf("pid: %u", pid);
    //printf("_task_sig_trap @0x%x\n", &_task_sig_trap);
}

int main(uint32_t argc, char** argv)
{
    init(argc, argv);

    while (1)
    {
        getcwd(path, NAME_MAX);
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
    while (i < BUF_LEN-1)
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
        fflush(stdout);
    }

    return i;
}

uint32_t b_off = 0;
char* get_next_arg()
{
    uint32_t ret = NULL;
    if (!buffer[b_off] || b_off >= BUF_LEN)
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
    { "test", &cmd_test },
    { "clear", &cmd_clear },
    { "cd", &cmd_cd },
    { "ls", &cmd_ls },
    { "exec", &cmd_exec },
    { "pid", &cmd_pid },
    { "cat", &cmd_cat },
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
    {
        set_fc_clr(4);
        printf("ERROR\n");
        set_fc_clr(7);
    }
}

uint32_t cmd_test()
{
    return raise(0x1e);
}

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
        printf("%s\t\t%s\n", ent->d_name, (t == DT_REG ? "FILE" : t == DT_DIR ? "DIR" : t == DT_LNK ? "LNK" : "???"));
    }
    closedir(dir);
    return 0;
}

uint32_t cmd_pid()
{
    int pid = getpid();

    printf("%u\n", pid);
    return 0;
}

uint32_t cmd_exec()
{
    char* binary = get_next_arg();

    if (!binary)
        return printf("exec need a file to execute\n");

    
}

uint32_t cmd_cd()
{
    char* target_dir = get_next_arg();
    if (!target_dir)
        return printf("cd needs a directory as argument\n");

    if (chdir(target_dir))
        return printf("could not open %s\n", target_dir);

    return 0;
}

uint32_t cmd_cat()
{
    char* file = get_next_arg();
    if (!file)
        return (printf("cat needs a filename in the current directory\n") | 1);

    uint32_t l = 0, ret = 0;
    char buf[BUF_LEN] = { 0 };
    FILE* fp = fopen(file, "r");
    if (fp)
    {
        size_t read = 0;
        while ((read = fread(buf, 1, BUF_LEN, fp)) != 0)
            fwrite(buf, 1, read, stdout);

        fflush(stdout);
        fclose(fp);
        return 0;
    }
    else
        return printf("File could not be opened.\n");
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