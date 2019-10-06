



# Name of the this module
LOCAL_NAME := libs


# Space-separated list of modules (libraries) your module depends upon.
# These should include the toplevel name, e.g. "libs/gps"
LOCAL_MODULE_DEPENDS := libs/gps \
                        libs/utils \
                        libs/cjson

ifeq "$(strip ${CFG_ALIYUN_SUPPORT})" "y"
    LOCAL_MODULE_DEPENDS += libs/aliyun
endif

ifeq "$(strip ${CFG_LCD_SUPPORT})" "y"
    LOCAL_MODULE_DEPENDS += libs/lcd
endif

ifeq "$(strip ${CFG_LVGL_SUPPORT})" "y"
    LOCAL_MODULE_DEPENDS += libs/lvgl
endif

ifeq "$(strip ${CFG_INA219_SUPPORT})" "y"
    LOCAL_MODULE_DEPENDS += libs/Adafruit_INA219
endif

ifeq "$(strip ${CFG_MLX90614_SUPPORT})" "y"
    LOCAL_MODULE_DEPENDS += libs/Adafruit_MLX90614
endif

ifeq "$(strip ${CFG_OPT3001_SUPPORT})" "y"
    LOCAL_MODULE_DEPENDS += libs/ClosedCube_OPT3001
endif

ifeq "$(strip ${CFG_MAX44009_SUPPORT})" "y"
    LOCAL_MODULE_DEPENDS += libs/MikeNZ_MAX44009
endif

ifeq "$(strip ${CFG_VEML6070_SUPPORT})" "y"
    LOCAL_MODULE_DEPENDS += libs/Adafruit_VEML6070
endif

ifeq "$(strip ${CFG_APDS9930_SUPPORT})" "y"
    LOCAL_MODULE_DEPENDS += libs/Depau_APDS9930
endif

ifeq "$(strip ${CFG_SI1145_SUPPORT})" "y"
    LOCAL_MODULE_DEPENDS += libs/Adafruit_SI1145
endif

# Add includes from other modules we do not wish to link to
LOCAL_API_DEPENDS := \


# include folder
LOCAL_ADD_INCLUDE := \



# `yes` if have submodule, or left empty  or `no`
IS_CONTAIN_SUB_MODULE := yes

## ------------------------------------ ##
## 	Add your custom flags here          ##
## ------------------------------------ ##
MYCFLAGS +=

## ------------------------------------- ##
##	List all your sources here           ##
## ------------------------------------- ##
S_SRC := ${notdir ${wildcard src/*.s}}
C_SRC := ${notdir ${wildcard src/*.c}}
CXX_SRC := ${notdir ${wildcard src/*.cpp}}

## ------------------------------------------------------------------- ##
##  Do Not touch below this line unless you know what you're doing.    ##
## ------------------------------------------------------------------- ##
include ${SOFT_WORKDIR}/platform/compilation/cust_rules.mk
