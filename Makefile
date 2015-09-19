SRCDIR := src/
SRCDIRUSER := user/

ASMSOURCES := $(filter-out payload.asm ,$(wildcard *.asm))
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

#CXX := g++
#CXXFLAGS := -m32 -fpermissive -w
CXXFLAGS := -m32 -std=c++11 -fpermissive -ffreestanding -fno-exceptions -fleading-underscore -fno-rtti -fno-builtin -enable-__cxa_atexit -nostdlib -nostdinc -nodefaultlibs -nostartfiles -w
LDFLAGS := -m elf_i386 -T linker.ld

all: lib.target user.target boot.o OS.iso

test: lib.target user.target boot.o OS.bin

user.target: 
	$(MAKE) -C $(SRCDIRUSER)

lib.target:
	$(MAKE) -C $(LIBDIR)

$(IMAGE):
	$(MAKE) -C $(SRCDIRUSER)

start: all
	qemu-system-i386 -hda OS.iso -d cpu_reset -no-reboot

payload.o: $(IMAGE) payload.asm
	$(AS) $(ASFLAGSOBJ) payload.asm -o payload.o

boot.o: boot.s
	gcc -m32 boot.s -o boot.o -c	

%.o: %.asm
	$(AS) $(ASFLAGSOBJ) $< -o $@

OS.bin: boot.o $(OBJECTS) payload.o
	$(LD) $(LDFLAGS) $+ -o $@

OS.iso: OS.bin
	mkdir -p isodir/boot/grub
	cp grub.cfg isodir/boot/grub/grub.cfg 
	cp OS.bin isodir/boot/OS.bin
	grub-mkrescue -o OS.iso isodir

map: LDFLAGS += -map kernel.map
map: all	

clean:
	@find . -name '*.o' -delete
	@find . -name '*.a' -delete
	@find . -name '*.dat' -delete
	@find . -name '*.img' -delete
	@find . -name '*.iso' -delete
	@find . -name '*.bin' -delete
	$(MAKE) -C $(SRCDIRUSER) clean 
