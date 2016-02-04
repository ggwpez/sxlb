[BITS 32]
[GLOBAL _task_sig_trap]

[SECTION .text]
extern raise
extern printf

_task_sig_trap:
	xchg bx, bx
	mov dword [save_eax], eax
	mov dword [save_ebx], ebx

    mov eax, dword [esp-16]		;ret
    mov ebx, dword [esp-20]		;sig
    
    mov dword [save_ret], eax
    mov dword [save_sig], ebx

    mov eax, dword [save_eax]
	mov ebx, dword [save_ebx]

    pushad
    ;push dword [save_sig]
    push 1eh
    call raise
    add esp, 4
    popad

    jmp dword [save_ret]

[SECTION .bss]
save_sig:   resb    4
save_ret:   resb    4
save_eax: 	resb	4
save_ebx:	resb	4