DIRS := $(filter-out bin/ newlib/ build-newlib/, $(wildcard */))
SRCFILES := $(patsubst %.c, %.o, $(wildcard **/*.c))
LIBS := $(patsubst %/, /bin/%.a, $(DIRS))

all: | bin build-newlib bin/i686-sxlb $(LIBS)

bin:
	mkdir -p bin

build-newlib: build-newlib/Makefile
	cd build-newlib && make all

#this is ./configure
build-newlib/Makefile:
	mkdir -p build-newlib
	cd build-newlib && ../newlib/configure --prefix=/ --target=i686-sxlb

#this is newlib install
bin/i686-sxlb:
	cd build-newlib && make DESTDIR=../bin/ install
	cd bin && ln -s usr/local/i686-sxlb i686-sxlb			#newlib somehow ignores the --prefix

/bin/%.a:
	$(MAKE) -C $(patsubst /bin/%.a, %/, $@)

clean:
	cd build-newlib && $(MAKE) clean
	rm -rf bin/