
@echo off
REM //set you csdtk path
call D:\CSDTK41\CSDTKvars.bat

set ss=%time:~6,2%
set mm=%time:~3,2%
set hh=%time:~0,2%
set /a startTime=(%hh%*60+%mm%)*60+%ss%




set PATH=platform\compilation\win32;%PATH%
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
    REM goto end_exit
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
            REM goto end_exit
        ) else (
            echo param %1% is not illege 
            goto usage_help
        )
    )
) 


:compile
    set LOG_FILE=build\%PROJ_NAME%_build.log
    make -r -j4 CT_RELEASE=%compileMode%  2>&1 | tee %LOG_FILE%
    REM make -r -j4 CT_RELEASE=%compileMode%  2>&1 
    REM copy hex\%PROJ_NAME%\%PROJ_NAME%_flash.lod hex\%PROJ_NAME%\%PROJ_NAME%_flash_%compileMode%.lod
    REM del hex\%PROJ_NAME%\%PROJ_NAME%_flash.lod

    set MAP_FILE_PATH=build\%PROJ_NAME%\%PROJ_NAME%.map
    set MEMD_DEF_PATH=platform\csdk\memd.def
    FOR /F %%i IN ('grep  -n  "USER_RAM_SIZE" %MEMD_DEF_PATH% ^| gawk  '{print $3}'') DO @set /a ram_total=%%i
    FOR /F %%i IN ('grep  -n  "USER_ROM_SIZE" %MEMD_DEF_PATH% ^| gawk  '{print $3}'') DO @set /a rom_total=%%i
    FOR /F %%i IN ('grep  -n  "__user_rw_size = (__user_rw_end - __user_rw_start)" %MAP_FILE_PATH% ^| gawk  '{print $2}'') DO @set /a use_ram_size=%%i
    FOR /F %%i IN ('grep  -n  "__rom_size = (__user_rw_lma - __rom_start)" %MAP_FILE_PATH% ^| gawk  '{print $2}'') DO @set /a use_rom_size=%%i
    FOR /F %%i IN ('grep  -n  "__user_bss_size = (__user_bss_end - __user_bss_start)" %MAP_FILE_PATH% ^| gawk  '{print $2}'') DO @set /a use_rom_bss_size=%%i
    REM echo %ram_total% %rom_total% %use_ram_size% %use_rom_size% %use_rom_bss_size%
    set /a ram_use=%use_ram_size%+%use_rom_bss_size%
    set /a rom_use=%use_rom_size%+%use_rom_bss_size%   
    REM set /a ram_percent=%ram_use%*10000/%ram_total%
    REM set /a rom_percent=rom_use*10000/%rom_total%
    echo ROM    total:%rom_total% Bytes     use:%rom_use% Bytes
    echo RAM    total:%ram_total% Bytes     use:%ram_use% Bytes
    goto end_exit

:clean_project
    if "%2%"x =="all"x (
        echo delte %SOFT_WORKDIR%/hex
        rd /s/q %BUILD_PATH%\hex
        rd /s/q %BUILD_PATH%\build
    ) else (
        echo delte %SOFT_WORKDIR%/hex/%2%
        rd /s/q %BUILD_PATH%\hex\%2%
        rd /s/q %BUILD_PATH%\build\%2%
    )                      
    goto end_exit

:usage_help
    echo usage:
    echo use 'build.bat PROJECTNAME'            to build the project in ./PROJECTNAME             
    echo               eg: build.bat app                                              
    echo use 'build.bat demo PROJECTNAME'       to build demo in ./demo/PROJECTNAME          
    echo use 'build.bat clean PROJECTNAME'      to clean the project PROJECTNAME build files
    echo use 'build.bat clean all'              to clean all the project build files                
    echo use 'build.bat ... release'            to build release software                         
    echo               eg: 'build.bat demo gpio release'                              
    goto end_exit


:end_exit
    set ss=%time:~6,2%
    set mm=%time:~3,2%
    set hh=%time:~0,2%
    set /a endTime=(%hh%*60+%mm%)*60+%ss%
    set /a total=%endTime%-%startTime%
    echo === Build Time: %total%s at %date% %time% ===
    pause