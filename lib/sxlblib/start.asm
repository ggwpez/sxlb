[BITS 32]
extern main

[GLOBAL _start]

;extern _init_signal
extern _exit
extern fflush			; actually somebody was so intelligent to compile with -fno_leading_underscore

_start:
	push ebx
	push eax

        ;call _init_signal

	call main

	push 0
	call fflush
	add esp, 4

	call _exit	
	jmp $
