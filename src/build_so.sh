#!/bin/bash

[ -z "$CC" -a -z "$MACHINE" ] && CC=gcc
[ -z "$MACHINE" ] && MACHINE=$($CC -dumpmachine)
[ -z "$CC" ] && CC=$MACHINE-gcc

if [[ $MACHINE == *x86_64* ]]
then
	OPTS="$OPTS -DPLT_ENTRIES_64Baa"
fi

OPTS="$OPTS -fPIC"
if [[ $MACHINE == *linux* ]]
then
	OPTS="$OPTS -DLINUX -ldl"
elif [[ $MACHINE == *freebsd* ]]
then
	OPTS="$OPTS -DFREEBSD"
elif [[ $MACHINE == *-elf* ]]
then
	OPTS="$OPTS -mabicalls"
fi

OUT=$1
IN=$2
shift 2

$CC $IN -o $OUT -shared $OPTS "$@" &&
mkdir -p ../$MACHINE &&
mv $OUT ../$MACHINE/
