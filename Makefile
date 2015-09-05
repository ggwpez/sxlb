SRCDIR := src/
SRCDIRUSER := user/

ASMSOURCES := kernel.asm $(filter-out payload.asm boot.asm kernel.asm ,$(wildcard *.asm))
CPPSOURCES := $(shell find $(SRCDIR) -name '*.cpp')
USERSRCFILES   := $(shell find $(SRCDIRUSER) -name '*.asm')
USROBJECTS := $(addsuffix .dat, $(basename $(USERSRCFILES)))
SOURCES := $(ASMSOURCES) $(CPPSOURCES)
OBJECTS := $(addsuffix .o, $(basename $(SOURCES)))
IMAGE := $(SRCDIRUSER)/data.img

ASFLAGSBIN := -O32 -f bin
ASFLAGSOBJ := -O32 -f elf32
NASM := nasm

CXXFLAGS := -m32 -std=c++11 -fpermissive -fno-exceptions -fleading-underscore -fno-rtti -fno-builtin -enable-__cxa_atexit -nostdlib -nostdinc -nodefaultlibs -nostartfiles -w
LDFLAGS := -m elf_i386 -T kernel.ld

all: boot.bin ckernel.bin OS.bin

$(IMAGE): $(USERSRCFILES)
	$(MAKE) -C $(SRCDIRUSER)

start: all
	bochs -q

debug: all
	bochsdbg -q

payload.o: $(IMAGE) payload.asm
	$(NASM) $(ASFLAGSOBJ) payload.asm -o payload.o

boot.bin: boot.asm
	$(NASM) $(ASFLAGSBIN) $< -o $@	

%.o: %.asm
	$(NASM) $(ASFLAGSOBJ) $< -o $@

ckernel.bin: $(OBJECTS) payload.o
	$(LD) $(LDFLAGS) $+ -o $@

OS.bin: boot.bin ckernel.bin
	cat boot.bin ckernel.bin > OS.bin

map: LDFLAGS += -map kernel.map
map: all	

clean:
	find . -name '*.o' -delete
	find . -name '*.bin' -delete