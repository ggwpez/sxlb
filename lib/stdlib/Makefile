CSRC := $(wildcard *.c)
COBJ := $(addsuffix .o,$(basename $(CSRC)))

LD := ld
AS := nasm

CCFLAGS := -m32 -c -std=gnu99 -Wall -ffunction-sections -fdata-sections -fno-pic -nostdinc -fno-builtin -fno-stack-protector -fomit-frame-pointer -fno-common
ARFLAGS := rs

OUT := ../bin/stdlib.a

all: $(OUT)

$(OUT): $(COBJ)
	$(AR) $(ARFLAGS) $(OUT) $(COBJ)

%.o: %.c
	$(CC) $(CCFLAGS) $< -o $@

clean:
	@find . -name '*.o' -delete
	@find . -name '*.a' -delete