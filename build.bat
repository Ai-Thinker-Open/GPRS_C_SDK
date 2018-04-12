
@echo off

call G:\CSDTK41\CSDTKvars.bat
set ss=%time:~6,2%
set mm=%time:~3,2%
set hh=%time:~0,2%
set /a startTime=(%hh%*60+%mm%)*60+%ss%




set PATH=platform\compilation\win32;%PATH%
set MEMD_DEF_PATH=platform\csdk\memd.def
set SOFT_WORKDIR=%cd:\=/%
set BUILD_PATH=%cd%
set compileMode=debug


if "%1%"x =="demo"x (
    set PROJ_NAME=%2%
    if "%3%"x =="release"x (
        set compileMode=release
    )
    sed -i "15d" Makefile
    sed -i "15i\LOCAL_MODULE_DEPENDS += demo/%2%" Makefile
    goto compile
)else (
    if "%1%"x =="clean"x (
        goto clean_project
    ) else (
        if exist "%1%" (
            set PROJ_NAME=%1%
            if "%2%"x =="release"x (
                set compileMode=release
            )
            sed -i "15d" Makefile
            sed -i "15i\LOCAL_MODULE_DEPENDS += %1%" Makefile
            goto compile                     
            REM goto end
        ) else (
            echo param %1% is not illege 
            goto usage_help
        )
    )
) 


:compile
    make -r -j4 CT_RELEASE=%compileMode%                           
    goto end

:clean_project
    if "%2%"x =="all"x (
        echo delte %SOFT_WORKDIR%/hex
        rd /s/q %BUILD_PATH%\hex
    ) else (
        echo delte %SOFT_WORKDIR%/hex/%2%
        rd /s/q %BUILD_PATH%\hex\%2%
    )                      
    goto end

:usage_help
    echo usage:
    echo use 'build.bat PROJECTNAME'            to build the project in ./PROJECTNAME             
    echo               eg: build.bat app                                              
    echo use 'build.bat demo PROJECTNAME'       to build demo in ./demo/PROJECTNAME          
    echo use 'build.bat clean PROJECTNAME'      to clean the project PROJECTNAME build files
    echo use 'build.bat clean all'              to clean all the project build files                
    echo use 'build.bat ... release'            to build release software                         
    echo               eg: 'build.bat demo gpio release'                              
    goto end

:end
    set MAP_FILE_PATH=build\%PROJ_NAME%\%PROJ_NAME%.map
    REM set ram_total=grep  -n  "USER_RAM_SIZE" %MEMD_DEF_PATH% | (gawk  '{print $3}'
    REM set rom_total=$(grep  -n  "USER_ROM_SIZE" $MEMD_DEF_PATH | gawk  '{print $3}')
    REM set rom_start=$(grep  -n  "__rom_start = ." $MAP_FILE_PATH | gawk  '{print $2}')
    REM set rom_rw_start=$(grep  -n  "__user_rw_lma = ." $MAP_FILE_PATH | gawk  '{print $2}')
    REM set ram_start=$(grep  -n  "__user_rw_start = ." $MAP_FILE_PATH | gawk  '{print $2}')
    REM set ram_rw_data_end=$(grep  -n  "__user_rw_end = ." $MAP_FILE_PATH | gawk  '{print $2}')
    REM set ram_end=$(grep  -n  "__user_bss_end = ." $MAP_FILE_PATH | gawk  '{print $2}')
    REM REM # echo $ram_start $ram_end
    REM set ram_used=$(($ram_end-$ram_start))
    REM set ram_used_percent=$(gawk 'BEGIN{printf "%.2f%\n",('$ram_used'/'$ram_total')*100}')
    REM set rw_data_size=$(($ram_rw_data_end-$ram_start))
    REM set rom_used=$(($rom_rw_start-$rom_start+$rw_data_size))
    REM set rom_used_percent=$(gawk 'BEGIN{printf "%.2f%\n",('$rom_used'/'$rom_total')*100}')
    REM echo ROM total: ${rom_total}\($((${rom_total}))\) Bytes, used: $rom_used Bytes \($rom_used_percent\)
    REM echo RAM total: ${ram_total}\($((${ram_total}))\) Bytes, used: $ram_used Bytes \($ram_used_percent\)

    FOR /F %%i IN ('grep  -n  "USER_RAM_SIZE" %MEMD_DEF_PATH% ^| gawk  '{print $3}'') DO @set ram_total=%%i
    FOR /F %%i IN ('grep  -n  "USER_ROM_SIZE" %MEMD_DEF_PATH% ^| gawk  '{print $3}'') DO @set rom_total=%%i
    FOR /F %%i IN ('grep  -n  "__rom_start = ." %MAP_FILE_PATH% ^| gawk  '{print $2}'') DO @set rom_start=%%i
    FOR /F %%i IN ('grep  -n  "__user_rw_lma = ." %MAP_FILE_PATH% ^| gawk  '{print $2}'') DO @set rom_rw_start=%%i
    FOR /F %%i IN ('grep  -n  "__user_rw_start = ." %MAP_FILE_PATH% ^| gawk  '{print $2}'') DO @set ram_start=%%i
    FOR /F %%i IN ('grep  -n  "__user_rw_end = ." %MAP_FILE_PATH% ^| gawk  '{print $2}'') DO @set ram_rw_data_end=%%i
    FOR /F %%i IN ('grep  -n  "__user_bss_end = ." %MAP_FILE_PATH% ^| gawk  '{print $2}'') DO @set ram_end=%%i

    echo %ram_total% %rom_total% %rom_start% %rom_rw_start% %ram_start% %ram_rw_data_end% %ram_end%
    REM REM REM # echo $ram_start $ram_end
    REM REM set ram_end=1233
    REM REM set ram_start=123
    REM set /a ram_used=%ram_end:~-8%-%ram_start:~-8%
    REM REM FOR /F %%i IN ('gawk 'BEGIN{printf "%.2f%\n",('%ram_used%'/'%ram_total%')*100}'') DO @set ram_used_percent=%%i
    REM set /a rw_data_size=%ram_rw_data_end%-%ram_start%
    REM set /a rom_used=%rom_rw_start%-%rom_start%+%rw_data_size%
    REM REM FOR /F %%i IN ('grep  -n  "USER_RAM_SIZE" %MEMD_DEF_PATH% ^| gawk  '{print $3}'') DO @set ram_total=%%i
    REM REM FOR /F %%i IN ('grep  -n  "USER_RAM_SIZE" %MEMD_DEF_PATH% ^| gawk  '{print $3}'') DO @set ram_total=%%i

    REM echo ram_used %ram_used% %ram_used_percent% %rw_data_size% %rom_used%
    set ss=%time:~6,2%
    set mm=%time:~3,2%
    set hh=%time:~0,2%
    set /a endTime=(%hh%*60+%mm%)*60+%ss%
    set /a total=%endTime%-%startTime%
    echo === Build Time: %total%s at %date% %time% ===
    pause