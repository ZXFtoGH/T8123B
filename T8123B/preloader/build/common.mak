# Copyright Statement:
#
# This software/firmware and related documentation ("MediaTek Software") are
# protected under relevant copyright laws. The information contained herein
# is confidential and proprietary to MediaTek Inc. and/or its licensors.
# Without the prior written permission of MediaTek inc. and/or its licensors,
# any reproduction, modification, use or disclosure of MediaTek Software,
# and information contained herein, in whole or in part, shall be strictly prohibited.

# MediaTek Inc. (C) 2010. All rights reserved.
#
# BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
# THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
# RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
# AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
# NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
# SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
# SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
# THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
# THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
# CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
# SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
# STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
# CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
# AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
# OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
# MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
#
# The following software/firmware and/or related documentation ("MediaTek Software")
# have been modified by MediaTek Inc. All revisions are subject to any receiver's
# applicable license agreements with MediaTek Inc.

ifeq ($(strip $(OBJ_PATH)),)
OBJ_PATH := $(PRELOADER_OUT)/obj
endif
MOD_OBJ := $(OBJ_PATH)

##############################################################
# Include MakeFile
##############################################################

ifneq ($(strip $(DA_MODE)),)
include $(MTK_PATH_PLATFORM)/flash/make_script/makefile.mak
else
include $(MTK_PATH_PLATFORM)/makefile.mak
endif

##############################################################
# Initialize Variables
##############################################################

C_FILES         := $(filter %.c, $(MOD_SRC))
ASM_FILES       := $(filter %.s, $(MOD_SRC))
ASM_FILES_S     := $(filter %.S, $(MOD_SRC))
CRYPTO_FILES := $(filter %.c, $(CRYPTO_SRC))
CRYPTO_FILES_ASM_FILES_S := $(filter %.S, $(CRYPTO_SRC))
COMMON_MOD_FILES  := $(filter %.c, $(MOD_COMMON_SRC))
PLAT_MOD_FILES  := $(filter %.c, $(MOD_PLAT_SRC))
DEVINFO_MOD_FILES  := $(filter %.c, $(MOD_DEVINFO_SRC))
DEVINFO_DEBUG_MOD_FILES  := $(filter %.c, $(MOD_DEVINFO_DEBUG_SRC))
C_FILES_PLUS    := $(filter %.c, $(MOD_SRC_PLUS))
HW_CYRPTO_LIB_FILES := $(filter %.c, $(HW_CYRPTO_LIB_SRC))
DA_FILES := $(filter %.c, $(DA_SRC))
DA_FILES_ASM_FILES_S := $(filter %.S, $(DA_SRC))
PL_PREBUILD_O_FILES := $(filter %.o, $(PL_PREBUILT_O_SRC))
DA_PREBUILT_O_FILES := $(filter %.o, $(DA_PREBUILT_O_SRC))

OBJS_FROM_C           := $(addprefix $(MOD_OBJ)/, $(C_FILES:%.c=%.o))
OBJS_FROM_ASM         := $(addprefix $(MOD_OBJ)/, $(ASM_FILES:%.s=%.o))
OBJS_FROM_ASM_S       := $(addprefix $(MOD_OBJ)/, $(ASM_FILES_S:%.S=%.o))
CRYPTO_OBJS_FROM_C := $(addprefix $(CRYPTO_OBJ)/, $(CRYPTO_FILES:%.c=%.o))
CRYPTO_OBJS_FROM_ASM  := $(addprefix $(CRYPTO_OBJ)/, $(CRYPTO_FILES_ASM_FILES_S:%.S=%.o))
COMMON_MOD_OBJS_FROM_C  := $(addprefix $(MOD_COMMON_OBJ)/, $(COMMON_MOD_FILES:%.c=%.o))
PLAT_MOD_OBJS_FROM_C  := $(addprefix $(MOD_PLAT_OBJ)/, $(PLAT_MOD_FILES:%.c=%.o))
DEVINFO_MOD_OBJS_FROM_C  := $(addprefix $(MOD_DEVINFO_OBJ)/, $(DEVINFO_MOD_FILES:%.c=%.o))
DEVINFO_DEBUG_MOD_OBJS_FROM_C  := $(addprefix $(MOD_DEVINFO_DEBUG_OBJ)/, $(DEVINFO_DEBUG_MOD_FILES:%.c=%.o))
OBJS_FROM_C_PLUS      := $(addprefix $(MOD_OBJ)/, $(C_FILES_PLUS:%.c=%.o))
HW_CYRPTO_LIB_OBJS_FROM_C := $(addprefix $(HW_CYRPTO_LIB_OBJ)/, $(HW_CYRPTO_LIB_FILES:%.c=%.o))
DA_OBJS_FROM_C := $(addprefix $(DA_OBJ)/, $(DA_FILES:%.c=%.o))
DA_OBJS_FROM_ASM  := $(addprefix $(DA_OBJ)/, $(DA_FILES_ASM_FILES_S:%.S=%.o))
PL_OBJS_FROM_PREBUILD_O := $(addprefix $(MOD_PLAT_OBJ)/, $(PL_PREBUILD_O_FILES))
DA_OBJS_FROM_PREBUILT_O := $(addprefix $(DA_OBJ)/, $(DA_PREBUILT_O_FILES))

