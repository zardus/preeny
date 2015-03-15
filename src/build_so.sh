#!/bin/bash

if [ $(uname -m) == "x86_64" -o $(uname -m) == "amd64" ]
then
	OPTS="$OPTS -DPLT_ENTRIES_64Baa"
fi

if [ $(uname -s) == "Linux" ]
then
	OPTS="$OPTS -DLINUX -ldl -fPIC"
elif [ $(uname -s) == "FreeBSD" ]
then
	OPTS="$OPTS -DFREEBSD -fPIC"
fi

OUT=$1
IN=$2
shift 2

gcc $IN -o $OUT -shared $OPTS "$@" &&
mkdir -p ../$(uname -s)_$(uname -m) &&
mv $OUT ../$(uname -s)_$(uname -m)/
