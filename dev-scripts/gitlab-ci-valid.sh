#!/bin/bash

if [ ! -f dev-scripts/gitlab-ci-valid.sh ]; then
	echo "Must be executed from project root directory" 1>&2
	exit 1
fi

set -e
set -x

rm -rf build
mkdir -p build
cd build
if [ -f /usr/lib/libgtes* ]; then
	../configure --enable-debug
else
	../configure --enable-debug --disable-tests
fi
make
make test