DEPS_FROM_OBJS        := $(wildcard $(patsubst %.o,%.P,$(OBJS_FROM_C) $(OBJS_FROM_ASM_S) $(CRYPTO_OBJS_FROM_C) $(CRYPTO_OBJS_FROM_ASM) $(COMMON_MOD_OBJS_FROM_C) $(PLAT_MOD_OBJS_FROM_C) $(DEVINFO_MOD_OBJS_FROM_C) $(DEVINFO_DEBUG_MOD_OBJS_FROM_C) $(OBJS_FROM_C_PLUS) $(HW_CYRPTO_LIB_OBJS_FROM_C) $(DA_OBJS_FROM_C) $(DA_OBJS_FROM_ASM)))
DEPS_FROM_MAKEFILE    := $(filter-out %.d %.P,$(ALL_DEPENDENCY_FILE) $(MAKEFILE_LIST))

cc-option = $(shell set -e; \
    TMP=".$$$$.tmp"; \
    TMPO=".$$$$.o"; \
    if ($(CC) -Werror $(1) -c -x c /dev/null -o "$$TMP") >/dev/null 2>&1; \
    then echo "$(1)"; \
    else echo "$(2)"; \
    fi; \
    rm -f "$$TMP" "$$TMPO")
UBSAN_FLAGS :=
ifeq ("$(UBSAN_SUPPORT)", "yes")
    UBSAN_FLAGS += $(call cc-option, -fsanitize=shift)
    UBSAN_FLAGS += $(call cc-option, -fsanitize=integer-divide-by-zero)
    UBSAN_FLAGS += $(call cc-option, -fsanitize=unreachable)
    UBSAN_FLAGS += $(call cc-option, -fsanitize=signed-integer-overflow)
    UBSAN_FLAGS += $(call cc-option, -fsanitize=bounds)
    UBSAN_FLAGS += $(call cc-option, -fsanitize=object-size)
    UBSAN_FLAGS += $(call cc-option, -fsanitize=returns-nonnull-attribute)
    UBSAN_FLAGS += $(call cc-option, -fsanitize=bool)
    UBSAN_FLAGS += $(call cc-option, -fsanitize=enum)
    #UBSAN_FLAGS += $(call cc-option, -fsanitize=alignment)
    #UBSAN_FLAGS += $(call cc-option, -fsanitize=null) # cause preloader size double and overflow!
    UBSAN_FLAGS += $(call cc-option, -Wno-maybe-uninitialized)
    UBSAN_FLAGS += -DUBSAN_SUPPORT
endif
KASAN_FLAGS :=
ifeq ("$(KASAN_SUPPORT)", "yes")
    KASAN_FLAGS += -DKASAN_SUPPORT
endif
MTK_CFLAGS += $(UBSAN_FLAGS) $(KASAN_FLAGS) # need add to MTK_CFLAGS, C_OPTION will clear after include $(COMMON_DIR_MK)
C_OPTION += $(UBSAN_FLAGS) $(KASAN_FLAGS)

##############################################################
# Specify Builld Command
##############################################################

define COMPILE_C
	$(CC) $(C_OPTION) $(C_VPF_FLAGS) -D__PL_FILE__='"$(patsubst $(D_ROOT)/%,%,$<)"' -MP -MD -MF $(patsubst %.o,%.d,$@) -o $@ $<
	sed -e 's%$(OBJ_PATH)%$$(OBJ_PATH)%g' -e 's%$(PRELOADER_ROOT_DIR)%$$(PRELOADER_ROOT_DIR)%g' $(patsubst %.o,%.d,$@) >$(patsubst %.o,%.P,$@)
	rm -f $(patsubst %.o,%.d,$@)
endef

define COMPILE_ASM
	$(AS) $(AFLAGS) -o $@ $<
endef

define COMPILE_ASM_ARMV8
       $(CC) $(C_OPTION) $(ASM_VPF_FLAGS) -MP -MD -MF $(patsubst %.o,%.d,$@) -DASSEMBLY -o $@ $<
	sed -e 's%$(OBJ_PATH)%$$(OBJ_PATH)%g' -e 's%$(PRELOADER_ROOT_DIR)%$$(PRELOADER_ROOT_DIR)%g' $(patsubst %.o,%.d,$@) >$(patsubst %.o,%.P,$@)
	rm -f $(patsubst %.o,%.d,$@)
endef
##############################################################
# Main Flow
##############################################################

CURDIR_FLAG := $(OBJ_PATH)/$(subst /,_,$(patsubst $(D_ROOT)/%,%,$(patsubst %/,%,$(CURDIR)))).flag
all: $(CURDIR_FLAG)

$(OBJS_FROM_C) : $(MOD_OBJ)/%.o : $(CURDIR)/%.c
	@mkdir -p $(dir $@)
	$(COMPILE_C)

