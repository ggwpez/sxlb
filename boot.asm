org 0x7C00  ; set up start address of bootloader

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; setup a stack and segment regs ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	mov ax, 0x1114	;set text-mode 80*25
	;mov ax, 0x1112	;set text-mode 80*50
	;mov ax, 0x13	;320x200 256 color

	int 0x10 

    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ax, 0x7000   ; stack address 
    mov ss, ax       
    xor sp, sp       ; set stackpointer to 0 
		
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; read kernel from floppy disk ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

    mov [bootdrive], dl ; boot drive stored by BIOS in DL. 
                        ; we save it to [bootdrive] to play for safety.

load_kernel:
    ;xor ax, ax         ; mov ax, 0  => function "reset"
    ;int 0x13          
    ;jc load_kernel     ; trouble? try again

    mov bx, 500
	mov di, bx 
	mov bx, 0x0800 ; kernel start address (cf. linker script)
	mov ch, 0
	mov dh, 0
	mov cl, 2
	      
	call read_kernel

	jmp 0x8000   ; address of asm-kernel
	ret
	
read_kernel:

.reset_floppy:
    xor ax, ax        ; mov ax, 0  => function "reset"
    int 0x13          
    jc .reset_floppy  ; trouble? try again

.next:
    mov es, bx
    xor bx, bx

.again:
    mov dl, [bootdrive]
    mov ax, 0x0201
    int 0x13
    jc .again

    inc cl
    cmp cl, 19
    jl .loop
    mov cl, 1

    inc dh
    cmp dh, 2
    jl .loop
    mov dh, 0

    inc ch
    cmp ch, 80
    jae .error

.loop:
    mov bx, es
    add bx, 512/16
    sub di, 1
    jnz .next

.done:
    push dx
    mov dx,0x3F2      ; switch off floppy disk motor
    mov al,0x0C
    out dx,al
    pop dx
    ret   

.error:
	hlt
.end:
    jmp .end 

    bootdrive db 0    ; boot drive
    
    times 510-($-$$) hlt 
    db 0x55
    db 0xAA
