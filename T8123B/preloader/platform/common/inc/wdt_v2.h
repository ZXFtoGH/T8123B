/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2018
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
*  RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
*
*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
*
*****************************************************************************/

#ifndef __MTK_WDT_H__
#define __MTK_WDT_H__

#define MTK_WDT_BASE            RGU_BASE

#define MTK_WDT_MODE            (MTK_WDT_BASE+0x0000)
#define MTK_WDT_IRQ_CLR         (MTK_WDT_MODE)
#define MTK_WDT_LENGTH            (MTK_WDT_BASE+0x0004)
#define MTK_WDT_RESTART            (MTK_WDT_BASE+0x0008)
#define MTK_WDT_STATUS            (MTK_WDT_BASE+0x000C)
#define MTK_WDT_INTERVAL        (MTK_WDT_BASE+0x0010)
#define MTK_WDT_SWRST            (MTK_WDT_BASE+0x0014)
#define MTK_WDT_SWSYSRST        (MTK_WDT_BASE+0x0018)
#define WDT_DUAL_LEN_EN         (MTK_WDT_SWSYSRST)
#define MTK_WDT_NONRST_REG        (MTK_WDT_BASE+0x0020)
#define MTK_WDT_NONRST_REG2        (MTK_WDT_BASE+0x0024)
#define MTK_WDT_REQ_MODE        (MTK_WDT_BASE+0x0030)
#define MTK_WDT_REQ_IRQ_EN        (MTK_WDT_BASE+0x0034)
#define MTK_WDT_DEBUG_CTL        (MTK_WDT_BASE+0x0040)
#define MTK_WDT_LATCH_CTL        (MTK_WDT_BASE+0x0044)
#define MTK_WDT_LATCH_CTL2        (MTK_WDT_BASE+0x0048)
#define MTK_WDT_DUAL_LENGTH        (MTK_WDT_BASE+0x0078)
#define MTK_WDT_RSTDEG_EN1        (MTK_WDT_BASE+0x0080)
#define MTK_WDT_RSTDEG_EN2        (MTK_WDT_BASE+0x0084)
#define MTK_WDT_RESET_PROTECT    (MTK_WDT_BASE+0x0090)
#define MTK_WDT_DEBUG_CTL2        (MTK_WDT_BASE+0x00A0)
#define MTK_WDT_PWR_LATCH        (MTK_WDT_BASE+0x00A4)
#define MTK_WDT_DEBUG_CTL3         (MTK_WDT_BASE+0x00A8)
#define MTK_WDT_DEBUG_0_REG        (MTK_WDT_BASE+0x0500)
#define MTK_WDT_DEBUG_1_REG        (MTK_WDT_BASE+0x0504)
#define MTK_WDT_DEBUG_2_REG        (MTK_WDT_BASE+0x0508)
#define MTK_WDT_DEBUG_3_REG        (MTK_WDT_BASE+0x050C)
#define MTK_WDT_DEBUG_4_REG        (MTK_WDT_BASE+0x0510)
#define MTK_WDT_DEBUG_5_REG        (MTK_WDT_BASE+0x0514)
#define MTK_WDT_DEBUG_6_REG        (MTK_WDT_BASE+0x0518)

/*WDT_MODE*/
#define MTK_WDT_MODE_KEYMASK        (0xff00)
#define MTK_WDT_MODE_KEY        (0x22000000)

#define MTK_WDT_MODE_DDR_RESERVE  (0x0080)
#define MTK_WDT_MODE_DUAL_MODE  (0x0040)
#define MTK_WDT_MODE_LVL_EN        (0x0020)
/*
 * MTK_WDT_MODE_AUTO_RESTART is replaced by MTK_WDT_NONRST2_BYPASS_PWR_KEY
 * of NONRST_REG2 for common kernel projects and two stage timeout design
 */
