#!/bin/bash
######################################################
#            PROJECT  : MPC_Allocator_CPP            #
#            VERSION  : 0.0.0                        #
#            DATE     : 07/2013                      #
#            AUTHOR   : Valat Sébastien              #
#            LICENSE  : CeCILL-C                     #
######################################################

#Test action to run in gitlab-ci (just due to a bug which do not permit 
#to create build directory in it's own job definition).

#Check execution path
if [ ! -f dev-scripts/gitlab-ci-valid.sh ]; then
	echo "Must be executed from project root directory" 1>&2
	exit 1
fi

#Made it verbose
set -e
set -x

#run actions
rm -rf build
mkdir -p build
cd build
../configure --enable-debug
make
make test
