#!/bin/bash


TOOL_GENLIB_PATH=`pwd`

function cancel()
{
    cd ${TOOL_GENLIB_PATH} # genlib
    cd ../../../ # GPRS_C_SDK
    if [[ -f "Makefile.bak" ]]; then
        mv -f Makefile.bak Makefile
    fi
    if [[ -f "libMakefile" ]]; then
        rm -f libMakefile
    fi
}

trap cancel SIGINT SIGTERM SIGQUIT

###############################
cp ./Makefile ../../../libMakefile
cd ../../../ && mv Makefile Makefile.bak
mv libMakefile Makefile

###############################
compileMode=debug

if [[ "$1xx" == "releasexx" ]]; then
    compileMode=release
elif [[ "$1xx" == "cleanxx" ]]; then
    echo "==Clean CSDK lib...=="
    rm -rf build
    echo "==Clean CSDK lib OK=="
    cancel
    exit 0
fi
echo "==Now generate lib=="
export SOFT_WORKDIR=`pwd -P`

echo "==Clean Complete=="
MAKE_J_NUMBER=`cat /proc/cpuinfo | grep vendor_id | wc -l`
echo "==core number:$MAKE_J_NUMBER=="
make -j${MAKE_J_NUMBER} CT_RELEASE=$compileMode 2>&1
mkdir -p hex/libcsdk
if [[ -f "build/csdk/lib/libcsdk_${compileMode}.a" ]]; then
    cp -f build/csdk/lib/libcsdk_${compileMode}.a hex/libcsdk/libcsdk_${compileMode}.a
    echo "Generate lib OK"
else
    echo "Generate lib FAIL"
fi




###############################
mv -f Makefile.bak Makefile
