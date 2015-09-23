[BITS 32]
extern main

[GLOBAL _start]
extern exit

_start:
	push ebx
	push eax
	call main

	push eax
	call exit
	;mov ebx, eax
	;mov eax, 15
	;int 0x7f	
	jmp $