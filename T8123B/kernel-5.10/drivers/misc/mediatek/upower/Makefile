
# Copyright (C) 2017 MediaTek Inc.
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
	-I$(srctree)/drivers/misc/mediatek/include/mt-plat/		\
	-I$(srctree)/drivers/misc/mediatek/include/			\
	-I$(srctree)/drivers/misc/mediatek/include/leakage_table_v2	\
	-I$(srctree)/drivers/misc/mediatek/include/upower		\
	-I$(srctree)/drivers/misc/mediatek/eem_v2/  			\
	-I$(srctree)/drivers/misc/mediatek/cpufreq_v2/		\
	-I$(srctree)/drivers/misc/mediatek/eem_v2/inc


ifneq ($(filter m y, $(CONFIG_MTK_TINYSYS_SSPM_SUPPORT)),)
ifneq ($(strip $(CONFIG_MTK_TINYSYS_SSPM_V0)),)
ccflags-y += -I$(srctree)/drivers/misc/mediatek/sspm/v0/
else ifneq ($(strip $(CONFIG_MTK_TINYSYS_SSPM_V1)),)
ccflags-y += -I$(srctree)/drivers/misc/mediatek/sspm/v1/
ccflags-y += -I$(srctree)/drivers/devfreq
endif
endif

obj-$(CONFIG_MTK_UNIFIED_POWER) += Upower.o
Upower-y += mtk_unified_power.o
Upower-y += mtk_unified_power_internal.o
Upower-y += mtk_unified_power_api.o

ifneq ($(strip $(CONFIG_MTK_PLAT_POWER_MT6768)),)
Upower-y += mtk_unified_power_plat.o
else ifneq ($(strip $(CONFIG_MTK_PLAT_POWER_MT6765)),)
ccflags-y += -I$(srctree)/drivers/misc/mediatek/cpufreq_v2/src/common_v2/
Upower-y += mtk_unified_power_plat_6765.o
else ifneq ($(strip $(CONFIG_MTK_PLAT_POWER_MT6833)),)
ccflags-y += -I$(srctree)/drivers/misc/mediatek/cpufreq_v2/src/common_v3/
Upower-y += mtk_unified_power_plat_6833.o
else ifneq ($(strip $(CONFIG_MTK_PLAT_POWER_MT6761)),)
Upower-y += mtk_unified_power_plat_6761.o
else ifneq ($(strip $(CONFIG_MTK_PLAT_POWER_MT6739)),)
Upower-y += mtk_unified_power_plat_6739.o
else ifneq ($(strip $(CONFIG_MTK_PLAT_POWER_6893)),)
Upower-y += mtk_unified_power_plat_6893.o
endif




