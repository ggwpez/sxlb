;first user program to be loaded by the kernel

[BITS 32]
main:
	mov byte [0xb8000], 'l'
	mov byte [0xb8002], 'o'
	mov byte [0xb8004], 'o'
	xor eax, eax				;ret 0
	jmp $
	ret