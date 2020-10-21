#!/bin/sh

#This script is designed to be used with cmake builds.

export PREENY_DEBUG=1
export PREENY_INFO=1
export PREENY_ERROR=1

# Sleep
LD_PRELOAD=lib/libdesleep.so ./bin/test_sleep

# UID
./bin/test_uid
LD_PRELOAD=lib/libdeuid.so FAKE_UID=1337 FAKE_EUID=1338 ./bin/test_uid

# Rand
LD_PRELOAD=lib/libderand.so RAND=1337 ./bin/test_rand

# Realloc
LD_PRELOAD=lib/libcrazyrealloc.so RAND=1337 ./bin/test_realloc

# Sock
echo "TEST" | LD_PRELOAD=lib/libdesock.so ./bin/test_sock

# Canary
LD_PRELOAD=lib/libgetcanary.so ./bin/test_hello

# SetSTDIN
LD_PRELOAD=lib/libsetstdin.so ./bin/test_setstdin_fread
LD_PRELOAD=lib/libsetstdin.so ./bin/test_setstdin_getc
LD_PRELOAD=lib/libsetstdin.so ./bin/test_setstdin_read
