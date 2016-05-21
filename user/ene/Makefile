LIBDIR := ../../lib/bin/
NEWLIBDIR := ../../lib/bin/i686-sxlb/lib/
USERLIBDIR := ../../lib/sxlblib/
LIBS := $(NEWLIBDIR)libc.a $(NEWLIBDIR)libg.a $(NEWLIBDIR)libm.a $(NEWLIBDIR)libnosys.a $(LIBDIR)sxlblib.a  $(LIBDIR)posixc.a

CSRC := $(wildcard *.ene)
COBJ := $(addsuffix .obj, $(CSRC))

LD := i686-sxlb-ld
CC := /home/vados/Code/Projects/eneV3/bin/ene

export LOCAL_ALLOW_UNDEFINED_SYMBOLS = true
CCFLAGS := -c -b32 -anasm -t /home/vados/Code/Projects/eneV3/src/templates/
LDFLAGS := -m elf_i386 -u _start -nostdlib --warn-common

OUT := ../ene.dat

all: $(OUT)

$(OUT): $(COBJ) $(LIBS)
	$(LD) $(COBJ) --start-group $(LIBS) --end-group  -T user.ld $(LDFLAGS) -o $(OUT)

%.ene.obj: %.ene
	$(CC) $(CCFLAGS) -i $< -o $@

clean:
	@find . -name '*.o' -delete
	@find . -name '*.nasm' -delete
	@find . -name '*.obj' -delete