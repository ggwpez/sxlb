LIBDIR := ../../lib/bin/
NEWLIBDIR := ../../lib/bin/i686-sxlb/lib/
LIBS :=  $(NEWLIBDIR)libc.a $(NEWLIBDIR)libg.a $(NEWLIBDIR)libm.a $(NEWLIBDIR)libnosys.a $(LIBDIR)sxlblib.a $(LIBDIR)posixc.a

ASSRC := $(wildcard *.asm)
ASOBJ := $(addsuffix .o,$(basename $(ASSRC)))

LD := i686-elf-ld
AS := nasm

ASFLAGS := -Ox -f elf
LDFLAGS := -m elf_i386 -u _start -nostdlib --warn-common -s

OUT := ../nasm.dat

all: $(OUT)

$(OUT): $(ASOBJ) $(LIBS)
	$(LD) $(ASOBJ) $(LIBS) -T user.ld $(LDFLAGS) -o $(OUT)

%.o: %.asm
	$(AS) $(ASFLAGS) $< -o $@

clean:
	@find . -name '*.o' -delete
