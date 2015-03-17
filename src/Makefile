CC=gcc
COMMON_DEPS=logging.c
PLATFORM?=-m64

all: desock.so defork.so dealarm.so patch.so derand.so desrand.so

desock.so: desock.c $(COMMON_DEPS)
	bash build_so.sh $@ $(PLATFORM) $^ -lpthread

defork.so: defork.c $(COMMON_DEPS)
	bash build_so.sh $@ $(PLATFORM) $^

dealarm.so: dealarm.c $(COMMON_DEPS)
	bash build_so.sh $@ $(PLATFORM) $^

derand.so: derand.c $(COMMON_DEPS)
	bash build_so.sh $@ $(PLATFORM) $^

desrand.so: desrand.c $(COMMON_DEPS)
	bash build_so.sh $@ $(PLATFORM) $^ -ldl

patch.so: patch.c $(COMMON_DEPS)
	bash build_so.sh $@ $(PLATFORM) $^ -lini_config

clean:
	rm -f *.o
	rm -f *.so
