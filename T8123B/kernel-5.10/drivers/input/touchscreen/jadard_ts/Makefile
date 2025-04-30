ccflags-$(CONFIG_DRM_MEDIATEK_V2) += -I$(srctree)/drivers/gpu/drm/mediatek/mediatek_v2/

obj-$(CONFIG_TOUCHSCREEN_JADARD_TS) += jadard_ts.o

jadard_ts-y += jadard_common.o
jadard_ts-y += jadard_ic_JD9366T.o
jadard_ts-y += jadard_module.o
jadard_ts-y += jadard_platform.o
#jadard_ts-y += jadard_debug.o
#jadard_ts-y += jadard_sorting.o

ccflags-y += -D__JADARD_KMODULE__
ccflags-y += -DDEBUG
#ccflags-y += -DCONFIG_TOUCHSCREEN_JADARD_DEBUG=y
#ccflags-y += -DCONFIG_TOUCHSCREEN_JADARD_SORTING=y