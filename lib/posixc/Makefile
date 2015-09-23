BINDIR := bin/
CSRC := $(wildcard ***/*.c) $(wildcard **/*.c) $(wildcard *.c) sys/utsname/uname.c
COBJ := $(addprefix $(BINDIR), $(addsuffix .o, $(basename $(CSRC))))

LD := ld
AS := nasm
AR := i686-elf-ar

CC := i686-elf-gcc
CCFLAGS := -I. -m32 -c -std=gnu99 -w -ffunction-sections -fdata-sections -fno-pic -nostdinc -fno-builtin -fno-stack-protector -fomit-frame-pointer -fno-common -I../stdlibc
ARFLAGS := rs

OUT := ../bin/posixc.a

all: $(OUT)

$(OUT): $(COBJ)
	mkdir -p $(@D)
	$(AR) $(ARFLAGS) $(OUT) $(COBJ)

bin/%.o: %.c
	mkdir -p $(@D)
	$(CC) $(CCFLAGS) $< -o $@

clean:
	@find . -name '*.o' -delete
	@find . -name '*.a' -delete