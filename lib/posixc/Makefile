BINDIR := bin/
CSRC := $(wildcard ***/*.c) $(wildcard **/*.c) $(wildcard *.c) sys/utsname/uname.c
COBJ := $(addprefix $(BINDIR), $(addsuffix .o, $(basename $(CSRC))))

LD := ld
AS := nasm
AR := i686-elf-ar

CC := i686-elf-gcc
CCFLAGS := -I. -m32 -c -std=gnu11 -w
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