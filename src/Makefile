.PHONY: all dist clean

PLATFORM?=

ifndef MACHINE
  MACHINE=$(shell $(CC) -dumpmachine)
else
  ifeq ($(CC),cc)
    CC=$(MACHINE)-gcc
  endif
endif

ifneq (,$(findstring linux,$(MACHINE)))
  CFLAGS:=$(CFLAGS) -DLINUX -ldl
else
ifneq (,$(findstring freebsd,$(MACHINE)))
  CFLAGS:=$(CFLAGS) -Dfreebsd
else
ifneq (,$(findstring -elf,$(MACHINE)))
  CFLAGS:=$(CFLAGS) -mabicalls
endif
endif
endif

#$(info $(MACHINE) $(CC))

ALL_MODULES=$(filter-out logging.c, $(patsubst %.c,%.so,$(wildcard *.c)))

all: $(ALL_MODULES)

COMMON_DEPS=logging.c
desock.so: CFLAGS+=-lpthread
desock_dup.so: CFLAGS+=-ldl
desrand.so: CFLAGS+=-ldl
deptrace.so: CFLAGS+=-ldl
mallocwatch.so: CFLAGS+=-ldl
crazyrealloc.so: CFLAGS+=-ldl
patch.so: CFLAGS+=-ldl -lini_config

%.so: %.c $(COMMON_DEPS)
	$(CC) $^ -o $@ -shared -fPIC $(CFLAGS)

dist: all
	mkdir -p ../$(MACHINE)
	cp *.so ../$(MACHINE)

clean:
	rm -f *.o
	rm -f *.so
