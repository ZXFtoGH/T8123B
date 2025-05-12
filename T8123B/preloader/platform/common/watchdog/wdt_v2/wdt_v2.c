/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein is
 * confidential and proprietary to MediaTek Inc. and/or its licensors. Without
 * the prior written permission of MediaTek inc. and/or its licensors, any
 * reproduction, modification, use or disclosure of MediaTek Software, and
 * information contained herein, in whole or in part, shall be strictly
 * prohibited.
 *
 * MediaTek Inc. (C) 2010. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER
 * ON AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL
 * WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR
 * NONINFRINGEMENT. NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH
 * RESPECT TO THE SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY,
 * INCORPORATED IN, OR SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES
 * TO LOOK ONLY TO SUCH THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO.
 * RECEIVER EXPRESSLY ACKNOWLEDGES THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO
 * OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES CONTAINED IN MEDIATEK
 * SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE
 * RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S
 * ENTIRE AND CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE
 * RELEASED HEREUNDER WILL BE, AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE
 * MEDIATEK SOFTWARE AT ISSUE, OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE
 * CHARGE PAID BY RECEIVER TO MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 * The following software/firmware and/or related documentation ("MediaTek
 * Software") have been modified by MediaTek Inc. All revisions are subject to
 * any receiver's applicable license agreements with MediaTek Inc.
 */

/* Use PMIC common API for PMIC full reset detection */
#define CFG_APWDT_PMIC_FULL_RST_COMMON_API  1

/* Timestamp for WDT */
#define CFG_APWDT_DBG_TIMESTAMP             0

#include "typedefs.h"
#include "platform.h"
#include "wdt.h"
#include "dramc_common.h"
#include "dramc_register.h"
#include "dramc_pi_api.h"
#include "emi_hw.h"
#include "pmic.h"
#include "pal_log.h"
#include "dconfig_env.h"
#if CFG_DRM_ENABLE
#include "mtk_drm.h"
#endif

#if !CFG_APWDT_PMIC_FULL_RST_COMMON_API
#include "upmu_hw.h"
#endif

#define RGUTAG                               "[RGU] "
#define RGULOG(fmt, arg...)                  pal_log_info(RGUTAG fmt, ##arg)
#define RGULOG_NO_MODULE_NAME(fmt, arg...)   pal_log_info(fmt, ##arg)
#define WDT_TIMEOUT_DEFAULT_VAL              (10)
#define WDT_DUAL_TIMEOUT_DEFAULT_VAL         (2)
#define MTK_WDT_NONRST2_BOOT_BOOTLOADER_DA_STAGE	4

unsigned int g_rgu_status = RE_BOOT_REASON_UNKNOW;

#if defined(CFG_APWDT) && (CFG_APWDT == V2)

static unsigned int timeout;
static unsigned int rgu_mode;
static int g_rgu_inited = 0;
static bool g_rgu_v2_dfd = false;
static char *g_doe_apwdt_en = NULL;
#if MTK_DOE_CONFIG_ENV_SUPPORT
static char *g_doe_pmic_rst = NULL;
#endif

#if CONFIG_WDT_DBG_TIMESTAMP
#include "timer.h"
static ulong time;
#endif

void __attribute__ ((weak)) pwrap_disable(void)
{
	RGULOG("empty pwrap_disable\n");
}

void __attribute__((weak)) pmic_dbg_status(unsigned char option)
{
	RGULOG("pmic do nothing\n");
}

int __attribute__((weak)) drm_latch_en(int enable)
{
	RGULOG("empty drm_latch_en\n");
	return 0;
}

static void mtk_wdt_mark_stage(unsigned int stage);

static void mtk_wdt_clear_all(void)
{
	u32 tmp;
	WRITE_REG(MTK_WDT_IRQ_CLR_MASK | MTK_WDT_IRQ_CLR_KEY, MTK_WDT_IRQ_CLR);
	udelay(40);

	tmp = DRV_Reg32(MTK_WDT_MODE);
	/* Workaround for clear interrupt design change issue
	 * that all interrupt triggered by RGU, including sspm
	 * spm, etc., would not be cleared. This is just fix
	 * AP side irq issue when level_en is set
	 */
	if (tmp & MTK_WDT_MODE_LVL_EN) {
		tmp &= ~MTK_WDT_MODE_LVL_EN;
		WRITE_REG(tmp | MTK_WDT_MODE_KEY, MTK_WDT_MODE);
		udelay(70);
		tmp = DRV_Reg32(MTK_WDT_MODE);
		tmp |= MTK_WDT_MODE_LVL_EN;
		WRITE_REG(tmp | MTK_WDT_MODE_KEY, MTK_WDT_MODE);
	} else {
		tmp |= MTK_WDT_MODE_LVL_EN;
		WRITE_REG(tmp | MTK_WDT_MODE_KEY, MTK_WDT_MODE);
		udelay(70);
		tmp = DRV_Reg32(MTK_WDT_MODE);
		tmp &= ~MTK_WDT_MODE_LVL_EN;
		WRITE_REG(tmp | MTK_WDT_MODE_KEY, MTK_WDT_MODE);
	}
}

BOOL mtk_is_rst_from_da(void)
{
	unsigned int reg;
	unsigned int last_stage;

	reg = DRV_Reg32(MTK_WDT_NONRST_REG2);

	last_stage = (reg >> MTK_WDT_NONRST2_LAST_STAGE_OFS) & RGU_STAGE_MASK;

	if (last_stage == RGU_STAGE_DA)
		return TRUE;

	return FALSE;
}

BOOL mtk_is_enter_fastboot(void)
{
	return (DRV_Reg32(MTK_WDT_NONRST_REG2) & MTK_WDT_NONRST2_BOOT_MASK) ==
		MTK_WDT_NONRST2_BOOT_BOOTLOADER;
}

void mtk_enter_fastboot(void)
{
	unsigned int reg;

	reg = DRV_Reg32(MTK_WDT_NONRST_REG2) & ~MTK_WDT_NONRST2_BOOT_MASK;
	reg |= MTK_WDT_NONRST2_BOOT_BOOTLOADER;
	WRITE_REG(reg, MTK_WDT_NONRST_REG2);
}

void mtk_mark_da_stage(void)
{
	mtk_wdt_set_aee_rsv(0);
	WRITE_REG(0, MTK_WDT_NONRST_REG);
	mtk_wdt_clear_all();
	mtk_wdt_mark_stage(RGU_STAGE_DA);
}

static void mtk_wdt_check_last_stage(void)
{
	unsigned int reg;
	unsigned int last_stage;

	/* check reboot source */
	reg = DRV_Reg32(MTK_WDT_NONRST_REG2);

	RGULOG("rst from: ");

	last_stage = (reg >> MTK_WDT_NONRST2_STAGE_OFS) & RGU_STAGE_MASK;

	if (last_stage == RGU_STAGE_PRELOADER)
		RGULOG_NO_MODULE_NAME("pl\n");
	else if (last_stage == RGU_STAGE_LK)
		RGULOG_NO_MODULE_NAME("lk\n");
	else if (last_stage == RGU_STAGE_KERNEL)
		RGULOG_NO_MODULE_NAME("kernel\n");
	else if (last_stage == RGU_STAGE_DA)
		RGULOG_NO_MODULE_NAME("DA\n");
	else if (last_stage == RGU_STAGE_BL2_EXT)
		RGULOG_NO_MODULE_NAME("BL2EXT\n");
	else if (last_stage == RGU_STAGE_AEE)
		RGULOG_NO_MODULE_NAME("AEE\n");
	else if (last_stage == RGU_STAGE_TFA)
		RGULOG_NO_MODULE_NAME("TFA\n");
	else
		RGULOG_NO_MODULE_NAME("?\n");

	reg = (reg & ~(RGU_STAGE_MASK << MTK_WDT_NONRST2_LAST_STAGE_OFS)) |
		(last_stage << MTK_WDT_NONRST2_LAST_STAGE_OFS);

	DRV_WriteReg32(MTK_WDT_NONRST_REG2, reg);
}

static void mtk_wdt_mark_stage(unsigned int stage)
{
	unsigned int reg = DRV_Reg32(MTK_WDT_NONRST_REG2);

	reg = (reg & ~(RGU_STAGE_MASK << MTK_WDT_NONRST2_STAGE_OFS))
		| (stage << MTK_WDT_NONRST2_STAGE_OFS);

	DRV_WriteReg32(MTK_WDT_NONRST_REG2, reg);
}

static void mtk_wdt_reset_deglitch_enable(void)
{
	DRV_WriteReg32(MTK_WDT_RSTDEG_EN1, MTK_WDT_RSTDEG_EN1_KEY);
	DRV_WriteReg32(MTK_WDT_RSTDEG_EN2, MTK_WDT_RSTDEG_EN2_KEY |
		(MTK_WDT_RSTDEG_CLOCK_32K << MTK_WDT_RSTDEG_CLOCK_SELETC_SHIFT) |
		(MTK_WDT_RSTDEG_LATENCY_230NS_183US << MTK_WDT_RSTDEG_LATENCY_SHIFT));
	RGULOG("%s: MTK_WDT_RSTDEG_EN1(%x), MTK_WDT_RSTDEG_EN2(%x)\n", __func__,
		READ_REG(MTK_WDT_RSTDEG_EN1), READ_REG(MTK_WDT_RSTDEG_EN2));
}

void mtk_wdt_disable(void)
{
	u32 tmp;

	mtk_wdt_clear_all();
	tmp = DRV_Reg32(MTK_WDT_MODE);
	tmp &= ~MTK_WDT_MODE_ENABLE;		/* disable watchdog */
	tmp |= (MTK_WDT_MODE_KEY);		 /* need key then write is allowed */
	DRV_WriteReg32(MTK_WDT_MODE, tmp);
}

void mtk_wdt_enable(void)
{
	u32 tmp;

	mtk_wdt_clear_all();
	tmp = DRV_Reg32(MTK_WDT_MODE);
	tmp |= MTK_WDT_MODE_ENABLE;		/* enable watchdog */
	tmp |= (MTK_WDT_MODE_KEY);		 /* need key then write is allowed */
	DRV_WriteReg32(MTK_WDT_MODE, tmp);
}

static bool mtk_wdt_is_v2_dfd(void)
{
	DRV_WriteReg32(MTK_WDT_LATCH_CTL2, MTK_LATCH_CTL2_KEY);
	if(READ_REG(MTK_WDT_LATCH_CTL2)) {
		g_rgu_v2_dfd = true;
		return g_rgu_v2_dfd;
	}
	return false;
}

static void mtk_wdt_restore_dfd_setting(void)
{
	unsigned int wdt_ctrl;

	/* mtk_wdt_is_v2_dfd() will change MTK_WDT_LATCH_CTL2, backup here */
	wdt_ctrl = READ_REG(MTK_WDT_LATCH_CTL2);
	/* Disable DFD_EN in da/pl to prevent long waiting time before reset */
	if(mtk_wdt_is_v2_dfd()) {
		DRV_WriteReg32(MTK_WDT_LATCH_CTL2, MTK_LATCH_CTL2_KEY2);
		if (drm_latch_en(0) < 0)
			RGULOG("Disable DRM latch failed\n");
	} else {
		wdt_ctrl = READ_REG(MTK_WDT_LATCH_CTL2);
		wdt_ctrl |= MTK_LATCH_CTL2_KEY;
		wdt_ctrl &= ~(MTK_RG_MCU_DFD_EN);
		DRV_WriteReg32(MTK_WDT_LATCH_CTL2, wdt_ctrl);
	}
}

static void mtk_wdt_reset(char mode)
{
	/* Watchdog Rest */
	unsigned int wdt_mode_val;
	unsigned int non_rst2 = DRV_Reg32(MTK_WDT_NONRST_REG2);

	mtk_wdt_clear_all();

	DRV_WriteReg32(MTK_WDT_RESTART, MTK_WDT_RESTART_KEY);
	/*set latch register to 0 before sw reset*/
	/* DRV_WriteReg32(MTK_WDT_LATCH_CTL, (MTK_WDT_LENGTH_CTL_KEY | 0x0)); */

	mtk_wdt_restore_dfd_setting();

	wdt_mode_val = DRV_Reg32(MTK_WDT_MODE);
	/* clear autorestart bit: autoretart: 1, bypass power key, 0: not bypass power key */
#if CFG_USB_AUTO_DETECT
	non_rst2 &= (~MTK_WDT_NONRST2_BYPASS_PWR_KEY)
#endif
	/* make sure WDT mode is hw reboot mode, can not config isr mode */
	wdt_mode_val &= (~(MTK_WDT_MODE_IRQ | MTK_WDT_MODE_DUAL_MODE | MTK_WDT_MODE_EXTRA_CNT));

	pmic_dbg_status(0);
	if (mode) { /* mode != 0 means by pass power key reboot, We using auto_restart bit as by pass power key flag */
		wdt_mode_val = wdt_mode_val | (MTK_WDT_MODE_KEY|MTK_WDT_MODE_EXTEN);
		DRV_WriteReg32(MTK_WDT_MODE, wdt_mode_val);
		/*
		 * MTK_WDT_MODE_AUTO_RESTART is replaced by MTK_WDT_NONRST2_BYPASS_PWR_KEY
		 * of NONRST_REG2 for common kernel projects and two stage timeout design
		 */
		DRV_WriteReg32(MTK_WDT_NONRST_REG2, non_rst2 | MTK_WDT_NONRST2_BYPASS_PWR_KEY);
	} else{
		RGULOG("disable pwrap before WDT\n");
		pwrap_disable();
		wdt_mode_val = wdt_mode_val | (MTK_WDT_MODE_KEY|MTK_WDT_MODE_EXTEN);
		DRV_WriteReg32(MTK_WDT_MODE, wdt_mode_val);
		/*
		 * MTK_WDT_MODE_AUTO_RESTART is replaced by MTK_WDT_NONRST2_BYPASS_PWR_KEY
		 * of NONRST_REG2 for common kernel projects and two stage timeout design
		 */
		DRV_WriteReg32(MTK_WDT_NONRST_REG2, non_rst2);
	}

	RGULOG("mtk_wdt_reset WDT MODE=%x\n", DRV_Reg32(MTK_WDT_MODE));
	gpt_busy_wait_us(100);
	DRV_WriteReg32(MTK_WDT_SWRST, MTK_WDT_SWRST_KEY);
}

static unsigned int mtk_wdt_get_status(void)
{
	static unsigned int wdt_sta = 0;
	static unsigned int wdt_sta_handled = 0;
	unsigned int reg;

	/*
	 * Note:
	 * Because WDT_STA register will be cleared after writing WDT_MODE,
	 * we use a static variable to keep orinigal WDT_STA.
	 *
	 * After reset, static varialbe will always be clear to 0,
	 * so only read WDT_STA when static variable is 0 is OK
	 *
	 * Use "wdt_sta_handled" to indicate if WDT_STATUS is preserved.
	 * Do not use "wdt_sta" as indication because dummy handling will be
	 * executed in case WDT_STATUS is 0 originally.
	 */
	if (wdt_sta_handled == 0) {

		wdt_sta = DRV_Reg32(MTK_WDT_STATUS);
		RGULOG("STA from reg:       0x%x\n", DRV_Reg32(MTK_WDT_STATUS));

		wdt_sta |= DRV_Reg32(MTK_WDT_NONRST_REG);

		wdt_sta_handled = 1;
	}

#if (CFG_APWDT_REQ_BYPASS)
	if(wdt_sta & CFG_APWDT_REQ_BYPASS) {
		wdt_sta &= ~CFG_APWDT_REQ_BYPASS;
		RGULOG("========bypass CFG_APWDT_REQ_BYPASS========\n");
	}
#endif

	return wdt_sta;
}

static unsigned int mtk_wdt_get_debug_ctl(void)
{
	static unsigned int wdt_debug_ctl = 0;

	/*
	 * Note:
	 * Because some bits in register DEBUG_CTL will be cleared after
	 * writing WDT_MODE, we use a static variable to keep original DEBUG_CTL.
	 *
	 * Effected Bits:
	 * 16: ddr_reserve_success
	 * 18: emi_dcs_success
	 * 20: dvfsrc_success
	 */
	if (wdt_debug_ctl == 0)
		wdt_debug_ctl = DRV_Reg32(MTK_WDT_DEBUG_CTL);

	return wdt_debug_ctl;
}

void mtk_wdt_keep_essential_info(void)
{
	mtk_wdt_get_status();
	mtk_wdt_get_debug_ctl();
}

int mtk_wdt_boot_check(void);

static void mtk_wdt_check_trig_reboot_reason(void)
{
	int ret;

	/* check bypass power key info */
	ret = mtk_wdt_boot_check();

	/* 3. By pass power key info */
	RGULOG("bypass pwrkey: ");

	if (ret == WDT_BY_PASS_PWK_REBOOT)
		RGULOG_NO_MODULE_NAME("set\n");
	else if (ret == WDT_NORMAL_REBOOT)
		RGULOG_NO_MODULE_NAME("NOT set\n");
	else
		RGULOG_NO_MODULE_NAME("wdt does not trigger rst\n");
}

static void mtk_wdt_mode_config(BOOL dual_mode_en,
					BOOL irq,
					BOOL ext_en,
					BOOL ext_pol,
					BOOL wdt_en)
{
	unsigned int tmp;
	unsigned int nonrst2 = 0;

	mtk_wdt_clear_all();

	/* RGULOG(" mtk_wdt_mode_config mode value=%x\n",DRV_Reg32(MTK_WDT_MODE)); */
	tmp = DRV_Reg32(MTK_WDT_MODE);
	tmp |= MTK_WDT_MODE_KEY | MTK_WDT_MODE_EXTRA_CNT;
#if (CFG_APWDT_LVL_IRQ)
	tmp |= MTK_WDT_MODE_LVL_EN;
#else
	tmp &= ~MTK_WDT_MODE_LVL_EN;
#endif
	/* Bit 0 : Whether enable watchdog or not */
	if (wdt_en == TRUE)
		tmp |= MTK_WDT_MODE_ENABLE;
	else
		tmp &= ~MTK_WDT_MODE_ENABLE;

	/* Bit 1 : Configure extern reset signal polarity. */
	if (ext_pol == TRUE)
		tmp |= MTK_WDT_MODE_EXT_POL;
	else
		tmp &= ~MTK_WDT_MODE_EXT_POL;

	/* Bit 2 : Whether enable external reset signal */
	if (ext_en == TRUE)
		tmp |= MTK_WDT_MODE_EXTEN;
	else
		tmp &= ~MTK_WDT_MODE_EXTEN;

	/* Bit 3 : Whether generating interrupt instead of reset signal */
	if (irq == TRUE)
		tmp |= MTK_WDT_MODE_IRQ;
	else
		tmp &= ~MTK_WDT_MODE_IRQ;

	/* Bit 6 : Whether enable debug module reset */
	if (dual_mode_en == TRUE)
		tmp |= MTK_WDT_MODE_DUAL_MODE;
	else
		tmp &= ~MTK_WDT_MODE_DUAL_MODE;

	DRV_WriteReg32(MTK_WDT_MODE, tmp);

	/*
	 * MTK_WDT_MODE_AUTO_RESTART is replaced by MTK_WDT_NONRST2_BYPASS_PWR_KEY
	 * of NONRST_REG2 for common kernel projects and two stage timeout design
	 */
	nonrst2 = DRV_Reg32(MTK_WDT_NONRST_REG2);
	DRV_WriteReg32(MTK_WDT_NONRST_REG2, nonrst2 | MTK_WDT_NONRST2_BYPASS_PWR_KEY);

	/* dual_mode(1); //always dual mode */
	/* mdelay(100); */
	RGULOG("mtk_wdt_mode_config mode value=%x, tmp:%x\n", DRV_Reg32(MTK_WDT_MODE), tmp);

}
/* EXPORT_SYMBOL(mtk_wdt_mode_config); */

static void mtk_wdt_set_time_out_value(UINT32 value)
{
	/*
	* TimeOut = BitField 15:5
	* Key	  = BitField  4:0 = 0x08
	*/

	/* sec * 32768 / 512 = sec * 64 = sec * 1 << 6 */
	timeout = (unsigned int)(value * (1 << 6));
	timeout = timeout << 5;
	DRV_WriteReg32(MTK_WDT_LENGTH, (timeout | MTK_WDT_LENGTH_KEY));
}

static void mtk_wdt_set_dual_time_out_value(UINT32 value)
{
	UINT32 dual_len_setting = 0;
	/*
	* TimeOut = BitField 15:5
	* Key	  = BitField  4:0 = 0x08
	*/

	/* sec * 32768 / 512 = sec * 64 = sec * 1 << 6 */
	timeout = (unsigned int)(value * (1 << 6));
	timeout &= MTK_WDT_DUAL_LENGTH_MASK;
	timeout = timeout << MTK_WDT_DUAL_LENGTH_SHIFT;

	if (!g_rgu_v2_dfd)
		dual_len_setting |=
			MTK_WDT_DUAL_LENGTH_EINT_EN | MTK_WDT_DUAL_LENGTH_SYSRST_EN;

	DRV_WriteReg32(MTK_WDT_DUAL_LENGTH,
			MTK_WDT_DUAL_LENGTH_KEY | timeout | dual_len_setting);
}

static void mtk_wdt_set_dual_time_out_en(UINT32 mark_bit)
{
	if (g_rgu_v2_dfd) {
		UINT32 tmp = DRV_Reg32(WDT_DUAL_LEN_EN);
		tmp |= (mark_bit & MTK_WDT_DUAL_LEN_MASK);
		DRV_WriteReg32(WDT_DUAL_LEN_EN, tmp | MTK_WDT_DUAL_LEN_EN_KEY);
		DRV_WriteReg32(WDT_DUAL_LEN_EN, tmp | MTK_WDT_DUAL_LEN_EN_KEY_2);          
	}
}

void mtk_wdt_timestamp_init(void)
{
#if CONFIG_WDT_DBG_TIMESTAMP
	RGULOG("init done! elapsed time: %d ms\n", get_timer(time));
	time = get_timer(0);
#endif
}

void mtk_wdt_timestamp_update(void)
{
#if CONFIG_WDT_DBG_TIMESTAMP
	ulong time_elapse;

	time_elapse = get_timer(time);

	if (time_elapse) {
		/*
		 * Since wdt kicking may be very frequency, set threshold to avoid
		 * lousy message.
		 *
		 * Preloader does not reset system while WDT timeout, thus debug message
		 * shall appear for timeout cases (long elapsed time).
		 */
		if (time_elapse > 5)
			RGULOG("kick wdt! elapsed time: %d ms\n", get_timer(time));

		time = get_timer(0);
	}
#endif
}

void mtk_wdt_restart(void)
{
	/* Reset WatchDogTimer's counting value to time out value */
	/* ie., keepalive() */

	DRV_WriteReg32(MTK_WDT_RESTART, MTK_WDT_RESTART_KEY);

	mtk_wdt_timestamp_update();
}

void mtk_wdt_sw_reset(void)
{
	RGULOG("WDT SW RESET\n");
	/* DRV_WriteReg32 (0x70025000, 0x2201); */
	/* DRV_WriteReg32 (0x70025008, 0x1971); */
	/* DRV_WriteReg32 (0x7002501C, 0x1209); */
	mtk_wdt_reset(1);/* NOTE here, this reset will cause by pass power key */

	/* system will reset */

	while (1)
	{
		RGULOG("SW reset happen ...\n");
	};
}

static void mtk_wdt_hw_reset(void)
{
	RGULOG("WDT_HW_Reset_test\n");

	/* 1. set WDT timeout 1 secs, 1*64*512/32768 = 1sec */
	mtk_wdt_set_time_out_value(1);

	/* 2. enable WDT debug reset enable, generating irq disable, ext reset disable */
	/* ext reset signal low, wdt enalbe */
	mtk_wdt_mode_config(TRUE, FALSE, TRUE, FALSE, TRUE);

	/* 3. reset the watch dog timer to the value set in WDT_LENGTH register */
	mtk_wdt_restart();

	/* 4. system will reset */
	while (1);
}

int mtk_wdt_boot_check(void)
{
	unsigned int wdt_sta = mtk_wdt_get_status();
	int ret = WDT_NOT_WDT_REBOOT;

	/*
	 * For DA download hope to timeout reboot, and boot to u-boot/kernel configurable reason,
	 * we set both timeout reboot and software reboot can check whether bypass power key.
	 */
	if (wdt_sta & (MTK_WDT_STATUS_HWWDT_RST | MTK_WDT_STATUS_SWWDT_RST |
		MTK_WDT_STATUS_SPM_THERMAL_RST | MTK_WDT_STATUS_SPMWDT_RST |
		MTK_WDT_STATUS_THERMAL_DIRECT_RST | MTK_WDT_STATUS_SECURITY_RST |
		MTK_WDT_STATUS_DEBUGWDT_RST | MTK_WDT_STATUS_EINT_RST |
		MTK_WDT_STATUS_SYSRST_RST | MTK_WDT_STATUS_DVFSP_RST | MTK_WDT_STATUS_MCUPM_RST |
		MTK_WDT_STATUS_SSPM_RST | MTK_WDT_STATUS_GPUEB_RST | MTK_WDT_STATUS_ERRFLG_RST |
		MTK_WDT_STATUS_MDDBG_RST | MTK_WDT_STATUS_UDISCN_RST | MTK_WDT_STATUS_DEBUG_RST |
		MTK_WDT_STATUS_HRE_RST)) {
		if (rgu_mode & MTK_WDT_NONRST2_BYPASS_PWR_KEY) {
			/* HW or SW reboot, and auto restart is set, means bypass power key */
			ret = WDT_BY_PASS_PWK_REBOOT;

		} else {

			/* HW or SW reboot, but auto restart is not set, means NOT bypass power key */
			ret = WDT_NORMAL_REBOOT;
		}
	} else {
		/*
		 * For PMIC full reset, return "bypass pwr key reboot" to help AEE works.
		 * I.e., Prevent entering charger mode.
		 */
		if (mtk_wdt_is_pmic_full_reset()) {
			RGULOG("PMIC full rst: true\n");
			ret = WDT_BY_PASS_PWK_REBOOT;
		}
	}

	return ret;
}

int mtk_wdt_is_pmic_full_reset(void)
{
#if CFG_APWDT_PMIC_FULL_RST_COMMON_API

	unsigned int val;

	/* Use PMIC common API for PMIC full reset detection */

	val = is_pmic_cold_reset();

#else

	static unsigned int val = 0;
	static unsigned int handled = 0;

	/*
	 * PMIC common API may be failed in some platforms.
	 *
	 * Use a non-volatile register (reset only after battery is removed)
	 * for PMIC full reset detection.
	 *
	 * In such platforms, use bit 0 in register PMIC_RG_RSV_SWREG.
	 */

	if (!handled) {

		pmic_read_interface(PMIC_RG_RSV_SWREG_ADDR, &val, 0x1, 0);

		if (val)
			pmic_config_interface(PMIC_RG_RSV_SWREG_ADDR, 0, 0x1, 0);

		RGULOG("PMIC full rst: %d\n", val);

		handled = 1;
	}

#endif

	return val ? 1 : 0;

}

void mtk_arch_reset(char mode)
{
	RGULOG("mtk_arch_reset at pre-loader!\n");

	/* mtk_wdt_hw_reset(); */
	mtk_wdt_reset(mode);

	while (1);
}

int rgu_dram_reserved(int enable)
{
	volatile unsigned int tmp = 0, ret = 0;

	/*
	 * DDR reserved mode may be switched on/off anytime by caller.
	 *
	 * DDR reserved mode switch on/off flow will modify register MODE.
	 * Since register STATUS and some bits in DEBUG_CTL will be reset
	 * if register MODE has any changes. Keep those register values first
	 * before register MODE is written later.
	 */
	mtk_wdt_keep_essential_info();

	if (enable == 1)
	{
		/* enable ddr reserved mode */
		tmp = READ_REG(MTK_WDT_MODE);
		tmp |= (MTK_WDT_MODE_DDR_RESERVE|MTK_WDT_MODE_KEY);
		WRITE_REG(tmp, MTK_WDT_MODE);

	} else if (enable == 0)
	{
		/* disable ddr reserved mode, set reset mode,
				disable watchdog output reset signal */
		tmp = READ_REG(MTK_WDT_MODE);
		tmp &= (~MTK_WDT_MODE_DDR_RESERVE);
		tmp |= MTK_WDT_MODE_KEY;
		WRITE_REG(tmp, MTK_WDT_MODE);
	} else
	{
		RGULOG("Wrong input %d, should be 1(enable) or 0(disable) in %s\n", enable, __func__);
		ret = -1;
	}
	RGULOG("%s: MTK_WDT_MODE(%x)\n", __func__, tmp);
	return ret;
}

int rgu_is_reserve_ddr_enabled(void)
{
	unsigned int wdt_mode;

	wdt_mode = READ_REG(MTK_WDT_MODE);

	if (wdt_mode & MTK_WDT_MODE_DDR_RESERVE)
		return 1;
	else
		return 0;
}

int rgu_is_dram_slf(void)
{
	unsigned int wdt_dbg_ctrl;

	wdt_dbg_ctrl = READ_REG(MTK_WDT_DEBUG_CTL);

	if (wdt_dbg_ctrl & MTK_DDR_SREF_STA) {
		RGULOG("DDR is in self-refresh. %x\n", wdt_dbg_ctrl);
		return 1;
	} else {
		RGULOG("DDR is not in self-refresh. %x\n", wdt_dbg_ctrl);
		return 0;
	}
}

int rgu_wait_for_reg_update_done(unsigned int reg_addr, unsigned mask, unsigned target_val)
{
	unsigned timeout_ms = 5;
	unsigned int read_val;
	unsigned int read_times = 0;
	unsigned int elapsed_ms = 0;

	while (1) {

		read_val = READ_REG(reg_addr);

		if (target_val == (read_val & mask))
			break;

		read_times++;

		if (read_times && 0 == (read_times % 100)) {

			mdelay(1);
			elapsed_ms += 1;

			if (elapsed_ms > timeout_ms) {
				RGULOG("%s: ERROR! Update reg 0x%x timeout! Mask: 0x%x Target: 0x%x\n",
				       __func__, reg_addr, mask, target_val);

				return -1;
			}
		}
	}

	return 0;
}

int rgu_update_reg(unsigned int reg, u32 func, u32 bits)
{
	volatile unsigned int val;
	int ret;

	val = READ_REG(reg);

	if (func == RGU_REG_SET)
		val |= bits;
	else if (func == RGU_REG_CLR)
		val &= (~bits);
	else
		return -1;

	/* add unlock key */
	if (reg == MTK_WDT_DEBUG_CTL)
		val |= MTK_DEBUG_CTL_KEY;
	else if (reg == MTK_WDT_DEBUG_CTL2)
		val |= MTK_DEBUG_CTL2_KEY;
	else {
		RGULOG("%s: Invalid! Set bit 0x%x in reg 0x%x fail!\n", __func__,
			bits, reg);
		return -1;
	}

	/* update register */
	WRITE_REG(val, reg);

	/* wait until register updating done */
	if (func == RGU_REG_SET)
		val = bits;
	else
		val = 0;

	/*
	 * Must ensure waiting-done API has timeout mechanism
	 * because in some scenarios with security feature toggled,
	 * below registers will be read-only. Writting operation
	 * will get timeout here.
	 *
	 * DEBUG_CTL / DEBUG_CLT2 / DDR_RESERVE_MODE bit in MODE
	 */
	ret = rgu_wait_for_reg_update_done(reg, bits, val);

	RGULOG("%s: %d, bits: 0x%x, addr: 0x%x, val: 0x%x\n", __func__,
		func, bits, reg, READ_REG(reg));

	return ret;
}

int rgu_is_reserve_ddr_mode_success(void)
{
	unsigned int wdt_dbg_ctrl;

	/*
	 * MTK_DDR_RESERVE_RTA bit will be reset by modifying register MODE.
	 * Read DEBUG_CTL value kept by mtk_wdt_get_debug_ctl().
	 */
	wdt_dbg_ctrl = mtk_wdt_get_debug_ctl();

	if (wdt_dbg_ctrl & MTK_DDR_RESERVE_RTA) {
		RGULOG("WDT DDR reserve mode success! %x\n", wdt_dbg_ctrl);
		return 1;
	} else {
		RGULOG("WDT DDR reserve mode FAIL! %x\n", wdt_dbg_ctrl);
		return 0;
	}
}

int rgu_cfg_emi_dcs_en(int enable)
{
	volatile unsigned int tmp;
	int ret;

	if (enable == 1) {

		ret = rgu_update_reg(MTK_WDT_DEBUG_CTL2,
			RGU_REG_SET, MTK_RGU_EMI_DCS_EN);

	} else if (enable == 0) {

		ret = rgu_update_reg(MTK_WDT_DEBUG_CTL2,
			RGU_REG_CLR, MTK_RGU_EMI_DCS_EN);

	} else
		ret = -1;

	return ret;
}

int rgu_release_rg_dramc_conf_iso(void)
{
	int ret;
	int success;

	ret = rgu_update_reg(MTK_WDT_DEBUG_CTL,
		RGU_REG_CLR, MTK_RG_CONF_ISO);

	success = rgu_is_reserve_ddr_mode_success();

	RGULOG("DDR RESERVE Success %d\n", success);

	return ret;
}

int rgu_release_rg_dramc_iso(void)
{
	return rgu_update_reg(MTK_WDT_DEBUG_CTL, RGU_REG_CLR, MTK_RG_DRAMC_ISO);
}

int rgu_release_rg_dramc_sref(void)
{
	return rgu_update_reg(MTK_WDT_DEBUG_CTL, RGU_REG_CLR, MTK_RG_DRAMC_SREF);
}

int rgu_is_emi_dcs_success(void)
{
	unsigned int wdt_dbg_ctrl;
	unsigned int success;

	/*
	 * MTK_RGU_EMI_DCS_SUCCESS bit will be reset by modifying register MODE.
	 * Read DEBUG_CTL value kept by mtk_wdt_get_debug_ctl().
	 */
	wdt_dbg_ctrl = mtk_wdt_get_debug_ctl();

	success = (wdt_dbg_ctrl & MTK_RGU_EMI_DCS_SUCCESS) >> MTK_RGU_EMI_DCS_SUCCESS_OFFSET;

	RGULOG("EMI_DCS_SUCCESS %d\n", success);

	return success;
}

int rgu_is_emi_dcs_enable(void)
{
	unsigned int reg;

	reg = READ_REG(MTK_WDT_DEBUG_CTL2);

	if (reg & MTK_RGU_EMI_DCS_EN)
		return 1;
	else
		return 0;
}

int rgu_is_dvfsrc_success(void)
{
	unsigned int wdt_dbg_ctrl;
	unsigned int success;

	/*
	 * MTK_RGU_EMI_DCS_SUCCESS bit will be reset by modifying register MODE.
	 * Read DEBUG_CTL value kept by mtk_wdt_get_debug_ctl().
	 */
	wdt_dbg_ctrl = mtk_wdt_get_debug_ctl();

	success = (wdt_dbg_ctrl & MTK_RGU_DVFSRC_SUCCESS) >> MTK_RGU_DVFSRC_SUCCESS_OFFSET;

	RGULOG("DVFSRC_SUCCESS %d\n", success);

	return success;
}

int rgu_is_dvfsrc_enable(void)
{
	unsigned int reg;

	reg = READ_REG(MTK_WDT_DEBUG_CTL2);

	if (reg & MTK_RGU_DVFSRC_EN)
		return 1;
	else
		return 0;
}

void rgu_swsys_reset(WD_SYS_RST_TYPE reset_type)
{
	if (reset_type == WD_MD_RST) {
		unsigned int wdt_dbg_ctrl;

		wdt_dbg_ctrl = READ_REG(MTK_WDT_SWSYSRST);
		wdt_dbg_ctrl |= MTK_WDT_SWSYS_RST_KEY;
		wdt_dbg_ctrl |= 0x80; /* 1<<7 */
		WRITE_REG(wdt_dbg_ctrl, MTK_WDT_SWSYSRST);
		udelay(1000);
		wdt_dbg_ctrl = READ_REG(MTK_WDT_SWSYSRST);
		wdt_dbg_ctrl |= MTK_WDT_SWSYS_RST_KEY;
		wdt_dbg_ctrl &= (~0x80); /* ~(1<<7) */
		WRITE_REG(wdt_dbg_ctrl, MTK_WDT_SWSYSRST);
		/* RGULOG("rgu pl md reset\n"); */
	}
}

int mtk_wdt_swsysret_config(int bit, int set_value)
{
	unsigned int wdt_sys_val;

	wdt_sys_val = READ_REG(MTK_WDT_SWSYSRST);
	wdt_sys_val |= MTK_WDT_SWSYS_RST_KEY;
	switch (bit) {
	case MTK_WDT_SWSYS_RST_MD_RST:
		if (set_value == 1)
			wdt_sys_val |= MTK_WDT_SWSYS_RST_MD_RST;
		if (set_value == 0)
			wdt_sys_val &= ~MTK_WDT_SWSYS_RST_MD_RST;
		break;
	case MTK_WDT_SWSYS_RST_MFG_RST: /* MFG reset */
		if (set_value == 1)
			wdt_sys_val |= MTK_WDT_SWSYS_RST_MFG_RST;
		if (set_value == 0)
			wdt_sys_val &= ~MTK_WDT_SWSYS_RST_MFG_RST;
		break;
	case MTK_WDT_SWSYS_RST_C2K_RST: /* c2k reset */
		if (set_value == 1)
			wdt_sys_val |= MTK_WDT_SWSYS_RST_C2K_RST;
		if (set_value == 0)
			wdt_sys_val &= ~MTK_WDT_SWSYS_RST_C2K_RST;
		break;
	case MTK_WDT_SWSYS_RST_CONN_MCU_RST:
		if (set_value == 1)
			wdt_sys_val |= MTK_WDT_SWSYS_RST_CONN_MCU_RST;
		if (set_value == 0)
			wdt_sys_val &= ~MTK_WDT_SWSYS_RST_CONN_MCU_RST;
		break;
	}
	WRITE_REG(wdt_sys_val, MTK_WDT_SWSYSRST);

	return 0;
}

int mtk_wdt_request_en_set(int mark_bit, WD_REQ_CTL en)
{
	unsigned int tmp, ext_req_con;

	if ((mark_bit != MTK_WDT_STATUS_SSPM_RST) &&
		(mark_bit != MTK_WDT_STATUS_SYSRST_RST) &&
		(mark_bit != MTK_WDT_STATUS_EINT_RST) &&
		(mark_bit != MTK_WDT_STATUS_SPM_THERMAL_RST))
		return -1;

	tmp = READ_REG(MTK_WDT_REQ_MODE);
	tmp |= MTK_WDT_REQ_MODE_KEY;

	if (en == WD_REQ_EN)
		tmp |= (mark_bit);
	if (en == WD_REQ_DIS)
		tmp &= ~(mark_bit);

	WRITE_REG(tmp, MTK_WDT_REQ_MODE);

#if MTK_DOE_CONFIG_ENV_SUPPORT
	/* disable all reset source's reset and irq mode */
	if (g_doe_apwdt_en == NULL)
		g_doe_apwdt_en = dconfig_getenv("apwdt_en");

	if (g_doe_apwdt_en != NULL && *g_doe_apwdt_en == '0')
	       WRITE_REG(MTK_WDT_REQ_MODE_KEY, MTK_WDT_REQ_MODE);
#endif

	return 0;
}

int mtk_wdt_request_mode_set(int mark_bit, WD_REQ_MODE mode)
{
	unsigned int tmp;

	if ((mark_bit != MTK_WDT_STATUS_SSPM_RST) &&
		(mark_bit != MTK_WDT_STATUS_SYSRST_RST) &&
		(mark_bit != MTK_WDT_STATUS_EINT_RST) &&
		(mark_bit != MTK_WDT_STATUS_SPM_THERMAL_RST))
		return -1;

	tmp = READ_REG(MTK_WDT_REQ_IRQ_EN);
	tmp |= MTK_WDT_REQ_IRQ_KEY;

	if (mode == WD_REQ_IRQ_MODE)
		tmp |= (mark_bit);
	if (mode == WD_REQ_RST_MODE)
		tmp &= ~(mark_bit);

	WRITE_REG(tmp, MTK_WDT_REQ_IRQ_EN);

#if MTK_DOE_CONFIG_ENV_SUPPORT
	/* disable all reset source's reset and irq mode */
	if (g_doe_apwdt_en == NULL)
		g_doe_apwdt_en = dconfig_getenv("apwdt_en");

	if (g_doe_apwdt_en != NULL && *g_doe_apwdt_en == '0')
	       WRITE_REG(MTK_WDT_REQ_IRQ_KEY, MTK_WDT_REQ_IRQ_EN);
#endif

	return 0;
}

void mtk_wdt_set_aee_rsv(unsigned char value)
{
    unsigned int tmp;

    tmp = READ_REG(MTK_WDT_NONRST_REG2);
    tmp &= ~MTK_WDT_NONRST2_AEE_MASK;
    tmp |= ((unsigned int)value) << MTK_WDT_NONRST2_AEE_RSV_SHIFT;
    WRITE_REG(tmp, MTK_WDT_NONRST_REG2);
}

unsigned char mtk_wdt_get_aee_rsv(void)
{
    unsigned int aee_rsv = 0;

    aee_rsv = READ_REG(MTK_WDT_NONRST_REG2);
    aee_rsv &= MTK_WDT_NONRST2_AEE_MASK;

    return (unsigned char)(aee_rsv >> MTK_WDT_NONRST2_AEE_RSV_SHIFT);
}

static void g_rgu_status_parsing(void)
{
	unsigned int wdt_sta = 0, nonrst2 = 0;

	wdt_sta = mtk_wdt_get_status();
	nonrst2 = DRV_Reg32(MTK_WDT_NONRST_REG2);

	if ((wdt_sta & MTK_WDT_STATUS_HWWDT_RST) &&
		    !(wdt_sta & MTK_WDT_STATUS_SWWDT_RST) &&
		    (nonrst2 & MTK_WDT_NONRST2_BYPASS_PWR_KEY)) {
		/* Time out reboot always by pass power key */
		g_rgu_status = RE_BOOT_BY_WDT_HW;
	} else if (wdt_sta & MTK_WDT_STATUS_SWWDT_RST)
		g_rgu_status = RE_BOOT_BY_WDT_SW;
	else
		g_rgu_status = RE_BOOT_REASON_UNKNOW;

	if (wdt_sta & MTK_WDT_STATUS_IRQWDT_RST)
		g_rgu_status |= RE_BOOT_WITH_INTTERUPT;

	if (wdt_sta & MTK_WDT_STATUS_SPMWDT_RST)
		g_rgu_status |= RE_BOOT_BY_SPM;

	if (wdt_sta & MTK_WDT_STATUS_THERMAL_DIRECT_RST)
		g_rgu_status |= RE_BOOT_BY_THERMAL_DIRECT;

	if (wdt_sta & MTK_WDT_STATUS_SECURITY_RST)
		g_rgu_status |= RE_BOOT_BY_SECURITY;

	if (wdt_sta & MTK_WDT_STATUS_SYSRST_RST)
		g_rgu_status |= RE_BOOT_BY_SYSRST;

	if (wdt_sta & MTK_WDT_STATUS_SSPM_RST)
		g_rgu_status |= RE_BOOT_BY_SSPM_RST;

	if (wdt_sta & MTK_WDT_STATUS_MDDBG_RST)
		g_rgu_status |= RE_BOOT_BY_MDDEBUG;

	if (wdt_sta & MTK_WDT_STATUS_MCUPM_RST)
		g_rgu_status |= RE_BOOT_BY_MCUPM_RST;

	if (wdt_sta & MTK_WDT_STATUS_GPUEB_RST)
		g_rgu_status |= RE_BOOT_BY_GPUEB_RST;

	if (wdt_sta & MTK_WDT_STATUS_DVFSP_RST)
		g_rgu_status |= RE_BOOT_BY_MDP2P_RST;

	if (wdt_sta & MTK_WDT_STATUS_ERRFLG_RST)
		g_rgu_status |= RE_BOOT_BY_ERRFLG;

	if (wdt_sta & MTK_WDT_STATUS_UDISCN_RST)
		g_rgu_status |= RE_BOOT_BY_UDISCN;

	if (wdt_sta & MTK_WDT_STATUS_DEBUG_RST)
		g_rgu_status |= RE_BOOT_BY_DEBUG;

	if (wdt_sta & MTK_WDT_STATUS_HRE_RST)
		g_rgu_status |= RE_BOOT_BY_HRE;

	if (wdt_sta & MTK_WDT_STATUS_TIA)
		g_rgu_status |= RE_BOOT_BY_TIA;

	/* SW workaround to avoid EINT_RST be triggered with other RST behavior */
	if ((wdt_sta & MTK_WDT_STATUS_EINT_RST) && (g_rgu_status == RE_BOOT_REASON_UNKNOW))
		g_rgu_status |= RE_BOOT_BY_EINT;
	else if ((READ_REG(MTK_WDT_REQ_IRQ_EN) & MTK_WDT_REQ_IRQ_EINT_EN) &&
		 (g_rgu_status & RE_BOOT_WITH_INTTERUPT) &&
		 (wdt_sta & MTK_WDT_STATUS_EINT_RST))
		g_rgu_status |= RE_BOOT_BY_EINT;

	RGULOG("parse g_rgu_status: %d (0x%x)\n", g_rgu_status, g_rgu_status);
}

#if MTK_DOE_CONFIG_ENV_SUPPORT
void mtk_wdt_get_env(void)
{
	g_doe_apwdt_en = dconfig_getenv("apwdt_en");

	if ((g_doe_apwdt_en != NULL) && (*g_doe_apwdt_en == '1')) {
		mtk_wdt_mode_config(TRUE, TRUE, TRUE, FALSE, FALSE);
		mtk_wdt_restart();
	}

	g_doe_pmic_rst = dconfig_getenv("rstmode");

	if (g_doe_pmic_rst != NULL && *g_doe_pmic_rst == '1')
		WRITE_REG(DRV_Reg32(MTK_WDT_NONRST_REG2) | MTK_WDT_NONRST2_PMIC_RST,
			  MTK_WDT_NONRST_REG2);
       else
		WRITE_REG(DRV_Reg32(MTK_WDT_NONRST_REG2) & ~MTK_WDT_NONRST2_PMIC_RST,
			  MTK_WDT_NONRST_REG2);
}
#endif


static unsigned char mtk_wdt_get_restart_reason(void)
{
	u32 tmp;

	tmp = DRV_Reg32(MTK_WDT_NONRST_REG2) & MTK_WDT_NONRST2_BOOT_MASK;
	return tmp;
}
static unsigned int mtk_wdt_check_current_stage(void)
{
	unsigned int reg;
	unsigned int current_stage;

	/* check reboot source */
	reg = DRV_Reg32(MTK_WDT_NONRST_REG2);
	current_stage = (reg >> MTK_WDT_NONRST2_STAGE_OFS) & RGU_STAGE_MASK;

	if (current_stage == RGU_STAGE_DA)
		return current_stage;
	else
		return 0;
}
void mtk_wdt_pre_init(void)
{
	unsigned int wdt_ctrl;
	unsigned int nonrst;

	/* skip if platform already run mtk_wdt_pre_init in early stage*/
	if (g_rgu_inited == 1) {
		RGULOG("WDT is already inited\n");
		return;
	}

	/* get last stage and mark current stage to preloader */
	mtk_wdt_check_last_stage();

	if(mtk_wdt_get_restart_reason() != MTK_WDT_NONRST2_BOOT_BOOTLOADER_DA_STAGE 
			|| mtk_wdt_check_current_stage() != RGU_STAGE_DA)
		mtk_wdt_mark_stage(RGU_STAGE_PRELOADER);
	mtk_wdt_restore_dfd_setting();

	/*
	 * Since RGU init flow will modify WDT_MODE which will reset WDT_STA and some bits
	 * in WDT_DEBUG_CTL, preserve these registers in static global variable first.
	 */
	mtk_wdt_keep_essential_info();

	/* Dump RGU regisers */
	RGULOG("MODE:               0x%x\n", DRV_Reg32(MTK_WDT_MODE));
	RGULOG("STA:                0x%x\n", mtk_wdt_get_status());
	RGULOG("LENGTH:             0x%x\n", DRV_Reg32(MTK_WDT_LENGTH));
	RGULOG("INTERVAL:           0x%x\n", DRV_Reg32(MTK_WDT_INTERVAL));
	RGULOG("SWSYSRST:           0x%x\n", DRV_Reg32(MTK_WDT_SWSYSRST));
	RGULOG("LATCH_CTL:          0x%x\n", DRV_Reg32(MTK_WDT_LATCH_CTL));
	RGULOG("LATCH_CTL2:         0x%x\n", DRV_Reg32(MTK_WDT_LATCH_CTL2));
	RGULOG("NONRST_REG:         0x%x\n", DRV_Reg32(MTK_WDT_NONRST_REG));
	RGULOG("NONRST_REG2:        0x%x\n", DRV_Reg32(MTK_WDT_NONRST_REG2));
	RGULOG("REQ_MODE:           0x%x\n", DRV_Reg32(MTK_WDT_REQ_MODE));
	RGULOG("REQ_IRQ_EN:         0x%x\n", DRV_Reg32(MTK_WDT_REQ_IRQ_EN));
	RGULOG("DEBUG_CTL:          0x%x\n", mtk_wdt_get_debug_ctl());

	g_rgu_status_parsing();

	rgu_mode = DRV_Reg32(MTK_WDT_MODE);

	/*
	 * MTK_WDT_MODE_AUTO_RESTART is replaced by MTK_WDT_NONRST2_BYPASS_PWR_KEY
	 * of NONRST_REG2 for common kernel projects and two stage timeout design
	 */
	if ((DRV_Reg32(MTK_WDT_NONRST_REG2) & MTK_WDT_NONRST2_BYPASS_PWR_KEY) ||
	    (DRV_Reg32(MTK_WDT_MODE) & MTK_WDT_MODE_EXTRA_CNT))
		rgu_mode |= MTK_WDT_NONRST2_BYPASS_PWR_KEY;

	WRITE_REG(DRV_Reg32(MTK_WDT_NONRST_REG2) & ~(MTK_WDT_NONRST2_SSPM_RAISEV |
		   MTK_WDT_NONRST2_VPROC_BEFORE | MTK_WDT_NONRST2_VPROC_AFTER |
		   MTK_WDT_NONRST2_FLUSH_AFTER | MTK_WDT_NONRST2_RST_LAST),
		   MTK_WDT_NONRST_REG2);

	nonrst = DRV_Reg32(MTK_WDT_NONRST_REG);
	if (nonrst & ~(MTK_WDT_STATUS_SWWDT_RST))
		nonrst = nonrst | mtk_wdt_get_status();
	else
		nonrst = mtk_wdt_get_status();

#if (CFG_APWDT_REQ_BYPASS)
	if(nonrst & CFG_APWDT_REQ_BYPASS) {
		nonrst &= ~CFG_APWDT_REQ_BYPASS;
		RGULOG("========bypass CFG_APWDT_REQ_BYPASS========\n");
	}
#endif

	WRITE_REG(nonrst & ~(MTK_WDT_STATUS_SWWDT_RST), MTK_WDT_NONRST_REG);
	RGULOG("Set NONRST_REG to 0x%x\n", nonrst);

	mtk_wdt_mode_config(FALSE, FALSE, TRUE, FALSE, FALSE); /* Wirte Mode register will clear status register */
	/* Setting timeout 10s */
	mtk_wdt_set_time_out_value(WDT_TIMEOUT_DEFAULT_VAL);
	mtk_wdt_set_dual_time_out_value(WDT_DUAL_TIMEOUT_DEFAULT_VAL);
	mtk_wdt_set_dual_time_out_en(MTK_WDT_DUAL_LEN_SPM_EN);

#if (CFG_APWDT_REQ_MODE)
	wdt_ctrl = MTK_WDT_REQ_MODE_KEY | CFG_APWDT_REQ_MODE;
#else
	wdt_ctrl = MTK_WDT_REQ_MODE_KEY | MTK_WDT_REQ_MODE_DEFAULT;
#endif
	WRITE_REG(wdt_ctrl, MTK_WDT_REQ_MODE);

#if (CFG_APWDT_REQ_IRQ)
	wdt_ctrl = MTK_WDT_REQ_IRQ_KEY | CFG_APWDT_REQ_IRQ;
#else
	wdt_ctrl = MTK_WDT_REQ_IRQ_KEY | MTK_WDT_REQ_IRQ_DEFAULT;
#endif
	WRITE_REG(wdt_ctrl, MTK_WDT_REQ_IRQ_EN);

#if (!CFG_APWDT_DISABLE)
	/* Config HW reboot mode */
#if MTK_DOE_CONFIG_ENV_SUPPORT
	g_doe_apwdt_en = dconfig_getenv("apwdt_en");
#endif
	if (g_doe_apwdt_en == NULL || *g_doe_apwdt_en == '1') {
		mtk_wdt_mode_config(TRUE, TRUE, TRUE, FALSE, TRUE);
		mtk_wdt_restart();
	}
#endif

	mtk_wdt_timestamp_init();

	mtk_wdt_reset_deglitch_enable();

	/* Configure WDT_LATCH_CTL */

	wdt_ctrl = READ_REG(MTK_WDT_LATCH_CTL);
	wdt_ctrl |= MTK_LATCH_CTL_KEY;
	wdt_ctrl |= MTK_RG_LATH_EN;
	wdt_ctrl |= MTK_RG_MCU_LATH_EN | MTK_RG_SPM_LATH_EN;
	wdt_ctrl |= MTK_RG_GPU_EXT_OFF_EN | MTK_RG_MD_EXT_OFF_EN;
	wdt_ctrl &= ~(MTK_RG_MCU_LATCH_SELECT | MTK_RG_SPM_LATCH_SELECT);
	wdt_ctrl |= MTK_RG_DRAMC_LATH_EN;
	wdt_ctrl |= MTK_RG_DEBUGSYS_LATCH_EN;
	wdt_ctrl |= (MTK_RG_DRAMC_RD_TEST_EN | MTK_RG_DRAMC_RDWT_TEST_EN);
	WRITE_REG(wdt_ctrl, MTK_WDT_LATCH_CTL);

	/* Configure WDT_LATCH_CTL2 */

	/* DFD feature, this timeout should be changed in kernel bring up stage
	 * wdt_ctrl = READ_REG(MTK_WDT_LATCH_CTL2);
	 * wdt_ctrl |= MTK_LATCH_CTL2_KEY;
	 * wdt_ctrl |= MTK_RG_MCU_DFD_EN;
	 * wdt_ctrl &= ~(MTK_RG_MCU_DFD_TIMEOUT_MASK << MTK_RG_MCU_DFD_TIMEOUT_OFS);
	 * wdt_ctrl |= (MTK_RG_MCU_DFD_TIMEOUT_VALUE << MTK_RG_MCU_DFD_TIMEOUT_OFS);
	 * WRITE_REG(wdt_ctrl, MTK_WDT_LATCH_CTL2);
	 */

	/* clear dram reboot */
	wdt_ctrl = readl(MTK_WDT_DEBUG_CTL3);
	wdt_ctrl &= ~MTK_WDT_DEBUG_CTL3_DIS_DDR_RSV;
	writel(wdt_ctrl | MTK_WDT_DEBUG_CTL3_KEY, MTK_WDT_DEBUG_CTL3);

	/*
	 * Configure WDT_DEBUG_CTL
	 *
	 * Release DDR reserve mode related control.
	 *
	 * Release DCS EN/PAUSE and DVFSRC EN/PAUSE in RGU initialization.
	 * NOTE: This job must be done earlier than DVS/DVFSRC initialization.
	 */
	rgu_update_reg(MTK_WDT_DEBUG_CTL, RGU_REG_CLR,
		MTK_RGU_EMI_DCS_PAUSE | MTK_RGU_DVFSRC_PAUSE);

	rgu_update_reg(MTK_WDT_DEBUG_CTL2, RGU_REG_CLR,
		MTK_RGU_EMI_DCS_EN | MTK_RGU_DVFSRC_EN);

	RGULOG("%s: MTK_WDT_DEBUG_CTL(0x%x)\n", __func__,
		READ_REG(MTK_WDT_DEBUG_CTL));

	RGULOG("%s: MTK_WDT_DEBUG_CTL2(0x%x)\n", __func__,
		READ_REG(MTK_WDT_DEBUG_CTL2));

	RGULOG("%s: MTK_WDT_LATCH_CTL(0x%x)\n", __func__,
		READ_REG(MTK_WDT_LATCH_CTL));

	RGULOG("%s: MTK_WDT_REQ_MODE(%x), MTK_WDT_REQ_IRQ_EN(%x)\n", __func__,
		READ_REG(MTK_WDT_REQ_MODE), READ_REG(MTK_WDT_REQ_IRQ_EN));

	mtk_wdt_timestamp_update();

	g_rgu_inited = 1;
}

void mtk_wdt_init(void)
{
	if (g_rgu_inited == 0) {
		mtk_wdt_pre_init();
	} else {
		/* RGU clock source might be changed since pre_init which might
		 * make rgu work abnormal due clock source switch. We restart wdt
		 * by disable and enable it again */
		mtk_wdt_mode_config(FALSE, FALSE, TRUE, FALSE, FALSE);
		mtk_wdt_set_time_out_value(WDT_TIMEOUT_DEFAULT_VAL);
#if (!CFG_APWDT_DISABLE)
		/* Config HW reboot mode */
		mtk_wdt_mode_config(TRUE, TRUE, TRUE, FALSE, TRUE);
		mtk_wdt_restart();
#endif
	}

	if (mtk_wdt_is_pmic_full_reset())
		g_rgu_status |= RE_BOOT_BY_PMIC_FULL_RST;

	RGULOG("g_rgu_status: %d (0x%x)\n", g_rgu_status, g_rgu_status);

#if MTK_DOE_CONFIG_ENV_SUPPORT
	if (g_doe_apwdt_en == NULL)
		g_doe_apwdt_en = dconfig_getenv("apwdt_en");

	if (g_doe_apwdt_en != NULL && *g_doe_apwdt_en == '0')
		mtk_wdt_disable();
#endif

	mtk_wdt_check_trig_reboot_reason();

#if (CFG_FORCE_RECOVERY)
	WRITE_REG((DRV_Reg32(MTK_WDT_NONRST_REG2) & ~MTK_WDT_NONRST2_BOOT_MASK) |
		MTK_WDT_NONRST2_BOOT_RECOVERY, MTK_WDT_NONRST_REG2);
#endif
}

#else /* Using dummy WDT functions */
void mtk_wdt_pre_init(void)
{
	RGULOG("PL WDT Dummy pre_init called\n");
}

void mtk_wdt_init(void)
{
	RGULOG("PL WDT Dummy init called\n");
}

BOOL mtk_is_rgu_trigger_reset(void)
{
	RGULOG("PL Dummy mtk_is_rgu_trigger_reset called\n");
	return FALSE;
}

void mtk_arch_reset(char mode)
{
	RGULOG("PL WDT Dummy arch reset called\n");
}

int mtk_wdt_boot_check(void)
{
	RGULOG("PL WDT Dummy mtk_wdt_boot_check called\n");
	return 0;
}

void mtk_wdt_disable(void)
{
	RGULOG("UB WDT Dummy mtk_wdt_disable called\n");
}

void mtk_wdt_restart(void)
{
	RGULOG("UB WDT Dummy mtk_wdt_restart called\n");
}

static void mtk_wdt_sw_reset(void)
{
	/* RGULOG("UB WDT Dummy mtk_wdt_sw_reset called\n"); */
}

static void mtk_wdt_hw_reset(void)
{
	RGULOG("UB WDT Dummy mtk_wdt_hw_reset called\n");
}

int rgu_dram_reserved(int enable)
{
	volatile unsigned int ret = 0;

	RGULOG("dummy RGU %s\n", __func__);
	return ret;
}

int rgu_is_reserve_ddr_enabled(void)
{
	RGULOG("dummy RGU %s\n", __func__);
	return 0;
}

int rgu_is_dram_slf(void)
{
	RGULOG("dummy RGU %s\n", __func__);
	return 0;
}

int rgu_release_rg_dramc_conf_iso(void)
{
	RGULOG("dummy RGU %s\n", __func__);
	return 0;
}

int rgu_release_rg_dram_setting(void)
{
	RGULOG("dummy RGU %s\n", __func__);
	return 0;
}

int rgu_release_rg_dramc_iso(void)
{
	RGULOG("dummy RGU %s\n", __func__);
	return 0;
}

int rgu_release_rg_dramc_sref(void)
{
	RGULOG("dummy RGU %s\n", __func__);
	return 0;
}

int rgu_is_reserve_ddr_mode_success(void)
{
	RGULOG("dummy RGU %s\n", __func__);
	return 0;
}

void rgu_swsys_reset(WD_SYS_RST_TYPE reset_type)
{
	/* RGULOG("dummy RGU %s\n", __func__); */
}

int mtk_wdt_request_en_set(int mark_bit, WD_REQ_CTL en)
{
	RGULOG("dummy RGU %s\n", __func__);
}

int mtk_wdt_request_mode_set(int mark_bit, WD_REQ_MODE mode)
{
	RGULOG("dummy RGU %s\n", __func__);
}

BOOL mtk_is_rst_from_da(void)
{
	return FALSE;
}

void mtk_wdt_set_aee_rsv(unsigned char value)
{
	RGULOG("dummy RGU %s\n", __func__);
}

unsigned char mtk_wdt_get_aee_rsv(void)
{
	RGULOG("dummy RGU %s\n", __func__);
}
#endif
