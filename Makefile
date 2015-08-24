ASMSOURCES := kernel.asm cpu.asm flush.asm idtList.asm
SOURCES := $(shell find $(src) -name '*.cpp') $(ASMSOURCES)
OBJECTS := $(addsuffix .o,$(basename $(SOURCES)))

ASFLAGSBIN := -O32 -f bin
ASFLAGSOBJ := -O32 -f elf32
NASM := nasm

CXXFLAGS := -m32 -std=c++11 -fpermissive -fno-exceptions -fleading-underscore -fno-rtti -fno-builtin -enable-__cxa_atexit -nostdlib -nodefaultlibs -nostartfiles
LDFLAGS := -m elf_i386 -T kernel.ld

all: boot.bin ckernel.bin image	

boot.bin: boot.asm
	$(NASM) $(ASFLAGSBIN) $< -o $@

%.o: %.asm
	$(NASM) $(ASFLAGSOBJ) $< -o $@

ckernel.bin: $(OBJECTS)
	$(LD) $(LDFLAGS) $+ -o $@

process.asm: initrd.img	
	
image:
	cat boot.bin ckernel.bin > OS.bin

clean:
	find . -name '*.o' -delete
	find . -name '*.bin' -delete

#    $<		Erste Abhängigkeit
#    $+		Liste aller Abhängigkeiten	
#    $@		Name des Targets