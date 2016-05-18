;multiboot header according to standart 1.6 aka multiboot2

MAGIC       equ      0xE85250D6
ARCH        equ      0x0
HDRLEN      equ      mbh_end -mbh
CHECKSUM    equ      -(MAGIC + ARCH + HDRLEN) 

mbh:
  dd   MAGIC
  dd   ARCH
  dd   HDRLEN
  dd   CHECKSUM

;Info request tag
mb2_info_req:
  dw 1
  dw 0
  dd (mb2_info_req_end -mb2_info_req)
  dd 4
  dd 6
mb2_info_req_end:
  
  align 8
  dw 6
  dw 0
  dd 8

  ;Console tag
  align 8
  dw 4
  dw 1
  dd 12
  dd 2

  ;Graphics
  align 8
  dw 5
  dw 1
  dd 20
  dd 800
  dd 600
  dd 24

  ;End
  align 8
  dw 0
  dw 0
  dd 8
mbh_end: