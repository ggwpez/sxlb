[BITS 32]
extern main

[GLOBAL _start]

_start:
	cli
	hlt
	call main

	mov ebx, eax	;return eax->ebx
	mov eax, 15		;exit(main)
	int 0x7f
	jmp $