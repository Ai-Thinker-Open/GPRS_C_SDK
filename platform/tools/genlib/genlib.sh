#!/bin/bash


###############################
cp ./Makefile ../../../libMakefile
cd ../../../ && mv Makefile Makefile.bak
mv libMakefile Makefile

###############################
echo "==Now generate lib=="
compileMode=debug

if [[ "$1xx" == "releasexx" ]]; then
    compileMode=release
fi
export SOFT_WORKDIR=`pwd -P`

echo "==Clean...=="
./build.sh clean
echo "==Clean Complete=="
MAKE_J_NUMBER=`cat /proc/cpuinfo | grep vendor_id | wc -l`
echo "==core number:$MAKE_J_NUMBER=="
make -j${MAKE_J_NUMBER} CT_RELEASE=$compileMode 2>&1
echo "Generate lib OK"

###############################
mv -f Makefile.bak Makefile