#define MTK_WDT_MODE_EXTRA_CNT    (0x0010)
#define MTK_WDT_MODE_IRQ        (0x0008)
#define MTK_WDT_MODE_EXTEN        (0x0004)
#define MTK_WDT_MODE_EXT_POL        (0x0002)
#define MTK_WDT_MODE_ENABLE        (0x0001)

/*WDT_IRQ_CLR*/
#define MTK_WDT_IRQ_CLR_KEY        (0x23000000)
#define MTK_WDT_IRQ_CLR_MASK       (0x1FF)
/* clear WDT_STATUS [4~0] */
#define MTK_WDT_IRQ_CLR_EXTB       (1 << 0)
/* clear WDT_STATUS [17~16] */
#define MTK_WDT_IRQ_CLR_EXT        (1 << 1)
/* clear WDT_STATUS [28] */
#define MTK_WDT_IRQ_CLR_SEJ        (1 << 2)
/* clear WDT_STATUS [29] */
#define MTK_WDT_IRQ_CLR_IRQ_STS    (1 << 3)
/* clear WDT_STATUS [30] */
#define MTK_WDT_IRQ_CLR_SW_WDT     (1 << 4)
/* clear WDT_STATUS [31] */
#define MTK_WDT_IRQ_CLR_WDT_STS    (1 << 5)
/* clear event_b to SPM */
#define MTK_WDT_IRQ_CLR_WAKE_UP    (1 << 6)
/* deassert wdt irq */
#define MTK_WDT_IRQ_CLR_DEASSERT   (1 << 7)
#define MTK_WDT_IRQ_CLR_DEBUGSYS   (1 << 8)

/*WDT_LENGTH*/
#define MTK_WDT_LENGTH_TIME_OUT        (0xffe0)
#define MTK_WDT_LENGTH_KEYMASK        (0x001f)
#define MTK_WDT_LENGTH_KEY        (0x0008)
#define MTK_WDT_LENGTH_CTL_KEY        (0x95U<<24)

/*WDT_RESTART*/
#define MTK_WDT_RESTART_KEY        (0x1971)

/*WDT_STATUS*/
#define MTK_WDT_STATUS_HWWDT_RST    (0x80000000)
#define MTK_WDT_STATUS_SWWDT_RST    (0x40000000)
#define MTK_WDT_STATUS_IRQWDT_RST    (0x20000000)
#define MTK_WDT_STATUS_DEBUGWDT_RST    (0x00080000)
#define MTK_WDT_STATUS_SPMWDT_RST    (0x0002)
#define MTK_WDT_STATUS_SPM_THERMAL_RST    (0x0001)
#define MTK_WDT_STATUS_TIA             MTK_WDT_STATUS_SPM_THERMAL_RST
#define MTK_WDT_STATUS_SECURITY_RST    (1<<28)
#define MTK_WDT_STATUS_EINT_RST        (1<<2)
#define MTK_WDT_STATUS_SYSRST_RST    (1<<3)
#define MTK_WDT_STATUS_DVFSP_RST    (1<<4)
#define MTK_WDT_STATUS_MCUPM_RST    (1<<5)
#define MTK_WDT_STATUS_GPUEB_RST    (1<<6)
#define MTK_WDT_STATUS_ERRFLG_RST   (1<<7)
#if (CFG_APWDT_STA_V2)
#define MTK_WDT_STATUS_SSPM_RST     (1<<14)
#define MTK_WDT_STATUS_MDDBG_RST    (1<<16)
#define MTK_WDT_STATUS_THERMAL_DIRECT_RST    (1<<17)
#define MTK_WDT_STATUS_UDISCN_RST   (1<<15) /* no udiscn_rst, find an empty slot */
#define MTK_WDT_STATUS_DEBUG_RST    (1<<18)
#define MTK_WDT_STATUS_HRE_RST      (1<<19)
#else
#define MTK_WDT_STATUS_SSPM_RST     (1<<16)
#define MTK_WDT_STATUS_MDDBG_RST    (1<<17)
#define MTK_WDT_STATUS_THERMAL_DIRECT_RST    (1<<18)
#define MTK_WDT_STATUS_UDISCN_RST   (1<<19)
#define MTK_WDT_STATUS_DEBUG_RST    (1<<20)
#define MTK_WDT_STATUS_HRE_RST      (1<<21)
#endif

