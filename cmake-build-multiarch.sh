#!/bin/sh
mkdir -p build_x64 build_x86
CFLAGS=-m32 CXXFLAGS=-m32 cmake . -Bbuild_x86
(cd build_x86 && make)
cmake . -Bbuild_x64
(cd build_x64 && make)
