ENTRY(_start)
OUTPUT_FORMAT(elf32-i386)
 
SECTIONS
{
    . = 0x401000;
    
    .text :
    {
      *(.text*)
    }
    .data :
    {
      *(.data*)
      *(.rodata*)
    }
    .bss :
    {
      *(.bss*)
    }
    _end = .;
}