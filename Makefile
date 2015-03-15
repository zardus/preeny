all:
	make -C src
	make -C tests

clean:
	make -C src clean
	make -C tests clean
	rm -rf `uname -s`_`uname -m`
