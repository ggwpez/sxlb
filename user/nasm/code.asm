section .text

[GLOBAL main]
extern printf

main:
	push msg
	call printf
	add esp, 4
ret

section .data
msg: db 'hey ho!', 10	; \n