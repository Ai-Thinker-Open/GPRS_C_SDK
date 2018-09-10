
# you can include you modules in the following script,just like 
# LOCAL_MODULE_DEPENDS += app
# LOCAL_MODULE_DEPENDS += demo/gpio

# add aliyun support, set CFG_ALIYUN_SUPPORT=y in config.mk
ifeq "$(strip ${CFG_ALIYUN_SUPPORT})" "y"
	LOCAL_LIBS += libs/aliyun/iotkit-embedded/output/release/lib/libiot_sdk.a
endif

