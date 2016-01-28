[BITS 32]
[GLOBAL _task_sig_trap]

[SECTION .text]
extern raise
extern do_not

_task_sig_trap:
    mov eax, 2
    mov ebx, 0
    div ebx
    pop dword [save_ret]
    pop dword [save_sig]

    ;pushad
    ;push dword [save_sig]
    ;call raise
    ;add esp, 4
    ;popad

    ;jmp far dword [save_ret]
    jmp $

[SECTION .bss]
save_sig:   resb    4
save_ret:   resb    4
