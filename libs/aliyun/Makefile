



# Name of the this module
LOCAL_NAME := libs/aliyun


# Space-separated list of modules (libraries) your module depends upon.
# These should include the toplevel name, e.g. "libs/gps"
LOCAL_MODULE_DEPENDS := libs/aliyun/platform
                        


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

ALIYUN_PATH = $(SOFT_WORKDIR)/libs/aliyun/iotkit-embedded
ALIYUN_LIB_PATH = $(ALIYUN_PATH)/output/release/lib
ALIYUN_LIB_NAME = libiot_sdk.a

all:
	@echo "========================"
	@echo " start build aliyun lib "
	@echo "========================"
	@if [ ! -f "$(ALIYUN_PATH)/src/configs/default_settings.mk.bak" ]; then \
		cp $(ALIYUN_PATH)/src/configs/default_settings.mk $(ALIYUN_PATH)/src/configs/default_settings.mk.bak; \
	fi
	cp $(SOFT_WORKDIR)/libs/aliyun/config/default_settings.mk $(ALIYUN_PATH)/src/configs/
	cp $(SOFT_WORKDIR)/libs/aliyun/config/config.Ai_Thinker.gprs_a9 $(ALIYUN_PATH)/src/configs/
	# make -C $(ALIYUN_PATH) distclean
	make -C $(ALIYUN_PATH) SOFT_WORKDIR=${SOFT_WORKDIR}
	rm $(ALIYUN_PATH)/src/configs/config.Ai_Thinker.gprs_a9
	cp $(ALIYUN_PATH)/src/configs/default_settings.mk.bak $(ALIYUN_PATH)/src/configs/default_settings.mk
	@echo "lib $(ALIYUN_LIB_PATH)/$(ALIYUN_LIB_NAME)"
	@if [ -f "$(ALIYUN_LIB_PATH)/$(ALIYUN_LIB_NAME)" ]; then \
		echo "========================";\
		echo "build aliyun lib success";\
		echo "========================";\
	else \
		echo "========================";\
		echo "build aliyun lib fail!!!";\
		echo "========================";\
	fi


## ------------------------------------------------------------------- ##
##  Do Not touch below this line unless you know what you're doing.    ##
## ------------------------------------------------------------------- ##
include ${SOFT_WORKDIR}/platform/compilation/cust_rules.mk
