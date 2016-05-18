;multiboot header according to standart 1.6 aka multiboot2

%define VIDEO

WIDTH	    equ	    800
HEIGHT	    equ	    600
BPP	    equ	    24

MAGIC       equ	    0xE85250D6
ARCH        equ	    0x0
HDRLEN      equ	    mbh_end -mbh
CHECKSUM    equ	    -(MAGIC + ARCH + HDRLEN)

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

    ; Module alignment
    align 8
    dw 6
    dw 0
    dd 8

    ;Flags tag
    align 8
    dw 4
    dw 1
    dd 12
    dd 2	    ; We do have EGA-text support

%ifdef VIDEO
    align 8
    dw 5
    dw 1
    dd 20
    dd WIDTH
    dd HEIGHT
    dd BPP
%endif

    ;End
    align 8
    dw 0
    dw 0
    dd 8
mbh_end:
