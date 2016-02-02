[BITS 32]
[GLOBAL _task_sig_trap]

[SECTION .text]
extern raise
extern printf

_task_sig_trap:
    pop dword [save_sig]
    pop dword [save_ret]

    pushad
    push dword [save_sig]
    call raise
    add esp, 4
    popad

    jmp dword [save_ret]

[SECTION .bss]
save_sig:   resb    4
save_ret:   resb    4
