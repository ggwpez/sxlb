[BITS 32]
[GLOBAL _task_sig_trap]

[SECTION .text]
extern raise
extern do_not
extern printf

_task_sig_trap:
	xchg bx, bx
;	mov dword [save_eax], eax

;	add esp, 8
;	mov eax, dword [ss:eax]
;	mov dword [save_ret], eax

;	pop dword eax
;	mov eax, dword [ss:eax]
;	mov dword [save_sig], eax
	push str
	call printf
	jmp $

	pushad
	push dword [save_sig]
	;push dword 1eh
	call raise
	add esp, 4
	popad

	jmp dword [save_ret]

[SECTION .data]
str: db "got: 0x%x and 0x%x",10,13, 0

[SECTION .bss]
save_sig:   resb    4
save_ret:   resb    4
save_eax:	resb	4
