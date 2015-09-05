;first user program to be loaded by the kernel

[BITS 32]
main:
	xor eax, ebx
	mov ebx, eax

	mov al, 2 		;putc('1')
	mov ebx, msg
	int 0x7f

	mov al, 15
	xor ebx, ebx	;exit(0)
	int 0x7f
	ret

msg: db 0x64,0