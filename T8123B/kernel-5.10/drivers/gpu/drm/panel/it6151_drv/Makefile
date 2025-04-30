#
# Makefile for misc devices that really don't fit anywhere else.
#
#include $(srctree)/drivers/misc/mediatek/Makefile.custom
ccflags-y += -I$(srctree)/drivers/misc/mediatek/include/mt-plat

#obj-$(CONFIG_DRM_PANEL_EDP_IT6151_DRV)  += it6151_drv.o
#it6151_drv-$(CONFIG_DRM_PANEL_EDP_IT6151_DRV) += it6151_edp.o it6151_mipirx.o it6151_test.o

obj-$(CONFIG_DRM_PANEL_EDP_IT6151_DRV) += it6151_edp.o
obj-$(CONFIG_DRM_PANEL_EDP_IT6151_DRV) += it6151_mipirx.o
#obj-$(CONFIG_DRM_PANEL_EDP_IT6151_DRV) += it6151_test.o
