ASMSOURCES := $(wildcard *.asm)
SOURCES := $(ASMSOURCES) $(CPPSOURCES)
FSFILES :=
OBJECTS := $(FSFILES) $(addsuffix .o,$(basename $(SOURCES)))

ASFLAGSBIN := -O32 -f bin
NASM := nasm
FS := ./fs

all: $(OBJECTS) data.img

data.img: $(OBJECTS)
	@$(FS) $(OBJECTS) $@

%.o: %.asm
	@$(NASM) $(ASFLAGSBIN) $< -o $@

clean:
	@find . -name '*.dat' -delete
