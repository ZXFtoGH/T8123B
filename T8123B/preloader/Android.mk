LOCAL_PATH := $(call my-dir)
PRELOADER_ROOT_DIR := $$(pwd)

ifdef PRELOADER_TARGET_PRODUCT
ifeq ($(BUILD_PRELOADER),yes)

  PRELOADER_DIR := $(LOCAL_PATH)
  PRELOADER_EFUSE_WRITER_SUPPORT := $(MTK_EFUSE_WRITER_SUPPORT)

include $(LOCAL_PATH)/platform/$(MTK_PLATFORM_DIR)/default.mak
ifdef VEXT_BASE_PROJECT
my_preloader_target := $(VEXT_BASE_PROJECT)
else ifdef MTK_BASE_PROJECT
my_preloader_target := $(MTK_BASE_PROJECT)
else
my_preloader_target := $(MTK_PROJECT_NAME)
endif
include $(LOCAL_PATH)/custom/$(my_preloader_target)/cust_bldr.mak
my_preloader_target :=

PRELOADER_UFS_HEADER_BLOCK := $(LOCAL_PATH)/platform/$(MTK_PLATFORM_DIR)/headerblock_for_ota_fastboot_ufs
PRELOADER_EMMC_HEADER_BLOCK := $(LOCAL_PATH)/platform/$(MTK_PLATFORM_DIR)/headerblock_for_ota_fastboot_emmc

ifeq ("$(CFG_BOOT_DEV)", "BOOTDEV_UFS")
  PRELOADER_HEADER_BLOCK := $(LOCAL_PATH)/platform/$(MTK_PLATFORM_DIR)/headerblock_for_ota_fastboot_ufs
else
  ifeq ("$(CFG_BOOT_DEV)", "BOOTDEV_SDMMC")
    PRELOADER_HEADER_BLOCK := $(LOCAL_PATH)/platform/$(MTK_PLATFORM_DIR)/headerblock_for_ota_fastboot_emmc
  endif
endif

  # Check flavor support
  PROJECT_CONFIGS         := $(PRELOADER_DIR)/custom/$(PRELOADER_TARGET_PRODUCT)/$(PRELOADER_TARGET_PRODUCT).mk
  ifdef TARGET_BOARD_PLATFORM
    PREBUILT_PI_IMG_TARGET := $(LOCAL_PATH)/custom/$(TARGET_BOARD_PLATFORM)/pi_img.img
  else
    PREBUILT_PI_IMG_TARGET := $(LOCAL_PATH)/custom/$(MTK_PLATFORM_DIR)/pi_img.img
  endif

  ifneq ($(wildcard $(PREBUILT_PI_IMG_TARGET)),)
    INSTALLED_PI_IMG_TARGET := $(PRODUCT_OUT)/pi_img.img
    PI_IMG_NAME := $(notdir $(INSTALLED_PI_IMG_TARGET))
  endif

  PL_MODE :=
  include $(LOCAL_PATH)/build_preloader.mk

  ifeq (yes,$(BOARD_BUILD_SBOOT_DIS))
    PL_MODE := SBOOT_DIS
    include $(LOCAL_PATH)/build_preloader.mk
  endif

  ifeq (yes,$(BOARD_BUILD_RPMB_KEY_PL))
    PL_MODE := RPMB_KEY
    include $(LOCAL_PATH)/build_preloader.mk
  endif

.PHONY: clean-preloader check-mtk-config check-pl-config
preloader pl: check-pl-config
ifneq ($(wildcard $(PREBUILT_PI_IMG_TARGET)),)
.PHONY: $(PI_IMG_NAME)
preloader pl: $(INSTALLED_PI_IMG_TARGET) $(PI_IMG_NAME)
$(PI_IMG_NAME):
$(INSTALLED_PI_IMG_TARGET): $(PREBUILT_PI_IMG_TARGET) | $(ACP)
	@echo  "!!!!!!!!!!!!!! PREBUILT_PI_IMG_TARGET2 $(PREBUILT_PI_IMG_TARGET) !!!!!!!!!!!!!!!!"
	$(copy-file-to-target)
endif

check-mtk-config: check-pl-config
check-pl-config:
ifneq (yes,$(strip $(DISABLE_MTK_CONFIG_CHECK)))
	python device/mediatek/build/build/tools/check_kernel_config.py -c $(VEXT_TARGET_PROJECT_FOLDER)/ProjectConfig.mk -b $(PROJECT_CONFIGS) -p $(MTK_PROJECT_NAME)
else
	-python device/mediatek/build/build/tools/check_kernel_config.py -c $(VEXT_TARGET_PROJECT_FOLDER)/ProjectConfig.mk -b $(PROJECT_CONFIGS) -p $(MTK_PROJECT_NAME)
endif

else

  PL_MODE :=
  include $(LOCAL_PATH)/prebuilt_preloader.mk

endif#BUILD_PRELOADER
.PHONY: preloader pl
droidcore: preloader
endif#PRELOADER_TARGET_PRODUCT


ifneq (,$(wildcard $(LOCAL_PATH)/platform/$(TARGET_BOARD_PLATFORM)/flash))
DA_DIR := $(LOCAL_PATH)
DA_ROOT_DIR := $(PWD)

ifdef VEXT_TARGET_PROJECT
my_da_project := $(VEXT_TARGET_PROJECT)
else
my_da_project := $(MTK_TARGET_PROJECT)
endif
ifneq ($(findstring fpga,$(my_da_project)),)
my_fpga_apprefix := fpga
else
my_fpga_apprefix :=
endif
FPGA_PROJECT := $(my_fpga_apprefix)
my_fpga_apprefix :=

DA_MODES :=
-include $(DA_DIR)/platform/$(TARGET_BOARD_PLATFORM)/flash/make_script/mode/mode_select.mak
$(info DA_MODES: $(DA_MODES))

DA_MAKE_DEPENDENCIES := $(shell find $(DA_DIR) -name .git -prune -o -type f | sort)
DA_MAKE_DEPENDENCIES := $(filter-out %/.git %/.gitignore %/.gitattributes,$(DA_MAKE_DEPENDENCIES))
$(foreach DA_MODE,$(DA_MODES),\
  $(eval include $(DA_DIR)/build_DA.mk)\
)

my_da_project :=
endif
