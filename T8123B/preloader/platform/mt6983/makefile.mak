###################################################################
# Include Files Directery
###################################################################

#include $(D_ROOT)/mtk_cust.mak

###################################################################
# Using GCC
###################################################################

ifeq ($(CFG_AARCH64_BUILD), 1)
    ARCH = armv8-a
    CROSS_COMPILE ?= aarch64-linux-android-
    CLANG_TRIPLE = --target=aarch64-arm-none-eabi
    THUMB =
else
    ARCH = armv7-a
    CROSS_COMPILE ?= arm-linux-androideabi-
    CLANG_TRIPLE = --target=thumbv7-unknown-none-gnu
    THUMB = -mthumb
endif

AS     = $(CLANG_PATH)clang
CC     = $(CLANG_PATH)clang
CPP    = $(CLANG_PATH)clang++
LD	= $(CROSS_COMPILE)ld
AR	= $(CROSS_COMPILE)ar
NM	= $(CROSS_COMPILE)nm
STRIP	= $(CROSS_COMPILE)strip
OBJCOPY = $(CROSS_COMPILE)objcopy
OBJDUMP = $(CROSS_COMPILE)objdump
RANLIB	= $(CROSS_COMPILE)RANLIB

###################################################################
# Initialize GCC Compile Parameter
###################################################################
DEFINE           = -D$(MTK_PLATFORM)
OBJCFLAGS 	 = --gap-fill=0xff
AFLAGS_DEBUG 	 = -Wa,-gstabs,
STRIP_SYMBOL	 = -fdata-sections -ffunction-sections

INCLUDE_FILE = $(INCLUDE_FILE_COMMON)
INCLUDE_FILE += \
    -I$(EMIGEN_OUT)/inc \
    -I$(MTK_PATH_CUSTOM)/inc \
    -I$(D_ROOT)/arch/arm64/include \
    -I$(D_ROOT)/custom/common/inc \
    -I$(D_ROOT)/inc/$(_CHIP) \
    -I$(MTK_ROOT_CUSTOM)/$(TARGET)/common \
    -I$(MTK_ROOT_CUSTOM)/kernel/dct \
    -I$(MTK_ROOT_OUT)/PTGEN/common \
    -I$(MTK_ROOT_OUT)/NANDGEN/common \
    -I$(PTGEN_OUT)/inc

ifeq ("$(MTK_EMMC_SUPPORT)","yes")
ifeq ($(strip "$(CFG_MMC_COMBO_DRV)"),"1")
INCLUDE_FILE += \
    -I$(MTK_PATH_COMMON)/storage/mmc/inc
endif
else
INCLUDE_FILE += \
    -I$(MTK_PATH_COMMON)/storage/mmc/inc
endif

ifeq ($(CFG_MICROTRUST_TEE_SUPPORT),1)
INCLUDE_FILE    += \
    -I$(MTK_PATH_PLATFORM)/src/security/trustzone/kdflib/inc
endif
###################################################################
# GCC Compile Options
###################################################################

ifeq ($(CREATE_SEC_LIB),TRUE)

INCLUDE_FILE     +=  \
    -I$(MTK_PATH_PLATFORM)/src/secure_lib/ \
    -I$(MTK_PATH_PLATFORM)/src/secure_lib/common/inc \
    -I$(MTK_PATH_PLATFORM)/src/secure_lib/common/src \
    -I$(MTK_PATH_PLATFORM)/src/secure_lib/common/crypto \
    -I$(MTK_PATH_PLATFORM)/src/secure_lib/platform/$(TARGET_BOARD_PLATFORM)/inc \
    -I$(MTK_PATH_PLATFORM)/src/secure_lib/platform/$(TARGET_BOARD_PLATFORM)/src \
