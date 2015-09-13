LIBDIR := ../../lib/bin/
LIBS := $(LIBDIR)sxlblib.a $(LIBDIR)stdlibc.a

CSRC := $(wildcard *.cpp)
COBJ := $(addsuffix .o,$(basename $(CSRC)))

LD := ld

CXX := g++ 
CCXXFLAGS := -m32 -c -std=c99 -march=i386 -w -nostdinc -fno-pic -fpermissive -fno-builtin -fno-stack-protector -fomit-frame-pointer -fno-common -I../../lib/stdlibc/
LDFLAGS := -m elf_i386 -u _start -nostdlib --warn-common -nmagic -gc-sections -s

OUT := ../console.dat

all: $(OUT)

$(OUT): $(COBJ) $(LIBS)
	$(LD) $(COBJ) $(LIBS) -T user.ld $(LDFLAGS) -o $(OUT)

%.o: %.cpp
	$(CXX) $(CCXXFLAGS) $< -o $@

clean:
	@find . -name '*.o' -delete
