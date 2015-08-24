[BITS 32]
SECTION .text
extern _ir_event_handler

; ISR
%assign ISR_C 32

%macro ISRs 1
    [global _isr%1]
    _isr%1:
	%if (%1!=8) && (%1<10 || %1>14)
	    push 0
	%endif
	push %1
	jmp ir_common_stub
%endmacro

%assign routine_nr_s 0
%rep ISR_C
    ISRs routine_nr_s
    %assign routine_nr_s routine_nr_s+1
%endrep

; IRQ
%assign IRQ_C 16

%macro IRQs 1
    [global _irq%1]
    _irq%1:
	push 0
	push %1+32
	jmp ir_common_stub
%endmacro

%assign routine_nr_q 0
%rep IRQ_C
    IRQs routine_nr_q
    %assign routine_nr_q routine_nr_q+1
%endrep

; General Interrupt method
ir_common_stub:
    push eax	;Pushad is for plebs, push them manually.
    push ebx	;Things like EFLAGS and so on are automatically
    push ecx	;saved by the CPU
    push edx
    push ebp
    push esi
    push edi

    push ds
    push es
    push fs
    push gs

    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    push esp
    cld
    call _ir_event_handler

_ir_tail:
    mov esp, eax	;get the new cpu_state_t* as return value from the C method

    pop gs
    pop fs
    pop es
    pop ds

    pop edi
    pop esi
    pop ebp
    pop edx
    pop ecx
    pop ebx
    pop eax
    add esp, 8
    iret		;iret also makes an sti

[global _syscall]
_syscall:
	mov eax, [esp+4]
	mov ebx, [esp+8]
	mov ecx, [esp+12]
	mov edx, [esp+16]
	mov esi, [esp+20]
	mov edi, [esp+24]

	int 37		;interrupt for syscalls, maybe change the number
	ret

[global _nop]
_nop:			;nop
    nop
    ret
