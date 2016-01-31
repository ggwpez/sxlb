SRCDIR := src/
SRCDIRUSER := user/

ASMSOURCES := $(filter-out payload.asm ,$(wildcard *.asm))
CPPSOURCES := $(shell find $(SRCDIR) -name '*.cpp')
USERSRCFILES := $(shell find $(SRCDIRUSER) -name '*.c')
USROBJECTS := $(addsuffix .dat, $(basename $(USERSRCFILES)))
SOURCES := $(ASMSOURCES) $(CPPSOURCES)
OBJECTS := $(addsuffix .o, $(basename $(SOURCES)))
BUILD_NUMBER_FILE=build_number.md
BINDIR := /tmp/						#mounted /tmp/ in the ram for faster iso building

IMAGE := $(SRCDIRUSER)data.img
GRUBCFG := isodir/boot/grub/grub.cfg
LIBDIR := lib/

ASFLAGSBIN := -O32 -f bin
ASFLAGSOBJ := -O32 -f elf32
AS := nasm

CXX := g++
LD := ld
#CXXFLAGS := -m32 -fpermissive -w
CXXFLAGS := -m32 -std=c++11 -fpermissive -ffreestanding -fno-exceptions -fleading-underscore -fno-rtti -fno-builtin -enable-__cxa_atexit -nostdlib -nostdinc -nodefaultlibs -nostartfiles -w
LDFLAGS := -m elf_i386 -T linker.ld

all: | lib.target user.target boot.o $(BINDIR)/OS.bin build_number.target 

test: | lib.target user.target boot.o OS.bin

user.target: 
	$(MAKE) -C $(SRCDIRUSER)

lib.target:
	$(MAKE) -C $(LIBDIR)

$(IMAGE):
	$(MAKE) -C $(SRCDIRUSER)

bochs: all
	bochs -q

bochsdbg: all
	bochsdbg -q

qemu: all
	qemu-system-i386 -hda $(BINDIR)/OS.bin -d cpu_reset -no-reboot

payload.o: $(IMAGE) payload.asm
	$(AS) $(ASFLAGSOBJ) payload.asm -o payload.o

#boot.o: boot.s
#	i686-elf-g++ -m32 boot.s -o boot.o -c	
boot.o: boot.mbh
	nasm -f elf -o boot.o boot.mbh


%.o: %.asm
	$(AS) $(ASFLAGSOBJ) $< -o $@

OS.bin: boot.o $(OBJECTS) payload.o
	$(LD) $(LDFLAGS) $(BUILD_NUMBER_LDFLAGS) $+ -o $@

$(GRUBCFG): grub.cfg
	cp grub.cfg $@

isodir/boot/grub:
	mkdir -p $@

isodir/boot/OS.bin: OS.bin
	cp OS.bin $@

$(BINDIR)/OS.bin: $(GRUBCFG) isodir/boot/grub isodir/boot/OS.bin
	grub-mkrescue -o $(BINDIR)/OS.bin isodir
	#grub-mkrescue -o $(BINDIR)/OS.bin isodir

map: $(LDFLAGS) += -M kernel.map
map: all

deploy: all
	#sudo dd if=$(BINDIR)/OS.bin of=/dev/sdb
	sync	

clean:
	@find . -name '*.o' -delete
	@find . -name '*.a' -delete
	@find . -name '*.dat' -delete
	@find . -name '*.img' -delete
	@find . -name '*.iso' -delete
	@find . -name '*.bin' -delete
	$(MAKE) -C $(SRCDIRUSER) clean 

include build_number.mak
