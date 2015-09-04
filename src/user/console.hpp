#pragma once

#include "../io/keyboard.hpp"
#include "../io/hwaccess.hpp"
#include "../task/task.hpp"
#include "../memory/memory.hpp"
#include "../memory/paging.hpp"
#include "../system/syscall.hpp"

class console
{
public:
    console();
    int main();

private:
    uint32_t s;     //buffer size
    uint32_t rpl;
    uchar_t buffer[128];

    //style
    uchar_t fc, bc;
    uchar_t usr_fc, usr_bc;
    char* bang;

    uint32_t get_line();
    void interpret_cmd();

    //cmd functions
    void cmd_help();
    void cmd_clear();
    void cmd_sys_info();
    void cmd_pag_info();
    void cmd_tss_info();
    void cmd_mem_info();
    void cmd_con_info();
    void cmd_reboot();
    void cmd_quit();
};
