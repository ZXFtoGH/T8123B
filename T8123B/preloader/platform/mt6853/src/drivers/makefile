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
	pmic.c \
	pmic_efuse.c \
	pmic_initial_setting.c \
	mtk_regulator_core.c \
	mt6359.c \
	mt6691-i2c.c \
	mt6691-regulator.c \
	charging_bat.c \
	battery.c \
	memory.c \
	memory_test.s \
	sync_write.c \
	spm_mtcmos.c \
	hotplug.s \
	mtk_drm.c \
	lastpc.c \
	debug_latch.c \
	mtk_eem.c \
	mtk_ses.c \
	mtk_vcore_opp.c \
	ufs_aio_platform.c \
	mtk_subpmic.c \
	plat_debug.c \
	connsys.c

ifneq ($(CFG_SYSTIMER),1)
	MOD_SRC  += timer.c
endif

ifeq ("$(A60931_SUPPORT)","yes")
	MOD_SRC  += usbphy-a60931.c
else
	MOD_SRC  += usbphy.c
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

ifeq ("$(MTK_MT6362_SUPPORT)", "yes")
	MOD_SRC	+= mt6362.c
endif

ifeq ("$(MTK_5G_B_MT6360_MT6315)","yes")
	MOD_SRC	+= mt6315-spmi.c
	MOD_SRC	+= mt6315-regulator.c
	MOD_SRC += mt6360.c
	MOD_SRC += mt6360_pmic.c
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
	MOD_SRC += dramc_top.c
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
	MOD_SRC += dramc_debug.c
	MOD_SRC += dramc_dv_freq_related.c
	MOD_SRC += dramc_dvfs.c
	MOD_SRC += dramc_lowpower.c
	MOD_SRC += dramc_slt.c
	MOD_SRC += dramc_tracking.c
	MOD_SRC += dramc_unit_test.c
	MOD_SRC += dramc_utility.c
	MOD_SRC += low_power_test.c
endif

ifeq ("$(CFG_GZ_SUPPORT)", "1")
ifeq ("$(CFG_GZ_REMAP)", "1")
	MOD_SRC	+= gz_remap_mt.c
endif
endif

###################################################################
# target
###################################################################

include $(COMMON_FILE_MK)

###################################################################
# trigger emigen
###################################################################
$(MOD_OBJ)/emi.o: $(EMIGEN_H_OUT)/custom_emi.h
