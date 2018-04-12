call G:\CSDTK41\CSDTKvars.bat
set SOFT_WORKDIR=%cd:\=/%
set PATH=platform\compilation\win32;%PATH%
set PROJ_NAME=uart
REM set SOFT_WORKDIR=/e/rdaprojects/work8955/GPRS_C_SDK

make -r -j4 CT_TARGET=%CT_TARGET% CT_RELEASE=debug