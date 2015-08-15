[BITS 32]
extern _ir_event_handler

%assign ISR_C 32

; Template for a single interrupt routine:
%macro ISRs 1
    [global _isr%1]
    _isr%1:
	%if (%1!=8) && (%1<10 || %1>14)
	    push dword 0
	%endif
	push dword %1
	jmp ir_common_stub
%endmacro

; Create the interrupt-routines
%assign routine_nr_s 0
%rep ISR_C
    ISRs routine_nr_s
    %assign routine_nr_s routine_nr_s+1
%endrep

%assign IRQ_C 16

%macro IRQs 1
    [global _irq%1]
    _irq%1:
	push dword 0
	push dword %1+32
	jmp ir_common_stub
%endmacro

; Create the interrupt-routines
%assign routine_nr_q 0
%rep IRQ_C
    IRQs routine_nr_q
    %assign routine_nr_q routine_nr_q+1
%endrep

extern _irq_event_handler
[global _irq_tail]

ir_common_stub:
 ; CPU-Zustand sichern
    push eax
    push ebx
    push ecx
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

_ir_tail:
    mov esp, eax

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

;    // Ruecksprung zum unterbrochenen Code
    iret

[global _syscall]
_syscall:
	mov eax, [esp+4]
	mov ebx, [esp+8]
	mov ecx, [esp+12]
	mov edx, [esp+16]
	mov esi, [esp+20]
	mov edi, [esp+24]

	int 37
	ret

[global _nop]
_nop:
    nop
    ret
