[BITS 32]
extern main

[GLOBAL _start]
extern exit

_start:
	call main

	push eax
	call exit	;== exit(main())
	jmp $