$(OBJS_FROM_ASM_S) : $(MOD_OBJ)/%.o : $(CURDIR)/%.S
	@mkdir -p $(dir $@)
	$(COMPILE_ASM_ARMV8)

$(OBJS_FROM_ASM) : $(MOD_OBJ)/%.o : $(CURDIR)/%.s
	@mkdir -p $(dir $@)
	$(COMPILE_ASM)

$(CRYPTO_OBJS_FROM_C) : $(CRYPTO_OBJ)/%.o : $(CURDIR)/%.c
	@mkdir -p $(dir $@)
	$(COMPILE_C)

$(COMMON_MOD_OBJS_FROM_C) : $(MOD_COMMON_OBJ)/%.o : $(CURDIR)/%.c
	@mkdir -p $(dir $@)
	$(COMPILE_C)

$(PLAT_MOD_OBJS_FROM_C) : $(MOD_PLAT_OBJ)/%.o : $(CURDIR)/%.c
	@mkdir -p $(dir $@)
	$(COMPILE_C)

$(CRYPTO_OBJS_FROM_ASM) : $(CRYPTO_OBJ)/%.o : $(CURDIR)/%.S
	@mkdir -p $(dir $@)
	$(COMPILE_ASM_ARMV8)

$(DEVINFO_MOD_OBJS_FROM_C) : $(MOD_DEVINFO_OBJ)/%.o : $(CURDIR)/%.c
	@mkdir -p $(dir $@)
	$(COMPILE_C)

$(DEVINFO_DEBUG_MOD_OBJS_FROM_C) : $(MOD_DEVINFO_DEBUG_OBJ)/%.o : $(CURDIR)/%.c
	@mkdir -p $(dir $@)
	$(COMPILE_C)

$(HW_CYRPTO_LIB_OBJS_FROM_C) : $(HW_CYRPTO_LIB_OBJ)/%.o : $(CURDIR)/%.c
	@mkdir -p $(dir $@)
	$(COMPILE_C)

# add for custom_emi.c/cust_part.c
$(OBJS_FROM_C_PLUS) : $(MOD_OBJ)/%.o : $(PRELOADER_OUT)/%.c
	@mkdir -p $(dir $@)
	$(COMPILE_C)

$(DA_OBJS_FROM_C) : $(DA_OBJ)/%.o : $(CURDIR)/%.c
	@mkdir -p $(dir $@)
	$(COMPILE_C)

$(DA_OBJS_FROM_ASM) : $(DA_OBJ)/%.o : $(CURDIR)/%.S
	@mkdir -p $(dir $@)
	$(COMPILE_C)

$(DA_OBJS_FROM_PREBUILT_O) : $(DA_OBJ)/%.o : $(CURDIR)/%.o
	@mkdir -p $(dir $@)
	cp -f $< $@

$(PL_OBJS_FROM_PREBUILD_O) : $(MOD_PLAT_OBJ)/%.o : $(CURDIR)/%.o
	@mkdir -p $(dir $@)
	cp -f $< $@

-include $(DEPS_FROM_OBJS)

$(OBJS_FROM_C) $(OBJS_FROM_ASM) $(OBJS_FROM_ASM_S) $(CRYPTO_OBJS_FROM_C) $(CRYPTO_OBJS_FROM_ASM) $(COMMON_MOD_OBJS_FROM_C) $(PLAT_MOD_OBJS_FROM_C) $(DEVINFO_MOD_OBJS_FROM_C) $(DEVINFO_DEBUG_MOD_OBJS_FROM_C) $(OBJS_FROM_C_PLUS) $(HW_CYRPTO_LIB_OBJS_FROM_C) $(DA_OBJS_FROM_C) $(DA_OBJS_FROM_ASM) $(DA_OBJS_FROM_PREBUILT_O) $(PL_OBJS_FROM_PREBUILD_O): $(DEPS_FROM_MAKEFILE)
$(CURDIR_FLAG): $(OBJS_FROM_C) $(OBJS_FROM_ASM) $(OBJS_FROM_ASM_S) $(CRYPTO_OBJS_FROM_C) $(CRYPTO_OBJS_FROM_ASM) $(COMMON_MOD_OBJS_FROM_C) $(PLAT_MOD_OBJS_FROM_C) $(DEVINFO_MOD_OBJS_FROM_C) $(DEVINFO_DEBUG_MOD_OBJS_FROM_C) $(OBJS_FROM_C_PLUS) $(DA_OBJS_FROM_C) $(DA_OBJS_FROM_ASM) $(DA_OBJS_FROM_PREBUILT_O) $(PL_OBJS_FROM_PREBUILD_O)

ifndef CURDIR_FLAG_RULE
# avoid warning when common-dir.mak and common.mak are both included
CURDIR_FLAG_RULE := true
$(CURDIR_FLAG):
	@mkdir -p $(dir $@)
	echo "$^" | sed -e 's%$(OBJ_PATH)%$$(OBJ_PATH)%g' > $@

endif
