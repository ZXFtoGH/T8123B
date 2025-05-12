###################################################################
# flag
###################################################################

MOD_SRC  =
MOD_OBJ  = $(PRELOADER_OUT)/obj

###################################################################
# source files
# - add your file here !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
###################################################################
include $(COMMON_FILE_MK)

MOD_SRC :=

ifeq ("$(CFG_EMI_MPU_LIB)", "1")
MOD_SRC += emi_mpu_v1.c
endif

ifeq ("$(CFG_EMI_MPU_LIB)", "2")
MOD_SRC += emi_mpu_v2.c
endif

ifeq ("$(CFG_EMI_ADDR2DRAM_VER)", "1")
MOD_SRC += emi-addr2dram-v1.c
endif

ifeq ("$(CFG_EMI_ADDR2DRAM_VER)", "2")

MOD_SRC += emi-addr2dram-v2.c
endif

###################################################################
# target
###################################################################
include $(COMMON_FILE_MK)

