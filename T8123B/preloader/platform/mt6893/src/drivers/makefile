###################################################################
# flag
###################################################################

MOD_SRC  =
MOD_OBJ  = $(PRELOADER_OUT)/obj

###################################################################
# source files
# - add your file here !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
###################################################################

MOD_SRC  := \
	circbuf.c \
	clkbuf_ctl.c \
	srclken_rc.c \
	rtc.c \
	cust_msdc.c \
	msdc_io.c \
	i2c.c \
	keypad.c \
	usbd.c \
	usbphy.c \
	usbtty.c \
	platform.c \
	pll.c \
	device_apc.c \
	gpio.c \
	pmic_wrap_init.c \
	tia.c \
	pmif.c \
	pmif_clkmgr.c \
	spmi_common.c \
	spmi.c \
	spmi_dbg.c \
	pmic_efuse.c \
	pmic_initial_setting.c \
	mtk_regulator_core.c \
	mt6315-spmi.c \
	mt6315-regulator.c \
	mt6359.c \
	mt6691-i2c.c \
	mt6691-regulator.c \
	hl7593-regulator.c \
	charging_bat.c \
	memory.c \
	memory_test.s \
	sync_write.c \
	spm_mtcmos.c \
	hotplug.s \
	delsel.c \
	mtk_drm.c \
	debug_latch.c \
	mtk_eem.c \
	mtk_vcore_opp.c \
	ufs_aio_platform.c \
	mtk_subpmic.c \
	mtk_ptp3_brisket2.c \
	plat_debug.c \
	mtk_ptp3_adcc_spmc.c \
	mtk_ptp3_adcc_64bit.s \
	mtk_ptp3_adcc.c \
	gals_bist_test_rel.c

ifneq ($(CFG_SYSTIMER),1)
	MOD_SRC  += timer.c
endif

ifeq ("$(CFG_BOOT_DEV)", "BOOTDEV_NAND")
	MOD_SRC  += nand.c
	MOD_SRC  += bmt.c
endif

ifeq ("$(CFG_APWDT)","PLATFORM")
	MOD_SRC  += wdt.c
endif

ifeq ("$(PL_MODE)","DUMMY_AP")
	MOD_SRC  += gpio_init.c
endif

ifeq ("$(PL_MODE)","TINY")
	MOD_SRC  += gpio_init.c
endif

ifeq ("$(PL_MODE)","SLT")
	MOD_SRC  += gpio_init.c
endif

ifeq ("$(MT6370_PMU)", "yes")
	MOD_SRC	+= mt6370.c
endif

ifeq ("$(MT6375_PMU)", "yes")
	MOD_SRC	+= mt6375.c
endif

ifeq ("$(MT6375_AUXADC)", "yes")
	MOD_SRC += pmic_auxadc.c
	MOD_SRC += mt6375_auxadc.c
	MOD_SRC += pmic_mt6375_swrgo.c
else
	MOD_SRC += pmic.c
endif

ifeq ("$(MT6375_BATTERY)", "yes")
	MOD_SRC += mt6375_battery.c
else
	MOD_SRC += battery.c
endif

ifeq ("$(MT6360_PMU)","yes")
	MOD_SRC += mt6360.c
endif

ifeq ("$(MT6360_PMIC)","yes")
	MOD_SRC += mt6360_pmic.c
endif

ifeq ("$(MT6360_LDO)","yes")
	MOD_SRC += mt6360_ldo.c
endif

ifeq ("$(CFG_CV_SUPPORT)", "yes")
	MOD_SRC	+= cv_emi.c
	MOD_SRC	+= cv_dramc_pi_main.c
	MOD_SRC	+= cv_dramc_pi_ddr_reserve.c
	MOD_SRC	+= cv_dramc_pi_basic_api.c
	MOD_SRC	+= cv_dramc_pi_calibration_api.c
	MOD_SRC	+= cv_Hal_io.c
	MOD_SRC += cv_rtc.c
else
	MOD_SRC	+= emi.c
	MOD_SRC	+= dramc_pi_main.c
	MOD_SRC	+= dramc_pi_ddr_reserve.c
	MOD_SRC	+= dramc_pi_basic_api.c
	MOD_SRC	+= dramc_pi_calibration_api.c
	MOD_SRC	+= Hal_io.c
	MOD_SRC += rtc.c
	MOD_SRC += ANA_init_config.c
	MOD_SRC += DIG_NONSHUF_config.c
	MOD_SRC += DIG_SHUF_config.c
	MOD_SRC += DRAMC_SUBSYS_config.c
	MOD_SRC += DRAM_config_collctioin.c
	MOD_SRC += LP4_dram_init.c
	MOD_SRC += LP5_dram_init.c
	MOD_SRC += dramc_actiming.c
	MOD_SRC += dramc_efuse.c
	MOD_SRC += dramc_debug.c
	MOD_SRC += dramc_dv_freq_related.c
	MOD_SRC += dramc_dvfs.c
	MOD_SRC += dramc_lowpower.c
	MOD_SRC += dramc_slt.c
	MOD_SRC += dramc_tracking.c
	MOD_SRC += dramc_unit_test.c
	MOD_SRC += dramc_utility.c
ifeq ("$(PL_MODE)","SLT")
	MOD_SRC += low_power_test.c
endif
endif

ifeq ("$(CFG_GZ_SUPPORT)", "1")
ifeq ("$(CFG_GZ_REMAP)", "1")
	MOD_SRC	+= gz_remap_mt.c
endif
endif

ifeq ("$(CFG_ARMPLL_BOOST)", "1")
	MOD_SRC	+= mtk_perf_setting.c
endif
###################################################################
# target
###################################################################

include $(COMMON_FILE_MK)

###################################################################
# trigger emigen
###################################################################
$(MOD_OBJ)/emi.o: $(EMIGEN_H_OUT)/custom_emi.h