/*WDT_RST_DEGLITCH*/
#define MTK_WDT_RSTDEG_EN1_KEY        (0xa357)
#define MTK_WDT_RSTDEG_EN2_KEY        (0x67d2)
#define MTK_WDT_RSTDEG_CLOCK_SELETC_SHIFT    (31)
#define MTK_WDT_RSTDEG_CLOCK_SELECT_MASK    (0x1U << MTK_WDT_RSTDEG_CLOCK_SELETC_SHIFT)
#define MTK_WDT_RSTDEG_CLOCK_26M            (0)
#define MTK_WDT_RSTDEG_CLOCK_32K            (1U)
#define MTK_WDT_RSTDEG_LATENCY_SHIFT        (24)
#define MTK_WDT_RSTDEG_LATENCY_MASK        (0xF << MTK_WDT_RSTDEG_LATENCY_SHIFT)
#define MTK_WDT_RSTDEG_LATENCY_118US_93MS    (0x0)
#define MTK_WDT_RSTDEG_LATENCY_59US_46MS    (0x1)
#define MTK_WDT_RSTDEG_LATENCY_29US_23MS    (0x2)
#define MTK_WDT_RSTDEG_LATENCY_14US_11MS    (0x3)
#define MTK_WDT_RSTDEG_LATENCY_7US_5MS        (0x4)
#define MTK_WDT_RSTDEG_LATENCY_3US_2MS        (0x5)
#define MTK_WDT_RSTDEG_LATENCY_1US_1MS        (0x6)
#define MTK_WDT_RSTDEG_LATENCY_923NS_732US    (0x7)
#define MTK_WDT_RSTDEG_LATENCY_461NS_366US    (0x8)
#define MTK_WDT_RSTDEG_LATENCY_230NS_183US    (0x9)
#define MTK_WDT_RSTDEG_LATENCY_115NS_91US    (0xA)

/*WDT_INTERVAL*/
#define MTK_WDT_INTERVAL_KEY         (0x66000000)
#define MTK_WDT_INTERVAL_MASK        (0x0FFF)

/*WDT_SWRST*/
#define MTK_WDT_SWRST_KEY        (0x1209)

/*WDT_SWSYSRST*/
#define MTK_WDT_SWSYS_RST_C2K_RST              (0x8000)
#define MTK_WDT_SWSYS_RST_CONN_MCU_RST         (0x1000)
#define MTK_WDT_SWSYS_RST_PWRAP_SPI_CTL_RST    (0x0800)
#define MTK_WDT_SWSYS_RST_APMIXED_RST    (0x0400)
#define MTK_WDT_SWSYS_RST_MD_LITE_RST    (0x0200)
#define MTK_WDT_SWSYS_RST_INFRA_AO_RST    (0x0100)
#define MTK_WDT_SWSYS_RST_MD_RST    (0x0080)
#define MTK_WDT_SWSYS_RST_DDRPHY_RST    (0x0040)
#define MTK_WDT_SWSYS_RST_IMG_RST    (0x0020)
#define MTK_WDT_SWSYS_RST_VDEC_RST    (0x0010)
#define MTK_WDT_SWSYS_RST_VENC_RST    (0x0008)
#define MTK_WDT_SWSYS_RST_MFG_RST    (0x0004)
#define MTK_WDT_SWSYS_RST_DISP_RST    (0x0002)
#define MTK_WDT_SWSYS_RST_INFRA_RST    (0x0001)

