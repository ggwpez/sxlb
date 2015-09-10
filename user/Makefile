DIRS := $(wildcard */)
USRSRCFILES := $(patsubst %.c, %.o, $(wildcard **/*.c))
LIBS := $(patsubst %/, %.dat, $(DIRS))

IMAGE := data.img
FILES := c_test.dat $(wildcard *.dat)

FS := fs

all: user_programs $(IMAGE)

user_programs: $(LIBS)

$(DIRS):
	$(MAKE) -C $@

$(IMAGE): $(FS) $(FILES)
	./$(FS) $(FILES) $@

$(FS):
	$(MAKE) -C fs_source/

%.dat:
	$(MAKE) -C $(patsubst %.dat, %/, $@)

clean:
	@find . -name '*.dat' -delete
	@find . -name '*.img' -delete