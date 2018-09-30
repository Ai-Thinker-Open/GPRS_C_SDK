
@echo off
REM //set you csdtk path
set USER_CSDTK=%GPRS_CSDTK42_PATH%
if %USER_CSDTK%a==a (echo NO CSDTK,please install CSDTK firstly  && pause && exit)

set startTime=%time%


if not defined CSDTK4INSTALLDIR (
    set ptemp=platform\compilation\win32;
    echo CSDTK PATH: %USER_CSDTK%
    call %USER_CSDTK%\CSDTKvars.bat
    REM echo first time set csdtk auto
) else (
    set ptemp=
)
set PATH=%ptemp%%PATH%
set SOFT_WORKDIR=%cd:\=/%
set BUILD_PATH=%cd%
set compileMode=debug


if "%1%"x =="demo"x (
    set PROJ_NAME=%2%
    if "%3%"x =="release"x (
        set compileMode=release
    )
    sed -i "15d" Makefile
    sed -i "15i\PROJECT_PATH += demo/%2%" Makefile
    goto compile
    REM goto end_exit
)else (
    if "%1%"x =="clean"x (
        goto clean_project
    ) else (
        if "%1%"x =="fota"x (
            goto run_fota
        ) else (
            if exist "%1%" (
                set PROJ_NAME=%1%
                if "%2%"x =="release"x (
                    set compileMode=release
                )
                sed -i "15d" Makefile
                sed -i "15i\PROJECT_PATH += %1%" Makefile
                goto compile                     
                REM goto end_exit
            ) else (
                echo param %1% is not illege 
                goto usage_help
            )
        )
    )
) 


:compile
    set LOG_FILE=%BUILD_PATH%\build\%PROJ_NAME%_build.log
    if exist "%BUILD_PATH%\build" (
        echo build folder exist
    ) else (
        md %BUILD_PATH%\build
    )
    echo number of processors: %number_of_processors%
    make -r -j%number_of_processors% CT_RELEASE=%compileMode%  2>&1 | tee %LOG_FILE%
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
    echo -------------------------------------------------
    echo ROM    total:%rom_total% Bytes     used:%rom_use% Bytes
    echo RAM    total:%ram_total% Bytes     used:%ram_use% Bytes
    goto end_exit

:clean_project
    if exist %BUILD_PATH%\hex (
        if "%2%"x =="all"x (
            echo delte %SOFT_WORKDIR%/hex
            rd /s/q %BUILD_PATH%\hex
            rd /s/q %BUILD_PATH%\build
        ) else (
            echo delte %SOFT_WORKDIR%/hex/%2%
            rd /s/q %BUILD_PATH%\hex\%2%
            rd /s/q %BUILD_PATH%\build\%2%
        )
    ) else (
        echo already clean
    )
    echo clean complete
    goto end_exit

:run_fota         
    if exist "%2%" (
        if exist "%3%" (
            echo [OTA] waiting for making fota pack...
            echo       this will take a few minutes...
            if not exist "hex\tmp" md hex\tmp
            python platform\compilation\lodtool.py gen_ota --lod %2% --out hex\tmp\old_ota_lod.lod
            python platform\compilation\lodtool.py gen_ota --lod %3% --out hex\tmp\new_ota_lod.lod
            platform\compilation\fota\fotacreate.exe 4194304 65536 hex\tmp\old_ota_lod.lod hex\tmp\new_ota_lod.lod %4%
            rd /q /s hex\tmp
        ) else (
            echo usage: 'build.bat fota old.lod new.lod fota.pack'
        )
    ) else (
        echo usage: 'build.bat fota old.lod new.lod fota.pack'
    )
    goto end_exit

:usage_help
    echo usage:
    echo use 'build.bat fota old.lod new.lod fota.pack'
    echo use 'build.bat PROJECTNAME'            to build the project in ./PROJECTNAME             
    echo               eg: build.bat app                                              
    echo use 'build.bat demo PROJECTNAME'       to build demo in ./demo/PROJECTNAME          
    echo use 'build.bat clean PROJECTNAME'      to clean the project PROJECTNAME build files
    echo use 'build.bat clean all'              to clean all the project build files                
    echo use 'build.bat ... release'            to build release software                         
    echo               eg: 'build.bat demo gpio release'                              
    goto end_exit


:end_exit
    set endTime=%time%
    if "a%startTime:~-11,1%"=="a " (
    set startTime=0%startTime:~-10%
    )
    if "a%endTime:~-11,1%"=="a " (
    set endTime=0%endTime:~-10%
    )
    set startTime=%startTime:~-11%
    set endTime=%endTime:~-11%

    set /a msec1=1%startTime:~-2,2%-100
    set /a second1=1%startTime:~-5,2%-100
    set /a minute1=1%startTime:~-8,2%-100
    set /a hour1=1%startTime:~-11,2%-100

    set /a msec2=1%endTime:~-2,2%-100
    set /a second2=1%endTime:~-5,2%-100
    set /a minute2=1%endTime:~-8,2%-100
    set /a hour2=1%endTime:~-11,2%-100

    set /a time1MS=%msec1%+%second1%*1000+%minute1%*1000*60+%hour1%*1000*60*60
    set /a time2MS=%msec2%+%second2%*1000+%minute2%*1000*60+%hour2%*1000*60*60
    set /a timeIntervalMS=%time2MS%-%time1MS%

    set /a intervalMS=1%timeIntervalMS:~-3,3%-1000
    set /a intervalS =%timeIntervalMS%/1000

    echo =================================================
    echo Start Time : %startTime%
    echo End   Time : %endTime%
    echo Build Time : %intervalS%.%intervalMS%s
    echo =================================================
