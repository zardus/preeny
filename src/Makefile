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

ALL_MODULES=desock.so defork.so dealarm.so patch.so derand.so desrand.so deptrace.so desock_dup.so startstop.so writeout.so ensock.so mallocwatch.so
all: $(ALL_MODULES)

COMMON_DEPS=logging.c
desock.so: desock.c $(COMMON_DEPS)
	$(CC) $^ -o $@ -shared -fPIC $(CFLAGS) -lpthread

desock_dup.so: desock_dup.c $(COMMON_DEPS)
	$(CC) $^ -o $@ -shared -fPIC $(CFLAGS) -ldl

defork.so: defork.c $(COMMON_DEPS)
	$(CC) $^ -o $@ -shared -fPIC $(CFLAGS)

startstop.so: startstop.c $(COMMON_DEPS)
	$(CC) $^ -o $@ -shared -fPIC $(CFLAGS)

dealarm.so: dealarm.c $(COMMON_DEPS)
	$(CC) $^ -o $@ -shared -fPIC $(CFLAGS)

derand.so: derand.c $(COMMON_DEPS)
	$(CC) $^ -o $@ -shared -fPIC $(CFLAGS)

ensock.so: ensock.c $(COMMON_DEPS)
	$(CC) $^ -o $@ -shared -fPIC $(CFLAGS)

writeout.so: writeout.c $(COMMON_DEPS)
	$(CC) $^ -o $@ -shared -fPIC $(CFLAGS)

desrand.so: desrand.c $(COMMON_DEPS)
	$(CC) $^ -o $@ -shared -fPIC $(CFLAGS) -ldl

deptrace.so: deptrace.c $(COMMON_DEPS)
	$(CC) $^ -o $@ -shared -fPIC $(CFLAGS) -ldl

mallocwatch.so: mallocwatch.c $(COMMON_DEPS)
	$(CC) $^ -o $@ -shared -fPIC $(CFLAGS) -ldl

patch.so: patch.c $(COMMON_DEPS)
	$(CC) $^ -o $@ -shared -fPIC $(CFLAGS) -ldl -lini_config

dist: all
	mkdir -p ../$(MACHINE)
	cp *.so ../$(MACHINE)

clean:
	rm -f *.o
	rm -f *.so
