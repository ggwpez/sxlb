SECTION .text
[extern _main]		    	;cpp-kernel main
[GLOBAL protected_mode]
[Bits 32]

protected_mode:                     	;= protected mode
	mov    ax, 0x10
	mov    ds, ax      	;set all segment registers up
	mov    ss, ax
	mov    es, ax

	xor    ax, ax
	mov    fs, ax      	;gp registers remain 0
	mov    gs, ax
	mov    esp, 0x200000 	;tricky shit, must equal placement address !!!

	call _main		;right into the cpp world!
	jmp $			;stop after _main returns
