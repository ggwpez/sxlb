
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; HenkesSoft 0.04 (version from Mar 26, 2009) ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;org 0x10000    ; code's start offset
[BITS 16]		; 16 Bit Code
[global RealMode]
[extern _main]	; this is in the c file

;;;;;;;;;;;;;
; Real Mode ;
;;;;;;;;;;;;;

RealMode:
    xor ax, ax         ; set up segments
    mov es, ax
    mov ds, ax
    mov ss, ax
    mov sp, ax

    add sp, -0x40     ; make room for input buffer (64 chars)
    
pm:
    cli               ; clear interrupts

    lgdt [gdtr]       ; load GDT via GDTR (defined in file "gtd.inc")
	
    in  al, 0x92      ; switch A20 gate via fast A20 port 92
    cmp al, 0xff      ; if it reads 0xFF, nothing's implemented on this port
    je .no_fast_A20
    
    or  al, 2         ; set A20_Gate_Bit (bit 1)
    and al, ~1        ; clear INIT_NOW bit (don't reset pc...)
    out 0x92, al
    jmp .A20_done
    
.no_fast_A20:         ; no fast shortcut -> use the slow kbc...
    call empty_8042   
    
    mov al, 0xD1      ; kbc command: write to output port
    out 0x64, al
    call empty_8042
    
    mov al, 0xDF      ; writing this to kbc output port enables A20
    out 0x60, al
    call empty_8042

.A20_done:
    mov eax, cr0      ; switch-over to Protected Mode
    or  eax, 1        ; set bit 0 of CR0 register
    mov cr0, eax      ; 

    jmp 0x8:ProtectedMode ; http://www.nasm.us/doc/nasmdo10.hpptml#section-10.1
 
;;;;;;;;;
; Calls ;
;;;;;;;;;

empty_8042:
    call Waitingloop
    in al, 0x64
    cmp al, 0xff      ; ... no real kbc at all?
    je .done
    
    test al, 1        ; something in input buffer?
    jz .no_output
    call Waitingloop
    in al, 0x60       ; yes: read buffer
    jmp empty_8042    ; and try again
    
.no_output:
    test al, 2        ; command buffer empty?
    jnz empty_8042    ; no: we can't send anything new till it's empty
.done:
ret

;;;;;;;;;;;;;;;;;;
; Protected Mode ;
;;;;;;;;;;;;;;;;;;

[Bits 32]

ProtectedMode:
	mov    ax, 0x10
	mov    ds, ax      ; data descriptor --> data, stack and extra segment
	mov    ss, ax            
	mov    es, ax
	xor    eax, eax    ; null desriptor --> FS and GS
	mov    fs, ax
	mov    gs, ax
	mov    esp, 0x200000 ;tricky shit, must equal placement address !!!
	
	call _main ; ->-> C-Kernel
	jmp $
 
Waitingloop:                    
    mov ebx,0x9FFFF
.loop_start:
    dec ebx      
    jnz .loop_start
    ret 

;;;;;;;;;;;;
; Includes ;
;;;;;;;;;;;;

%include "gdt.inc"