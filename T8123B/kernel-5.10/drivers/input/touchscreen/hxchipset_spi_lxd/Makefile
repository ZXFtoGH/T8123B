# Makefile for the Himax touchscreen drivers.

ifeq ($(CONFIG_DRM_MEDIATEK), y)
ccflags-$(CONFIG_DRM_MEDIATEK) += -I$(srctree)/drivers/gpu/drm/mediatek/
endif
ccflags-$(CONFIG_DRM_MEDIATEK_V2) += -I$(srctree)/drivers/gpu/drm/mediatek/mediatek_v2/


ccflags-y += -DHX_MTK_K510

ifneq ($(filter y, $(CONFIG_KALLSYMS_ALL)),)
	ccflags-y += -D__KERNEL_KALLSYMS_ALL_ENABLED__
endif

ifneq ($(filter y, $(CONFIG_FB)),)
	ccflags-y += -DHX_CONFIG_FB
endif

ifneq ($(filter y, $(CONFIG_PM)),)
	ccflags-y += -DHX_CONFIG_PM
endif

# Generally, it is useless for this platform
# ifneq ($(filter y, $(CONFIG_DRM)),)
# 	ccflags-y += -DHX_CONFIG_DRM
# endif

# obj-y += himax_modular_table.o

ifneq ($(filter y m, $(CONFIG_TOUCHSCREEN_HIMAX_DEBUG)),)
	hxchipset_spi_lxd-objs += himax_debug.o
endif

ifneq ($(filter y m, $(CONFIG_TOUCHSCREEN_HIMAX_INSPECT)),)
	hxchipset_spi_lxd-objs += himax_inspection.o
endif

ifneq ($(filter y m, $(CONFIG_TOUCHSCREEN_HIMAX_IC_HX83108A)),)
  ccflags-y += -D__HIMAX_HX83108A_MOD__
	hxchipset_spi_lxd-objs += himax_ic_HX83108a.o
endif

ifneq ($(filter y m, $(CONFIG_TOUCHSCREEN_HIMAX_IC_HX83102J)),)
  ccflags-y += -D__HIMAX_HX83102J_MOD__
	hxchipset_spi_lxd-objs += himax_ic_HX83102j.o
endif

ifneq ($(filter y m, $(CONFIG_TOUCHSCREEN_HIMAX_IC_HX83112F)),)
  ccflags-y += -D__HIMAX_HX83112F_MOD__
	hxchipset_spi_lxd-objs += himax_ic_HX83112f.o
endif

ifneq ($(filter y m, $(CONFIG_TOUCHSCREEN_HIMAX_IC_HX83121A)),)
  ccflags-y += -D__HIMAX_HX83121A_MOD__
	hxchipset_spi_lxd-objs += himax_ic_HX83121a.o
endif

ifneq ($(filter y m, $(CONFIG_TOUCHSCREEN_HIMAX_IC_HX83122A)),)
  ccflags-y += -D__HIMAX_HX83122A_MOD__
	hxchipset_spi_lxd-objs += himax_ic_HX83122a.o
endif

ifneq ($(filter y m, $(CONFIG_TOUCHSCREEN_HIMAX_IC_HX8530K)),)
  ccflags-y += -D__HIMAX_HX8530K_MOD__
	hxchipset_spi_lxd-objs += himax_ic_HX8530k.o
endif

ifneq ($(filter m, $(CONFIG_TOUCHSCREEN_HIMAX_COMMON)),)
  ccflags-y += -D__HIMAX_MOD__
  hxchipset_spi_lxd-objs += himax_ic_core.o
  hxchipset_spi_lxd-objs += himax_common.o
  hxchipset_spi_lxd-objs += himax_platform.o
  ifneq ($(filter y, $(CONFIG_TOUCHSCREEN_HIMAX_EMBEDDED_FIRMWARE)),)
    ccflags-y += -DHX_FIRMWARE_HEADER
    hxchipset_spi_lxd-objs += himax_firmware.o
  endif
  obj-m += hxchipset_spi_lxd.o
endif
ifneq ($(filter y, $(CONFIG_TOUCHSCREEN_HIMAX_COMMON)),)
  hxchipset_spi_lxd-objs += himax_ic_core.o
  hxchipset_spi_lxd-objs += himax_common.o
  hxchipset_spi_lxd-objs += himax_platform.o
  ifneq ($(filter y, $(CONFIG_TOUCHSCREEN_HIMAX_EMBEDDED_FIRMWARE)),)
	ccflags-y += -DHX_FIRMWARE_HEADER
	hxchipset_spi_lxd-objs += himax_firmware.o
  endif
  obj-y += hxchipset_spi_lxd.o
endif

