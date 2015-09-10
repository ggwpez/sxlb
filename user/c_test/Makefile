LIBDIR := ../../lib/bin/
LIBS := $(wildcard $(LIBDIR)*.a)

CSRC := $(wildcard *.c)
COBJ := $(addsuffix .o,$(basename $(CSRC)))

LD := ld

CCFLAGS := -m32 -c -std=c99 -march=i386 -Wshadow -Wall -nostdinc -fno-pic -fno-builtin -fno-stack-protector -fomit-frame-pointer -fno-common -Iinclude
LDFLAGS := -m elf_i386 -u _start -nostdlib --warn-common -nmagic -gc-sections -s

OUT := ../c_test.dat

all: $(OUT)

$(OUT): $(COBJ) $(LIBS)
	$(LD) $(COBJ) $(LIBS) -T user.ld $(LDFLAGS) -o $(OUT)

%.o: %.c
	$(CC) $(CCFLAGS) $< -o $@

clean:
	@find . -name '*.o' -delete
