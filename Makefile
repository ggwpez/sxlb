SRCDIR := src/
SRCDIRUSER := user/

ASMSOURCES := kernel.asm $(filter-out payload.asm boot.asm kernel.asm ,$(wildcard *.asm))
CPPSOURCES := $(shell find $(SRCDIR) -name '*.cpp')
USERSRCFILES   := $(shell find $(SRCDIRUSER) -name '*.c')
USROBJECTS := $(addsuffix .dat, $(basename $(USERSRCFILES)))
SOURCES := $(ASMSOURCES) $(CPPSOURCES)
OBJECTS := $(addsuffix .o, $(basename $(SOURCES)))

IMAGE := $(SRCDIRUSER)data.img
LIBDIR := lib/

ASFLAGSBIN := -O32 -f bin
ASFLAGSOBJ := -O32 -f elf32
AS := nasm

CXXFLAGS := -m32 -std=c++11 -fpermissive -fno-exceptions -fleading-underscore -fno-rtti -fno-builtin -enable-__cxa_atexit -nostdlib -nostdinc -nodefaultlibs -nostartfiles -w
LDFLAGS := -m elf_i386 -T kernel.ld

all: lib.d boot.bin ckernel.bin OS.bin

lib.d:
	$(MAKE) -C $(LIBDIR)

$(IMAGE):
	$(MAKE) -C $(SRCDIRUSER)

start: all
	bochs -q

debug: all
	bochsdbg -q

payload.o: $(IMAGE) payload.asm
	$(AS) $(ASFLAGSOBJ) payload.asm -o payload.o

boot.bin: boot.asm
	$(AS) $(ASFLAGSBIN) $< -o $@	

%.o: %.asm
	$(AS) $(ASFLAGSOBJ) $< -o $@

ckernel.bin: $(OBJECTS) payload.o
	$(LD) $(LDFLAGS) $+ -o $@

OS.bin: boot.bin ckernel.bin
	cat boot.bin ckernel.bin > OS.bin

map: LDFLAGS += -map kernel.map
map: all	

clean:
	@find . -name '*.o' -delete
	@find . -name '*.a' -delete
	@find . -name '*.dat' -delete
	@find . -name '*.img' -delete
	@find . -name '*.bin' -delete
	$(MAKE) -C $(SRCDIRUSER) clean 
