CC = g++
LD = ld
CFLAGS 	 := -m32 -std=c++11 -fpermissive -fno-exceptions -fleading-underscore -fno-rtti -enable-__cxa_atexit -nostdlib -nodefaultlibs -nostartfiles -lgpp -lgpl -lm -lpc -w
LDFLAGS	 := -m elf_i386 -T kernel.ld @linker.txt

# This is a list of all non-source files that are part of the distribution.
SOURCES 	:= $(shell find $(SOURCEDIR) -name '*.cpp')
OBJECTS 	:= $(SOURCES:.cpp=.o)
OUT_FILE 	:= out

all: $(SOURCES) $(OUT_FILE)
    
$(OUT_FILE): $(OBJECTS) 
	$(CC) $(CFLAGS) $(OBJECTS) -o $@ $(LDFLAGS)

