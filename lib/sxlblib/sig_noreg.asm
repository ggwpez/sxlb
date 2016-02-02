;second version of sig.asm, without RAM access

[BITS 32]
[GLOBAL _task_sig_trap]

[SECTION .text]
extern raise
extern printf

_task_sig_trap:
    xchg bx, bx
    pushad

    mov eax, [esp+32]   ;sig
    mov ebx, [esp+36]   ;ret

    push eax
    call raise
    add esp, 4
    popad

    add esp, 4
    ret