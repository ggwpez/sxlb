#pragma once
#include "stdint.h"

int main();

#define s 255
#define NAME_MAX 255

typedef unsigned char uchar_t;
//style
uchar_t fc, bc;
uchar_t usr_fc, usr_bc;


uint32_t get_line();
void interpret_cmd();

//cmd functions
uint32_t cmd_help();
uint32_t cmd_ls();
uint32_t cmd_cd();
uint32_t cmd_cat();
uint32_t cmd_clear();
uint32_t cmd_uname();
uint32_t cmd_pag_info();
uint32_t cmd_tss_info();
uint32_t cmd_mem_info();
uint32_t cmd_con_info();
uint32_t cmd_reboot();
uint32_t cmd_quit();
uint32_t cmd_atoi();