/*WDT_DUAL_LEN_EN*/
#define MTK_WDT_DUAL_LEN_EN_KEY        (0x9U << 28)
#define MTK_WDT_DUAL_LEN_EN_KEY_2      (0x66 << 24)
#define MTK_WDT_DUAL_LEN_MASK          (0xFFFFFFF)
#define MTK_WDT_DUAL_LEN_SPM_EN        (MTK_WDT_STATUS_SPMWDT_RST)

/* Reboot source */
#define RGU_STAGE_MASK      (0x7U)
#define RGU_STAGE_PRELOADER (0x1)
#define RGU_STAGE_LK        (0x2)
#define RGU_STAGE_KERNEL    (0x3)
#define RGU_STAGE_DA        (0x4)
#define RGU_STAGE_BL2_EXT   (0x5)
#define RGU_STAGE_AEE       (0x6)
#define RGU_STAGE_TFA       (0x7)

/* WDT_NONRST_REG2 */
#define MTK_WDT_NONRST2_BOOT_MASK      (0xF)
#define MTK_WDT_NONRST2_AEE_MASK       (0xFF0)
#define MTK_WDT_NONRST2_AEE_RSV_SHIFT   4
#define MTK_WDT_NONRST2_BOOT_BOOTLOADER 3
#define MTK_WDT_NONRST2_BOOT_RECOVERY   2
#define MTK_WDT_NONRST2_RPMB_PROG_KEY  (1 << 3)
#define MTK_WDT_NONRST2_BYPASS_PWR_KEY (1 << 13)
#define MTK_WDT_NONRST2_SSPM_RAISEV    (1 << 16)
#define MTK_WDT_NONRST2_VPROC_BEFORE   (1 << 17)
#define MTK_WDT_NONRST2_VPROC_AFTER    (1 << 18)
#define MTK_WDT_NONRST2_FLUSH_AFTER    (1 << 19)
#define MTK_WDT_NONRST2_RST_LAST       (1 << 20)
#define MTK_WDT_NONRST2_PMIC_RST       (1 << 23)
#define MTK_WDT_NONRST2_STAGE_OFS      (29) /* 31:29: 2-bits for current stage */
#define MTK_WDT_NONRST2_LAST_STAGE_OFS (26) /* 28:26: 2-bits for last stage */

#define MTK_WDT_SWSYS_RST_KEY        (0x88000000)
#define MTK_WDT_REQ_IRQ_KEY        (0x44000000)
#define MTK_WDT_REQ_IRQ_EINT_EN      (1<<2)
#define MTK_WDT_REQ_IRQ_DEFAULT    (0x3B00F0)

#define MTK_WDT_REQ_MODE_KEY    (0x33000000)
#define MTK_WDT_REQ_MODE_DEFAULT   (0x3F00F2)

/* WDT_NONRST_REG */
#define MTK_WDT_NONRST_DL              (0x00008000)

/* MTK_WDT_DEBUG_CTL */
#define MTK_DEBUG_CTL_KEY              (0x59000000)
#define MTK_RG_DRAMC_SREF              (0x00100)
#define MTK_RG_DRAMC_ISO               (0x00200)
#define MTK_RG_CONF_ISO                (0x00400)
#define MTK_RGU_EMI_DCS_PAUSE          (0x00004000) /* bit 14: dcs_pause */
#define MTK_RGU_DVFSRC_PAUSE           (0x00008000) /* bit 15: dvfsrc_pause */
#define MTK_DDR_RESERVE_RTA            (0x10000)    /* bit 16: ddr_reserve_success */
#define MTK_DDR_SREF_STA               (0x20000)    /* bit 17: ddr_sref_status */
#define MTK_RGU_EMI_DCS_SUCCESS_OFFSET (18)         /* bit 18: emi_dcs_success */
#define MTK_RGU_EMI_DCS_SUCCESS        (1 << MTK_RGU_EMI_DCS_SUCCESS_OFFSET)
#define MTK_RGU_DVFSRC_SUCCESS_OFFSET  (20)         /* bit 20: dvfsrc_success */
#define MTK_RGU_DVFSRC_SUCCESS         (1 << MTK_RGU_DVFSRC_SUCCESS_OFFSET)

