#!/bin/bash
#
# Author: vencol,neucrack
# 


####################################
# help info

function help()
{
    echo "Usage:"
    echo "use ./build.bat fota old.lod new.lod fota.pack"
    echo "use './build.sh PROJECTNAME' to build the project in ./PROJECTNAME             "
    echo "               eg: ./build.sh app                                              "
    echo "use './build.sh demo PROJECTNAME' to build demo in ./demo/PROJECTNAME          "
    echo "use './build.sh clean PROJECTNAME' to clean the project PROJECTNAME build files"
    echo "use './build.sh clean all' to clean all the project build files                "
    echo "use './build.sh ... release' to build release software                         "
    echo "               eg: './build.sh demo gpio release'                              "
}

if [[ "$1x" == "helpx" || "$1x" == "-hx" || "$1x" == "-helpx" || "$1x" == "--helpx" ]]; then
    help
    exit 0
fi

######################################


compileMode='debug'
paramNum=$#

if [[ "${!#}x" == "releasex" ]]; then
    compileMode='release'
    paramNum=$(($paramNum-1))
fi

####################################
# check lib files

CSDK_MEM_DEF_PATH=./platform/csdk
CSDK_LIB_PATH=./platform/csdk/$compileMode

memdef_file=$(ls $CSDK_MEM_DEF_PATH|grep -w 'memd.def')
elf_file=$(ls $CSDK_LIB_PATH|grep '.elf')
lib_file=$(ls $CSDK_LIB_PATH|grep '.lod')

if [[ "${memdef_file}aa" = "aa" || "${elf_file}aa" = "aa" || "${lib_file}aa" = "aa" ]]; then
    echo "!!!!!!!!!!!!!!"
    echo "NO LIB FILES"
    echo "!!!!!!!!!!!!!!"
    echo "please check  platform/csdk folder, it can not be empty"
    echo ""
    echo "Plese download again from "
    echo ""
    echo "      https://github.com/Ai-Thinker-Open/GPRS_C_SDK/releases"
    echo ""
    exit 1
fi
#####################################

start_time=`date +%s`
#where the cygwin install in unix path,example if windows path is G:\CSDTK\cygwin,cygwin path may be /cygdrive/g/CSDTK/cygwin
# CYGWIN_HOME= 
# if [[ ! -d $CYGWIN_HOME ]]; then
#     echo  PATH $CYGWIN_HOME is not exist
#     exit
# fi

#set the path
# export PATH=$CYGWIN_HOME/bin:$CYGWIN_HOME/crosscompiler/bin:$CYGWIN_HOME/cooltools:/bin:/usr/bin;
export PATH=/bin:/crosscompiler/bin:/cooltools:/bin:/usr/bin:$PATH;
# echo path:$PATH

export SOFT_WORKDIR=`pwd`

echo "param number:$paramNum"
echo "compileMode:$compileMode"

if [[ $paramNum -eq 1  ]]; then
    export IS_PROJECT_DIR=$SOFT_WORKDIR/$1
    if [[ ! -d $IS_PROJECT_DIR ]]; then
        echo "project $1 error path:$IS_PROJECT_DIR";
        exit
    fi
    
    if [[ "$1x" == "initx" ]]; then
        sed -i '15d' Makefile
        sed -i "15i\#" Makefile
    else
        sed -i '15d' Makefile
        sed -i "15i\LOCAL_MODULE_DEPENDS += $1" Makefile
    fi
    export PROJ_NAME=$1
elif [[ $paramNum -eq 2  ]]; then
    if [[ "$1x" == "cleanx" ]]; then
        if [[ "$2x" == "allx" ]]; then
            rm -rf $SOFT_WORKDIR/build
            rm -rf $SOFT_WORKDIR/hex
        else
            rm -rf $SOFT_WORKDIR/build/$2_$compileMode
            rm -rf $SOFT_WORKDIR/hex/$2_$compileMode
            rm -rf $SOFT_WORKDIR/build/$2
            rm -rf $SOFT_WORKDIR/hex/$2
            rm -f $SOFT_WORKDIR/build/$2_build.log
        fi
        echo "clear project $2 end";
        exit
    elif [[ "$1x" == "demox" ]]; then
        export IS_PROJECT_DIR=$SOFT_WORKDIR/demo/$2
        if [[ ! -d $IS_PROJECT_DIR ]]; then
            echo "demo $2 error path $IS_PROJECT_DIR";
            exit
        fi
        export PROJ_NAME=$2

        # sed -i '5d' Makefile
        # sed -i "5i\LOCAL_LIBS += platform/lib/libinit.a" Makefile
        sed -i '15d' Makefile
        sed -i "15i\LOCAL_MODULE_DEPENDS += demo/$2" Makefile
    elif [[ "$1x" == "projectx" ]]; then
        export IS_PROJECT_DIR=$SOFT_WORKDIR/project/$2
        if [[ ! -d $IS_PROJECT_DIR ]]; then
            echo "demo $2 error path $IS_PROJECT_DIR";
            exit
        fi
        export PROJ_NAME=$2
        
        # sed -i '5d' Makefile
        # sed -i "5i\LOCAL_LIBS += platform/lib/libinit.a" Makefile
        sed -i '15d' Makefile
        sed -i "15i\LOCAL_MODULE_DEPENDS += project/$2" Makefile
    fi
