.PHONY: all dist tests clean

all: dist tests

dist:
	make -C src clean
	make -C src dist

tests:
	make -C tests

clean:
	make -C src clean
	make -C tests clean
	rm -rf *-*-*
