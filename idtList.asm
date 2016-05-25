[BITS 32]
SECTION .text
extern _ir_event_handler

; ISR
%assign ISR_C 32

%macro ISRs 1
    [global _isr%1]
    _isr%1:
	cli
	%if (%1!=8) && (%1<10 || %1>14) && (%1!=17 && %1!=30)
	    push 0
	%endif
	push %1
	jmp ir_common_stub
%endmacro

[global  _isr126]
_isr126:
	cli
	push 0
	push 126
	jmp ir_common_stub

[global  _isr127]
_isr127:
	cli
	push 0
	push 127
	jmp ir_common_stub


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
	cli
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
    call _ir_event_handler
[GLOBAL _ir_tail]
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

[GLOBAL _nop]
_nop:
	nop
	ret
