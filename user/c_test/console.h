#pragma once
#include "stdint.h"

int main();

#define s 128

//style
uchar_t fc, bc;
uchar_t usr_fc, usr_bc;


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
