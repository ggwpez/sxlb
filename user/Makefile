ASMSOURCES := $(wildcard *.asm)
SOURCES := $(ASMSOURCES) $(CPPSOURCES)
FSSRCDIR := fs_source/
OBJECTS := $(addsuffix .o,$(basename $(SOURCES)))

ASFLAGSBIN := -O32 -f bin
NASM := nasm

all: data.img

data.img: fs $(OBJECTS)
	./fs $(OBJECTS) $@

fs:
	$(MAKE) -C $(FSSRCDIR)

%.o: %.asm
	$(NASM) $(ASFLAGSBIN) $< -o $@

clean:
	@find . -name '*.dat' -delete
	$(MAKE) -C $(FSSRCDIR) clean
