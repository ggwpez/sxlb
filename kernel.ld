OUTPUT_FORMAT("binary")
ENTRY(protected_mode)
SECTIONS
{
  .text 0x8000 : { *(.text) }
  .data        : { *(.data) }
  .bss         : { *(.bss)  }
  _kernel_end = .;
}