########
# TZCC
########
INCLUDE_FILE     +=  \
	-I$(MTK_PATH_PLATFORM)/src/secure_lib/platform/$(TARGET_BOARD_PLATFORM)/src/tzcc/codesafe/src/secure_boot/crypto_driver/dsc \
	-I$(MTK_PATH_PLATFORM)/src/secure_lib/platform/$(TARGET_BOARD_PLATFORM)/src/tzcc/shared/include/pal/no_os  \
	-I$(MTK_PATH_PLATFORM)/src/secure_lib/platform/$(TARGET_BOARD_PLATFORM)/src/tzcc/shared/include/sbrom \
	-I$(MTK_PATH_PLATFORM)/src/secure_lib/platform/$(TARGET_BOARD_PLATFORM)/src/tzcc/shared/include/proj/ssi64 \
	-I$(MTK_PATH_PLATFORM)/src/secure_lib/platform/$(TARGET_BOARD_PLATFORM)/src/tzcc/shared/include/pal \
	-I$(MTK_PATH_PLATFORM)/src/secure_lib/platform/$(TARGET_BOARD_PLATFORM)/src/tzcc/codesafe/src/secure_boot/platform/common/tz  \
	-I$(MTK_PATH_PLATFORM)/src/secure_lib/platform/$(TARGET_BOARD_PLATFORM)/src/tzcc/codesafe/src/secure_boot/secure_boot_gen  \
	-I$(MTK_PATH_PLATFORM)/src/secure_lib/platform/$(TARGET_BOARD_PLATFORM)/src/tzcc/codesafe/src/crys/sym/driver/  \
	-I$(MTK_PATH_PLATFORM)/src/secure_lib/platform/$(TARGET_BOARD_PLATFORM)/src/tzcc/host/src/sbromlib  \
	-I$(MTK_PATH_PLATFORM)/src/secure_lib/platform/$(TARGET_BOARD_PLATFORM)/src/tzcc/codesafe/src/secure_boot/platform/pal/dx_linux	\
	-I$(MTK_PATH_PLATFORM)/src/secure_lib/platform/$(TARGET_BOARD_PLATFORM)/src/tzcc/shared/include	\
	-I$(MTK_PATH_PLATFORM)/src/secure_lib/platform/$(TARGET_BOARD_PLATFORM)/src/tzcc/shared/hw/include  \
	-I$(MTK_PATH_PLATFORM)/src/secure_lib/platform/$(TARGET_BOARD_PLATFORM)/src/tzcc/host/src/ssi4tzlib	\
	-I$(MTK_PATH_PLATFORM)/src/secure_lib/platform/$(TARGET_BOARD_PLATFORM)/src/tzcc/codesafe/src/secure_boot/platform/pal  \
	-I$(MTK_PATH_PLATFORM)/src/secure_lib/platform/$(TARGET_BOARD_PLATFORM)/src/tzcc/codesafe/src/secure_boot/crypto_driver	\
	-I$(MTK_PATH_PLATFORM)/src/secure_lib/platform/$(TARGET_BOARD_PLATFORM)/src/tzcc/codesafe/src/secure_boot/util  \
	-I$(MTK_PATH_PLATFORM)/src/secure_lib/platform/$(TARGET_BOARD_PLATFORM)/src/tzcc/codesafe/src/secure_boot/platform/hal/hal_host/dsc \
	-I$(MTK_PATH_PLATFORM)/src/secure_lib/platform/$(TARGET_BOARD_PLATFORM)/src/tzcc/codesafe/src/secure_boot/platform/hal
endif

ifeq ($(CREATE_DEVINFO_LIB),TRUE)
INCLUDE_FILE	 +=  \
	-I$(MTK_PATH_PLATFORM)/src/devinfo_lib/ \
	-I$(MTK_PATH_PLATFORM)/src/devinfo_lib/inc \
	-I$(MTK_PATH_PLATFORM)/src/devinfo_lib/src/platform/$(PLATFORM)/inc \
	-I$(D_ROOT)/platform/flashc \
	-I$(D_ROOT)/platform/flashc/include
endif

ifneq ($(filter TRUE,$(CREATE_SEC_LIB) $(CREATE_DEVINFO_LIB)),)
# if it's security.lib, we must remove gcc debug message
C_OPTION	 := $(CLANG_TRIPLE) -mgeneral-regs-only -gdwarf-2 -Os -fdata-sections -ffunction-sections -fno-strict-aliasing -fno-common -fno-builtin -ffreestanding -pipe -Wstrict-prototypes -march=$(ARCH) $(DEFINE) -c $(INCLUDE_FILE) -D__ASSEMBLY__  -DPRELOADER_HEAP -mno-unaligned-access
C_OPTION_OPTIMIZE	 := $(CLANG_TRIPLE) -mgeneral-regs-only -Os -fdata-sections -ffunction-sections -fno-strict-aliasing -fno-common -fno-builtin -ffreestanding -pipe -Wstrict-prototypes -march=$(ARCH) $(DEFINE) -c $(INCLUDE_FILE) -D__ASSEMBLY__  -DPRELOADER_HEAP -mno-unaligned-access
AFLAGS 		 := $(CLANG_TRIPLE) -c -march=$(ARCH) -g
AFLAGS_OPTIMIZE	 := $(CLANG_TRIPLE) -c -march=$(ARCH) -g
C_OPTION            += -D$(PLATFORM)
else
C_OPTION	    := $(CLANG_TRIPLE) -c -mno-global-merge -gdwarf-2 -Oz $(STRIP_SYMBOL) -fno-strict-aliasing -fno-common -fno-builtin -ffreestanding -pipe -Wstrict-prototypes -march=$(ARCH) $(DEFINE) $(INCLUDE_FILE) -msoft-float -D__ASSEMBLY__ -g -mgeneral-regs-only -mno-unaligned-access
C_OPTION_OPTIMIZE   := $(CLANG_TRIPLE) -c -mno-global-merge -gdwarf-2 -Oz $(STRIP_SYMBOL) -fno-strict-aliasing -fno-common -fno-builtin -ffreestanding -pipe -Wstrict-prototypes -march=$(ARCH) $(DEFINE) $(INCLUDE_FILE) -msoft-float -D__ASSEMBLY__ -g -mgeneral-regs-only -mno-unaligned-access
AFLAGS 		 := $(CLANG_TRIPLE) -c -march=$(ARCH) -g $(THUMB)
AFLAGS_OPTIMIZE	 := $(CLANG_TRIPLE) -c -march=$(ARCH) -g $(THUMB)
endif

