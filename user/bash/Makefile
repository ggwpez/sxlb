LIBDIR := ../../lib/bin/
NEWLIBDIR := ../../lib/bin/i686-sxlb/lib/
USERLIBDIR := ../../lib/sxlblib/
#LIBS := $(LIBDIR)sxlblib.a $(LIBDIR)posixc.a $(LIBDIR)stdlibc.a
LIBS := $(NEWLIBDIR)libc.a $(NEWLIBDIR)libg.a $(NEWLIBDIR)libm.a $(NEWLIBDIR)libnosys.a $(LIBDIR)sxlblib.a $(LIBDIR)posixc.a 

CSRC := $(wildcard *.c)
COBJ := $(addsuffix .o,$(basename $(CSRC)))

LD := i686-sxlb-ld
CC := i686-sxlb-gcc

#CCFLAGS := -m32 -c -std=gnu11 -w -I../../lib/stdlibc/
CCFLAGS := -m32 -c -std=gnu11 -w -fdelete-null-pointer-checks -I$(USERLIBDIR) -I$(NEWLIBDIR)../include/
LDFLAGS := -m elf_i386 -u _start -nostdlib --warn-common

OUT := ../bash.dat

all: $(OUT)

$(OUT): $(COBJ) $(LIBS)
	$(LD) $(COBJ) $(LIBS) -T user.ld $(LDFLAGS) -o $(OUT)

%.o: %.c
	$(CC) $(CCFLAGS) $< -o $@

clean:
	@find . -name '*.o' -delete