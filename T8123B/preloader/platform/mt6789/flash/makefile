###################################################################
# flag
###################################################################

MOD_SRC  = 
MOD_OBJ  = $(OBJ_PATH)

###################################################################
# source files
# - add your file here !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
###################################################################

MOD_SRC  := ext_commands.c

ifeq ($(DA_BUILD_STAGE),1)
MOD_SRC += cmd_bootstrap1.c
MOD_SRC += sec_policy_wrapper1.c
else
MOD_SRC += cmd_bootstrap2.c
MOD_SRC += sec_policy_wrapper2.c
MOD_SRC += override_interface.c
endif

###################################################################
# target
###################################################################

include $(COMMON_FILE_MK)

###################################################################
# include sub dir
###################################################################

SUBDIRS += custom

ifeq ($(DA_USE_PL_HW_DRV),yes)
SUBDIRS += dev
else
SUBDIRS += dev-sub
endif

ifeq ($(CREATE_SEC_LIB),TRUE)
SUBDIRS += security
endif

include $(COMMON_DIR_MK)
