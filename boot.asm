%include "multiboot2.asm"

[global _start]
[extern _main]
[global _kernel_start]
[global _kernel_end]

[section .text]

_start:
    mov esp, 0x200000

    push eax
    push ebx

    call _main

    add esp, 8

    cli ; falls der Kernel bis hier her kommt, CPU anhalten
    hlt
