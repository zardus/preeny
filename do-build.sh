#/bin/sh
mkdir -p build
cmake . -Bbuild
(cd build && make)
