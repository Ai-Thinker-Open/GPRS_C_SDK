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
    echo -ne "\033[1;31m" #red
    echo "!!!!!!!!!!!!!!"
    echo "NO LIB FILES"
    echo "!!!!!!!!!!!!!!"
    echo "please check  platform/csdk folder, it can not be empty"
    echo ""
    echo "Plese download again from "
    echo ""
    echo "      https://github.com/Ai-Thinker-Open/GPRS_C_SDK/releases"
    echo ""
    echo -ne "\033[0m" #normal
    exit 1
fi
#####################################

start_time=`date +%s`

export SOFT_WORKDIR=`pwd -P`

echo ""
echo "===================Start build====================="
echo "-- Param   number : $paramNum"
echo "-- Compile mode   : $compileMode"

if [[ $paramNum -eq 1  ]]; then
    if [[ "$1xx" == "cleanxx" ]]; then
        echo "Clean all..."
        rm -rf $SOFT_WORKDIR/build
        rm -rf $SOFT_WORKDIR/hex
        echo "====================Clean End======================"
        exit 0
    fi
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
        sed -i "15i\PROJECT_PATH += $1" Makefile
    fi
    export PROJ_NAME=$1
elif [[ $paramNum -eq 2  ]]; then
    if [[ "$1x" == "cleanx" ]]; then
        if [[ "$2x" == "allx" ]]; then
            rm -rf $SOFT_WORKDIR/build
            rm -rf $SOFT_WORKDIR/hex
        elif [[ "$2x" == "aliyunx" ]]; then
            make -C libs/aliyun/iotkit-embedded distclean
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
        sed -i "15i\PROJECT_PATH += demo/$2" Makefile
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
        sed -i "15i\PROJECT_PATH += project/$2" Makefile
    fi
elif [[ $paramNum -eq 4  ]]; then
    if [[ "$1x" == "fotax" ]]; then
        if [ -f "$2" ]; then
            if [ -f "$3" ]; then
                echo "[OTA] waiting for making fota pack..."
                echo "      this will take a few minutes..."
                mkdir -p hex/tmp
                old_ota_path=hex/tmp/old_ota_lod.lod
                new_ota_path=hex/tmp/new_ota_lod.lod
                python platform/compilation/lodtool.py gen_ota --lod $2 --out $old_ota_path
                python platform/compilation/lodtool.py gen_ota --lod $3 --out $new_ota_path
                platform/compilation/fota/linux/fotacreate 4194304 65536  $old_ota_path  $new_ota_path $4
                rm -rf hex/tmp
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


#build path and log
LOG_FILE_PATH=$SOFT_WORKDIR/build
if [ ! -d ${LOG_FILE_PATH} ]; then
	mkdir ${LOG_FILE_PATH}
fi
LOG_FILE=${LOG_FILE_PATH}/${PROJ_NAME}_build.log

MAKE_J_NUMBER=`cat /proc/cpuinfo | grep vendor_id | wc -l`
echo "-- Core    number : $MAKE_J_NUMBER"
echo "-- Compile project: $PROJ_NAME";
echo "-- Project path   : "
echo "  |"
echo "   --$IS_PROJECT_DIR";
echo "---------------------------------------------------"

cd $SOFT_WORKDIR
if [ ${MAKE_J_NUMBER} -gt 1 ]; then
    make -j${MAKE_J_NUMBER} CT_RELEASE=$compileMode 2>&1 | tee ${LOG_FILE}
else
    make CT_RELEASE=$compileMode 2>&1 | tee ${LOG_FILE}
fi

end_time=`date +%s`
time_distance=`expr ${end_time} - ${start_time}`
date_time_now=$(date +%F\ \ %H:%M:%S)
echo -ne "\033[1;32m" #green
echo ====== Build Time: ${time_distance}s  complete at  ${date_time_now} ======= | tee -a ${LOG_FILE}
echo -ne "\033[0m"

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

result=`cat ${LOG_FILE}|grep "Error"`

if [[ "${result}aa" != "aa" || "${map_file}aa" = "aa" ]]; then
    echo -ne "\033[1;31m"
    echo "!!!!!!!!!!!!!!!!!!!!"
    echo "   BUILD FAILED"
    echo "!!!!!!!!!!!!!!!!!!!!"
    echo -ne "\033[0m"
    echo "============================================================="
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
ram_used_percent=$(awk 'BEGIN{printf "%.2f%%\n",('$ram_used'/'$((${ram_total}))')*100}')

rw_data_size=$(($ram_rw_data_end-$ram_start))
rom_used=$(($rom_rw_start-$rom_start+$rw_data_size))
rom_used_percent=$(awk 'BEGIN{printf "%.2f%%\n",('$rom_used'/'$(($rom_total))')*100}')


echo ROM total: ${rom_total}\($((${rom_total}))\) Bytes, used: $rom_used Bytes \($rom_used_percent\)
echo RAM total: ${ram_total}\($((${ram_total}))\) Bytes, used: $ram_used Bytes \($ram_used_percent\)
echo -ne "\033[1;32m" #green
echo "============================================================="
echo -ne "\033[0m" #normal

exit
