DIRS := $(wildcard */)
USRSRCFILES := $(patsubst %.c, %.o, $(wildcard **/*.c))
EXECS := $(patsubst %/, %.target, $(DIRS))

IMAGE := data.img
FILES := $(wildcard *.dat)

FS := fs

all: $(EXECS) $(IMAGE)

user_programs: $(EXECS)

$(DIRS):
	$(MAKE) -C $@

$(IMAGE): $(FS) $(FILES)
	./$(FS) $(FILES) $@

$(FS):
	$(MAKE) -C fs_source/

%.target:
	$(MAKE) -C $(patsubst %.target, %/, $@)

clean:
	@find . -name '*.dat' -delete
	@find . -name '*.img' -delete