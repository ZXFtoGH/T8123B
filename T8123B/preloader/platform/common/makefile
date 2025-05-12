###################################################################
# flag
###################################################################

MOD_SRC  =
MOD_OBJ  = $(PRELOADER_OUT)/obj

###################################################################
# source files
# - add your file here !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
###################################################################

MOD_SRC  := \
        addr_trans.c \
        div0.c \
        stdlib.c \
        sboot.c \
        sec_hrid.c \
        string.c \
        error.c \
        hmac.c \
        pl_build_ver_check.c

ifndef CFG_AARCH64_BUILD
MOD_SRC  += \
        div/crt.S \
        div/idiv.S \
        div/idivmod.S \
        div/ldivmod.S \
        div/umodsi3.S \
        div/umoddi3.S \
        div/modsi3.S \
        div/llsl.S
endif

ifeq ("$(CFG_PROFILING_SUPPORT)", "yes")
MOD_SRC	 += profiling.c
else
MOD_SRC	 += profiling_dummy.c
endif

ifeq ("$(CFG_PLAT_DBG_INFO_LIB)", "1")
	MOD_SRC  += plat_dbg_info.c
endif

ifeq ("$(UBSAN_SUPPORT)", "yes")
	MOD_SRC  += sanitize_helper.c
endif

ifeq ("$(CFG_RAM_CONSOLE)", "1")
	MOD_SRC	+= ram_console.c
endif

ifeq ($(strip $(TARGET_BUILD_VARIANT)),user)
MTK_CFLAGS += -DTARGET_BUILD_VARIANT_USER
endif

ifeq ($(strip $(TARGET_BUILD_VARIANT)),eng)
MTK_CFLAGS += -DTARGET_BUILD_VARIANT_ENG
endif

ifeq ($(strip $(TARGET_BUILD_VARIANT)),userdebug)
MTK_CFLAGS += -DTARGET_BUILD_VARIANT_USERDEBUG
endif

###################################################################
# target
###################################################################
include $(COMMON_FILE_MK)

SUBDIRS += partition
SUBDIRS += storage
SUBDIRS += loader
SUBDIRS += bootctrl
SUBDIRS += emi
SUBDIRS += mblock
SUBDIRS += watchdog
ifeq ($(CFG_SYSTIMER),1)
	SUBDIRS += systimer
endif

ifeq ("$(CFG_GIC_COMMON)", "1")
	SUBDIRS += gic
endif

ifeq ("$(CFG_UART_COMMON)", "1")
	SUBDIRS += uart
endif

ifeq ($(CFG_ATF_SUPPORT),1)
SUBDIRS += trustzone
endif

ifeq ($(CFG_LOG_STORE_SUPPORT),1)
SUBDIRS += log_store
endif

ifeq ($(CFG_GZ_SUPPORT),1)
SUBDIRS += geniezone
endif

ifeq ("$(CREATE_PICACHU_LIB)", "TRUE")
ifneq ("$(wildcard $(MTK_PATH_COMMON)/picachu)", "")
SUBDIRS += picachu
endif
endif

SUBDIRS += gpio

ifeq ($(CFG_COMMON_SDA), 1)
	SUBDIRS += sda
endif

ifeq ("$(CFG_COMMON_RNG)", "1")
	SUBDIRS += rng
endif

include $(COMMON_DIR_MK)

