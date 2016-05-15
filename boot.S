.include "./src/boot/multiboot_def.hpp"

.Lmultiboot2_header:
    /* Magic number indicating a Multiboot2 header. */
    .long   0xe85250d6
    /* Architecture: i386. */
    .long   0
    /* Multiboot2 header length. */
    .long   .Lmultiboot2_header_end - .Lmultiboot2_header
    /* Multiboot2 header checksum. */
    .long   -(0xe85250d6 + 0 + (.Lmultiboot2_header_end - .Lmultiboot2_header))

    /* Multiboot2 tags... */
.Lmultiboot2_info_req:
    /* Multiboot2 information request tag. */
    .short  1
    .short  0
    .long   .Lmultiboot2_info_req_end - .Lmultiboot2_info_req
    .long   4
    .long   6
.Lmultiboot2_info_req_end:

    .align  8
    .short  6
    .short  0
    .long   8 /* Tag size. */

    /* Console flags tag. */
    .align  8
    .short  4
    .short  1
    .long   12 /* Tag size. */
    .long   2

    /* Framebuffer tag. */
    .align  8
    .short  5
    .short  1
    .long   20 /* Tag size. */
    .long   800 /* Number of the columns - no preference. */
    .long   600 /* Number of the lines - no preference. */
    .long   24 /* Number of bits per pixel - no preference. */

    /* Multiboot2 header end tag. */
    .align  8
    .short  0
    .short  0
    .long   8 /* Tag size. */
.Lmultiboot2_header_end:



.section .bootstrap_stack, "aw", @nobits
stack_bottom:
.skip 16384 # 16 KiB
stack_top:


.section .text
.extern _main
.global _start
.type _start, @function
_start:
  cli
  push %eax
  push %ebx
  call _main
  hlt
.Lhang:
  jmp .Lhang
.global _kernel_start
_kernel_start:
.global _kernel_end
_kernel_end:

.size _start, . - _start
