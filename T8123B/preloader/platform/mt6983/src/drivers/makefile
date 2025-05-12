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
	src_coordinator.c \
	cust_msdc.c \
	msdc_io.c \
	i2c.c \
	keypad.c \
	platform.c \
	pll.c \
	device_apc.c \
	gpio.c \
	gpu.c \
	tia.c \
	pmif.c \
	pmif_clkmgr.c \
	spmi_common.c \
	spmi.c \
	spmi_dbg.c \
	pmic.c \
	pmic_efuse.c \
	pmic_initial_setting.c \
	pmic_auxadc.c \
	mt6363_auxadc.c \
	mt6363-regulator.c \
	mt6373_pmic.c \
	mt6373_efuse.c \
	mt6373_initial_setting.c \
	mt6373_auxadc.c \
	mt6373-regulator.c \
	mtk_dcm.c \
	mtk_pcie.c \
	mtk_dcm_busdvt.c \
	mtk_regulator_core.c \
	mt6319-spmi.c \
	mt6319-regulator.c \
	mt6685_adc_init.c \
	mt6685_rtc.c \
	charging_bat.c \
	memory.c \
	sync_write.c \
	spm_mtcmos.c \
	hotplug.s \
	cpu/delsel.c \
	cpu/dcc.c \
	mtk_drm.c \
	debug_latch.c \
	mtk_eem.c \
	mtk_vcore_opp.c \
	ufs_aio_platform.c \
	mtk_subpmic.c \
	plat_debug.c \
	mtk_ptp3_adcc.c \
	mtk_ptp3_cpu_pwr.c \
	mtk_adcc_main.c \
	mtk_adcc_env_asm.S


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
	MOD_SRC += mt6375_auxadc.c
endif

ifeq ("$(MT6375_BATTERY)", "yes")
	MOD_SRC += mt6375_battery.c
	MOD_SRC += mt6363_battery.c
else
	MOD_SRC += battery.c
endif

ifeq ("$(MT6360_PMU)","yes")
	#MOD_SRC += mt6360.c
endif

ifeq ("$(MT6360_PMIC)","yes")
	#MOD_SRC += mt6360_pmic.c
endif

ifeq ("$(MT6360_LDO)","yes")
	#MOD_SRC += mt6360_ldo.c
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
	MOD_SRC += mt6685_rtc.c
	MOD_SRC += ANA_init_config.c
	MOD_SRC += DIG_NONSHUF_config.c
	MOD_SRC += DIG_SHUF_config.c
	MOD_SRC += DRAMC_SUBSYS_config.c
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
	MOD_SRC += dramc_utility.c
	MOD_SRC += DRAM_config_collection.c
	MOD_SRC += ESL_DPHY_FPGA_RX_settings.c
	MOD_SRC += ESL_DPHY_FPGA_TX_settings.c
	MOD_SRC += FPGA_DIGPHY_settings.c
	MOD_SRC += HW_FUNC_MANAGE.c
	MOD_SRC += MD32_initial.c
	MOD_SRC += RX_path_auto_gen.c
	MOD_SRC += SRAM_init_lib.c
	MOD_SRC += TX_RX_auto_gen_library.c
	MOD_SRC += TX_path_auto_gen.c
	MOD_SRC += dramc_dv_dut.c
	MOD_SRC += dramc_dv_main.c
	MOD_SRC += dramc_top.c
	MOD_SRC += dvsim_dummy.c
	MOD_SRC += dramc_pinmux.c


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
ifeq ($(strip "$(MTK_COMBO_NAND_SUPPORT)"),"yes")
	MOD_SRC += nand/nandx.c
endif
else
	MOD_SRC += usb/pl/usbd.c
	MOD_SRC += usb/pl/usbtty.c

	ifeq ("$(A60931_SUPPORT)","yes")
		MOD_SRC += usb/pl/usbphy-a60931.c
	else
		MOD_SRC += usb/pl/usbphy.c
	endif
endif

ifeq ("$(CFG_AOC_HRE)", "1")
	MOD_SRC += aoc.c
	MOD_SRC += hre/API_HRE_INFRA_BUS.c
	MOD_SRC += hre/API_HRE_INFRA_DEVAPC.c
	MOD_SRC += hre/API_HRE_PERI_DEVAPC.c
	MOD_SRC += hre/API_HRE_EMI.c
	MOD_SRC += hre/API_HRE_SMPU.c
endif
###################################################################
# target
###################################################################

include $(COMMON_FILE_MK)

###################################################################
# trigger emigen
###################################################################
$(MOD_OBJ)/emi.o: $(EMIGEN_H_OUT)/custom_emi.h
