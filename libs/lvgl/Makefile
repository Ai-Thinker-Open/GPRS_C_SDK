



# Name of the this module
LOCAL_NAME := libs/lvgl


# Space-separated list of modules (libraries) your module depends upon.
# These should include the toplevel name, e.g. "libs/gps"
LOCAL_MODULE_DEPENDS := 



# Add includes from other modules we do not wish to link to
LOCAL_API_DEPENDS := libs/utils \


# include folder
LOCAL_ADD_INCLUDE := include \
                     include/std_inc \
                     include/api_inc \
                     libs/lvgl \
                     libs/lvgl/src \
                     libs/lvgl/src/lv_core \
                     libs/lvgl/src/lv_draw \
                     libs/lvgl/src/lv_hal \
                     libs/lvgl/src/lv_misc \
                     libs/lvgl/src/lv_objx \
                     libs/lvgl/src/lv_themes 

                    
# `yes` if have submodule or left empty  or `no`
IS_CONTAIN_SUB_MODULE := no

## ------------------------------------ ##
## 	Add your custom flags here          ##
## ------------------------------------ ##
MYCFLAGS += 

LOCAL_EXPORT_FLAG += LVGL_CONFIG_FILE=\"${CFG_LVGL_CONFIG_FILE}\"


## ------------------------------------- ##
##	List all your sources here           ##
## ------------------------------------- ##
S_SRC := ${notdir ${wildcard src/*.s}}
C_SRC := ${notdir ${wildcard src/*.c}}
C_SRC_FILES +=  ${wildcard src/lv_core/*.c} \
                ${wildcard src/lv_draw/*.c} \
                ${wildcard src/lv_hal/*.c} \
                ${wildcard src/lv_misc/*.c} \
                ${wildcard src/lv_objx/*.c} \
                ${wildcard src/lv_themes/*.c} \
                ${wildcard src/lv_misc/lv_fonts/*.c} 



## ------------------------------------------------------------------- ##
##  Do Not touch below this line unless you know what you're doing.    ##
## ------------------------------------------------------------------- ##
include ${SOFT_WORKDIR}/platform/compilation/cust_rules.mk
