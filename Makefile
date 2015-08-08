# This is a list of all non-source files that are part of the distribution.
AUXFILES := Makefile LICENSE.md filex.txt linker.txt

PROJDIRS := src

SRCFILES := $(shell find $(PROJDIRS) -type f -name "\*.cpp")
HDRFILES := $(shell find $(PROJDIRS) -type f -name "\*.hpp")

OBJFILES := $(patsubst %.cpp,%.o,$(SRCFILES))
# changes from .cpp to .d, auxilary files craeted my gcc
DEPFILES    := $(patsubst %.cpp,%.d,$(SRCFILES))

ALLFILES := $(SRCFILES) $(HDRFILES) $(AUXFILES)

CFLAGS 	 := -m32 -std=c++11 -fpermissive -fno-exceptions -fleading-underscore -fno-rtti -enable-__cxa_atexit -nostdlib -nodefaultlibs -nostartfiles -lgpp -lgpl -lm -lpc -w -c