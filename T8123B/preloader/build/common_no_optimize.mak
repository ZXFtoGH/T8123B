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

include $(MTK_PATH_PLATFORM)/makefile.mak

C_FILES   := $(filter %.c, $(MOD_SRC))
ASM_FILES := $(filter %.s, $(MOD_SRC)) 

OBJS_FROM_C := $(C_FILES:%.c=%.o)
OBJS_FROM_ASM := $(ASM_FILES:%.s=%.o)

All_OBJS += OBJS_FROM_C
All_OBJS += OBJS_FROM_ASM

OBJS_FROM_C := $(addprefix $(MOD_OBJ)/, $(C_FILES:%.c=%.o))
OBJS_FROM_ASM := $(addprefix $(MOD_OBJ)/, $(ASM_FILES:%.s=%.o))

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
C_OPTION_NO_OPTIMIZE += $(UBSAN_FLAGS) $(KASAN_FLAGS)

define COMPILE_C
	@echo [CC] $@
	@$(CC) $(C_OPTION_NO_OPTIMIZE) -o $@ $<
endef

define COMPILE_ASM
	@echo [AS] $@
	@$(CC) $(AFLAGS_NO_OPTIMIZE) -o $@ $<
endef

all: show $(OBJS_FROM_C) $(OBJS_FROM_ASM) 

show:
	@echo .......... Complete .........

$(OBJS_FROM_C) : $(MOD_OBJ)/%.o : %.c
	$(COMPILE_C)	

$(OBJS_FROM_ASM) : $(MOD_OBJ)/%.o : %.s
	$(COMPILE_ASM)
