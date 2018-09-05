



# Name of the this module
LOCAL_NAME := demo/gps


# Space-separated list of modules (libraries) your module depends upon.
# These should include the toplevel name, e.g. "libs/gps"
LOCAL_MODULE_DEPENDS :=  \


# Add includes from other modules we do not wish to link to
LOCAL_API_DEPENDS := libs/gps \
                     libs/utils \


# include folder
LOCAL_ADD_INCLUDE := include \
                     include/std_inc \
                     include/api_inc \
                     libs/gps/minmea/src \

                    
# `yes` if have submodule or left empty  or `no`
IS_CONTAIN_SUB_MODULE := no

## ------------------------------------ ##
## 	Add your custom flags here          ##
## ------------------------------------ ##
MYCFLAGS += 

## ------------------------------------- ##
##	List all your sources here           ##
## ------------------------------------- ##
S_SRC := ${notdir ${wildcard src/*.s}}
C_SRC := ${notdir ${wildcard src/*.c}}


## ------------------------------------------------------------------- ##
##  Do Not touch below this line unless you know what you're doing.    ##
## ------------------------------------------------------------------- ##
include ${SOFT_WORKDIR}/platform/compilation/cust_rules.mk
