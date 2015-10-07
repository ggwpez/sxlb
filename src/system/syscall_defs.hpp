#pragma once

enum CALL
{
    SYSCNUM_NOP = 0,
    SYSCNUM_PUTC,
    SYSCNUM_PRINT,
    SYSCNUM_PRINTL,
    SYSCNUM_PRINTF,
    SYSCNUM_PRINTFL,                //5
    SYSCNUM_UI_TEXT_CLEAR_SCREEN,
    SYSCNUM_UI_TEXT_SET_COLOR,
    SYSCNUM_UI_TEXT_SET_COLOR_ALL,
    SYSCNUM_UI_TEXT_SET_BC,
    SYSCNUM_UI_TEXT_SET_BC_ALL,     //10
    SYSCNUM_UI_TEXT_SET_FC,
    SYSCNUM_UI_TEXT_SET_FC_ALL,
    SYSCNUM_GET,
    SYSCNUM_GETC,
    SYSCNUM_TASK_EXIT,              //15
    SYSCNUM_TASK_GET_RPL,
    SYSCNUM_TASK_GET_PID,
    SYSCNUM_TASK_GET_SPAWN_TIME,
    SYSCNUM_TASK_GET_WORKING_DIR,
    SYSCNUM_TASK_SET_WORKING_DIR,   //20
    SYSCNUM_VFS_GET_ROOT,
    SYSCNUM_VFS_READ,
    SYSCNUM_VFS_WRITE,
    SYSCNUM_VFS_OPEN,               //25
    SYSCNUM_VFS_CLOSE,
    SYSCNUM_VFS_READ_DIR,
    SYSCNUM_VFS_FIND_DIR,           //28
    SYSCNUM_UTILS_GET_VERSION,
    SYSCNUM_SYSTEM_REBOOT,
    SYSCNUM_CALL_ENTRYS
};

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)
#define _SYSCALL_INT 127
#define _SYSCALL_INT_S STR(_SYSCALL_INT)

#define SYSCALL_RET0(num, ret) asm volatile("xor %%eax, %%eax\nint $" _SYSCALL_INT_S : "=r" (ret) : "r" (num))
#define SYSCALL_RET1(num, ret, arg0) asm volatile("xor %%eax, %%eax\nint $" _SYSCALL_INT_S : "=a" (ret) : "0" (num), "b" (arg0) : "%eax")
#define SYSCALL_RET2(num, ret, arg0, arg1) asm volatile("xor %%eax, %%eax\nint $" _SYSCALL_INT_S : "=a" (ret) : "0" (num), "b" (arg0), "c" (arg1) : "%eax")
#define SYSCALL_RET3(num, ret, arg0, arg1, arg2) asm volatile("xor %%eax, %%eax\nint $" _SYSCALL_INT_S : "=a" (ret) : "0" (num), "b" (arg0), "c" (arg1), "d" (arg2) : "%eax")
#define SYSCALL_RET4(num, ret, arg0, arg1, arg2, arg3) asm volatile("xor %%eax, %%eax\nint $" _SYSCALL_INT_S : "=a" (ret) : "0" (num), "b" (arg0), "c" (arg1), "d" (arg2), "S" (arg3) : "%eax")
#define SYSCALL_RET5(num, ret, arg0, arg1, arg2, arg3, arg4) asm volatile("xor %%eax, %%eax\nint $" _SYSCALL_INT_S : "=a" (ret) : "0" (num), "b" (arg0), "c" (arg1), "d" (arg2), "S" (arg3), "D" (arg4) : "%eax")

#define SYSCALL0(num) asm volatile("mov %0, %%eax\nint $" _SYSCALL_INT_S : : "r" (num) : "%eax")
#define SYSCALL1(num, arg0) asm volatile("mov %0, %%eax\nint $" _SYSCALL_INT_S : : "r" (num), "b" (arg0) : "%eax")
#define SYSCALL2(num, arg0, arg1) asm volatile("mov %0, %%eax\nint $" _SYSCALL_INT_S : : "r" (num), "b" (arg0), "c" (arg1) : "%eax")
#define SYSCALL3(num, arg0, arg1, arg2) asm volatile("mov %0, %%eax\nint $" _SYSCALL_INT_S : : "r" (num), "b" (arg0), "c" (arg1), "d" (arg2) : "%eax")
#define SYSCALL4(num, arg0, arg1, arg2, arg3) asm volatile("mov %0, %%eax\nint $" _SYSCALL_INT_S : : "r" (num), "b" (arg0), "c" (arg1), "d" (arg2), "S" (arg3) : "%eax")
#define SYSCALL5(num, arg0, arg1, arg2, arg3, arg4) asm volatile("mov %0, %%eax\nint $" _SYSCALL_INT_S : : "r" (num), "b" (arg0), "c" (arg1), "d" (arg2), "S" (arg3), "D" (arg4) : "%eax")

#define EXIT(status) SYSCALL1(SYSCNUM_TASK_EXIT, status);
