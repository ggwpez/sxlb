[BITS 16]
[global RealMode]
[extern _main]		    	;cpp-kernel main

RealMode:
	xor ax, ax
	mov es, ax
	mov ds, ax
	mov ss, ax
	mov sp, ax

	add sp, -0x40		;64byte stack pointer, for input buffer

	cli

	lgdt [gdtr]         	;load gdt.inc into gdt-register

	in  al, 0x92	   	;some A20 gate poop, wont look at this again, so no comments
	cmp al, 0xff
	je .no_fast_A20

	or  al, 2
	and al, ~1
	out 0x92, al
	jmp .pm_init

.no_fast_A20:
	call empty_8042

	mov al, 0xD1
	out 0x64, al
	call empty_8042

	mov al, 0xDF
	out 0x60, al
	call empty_8042

	.pm_init:
	mov eax, cr0
	or  eax, 1
	mov cr0, eax		;set pm bit in CR0

	jmp 0x8:pm ;http://www.nasm.us/doc/nasmdo10.hpptml#section-10.1

empty_8042:             	;this hopefully 'just werks'
	call wait_
	in al, 0x64
	cmp al, 0xff
	je .done

	test al, 1
	jz .no_output
	call wait_
	in al, 0x60
	jmp empty_8042

.no_output:
	test al, 2
	jnz empty_8042
.done:
ret

[Bits 32]

pm:                     	;= protected mode
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

wait_:                  	;somehow i cant just call it 'wait'
	mov ebx, 0x9FFFF
.loop_start:
	dec ebx
	jnz .loop_start
	ret

%include "gdt.inc"      	;append gdt
