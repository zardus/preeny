.PHONY: all dist test clean

all: dist test

dist:
	$(MAKE) -C src clean
	$(MAKE) -C src dist

test:
	$(MAKE) -C test

clean:
	$(MAKE) -C src clean
	$(MAKE) -C test clean
	rm -rf *-*-*/

archinfo:
	$(MAKE) -C src archinfo
