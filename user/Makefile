ASMSOURCES := $(wildcard *.asm)
SOURCES := $(ASMSOURCES) $(CPPSOURCES)
OBJECTS := $(addsuffix .dat,$(basename $(SOURCES)))

ASFLAGSBIN := -O32 -f bin
NASM := nasm

all: $(OBJECTS)

%.dat: %.asm
	$(NASM) $(ASFLAGSBIN) $< -o $@

clean:
	@find . -name '*.dat' -delete
