OUTPUT_FORMAT("binary")
ENTRY(RealMode)
SECTIONS
{
  .text 0x8000 : { *(.text) }
  .data        : { *(.data) }
  .bss         : { *(.bss)  }
  _kernel_end = .;
}