
# Copyright (C) 2016 MediaTek Inc.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License version 2 as
# published by the Free Software Foundation.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#

ccflags-y += \
	-I$(srctree)/drivers/misc/mediatek/include/			\
	-I$(srctree)/drivers/misc/mediatek/include/ppm_v3/		\
	-I$(srctree)/drivers/misc/mediatek/ppm_v3/inc/			\
	-I$(srctree)/drivers/misc/mediatek/include/upower/		\
	-I$(srctree)/drivers/misc/mediatek/include/spm/			\
	-I$(srctree)/drivers/misc/mediatek/include/mt-plat/		\
	-I$(srctree)/include/linux/					\
	-I$(srctree)/drivers/misc/mediatek/mcdi/include/		\
	-I$(srctree)/drivers/misc/mediatek/cpufreq_v2/inc/		\
	-I$(srctree)/drivers/gpu/mediatek/gpufreq/			\
	-I$(srctree)/drivers/misc/mediatek/mcupm/include/		\
	-I$(srctree)/drivers/misc/mediatek/mcdi/include/		\
	-I$(srctree)/include/linux/soc/mediatek/			\
	-I$(srctree)/drivers/gpu/mediatek/gpufreq/v2/include/





ifneq (,$(filter y m, $(CONFIG_MTK_TINYSYS_SSPM_SUPPORT)))
ifneq ($(strip $(CONFIG_MTK_TINYSYS_SSPM_V0)),)
ccflags-y += -I$(srctree)/drivers/misc/mediatek/sspm/v0/
else ifneq ($(strip $(CONFIG_MTK_TINYSYS_SSPM_V1)),)
ccflags-y += -I$(srctree)/drivers/misc/mediatek/sspm/v1/
endif
endif

ifeq ($(findstring k68v1_64_aging,$(PROJECT_DTB_NAMES)),k68v1_64_aging)
subdir-ccflags-y += -DAGING_LOAD
endif

# HQA load
ifeq ($(PROJECT_DTB_NAMES),mediatek/evb6779_64_hqact)
subdir-ccflags-y += -DCORN_LOAD
endif

# HQA load
ifeq ($(PROJECT_DTB_NAMES),mediatek/k79v1_64_ctighten)
subdir-ccflags-y += -DCORN_LOAD
endif

ifneq ($(strip $(CONFIG_MTK_PLAT_POWER_MT6768)),)
ccflags-y += -I$(srctree)/drivers/misc/mediatek/thermal/inc/
ccflags-y += -I$(srctree)/drivers/misc/mediatek/eem_v2/inc
obj-$(CONFIG_MTK_PTPOD_LEGACY) += mediatek_eem.o
mediatek_eem-y += mtk_eem.o
mediatek_eem-y += mtk_eem_platform.o
mediatek_eem-y += mtk_eem_internal.o
mediatek_eem-y += mtk_eem_api.o
mediatek_eem-y += mtk_aee_rr.o

else ifneq ($(strip $(CONFIG_MTK_PLAT_POWER_MT6765)),)
ccflags-y  += -I$(srctree)/drivers/misc/mediatek/thermal/v2/inc
ccflags-y += -I$(srctree)/drivers/misc/mediatek/eem_v2/common_v2/inc
obj-$(CONFIG_MTK_PTPOD_LEGACY) += mediatek_eem.o
mediatek_eem-y += common_v2/mtk_eem.o
mediatek_eem-y += common_v2/mtk_eem_platform.o
mediatek_eem-y += common_v2/mtk_eem_internal.o
mediatek_eem-y += common_v2/mtk_eem_api.o
obj-$(CONFIG_MTK_PTPOD_LEGACY) += mtk_picachu.o
mtk_picachu-y += common_v2/mtk_picachu.o
else ifneq ($(strip $(CONFIG_MTK_PLAT_POWER_MT6833)),)
ccflags-y  += -I$(srctree)/drivers/misc/mediatek/thermal/v1/inc
ccflags-y += -I$(srctree)/drivers/misc/mediatek/eem_v2/common_v3/inc
obj-$(CONFIG_MTK_PTPOD_LEGACY) += mediatek_eem.o
mediatek_eem-y += common_v3/mtk_eem.o
mediatek_eem-y += common_v3/mtk_eem_platform.o
mediatek_eem-y += common_v3/mtk_eem_internal.o
obj-$(CONFIG_MTK_PTPOD_LEGACY) += mtk_picachu.o
mtk_picachu-y += common_v3/mtk_picachu.o

else ifneq ($(strip $(CONFIG_MTK_PLAT_POWER_MT6739)),)
ccflags-y += -I$(srctree)/drivers/misc/mediatek/eem_v2/common_v0/inc
ccflags-y  += -I$(srctree)/drivers/misc/mediatek/thermal/v4/inc
obj-$(CONFIG_MTK_PTPOD_LEGACY) += mediatek_eem.o
mediatek_eem-y += common_v0/mtk_eem.o
mediatek_eem-y += common_v0/mtk_eem_platform.o
mediatek_eem-y += common_v0/mtk_eem_internal.o

else ifneq ($(strip $(CONFIG_MTK_PLAT_POWER_MT6761)),)
ccflags-y += -I$(srctree)/drivers/misc/mediatek/thermal
ccflags-y += -I$(srctree)/drivers/misc/mediatek/eem_v2/common_v4/inc
obj-$(CONFIG_MTK_PTPOD_LEGACY) += mediatek_eem.o
mediatek_eem-y += common_v4/mtk_eem.o
mediatek_eem-y += common_v4/mtk_eem_platform.o
mediatek_eem-y += common_v4/mtk_eem_internal.o
mediatek_eem-y += common_v4/mtk_eem_api.o
mediatek_eem-y += common_v4/mtk_aee_rr.o

else ifneq ($(strip $(CONFIG_MTK_PLAT_POWER_6893)),)
ccflags-y += -I$(srctree)/drivers/misc/mediatek/thermal/v5/inc/
ccflags-y += -I$(srctree)/drivers/misc/mediatek/eem_v2/common_6893/inc
obj-$(CONFIG_MTK_PTPOD_LEGACY) += mediatek_eem.o
mediatek_eem-y += common_6893/mtk_eem.o
mediatek_eem-y += common_6893/mtk_eem_platform.o
mediatek_eem-y += common_6893/mtk_eem_internal.o
mediatek_eem-y += common_6893/mtk_eem_api.o
endif
