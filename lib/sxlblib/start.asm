[BITS 32]
extern main

[GLOBAL _start]

extern _init_signal
extern _exit
extern fflush			; actually somebody was so intelligent to compile with -fno_leading_underscore

_start:
	push ebx
	push eax

    call _init_signal

	call main

	push 0
	call fflush
	add esp, 4

	call _exit	
	jmp $

[BITS 32]
[GLOBAL _task_sig_trap]

[SECTION .text]
extern raise

_task_sig_trap:
    pop dword [save_ret]
    pop dword [save_sig]

    pushad
    push dword [save_sig]
    call raise
    popad

    push dword [save_ret]
    ret 0

[SECTION .bss]
save_sig:   resb    4
save_ret:   resb    4
