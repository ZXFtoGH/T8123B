###################################################################
# flag
###################################################################

MOD_SRC  =
MOD_OBJ  = $(PRELOADER_OUT)/obj

###################################################################
# source files
# - add your file here !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
###################################################################

#ifeq ("$(SECOND_PMIC_CHIP)","0x6373")
ifeq ("$(USE_MT6373_GPIO)", "yes")
MOD_SRC := gpio_ext.c
endif

###################################################################
# target
###################################################################

include $(COMMON_FILE_MK)

