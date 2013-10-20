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
../configure --enable-debug
make
make test
