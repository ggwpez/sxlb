DIRS := $(filter-out bin/ newlib/ build-newlib/, $(wildcard */))
SRCFILES := $(patsubst %.c, %.o, $(wildcard **/*.c))
LIBS := $(patsubst %/, /bin/%.a, $(DIRS))

all: | bin build-newlib bin/i686-sxlb $(LIBS)

bin:
	mkdir -p bin

build-newlib: build-newlib/Makefile
	cd build-newlib && make all -j6 CXXFLAGS+='-g' CFLAGS+='-g'

#this is ./configure
build-newlib/Makefile:
	mkdir -p build-newlib
	cd build-newlib && ../newlib/configure --prefix=/ --target=i686-sxlb

#this is newlib install
bin/i686-sxlb:
	cd build-newlib && make DESTDIR=~/Code/Projects/sxlb/lib/bin/ install	#care!!!

/bin/posixc.a: build-newlib bin/i686-sxlb
	$(MAKE) -C $(patsubst /bin/posixc.a, posixc/, $@)

/bin/%.a:
	$(MAKE) -C $(patsubst /bin/%.a, %/, $@)

clean:
	rm -rf build-newlib/
	rm -rf bin/