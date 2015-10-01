ASSRC := $(wildcard *.asm)
CSRC  := $(wildcard *.c)
ASOBJ := $(addsuffix .o,$(basename $(ASSRC)))
COBJ  := $(addsuffix .o, $(basename $(CSRC)))
OBJ   := $(ASOBJ) #$(COBJ)

AS := nasm
AR := i686-elf-ar
CC := i686-elf-gcc
CCFLAGS := -m32 -I /home/vados/src/newlib/newlib/libc/include -c -std=gnu11 -w
ASFLAGS := -Ox -f elf
ARFLAGS := rs

OUT := ../bin/sxlblib.a

all: $(OUT)

$(OUT): $(OBJ)
	mkdir -p $(@D)
	$(AR) $(ARFLAGS) $(OUT) $(OBJ)

%.o: %.asm
	$(AS) $< $(ASFLAGS)	-o $@

%.o: %.c
	$(CC) $< $(CCFLAGS)	-o $@

clean:
	@find . -name '*.o' -delete
	@find . -name '*.a' -delete