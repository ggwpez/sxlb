DIRS := $(filter-out bin/, $(wildcard */))
SRCFILES := $(patsubst %.c, %.o, $(wildcard **/*.c))
LIBS := $(patsubst %/, /bin/%.a, $(DIRS))

all: $(LIBS)

/bin/%.a:
	$(MAKE) -C $(patsubst /bin/%.a, %/, $@)

clean:
	@find ./bin/ -name '*.a' -delete
	@find ./bin/ -name '*.o' -delete