ifeq ($(CREATE_SEC_LIB),TRUE)
C_OPTION            += -DBUILD_SEC_LIB_AARCH64
endif

ifeq ($(THUMB_MODE),TRUE)
#thumb
C_OPTION            += $(THUMB)
C_OPTION_OPTIMIZE   += $(THUMB)
endif

#priority: mode > project config > cust > default
include $(MTK_PATH_PLATFORM)/default.mak
include ${MTK_ROOT_CUSTOM}/${TARGET}/cust_bldr.mak
PROJECT_CONFIGS := $(MTK_ROOT_CUSTOM)/$(MTK_PROJECT)/$(MTK_PROJECT).mk
include $(PROJECT_CONFIGS)

ifdef PL_MODE
C_OPTION += -D$(PL_MODE)
C_OPTION_OPTIMIZE += -D$(PL_MODE)
include $(MTK_PATH_PLATFORM)/mode/$(PL_MODE).mak
endif

ifeq ("$(MTK_SECURITY_SW_SUPPORT)","yes")
	C_OPTION += -DMTK_SECURITY_SW_SUPPORT
endif

ifeq ($(strip $(TARGET_BUILD_VARIANT)),eng)
    C_OPTION += -DTARGET_BUILD_VARIANT_ENG
endif

ifeq ($(strip $(TARGET_BUILD_VARIANT)),userdebug)
    C_OPTION += -DTARGET_BUILD_VARIANT_USERDEBUG
endif

ifeq ($(strip $(TARGET_BUILD_VARIANT)),user)
    C_OPTION += -DTARGET_BUILD_VARIANT_USER
endif

ifeq ("$(MTK_EMMC_SUPPORT)","yes")
    C_OPTION += -DMTK_EMMC_SUPPORT
endif

ifeq ("$(MTK_I2C_CH0_PULL_DIS)","yes")
    C_OPTION += -DMTK_I2C_CH0_PULL_DIS
endif

ifeq ("$(MTK_I2C_CH1_PULL_DIS)","yes")
    C_OPTION += -DMTK_I2C_CH1_PULL_DIS
endif

ifeq ("$(MTK_I2C_CH2_PULL_DIS)","yes")
    C_OPTION += -DMTK_I2C_CH2_PULL_DIS
endif

ifeq ("$(MTK_I2C_CH3_PULL_DIS)","yes")
    C_OPTION += -DMTK_I2C_CH3_PULL_DIS
endif

ifeq ("$(MTK_I2C_CH4_PULL_DIS)","yes")
    C_OPTION += -DMTK_I2C_CH4_PULL_DIS
endif

ifeq ("$(MTK_I2C_CH5_PULL_DIS)","yes")
    C_OPTION += -DMTK_I2C_CH5_PULL_DIS
endif

ifeq ("$(MTK_I2C_CH6_PULL_DIS)","yes")
    C_OPTION += -DMTK_I2C_CH6_PULL_DIS
endif

include $(MTK_PATH_PLATFORM)/feature.mak

MTK_CDEFS := $(PL_MTK_CDEFS)
MTK_ADEFS := $(PL_MTK_ADEFS)

C_OPTION += $(MTK_CFLAGS) $(MTK_CDEFS) $(MTK_INC)
AFLAGS   += $(MTK_AFLAGS)

ifeq ("$(CFG_FUNCTION_PICACHU_SUPPORT)", "1")
AFLAGS   += -mfpu=neon-vfpv4 -mfloat-abi=softfp
endif
###################################################################
# gcc link descriptor
###################################################################

ifeq ($(findstring fpga, $(TARGET)),fpga)
LDSCRIPT	:= $(MTK_PATH_PLATFORM)/link_descriptor_fpga.ld
else
LDSCRIPT	:= $(MTK_PATH_PLATFORM)/link_descriptor.ld
ifeq ("$(CFG_ENABLE_DCACHE)","1")
LDSCRIPT	:= $(MTK_PATH_PLATFORM)/link_descriptor_pgtbl.ld
endif
ifdef PL_MODE
ifeq ($(strip $(PL_MODE)), TRNG_R1)
LDSCRIPT	:= $(MTK_PATH_PLATFORM)/link_descriptor_trng.ld
endif
ifeq ($(strip $(PL_MODE)), TRNG_R2)
LDSCRIPT	:= $(MTK_PATH_PLATFORM)/link_descriptor_trng.ld
endif
endif # ifdef PL_MODE
endif # ifeq ($(findstring fpga, $(TARGET)),fpga)


LINKFILE	:= $(LD)
LINK		:= $(LINKFILE) -Bstatic -T $(LDSCRIPT) --gc-sections

###################################################################
# Object File
###################################################################

export All_OBJS
