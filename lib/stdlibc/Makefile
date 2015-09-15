BINDIR := bin/
CSRC := $(wildcard **/*.c) $(wildcard *.c)
COBJ := $(addprefix $(BINDIR), $(addsuffix .o, $(basename $(CSRC))))

LD := ld
AS := nasm

CC := gcc
CCFLAGS := -I. -m32 -c -std=gnu99 -w -ffunction-sections -fdata-sections -fno-pic -nostdinc -fno-builtin -fno-stack-protector -fomit-frame-pointer -fno-common
ARFLAGS := rs

OUT := ../bin/stdlibc.a

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