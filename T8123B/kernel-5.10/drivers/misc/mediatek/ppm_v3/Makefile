#
# Copyright (C) 2015 MediaTek Inc.
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
	-I$(srctree)/drivers/misc/mediatek/ppm_v3/inc			\
	-I$(srctree)/drivers/gpu/mediatek/gpufreq/include		\
	-I$(srctree)/drivers/misc/mediatek/power_throttling		\
	-I$(srctree)/drivers/misc/mediatek/include/			\
	-I$(srctree)/drivers/misc/mediatek/include/ppm_v3		\
	-I$(srctree)/drivers/misc/mediatek/include/upower		\
	-I$(srctree)/drivers/gpu/mediatek/gpufreq/			\
	-I$(srctree)/drivers/misc/mediatek/thermal/inc/			\
	-I$(srctree)/drivers/misc/mediatek/include/leakage_table_v2	\
	-I$(srctree)/drivers/misc/mediatek/cpufreq_v2/inc \
	-I$(srctree)/drivers/gpu/drm/mediatek/mediatek_v2 \
	-I$(srctree)/drivers/misc/mediatek/pbm

ifneq ($(CONFIG_FPGA_EARLY_PORTING),y)

obj-$(CONFIG_MTK_PPM_V3) += mtk_ppm_v3.o

mtk_ppm_v3-objs := mtk_ppm_main.o mtk_ppm_interface.o mtk_ppm_api.o mtk_ppm_profile.o mtk_ppm_cpi.o mtk_ppm_ipi.o mtk_ppm_policy_dlpt.o mtk_ppm_policy_sys_boost.o mtk_ppm_policy_ptpod.o mtk_ppm_policy_pwr_thro.o mtk_ppm_policy_thermal.o mtk_ppm_policy_lcm_off.o mtk_ppm_policy_hard_user_limit.o mtk_ppm_policy_user_limit.o mtk_ppm_policy_force_limit.o mtk_ppm_policy_ut.o

ifneq ($(strip $(CONFIG_MTK_PLAT_POWER_MT6768)),)
mtk_ppm_v3-objs += mtk_ppm_platform.o mtk_ppm_power_data.o mtk_ppm_cobra_algo.o
else ifneq ($(strip $(CONFIG_MTK_PLAT_POWER_MT6765)),)
mtk_ppm_v3-objs += mtk_ppm_platform_6765.o mtk_ppm_power_data.o mtk_ppm_cobra_algo_6765.o
else ifneq ($(strip $(CONFIG_MTK_PLAT_POWER_MT6833)),)
mtk_ppm_v3-objs += mtk_ppm_platform_6833.o mtk_ppm_cobra_algo_6833.o mtk_ppm_power_data_6833.o
else ifneq ($(strip $(CONFIG_MTK_PLAT_POWER_MT6761)),)
mtk_ppm_v3-objs += mtk_ppm_platform_6761.o mtk_ppm_cobra_algo_6761.o mtk_ppm_power_data_6761.o
else ifneq ($(strip $(CONFIG_MTK_PLAT_POWER_MT6739)),)
mtk_ppm_v3-objs += mtk_ppm_platform_6739.o mtk_ppm_cobra_algo_6739.o mtk_ppm_power_data_6739.o
else ifneq ($(strip $(CONFIG_MTK_PLAT_POWER_6893)),)
mtk_ppm_v3-objs += mtk_ppm_platform_6893.o mtk_ppm_cobra_algo_6893.o mtk_ppm_power_data_6893.o
endif

else
# dummy driver to avoid build error
obj-m := mtk_ppm_dummy.o
endif
