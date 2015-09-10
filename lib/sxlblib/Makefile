ASSRC := $(wildcard *.asm)
ASOBJ := $(addsuffix .o,$(basename $(ASSRC)))

AS := nasm

ASFLAGS := -Ox -f elf
ARFLAGS := rs

OUT := ../bin/sxlblib.a

all: $(OUT)

$(OUT): $(ASOBJ)
	$(AR) $(ARFLAGS) $(OUT) $(ASOBJ)

%.o: %.asm
	$(AS) $< $(ASFLAGS) -o $@

clean:
	@find . -name '*.o' -delete
	@find . -name '*.a' -delete