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
	cust_msdc.c \
	msdc_io.c \
	i2c.c \
	keypad.c \
	platform.c \
	pll.c \
	device_apc.c \
	gpio.c \
	pmic_wrap_init.c \
	tia.c \
	pmif.c \
	pmif_clkmgr.c \
	pmic.c \
	pmic_efuse.c \
	pmic_initial_setting.c \
	mtk_regulator_core.c \
	mt6358.c \
	mtk_dcm.c \
	mtk_dcm_busdvt.c \
	rtc.c \
	charging_bat.c \
	battery.c \
	memory.c \
	sync_write.c \
	spm_mtcmos.c \
	hotplug.s \
	delsel.c \
	mtk_drm.c \
	debug_latch.c \
	mtk_eem.c \
	mtk_ptp3_adcc.c \
	mtk_ptp3_brisket2.c \
	mtk_vcore_opp.c \
	ufs_aio_platform.c \
	plat_debug.c \
	mtk_subpmic.c

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

ifeq ("$(MT6375_PMU)", "yes")
	MOD_SRC	+= mt6375.c
endif

ifeq ("$(SGM41513_CHG)", "yes")
	MOD_SRC	+= sgm41513_chg.c
endif

ifeq ("$(MT6375_AUXADC)", "yes")
	MOD_SRC += mt6375_auxadc.c
endif

ifeq ("$(SGM41573_CHG)", "yes")
	MOD_SRC	+= sgm41573_chg.c
endif

ifeq ("$(UPM6910_CHG)", "yes")
	MOD_SRC	+= upm6910_pl.c
endif

ifeq ("$(ETA6947_CHG)", "yes")
	MOD_SRC	+= eta6947.c
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
	MOD_SRC += LP4_dram_init.c
	MOD_SRC += LP5_dram_init.c
	MOD_SRC += dramc_actiming.c
	MOD_SRC += dramc_efuse.c
	MOD_SRC += dramc_debug.c
	MOD_SRC += dramc_dvfs.c
	MOD_SRC += dramc_lowpower.c
	MOD_SRC += dramc_slt.c
	MOD_SRC += dramc_tracking.c
	MOD_SRC += dramc_unit_test.c
	MOD_SRC += dramc_utility.c
	MOD_SRC += DRAM_config_collection.c
	MOD_SRC += HW_FUNC_MANAGE.c
	MOD_SRC += MD32_initial.c
	MOD_SRC += RX_path_auto_gen.c
	MOD_SRC += TX_RX_auto_gen_library.c
	MOD_SRC += TX_path_auto_gen.c
	MOD_SRC += dramc_top.c
	MOD_SRC += dramc_pinmux.c
	MOD_SRC += dramc_dv_dut.c
	MOD_SRC += dramc_dv_main.c
	MOD_SRC += dramc_dv_freq_related.c

endif

ifeq ("$(CFG_GZ_SUPPORT)", "1")
ifeq ("$(CFG_GZ_REMAP)", "1")
	MOD_SRC	+= gz_remap_mt.c
endif
endif

ifeq ("$(CFG_ARMPLL_BOOST)", "1")
	MOD_SRC	+= mtk_perf_setting.c
endif
ifeq ($(DA_USE_PL_HW_DRV),yes)
	MOD_SRC += usb/da/dl_usb.c
	MOD_SRC += usb/da/dl_usb_debug_tool.c
	MOD_SRC += usb/da/dl_usb_drv.c
	MOD_SRC += usb/da/dl_usb_mode.c
	MOD_SRC += usb/da/dl_usb_resource.c
	MOD_SRC += usb/da/dl_usbacm_adap.c
	MOD_SRC += usb/da/dl_usbacm_drv.c
	MOD_SRC += usb/da/usbphy.c
else
	MOD_SRC += usb/pl/usbd.c
	MOD_SRC += usb/pl/usbtty.c
	ifeq ("$(A60931_SUPPORT)","yes")
		MOD_SRC  += usb/pl/usbphy-a60931.c
	else
		MOD_SRC  += usb/pl/usbphy.c
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