/* MTK_WDT_DEBUG_CTL2 */
#define MTK_DEBUG_CTL2_KEY           (0x55000000)
#define MTK_RGU_EMI_DCS_EN           (1 << 8) /* emi_dcs_en */
#define MTK_RGU_DVFSRC_EN            (1 << 9) /* dvfsrc_en */

/* MTK_WDT_LATCH_CTL */
#define MTK_LATCH_CTL_KEY            (0x95000000)
#define MTK_RG_LATH_EN               (1 << 0)
#define MTK_RG_MCU_LATCH_SELECT      (1 << 1)
#define MTK_RG_SPM_LATCH_SELECT      (1 << 2)
#define MTK_RG_MCU_LATH_EN           (1 << 4)
#define MTK_RG_SPM_LATH_EN           (1 << 5)
#define MTK_RG_DRAMC_LATH_EN         (1 << 6)
#define MTK_RG_MPO_EXT_OFF_EN        (1 << 8)
#define MTK_RG_GPU_EXT_OFF_EN        (1 << 9)
#define MTK_RG_MD_EXT_OFF_EN         (1 << 10)
#define MTK_RG_DRAMC_RD_TEST_EN      (1 << 11)
#define MTK_RG_DRAMC_RDWT_TEST_EN    (1 << 12)
#define MTK_RG_DVFSRC_LATCH_EN       (1 << 13) /* obsolete */
#define MTK_RG_EMI_LATCH_EN          (1 << 14)
#define MTK_RG_DEBUGSYS_LATCH_EN     (1 << 17)

/* MTK_WDT_LATCH_CTL2 */
#define MTK_LATCH_CTL2_KEY           (0x95000000)
#define MTK_LATCH_CTL2_KEY2          (0x90000000)
#define MTK_WDT_LATCH_CTL2_DFD_DEF   (0x2F)
#define MTK_RG_MCU_DFD_EN            (1 << 17)
#define MTK_RG_MCU_DFD_TIMEOUT_MASK  (0x1FFFF)
#define MTK_RG_MCU_DFD_TIMEOUT_OFS   (0)
#define MTK_RG_MCU_DFD_TIMEOUT_VALUE (0x1FFFF)

/* MTK_WDT_DUAL_LENGTH */
#define MTK_WDT_DUAL_LENGTH_KEY           (0x8)
#define MTK_WDT_DUAL_LENGTH_MASK          (0x7FF)
#define MTK_WDT_DUAL_LENGTH_SHIFT         5
#define MTK_WDT_DUAL_LENGTH_EINT_EN       (1 << 16)
#define MTK_WDT_DUAL_LENGTH_SYSRST_EN     (1 << 17)

/* MTK_WDT_DEBUG_CTL3 */
#define MTK_WDT_DEBUG_CTL3_KEY	 (0x51000000)
#define MTK_WDT_DEBUG_CTL3_DIS_DDR_RSV	(1 << 15)

