.PHONY: all dist tests clean

all: dist tests

dist:
	$(MAKE) -C src clean
	$(MAKE) -C src dist

tests:
	$(MAKE) -C tests

clean:
	$(MAKE) -C src clean
	$(MAKE) -C tests clean
	rm -rf *-*-*

archinfo:
	$(MAKE) -C src archinfo
