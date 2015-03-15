CC=gcc
COMMON_DEPS=logging.c
PLATFORM?=-m64

all: desock.so defork.so dealarm.so

desock.so: desock.c $(COMMON_DEPS)
	bash build_so.sh $@ $(PLATFORM) $^ -lpthread

defork.so: defork.c $(COMMON_DEPS)
	bash build_so.sh $@ $(PLATFORM) $^

dealarm.so: dealarm.c $(COMMON_DEPS)
	bash build_so.sh $@ $(PLATFORM) $^

clean:
	rm -f *.o
	rm -f *.so
