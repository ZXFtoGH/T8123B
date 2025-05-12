###################################################################
# Include Files Directery
###################################################################
#include $(D_ROOT)/mtk_cust.mak
###################################################################
# Using arm & CLANG
###################################################################
ARCH    := arm
CLANG_PATH = $(D_ROOT)/../../../../../../prebuilts/clang/host/linux-x86/clang-r383902/bin/
CROSS_COMPILE = $(D_ROOT)/../../../../../../prebuilts/gcc/linux-x86/arm/arm-linux-androideabi-4.9.1/bin/arm-linux-androideabi-
AS  = $(CLANG_PATH)clang
LD	= $(CROSS_COMPILE)ld
CC  = $(CLANG_PATH)clang
CPP = $(CLANG_PATH)clang++
AR	= $(CROSS_COMPILE)ar
NM	= $(CROSS_COMPILE)nm
STRIP	= $(CROSS_COMPILE)strip
OBJCOPY = $(CROSS_COMPILE)objcopy
OBJDUMP = $(CROSS_COMPILE)objdump
RANLIB	= $(CROSS_COMPILE)RANLIB
CLANG_TRIPLE = --target=armv7-unknown-none-gnu
$(info (CC) -v)
###################################################################
# Initialize GCC Compile Parameter
###################################################################
C_VPF_FLAGS :=  -march=armv7-a  -mfpu=vfpv4  -mfloat-abi=soft
ASM_VPF_FLAGS :=  -march=armv7-a  -mfpu=vfpv4  -mfloat-abi=softfp

C_OPTION += $(CLANG_TRIPLE) -Os -g
C_OPTION += -mno-unaligned-access
C_OPTION += -c -Wall -ffunction-sections -fdata-sections --std=gnu11 -fno-builtin-bcmp
  
LDFLAGS += --gc-sections --relax -Bstatic --verbose
AFLAGS += $(CLANG_TRIPLE) -c $(ASM_VPF_FLAGS) -g  -DASSEMBLY -D__ASSEMBLY__ 
DA_OBJCOPY_FLAG := -j .text -j .data
#PL_CODE==1 --> BUILDING PL CODE
ifneq ($(strip $(PL_CODE)),)
###################################################################
INCLUDE_FILE := $(PL_INCLUDE_FILE)
#priority: da mode > pl project config > pl cust > pl default
include $(MTK_PATH_PLATFORM)/default.mak
include ${MTK_ROOT_CUSTOM}/${TARGET}/cust_bldr.mak
PROJECT_CONFIGS := $(MTK_ROOT_CUSTOM)/$(MTK_PROJECT)/$(MTK_PROJECT).mk
include $(PROJECT_CONFIGS)
#this must after -- include $(PROJECT_CONFIGS)
include $(MTK_PATH_PLATFORM)/flash/make_script/pl_feature_mask.mak
include $(MTK_PATH_PLATFORM)/feature.mak
C_OPTION += -D_FLASH_MODE_DA_
###################################################################
else
INCLUDE_FILE     := -I$(MTK_ROOT_PLATFORM)/flashc
INCLUDE_FILE     += -I$(MTK_ROOT_PLATFORM)/flashc/lib
INCLUDE_FILE     += -I$(MTK_ROOT_PLATFORM)/flashc/include
INCLUDE_FILE     += -I$(MTK_ROOT_PLATFORM)/flashc/include/lib
INCLUDE_FILE     += -I$(MTK_ROOT_PLATFORM)/flashc/arch/$(ARCH)/include
INCLUDE_FILE     += -I$(MTK_ROOT_PLATFORM)/flashc/driver
INCLUDE_FILE     += -I$(MTK_ROOT_PLATFORM)/flashc/driver/ufs
INCLUDE_FILE     += -I$(MTK_ROOT_PLATFORM)/common/storage/nand/include/internal
INCLUDE_FILE     += -I$(MTK_PATH_PLATFORM)/flash
INCLUDE_FILE     += -I$(MTK_PATH_PLATFORM)/flash/custom
INCLUDE_FILE     += -I$(MTK_PATH_PLATFORM)/flash/inc
INCLUDE_FILE     += -I$(MTK_PATH_PLATFORM)/flash/dev/ufs
include $(MTK_ROOT_CUSTOM)/$(MTK_PROJECT)/$(MTK_PROJECT).mk
include $(MTK_PATH_PLATFORM)/flash/make_script/feature.mak
endif
C_OPTION += -c $(INCLUDE_FILE) 
C_OPTION += -DDA_BUILD_STAGE=$(DA_BUILD_STAGE) -DUSE_CLANG_COMPILER
AFLAGS += -c $(INCLUDE_FILE)
   
ifdef DA_MODE
include $(MTK_PATH_PLATFORM)/flash/make_script/mode/$(DA_MODE).mak
endif
 
DA_1ST_SEC_LIB          := $(MTK_PATH_PLATFORM)/flash/sec_lib/libsec_1st.a 
DA_2ND_SEC_LIB          := $(MTK_PATH_PLATFORM)/flash/sec_lib/libsec_2nd.a 
DA_DEVINFO_LIB          := $(MTK_PATH_PLATFORM)/lib/libdevinfo.a $(MTK_PATH_PLATFORM)/lib/libdevinfo_debug.a
LDSCRIPT_STAGE1	:= $(MTK_PATH_PLATFORM)/flash/make_script/link_script_1st.ld
LDSCRIPT_STAGE2	:= $(MTK_PATH_PLATFORM)/flash/make_script/link_script_2nd.ld
###################################################################
#key words: version, contact, arch
FLASH_XML_CFG:=version=1.0,contact=Shuai.Zhang
export All_OBJS