elif [[ $paramNum -eq 4  ]]; then
    if [[ "$1x" == "fotax" ]]; then
        if [ -f "$2" ]; then
            if [ -f "$3" ]; then
                echo "waiting for making fota pack..."
                echo "this will take a few minutes"
                platform/compilation/fota/fotacreate.exe 4194304 65536 $2 $3 $4
                exit 0
            fi
        fi
    fi
    help
    exit 0
else
    help
    exit 0
fi

# if [[ ! -d target/$PROJ_NAME ]]; then
#     cp -rf target/init target/$PROJ_NAME
#     echo "user default for init-target";
# fi

#build path and log
LOG_FILE_PATH=$SOFT_WORKDIR/build
if [ ! -d ${LOG_FILE_PATH} ]; then
	mkdir ${LOG_FILE_PATH}
fi
LOG_FILE=${LOG_FILE_PATH}/${PROJ_NAME}_build.log

echo "compile project $PROJ_NAME";
echo "compile path $IS_PROJECT_DIR";

MAKE_J_NUMBER=`cat /proc/cpuinfo | grep vendor_id | wc -l`
echo "core number:$MAKE_J_NUMBER"
# rm -rf $SOFT_WORKDIR/hex/$PROJ_NAME

cd $SOFT_WORKDIR
if [ ${MAKE_J_NUMBER} -gt 1 ]; then
    make -j${MAKE_J_NUMBER} CT_RELEASE=$compileMode 2>&1 | tee ${LOG_FILE}
else
    make CT_RELEASE=$compileMode 2>&1 | tee ${LOG_FILE}
fi

# rm -f $SOFT_WORKDIR/hex/${PROJ_NAME}_${compileMode}/*
# rm -rf $SOFT_WORKDIR/hex/${PROJ_NAME}_${compileMode}
# mkdir $SOFT_WORKDIR/hex/${PROJ_NAME}_${compileMode}
# cp -f $SOFT_WORKDIR/hex/$PROJ_NAME/* $SOFT_WORKDIR/hex/${PROJ_NAME}_${compileMode}
# rm -rf $SOFT_WORKDIR/hex/$PROJ_NAME
# if [[ "$1x" == "initx" ]]; then
#     cp build/init/init/lib/libinit_*.a platform/lib/libinit.a
# fi

end_time=`date +%s`
time_distance=`expr ${end_time} - ${start_time}`
date_time_now=$(date +%F\ \ %H:%M:%S)
echo === Build Time: ${time_distance}s  at  ${date_time_now} === | tee -a ${LOG_FILE}


# print RAM and ROM info

if [[ $paramNum -eq 1  ]]; then
    if [[ "$1aa" != "cleanaa" ]]; then
        mapPathName=$1
    else
        exit 0
    fi
else
    mapPathName=$2
fi

MAP_FILE_PATH=./build/$mapPathName/$mapPathName.map
MEMD_DEF_PATH=./platform/csdk/memd.def

map_file=$(ls ./build/$mapPathName|grep '.map')


if [[ "${map_file}aa" = "aa" ]]; then
    echo "!!!!!!!!!!!!!!!!!!!!"
    echo "   BUILD FAILED"
    echo "!!!!!!!!!!!!!!!!!!!!"
    exit 1
fi

ram_total=$(grep  -n  "USER_RAM_SIZE" $MEMD_DEF_PATH | awk  '{print $3}')
rom_total=$(grep  -n  "USER_ROM_SIZE" $MEMD_DEF_PATH | awk  '{print $3}')

rom_start=$(grep  -n  "__rom_start = ." $MAP_FILE_PATH | awk  '{print $2}')
rom_rw_start=$(grep  -n  "__user_rw_lma = ." $MAP_FILE_PATH | awk  '{print $2}')

ram_start=$(grep  -n  "__user_rw_start = ." $MAP_FILE_PATH | awk  '{print $2}')
ram_rw_data_end=$(grep  -n  "__user_rw_end = ." $MAP_FILE_PATH | awk  '{print $2}')
ram_end=$(grep  -n  "__user_bss_end = ." $MAP_FILE_PATH | awk  '{print $2}')

# echo $ram_start $ram_end
ram_used=$(($ram_end-$ram_start))
ram_used_percent=$(awk 'BEGIN{printf "%.2f%\n",('$ram_used'/'$ram_total')*100}')

rw_data_size=$(($ram_rw_data_end-$ram_start))
rom_used=$(($rom_rw_start-$rom_start+$rw_data_size))
rom_used_percent=$(awk 'BEGIN{printf "%.2f%\n",('$rom_used'/'$rom_total')*100}')


echo ROM total: ${rom_total}\($((${rom_total}))\) Bytes, used: $rom_used Bytes \($rom_used_percent\)
echo RAM total: ${ram_total}\($((${ram_total}))\) Bytes, used: $ram_used Bytes \($ram_used_percent\)

exit