/* Reboot reason */
#define RE_BOOT_REASON_UNKNOW           (0x00)
#define RE_BOOT_BY_WDT_HW               (0x01)
#define RE_BOOT_BY_WDT_SW               (0x02)
#define RE_BOOT_WITH_INTTERUPT          (0x04)
#define RE_BOOT_BY_SPM_THERMAL          (0x08)
#define RE_BOOT_BY_TIA                  RE_BOOT_BY_SPM_THERMAL
#define RE_BOOT_BY_SPM                  (0x10)
#define RE_BOOT_BY_THERMAL_DIRECT       (0x20)
#define RE_BOOT_BY_DEBUG                (0x40)
#define RE_BOOT_BY_SECURITY                (0x80)
#define RE_BOOT_BY_EINT                (0x100)
#define RE_BOOT_BY_SYSRST           (0x200)
#define RE_BOOT_BY_SSPM_RST         (0x400)
#define RE_BOOT_BY_PMIC_FULL_RST    (0x800)    /* PMIC full (cold) reset */
#define RE_BOOT_BY_MCUPM_RST        (0x1000)
#define RE_BOOT_BY_GPUEB_RST        (0x2000)
#define RE_BOOT_BY_MDP2P_RST        (0x4000)
#define RE_BOOT_BY_ERRFLG           (0x8000)
#define RE_BOOT_BY_UDISCN           (0x10000)
#define RE_BOOT_BY_MDDEBUG          (0x20000)
#define RE_BOOT_BY_HRE              (0x40000)
#define RE_BOOT_ABNORMAL                (0xF0)

/* Reboot from which stage */
#define RE_BOOT_FROM_UNKNOW             (0x00)
#define RE_BOOT_FROM_PRE_LOADER         (0x01)
#define RE_BOOT_FROM_U_BOOT             (0x02)
#define RE_BOOT_FROM_KERNEL             (0x03)
#define RE_BOOT_FROM_POWER_ON           (0x04)

#define WDT_NORMAL_REBOOT        (0x01)
#define WDT_BY_PASS_PWK_REBOOT        (0x02)
#define WDT_NOT_WDT_REBOOT        (0x00)

typedef enum wd_swsys_reset_type {
    WD_MD_RST,
}WD_SYS_RST_TYPE;

typedef enum wk_req_en {
    WD_REQ_DIS,
    WD_REQ_EN,
} WD_REQ_CTL;

typedef enum wk_req_mode {
    WD_REQ_IRQ_MODE,
    WD_REQ_RST_MODE,
} WD_REQ_MODE;

enum {
    RGU_REG_CLR = 0,
    RGU_REG_SET = 1
};

extern void mtk_wdt_init(void);
extern void mtk_arch_reset(char mode);
extern BOOL mtk_is_rgu_trigger_reset(void);
extern BOOL mtk_is_rst_from_da(void);
extern BOOL mtk_is_enter_fastboot(void);
extern void mtk_enter_fastboot(void);
extern void mtk_mark_da_stage(void);
extern int  mtk_wdt_boot_check(void);
void        mtk_wdt_disable(void);
#if MTK_DOE_CONFIG_ENV_SUPPORT
void        mtk_wdt_get_env(void);
#endif
void        mtk_wdt_pre_init(void);
void        mtk_wdt_restart(void);
void        mtk_wdt_sw_reset(void);
int         mtk_wdt_swsysret_config(int bit, int set_value);
int         mtk_wdt_request_en_set(int mark_bit, WD_REQ_CTL en);
int         mtk_wdt_request_mode_set(int mark_bit, WD_REQ_MODE mode);
void        mtk_wdt_set_aee_rsv(unsigned char value);
unsigned char mtk_wdt_get_aee_rsv(void);
int         rgu_dram_reserved(int enable);
int         rgu_cfg_emi_dcs_en(int enable);
int         rgu_is_dram_slf(void);
int         rgu_is_dvfsrc_enable(void);
int         rgu_is_dvfsrc_success(void);
int         rgu_is_emi_dcs_enable(void);
int         rgu_is_emi_dcs_success(void);
int         rgu_is_reserve_ddr_enabled(void);
int         rgu_is_reserve_ddr_mode_success(void);
int         rgu_release_rg_dram_setting(void);
int         rgu_release_rg_dramc_conf_iso(void);
int         rgu_release_rg_dramc_iso(void);
int         rgu_release_rg_dramc_sref(void);
void        rgu_swsys_reset(WD_SYS_RST_TYPE reset_type);

extern unsigned g_rgu_status;

#endif   /*__MTK_WDT_H__*/
