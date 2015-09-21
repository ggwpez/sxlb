LIBDIR := ../../lib/bin/
LIBS := $(LIBDIR)sxlblib.a $(LIBDIR)stdlibc.a $(LIBDIR)posixc.a

CSRC := $(wildcard *.c)
COBJ := $(addsuffix .o,$(basename $(CSRC)))

LD := ld

CCFLAGS := -m32 -c -std=gnu99 -march=i386 -Wshadow -w -nostdinc -fno-pic -fno-builtin -fno-stack-protector -fomit-frame-pointer -fno-common -I../../lib/stdlibc/
LDFLAGS := -m elf_i386 -u _start -nostdlib --warn-common -s

OUT := ../c_test.dat

all: $(OUT)

$(OUT): $(COBJ) $(LIBS)
	$(LD) $(COBJ) $(LIBS) -T user.ld $(LDFLAGS) -o $(OUT)

%.o: %.c
	$(CC) $(CCFLAGS) $< -o $@

clean:
	@find . -name '*.o' -delete
