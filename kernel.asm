SECTION .text
[extern _main]		    	;cpp-kernel main
[GLOBAL protected_mode]
[Bits 32]

[GLOBAL _kernel_start]
_kernel_start:			;this is needed for calculating the kernel's size
protected_mode:
	mov    ax, 0x10
	mov    ds, ax      	;set up all segment registers
	mov    ss, ax
	mov    es, ax

	xor    ax, ax
	mov    fs, ax      	;gp registers remain 0
	mov    gs, ax
	mov    esp, 0x200000 	;tricky shit: must equal placement address !!!

	call _main		;right into the cpp world!
	jmp $			;stop after main returns