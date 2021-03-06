#!/bin/bash
if [ -n "$1" ]; then
        BUILDTHREADS=$1
else
        BUILDTHREADS=2
fi

SCRIPTDIR="$( cd "$(dirname $0)" && pwd )"
cd $SCRIPTDIR
rm -Rf autom4te.cache
./bootstrap || exit 1
./configure --prefix=/usr --localstatedir=/var --sysconfdir=/etc --libdir=/usr/lib || exit 1
make -j${BUILDTHREADS} CPPFLAGS="-DDEBUG" CXXFLAGS="-g -O0 -fsanitize=address -fno-omit-frame-pointer" && make install
