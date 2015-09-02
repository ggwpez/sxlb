section .text

; flush.asm -- contains global descriptor table and interrupt descriptor table setup code

GLOBAL _gdt_flush    ; Allows the C code to call gdt_flush().

_gdt_flush:
    mov eax, [esp+4]  ; Get the pointer to the GDT, passed as a parameter.
    lgdt [eax]        ; Load the new GDT register

    mov ax, 0x10      ; 0x10 is the offset in the GDT to our data segment
    mov ds, ax        ; Load all data segment selectors
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
	jmp 0x08:.flush   ; 0x08 is the offset to our code segment: Far jump!
.flush:
    ret

GLOBAL _idt_flush    ; Allows the C code to call idt_flush().

_idt_flush:
    mov eax, [esp+4]  ; Get the pointer to the IDT, passed as a parameter. 
    lidt [eax]        ; Load the IDT register
    ret

GLOBAL _tss_flush    ; Allows our C code to call tss_flush().

_tss_flush:
   mov ax, 40        ; Load the index of our TSS structure - The index is
                     ; 0x28, as it is the 5th selector and each is 8 bytes
                     ; long, but we set the bottom two bits (making 0x2B)
                     ; so that it has an RPL of 3, not zero.
   ltr ax            ; Load 0x2B into the task state register.
   ret


GLOBAL _copy_page_physical
_copy_page_physical:
   push ebx              ; According to __cdecl, we must preserve the contents of EBX.
   pushf                 ; push EFLAGS, so we can pop it and reenable interrupts
                         ; later, if they were enabled anyway.
   cli                   ; Disable interrupts, so we aren't interrupted.
                         ; Load these in BEFORE we disable paging!
   mov ebx, [esp+12]     ; Source address
   mov ecx, [esp+16]     ; Destination address

   mov edx, cr0          ; Get the control register...
   and edx, 0x7fffffff   ; and...
   mov cr0, edx          ; Disable paging.

   mov edx, 1024         ; 1024*4bytes = 4096 bytes to copy

.loop:
   mov eax, [ebx]        ; Get the word at the source address
   mov [ecx], eax        ; Store it at the dest address
   add ebx, 4            ; Source address += sizeof(word)
   add ecx, 4            ; Dest address += sizeof(word)
   dec edx               ; One less word to do
   jnz .loop

   mov edx, cr0          ; Get the control register again
   or  edx, 0x80000000   ; and...
   mov cr0, edx          ; Enable paging.

   popf                  ; Pop EFLAGS back.
   pop ebx               ; Get the original value of EBX back.
   ret
