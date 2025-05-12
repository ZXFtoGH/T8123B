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
 * MediaTek Inc. (C) 2010-2021. All rights reserved.
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
 * The following software/firmware and/or related documentation ("Media Tek
 * Software") have been modified by MediaTek Inc. All revisions are subject to
 * any receiver's applicable license agreements with MediaTek Inc.
 */

//=============================================================================
//  Include Files
//=============================================================================
//#include <common.h>
//#include <ett_common.h>
//#include <test_case_controller.h>
//#include <api.h>
//#include "gpio.h"
//#include "ett_cust.h"
//#include "emi_setting.h"
//#include "pll.h"
//#include "dramc_pi_api.h"
//#include "dramc_register.h"
#include "dramc_common.h"
#include "dramc_int_global.h"
#include "Register_spm_pinmux.h"
#include "Register_DRAMC_NAO_pinmux.h"
//#include "dramc_register.h"


#if __ETT__
#include <ett_common.h>
#endif

#if !__ETT__
#if (FOR_DV_SIMULATION_USED==0)
#include "dram_buffer.h"
#endif
#else
#include <barriers.h>
#endif

#if __ETT__
//#warning "defined __ETT__"
#endif

#if !__ETT__
#if (FOR_DV_SIMULATION_USED==0)
   #include "platform.h"
#if defined(DEF_LAST_DRAMC)
   #include "plat_dbg_info.h"
#endif
#endif
#endif

#if !__ETT__
#include "custom_emi.h"
#endif

#if DRAMC_CTRL_BY_SOC
#include "pll.h"
#endif

#include <emi_hw.h>
#include <emi.h>
#include <emi-addr2dram.h>

#include "voltage.h"


#define bootarg g_dram_buf->bootarg

#if (FOR_DV_SIMULATION_USED==0)
#include <pmic.h>
/* now we can use definition MTK_PMIC_MT6363
 * =====================================================================
 * PMIC   |Power               |Dflt. Volt. |Step   |Support FPWM |Cmt.
  * ---------------------------------------------------------------------
 * Common
 * ---------------------------------------------------------------------
 * MT6363 |Vcore               |0.8v        |6.25mV |Yes          |
 *        |Vm18                |1.8v        |10mV   |No           |
 * ---------------------------------------------------------------------
 * MT6360 |VMDDR               |0.75v       |10mV   |No           |
 * ---------------------------------------------------------------------
 * LP4X
 * ---------------------------------------------------------------------
 * MT6360 |LP4X_VDD2_LP5_VDD2H |1.11v       |5mV    |YES          |
 *        |LP4X_VDDQ_LP5_VDD2L |0.61v       |5mV    |YES          |
 * ---------------------------------------------------------------------
 * LP5
 * ---------------------------------------------------------------------
 * MT6360 |LP4X_VDD2_LP5_VDD2H |1.06v       |5mV    |YES          |
 *        |LP4X_VDDQ_LP5_VDD2L |0.91v       |5mV    |YES          |
 * ---------------------------------------------------------------------
 * MT6315 |LP5_VDDQ            |0.5v        |6.25mV |Yes          |
 * =====================================================================
 */
#define MTK_PMIC_MT6363
#endif

#if !__ETT__
#define mt_reg_sync_write(x,y) mt_reg_sync_writel(y,x)
#endif

#ifdef MTK_PMIC_MT6363
#include <regulator/mtk_regulator.h>
#include <pmic.h>
#endif

#if !__ETT__
#define CQ_DMA_BASE (0x10212000)
#endif

#if !__ETT__
#include "sec_devinfo.h"
#endif

#if !__ETT__ && CFG_ENABLE_DCACHE
#define DRAMC_ASSERT(expr)                                            \
    do {                                                        \
        if (!(expr)) {                                          \
      plat_clean_invalidate_dcache();														\
	    ASSERT(expr);					\
	}							\
    } while (0)
#else
#define DRAMC_ASSERT(expr)         ASSERT(expr)
#endif

//=============================================================================
//  Definition
//=============================================================================

#define VDD1_REGULATOR_NAME         "vm18"
#define VMDDR_DVS_REGULATOR_NAME    "vbuck4_vmddr"	/* use mt6373 BUCK4 for Vmddr */
#define VMDDR_REGULATOR_NAME        "vmddr"
#define VCORE_REGULATOR_NAME        "vcore"
#define VDD2H_LP5_REGULATOR_NAME    "vdd2h"
#define VDD2L_LP5_REGULATOR_NAME    "vdd2l"
#define VDD2H_LP5X_REGULATOR_NAME   "vdd2h"
#define VDD2L_LP5X_REGULATOR_NAME   "vdd2l"
#define VDDQ_LP5_REGULATOR_NAME     "vddq"
#define VDDQ_LP5X_REGULATOR_NAME    "vddq"
int doe_get_config(char* feature);

//========================================================================
#if APPLY_NEW_IO_API
#define Channel_A_DDRPHY_AO_BASE_ADDRESS	0x10238000
#define Channel_B_DDRPHY_AO_BASE_ADDRESS	0x10248000
#define Channel_C_DDRPHY_AO_BASE_ADDRESS	0x10258000
#define Channel_D_DDRPHY_AO_BASE_ADDRESS	0x10268000

#define Channel_A_DDRPHY_NAO_BASE_ADDRESS	0x10236000
#define Channel_B_DDRPHY_NAO_BASE_ADDRESS	0x10246000
#define Channel_C_DDRPHY_NAO_BASE_ADDRESS	0x10256000
#define Channel_D_DDRPHY_NAO_BASE_ADDRESS	0x10266000

#define Channel_A_DRAMC_NAO_BASE_ADDRESS    0x10234000
#define Channel_B_DRAMC_NAO_BASE_ADDRESS    0x10244000
#define Channel_C_DRAMC_NAO_BASE_ADDRESS    0x10254000
#define Channel_D_DRAMC_NAO_BASE_ADDRESS    0x10264000
#endif


//=============================================================================
//  Global Variables
//=============================================================================
static unsigned char auto_detect_done;
static int enable_combo_dis = 0;
static unsigned short mr5;
static unsigned short mr6;
static unsigned short mr7;
static unsigned short mr8;
extern DRAMC_CTX_T *psCurrDramCtx;
extern EMI_SETTINGS g_default_emi_setting;
#ifdef MTK_PMIC_MT6363
static struct mtk_regulator reg_vio18, reg_vdd2, reg_vcore, reg_vddq, reg_vmddr;
static struct mtk_regulator reg_vdd2H, reg_vdd2L;
#endif

#ifdef LAST_DRAMC
static LAST_DRAMC_INFO_T* last_dramc_info_ptr;
#endif

#ifdef VOLTAGE_SEL
static VOLTAGE_SEL_INFO_T voltage_sel_info_ptr;
#endif

#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
#if (!__ETT__)
#include <storage_api.h>
#endif
extern u64 get_part_addr(const char *name);
static int read_offline_dram_mdl_data(DRAM_INFO_BY_MRR_T *DramInfo);
static int write_offline_dram_mdl_data(DRAM_INFO_BY_MRR_T *DramInfo);
static u64 part_dram_data_addr = 0;
static unsigned int dram_offline_data_flags = 0;
#endif

#if defined(DRAM_SLT) && (!__ETT__)
#include <pl_version.h>
static u64 part_dram_data_addr_slt = 0;
int read_slt_data(DRAM_SLT_DATA_T *data);
int write_slt_data(DRAM_SLT_DATA_T *data);
int clean_slt_data(void);
#endif
//=============================================================================
//  External references
//=============================================================================
extern DRAMC_CTX_T *psCurrDramCtx;
extern char* opt_dle_value;

void print_DBG_info(DRAMC_CTX_T *p);
unsigned int mt_get_dram_type_from_hw_trap(void);

void MPLLInit(void)
{
#if DRAMC_CTRL_BY_SOC
#if 0//__A60868_TO_BE_PORTING__

#if (FOR_DV_SIMULATION_USED == 0)
	unsigned int tmp;

	DRV_WriteReg32(AP_PLL_CON0, 0x11); // CLKSQ Enable
	mcDELAY_US(100);
	DRV_WriteReg32(AP_PLL_CON0, 0x13); // CLKSQ LPF Enable
	mcDELAY_MS(1);
	DRV_WriteReg32(MPLL_PWR_CON0, 0x3); // power on MPLL
	mcDELAY_US(30);
	DRV_WriteReg32(MPLL_PWR_CON0, 0x1); // turn off ISO of  MPLL
	mcDELAY_US(1);
	tmp = DRV_Reg32(MPLL_CON1);
	DRV_WriteReg32(MPLL_CON1, tmp | 0x80000000); // Config MPLL freq
	DRV_WriteReg32(MPLL_CON0, 0x181); // enable MPLL
	mcDELAY_US(20);
#endif
#else
	unsigned int tmp;

	DRV_WriteReg32(MPLL_CON3, 0x3); // power on MPLL

	mcDELAY_US(30);

	tmp = DRV_Reg32(MPLL_CON3);
	DRV_WriteReg32(MPLL_CON3, tmp & 0xFFFFFFFD); // turn off ISO of  MPLL

	mcDELAY_US(1);

	DRV_WriteReg32(MPLL_CON1, 0x84200000); // Config MPLL freq

	tmp = DRV_Reg32(MPLL_CON0);
	DRV_WriteReg32(MPLL_CON0, tmp | 0x1); // enable MPLL

	mcDELAY_US(20);



#endif
#endif
}


#define CKGEN_FMETER 0x0
#define ABIST_FMETER 0x1
/*
1. Select meter clock input: CLK_DBG_CFG[1:0] = 0x0
2. Select clock source from below table: CLK_DBG_CFG[21:16] = 0x39
3. Setup meter div: CLK_MISC_CFG_0[31:24] = 0x0
4. Enable frequency meter: CLK26CALI_0[12] = 0x1
5. Trigger frequency meter: CLK26CALI_0[4] = 0x1
6. Wait until CLK26CALI_0[4] = 0x0
7. Read meter count: CLK26CALI_1[15:0]
8. Calculate measured frequency: freq. = (26 * cal_cnt) / 1024

DE: Mas Lin
*/
unsigned int FMeter(unsigned char u1CLKMeterSel, unsigned char u1CLKMuxSel) {
  unsigned int tmp      ;
  unsigned int u4CalCnt ;
  
  /*TINFO="\n[Fmeter] debug-0"*/
  // enable fmeter
  DRV_WriteReg32(CLK26CALI_0, (0x1 << 12)); // [12]=1: [8]=0: switch in_clk/clk27m
  
  // select meter
  tmp = DRV_Reg32(CLK_DBG_CFG);
  tmp &= ~0x3;
  
  if (u1CLKMeterSel == CKGEN_FMETER)
    tmp |= 0x1;
  
  DRV_WriteReg32(CLK_DBG_CFG, tmp);
  
  // select fmeter's input clock
  tmp = DRV_Reg32(CLK_DBG_CFG);
  
  if (u1CLKMeterSel == CKGEN_FMETER) {
    tmp &= ~(0x3FU << 8);
    tmp |=  u1CLKMuxSel << 8;
  } else {
    tmp &= ~(0x3FU << 16);
    tmp |=  u1CLKMuxSel << 16;
  }
  
  DRV_WriteReg32(CLK_DBG_CFG, tmp);
  
  // setup fmeter div
  tmp = DRV_Reg32(CLK_MISC_CFG_0);
  tmp &= ~(0xFFU << 24);
  DRV_WriteReg32(CLK_MISC_CFG_0, tmp);
  
  // set load_cnt to 0x3FF (1024 - 1)
  tmp = DRV_Reg32(CLK26CALI_1);
  tmp &= ~(0x3FFU << 16);
  tmp |= 0x3FFU << 16;
  DRV_WriteReg32(CLK26CALI_1, tmp);
  
  // trigger fmeter
  tmp = DRV_Reg32(CLK26CALI_0);
  tmp |= (0x1U << 4);
  DRV_WriteReg32(CLK26CALI_0, tmp);
  
  /*TINFO="\n[Fmeter] debug-1"*/
  // wait
  while (DRV_Reg32(CLK26CALI_0) & (0x1 << 4)) {
    mcDELAY_US(1);
  }
  
  /*TINFO="\n[Fmeter] debug-2"*/
  // read
  u4CalCnt = DRV_Reg32(CLK26CALI_1) & 0xFFFF;
  
  // disable fmeter
  tmp = DRV_Reg32(CLK26CALI_0);
  tmp &= ~(0x1U << 12);
  DRV_WriteReg32(CLK26CALI_0, tmp);
  
  return ((u4CalCnt * 26) / 1024);
}


#define ABIST_SEL_AD_MPLL           57
//! CH0
#define ABIST_SEL_CH0_MCK_BFE_DCM   27
#define ABIST_SEL_CH0_MCK_AFT_DCM   26
#define ABIST_SEL_CH0_AD_PHYPLL     25
#define ABIST_SEL_CH0_AD_CLRPLL     25
//! CH1
#define ABIST_SEL_CH1_MCK_BFE_DCM   19
#define ABIST_SEL_CH1_MCK_AFT_DCM   18
#define ABIST_SEL_CH1_AD_PHYPLL     17
#define ABIST_SEL_CH1_AD_CLRPLL     17
//! CH2
#define ABIST_SEL_CH2_MCK_BFE_DCM   23
#define ABIST_SEL_CH2_MCK_AFT_DCM   22
#define ABIST_SEL_CH2_AD_PHYPLL     21
#define ABIST_SEL_CH2_AD_CLRPLL     21
//! CH3
#define ABIST_SEL_CH3_MCK_BFE_DCM   15
#define ABIST_SEL_CH3_MCK_AFT_DCM   14
#define ABIST_SEL_CH3_AD_PHYPLL     13
#define ABIST_SEL_CH3_AD_CLRPLL     13

#define DPHY_MISC_CG_CTRL7_ADDR     0x684

#define DPHY_SHU_PHYPLL3_SHU0_ADDR  0x90c

#define DPHY_SHU_CLRPLL3_SHU0_ADDR  0x92c

unsigned int DDRPhyFreqMeter(DRAMC_CTX_T *p)
{
#if 1
#if (FOR_DV_SIMULATION_USED==0)

    unsigned int reg0=0, reg1=0;
    unsigned int before_value=0, after_value=0;
    unsigned int before_shift = 0, after_shift = 0;
    unsigned int meter_value=0;
    unsigned int frqValue = 0;

#if 1//def HJ_SIM
    unsigned int test_pass = 1                      ;
    unsigned int frq_result_208 = 0                 ;
    unsigned int frq_result_mem[3*CHANNEL_NUM]      ;
    unsigned int temp_wbr = 0;
    unsigned int temp_phypll3, temp_clrpll3;
    unsigned int temp_cg_ctrl7;

    unsigned int freq_meter_mpll_min = (208*0.95) ;
    unsigned int freq_meter_mpll_max = (208*1.05) ;
    unsigned int freq_meter_fmem_min = (459*0.95) ;
    unsigned int freq_meter_fmem_max = (459*1.05) ;
    /*TINFO="\n[PhyFreqMeter @DSIM]"*/

    //*DRAMC_WBR = 0x00027f7f ;  //! broadcast_on
    temp_wbr = DRV_Reg32(DRAMC_WBR);
    DRV_WriteReg32  (DRAMC_WBR, 0x00027f7f);  //DRAMC APB Broadcast on

    /*TINFO="\n[Enable PHYPLL and CLRPLL mon clock]"*/
    //! RG_RG_RPHYPLL_MONCK_EN_SHU0
    temp_phypll3 = DRV_Reg32(Channel_A_DDRPHY_AO_BASE_ADDRESS + DPHY_SHU_PHYPLL3_SHU0_ADDR) ;
    DRV_WriteReg32  (Channel_A_DDRPHY_AO_BASE_ADDRESS + DPHY_SHU_PHYPLL3_SHU0_ADDR  , temp_phypll3 | (1 << 16));
    temp_clrpll3 = DRV_Reg32(Channel_A_DDRPHY_AO_BASE_ADDRESS + DPHY_SHU_CLRPLL3_SHU0_ADDR) ;
    DRV_WriteReg32  (Channel_A_DDRPHY_AO_BASE_ADDRESS + DPHY_SHU_CLRPLL3_SHU0_ADDR  , temp_clrpll3 | (1 << 16));
    
    //! RG_ck_bfe_dcm_en
    temp_cg_ctrl7 = DRV_Reg32(Channel_A_DDRPHY_AO_BASE_ADDRESS + DPHY_MISC_CG_CTRL7_ADDR)    ;
    DRV_WriteReg32  (Channel_A_DDRPHY_AO_BASE_ADDRESS + DPHY_MISC_CG_CTRL7_ADDR     , temp_cg_ctrl7 | (1 << 11));

    // abist_clk: AD_MPLL_CK
    /*TINFO="[DDRPhyFreqMeter] start to test AD_MPLL_CK -> sel =%d\n", ABIST_SEL_AD_MPLL*/
    frq_result_208 = FMeter(ABIST_FMETER, ABIST_SEL_AD_MPLL) ;
	mcSHOW_DBG_MSG("ABIST_SEL_AD_MPLL = %d\n", frq_result_208);
    /*TINFO="[DDRPhyFreqMeter] AD_MPLL_CK FREQ=%d\n", frq_result_208*/

    //! CH0
    /*TINFO="\n[DdrphyFreqMeter] ABIST %d - MCK_BFE_DCM", ABIST_SEL_CH0_MCK_BFE_DCM*/
    frq_result_mem[0] = FMeter(ABIST_FMETER, ABIST_SEL_CH0_MCK_BFE_DCM) ;
	mcSHOW_DBG_MSG("ABIST_SEL_CH0_MCK_BFE_DCM = %d\n", frq_result_mem[0]);
    /*TINFO="[DdrphyFreqMeter] MCK_BFE_DCM freq_meter=%d\n", frq_result_mem[0]*/
    
    /*TINFO="\n[DdrphyFreqMeter] ABIST %d - ABIST_SEL_CH0_AD_PHYPLL", ABIST_SEL_CH0_AD_PHYPLL*/
    frq_result_mem[1] = FMeter(ABIST_FMETER, ABIST_SEL_CH0_AD_PHYPLL) ;
	mcSHOW_DBG_MSG("ABIST_SEL_CH0_AD_PHYPLL = %d\n", frq_result_mem[1]);
    /*TINFO="[DdrphyFreqMeter] ABIST_SEL_CH0_AD_PHYPLL freq_meter=%d\n", frq_result_mem[1]*/
    
    /*TINFO="\n[DdrphyFreqMeter] ABIST %d - ABIST_SEL_CH0_AD_CLRPLL", ABIST_SEL_CH0_AD_CLRPLL*/
    frq_result_mem[2] = FMeter(ABIST_FMETER, ABIST_SEL_CH0_AD_CLRPLL) ;
	mcSHOW_DBG_MSG("ABIST_SEL_CH0_AD_CLRPLL = %d\n", frq_result_mem[2]);
    /*TINFO="[DdrphyFreqMeter] ABIST_SEL_CH0_AD_CLRPLL freq_meter=%d\n", frq_result_mem[2]*/

    if( (frq_result_208     < freq_meter_mpll_min) || (frq_result_208     > freq_meter_mpll_max) ||
        (frq_result_mem[0]  < freq_meter_fmem_min) || (frq_result_mem[0]  > freq_meter_fmem_max) ||
        (frq_result_mem[1]  < freq_meter_fmem_min) || (frq_result_mem[1]  > freq_meter_fmem_max) ||
        (frq_result_mem[2]  < freq_meter_fmem_min) || (frq_result_mem[2]  > freq_meter_fmem_max) )
    {
      /*TINFO="\n[DdrphyFreqMeter] CH0 DDRPHY FreqMeter Error ...*/
      //FPC_GPIO_FLAG__PAT_DBG_0
      test_pass = 0 ;
    }

    //! CH1
    /*TINFO="\n[DdrphyFreqMeter] ABIST %d - MCK_BFE_DCM", ABIST_SEL_CH1_MCK_BFE_DCM*/
    frq_result_mem[3] = FMeter(ABIST_FMETER, ABIST_SEL_CH1_MCK_BFE_DCM) ;
	mcSHOW_DBG_MSG("ABIST_SEL_CH1_MCK_BFE_DCM = %d\n", frq_result_mem[3]);
    /*TINFO="[DdrphyFreqMeter] MCK_BFE_DCM freq_meter=%d\n", frq_result_mem[3]*/
    
    /*TINFO="\n[DdrphyFreqMeter] ABIST %d - ABIST_SEL_CH1_AD_PHYPLL", ABIST_SEL_CH1_AD_PHYPLL*/
    frq_result_mem[4] = FMeter(ABIST_FMETER, ABIST_SEL_CH1_AD_PHYPLL) ;
	mcSHOW_DBG_MSG("ABIST_SEL_CH1_AD_PHYPLL = %d\n", frq_result_mem[4]);
    /*TINFO="[DdrphyFreqMeter] ABIST_SEL_CH1_AD_PHYPLL freq_meter=%d\n", frq_result_mem[4]*/
    
    /*TINFO="\n[DdrphyFreqMeter] ABIST %d - ABIST_SEL_CH1_AD_CLRPLL", ABIST_SEL_CH1_AD_CLRPLL*/
    frq_result_mem[5] = FMeter(ABIST_FMETER, ABIST_SEL_CH1_AD_CLRPLL) ;
	mcSHOW_DBG_MSG("ABIST_SEL_CH1_AD_CLRPLL = %d\n", frq_result_mem[5]);
    /*TINFO="[DdrphyFreqMeter] ABIST_SEL_CH1_AD_CLRPLL freq_meter=%d\n", frq_result_mem[5]*/


    if( (frq_result_mem[3]  < freq_meter_fmem_min) || (frq_result_mem[3]  > freq_meter_fmem_max) ||
        (frq_result_mem[4]  < freq_meter_fmem_min) || (frq_result_mem[4]  > freq_meter_fmem_max) ||
        (frq_result_mem[5]  < freq_meter_fmem_min) || (frq_result_mem[5]  > freq_meter_fmem_max) )
    {
      /*TINFO="\n[DdrphyFreqMeter] CH1 DDRPHY FreqMeter Error ...*/
      //FPC_GPIO_FLAG__PAT_DBG_1
      test_pass = 0 ;
    }

  #if (CHANNEL_NUM > 2)
    //! CH2
    /*TINFO="\n[DdrphyFreqMeter] ABIST %d - MCK_BFE_DCM", ABIST_SEL_CH2_MCK_BFE_DCM*/
    frq_result_mem[6] = FMeter(ABIST_FMETER, ABIST_SEL_CH2_MCK_BFE_DCM) ;
	mcSHOW_DBG_MSG("ABIST_SEL_CH2_MCK_BFE_DCM = %d\n", frq_result_mem[6]);
    /*TINFO="[DdrphyFreqMeter] MCK_BFE_DCM freq_meter=%d\n", frq_result_mem[6]*/
    
    /*TINFO="\n[DdrphyFreqMeter] ABIST %d - ABIST_SEL_CH2_AD_PHYPLL", ABIST_SEL_CH2_AD_PHYPLL*/
    frq_result_mem[7] = FMeter(ABIST_FMETER, ABIST_SEL_CH2_AD_PHYPLL) ;
	mcSHOW_DBG_MSG("ABIST_SEL_CH2_AD_PHYPLL = %d\n", frq_result_mem[7]);
    /*TINFO="[DdrphyFreqMeter] ABIST_SEL_CH2_AD_PHYPLL freq_meter=%d\n", frq_result_mem[7]*/
    
    /*TINFO="\n[DdrphyFreqMeter] ABIST %d - ABIST_SEL_CH2_AD_CLRPLL", ABIST_SEL_CH2_AD_CLRPLL*/
    frq_result_mem[8] = FMeter(ABIST_FMETER, ABIST_SEL_CH2_AD_CLRPLL) ;
	mcSHOW_DBG_MSG("ABIST_SEL_CH2_AD_CLRPLL = %d\n", frq_result_mem[8]);
    /*TINFO="[DdrphyFreqMeter] ABIST_SEL_CH2_AD_CLRPLL freq_meter=%d\n", frq_result_mem[8]*/


    if( (frq_result_mem[6]  < freq_meter_fmem_min) || (frq_result_mem[6]  > freq_meter_fmem_max) ||
        (frq_result_mem[7]  < freq_meter_fmem_min) || (frq_result_mem[7]  > freq_meter_fmem_max) ||
        (frq_result_mem[8]  < freq_meter_fmem_min) || (frq_result_mem[8]  > freq_meter_fmem_max) )
    {
      /*TINFO="\n[DdrphyFreqMeter] CH2 DDRPHY FreqMeter Error ...*/
      //FPC_GPIO_FLAG__PAT_DBG_2
      test_pass = 0 ;
    }

    //! CH3
    /*TINFO="\n[DdrphyFreqMeter] ABIST %d - MCK_BFE_DCM", ABIST_SEL_CH3_MCK_BFE_DCM*/
    frq_result_mem[9] = FMeter(ABIST_FMETER, ABIST_SEL_CH3_MCK_BFE_DCM) ;
	mcSHOW_DBG_MSG("ABIST_SEL_CH3_MCK_BFE_DCM = %d\n", frq_result_mem[9]);
    /*TINFO="[DdrphyFreqMeter] MCK_BFE_DCM freq_meter=%d\n", frq_result_mem[9]*/
    
    /*TINFO="\n[DdrphyFreqMeter] ABIST %d - ABIST_SEL_CH3_AD_PHYPLL", ABIST_SEL_CH3_AD_PHYPLL*/
    frq_result_mem[10]= FMeter(ABIST_FMETER, ABIST_SEL_CH3_AD_PHYPLL) ;
	mcSHOW_DBG_MSG("ABIST_SEL_CH3_AD_PHYPLL = %d\n", frq_result_mem[10]);
    /*TINFO="[DdrphyFreqMeter] ABIST_SEL_CH3_AD_PHYPLL freq_meter=%d\n", frq_result_mem[10]*/
    
    /*TINFO="\n[DdrphyFreqMeter] ABIST %d - ABIST_SEL_CH3_AD_CLRPLL", ABIST_SEL_CH3_AD_CLRPLL*/
    frq_result_mem[11]= FMeter(ABIST_FMETER, ABIST_SEL_CH3_AD_CLRPLL) ;
	mcSHOW_DBG_MSG("ABIST_SEL_CH3_AD_CLRPLL = %d\n", frq_result_mem[11]);
    /*TINFO="[DdrphyFreqMeter] ABIST_SEL_CH3_AD_CLRPLL freq_meter=%d\n", frq_result_mem[11]*/


    if( (frq_result_mem[ 9] < freq_meter_fmem_min) || (frq_result_mem[ 9] > freq_meter_fmem_max) ||
        (frq_result_mem[10] < freq_meter_fmem_min) || (frq_result_mem[10] > freq_meter_fmem_max) ||
        (frq_result_mem[11] < freq_meter_fmem_min) || (frq_result_mem[11] > freq_meter_fmem_max) )
    {
      /*TINFO="\n[DdrphyFreqMeter] CH3 DDRPHY FreqMeter Error ...*/
      //FPC_GPIO_FLAG__PAT_DBG_3
      test_pass = 0 ;
    }

  #endif

    switch (p->channel) {
        case CHANNEL_A:
            before_shift = ABIST_SEL_CH0_MCK_BFE_DCM;
            after_shift = ABIST_SEL_CH0_MCK_AFT_DCM;
            break;
        case CHANNEL_B:
            before_shift = ABIST_SEL_CH1_MCK_BFE_DCM;
            after_shift = ABIST_SEL_CH1_MCK_AFT_DCM;
            break;
        case CHANNEL_C:
            before_shift = ABIST_SEL_CH2_MCK_BFE_DCM;
            after_shift = ABIST_SEL_CH2_MCK_AFT_DCM;
            break;
        case CHANNEL_D:
            before_shift = ABIST_SEL_CH3_MCK_BFE_DCM;
            after_shift = ABIST_SEL_CH3_MCK_AFT_DCM;
            break;
        default:
            dramc_crit("Input the invalid channel\n");
            break;
    }
    // abistgen_clk46: fmem_ck_aft_dcm_ch2 (DRAMC CHC's clock after idle mask)
    before_value = FMeter(ABIST_FMETER, before_shift);
    dramc_crit("fmem_ck_aft_dcm_ch2 FREQ=%d\n", before_value);
    /*TINFO="fmem_ck_aft_dcm_ch2 FREQ=%d\n", after_value*/

    // abistgen_clk47: fmem_ck_aft_dcm_ch3 (DRAMC CHC's clock after idle mask)
    after_value = FMeter(ABIST_FMETER, after_shift);
    dramc_crit("fmem_ck_aft_dcm_ch3 FREQ=%d\n", after_value);
    /*TINFO="fmem_ck_aft_dcm_ch3 FREQ=%d\n", after_value*/
    
    meter_value = (before_value<<16 | after_value);
    DRV_WriteReg32  (Channel_A_DDRPHY_AO_BASE_ADDRESS + DPHY_MISC_CG_CTRL7_ADDR     , temp_cg_ctrl7);
    DRV_WriteReg32  (Channel_A_DDRPHY_AO_BASE_ADDRESS + DPHY_SHU_PHYPLL3_SHU0_ADDR  , temp_phypll3);
    DRV_WriteReg32  (Channel_A_DDRPHY_AO_BASE_ADDRESS + DPHY_SHU_CLRPLL3_SHU0_ADDR  , temp_clrpll3);

    DRV_WriteReg32  (DRAMC_WBR, temp_wbr);  //DRAMC APB Broadcast on

    if(vGet_Div_Mode(p) == DIV8_MODE)
    {
        frqValue = (meter_value & 0xFFFF) << 2;
    }
    else if(vGet_Div_Mode(p) == DIV4_MODE)
    {
        frqValue = (meter_value & 0xFFFF) << 1;
    }
    else if(vGet_Div_Mode(p) == DIV16_MODE)
    {
        frqValue = (meter_value & 0xFFFF) << 3;
    }
    else
    {
        mcSHOW_ERR_MSG("[DDRPhyFreqMeter] Get DIV mode error!\n");
    #if __ETT__
        while (1);
    #endif
    }
    return frqValue;

#else
    mcSHOW_DBG_MSG3("\n[PhyFreqMeter]\n");
    // abist_clk29: AD_MPLL_CK
    mcSHOW_DBG_MSG("AD_MPLL_CK FREQ=%d\n", FMeter(ABIST_FMETER, 29));

    // abist_clk31: AD_RCLRPLL_DIV4_CK_ch02
    mcSHOW_DBG_MSG("AD_RCLRPLL_DIV4_CK_ch02 FREQ=%d\n", FMeter(ABIST_FMETER, 31));

    // abist_clk32: AD_RCLRPLL_DIV4_CK_ch13
    mcSHOW_DBG_MSG("AD_RCLRPLL_DIV4_CK_ch13 FREQ=%d\n", FMeter(ABIST_FMETER, 32));

    // abist_clk33: AD_RPHYRPLL_DIV4_CK_ch02
    mcSHOW_DBG_MSG("AD_RPHYRPLL_DIV4_CK_ch02 FREQ=%d\n", FMeter(ABIST_FMETER, 33));

    // abist_clk34: AD_RPHYRPLL_DIV4_CK_ch13
    mcSHOW_DBG_MSG("AD_RPHYRPLL_DIV4_CK_ch13 FREQ=%d\n", FMeter(ABIST_FMETER, 34));

    // enable ck_bfe_dcm_en for freqmeter measure ddrphy clock, not needed for normal use
    reg0 = DRV_Reg32(Channel_A_DDRPHY_AO_BASE_ADDRESS + 0x504) ;
    DRV_WriteReg32  (Channel_A_DDRPHY_AO_BASE_ADDRESS + 0x504  , reg0 | (1 << 11));

    reg0 = DRV_Reg32(Channel_B_DDRPHY_AO_BASE_ADDRESS + 0x504) ;
    DRV_WriteReg32  (Channel_B_DDRPHY_AO_BASE_ADDRESS + 0x504  , reg0 | (1 << 11));

    reg0 = DRV_Reg32(Channel_C_DDRPHY_AO_BASE_ADDRESS + 0x504) ;
    DRV_WriteReg32  (Channel_C_DDRPHY_AO_BASE_ADDRESS + 0x504  , reg0 | (1 << 11));

    reg0 = DRV_Reg32(Channel_D_DDRPHY_AO_BASE_ADDRESS + 0x504) ;
    DRV_WriteReg32  (Channel_D_DDRPHY_AO_BASE_ADDRESS + 0x504  , reg0 | (1 << 11));

    // abistgen_clk44: fmem_ck_bfe_dcm_ch0 (DRAMC CHA's clock before idle mask)
    before_value = FMeter(ABIST_FMETER, 44);
    mcSHOW_DBG_MSG("fmem_ck_bfe_dcm_ch0 FREQ=%d\n", FMeter(ABIST_FMETER, 44));

    // abistgen_clk45: fmem_ck_bfe_dcm_ch1 (DRAMC CHB's clock before idle mask)
    before_value = FMeter(ABIST_FMETER, 45);
    mcSHOW_DBG_MSG("fmem_ck_bfe_dcm_ch1 FREQ=%d\n", FMeter(ABIST_FMETER, 45));

    // abistgen_clk46: fmem_ck_bfe_dcm_ch2 (DRAMC CHC's clock before idle mask)
    before_value = FMeter(ABIST_FMETER, 46);
    mcSHOW_DBG_MSG("fmem_ck_bfe_dcm_ch2 FREQ=%d\n", FMeter(ABIST_FMETER, 46));

    // abistgen_clk47: fmem_ck_bfe_dcm_ch3 (DRAMC CHC's clock before idle mask)
    before_value = FMeter(ABIST_FMETER, 47);
    mcSHOW_DBG_MSG("fmem_ck_bfe_dcm_ch3 FREQ=%d\n", FMeter(ABIST_FMETER, 47));

    return 1;
#if 0
    DRV_WriteReg32(DRAMC_CH0_TOP0_BASE + 0x2a0, reg0);
    DRV_WriteReg32(DRAMC_CH1_TOP0_BASE + 0x2a0, reg1);

    return ((before_value<<16) | after_value);
#endif

    if(vGet_Div_Mode(p) == DIV8_MODE)
    {
        frqValue = (meter_value & 0xFFFF) << 2;
    }
    else if(vGet_Div_Mode(p) == DIV4_MODE)
    {
        frqValue = (meter_value & 0xFFFF) << 1;
    }
    else if(vGet_Div_Mode(p) == DIV16_MODE)
    {
        frqValue = (meter_value & 0xFFFF) << 3;
    }
    else
    {
        mcSHOW_ERR_MSG("[DDRPhyFreqMeter] Get DIV mode error!\n");
    #if __ETT__
        while (1);
    #endif
    }
    return frqValue;

#endif //! end DSIM HJ
#endif // (FOR_DV_SIMULATION_USED==0) 
#endif
}


#if ENABLE_PINMUX_FOR_RANK_SWAP
static void EMI_rank_swap_emi_setting(EMI_SETTINGS *emi_set)
{
	u32 temp = 0;
	u64 temp64 = 0;

	if (emi_set->EMI_CONA_VAL & 0x20000) {
		temp = emi_set->EMI_CONA_VAL;
		emi_set->EMI_CONA_VAL &= ~(0xF3F0F0F0);
		emi_set->EMI_CONA_VAL |= (temp & 0xC0C0C0C0) >> 2;
		emi_set->EMI_CONA_VAL |= (temp & 0x30303030) << 2;
		emi_set->EMI_CONA_VAL |= (temp & 0x02000000) >> 1;
		emi_set->EMI_CONA_VAL |= (temp & 0x01000000) << 1;

		temp = emi_set->EMI_CONH_VAL;
		emi_set->EMI_CONH_VAL &= ~(0xFFFF0030);
		emi_set->EMI_CONH_VAL |= (temp & 0xF0F00000) >> 4;
		emi_set->EMI_CONH_VAL |= (temp & 0x0F0F0000) << 4;
		emi_set->EMI_CONH_VAL |= (temp & 0x00000020) >> 1;
		emi_set->EMI_CONH_VAL |= (temp & 0x00000010) << 1;

		temp = emi_set->CHN0_EMI_CONA_VAL;
		emi_set->CHN0_EMI_CONA_VAL &= ~(0x00FFF0FC);
		emi_set->CHN0_EMI_CONA_VAL |= (temp & 0x00F00000) >> 4;
		emi_set->CHN0_EMI_CONA_VAL |= (temp & 0x000F0000) << 4;
		emi_set->CHN0_EMI_CONA_VAL |= (temp & 0x0000C0C0) >> 2;
		emi_set->CHN0_EMI_CONA_VAL |= (temp & 0x00003030) << 2;
		emi_set->CHN0_EMI_CONA_VAL |= (temp & 0x00000008) >> 1;
		emi_set->CHN0_EMI_CONA_VAL |= (temp & 0x00000004) << 1;

		temp = emi_set->CHN1_EMI_CONA_VAL;
		emi_set->CHN1_EMI_CONA_VAL &= ~(0x00FFF0FC);
		emi_set->CHN1_EMI_CONA_VAL |= (temp & 0x00F00000) >> 4;
		emi_set->CHN1_EMI_CONA_VAL |= (temp & 0x000F0000) << 4;
		emi_set->CHN1_EMI_CONA_VAL |= (temp & 0x0000C0C0) >> 2;
		emi_set->CHN1_EMI_CONA_VAL |= (temp & 0x00003030) << 2;
		emi_set->CHN1_EMI_CONA_VAL |= (temp & 0x00000008) >> 1;
		emi_set->CHN1_EMI_CONA_VAL |= (temp & 0x00000004) << 1;

		emi_set->CHN0_EMI_CONA_VAL |= 0x80000000;
		emi_set->CHN1_EMI_CONA_VAL |= 0x80000000;

		temp64 = emi_set->DRAM_RANK_SIZE[0];
		emi_set->DRAM_RANK_SIZE[0] = emi_set->DRAM_RANK_SIZE[1];
		emi_set->DRAM_RANK_SIZE[1] = temp64;

		if (emi_set->dram_cbt_mode_extern == CBT_R0_NORMAL_R1_BYTE)
			emi_set->dram_cbt_mode_extern = CBT_R0_BYTE_R1_NORMAL;
		else if (emi_set->dram_cbt_mode_extern == CBT_R0_BYTE_R1_NORMAL)
			emi_set->dram_cbt_mode_extern = CBT_R0_NORMAL_R1_BYTE;
	}
}

void EMI_rank_swap_handle(void)
{
	static unsigned int handled = 0;
	int i;

	if (!handled) {
		EMI_rank_swap_emi_setting(&g_default_emi_setting);
		handled = 1;
	}
}
#endif

void mdl_setting(DRAMC_CTX_T *p)
{
	EMI_SETTINGS *emi_set = &g_default_emi_setting;
	
	emi_init();
	mt_emi_sync_write(INFRACFG_AO_BASE+0x000000b4, 0x000f7f7f); // enable broadcast for 2+2CH
	//The following is MDL settings
	set_cen_emi_cona(emi_set->EMI_CONA_VAL);
	set_cen_emi_conf(emi_set->EMI_CONF_VAL);
	set_cen_emi_conh(emi_set->EMI_CONH_VAL);
	set_cen_emi_conh_2nd(emi_set->EMI_CONH_2ND_VAL | 0x10);

	// CHNA and CHNB uses the same CH0 setting
	set_chn_emi_cona(emi_set->CHN0_EMI_CONA_VAL);
	set_chn_emi_conc(emi_set->CHN0_EMI_CONC_VAL);
	//set_chn_emi_conc(0x4);

	p->vendor_id = emi_set->iLPDDR3_MODE_REG_5;
	mt_reg_sync_write(INFRACFG_AO_BASE+0x000000b4,0x00000000);
}

unsigned int check_gating_error(void)
{
	unsigned int ret = 0, i, err_code = 0;
    PTR_T phy_base;
	unsigned int misc_stberr_all, misc_stberr_rk0_r, misc_stberr_rk0_f, misc_stberr_rk1_r, misc_stberr_rk1_f;

	phy_base = Channel_A_DDRPHY_AO_BASE_ADDRESS;

	for (i = 0; i < CHANNEL_NUM; ++i, phy_base += 0x10000, err_code = 0) {
		misc_stberr_all   = *(volatile unsigned int*)(phy_base + 0x1500);
		misc_stberr_rk0_r = *(volatile unsigned int*)(phy_base + 0x1504);
		misc_stberr_rk0_f = *(volatile unsigned int*)(phy_base + 0x1508);
		misc_stberr_rk1_r = *(volatile unsigned int*)(phy_base + 0x150c);
		misc_stberr_rk1_f = *(volatile unsigned int*)(phy_base + 0x1510);

		if (misc_stberr_all & (1 << 16)) {
			ret |= (1 << i);
#ifdef LAST_DRAMC
			if ((misc_stberr_rk0_r & 0xffff) != 0) {
				err_code |= ERR_DRAM_GATING_RK0_R;
			}
			if ((misc_stberr_rk0_f & 0xffff) != 0) {
				err_code |= ERR_DRAM_GATING_RK0_F;
			}
			if ((misc_stberr_rk1_r & 0xffff) != 0) {
				err_code |= ERR_DRAM_GATING_RK1_R;
			}
			if ((misc_stberr_rk1_f & 0xffff) != 0) {
				err_code |= ERR_DRAM_GATING_RK1_F;
			}
			dram_fatal_set_gating_err(i, err_code);
			dram_fatal_set_stberr(i, 0, (misc_stberr_rk0_r & 0xffff) | ((misc_stberr_rk0_f & 0xffff) << 16));
			dram_fatal_set_stberr(i, 1, (misc_stberr_rk1_r & 0xffff) | ((misc_stberr_rk1_f & 0xffff) << 16));
		} else {
			dram_fatal_set_gating_err(i, 0);
			dram_fatal_set_stberr(i, 0, 0);
			dram_fatal_set_stberr(i, 1, 0);
#endif
		}
	}

	return ret;
}

void print_DBG_info(DRAMC_CTX_T *p)
{
#ifndef OLYMPUS_TO_BE_PORTING
	unsigned int addr = 0x0;
	U32 u4value;

#ifdef DDR_INIT_TIME_PROFILING
	return;
#endif

	mcSHOW_DBG_MSG("EMI_CONA=%x\n", get_cen_emi_cona());
	mcSHOW_DBG_MSG("EMI_CONH=%x\n", get_cen_emi_conh());

	//RISCReadAll();
#endif
}

unsigned int mt_get_dram_type(void)
{
	return mt_get_dram_type_from_hw_trap();
}

int mt_get_freq_setting(DRAMC_CTX_T *p)
{
	return p->frequency;
}

#ifdef DDR_RESERVE_MODE
extern u32 g_ddr_reserve_enable;
extern u32 g_ddr_reserve_success;
#define TIMEOUT 3
extern void before_Dramc_DDR_Reserved_Mode_setting(void);

#define	CHAN_DRAMC_NAO_MISC_STATUSA(base)	(base + 0x80)
#define SREF_STATE				(1 << 16)

static unsigned int is_dramc_exit_slf(void)
{
	unsigned int ret;

	ret = *(volatile unsigned *)CHAN_DRAMC_NAO_MISC_STATUSA(Channel_A_DRAMC_NAO_BASE_ADDRESS);
	if ((ret & SREF_STATE) != 0) {
		dramc_crit("DRAM CHAN-A is in self-refresh (MISC_STATUSA = 0x%x)\n", ret);
		return 0;
	}

	ret = *(volatile unsigned *)CHAN_DRAMC_NAO_MISC_STATUSA(Channel_B_DRAMC_NAO_BASE_ADDRESS);
	if ((ret & SREF_STATE) != 0) {
		dramc_crit("DRAM CHAN-B is in self-refresh (MISC_STATUSA = 0x%x)\n", ret);
		return 0;
	}
	ret = *(volatile unsigned *)CHAN_DRAMC_NAO_MISC_STATUSA(Channel_C_DRAMC_NAO_BASE_ADDRESS);
	if ((ret & SREF_STATE) != 0) {
		dramc_crit("DRAM CHAN-C is in self-refresh (MISC_STATUSA = 0x%x)\n", ret);
		return 0;
	}

	ret = *(volatile unsigned *)CHAN_DRAMC_NAO_MISC_STATUSA(Channel_D_DRAMC_NAO_BASE_ADDRESS);
	if ((ret & SREF_STATE) != 0) {
		dramc_crit("DRAM CHAN-D is in self-refresh (MISC_STATUSA = 0x%x)\n", ret);
		return 0;
	}

	dramc_crit("ALL DRAM CHAN is not in self-refresh\n");
	return 1;
}

#endif

unsigned int dramc_set_vcore_voltage(unsigned int vcore)
{
#ifdef MTK_PMIC_MT6363
	return mtk_regulator_set_voltage(&reg_vcore, vcore, MAX_VCORE);
#endif
	return 0;
}

unsigned int dramc_get_vcore_voltage(void)
{
#ifdef MTK_PMIC_MT6363
	return mtk_regulator_get_voltage(&reg_vcore);
#else
	return 0;
#endif
}

unsigned int dramc_set_vdd2_voltage(unsigned int vdram)
{
#ifdef MTK_PMIC_MT6363
	mtk_regulator_set_voltage(&reg_vdd2, vdram, MAX_VDD2);
#endif
	return 0;
}

unsigned int dramc_get_vdd2_voltage(void)
{
#ifdef MTK_PMIC_MT6363
	return mtk_regulator_get_voltage(&reg_vdd2);
#else
	return 0;
#endif
}

unsigned int dramc_set_vddq_voltage(unsigned int vddq)
{
#ifdef MTK_PMIC_MT6363
	mtk_regulator_set_voltage(&reg_vddq, vddq, MAX_VDDQ);
#endif
	return 0;
}

unsigned int dramc_get_vddq_voltage(void)
{
#ifdef MTK_PMIC_MT6363
	return mtk_regulator_get_voltage(&reg_vddq);
#else
	return 0;
#endif
}

unsigned int dramc_set_vmddr_voltage(unsigned int vmddr)
{
#ifdef MTK_PMIC_MT6363
	return mtk_regulator_set_voltage(&reg_vmddr, vmddr, MAX_VMDDR);
#endif
	return 0;
}

unsigned int dramc_vmddr_enable(unsigned int enable)
{
#ifdef MTK_PMIC_MT6363
	mtk_regulator_enable(&reg_vmddr, enable);
#endif
	return 0;
}

unsigned int dramc_get_vmddr_voltage(void)
{
#ifdef MTK_PMIC_MT6363
	return mtk_regulator_get_voltage(&reg_vmddr);
#else
	return 0;
#endif
}

unsigned int dramc_set_vio18_voltage(unsigned int vio18)
{
#ifdef MTK_PMIC_MT6363
	if (vio18 > 1900000) {
		upmu_set_reg_value16(PMIC_TMA_KEY_L_ADDR, 0x9C9C);
		pmic_config_interface(PMIC_RG_BUCK_VS1_VOSEL_ADDR, 0xA8, PMIC_RG_BUCK_VS1_VOSEL_MASK, PMIC_RG_BUCK_VS1_VOSEL_SHIFT);
		pmic_config_interface(PMIC_RG_VM18_VOTRIM_ADDR, 0x8, PMIC_RG_VM18_VOTRIM_MASK, PMIC_RG_VM18_VOTRIM_SHIFT);
		upmu_set_reg_value16(PMIC_TMA_KEY_L_ADDR, 0);
		vio18 = 1900000;
	}
	return mtk_regulator_set_voltage(&reg_vio18, vio18, MAX_VIO18);
#else
	return 0;
#endif
}


unsigned int dramc_get_vio18_voltage(void)
{
#ifdef MTK_PMIC_MT6363
	return mtk_regulator_get_voltage(&reg_vio18);
#else
	return 0;
#endif
}

unsigned int dramc_set_vdd2H_voltage(unsigned int vdd2H)
{
#ifdef MTK_PMIC_MT6363
	return mtk_regulator_set_voltage(&reg_vdd2H, vdd2H, MAX_VDD2H);
#endif
	return 0;
}

unsigned int dramc_get_vdd2H_voltage(void)
{
#ifdef MTK_PMIC_MT6363
	return mtk_regulator_get_voltage(&reg_vdd2H);
#else
	return 0;
#endif
}

unsigned int dramc_set_vdd2L_voltage(unsigned int vdd2L)
{
#ifdef MTK_PMIC_MT6363
	return mtk_regulator_set_voltage(&reg_vdd2L, vdd2L, MAX_VDD2L);
#endif
	return 0;
}

unsigned int dramc_get_vdd2L_voltage(void)
{
#ifdef MTK_PMIC_MT6363
	return mtk_regulator_get_voltage(&reg_vdd2L);
#else
	return 0;
#endif
}

#define GPIO_TRAPPING_REG (0x100056f0)
unsigned int mt_get_ddr_gpio_trap_value(void)
{
	static unsigned int trap_detected_done = 0;
	static unsigned int trap_value = 0;

	if (!trap_detected_done) {
		/* bit 20 latch GPIO53, bit19 latch GPIO54  */
		trap_value = (*((volatile unsigned int *)GPIO_TRAPPING_REG) >> 14) & 0x1;
		dramc_debug("%s : %d \n", __func__, trap_value);
		trap_detected_done = 1;
	}
	return trap_value;
}

unsigned int is_discrete_lpddr4(void)
{
	unsigned int trap_value;
	trap_value = mt_get_ddr_gpio_trap_value();
	if (trap_value == 0x1)
		return 1;
	else
		return 0;
}

unsigned int is_highest_freq_3733_lpddr4(void)
{
	unsigned int trap_value;
	trap_value = mt_get_ddr_gpio_trap_value();
	if (trap_value == 0x3)
		return 1;
	else
		return 0;
}

unsigned int get_dram_type()
{
	return 0;
}

unsigned int mt_get_dram_type_from_hw_trap(void)
{
	static unsigned int ddr_type_detected_done = 0;
	static unsigned int ddr_trap_type = 0;
	unsigned int trap, gpio_trap_value, efuse_value;

	if (!ddr_type_detected_done) {
		gpio_trap_value = mt_get_ddr_gpio_trap_value();
		switch (gpio_trap_value) {
			case 0:
				ddr_trap_type = TYPE_LPDDR5X;
				break;
			case 1:
				ddr_trap_type = TYPE_LPDDR5;
				break;
			default:
				dramc_crit("[dramc] Wrong GPIO Trapping.\n");
				DRAMC_ASSERT(0);
				break;
		}
#if defined(DRAM_SLT)
		if (mt_get_chip_sw_ver() == CHIP_VER_E1) {
			#if __ETT__
			efuse_value = (get_chip_sec_devinfo_efuse_type(54) >> 28) & 3;
			#else
			efuse_value = (seclib_get_devinfo_with_index(54) >> 28) & 3;
			#endif
			if (efuse_value == 0)
				ddr_trap_type = TYPE_LPDDR5;
			else if (efuse_value == 1) {
				if (ddr_trap_type == TYPE_LPDDR5) {
					dramc_crit("[dramc] TYPE_LPDDR5 use 7500 data rate\n");
					DRAMC_ASSERT(0);
				}
			} else if (efuse_value == 2) {
				ddr_trap_type = TYPE_LPDDR5;
			} else {
				dramc_crit("[dramc] Efuse value Error\n");
				DRAMC_ASSERT(0);
			}
		}
#endif
		ddr_type_detected_done = 1;
	}

	return ddr_trap_type;
}

void update_highest_dram_freq(void)
{
	if (is_highest_freq_3733_lpddr4())
		g_default_emi_setting.highest_freq = 3733;
}

void init_dram_regulator(void)
{
#ifdef MTK_PMIC_MT6363
	int ret;

	ret = mtk_regulator_get(VDD1_REGULATOR_NAME, &reg_vio18);
	if (ret)
		dramc_debug("mtk_regulator_get vio18 fail\n");

	ret = mtk_regulator_get(VCORE_REGULATOR_NAME, &reg_vcore);
	if (ret)
		dramc_debug("mtk_regulator_get vcore fail\n");

	if (is_second_pmic_pp_swap())
	{
		ret = mtk_regulator_get(VMDDR_DVS_REGULATOR_NAME, &reg_vmddr);
	}
	else
	{
		ret = mtk_regulator_get(VMDDR_REGULATOR_NAME, &reg_vmddr);
	}

	if (ret)
		dramc_debug("mtk_regulator_get vmddr fail\n");

	if (mt_get_dram_type() == TYPE_LPDDR5X) {
		ret = mtk_regulator_get(VDD2H_LP5X_REGULATOR_NAME, &reg_vdd2H);
		if (ret)
			dramc_debug("mtk_regulator_get LP5X vdd2H fail\n");

		ret = mtk_regulator_get(VDD2L_LP5X_REGULATOR_NAME, &reg_vdd2L);
		if (ret)
			dramc_debug("mtk_regulator_get LP5X vdd2L fail\n");

		ret = mtk_regulator_get(VDDQ_LP5X_REGULATOR_NAME, &reg_vddq);
		if (ret)
			dramc_debug("mtk_regulator_get LP5X vddq fail\n");
	} else {
		ret = mtk_regulator_get(VDD2H_LP5_REGULATOR_NAME, &reg_vdd2H);
		if (ret)
			dramc_debug("mtk_regulator_get vdd2H fail\n");

		ret = mtk_regulator_get(VDD2L_LP5_REGULATOR_NAME, &reg_vdd2L);
		if (ret)
			dramc_debug("mtk_regulator_get vdd2L fail\n");

		ret = mtk_regulator_get(VDDQ_LP5_REGULATOR_NAME, &reg_vddq);
		if (ret)
			dramc_debug("mtk_regulator_get LP5 vddq fail\n");
	}
#endif
}

void switch_dramc_voltage_to_auto_mode(void)
{
#ifdef MTK_PMIC_MT6363
	mtk_regulator_set_mode(&reg_vcore, 0x0);
	mtk_regulator_set_mode(&reg_vddq, 0x0);
	mtk_regulator_set_mode(&reg_vdd2H, 0x0);
	mtk_regulator_set_mode(&reg_vdd2L, 0x0);
	mtk_regulator_set_mode(&reg_vmddr, 0x0);
#endif
}

void switch_dramc_voltage_to_pwm_mode(void)
{
#ifdef MTK_PMIC_MT6363
	mtk_regulator_set_mode(&reg_vcore, 0x1);
	mtk_regulator_set_mode(&reg_vddq, 0x1);
	mtk_regulator_set_mode(&reg_vdd2H, 0x1);
	mtk_regulator_set_mode(&reg_vdd2L, 0x1);
	mtk_regulator_set_mode(&reg_vmddr, 0x1);
#endif
}

void setup_dramc_voltage_by_pmic(void)
{
#ifdef VOLTAGE_SEL
	int vcore, vmddr;
#endif
	init_dram_regulator();
	switch_dramc_voltage_to_pwm_mode();

	//VDD1
	#ifdef VOLTAGE_SEL
	dramc_set_vio18_voltage(vio18_voltage_select());
	#else
	dramc_set_vio18_voltage(SEL_VIO18);
	#endif

	//VMDDR
	if (is_second_pmic_pp_swap()) {
		#ifdef VOLTAGE_SEL
		vmddr = vmddr_voltage_select(KSHU0);
		if (vmddr)
			dramc_set_vmddr_voltage(vmddr);
		else
		#endif
		dramc_set_vmddr_voltage(SEL_PREFIX_DVS_VMDDR(KSHU0));
	} else {
		#ifdef VOLTAGE_SEL
		dramc_set_vmddr_voltage(vmddr_voltage_select(KSHU0));
		#else
		dramc_set_vmddr_voltage(SEL_VMDDR);
		#endif
	}

	if (mt_get_dram_type() == TYPE_LPDDR5X) {
		//Vcore LP5X
	#if defined(VCORE_BIN)
		#ifdef VOLTAGE_SEL
		vcore = vcore_voltage_select(KSHU0);
		if (vcore)
			dramc_set_vcore_voltage(vcore);
		else
		#endif
		dramc_set_vcore_voltage(get_vcore_uv_table(0));
	#else
		#ifdef VOLTAGE_SEL
		dramc_set_vcore_voltage(vcore_voltage_select(KSHU0));
		#else
		dramc_set_vcore_voltage(SEL_PREFIX_VCORE(LP5X, KSHU0));
		#endif
	#endif

		//VDDQ LP5X
		#ifdef VOLTAGE_SEL
		dramc_set_vddq_voltage(vddq_voltage_select(VDDQ_HIGHFREQ));
		#else
		dramc_set_vddq_voltage(SEL_PREFIX_VDDQ_LP5X(HIGHFREQ));
		#endif

		//VDD2H LP5X
		#ifdef VOLTAGE_SEL
		dramc_set_vdd2H_voltage(vdd2H_voltage_select());
		#else
		dramc_set_vdd2H_voltage(SEL_PREFIX_VDD2H_LP5X);
		#endif

		//VDD2L LP5X
		#ifdef VOLTAGE_SEL
		dramc_set_vdd2L_voltage(vdd2L_voltage_select());
		#else
		#ifdef NVCORE_NVDRAM
		if(mr5 == 1) {
			dramc_set_vdd2L_voltage(HQA_VDD2L_SAMSUNG_NV_LP5X);
		} else {
			dramc_set_vdd2L_voltage(SEL_PREFIX_VDD2L_LP5X);
		}
		#else
		dramc_set_vdd2L_voltage(SEL_PREFIX_VDD2L_LP5X);
		#endif
		#endif
	} else {
		//Vcore LP5
	#if defined(VCORE_BIN)
		#ifdef VOLTAGE_SEL
		vcore = vcore_voltage_select(KSHU0);
		if (vcore)
			dramc_set_vcore_voltage(vcore);
		else
		#endif
		dramc_set_vcore_voltage(get_vcore_uv_table(0));
	#else
		#ifdef VOLTAGE_SEL
		dramc_set_vcore_voltage(vcore_voltage_select(KSHU0));
		#else
		dramc_set_vcore_voltage(SEL_PREFIX_VCORE(LP5, KSHU0));
		#endif
	#endif

		//VDDQ LP5
		#ifdef VOLTAGE_SEL
		dramc_set_vddq_voltage(vddq_voltage_select(VDDQ_HIGHFREQ));
		#else
		dramc_set_vddq_voltage(SEL_PREFIX_VDDQ_LP5(HIGHFREQ));
		#endif

		//VDD2H LP5
		#ifdef VOLTAGE_SEL
		dramc_set_vdd2H_voltage(vdd2H_voltage_select());
		#else
		dramc_set_vdd2H_voltage(SEL_PREFIX_VDD2H);
		#endif

		//VDD2L LP5
		#ifdef VOLTAGE_SEL
		dramc_set_vdd2L_voltage(vdd2L_voltage_select());
		#else
		#ifdef NVCORE_NVDRAM
		if(mr5 == 1) {
			dramc_set_vdd2L_voltage(HQA_VDD2L_SAMSUNG_NV_LP5);
		} else {
			dramc_set_vdd2L_voltage(SEL_PREFIX_VDD2L);
		}
		#else
		dramc_set_vdd2L_voltage(SEL_PREFIX_VDD2L);
		#endif
		#endif
    }

	dramc_debug("Vio18 = %d\n", dramc_get_vio18_voltage());
	dramc_debug("Vcore = %d\n", dramc_get_vcore_voltage());
	dramc_debug("Vddq = %d\n", dramc_get_vddq_voltage());
	dramc_debug("vmddr = %d\n", dramc_get_vmddr_voltage());
	dramc_debug("vdd2H = %d\n", dramc_get_vdd2H_voltage());
	dramc_debug("vdd2L = %d\n", dramc_get_vdd2L_voltage());
}

int get_sel_prefix_dvfs_vmddr(DRAM_KSHU kshu)
{
	int ret = 0;
	switch(kshu) {
		case KSHU0:
			ret = SEL_PREFIX_DVS_VMDDR(KSHU0);
			break;
		case KSHU1:
			ret = SEL_PREFIX_DVS_VMDDR(KSHU1);
			break;
		case KSHU2:
			ret = SEL_PREFIX_DVS_VMDDR(KSHU2);
			break;
		case KSHU3:
			ret = SEL_PREFIX_DVS_VMDDR(KSHU3);
			break;
		case KSHU4:
			ret = SEL_PREFIX_DVS_VMDDR(KSHU4);
			break;
		case KSHU5:
			ret = SEL_PREFIX_DVS_VMDDR(KSHU5);
			break;
		case KSHU6:
			ret = SEL_PREFIX_DVS_VMDDR(KSHU6);
			break;
		case KSHU7:
			ret = SEL_PREFIX_DVS_VMDDR(KSHU7);
			break;
		case KSHU8:
			ret = SEL_PREFIX_DVS_VMDDR(KSHU8);
			break;
	}
	return ret;
}


void set_vmddr_setting(DRAM_KSHU kshu)
{
	int vmddr;

	if (is_second_pmic_pp_swap()) {
		#ifdef VOLTAGE_SEL
		vmddr = vmddr_voltage_select(kshu);
		if (vmddr)
		  dramc_set_vmddr_voltage(vmddr);
		else 
		#endif
		/* TODO: check the default setting */
		dramc_set_vmddr_voltage(get_sel_prefix_dvfs_vmddr(kshu));
	} else {
		#ifdef VOLTAGE_SEL
		dramc_set_vmddr_voltage(vmddr_voltage_select(kshu));
		#else
		dramc_set_vmddr_voltage(SEL_VMDDR);
		#endif
	}
}

static void restore_vmddr_setting(void)
{
	int vmddr;

	if (is_second_pmic_pp_swap()) {
		#ifdef VOLTAGE_SEL
		vmddr = vmddr_voltage_select(KSHU0);
		if (vmddr)
			dramc_set_vmddr_voltage(vmddr);
		else
		#endif
		/* TODO: check the default setting */
		dramc_set_vmddr_voltage(SEL_PREFIX_DVS_VMDDR(KSHU0));
	} else {
		#ifdef VOLTAGE_SEL
		vmddr = vmddr_voltage_select(KSHU0);
		if (vmddr)
			dramc_set_vmddr_voltage(vmddr);
		else
		#endif
		dramc_set_vmddr_voltage(SEL_VMDDR);
	}
}

static void restore_vcore_vddq_setting(void)
{
#ifdef VOLTAGE_SEL
	int vcore;
#endif
#ifdef MTK_PMIC_MT6363
	int chipVer = mt_get_chip_sw_ver();
	if (mt_get_dram_type() == TYPE_LPDDR5X) {
	#if defined(VCORE_BIN)
		#ifdef VOLTAGE_SEL
		vcore = vcore_voltage_select(KSHU0, TYPE_LPDDR5X);
		if (vcore)
			dramc_set_vcore_voltage(vcore);
		else
		#endif
		dramc_set_vcore_voltage(get_vcore_uv_table(0));
	#else
		#ifdef VOLTAGE_SEL
		dramc_set_vcore_voltage(vcore_voltage_select(KSHU0, TYPE_LPDDR5X));
		#else
		if(chipVer == CHIP_VER_E2){
			dramc_set_vcore_voltage(SEL_PREFIX_VCORE_E2(LP5X, KSHU0));
		}else{
			dramc_set_vcore_voltage(SEL_PREFIX_VCORE(LP5X, KSHU0));
		}
		#endif
	#endif

	} else {
	#if defined(VCORE_BIN)
		#ifdef VOLTAGE_SEL
		vcore = vcore_voltage_select(KSHU0, TYPE_LPDDR5);
		if (vcore)
			dramc_set_vcore_voltage(vcore);
		else
		#endif
		#if !defined(SLT)
		dramc_set_vcore_voltage(get_vcore_uv_table(0));
		#else
		dramc_set_vcore_voltage(get_vcore_uv_table(1));
		#endif
	#else
		#ifdef VOLTAGE_SEL
		dramc_set_vcore_voltage(vcore_voltage_select(KSHU0, TYPE_LPDDR5));
		#else
		if(chipVer == CHIP_VER_E2){
			dramc_set_vcore_voltage(SEL_PREFIX_VCORE_E2(LP5, KSHU0));
		}else{
			dramc_set_vcore_voltage(SEL_PREFIX_VCORE(LP5, KSHU0));
		}
		#endif
	#endif

	}
	dramc_debug("Vddq = %d\n", dramc_get_vddq_voltage());
	dramc_debug("Vcore = %d\n", dramc_get_vcore_voltage());
#endif
}

static void restore_pmic_setting(void)
{
#ifdef LAST_DRAMC
	last_dramc_info_ptr = (LAST_DRAMC_INFO_T *) get_dbg_info_base(KEY_LAST_DRAMC);
	mr5 = last_dramc_info_ptr->mr5;
#endif
#ifdef MTK_PMIC_MT6363
	init_dram_regulator();
	restore_vcore_vddq_setting();

	if (mt_get_dram_type() == TYPE_LPDDR5X) {
		#ifdef VOLTAGE_SEL
		dramc_set_vdd2H_voltage(vdd2H_voltage_select());
		#else
		dramc_set_vdd2H_voltage(SEL_PREFIX_VDD2H_LP5X);
		#endif

		#ifdef VOLTAGE_SEL
		dramc_set_vdd2L_voltage(vdd2L_voltage_select());
		#else
		#ifdef NVCORE_NVDRAM
		if(mr5 == 1) {
			dramc_set_vdd2L_voltage(HQA_VDD2L_SAMSUNG_NV_LP5X);
		} else {
			dramc_set_vdd2L_voltage(SEL_PREFIX_VDD2L_LP5X);
		}
		#else
		dramc_set_vdd2L_voltage(SEL_PREFIX_VDD2L_LP5X);
		#endif
		#endif

	} else {
		#ifdef VOLTAGE_SEL
		dramc_set_vdd2H_voltage(vdd2H_voltage_select());
		#else
		dramc_set_vdd2H_voltage(SEL_PREFIX_VDD2H);
		#endif

		#ifdef VOLTAGE_SEL
		dramc_set_vdd2L_voltage(vdd2L_voltage_select());
		#else
		#ifdef NVCORE_NVDRAM
		if(mr5 == 1) {
			dramc_set_vdd2L_voltage(HQA_VDD2L_SAMSUNG_NV_LP5);
		} else {
			dramc_set_vdd2L_voltage(SEL_PREFIX_VDD2L);
		}
		#else
		dramc_set_vdd2L_voltage(SEL_PREFIX_VDD2L);
		#endif
		#endif

	}

	restore_vmddr_setting();

	#ifdef VOLTAGE_SEL
	dramc_set_vio18_voltage(vio18_voltage_select());
	#else
	dramc_set_vio18_voltage(SEL_VIO18);
	#endif
#endif
	dramc_debug("Vdd2H = %d\n", dramc_get_vdd2H_voltage());
	dramc_debug("Vdd2L = %d\n", dramc_get_vdd2L_voltage());
	dramc_debug("vmddr = %d\n", dramc_get_vmddr_voltage());
	dramc_debug("Vio18 = %d\n", dramc_get_vio18_voltage());
}

#ifdef DDR_RESERVE_MODE
extern unsigned int GetWDTDpmDramcPwrAckStateNotS0(void);
#endif
void release_dram(void)
{
#ifdef DDR_RESERVE_MODE
	int i;
	int counter = TIMEOUT;

	if(GetWDTDpmDramcPwrAckStateNotS0())
	{
	// scy: restore pmic setting (VCORE, VDRAM, VSRAM, VDDQ)
	restore_pmic_setting();

	drm_release_rg_dramc_conf_iso();//Release DRAMC/PHY conf ISO

#if DDR_RESERVE_NEW_MODE //new modw
	//ASVA5_8_New_Mode_1();
	Dramc_DDR_Reserved_Mode_setting();
	drm_release_rg_dramc_iso();//Release PHY IO ISO
	//ASVA5_8_New_Mode_2();
#else //old mode
	Dramc_DDR_Reserved_Mode_setting();
	ASVA5_8_CSCA_Pull_Down_EN();
	drm_release_rg_dramc_iso();//Release PHY IO ISO
	ASVA5_8_CSCA_Pull_Down_DIS();
#endif

	Dramc_DDR_Reserved_Mode_BeforeSR();  // mt6983

	drm_release_rg_dramc_sref();//Let DRAM Leave SR

	Dramc_DDR_Reserved_Mode_AfterSR();

	while(counter) {
		if(is_dramc_exit_slf() == 1) /* expect to exit dram-self-refresh */
			break;
		counter--;
    }

	if(counter == 0) {
		if(g_ddr_reserve_enable==1 && g_ddr_reserve_success==1) {
			dramc_crit("[DDR Reserve] release dram from self-refresh FAIL!\n");
			g_ddr_reserve_success = 0;
		}
	}
	else {
		dramc_crit("[DDR Reserve] release dram from self-refresh PASS!\n");
	}

#if DDR_RESERVE_NEW_MODE //new modw
	//ASVA5_8_New_Mode_3();
#endif

	//Expect to Use LPDDR3200 and PHYPLL as output, so no need to handle
	//shuffle status since the status will be reset by system reset
	//There is an PLLL_SHU_GP in SPM which will reset by system reset

	// setup for EMI: touch center EMI and channel EMI to enable CLK
	dramc_crit("[DDR reserve] EMI CEN CONA: %x\n", get_cen_emi_cona());
	dramc_crit("[DDR reserve] EMI CHN CONA: %x\n", get_chn_emi_cona());
	udelay(1);
	}
	return;
#endif
}

#if APPLY_NEW_IO_API
#define Channel_A_DDRPHY_DPM_BASE_ADDRESS	0x10900000
#define DPM_BASE_SHIFT 0x100000
#endif

/* you should confirm the function with SA */
static void enable_dramc_clk(void) {

	unsigned int reg = 0;
	unsigned int i = 0;

#if 0
	for (i = 0; i < DRAMC_DPM_CNT; i++) {
		/* 0x47004[15:14]  = 0 */
		reg = DRV_Reg32(Channel_A_DDRPHY_DPM_BASE_ADDRESS + DPM_BASE_SHIFT * i + 0x47004);
		DRV_WriteReg32(Channel_A_DDRPHY_DPM_BASE_ADDRESS + DPM_BASE_SHIFT * i + 0x47004, reg & ~(0xC000));
		udelay(1);

		/* 0x47034[0] = 0 EED_DE_NOTICE */
		reg = DRV_Reg32(Channel_A_DDRPHY_DPM_BASE_ADDRESS + DPM_BASE_SHIFT * i + 0x47034);
		DRV_WriteReg32(Channel_A_DDRPHY_DPM_BASE_ADDRESS + DPM_BASE_SHIFT * i + 0x47034, reg & ~(1 << 0));
          
		/* 0x47020[28] = 1 */
		reg = DRV_Reg32(Channel_A_DDRPHY_DPM_BASE_ADDRESS + DPM_BASE_SHIFT * i + 0x47020);
		DRV_WriteReg32(Channel_A_DDRPHY_DPM_BASE_ADDRESS + DPM_BASE_SHIFT * i + 0x47020, reg | (1 << 28));
	}
#endif 
	MD32_init();

}

void check_ddr_reserve_status(void)
{
	/* get status of DCS and DVFSRC */
	int dvfsrc_success = drm_is_dvfsrc_success();
	int dvfsrc_en = drm_is_dvfsrc_enable();

#ifdef DDR_RESERVE_MODE
	int counter = TIMEOUT;
	if(drm_is_reserve_ddr_enabled()) {
		g_ddr_reserve_enable = 1;
#ifdef LAST_DRAMC
		dram_fatal_set_ddr_rsv_mode_flow();
#endif

		if(drm_is_reserve_ddr_mode_success()) {
			while(counter) {
				if(drm_is_dram_slf()) {
					g_ddr_reserve_success = 1;
					break;
				}
				counter--;
			}
			if(counter == 0) {
				dramc_crit("[DDR Reserve] ddr reserve mode success but DRAM not in self-refresh!\n");
				g_ddr_reserve_success = 0;
#ifdef LAST_DRAMC
				dram_fatal_set_ddr_rsv_mode_err();
#endif
			}
		} else {
			dramc_crit("[DDR Reserve] ddr reserve mode FAIL!\n");
			drm_dram_reserved(0);
			g_ddr_reserve_success = 0;
#ifdef LAST_DRAMC
			dram_fatal_set_ddr_rsv_mode_err();
#endif
		}

		/* overwrite g_ddr_reserve_success if dvfsrc failed */
		if (dvfsrc_en == 1 && dvfsrc_success == 0) {
			dramc_crit("[DDR Reserve] DVFSRC fail!\n");
			drm_dram_reserved(0);
			g_ddr_reserve_success = 0;
#if 0//def LAST_DRAMC
			dram_fatal_set_dvfsrc_err();
#endif
		} else {
			dramc_crit("[DDR Reserve] DVFSRC success! (dvfsrc_en=%d)\n", dvfsrc_en);
		}

		/* release dram, no matter success or failed */
		release_dram();
	} else {
		dramc_crit("[DDR Reserve] ddr reserve mode not be enabled yet\n");
		g_ddr_reserve_enable = 0;

		#ifdef LAST_DRAMC
		enable_dramc_clk();
		#endif
	}
#else
#ifdef LAST_DRAMC
	enable_dramc_clk();
#endif
#endif
}

unsigned int DRAM_MRR(int MRR_num)
{
	u16 MRR_value = 0x0;
	DRAMC_CTX_T *p = psCurrDramCtx;

	DramcModeRegRead(p, MRR_num, &MRR_value);
	return MRR_value;
}

int get_dram_channel_support_nr(void)
{
	return DRAMC_MAX_CH;
}

int get_dram_channel_nr(void)
{
	return get_channel_nr_by_emi();
}

int get_dram_rank_nr(void)
{
	int cen_emi_cona;

#ifdef DDR_RESERVE_MODE
	if (g_ddr_reserve_enable==1 && g_ddr_reserve_success==1)
		return get_rank_nr_by_emi();
#endif

#ifdef DRAM_ADAPTIVE
	if (!auto_detect_done)
		DRAMC_ASSERT(0);
#endif

	cen_emi_cona = g_default_emi_setting.EMI_CONA_VAL;

	if ((cen_emi_cona & (1 << 17)) != 0 || //for channel 0  
		(cen_emi_cona & (1 << 16)) != 0 )  //for channel 1
		return 2;
	else
		return 1;
}

int get_dram_mr_cnt(void)
{
	return DRAMC_MR_CNT;
}

int get_dram_freq_cnt(void)
{
	return DRAMC_FREQ_CNT;
}

void set_dram_mr5(DRAM_INFO_BY_MRR_T dram_info)
{
	mr5 = dram_info.u2MR5VendorID & 0xFF;
	return;
}

unsigned int get_dram_mr5()
{
	return mr5;
}

#if (FOR_DV_SIMULATION_USED==0)
#if !__ETT__
/* 
 * setup block
 */
void get_orig_dram_rank_info(dram_info_t *orig_dram_info)
{
	int i, j;
	u64 base = DRAM_BASE;
	u64 rank_size[DRAMC_MAX_RK];

	i = get_dram_rank_nr();

	orig_dram_info->rank_num = (i > 0) ? i : 0;
	get_dram_rank_size(rank_size);

	orig_dram_info->rank_info[0].start = base;
	for (i = 0; i < orig_dram_info->rank_num && i < DRAMC_MAX_RK; i++) {

		orig_dram_info->rank_info[i].size = (u64)rank_size[i];

		if (i > 0) {
			orig_dram_info->rank_info[i].start =
				orig_dram_info->rank_info[i - 1].start +
				orig_dram_info->rank_info[i - 1].size;
		}
		dramc_debug("orig_dram_info[%d] start: 0x%llx, size: 0x%llx\n",
				i, orig_dram_info->rank_info[i].start,
				orig_dram_info->rank_info[i].size);
	}
	
	for(j=i; j<4; j++)
	{
		orig_dram_info->rank_info[j].start = 0;
		orig_dram_info->rank_info[j].size = 0;
	}
#ifdef CUSTOM_CONFIG_MAX_DRAM_SIZE
	for (i = 1; i < orig_dram_info->rank_num; i++) {
		if (orig_dram_info->rank_info[i].start >= (get_config_max_dram_size() + DRAM_BASE)) {
			orig_dram_info->rank_num = i;
			dramc_crit("[dramc] reduce rank size = %u\n", (unsigned int) orig_dram_info->rank_num);
			break;
		}
	}
#endif
}

void get_dram_rank_size(u64 dram_rank_size[DRAMC_MAX_RK])
{
	int rank_nr, i;

#ifdef DDR_RESERVE_MODE
	if(g_ddr_reserve_enable==1 && g_ddr_reserve_success==1){
		get_rank_size_by_emi(dram_rank_size);
		return;
	}
#endif

#ifdef DRAM_ADAPTIVE
	if (!auto_detect_done)
		DRAMC_ASSERT(0);
#endif

	rank_nr = get_dram_rank_nr();

	for(i = 0; i < rank_nr; i++) {
		dram_rank_size[i] = g_default_emi_setting.DRAM_RANK_SIZE[i];
		dramc_debug("%d:dram_rank_size:%llx\n",i,dram_rank_size[i]);
	}
}

void get_dram_freq_step(u32 dram_freq_step[])
{
	unsigned int i;
	unsigned int defined_step[DRAMC_FREQ_CNT] = {
            7500, 6400, 5500, 4266, 3200, 2667, 2133, 1866, 800};
	if (mt_get_dram_type() == TYPE_LPDDR5) {
		defined_step[0] = 6400;
	}

	for (i = 0; i < DRAMC_FREQ_CNT; i++) {
		dram_freq_step[i] = defined_step[i];
	}
}

void set_dram_mr(unsigned int index, unsigned short value)
{
#if 0
	unsigned short value_2rk;

	value_2rk = value & 0xFF;
	value_2rk |= (value_2rk << 8);

	switch (index) {
	case 5:
		mr5 = value_2rk;
		break;
	case 6:
		mr6 = value_2rk;
		break;
	case 7:
		mr7 = value_2rk;
		break;
	case 8:
		mr8 = value;
	default:
		break;
	}
#endif
}

unsigned short get_dram_mr(unsigned int index)
{
	unsigned int value = 0;
#if 0
	switch (index) {
	case 5:
		value = last_dramc_info_ptr->mr5;
		break;
	case 6:
		value = last_dramc_info_ptr->mr6;
		break;
	case 7:
		value = last_dramc_info_ptr->mr7;
		break;
	case 8:
		value = last_dramc_info_ptr->mr8;
	default:
		break;
	}
	return (unsigned short)(value & 0xFFFF);
#else
	return (unsigned short)(value & 0xFFFF);
#endif
}

void get_dram_mr_info(struct mr_info_t mr_info[])
{
#if 0
	unsigned int i;
	unsigned int mr_list[DRAMC_MR_CNT] = {5, 6, 7, 8};

	for (i = 0; i < DRAMC_MR_CNT; i++) {
		mr_info[i].mr_index = mr_list[i];
		mr_info[i].mr_value = get_dram_mr(mr_list[i]);
	}
#endif
}

void reserve_dramc_dummy_read(void)
{
    unsigned long long reserve_start;
    unsigned long long reserve_size = 0x1000;
    unsigned long long full_sys_addr;
    char *reserve_name[4] = {"dramc-rk0", "dramc-rk1", "dramc-rk2", "dramc-rk3"};
    unsigned int i;
    int rank_nr;
    struct emi_addr_map dram_addr;

    if (mt_get_dram_type() != TYPE_LPDDR4X)
        reserve_size = 0x2000;
    dram_addr.channel = 0;

    rank_nr = get_dram_rank_nr();
    if (rank_nr <= 0) {
        dramc_crit("[DRAMC] reserve dummy read fail\n");
        DRAMC_ASSERT(0);
    }

    for (i = 0; i < (unsigned int)rank_nr; i++) {
        dram_addr.rank = i;
        full_sys_addr = get_dramc_addr(&dram_addr, 0x0);
#ifdef CUSTOM_CONFIG_MAX_DRAM_SIZE
        if (full_sys_addr > get_config_max_dram_size())
            break;
#endif
        reserve_start = mblock_reserve_ext(&bootarg.mblock_info, reserve_size, 0x1000, full_sys_addr, 0, reserve_name[i]);
        if (reserve_start != (full_sys_addr - reserve_size)) {
            dramc_crit("[DRAMC] dummy read fail (0x%llx)\n", reserve_start);
            DRAMC_ASSERT(0);
        }
    }
}

#endif //#if !__FLASH_TOOL_DA__ && !__ETT__
#endif


#if (CFG_DRAM_LOG_TO_STORAGE)
#include <blkdev.h>
#include <partition.h>
#include <storage_api.h>

extern u64 get_part_addr(const char *name);
u64 part_dram_data_addr_uart = 0;
u32 log_start = 0;
static char logbuf[1024];
static int logcount;
#endif

#ifdef VOLTAGE_SEL
void update_voltage_select_info(void)
{
	voltage_sel_info_ptr.vcore = doe_get_config("vcore");
	voltage_sel_info_ptr.vdd2 = doe_get_config("vdd2");
	voltage_sel_info_ptr.vddq = doe_get_config("vddq");
	voltage_sel_info_ptr.vmddr = doe_get_config("vmddr");
	voltage_sel_info_ptr.vio18 = doe_get_config("vio18");
	voltage_sel_info_ptr.vdd2H = doe_get_config("vdd2H");
	voltage_sel_info_ptr.vdd2L = doe_get_config("vdd2L");

	print("DOE setting: vcore:%d, vdd2:%d, vddq:%d, vmddr:%d,"
		" vio18:%d, vdd2H:%d, vdd2L:%d\n", voltage_sel_info_ptr.vcore,
		voltage_sel_info_ptr.vdd2, voltage_sel_info_ptr.vddq,
		voltage_sel_info_ptr.vmddr, voltage_sel_info_ptr.vio18,
		voltage_sel_info_ptr.vdd2H, voltage_sel_info_ptr.vdd2L);
}

int vio18_voltage_select(void)
{
	if (voltage_sel_info_ptr.vio18 == LEVEL_LV) {
		return HQA_VIO18_LV;
	} else if (voltage_sel_info_ptr.vio18 == LEVEL_HV) {
		return HQA_VIO18_HV;
	} else {
		return HQA_VIO18_NV;
	}
}

int vmddr_voltage_select(DRAM_KSHU kshu)
{
	int vmddr = 0;

	if (voltage_sel_info_ptr.vmddr == LEVEL_LV) {
		switch (kshu) {
		case KSHU0:
			vmddr = HQA_VMDDR_LV_LP5_KSHU0_PL;
			break;
		case KSHU1:
			vmddr = HQA_VMDDR_LV_LP5_KSHU1_PL;
			break;
		case KSHU2:
			vmddr = HQA_VMDDR_LV_LP5_KSHU2_PL;
			break;
		case KSHU3:
			vmddr = HQA_VMDDR_LV_LP5_KSHU3_PL;
			break;
		case KSHU4:
			vmddr = HQA_VMDDR_LV_LP5_KSHU4_PL;
			break;
		case KSHU5:
			vmddr = HQA_VMDDR_LV_LP5_KSHU5_PL;
			break;
		case KSHU6:
			vmddr = HQA_VMDDR_LV_LP5_KSHU6_PL;
			break;
		case KSHU7:
			vmddr = HQA_VMDDR_LV_LP5_KSHU7_PL;
			break;
		case KSHU8:
			vmddr = HQA_VMDDR_LV_LP5_KSHU8_PL;
			break;
		default:
			break;
		}
	} else if (voltage_sel_info_ptr.vmddr == LEVEL_HV) {
		switch (kshu) {
		case KSHU0:
			vmddr = HQA_VMDDR_HV_LP5_KSHU0_PL;
			break;
		case KSHU1:
			vmddr = HQA_VMDDR_HV_LP5_KSHU1_PL;
			break;
		case KSHU2:
			vmddr = HQA_VMDDR_HV_LP5_KSHU2_PL;
			break;
		case KSHU3:
			vmddr = HQA_VMDDR_HV_LP5_KSHU3_PL;
			break;
		case KSHU4:
			vmddr = HQA_VMDDR_HV_LP5_KSHU4_PL;
			break;
		case KSHU5:
			vmddr = HQA_VMDDR_HV_LP5_KSHU5_PL;
			break;
		case KSHU6:
			vmddr = HQA_VMDDR_HV_LP5_KSHU6_PL;
			break;
		case KSHU7:
			vmddr = HQA_VMDDR_HV_LP5_KSHU7_PL;
			break;
		case KSHU8:
			vmddr = HQA_VMDDR_HV_LP5_KSHU8_PL;
			break;
		default:
			break;
		}
	} else {
		switch (kshu) {
		case KSHU0:
			vmddr = HQA_VMDDR_NV_LP5_KSHU0_PL;
			break;
		case KSHU1:
			vmddr = HQA_VMDDR_NV_LP5_KSHU1_PL;
			break;
		case KSHU2:
			vmddr = HQA_VMDDR_NV_LP5_KSHU2_PL;
			break;
		case KSHU3:
			vmddr = HQA_VMDDR_NV_LP5_KSHU3_PL;
			break;
		case KSHU4:
			vmddr = HQA_VMDDR_NV_LP5_KSHU4_PL;
			break;
		case KSHU5:
			vmddr = HQA_VMDDR_NV_LP5_KSHU5_PL;
			break;
		case KSHU6:
			vmddr = HQA_VMDDR_NV_LP5_KSHU6_PL;
			break;
		case KSHU7:
			vmddr = HQA_VMDDR_NV_LP5_KSHU7_PL;
			break;
		case KSHU8:
			vmddr = HQA_VMDDR_NV_LP5_KSHU8_PL;
			break;
		default:
			break;
		}
	}

	return vmddr;
}

int vdd2H_voltage_select(void)
{
	if (mt_get_dram_type() == TYPE_LPDDR5X) {
		if (voltage_sel_info_ptr.vdd2 == LEVEL_LV) {
			return HQA_VDD2H_LV_LP5X;
		} else if (voltage_sel_info_ptr.vdd2 == LEVEL_HV) {
			return HQA_VDD2H_HV_LP5X;
		} else {
			return HQA_VDD2H_NV_LP5X;
		}
	} else {
		if (voltage_sel_info_ptr.vdd2 == LEVEL_LV) {
			return HQA_VDD2H_LV_LP5;
		} else if (voltage_sel_info_ptr.vdd2 == LEVEL_HV) {
			return HQA_VDD2H_HV_LP5;
		} else {
			return HQA_VDD2H_NV_LP5;
		}
	}
}

int vdd2L_voltage_select(void)
{
	if (mt_get_dram_type() == TYPE_LPDDR5X) {
		if (voltage_sel_info_ptr.vdd2L == LEVEL_LV) {
			return HQA_VDD2L_LV_LP5X;
		} else if (voltage_sel_info_ptr.vdd2L == LEVEL_HV) {
			return HQA_VDD2L_HV_LP5X;
		} else {
			if (mr5 == 1)
				return HQA_VDD2L_SAMSUNG_NV_LP5X;
			else
				return HQA_VDD2L_NV_LP5X;
		}
	} else {
		if (voltage_sel_info_ptr.vdd2L == LEVEL_LV) {
			return HQA_VDD2L_LV_LP5;
		} else if (voltage_sel_info_ptr.vdd2L == LEVEL_HV) {
			return HQA_VDD2L_HV_LP5;
		} else {
			if (mr5 == 1)
				return HQA_VDD2L_SAMSUNG_NV_LP5;
			else
				return HQA_VDD2L_NV_LP5;
		}
	}
}


int vddq_voltage_select(VDDQ_LEVEL level)
{
	if(mt_get_dram_type() == TYPE_LPDDR5X) {
		if (level == VDDQ_LOWFREQ) {
			if (voltage_sel_info_ptr.vddq == LEVEL_LV) {
				return HQA_VDDQ_LOWFREQ_LV_LP5X;
			} else if (voltage_sel_info_ptr.vddq == LEVEL_HV) {
				return HQA_VDDQ_LOWFREQ_HV_LP5X;
			} else {
				return HQA_VDDQ_LOWFREQ_NV_LP5X;
			}
		} else { //VDDQ_HIGHFREQ
			if (voltage_sel_info_ptr.vddq == LEVEL_LV) {
				return HQA_VDDQ_HIGHFREQ_LV_LP5X;
			} else if (voltage_sel_info_ptr.vddq == LEVEL_HV) {
				return HQA_VDDQ_HIGHFREQ_HV_LP5X;
			} else {
				return HQA_VDDQ_HIGHFREQ_NV_LP5X;
			}
		}
	} else {
		if (level == VDDQ_LOWFREQ) {
			if (voltage_sel_info_ptr.vddq == LEVEL_LV) {
				return HQA_VDDQ_LOWFREQ_LV_LP5;
			} else if (voltage_sel_info_ptr.vddq == LEVEL_HV) {
				return HQA_VDDQ_LOWFREQ_HV_LP5;
			} else {
				return HQA_VDDQ_LOWFREQ_NV_LP5;
			}
		} else { //VDDQ_HIGHFREQ
			if (voltage_sel_info_ptr.vddq == LEVEL_LV) {
				return HQA_VDDQ_HIGHFREQ_LV_LP5;
			} else if (voltage_sel_info_ptr.vddq == LEVEL_HV) {
				return HQA_VDDQ_HIGHFREQ_HV_LP5;
			} else {
				return HQA_VDDQ_HIGHFREQ_NV_LP5;
			}
		}
	}
}
int vcore_voltage_select(DRAM_KSHU kshu)
{
	int ret = 0;
	int chipVer = mt_get_chip_sw_ver();
	if(mt_get_dram_type() == TYPE_LPDDR5X) {
		if (voltage_sel_info_ptr.vcore == LEVEL_LV) {
			if(chipVer == CHIP_VER_E2){
				switch(kshu) {
					case KSHU0:
						ret = HQA_VCORE_LV_LP5X_KSHU0_PL_E2;
						break;
					case KSHU1:
						ret = HQA_VCORE_LV_LP5X_KSHU1_PL_E2;
						break;
					case KSHU2:
						ret = HQA_VCORE_LV_LP5X_KSHU2_PL_E2;
						break;
					case KSHU3:
						ret = HQA_VCORE_LV_LP5X_KSHU3_PL_E2;
						break;
					case KSHU4:
						ret = HQA_VCORE_LV_LP5X_KSHU4_PL_E2;
						break;
					case KSHU5:
						ret = HQA_VCORE_LV_LP5X_KSHU5_PL_E2;
						break;
					case KSHU6:
						ret = HQA_VCORE_LV_LP5X_KSHU6_PL_E2;
						break;
					case KSHU7:
						ret = HQA_VCORE_LV_LP5X_KSHU7_PL_E2;
						break;
					case KSHU8:
						ret = HQA_VCORE_LV_LP5X_KSHU8_PL_E2;
						break;
				};
			}else {			
				switch(kshu) {
					case KSHU0:
						ret = HQA_VCORE_LV_LP5X_KSHU0_PL;
						break;
					case KSHU1:
						ret = HQA_VCORE_LV_LP5X_KSHU1_PL;
						break;
					case KSHU2:
						ret = HQA_VCORE_LV_LP5X_KSHU2_PL;
						break;
					case KSHU3:
						ret = HQA_VCORE_LV_LP5X_KSHU3_PL;
						break;
					case KSHU4:
						ret = HQA_VCORE_LV_LP5X_KSHU4_PL;
						break;
					case KSHU5:
						ret = HQA_VCORE_LV_LP5X_KSHU5_PL;
						break;
					case KSHU6:
						ret = HQA_VCORE_LV_LP5X_KSHU6_PL;
						break;
					case KSHU7:
						ret = HQA_VCORE_LV_LP5X_KSHU7_PL;
						break;
					case KSHU8:
						ret = HQA_VCORE_LV_LP5X_KSHU8_PL;
						break;
				};
			}
		} else if (voltage_sel_info_ptr.vcore == LEVEL_HV) {
			if(chipVer == CHIP_VER_E2){
				switch(kshu) {
					case KSHU0:
						ret = HQA_VCORE_HV_LP5X_KSHU0_PL_E2;
						break;
					case KSHU1:
						ret = HQA_VCORE_HV_LP5X_KSHU1_PL_E2;
						break;
					case KSHU2:
						ret = HQA_VCORE_HV_LP5X_KSHU2_PL_E2;
						break;
					case KSHU3:
						ret = HQA_VCORE_HV_LP5X_KSHU3_PL_E2;
						break;
					case KSHU4:
						ret = HQA_VCORE_HV_LP5X_KSHU4_PL_E2;
						break;
					case KSHU5:
						ret = HQA_VCORE_HV_LP5X_KSHU5_PL_E2;
						break;
					case KSHU6:
						ret = HQA_VCORE_HV_LP5X_KSHU6_PL_E2;
						break;
					case KSHU7:
						ret = HQA_VCORE_HV_LP5X_KSHU7_PL_E2;
						break;
					case KSHU8:
						ret = HQA_VCORE_HV_LP5X_KSHU8_PL_E2;
						break;
				};
			}else {
				switch(kshu) {
					case KSHU0:
						ret = HQA_VCORE_HV_LP5X_KSHU0_PL;
						break;
					case KSHU1:
						ret = HQA_VCORE_HV_LP5X_KSHU1_PL;
						break;
					case KSHU2:
						ret = HQA_VCORE_HV_LP5X_KSHU2_PL;
						break;
					case KSHU3:
						ret = HQA_VCORE_HV_LP5X_KSHU3_PL;
						break;
					case KSHU4:
						ret = HQA_VCORE_HV_LP5X_KSHU4_PL;
						break;
					case KSHU5:
						ret = HQA_VCORE_HV_LP5X_KSHU5_PL;
						break;
					case KSHU6:
						ret = HQA_VCORE_HV_LP5X_KSHU6_PL;
						break;
					case KSHU7:
						ret = HQA_VCORE_HV_LP5X_KSHU7_PL;
						break;
					case KSHU8:
						ret = HQA_VCORE_HV_LP5X_KSHU8_PL;
						break;
				};
			}
		} else {
#if defined(VCORE_BIN)
			ret = 0;
#else
			if(chipVer == CHIP_VER_E2){
				switch(kshu) {
					case KSHU0:
						ret = HQA_VCORE_NV_LP5X_KSHU0_PL_E2;
						break;
					case KSHU1:
						ret = HQA_VCORE_NV_LP5X_KSHU1_PL_E2;
						break;
					case KSHU2:
						ret = HQA_VCORE_NV_LP5X_KSHU2_PL_E2;
						break;
					case KSHU3:
						ret = HQA_VCORE_NV_LP5X_KSHU3_PL_E2;
						break;
					case KSHU4:
						ret = HQA_VCORE_NV_LP5X_KSHU4_PL_E2;
						break;
					case KSHU5:
						ret = HQA_VCORE_NV_LP5X_KSHU5_PL_E2;
						break;
					case KSHU6:
						ret = HQA_VCORE_NV_LP5X_KSHU6_PL_E2;
						break;
					case KSHU7:
						ret = HQA_VCORE_NV_LP5X_KSHU7_PL_E2;
						break;
					case KSHU8:
						ret = HQA_VCORE_NV_LP5X_KSHU8_PL_E2;
						break;
				};
		  }else{
				switch(kshu) {
					case KSHU0:
						ret = HQA_VCORE_NV_LP5X_KSHU0_PL;
						break;
					case KSHU1:
						ret = HQA_VCORE_NV_LP5X_KSHU1_PL;
						break;
					case KSHU2:
						ret = HQA_VCORE_NV_LP5X_KSHU2_PL;
						break;
					case KSHU3:
						ret = HQA_VCORE_NV_LP5X_KSHU3_PL;
						break;
					case KSHU4:
						ret = HQA_VCORE_NV_LP5X_KSHU4_PL;
						break;
					case KSHU5:
						ret = HQA_VCORE_NV_LP5X_KSHU5_PL;
						break;
					case KSHU6:
						ret = HQA_VCORE_NV_LP5X_KSHU6_PL;
						break;
					case KSHU7:
						ret = HQA_VCORE_NV_LP5X_KSHU7_PL;
						break;
					case KSHU8:
						ret = HQA_VCORE_NV_LP5X_KSHU8_PL;
						break;
				};
		}
#endif
		}

	} else {
		if (voltage_sel_info_ptr.vcore == LEVEL_LV) {
			if(chipVer == CHIP_VER_E2){
				switch(kshu) {
					case KSHU0:
					  #if defined(HIGHEST_DRAM_DATARATE_DDR5500)
						if((seclib_get_devinfo_with_index(52) & 0xff) == 0)
						    ret = HQA_VCORE_LV_LP5_KSHU0_PL_5500;
						else
					  #endif
						    ret = HQA_VCORE_LV_LP5_KSHU0_PL_E2;
						break;
					case KSHU1:
					  #if defined(HIGHEST_DRAM_DATARATE_DDR5500)
						if((seclib_get_devinfo_with_index(52) & 0xff) == 0)
						    ret = HQA_VCORE_LV_LP5_KSHU1_PL_5500;
						else
					  #endif
						    ret = HQA_VCORE_LV_LP5_KSHU1_PL_E2;
						break;
					case KSHU2:
						ret = HQA_VCORE_LV_LP5_KSHU2_PL_E2;
						break;
					case KSHU3:
						ret = HQA_VCORE_LV_LP5_KSHU3_PL_E2;
						break;
					case KSHU4:
						ret = HQA_VCORE_LV_LP5_KSHU4_PL_E2;
						break;
					case KSHU5:
						ret = HQA_VCORE_LV_LP5_KSHU5_PL_E2;
						break;
					case KSHU6:
						ret = HQA_VCORE_LV_LP5_KSHU6_PL_E2;
						break;
					case KSHU7:
						ret = HQA_VCORE_LV_LP5_KSHU7_PL_E2;
						break;
					case KSHU8:
						ret = HQA_VCORE_LV_LP5_KSHU8_PL_E2;
						break;
				};
			}else{
				switch(kshu) {
					case KSHU0:
					  #if defined(HIGHEST_DRAM_DATARATE_DDR5500)
						if((seclib_get_devinfo_with_index(52) & 0xff) == 0)
						    ret = HQA_VCORE_LV_LP5_KSHU0_PL_5500;
						else
					  #endif
						    ret = HQA_VCORE_LV_LP5_KSHU0_PL;
						break;
					case KSHU1:
					  #if defined(HIGHEST_DRAM_DATARATE_DDR5500)
						if((seclib_get_devinfo_with_index(52) & 0xff) == 0)
						    ret = HQA_VCORE_LV_LP5_KSHU1_PL_5500;
						else
					  #endif
						    ret = HQA_VCORE_LV_LP5_KSHU1_PL;
						break;
					case KSHU2:
						ret = HQA_VCORE_LV_LP5_KSHU2_PL;
						break;
					case KSHU3:
						ret = HQA_VCORE_LV_LP5_KSHU3_PL;
						break;
					case KSHU4:
						ret = HQA_VCORE_LV_LP5_KSHU4_PL;
						break;
					case KSHU5:
						ret = HQA_VCORE_LV_LP5_KSHU5_PL;
						break;
					case KSHU6:
						ret = HQA_VCORE_LV_LP5_KSHU6_PL;
						break;
					case KSHU7:
						ret = HQA_VCORE_LV_LP5_KSHU7_PL;
						break;
					case KSHU8:
						ret = HQA_VCORE_LV_LP5_KSHU8_PL;
						break;
				};
			}
		} else if (voltage_sel_info_ptr.vcore == LEVEL_HV) {
			if(chipVer == CHIP_VER_E2){
				switch(kshu) {
					case KSHU0:
						ret = HQA_VCORE_HV_LP5_KSHU0_PL_E2;
						break;
					case KSHU1:
						ret = HQA_VCORE_HV_LP5_KSHU1_PL_E2;
						break;
					case KSHU2:
						ret = HQA_VCORE_HV_LP5_KSHU2_PL_E2;
						break;
					case KSHU3:
						ret = HQA_VCORE_HV_LP5_KSHU3_PL_E2;
						break;
					case KSHU4:
						ret = HQA_VCORE_HV_LP5_KSHU4_PL_E2;
						break;
					case KSHU5:
						ret = HQA_VCORE_HV_LP5_KSHU5_PL_E2;
						break;
					case KSHU6:
						ret = HQA_VCORE_HV_LP5_KSHU6_PL_E2;
						break;
					case KSHU7:
						ret = HQA_VCORE_HV_LP5_KSHU7_PL_E2;
						break;
					case KSHU8:
						ret = HQA_VCORE_HV_LP5_KSHU8_PL_E2;
						break;
				};
			}else {	
				switch(kshu) {
					case KSHU0:
						ret = HQA_VCORE_HV_LP5_KSHU0_PL;
						break;
					case KSHU1:
						ret = HQA_VCORE_HV_LP5_KSHU1_PL;
						break;
					case KSHU2:
						ret = HQA_VCORE_HV_LP5_KSHU2_PL;
						break;
					case KSHU3:
						ret = HQA_VCORE_HV_LP5_KSHU3_PL;
						break;
					case KSHU4:
						ret = HQA_VCORE_HV_LP5_KSHU4_PL;
						break;
					case KSHU5:
						ret = HQA_VCORE_HV_LP5_KSHU5_PL;
						break;
					case KSHU6:
						ret = HQA_VCORE_HV_LP5_KSHU6_PL;
						break;
					case KSHU7:
						ret = HQA_VCORE_HV_LP5_KSHU7_PL;
						break;
					case KSHU8:
						ret = HQA_VCORE_HV_LP5_KSHU8_PL;
						break;
				};
			}
		} else {
#if defined(VCORE_BIN)
			ret = 0;
#else
			if(chipVer == CHIP_VER_E2){
				switch(kshu) {
					case KSHU0:
					  #if defined(HIGHEST_DRAM_DATARATE_DDR5500)
						if((seclib_get_devinfo_with_index(52) & 0xff) == 0)
						ret = HQA_VCORE_NV_LP5_KSHU0_PL_5500;
						else
					  #endif
				    ret = HQA_VCORE_NV_LP5_KSHU0_PL_E2;
						break;
					case KSHU1:
					  #if defined(HIGHEST_DRAM_DATARATE_DDR5500)
						if((seclib_get_devinfo_with_index(52) & 0xff) == 0)
						ret = HQA_VCORE_NV_LP5_KSHU1_PL_5500;
						else
					  #endif
					  ret = HQA_VCORE_NV_LP5_KSHU1_PL_E2;
						break;
					case KSHU2:
						ret = HQA_VCORE_NV_LP5_KSHU2_PL_E2;
						break;
					case KSHU3:
						ret = HQA_VCORE_NV_LP5_KSHU3_PL_E2;
						break;
					case KSHU4:
						ret = HQA_VCORE_NV_LP5_KSHU4_PL_E2;
						break;
					case KSHU5:
						ret = HQA_VCORE_NV_LP5_KSHU5_PL_E2;
						break;
					case KSHU6:
						ret = HQA_VCORE_NV_LP5_KSHU6_PL_E2;
						break;
					case KSHU7:
						ret = HQA_VCORE_NV_LP5_KSHU7_PL_E2;
						break;
					case KSHU8:
						ret = HQA_VCORE_NV_LP5_KSHU8_PL_E2;
						break;
				};
			}else{
				switch(kshu) {
					case KSHU0:
					  #if defined(HIGHEST_DRAM_DATARATE_DDR5500)
						if((seclib_get_devinfo_with_index(52) & 0xff) == 0)
						ret = HQA_VCORE_NV_LP5_KSHU0_PL_5500;
						else
					  #endif					  
						ret = HQA_VCORE_NV_LP5_KSHU0_PL;
						break;
					case KSHU1:
					  #if defined(HIGHEST_DRAM_DATARATE_DDR5500)
						if((seclib_get_devinfo_with_index(52) & 0xff) == 0)
						ret = HQA_VCORE_NV_LP5_KSHU1_PL_5500;
						else
					  #endif
						ret = HQA_VCORE_NV_LP5_KSHU1_PL;
						break;
					case KSHU2:
						ret = HQA_VCORE_NV_LP5_KSHU2_PL;
						break;
					case KSHU3:
						ret = HQA_VCORE_NV_LP5_KSHU3_PL;
						break;
					case KSHU4:
						ret = HQA_VCORE_NV_LP5_KSHU4_PL;
						break;
					case KSHU5:
						ret = HQA_VCORE_NV_LP5_KSHU5_PL;
						break;
					case KSHU6:
						ret = HQA_VCORE_NV_LP5_KSHU6_PL;
						break;
					case KSHU7:
						ret = HQA_VCORE_NV_LP5_KSHU7_PL;
						break;
					case KSHU8:
						ret = HQA_VCORE_NV_LP5_KSHU8_PL;
						break;
				};
			}
#endif
		}
	}
	return ret;
}
#endif

#ifdef DRAM_ADAPTIVE
static int update_dram_setting(EMI_SETTINGS *default_emi_setting, unsigned int dram_type, DRAM_INFO_BY_MRR_T *dram_info)
{
	default_emi_setting->type = dram_type;

	if (dram_info != NULL) {
		default_emi_setting->DRAM_RANK_SIZE[0] = (u64)dram_info->u8MR8Density[0];
		default_emi_setting->DRAM_RANK_SIZE[1] = (u64)dram_info->u8MR8Density[1];

		default_emi_setting->iLPDDR3_MODE_REG_5 = dram_info->u2MR5VendorID;

		if (dram_info->u4RankNum == 1) { // single rank
			if (dram_info->u1DieNum[RANK_0] == 1)
				default_emi_setting->dram_cbt_mode_extern = CBT_R0_R1_NORMAL;
			else if (dram_info->u1DieNum[RANK_0] == 2)
				default_emi_setting->dram_cbt_mode_extern = CBT_R0_R1_BYTE;
			else
				return -1;
		} else if (dram_info->u4RankNum == 2) { // dual rank
			if ((dram_info->u1DieNum[RANK_0] == 1) && (dram_info->u1DieNum[RANK_1] == 1))
				default_emi_setting->dram_cbt_mode_extern = CBT_R0_R1_NORMAL;
			else if ((dram_info->u1DieNum[RANK_0] == 1) && (dram_info->u1DieNum[RANK_1] == 2))
				default_emi_setting->dram_cbt_mode_extern = CBT_R0_NORMAL_R1_BYTE;
			else if ((dram_info->u1DieNum[RANK_0] == 2) && (dram_info->u1DieNum[RANK_1] == 1))
				default_emi_setting->dram_cbt_mode_extern = CBT_R0_BYTE_R1_NORMAL;
			else if ((dram_info->u1DieNum[RANK_0] == 2) && (dram_info->u1DieNum[RANK_1] == 2))
				default_emi_setting->dram_cbt_mode_extern = CBT_R0_R1_BYTE;
			else
				return -2;
		} else
			return -3;
	} else
		return -4;

	return 0;
}

static int decode_emi_info(EMI_INFO_T *emi_info, unsigned int dram_type, DRAM_INFO_BY_MRR_T *dram_info)
{
	unsigned int i;
	unsigned long long die_size;
	DRAMC_CTX_T *p = psCurrDramCtx;

	p->dram_type = dram_type;
	emi_info->dram_type = dram_type;
	emi_info->ch_num = 2;

	if (is_lp4_family(p)) { //LP4X
		emi_info->bank_width[0] = 3;
		emi_info->bank_width[1] = 3;
	} else {  //LP5
		emi_info->bank_width[0] = 4;
		emi_info->bank_width[1] = 4;
	}
		emi_info->col_width[0] = 10;
		emi_info->col_width[1] = 10;

	if (dram_info != NULL) {
		emi_info->rank_size[0] = (u64)dram_info->u8MR8Density[0];
		emi_info->rank_size[1] = (u64)dram_info->u8MR8Density[1];

		emi_info->rk_num = dram_info->u4RankNum;

		for (i = 0; i < emi_info->rk_num; i++) {
			die_size = emi_info->rank_size[i] / dram_info->u1DieNum[i];
			dramc_crit("die_size = 0x%x, dram_info = 0x%x dram_info->u1DieNum[i] = %d\n", die_size, dram_info->u1DieNum[i] << 4, dram_info->u1DieNum[i]);
			if (CHANNEL_NUM == 4)
				die_size >>= 2;
			else if (CHANNEL_NUM == 2)
				die_size >>= 1;

			switch (die_size | dram_info->u1DieNum[i] << 4) {
				case 0x010000010ULL: // 2Gb, x16, LP5
					emi_info->row_width[i] = 13;
					break;
				case 0x010000020ULL: // 2Gb, x8, LP5
				case 0x018000010ULL: // 3Gb, x16, LP5
				case 0x020000010ULL: // 4Gb, x16, LP5
					emi_info->row_width[i] = 14;
					break;
				case 0x018000020ULL: // 3Gb, x8, LP5
				case 0x020000020ULL: // 4Gb, x8, LP5
				case 0x030000010ULL: // 6Gb, x16, LP5
				case 0x040000010ULL: // 8Gb, x16, LP5
					emi_info->row_width[i] = 15;
					break;
				case 0x030000020ULL: // 6Gb, x8, LP5
				case 0x040000020ULL: // 8Gb, x8, LP5
				case 0x060000010ULL: // 12Gb, x16, LP5
				case 0x080000010ULL: // 16Gb, x16, LP5
					emi_info->row_width[i] = 16;
					break;
				case 0x060000020ULL: // 12Gb, x8, LP5
				case 0x080000020ULL: // 16Gb, x8, LP5
				case 0x0C0000010ULL: // 24Gb, x16, LP5
				case 0x100000010ULL: // 32Gb, x16, LP5
					emi_info->row_width[i] = 17;
					break;
				case 0x0C0000020ULL: // 24Gb, x8, LP5
				case 0x100000020ULL: // 32Gb, x8, LP5
					emi_info->row_width[i] = 18;
					break;
				default:
					dramc_crit("Error : die_size = %x, dram_info = %x\n", die_size, dram_info->u1DieNum[i] << 4);
					return -1;
			}
		}
	} else
		return -1;
	return 0;
}
#endif

#if (FOR_DV_SIMULATION_USED==0)
void dram_auto_detection(void)
{
	DRAM_INFO_BY_MRR_T dram_info;
	EMI_INFO_T emi_info;
	DRAM_CBT_MODE_EXTERN_T dram_mode = CBT_R0_R1_BYTE;
	DRAMC_CTX_T *p = psCurrDramCtx;
	unsigned int dram_type;
	int ret;

	memset((void *)&dram_info, 0, sizeof(dram_info));
	dram_type = (unsigned int)mt_get_dram_type();
	p->dram_type = dram_type;
	g_default_emi_setting.type &= ~0xFF;
	g_default_emi_setting.type |= (dram_type & 0xFF);
#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
	if (!is_lp4_family(p) ||
		read_offline_dram_mdl_data(&dram_info) < 0) {
#endif
		Init_DRAM(dram_type, dram_mode, &dram_info, GET_MDL_USED);
#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
		if (is_lp4_family(p))
			write_offline_dram_mdl_data(&dram_info);
	}
#endif

#if !__ETT__
#ifndef _FLASH_MODE_DA_
	ret = sec_dram_monitor(&dram_info, dram_type);
	if (ret)
		dramc_crit("[DRAMC] dram_monitor is not safe\n");
#endif
#endif

	ret = update_dram_setting(&g_default_emi_setting, dram_type, &dram_info);
	if (ret) {
		dramc_crit("[DRAMC] update_dram_setting err %d\n", ret);
		DRAMC_ASSERT(0);
	}

	ret = decode_emi_info(&emi_info, dram_type, &dram_info);
	if (ret) {
		dramc_crit("[DRAMC] decode_emi_info err %d\n", ret);
		DRAMC_ASSERT(0);
	}

	ret = update_emi_setting(&g_default_emi_setting, &emi_info);
	if (ret) {
		dramc_crit("[DRAMC] update_emi_setting err %d\n", ret);
		DRAMC_ASSERT(0);
	}
#if !__ETT__
	set_dram_mr5 (dram_info);
#endif
	auto_detect_done = 1;
}

void mt_set_emi(void)
{
	int index;
	/*unsigned int SW_CTRL_VC, HW_CTRL_VC;*/
	EMI_SETTINGS *emi_set;

#ifdef VOLTAGE_SEL
	update_voltage_select_info();
#endif

#if ENABLE_PINMUX_FOR_RANK_SWAP
	EMI_rank_swap_handle();
#endif
	// set voltage and hw trapping before mdl
	setup_dramc_voltage_by_pmic();
	
	if (mt_get_dram_type() == TYPE_LPDDR5X)
		memcpy((void *)&g_default_emi_setting, (void *)&g_default_emi_setting_lp5x,
			sizeof(g_default_emi_setting));
	if ((g_default_emi_setting.type & 0xFF) != mt_get_dram_type()) {
		dramc_crit("[dramc]HW Trapping not match the default seeting, Please Check dramc_common.h \n");
		DRAMC_ASSERT(0);
	}

#ifdef DDR_RESERVE_MODE
	if(g_ddr_reserve_enable==1 && g_ddr_reserve_success==0) {
		enable_gmc();
		Before_Init_DRAM_While_Reserve_Mode_fail(mt_get_dram_type());
		disable_gmc();
	}
#endif

#ifdef DRAM_ADAPTIVE
	dram_auto_detection();
#endif

	if(mr5 == 1) {
	  if (mt_get_dram_type() == TYPE_LPDDR5X){
	    dramc_set_vdd2L_voltage(HQA_VDD2L_SAMSUNG_NV_LP5X);
	  } else {
	    dramc_set_vdd2L_voltage(HQA_VDD2L_SAMSUNG_NV_LP5);
	  }
	}
		
	update_highest_dram_freq();
	emi_set = &g_default_emi_setting;

#if (CFG_DRAM_LOG_TO_STORAGE)
		log_start = 1;
		print("log_start=0x%x part_dram_data_addr_uart=0x%llx \n",log_start,part_dram_data_addr_uart);
#endif

	Init_DRAM((emi_set->type & 0xF), emi_set->dram_cbt_mode_extern, NULL, NORMAL_USED);

	switch_dramc_voltage_to_auto_mode();

#if !defined(SLT)
	restore_vcore_vddq_setting();
#endif

#if (CFG_DRAM_LOG_TO_STORAGE)
	log_start = 0;
	print("log_start=0x%x part_dram_data_addr_uart=0x%llx \n",log_start,part_dram_data_addr_uart);
#endif
#if 0
	{
		DRAMC_CTX_T * p = psCurrDramCtx;
		DramcRegDump(p);
	}
#endif
}
#endif

#define DRAMC_ADDR_SHIFT_CHN(addr, channel) (addr + (channel * 0x100000))

/* dst_pa is an exception for address.
 * dst_pa can be more than 32bit, so use u64 instead of PTR_T.
 */
void put_dummy_read_pattern(U64 dst_pa, PTR_T src_pa, unsigned int len)
{
	*((volatile unsigned int *)(PTR_T)(CQ_DMA_BASE + 0x018)) = 7 << 16;

	*((volatile unsigned int *)(PTR_T)(CQ_DMA_BASE + 0x01c)) = src_pa;
	*((volatile unsigned int *)(PTR_T)(CQ_DMA_BASE + 0x060)) = 0;

	*((volatile unsigned int *)(PTR_T)(CQ_DMA_BASE + 0x020)) = dst_pa & 0xffffffff;
	*((volatile unsigned int *)(PTR_T)(CQ_DMA_BASE + 0x064)) = dst_pa >> 32;

	*((volatile unsigned int *)(PTR_T)(CQ_DMA_BASE + 0x024)) = len;
	dsb();
	*((volatile unsigned int *)(PTR_T)(CQ_DMA_BASE + 0x008)) = 0x1;

	while(*((volatile unsigned int *)(PTR_T)(CQ_DMA_BASE + 0x008)));
}

unsigned long long get_dramc_addr(struct emi_addr_map *dram_addr, unsigned int offset)
{
	static char init_pattern = 0;
	unsigned int channel_num, rank_num;
	unsigned long long dummy_read_addr;
	unsigned long long rank_size[DRAMC_MAX_RK];
	unsigned int index, size_shift;
	unsigned int *src_addr;
	int channel_backup;
	int result = 0;

	channel_num = (unsigned int) get_dram_channel_nr();
	rank_num = (unsigned int) get_dram_rank_nr();
	get_rank_size_by_emi(rank_size);
	dummy_read_addr = 0x40000000;
	src_addr = (unsigned int *) 0x40000000;

	if ((unsigned int)dram_addr->channel >= channel_num)
	{
		dramc_crit("[DRAMC] invalid channel: %d channel_num : %d\n", dram_addr->channel, channel_num);
		return 0;
	}

	if ((unsigned int)dram_addr->rank >= rank_num)
	{
		dramc_crit("[DRAMC] invalid rank: %d\n", dram_addr->rank);
		return 0;
	}

	for (index = 0; index <= (unsigned int)dram_addr->rank; index++)
		dummy_read_addr += rank_size[index];

	dummy_read_addr -= offset; //0x14000 0000 - 0x40 = 0x13FFF FFC0
	dummy_read_addr &= ~(0x300); //0x13FFF FFC0 & 0xffffffcff = 0x13FFF FCC0

	channel_backup = dram_addr->channel;
	for(size_shift = 0; size_shift < 4 && offset; size_shift++) {
		result = emi_addr2dram(dummy_read_addr | (size_shift << 8), dram_addr);
		/*
		 * ch0: emi0, channel0
		 * ch1: emi0, channel1
		 * ch2: emi1, channel0
		 * ch3: emi1, channel1
		 */
		//dramc_debug("emi_addr2dram result = %d\n", result);
		if (channel_backup == (dram_addr->channel | (dram_addr->emi << 1)))
			break;

	}
	if (size_shift >= 4) {
		dramc_crit("[dramc] can't find right address size_shift:%d.\n", size_shift);
		DRAMC_ASSERT(0);
	}

	// TODO: should rewirte it when enable dummy read
	#if 0
	if (offset == 0x20) {
		for (index = 0; index < 4; index++)
			*(src_addr + index) = 0xAAAA5555;

		if (!init_pattern) {
			for (index = 0; index < channel_num; index++) {
				put_dummy_read_pattern(dummy_read_addr | (index << 8),
					(unsigned int) src_addr, 16);
			}

			init_pattern = 1;
		}
	}
	#endif
	return dummy_read_addr | (size_shift << 8);
}

unsigned long long get_dqsg_dramc_addr(struct emi_addr_map *dram_addr)
{
	return get_dramc_addr(dram_addr, 0x40); // 32-byte align for dqsg
}
unsigned long long get_dummy_read_addr(struct emi_addr_map *dram_addr)
{
	return get_dramc_addr(dram_addr, 0x20); // 32-byte align for dummy RW pattern
}

unsigned int get_ta2_addr(struct emi_addr_map *dram_addr)
{
	unsigned int ta2_addr;

	if (mt_get_dram_type() == TYPE_LPDDR4X) {
		get_dramc_addr(dram_addr, 0x1000);
		ta2_addr = (dram_addr->row & 0x3FFFF) << 12 | (dram_addr->bank & 0x7) << 9 | ((dram_addr->column & 0x3FF) >> 4) << 3;
	} else {
		get_dramc_addr(dram_addr, 0x2000);
		ta2_addr = (dram_addr->row & 0x3FFFF) << 13 | (dram_addr->bank & 0xF) << 9 | ((dram_addr->column & 0x3FF) >> 4) << 3;
	}
	//print("%s, channel: 0x%x, rank:0x%x \n", __func__, dram_addr->channel, dram_addr->rank);
	//print("%s, row: 0x%x, bank:0x%x, col:0x%x \n", __func__, dram_addr->row, dram_addr->bank, dram_addr->column);
	return ta2_addr;
}

void DramcDqsgDummyReadAddressSetting(DRAMC_CTX_T *p)
{
	U8 backup_channel = p->channel, backup_rank = p->rank;
	U8 channelIdx, rankIdx;
	struct emi_addr_map map;

	for (channelIdx = CHANNEL_A; channelIdx < CHANNEL_NUM; channelIdx++)
	{
		vSetPHY2ChannelMapping(p, channelIdx);
		for (rankIdx = RANK_0; rankIdx < RANK_MAX; rankIdx++)
		{
			vSetRank(p, rankIdx);
			map.emi = 0;
			map.channel= channelIdx;
			map.rank= rankIdx;
			map.row= 0;
			map.bank= 0;
			map.column= 0;

			get_dqsg_dramc_addr(&map);
			mcSHOW_DBG_MSG3("=== dqsg dummy read address: CH_%d, RK%d, row: 0x%x, bk: %d, col: 0x%x\n\n",
				map.channel, map.rank, map.row, map.bank, map.column);

			vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_RK_DQSG_DUMMY_RD_ADR), P_Fld(map.row, RK_DQSG_DUMMY_RD_ADR_DQSG_DMY_RD_ROW_ADR)
																			| P_Fld(map.column, RK_DQSG_DUMMY_RD_ADR_DQSG_DMY_RD_COL_ADR));
			vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_RK_DUMMY_RD_ADR), P_Fld(map.bank, RK_DUMMY_RD_ADR_DQSG_DMY_RD_BK)
																			| P_Fld(0, RK_DUMMY_RD_ADR_DMY_RD_LEN));
		}
	}

	vSetRank(p, backup_rank);
	vSetPHY2ChannelMapping(p, backup_channel);
}

void init_ta2_single_channel(unsigned int channel)
{
	unsigned int temp;
	struct emi_addr_map dram_addr;
	DRAMC_CTX_T *p = psCurrDramCtx;
	int test_cnt;
	U8 backup_rank = p->rank;

	channel_backup_and_set(p,channel);
	// disable self test engine1 and self test engine2
	temp = u4IO32Read4B(DRAMC_REG_TEST2_A3) & 0x1FFFFFFF;
	vIO32Write4B(DRAMC_REG_TEST2_A3, temp);

	// set rank address for test agent to auto
	temp = u4IO32Read4B(DRAMC_REG_TEST2_A4) & 0x8FFFFFFF;
	temp |= (0x4 << 28);
	vIO32Write4B(DRAMC_REG_TEST2_A4, temp);

	// set test for both rank0 and rank1
	temp = u4IO32Read4B(DRAMC_REG_TEST2_A3) & 0xFFFFFFF0;
	vIO32Write4B(DRAMC_REG_TEST2_A3, temp | 0x1);


	// set base address for test agent to reserved space
	dram_addr.channel = channel;
	dram_addr.rank = 0;

	vSetRank(p, RANK_0);
	temp = (u4IO32Read4B(DRAMC_REG_RK_TEST2_A1) & 0x00000007);
	vIO32Write4B(DRAMC_REG_RK_TEST2_A1, temp | get_ta2_addr(&dram_addr));
	
	dram_addr.rank = 1;
	vSetRank(p, RANK_1);
	temp = (u4IO32Read4B(DRAMC_REG_RK_TEST2_A1) & 0x00000007);
	vIO32Write4B(DRAMC_REG_RK_TEST2_A1, temp | get_ta2_addr(&dram_addr));
	vSetRank(p, backup_rank);
	
	// set test length (offset) to 0x20
	temp = (u4IO32Read4B(DRAMC_REG_TEST2_A2) & 0x0000000F) | (0x20 << 4);
	vIO32Write4B(DRAMC_REG_TEST2_A2, temp);

	// set TA2 pattern to the worst case
	test_cnt = (get_dram_rank_nr() > 1) ? 1 : 0;
	vIO32WriteFldAlign(DRAMC_REG_TEST2_A3, 0, TEST2_A3_TESTAUDPAT);
	vIO32WriteFldAlign(DRAMC_REG_TEST2_A3, test_cnt, TEST2_A3_TESTCNT);
	vIO32WriteFldAlign(DRAMC_REG_TEST2_A4, 1, TEST2_A4_TESTXTALKPAT);
	channel_restore(p);

	return;
}

#ifdef LAST_DRAMC

static unsigned int is_last_dramc_initialized(void)
{
	if(last_dramc_info_ptr->ta2_result_magic != LAST_DRAMC_MAGIC_PATTERN) {
		return 0;
	} else {
		return 1;
	}
}

void update_last_dramc_info(void)
{
	unsigned int chn;
	unsigned int gating_latch_result[CHANNEL_NUM] = {0};
	unsigned int emi_latch_result[CHANNEL_NUM] = {0};
	unsigned int emi_hang_dbg1_result[CHANNEL_NUM] = {0};
	unsigned int temp;
	unsigned int *curr;
	DRAMC_CTX_T *p = psCurrDramCtx;

	// init checksum and magic pattern
	if(last_dramc_info_ptr->ta2_result_magic != LAST_DRAMC_MAGIC_PATTERN) {
		last_dramc_info_ptr->ta2_result_magic = LAST_DRAMC_MAGIC_PATTERN;
		for (chn =0; chn < CHANNEL_NUM; ++chn) {
			last_dramc_info_ptr->ta2_result_last[chn] = 0;
			last_dramc_info_ptr->ta2_result_past[chn] = 0;
			last_dramc_info_ptr->emi_hang_info[chn] = 0;
		}
		last_dramc_info_ptr->ta2_result_checksum = LAST_DRAMC_MAGIC_PATTERN;
		last_dramc_info_ptr->reboot_count = 0;
		last_dramc_info_ptr->mr5 = mr5;
		last_dramc_info_ptr->mr6 = mr6;
		last_dramc_info_ptr->mr7 = mr7;
		last_dramc_info_ptr->mr8 = mr8;
	} else {
		last_dramc_info_ptr->ta2_result_checksum ^= last_dramc_info_ptr->reboot_count;
		last_dramc_info_ptr->reboot_count++;
		last_dramc_info_ptr->ta2_result_checksum ^= last_dramc_info_ptr->reboot_count;
	}

	// TODO: check DCS status

	// read data from latch register and reset
	channel_backup_and_set(p, CHANNEL_A);
	chn = 0;
	do {
		//dramc_crit("[LastDRAMC] latch result before RST: %x\n", u4IO32Read4B(DRAMC_ADDR_SHIFT_CHN(DRAMC_REG_WDT_DBG_SIGNAL, chn)))
		gating_latch_result[chn] = u4IO32Read4B(DRAMC_REG_WDT_DBG_SIGNAL) & 0x1FFFF;
		emi_latch_result[chn] = u4IO32Read4B(DRAMC_REG_EMI_HANG_DGB) & 0x1FF01FF;
		emi_hang_dbg1_result[chn] =  u4IO32Read4B(DRAMC_REG_EMI_HANG_DBG1) & 0x1FF01FF;
		temp = u4IO32Read4B(DRAMC_REG_WDT_RST);
		vIO32Write4B(DRAMC_REG_WDT_RST, temp | 0x00000001);
		vIO32Write4B(DRAMC_REG_WDT_RST, temp & 0xFFFFFFFE);
		//dramc_crit("[LastDRAMC] latch result before RST: %x\n", u4IO32Read4B(DRAMC_ADDR_SHIFT_CHN(DRAMC_REG_WDT_DBG_SIGNAL, chn)))
		channel_advance(p);
		chn++;
	} while (!channel_process_complete(p));
	channel_restore(p);

	for (chn =0; chn < CHANNEL_NUM; ++chn) {
		last_dramc_info_ptr->ta2_result_checksum ^= last_dramc_info_ptr->ta2_result_past[chn] ^ gating_latch_result[chn];
		last_dramc_info_ptr->ta2_result_checksum ^= last_dramc_info_ptr->emi_hang_info[chn] ^ emi_latch_result[chn];
		last_dramc_info_ptr->ta2_result_past[chn] = last_dramc_info_ptr->ta2_result_last[chn];
		last_dramc_info_ptr->ta2_result_last[chn] = gating_latch_result[chn];
		last_dramc_info_ptr->emi_hang_info[chn] = emi_latch_result[chn];
		last_dramc_info_ptr->emi_hang_dbg1_info[chn] = emi_hang_dbg1_result[chn];
	}

	for (temp = 0; temp < sizeof(LAST_DRAMC_INFO_T) / sizeof(temp); temp++) {
		curr = (unsigned int *)last_dramc_info_ptr + temp;
		dramc_crit("[LastDRAMC] 0x%x: 0x%x\n", curr, *curr);
	}

	return;
}

void init_ta2_all_channel(void)
{
	unsigned int chn = 0;
	DRAMC_CTX_T *p = psCurrDramCtx;

	update_last_dramc_info();

	//cache flush after update dramc info
	#if CFG_ENABLE_DCACHE
	plat_clean_invalidate_dcache();
	#endif
	// TODO: consider DCS
	for (chn = 0; chn < CHANNEL_NUM; ++chn)
		init_ta2_single_channel(chn);
}


unsigned int check_gating_err_in_dramc_latch(void)
{
	unsigned int chn = 0, ret = 0;
	DRAMC_CTX_T *p = psCurrDramCtx;
	U8 channel_bk = p->channel;

	if ((g_boot_reason == BR_POWER_KEY) || (g_boot_reason == BR_USB)
			|| mtk_wdt_is_pmic_full_reset() || (is_last_dramc_initialized() == 0)){
		dramc_crit("for cold boot, always return 0\n");
		return 0;
	}


	channel_backup_and_set(p,CHANNEL_A);
	do {
		dramc_crit("[dramc] latch check in channel %d (0x%x)\n",
				p->channel, u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_WDT_DBG_SIGNAL)));
		if (u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_WDT_DBG_SIGNAL)) & 0x4000) {
			dramc_crit("[dramc] found gating error in channel %d (0x%x)\n",
					chn, u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_WDT_DBG_SIGNAL)));
			ret |= (1 << (p->channel));
		}
		channel_advance(p);
	} while(!channel_process_complete(p));
	channel_restore(p);

	vSetPHY2ChannelMapping(p, channel_bk);

	return ret;
}

void dram_fatal_exception_detection_start(void)
{
	last_dramc_info_ptr = (LAST_DRAMC_INFO_T *) get_dbg_info_base(KEY_LAST_DRAMC);

#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
#if (CFG_DRAM_PARTITION)
	part_dram_data_addr = get_part_addr("dram_para"); // addr = 0x108000
#else
	part_dram_data_addr = get_part_addr("boot_para") + 0x100000; // addr = 0x108000
#endif

	if (part_dram_data_addr != 0x0)
		dramc_crit("[dramc] init partition address is 0x%llx\n", part_dram_data_addr);
	else {
		dramc_crit("[dramc] init partition address is incorrect !!!\n");
	}
#endif

#if defined(DRAM_SLT)
#if (CFG_DRAM_PARTITION)
	part_dram_data_addr_slt = get_part_addr("dram_para");
#else
	part_dram_data_addr_slt = get_part_addr("boot_para") + 0x100000;
#endif
#endif

	if ((g_boot_reason == BR_POWER_KEY) || (g_boot_reason == BR_USB)
			|| mtk_wdt_is_pmic_full_reset() || (is_last_dramc_initialized() == 0)){
		/* cold boot: initialize last_dram_fatal_err_flag and dram_fatal_err_flag */
		dramc_crit("[dramc] init SRAM region for DRAM exception detection\n");
		last_dramc_info_ptr->last_fatal_err_flag = 0x0;
		last_dramc_info_ptr->storage_api_err_flag = 0x0;
		dram_fatal_init_stberr();
	} else {
		last_dramc_info_ptr->last_fatal_err_flag = last_dramc_info_ptr->fatal_err_flag;
		last_dramc_info_ptr->storage_api_err_flag = 0x0;
		dram_fatal_backup_stberr();
		dram_fatal_init_stberr();
	}

	last_dramc_info_ptr->dram_type = mt_get_dram_type();
	last_dramc_info_ptr->fatal_err_flag = 0x1U << OFFSET_DRAM_FATAL_ERR;
	dsb();
}

void dram_fatal_exception_detection_end(void)
{
	last_dramc_info_ptr->fatal_err_flag = 0x0;
	dsb();
}

unsigned int check_dram_fatal_exception(void)
{
	dramc_crit("[dramc] DRAM_FATAL_ERR_FLAG = 0x%x\n", last_dramc_info_ptr->fatal_err_flag);

	return ((last_dramc_info_ptr->fatal_err_flag & ~((0x1U << OFFSET_DRAM_FATAL_ERR)|DDR_RSV_MODE_ERR_MASK)) != 0x0) ? 1 : 0;
}

unsigned int check_last_dram_fatal_exception(void)
{
	dramc_crit("[dramc] LAST_DRAM_FATAL_ERR_FLAG = 0x%x\n", last_dramc_info_ptr->last_fatal_err_flag);

	return ((last_dramc_info_ptr->last_fatal_err_flag & ~(DDR_RSV_MODE_ERR_MASK)) != 0x0) ? 1 : 0;
}

void dram_fatal_set_ta2_err(unsigned int chn, unsigned int err_code)
{
	unsigned int shift = OFFSET_DRAM_TA2_ERR + 2 * chn, ret;

	if (chn > CHANNEL_NUM)
		return;

	ret = last_dramc_info_ptr->fatal_err_flag & ~(0x7 << shift);
	last_dramc_info_ptr->fatal_err_flag = ret | ((err_code & 0x7) << shift);
	dsb();
}

void dram_fatal_set_gating_err(unsigned int chn, unsigned int err_code)
{
	unsigned int shift = OFFSET_DRAM_GATING_ERR + 4 * chn, ret;

	if (chn > CHANNEL_NUM)
		return;

	ret = last_dramc_info_ptr->fatal_err_flag & ~(0xf << shift);
	last_dramc_info_ptr->fatal_err_flag = ret | ((err_code & 0xf) << shift);
	dsb();
}

void dram_fatal_init_stberr(void)
{
	int i = 0, j = 0;
	for (i = 0; i < CHANNEL_NUM; i++)
		for (j = 0; j < 2; j++) //rank number
			last_dramc_info_ptr->gating_err[i][j] = 0x0;

	dsb();
}

void dram_fatal_backup_stberr(void)
{
	int i = 0, j = 0;
	for (i = 0; i < CHANNEL_NUM; i++)
		for (j = 0; j < 2; j++)  //rank number
			last_dramc_info_ptr->last_gating_err[i][j] = last_dramc_info_ptr->gating_err[i][j];

	dsb();
}

void dram_fatal_set_stberr(unsigned int chn, unsigned int rk, unsigned int err_code)
{
	if ((chn > 1) || (rk > 1))
		return;

	last_dramc_info_ptr->gating_err[chn][rk] = err_code;

	dsb();
}

void dram_fatal_set_err(unsigned int err_code, unsigned int mask, unsigned int offset)
{
	unsigned int ret;

	ret = last_dramc_info_ptr->fatal_err_flag & ~(mask << offset);
	last_dramc_info_ptr->fatal_err_flag = ret | ((err_code & mask) << offset);
	dsb();
}

#endif

#if (FOR_DV_SIMULATION_USED==0) // for DV sim build pass
int doe_get_config(char* feature)
{
#if defined(ENABLE_DOE)
	char *doe_feature = dconfig_getenv(feature);
	int doe_result = atoi(doe_feature);
	dramc_crit("DOE force setting %s=%d\n", feature, doe_result);
	return doe_result;
#else
	return 0;
#endif
}
#endif

#if (CFG_DRAM_LOG_TO_STORAGE)
void log_to_storage(const char c)
{
	int ret, clr_count;
	blkdev_t *bootdev = NULL;
	static u8 logen = 0;

	bootdev = blkdev_get(CFG_BOOT_DEV);

	if (log_start && (!logen)) {
		logen = 1;
		logcount = 0;
#if (CFG_DRAM_PARTITION)
		part_dram_data_addr_uart = get_part_addr("dram_para"); // addr = 0x1f300000, the first 1MB for debug
#else
		part_dram_data_addr_uart = get_part_addr("boot_para") + 0x100000; // addr = 0x1f300000, the first 1MB for debug
#endif
		memset(&logbuf, 0, sizeof(logbuf));
		for (clr_count = 0; clr_count < 3072 ; clr_count++) //3M
				ret = blkdev_write(bootdev, (part_dram_data_addr_uart + (1024 * clr_count)),
				1024, (u8*)&logbuf, storage_get_part_id(STORAGE_PHYS_PART_USER));
	}

	if (log_start) {
		if (((((char) c >> 4) & 0x7) > 1) & ((((char) c >> 4) & 0x7) < 7))
			logbuf[logcount] = ((char) c & 0xF0) | (((char) c >> 2) & 0x03)
			| (((char) c << 2) & 0x0C);
			else
				logbuf[logcount] = (char) c;
		logcount = logcount + 1;
		//write to storage
		if (logcount==1024) {
			logcount = 0;
			ret = blkdev_write(bootdev, part_dram_data_addr_uart, 1024,
					(u8*)&logbuf, storage_get_part_id(STORAGE_PHYS_PART_USER));
			part_dram_data_addr_uart = part_dram_data_addr_uart + 1024;
		}
	}
}
#endif
#if SUPPORT_SAVE_TIME_FOR_CALIBRATION

#if !__ETT__
#include <blkdev.h>
#include <partition.h>
#include <pl_version.h>
#else
#include "ett_common.h"
#include "emi.h"
#endif

u32 g_dram_storage_api_err_code;

static u16 crc16(const u8* data, u32 length){
	u8 x;
	u16 crc = 0xFFFF;

	while (length--) {
		x = crc >> 8 ^ *data++;
		x ^= x >> 4;
		crc = (crc << 8) ^ ((u8)(x << 12)) ^ ((u8)(x <<5)) ^ ((u8)x);
	}
	return crc;
}

static void assign_checksum_for_dram_data(DRAM_CALIBRATION_SHU_DATA_T *shu_data)
{
	/* need to initialize checksum to 0 before calculation */
	shu_data->checksum = 0;
	shu_data->checksum = crc16((u8*)shu_data, sizeof(*shu_data));
}

static int check_checksum_for_dram_data(DRAM_CALIBRATION_SHU_DATA_T *shu_data)
{
	u16 checksum_in_storage = shu_data->checksum;

	assign_checksum_for_dram_data(shu_data);

	return (shu_data->checksum == checksum_in_storage) ? 1 : 0;
}

#if !__ETT__ && FAST_K
static void assign_checksum_for_mdl_data(DRAM_CALIBRATION_MRR_DATA_T *mrr_info)
{
	/* need to initialize checksum to 0 before calculation */
	mrr_info->checksum = 0;
	mrr_info->checksum = crc16((u8*)mrr_info, sizeof(*mrr_info));
}

static int check_checksum_for_mdl_data(DRAM_CALIBRATION_MRR_DATA_T *mrr_info)
{
	u16 checksum_in_storage = mrr_info->checksum;

	assign_checksum_for_mdl_data(mrr_info);

	return (mrr_info->checksum == checksum_in_storage) ? 1 : 0;
}

static int read_offline_dram_mdl_data(DRAM_INFO_BY_MRR_T *DramInfo)
{
	int i, ret;
	u16 emi_checksum;
	blkdev_t *bootdev = NULL;
	DRAM_CALIBRATION_HEADER_T hdr;
	DRAM_CALIBRATION_MRR_DATA_T mrr_info;
	DRAM_CALIBRATION_SHU_DATA_T shu_data;
	DRAM_CALIBRATION_DATA_T *datap = NULL;

	if (dram_offline_data_flags)
		goto exit;

	if (DramInfo == NULL) {
		dramc_crit("[dramc] DramInfo == NULL, skip\n");
		dram_offline_data_flags = ERR_NULL_POINTER;
		goto exit;
	}

	bootdev = blkdev_get(CFG_BOOT_DEV);
	if (bootdev == NULL) {
		dramc_crit("[dramc] can't find boot device(%d)\n", CFG_BOOT_DEV);
		dram_offline_data_flags = ERR_BLKDEV_NOT_FOUND;
		goto exit;
	}

	if (!part_dram_data_addr) {
		dram_offline_data_flags = ERR_BLKDEV_NO_PART;
		goto exit;
	}

	ret = blkdev_read(bootdev, part_dram_data_addr, sizeof(hdr), (u8*)&hdr, storage_get_part_id(STORAGE_PHYS_PART_USER));
	if (ret != 0) {
		dramc_crit("[dramc] blkdev_read %s failed\n", "hdr");
		dram_offline_data_flags = ERR_BLKDEV_READ_FAIL;
		goto exit;
	}

	if (hdr.pl_version != PL_VERSION) {
		/* current preloader version does not match the calibration hdr in storage -> erase the partition */
		dramc_crit("[dramc] PL_VERSION is updated, erase the DRAM shu_data\n");

		shu_data.checksum = 0;

		/* clear each shuffle */
		for (i = 0; i < DRAM_DFS_SRAM_MAX; i++) {
                       //ret = blkdev_write(bootdev, part_dram_data_addr + ((unsigned long) &datap->data[i]), sizeof(shu_data), (u8*)&shu_data, storage_get_part_id(STORAGE_PHYS_PART_USER));
			ret = blkdev_write(bootdev, part_dram_data_addr + ((unsigned long) (offsetof(DRAM_CALIBRATION_DATA_T, data[i]))), sizeof(shu_data), (u8*)&shu_data, storage_get_part_id(STORAGE_PHYS_PART_USER));
			dramc_crit("i = %d, offsetof(DRAM_CALIBRATION_DATA_T, data[i]) = %d\n", i, offsetof(DRAM_CALIBRATION_DATA_T, data[i]));
			if (ret != 0) {
				dramc_crit("[dramc] blkdev_write failed\n");
				dram_offline_data_flags = ERR_BLKDEV_WRITE_FAIL;
				goto exit;
			}
		}

		dram_offline_data_flags = ERR_PL_UPDATED;
		goto exit;
	}

	/* check magic number */
	if (hdr.magic_number != DRAM_CALIBRATION_DATA_MAGIC) {
		dramc_crit("[dramc] magic number mismatch\n");
		dram_offline_data_flags = ERR_MAGIC_NUMBER;
		goto exit;
	}

	//ret = blkdev_read(bootdev, part_dram_data_addr + ((unsigned long) &datap->mrr_info), sizeof(mrr_info), (u8*)&mrr_info, storage_get_part_id(STORAGE_PHYS_PART_USER));
	ret = blkdev_read(bootdev, part_dram_data_addr + ((unsigned long) (offsetof(DRAM_CALIBRATION_DATA_T, mrr_info))), sizeof(mrr_info), (u8*)&mrr_info, storage_get_part_id(STORAGE_PHYS_PART_USER));
	dramc_crit("offsetof(DRAM_CALIBRATION_DATA_T, mrr_info) = %d\n", offsetof(DRAM_CALIBRATION_DATA_T, mrr_info));
	if (ret != 0) {
		dramc_crit("[dramc] blkdev_read %s failed\n", "data");
		dram_offline_data_flags = ERR_BLKDEV_READ_FAIL;
		goto exit;
	}

	/* check checksum */
	if (check_checksum_for_mdl_data(&mrr_info) != 1) {
		dramc_crit("[dramc] checksum failed\n");
		dram_offline_data_flags = ERR_CHECKSUM;

		goto exit;
	}

	emi_checksum = crc16((u8*)qvl_list, sizeof(qvl_list));

	if (emi_checksum != mrr_info.emi_checksum) {
		dramc_crit("[dramc] emi checksum failed\n");
		dram_offline_data_flags = ERR_CHECKSUM;

		goto exit;
	}

	/* copy the data stored in storage to the data structure for calibration */
	memcpy(DramInfo, &(mrr_info.DramInfo), sizeof(*DramInfo));

exit:
	if (dram_offline_data_flags)
		SET_DRAM_STORAGE_API_ERR(dram_offline_data_flags, DRAM_STORAGE_API_READ);

	return 0 - dram_offline_data_flags;
}

static int write_offline_dram_mdl_data(DRAM_INFO_BY_MRR_T *DramInfo)
{
	int ret;
	blkdev_t *bootdev = NULL;
	DRAM_CALIBRATION_HEADER_T hdr;
	DRAM_CALIBRATION_MRR_DATA_T mrr_info;
	DRAM_CALIBRATION_DATA_T *datap = NULL;

	if (DramInfo == NULL) {
		dramc_crit("[dramc] DramInfo == NULL, skip\n");
		SET_DRAM_STORAGE_API_ERR(ERR_NULL_POINTER, DRAM_STORAGE_API_WRITE);
		return -ERR_NULL_POINTER;
	}

	bootdev = blkdev_get(CFG_BOOT_DEV);
	if (bootdev == NULL) {
		dramc_crit("[dramc] can't find boot device(%d)\n", CFG_BOOT_DEV);
		SET_DRAM_STORAGE_API_ERR(ERR_BLKDEV_NOT_FOUND, DRAM_STORAGE_API_WRITE);
		return -ERR_BLKDEV_NOT_FOUND;
	}

	if (!part_dram_data_addr) {
		return -ERR_BLKDEV_NO_PART;
	}

	memcpy(&(mrr_info.DramInfo), DramInfo, sizeof(*DramInfo));

#if 0
	/* assign PL version */
	hdr.pl_version = PL_VERSION;

	/* assign magic number */
	hdr.magic_number = DRAM_CALIBRATION_DATA_MAGIC;

	/* assign api error code */
	hdr.calib_err_code = g_dram_storage_api_err_code;

	ret = blkdev_write(bootdev, part_dram_data_addr, sizeof(hdr), (u8*)&hdr, PART_ID_DRAM_DATA);
	if (ret != 0) {
		dramc_crit("[dramc] blkdev_write failed\n");
		SET_DRAM_STORAGE_API_ERR(ERR_BLKDEV_WRITE_FAIL, DRAM_STORAGE_API_WRITE);
		return -ERR_BLKDEV_WRITE_FAIL;
	}
#endif

	/* calculate and assign checksum */
	mrr_info.emi_checksum = crc16((u8*)qvl_list, sizeof(qvl_list));
	assign_checksum_for_mdl_data(&mrr_info);

	//ret = blkdev_write(bootdev, part_dram_data_addr + ((unsigned long) &datap->mrr_info), sizeof(mrr_info), (u8*)&mrr_info, storage_get_part_id(STORAGE_PHYS_PART_USER));
	ret = blkdev_write(bootdev, part_dram_data_addr + ((unsigned long) (offsetof(DRAM_CALIBRATION_DATA_T, mrr_info))), sizeof(mrr_info), (u8*)&mrr_info, storage_get_part_id(STORAGE_PHYS_PART_USER));
	dramc_crit("offsetof(DRAM_CALIBRATION_DATA_T, mrr_info) = %d\n", offsetof(DRAM_CALIBRATION_DATA_T, mrr_info));
	if (ret != 0) {
		dramc_crit("[dramc] blkdev_write failed\n");
		SET_DRAM_STORAGE_API_ERR(ERR_BLKDEV_WRITE_FAIL, DRAM_STORAGE_API_WRITE);
		return -ERR_BLKDEV_WRITE_FAIL;
	}

	return 0;
}

int read_offline_dram_calibration_data(DRAM_DFS_SRAM_SHU_T shuffle, SAVE_TIME_FOR_CALIBRATION_T *offLine_SaveData)
{
	int i, ret;
	blkdev_t *bootdev = NULL;
	DRAM_CALIBRATION_HEADER_T hdr;
	DRAM_CALIBRATION_SHU_DATA_T shu_data;
	DRAM_CALIBRATION_DATA_T *datap = NULL;
	unsigned char *ch;

	if (dram_offline_data_flags)
		goto exit;

	if (offLine_SaveData == NULL) {
		dramc_crit("[dramc] offLine_SaveData == NULL, skip\n");
		dram_offline_data_flags = ERR_NULL_POINTER;
		goto exit;
	}

	bootdev = blkdev_get(CFG_BOOT_DEV);
	if (bootdev == NULL) {
		dramc_crit("[dramc] can't find boot device(%d)\n", CFG_BOOT_DEV);
		dram_offline_data_flags = ERR_BLKDEV_NOT_FOUND;
		goto exit;
	}

	if (!part_dram_data_addr) {
		dram_offline_data_flags = ERR_BLKDEV_NO_PART;
		goto exit;
	}

	ret = blkdev_read(bootdev, part_dram_data_addr, sizeof(hdr), (u8*)&hdr, storage_get_part_id(STORAGE_PHYS_PART_USER));
	if (ret != 0) {
		dramc_crit("[dramc] blkdev_read %s failed\n", "hdr");
		dram_offline_data_flags = ERR_BLKDEV_READ_FAIL;
		goto exit;
	}

	/* check preloader version */
	if (hdr.pl_version != PL_VERSION) {
		/* current preloader version does not match the calibration hdr in storage -> erase the partition */
		dramc_crit("[dramc] PL_VERSION is updated, erase the DRAM shu_data\n");

		shu_data.checksum = 0;

		/* clear each shuffle */
		for (i = 0; i < DRAM_DFS_SRAM_MAX; i++) {
			//ret = blkdev_write(bootdev, part_dram_data_addr + ((unsigned long) &datap->data[i]),
			//sizeof(shu_data), (u8*)&shu_data, storage_get_part_id(STORAGE_PHYS_PART_USER));
			ret = blkdev_write(bootdev, part_dram_data_addr + ((unsigned long) (offsetof(DRAM_CALIBRATION_DATA_T, data[i]))),
			sizeof(shu_data), (u8*)&shu_data, storage_get_part_id(STORAGE_PHYS_PART_USER));
			dramc_crit("i = %d, offsetof(DRAM_CALIBRATION_DATA_T, data[i]) = %d\n", i, offsetof(DRAM_CALIBRATION_DATA_T, data[i]));
			if (ret != 0) {
				dramc_crit("[dramc] blkdev_write failed\n");
				dram_offline_data_flags = ERR_BLKDEV_WRITE_FAIL;
				goto exit;
			}
		}

		dram_offline_data_flags = ERR_PL_UPDATED;
		goto exit;
	}

	/* check magic number */
	if (hdr.magic_number != DRAM_CALIBRATION_DATA_MAGIC) {
		dramc_crit("[dramc] magic number mismatch\n");
		dram_offline_data_flags = ERR_MAGIC_NUMBER;
		goto exit;
	}

	//ret = blkdev_read(bootdev, part_dram_data_addr + ((unsigned long) &datap->data[shuffle]),
	//              sizeof(shu_data), (u8*)&shu_data, storage_get_part_id(STORAGE_PHYS_PART_USER));
	ret = blkdev_read(bootdev, part_dram_data_addr + ((unsigned long) (offsetof(DRAM_CALIBRATION_DATA_T, data[shuffle]))),
			sizeof(shu_data), (u8*)&shu_data, storage_get_part_id(STORAGE_PHYS_PART_USER));
	dramc_crit("offsetof(DRAM_CALIBRATION_DATA_T, data[shuffle]) = %d", offsetof(DRAM_CALIBRATION_DATA_T, data[shuffle]));
	if (ret != 0) {
		dramc_crit("[dramc] blkdev_read %s failed\n", "data");
		dram_offline_data_flags = ERR_BLKDEV_READ_FAIL;
		goto exit;
	}

	/* check checksum */
	if (check_checksum_for_dram_data(&shu_data) != 1) {
		dramc_crit("[dramc] checksum failed\n");
		dram_offline_data_flags = ERR_CHECKSUM;
		goto exit;
	}

	/* copy the data stored in storage to the data structure for calibration */
	memcpy(offLine_SaveData, &(shu_data.calibration_data), sizeof(*offLine_SaveData));

exit:
	if (dram_offline_data_flags)
		SET_DRAM_STORAGE_API_ERR(dram_offline_data_flags, DRAM_STORAGE_API_READ);

	return 0 - dram_offline_data_flags;
}

int write_offline_dram_calibration_data(DRAM_DFS_SRAM_SHU_T shuffle, SAVE_TIME_FOR_CALIBRATION_T *offLine_SaveData)
{
	int ret;
	blkdev_t *bootdev = NULL;
	DRAM_CALIBRATION_HEADER_T hdr;
	DRAM_CALIBRATION_SHU_DATA_T shu_data;
	DRAM_CALIBRATION_DATA_T *datap = NULL;

	if (offLine_SaveData == NULL) {
		dramc_crit("[dramc] offLine_SaveData == NULL, skip\n");
		SET_DRAM_STORAGE_API_ERR(ERR_NULL_POINTER, DRAM_STORAGE_API_WRITE);
		return -ERR_NULL_POINTER;
	}

	bootdev = blkdev_get(CFG_BOOT_DEV);
	if (bootdev == NULL) {
		dramc_crit("[dramc] can't find boot device(%d)\n", CFG_BOOT_DEV);
		SET_DRAM_STORAGE_API_ERR(ERR_BLKDEV_NOT_FOUND, DRAM_STORAGE_API_WRITE);
		return -ERR_BLKDEV_NOT_FOUND;
	}

	if (!part_dram_data_addr) {
		return -ERR_BLKDEV_NO_PART;
	}

	memcpy(&(shu_data.calibration_data), offLine_SaveData, sizeof(*offLine_SaveData));
    
	/* assign PL version */
	hdr.pl_version = PL_VERSION;

	/* assign magic number */
	hdr.magic_number = DRAM_CALIBRATION_DATA_MAGIC;

	/* assign api error code */
	hdr.calib_err_code = g_dram_storage_api_err_code;

	ret = blkdev_write(bootdev, part_dram_data_addr, sizeof(hdr), (u8*)&hdr, storage_get_part_id(STORAGE_PHYS_PART_USER));
	if (ret != 0) {
		dramc_crit("[dramc] blkdev_write failed\n");
		SET_DRAM_STORAGE_API_ERR(ERR_BLKDEV_WRITE_FAIL, DRAM_STORAGE_API_WRITE);
		return -ERR_BLKDEV_WRITE_FAIL;
	}

	/* calculate and assign checksum */
	assign_checksum_for_dram_data(&shu_data);

	//ret = blkdev_write(bootdev, part_dram_data_addr + ((unsigned long) &datap->data[shuffle]), sizeof(shu_data), (u8*)&shu_data, storage_get_part_id(STORAGE_PHYS_PART_USER));
	ret = blkdev_write(bootdev, part_dram_data_addr + ((unsigned long) (offsetof(DRAM_CALIBRATION_DATA_T, data[shuffle]))), sizeof(shu_data), (u8*)&shu_data, storage_get_part_id(STORAGE_PHYS_PART_USER));
	dramc_crit("offsetof(DRAM_CALIBRATION_DATA_T, data[shuffle]) = %d\n", offsetof(DRAM_CALIBRATION_DATA_T, data[shuffle]));
	if (ret != 0) {
		dramc_crit("[dramc] blkdev_write failed\n");
		SET_DRAM_STORAGE_API_ERR(ERR_BLKDEV_WRITE_FAIL, DRAM_STORAGE_API_WRITE);
		return -ERR_BLKDEV_WRITE_FAIL;
	}

	return 0;
}

int clean_dram_calibration_data(void)
{
	int i, ret;
	blkdev_t *bootdev = NULL;
	DRAM_CALIBRATION_HEADER_T hdr;
	DRAM_CALIBRATION_SHU_DATA_T shu_data;
	DRAM_CALIBRATION_DATA_T *datap = NULL;

	bootdev = blkdev_get(CFG_BOOT_DEV);
	if (bootdev == NULL) {
		dramc_crit("[dramc] can't find block device(%d)\n", CFG_BOOT_DEV);
		SET_DRAM_STORAGE_API_ERR(ERR_BLKDEV_NOT_FOUND, DRAM_STORAGE_API_CLEAN);
		return -ERR_BLKDEV_NOT_FOUND;
	}

	if (!part_dram_data_addr) {
		return -ERR_BLKDEV_NO_PART;
	}

	memset(&hdr, 0, sizeof(hdr));

	ret = blkdev_write(bootdev, part_dram_data_addr, sizeof(hdr), (u8*)&hdr, storage_get_part_id(STORAGE_PHYS_PART_USER));
	if (ret != 0) {
		dramc_crit("[dramc] blkdev_write failed\n");
		SET_DRAM_STORAGE_API_ERR(ERR_BLKDEV_WRITE_FAIL, DRAM_STORAGE_API_CLEAN);
		return -ERR_BLKDEV_WRITE_FAIL;
	}
    
	shu_data.checksum = 0;

	/* clear each shuffle */
	for (i = 0; i < DRAM_DFS_SRAM_MAX; i++) {
		//ret = blkdev_write(bootdev, part_dram_data_addr + ((unsigned long) &datap->data[i]), sizeof(shu_data), (u8*)&shu_data, storage_get_part_id(STORAGE_PHYS_PART_USER));
		ret = blkdev_write(bootdev, part_dram_data_addr + ((unsigned long) (offsetof(DRAM_CALIBRATION_DATA_T, data[i]))), sizeof(shu_data), (u8*)&shu_data, storage_get_part_id(STORAGE_PHYS_PART_USER));
		dramc_crit("i = %d, offsetof(DRAM_CALIBRATION_DATA_T, data[i]) = %d\n", i, offsetof(DRAM_CALIBRATION_DATA_T, data[i]));
		if (ret != 0) {
			dramc_crit("[dramc] blkdev_write failed\n");
			SET_DRAM_STORAGE_API_ERR(ERR_BLKDEV_WRITE_FAIL, DRAM_STORAGE_API_WRITE);
			return -ERR_BLKDEV_WRITE_FAIL;
		}
	}

	return 0;
}

#else

DRAM_CALIBRATION_DATA_T dram_data; // using global variable to avoid stack overflow

static int read_offline_dram_mdl_data(DRAM_INFO_BY_MRR_T *DramInfo)
{
	return -1;
}
static int write_offline_dram_mdl_data(DRAM_INFO_BY_MRR_T *DramInfo)
{
	return -1;
}

int read_offline_dram_calibration_data(DRAM_DFS_SRAM_SHU_T shuffle, SAVE_TIME_FOR_CALIBRATION_T *offLine_SaveData)
{
	return 0;
}

int write_offline_dram_calibration_data(DRAM_DFS_SRAM_SHU_T shuffle, SAVE_TIME_FOR_CALIBRATION_T *offLine_SaveData)
{
	return 0;
}

int clean_dram_calibration_data(void)
{
	return;
}
#endif


void set_err_code_for_storage_api(void)
{
#ifdef LAST_DRAMC
	last_dramc_info_ptr->storage_api_err_flag = g_dram_storage_api_err_code;
	dsb();
#endif
}

#endif

#if defined(DRAM_SLT) && (!__ETT__)
#include <storage_api.h>
#include <emi.h>
int clean_slt_data(void)
{
	DRAM_SLT_DATA_T data;
	data.header.stage_status = -1;
	data.header.pl_version = PL_VERSION;
	return write_slt_data(&data);
}

int read_slt_data(DRAM_SLT_DATA_T *data)
{
	int i, ret;
	blkdev_t *bootdev = NULL;

	if (data == NULL) {
		dramc_crit("[dramc_slt] SLT data == NULL, skip\n");
	}

	bootdev = blkdev_get(CFG_BOOT_DEV);
	if (bootdev == NULL) {
		dramc_crit("[dramc_slt] can't find boot device(%d)\n", CFG_BOOT_DEV);
		return SLT_ERR_NO_DEV;
	}

	if (!part_dram_data_addr_slt) {
		return SLT_ERR_NO_ADDR;
	}

	ret = blkdev_read(bootdev, part_dram_data_addr_slt, sizeof(DRAM_SLT_DATA_T), (u8*)data, storage_get_part_id(STORAGE_PHYS_PART_USER));

	if (ret != 0) {
		return SLT_ERR_READ_FAIL;
	}

	if (data->header.pl_version != PL_VERSION) {
		dramc_crit("[dramc_slt] PL_VERSION mismatch\n");
		clean_slt_data();
		blkdev_read(bootdev, part_dram_data_addr_slt, sizeof(DRAM_SLT_DATA_T), (u8*)data, storage_get_part_id(STORAGE_PHYS_PART_USER));
	}

	return 0;
}

int write_slt_data(DRAM_SLT_DATA_T *data)
{
	int ret;
	blkdev_t *bootdev = NULL;

	if (data == NULL) {
		dramc_crit("[dramc_slt] data == NULL, skip\n");
		return SLT_ERR_NO_DATA;
	}

	bootdev = blkdev_get(CFG_BOOT_DEV);
	if (bootdev == NULL) {
		dramc_crit("[dramc_slt] can't find boot device(%d)\n", CFG_BOOT_DEV);
		return SLT_ERR_NO_DEV;
	}

	if (!part_dram_data_addr_slt) {
		return SLT_ERR_NO_ADDR;
	}

	data->header.pl_version = PL_VERSION;

	ret = blkdev_write(bootdev, part_dram_data_addr_slt, sizeof(DRAM_SLT_DATA_T), (u8*)data, storage_get_part_id(STORAGE_PHYS_PART_USER));
	if (ret != 0) {
		dramc_crit("[dramc_slt] blkdev_write failed\n");
		return SLT_ERR_WRITE_FAIL;
	}

	return 0;
}
#endif

#ifdef __DRAM_FLASH_TOOL_DA__
unsigned int get_mr8_by_mrr(U8 channel, U8 rank)
{
	DRAMC_CTX_T *p = psCurrDramCtx;
	unsigned int mr8_value;

	p->channel = channel;

	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SWCMD_CTRL0), rank, SWCMD_CTRL0_MRRRK);
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SWCMD_CTRL0), 8, SWCMD_CTRL0_MRSMA);
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SWCMD_EN), 1, SWCMD_EN_MRREN);
	while (u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMDRESP), SPCMDRESP_MRR_RESPONSE) ==0)
		mcDELAY_US(1);
	mr8_value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MRR_STATUS), MRR_STATUS_MRR_REG);
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SWCMD_EN), 0, SWCMD_EN_MRREN);
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SWCMD_CTRL0), 0, SWCMD_CTRL0_MRRRK);

	return (mr8_value & 0xff);
}
#endif

void vSetVddqForLP5(DRAMC_CTX_T *p, VDDQ_LEVEL level)
{
#if DRAMC_CTRL_BY_SOC
    unsigned int vddq = 0 ;

	 if (!is_lp4_family(p)) {
        #if __ETT__
	    hqa_set_lp5_vddq_voltage(p,level,&vddq);
        #else
        if (level == VDDQ_LOWFREQ) {
            #ifdef VOLTAGE_SEL
            vddq = vddq_voltage_select(VDDQ_LOWFREQ);
            #else
            vddq = SEL_PREFIX_VDDQ_LP5(LOWFREQ);
            #endif
        } else {
            #ifdef VOLTAGE_SEL
            vddq = vddq_voltage_select(VDDQ_HIGHFREQ);
            #else
            vddq = SEL_PREFIX_VDDQ_LP5(HIGHFREQ);
            #endif
        }
        #endif

        if (vddq)
            dramc_set_vddq_voltage(vddq);

        #ifndef DDR_INIT_TIME_PROFILING
        print("Vddq = %d\n", dramc_get_vddq_voltage());
        #endif
   }
#endif
}

void vSetVcoreByFreq(DRAMC_CTX_T *p)
{
#if DRAMC_CTRL_BY_SOC
	unsigned int vio18, vcore, vdd2, vddq, vmddr, vdd2H, vdd2L;

	vio18 = vcore = vdd2 = vddq = vmddr = vdd2H = vdd2L = 0;

#if __ETT__
	hqa_set_voltage_by_freq(p, &vio18, &vcore, &vdd2, &vddq, &vmddr, &vdd2H, &vdd2L, vddq); //set lp4x vddq,ignore lp5

#elif defined(VCORE_BIN)
	switch (vGet_Current_SRAMIdx(p)) {
	case SRAM_SHU0:  //6384 for lp5, 7500 for lp5x
	#ifdef VOLTAGE_SEL
		vcore = vcore_voltage_select(KSHU0);
		if (!vcore)
	#endif
	#if defined(HIGHEST_DRAM_DATARATE_DDR5500)
		if((seclib_get_devinfo_with_index(52) & 0xff) == 0)
		    vcore = (get_vcore_uv_table(0) + get_vcore_uv_table(1)) >> 1;
	        else
	#endif
		    vcore = get_vcore_uv_table(0);
		break;
	case SRAM_SHU1:   //6384
	#ifdef VOLTAGE_SEL
		vcore = vcore_voltage_select(KSHU1);
		if (!vcore)
	#endif
	#if defined(HIGHEST_DRAM_DATARATE_DDR5500)
		if((seclib_get_devinfo_with_index(52) & 0xff) == 0)
		    vcore = (get_vcore_uv_table(0) + get_vcore_uv_table(1)) >> 1;
	        else
	#endif
		    vcore = get_vcore_uv_table(0);
		break;
	case SRAM_SHU2:  //5500
	#ifdef VOLTAGE_SEL
		vcore = vcore_voltage_select(KSHU2);
		if (!vcore)
	#endif
		vcore = (get_vcore_uv_table(0) + get_vcore_uv_table(1)) >> 1;
		break;
	case SRAM_SHU3:  //4266
	#ifdef VOLTAGE_SEL
		vcore = vcore_voltage_select(KSHU3);
		if (!vcore)
	#endif
		vcore = (get_vcore_uv_table(0) + get_vcore_uv_table(1)) >> 1;
		break;
	case SRAM_SHU4:  //3094
	#ifdef VOLTAGE_SEL
		vcore = vcore_voltage_select(KSHU4);
		if (!vcore)
	#endif
		vcore = (get_vcore_uv_table(0) + get_vcore_uv_table(2)) >> 1;
		break;
	case SRAM_SHU5:  //2667
	#ifdef VOLTAGE_SEL
		vcore = vcore_voltage_select(KSHU5);
		if (!vcore)
	#endif
		vcore = (get_vcore_uv_table(0) + get_vcore_uv_table(3)) >> 1;
		break;
	case SRAM_SHU6:  //2133
	#ifdef VOLTAGE_SEL
		vcore = vcore_voltage_select(KSHU6);
		if (!vcore)
	#endif
		vcore = (get_vcore_uv_table(0) + get_vcore_uv_table(3)) >> 1;
		break;
	case SRAM_SHU7:  //1866
	#ifdef VOLTAGE_SEL
		vcore = vcore_voltage_select(KSHU7);
		if (!vcore)
	#endif
		vcore = (get_vcore_uv_table(0) + get_vcore_uv_table(4)) >> 1;
		break;
	case SRAM_SHU8:  //754
	#ifdef VOLTAGE_SEL
		vcore = vcore_voltage_select(KSHU8);
		if (!vcore)
	#endif
		vcore = (get_vcore_uv_table(0) + get_vcore_uv_table(4)) >> 1;
		break;
	}
#else
	int chipVer = mt_get_chip_sw_ver();
	switch (vGet_Current_SRAMIdx(p)) {
	case SRAM_SHU0: //6384 for lp5, 7500 for lp5x
	#ifdef VOLTAGE_SEL
		vcore = vcore_voltage_select(KSHU0);
	#else
		if (mt_get_dram_type() == TYPE_LPDDR5X)
			if(chipVer == CHIP_VER_E2 )
			  vcore = SEL_PREFIX_VCORE_E2(LP5X, KSHU0);	
			else	
			  vcore = SEL_PREFIX_VCORE(LP5X, KSHU0);
		else
			if(chipVer == CHIP_VER_E2 )
			  vcore = SEL_PREFIX_VCORE_E2(LP5, KSHU0);
			else
			  vcore = SEL_PREFIX_VCORE(LP5, KSHU0);
	#endif
		break;
	case SRAM_SHU1: // 6384
	#ifdef VOLTAGE_SEL
		vcore = vcore_voltage_select(KSHU1);
	#else
		if (mt_get_dram_type() == TYPE_LPDDR5X)
			if(chipVer == CHIP_VER_E2 )
			  vcore = SEL_PREFIX_VCORE_E2(LP5X, KSHU1);
			else
			  vcore = SEL_PREFIX_VCORE(LP5X, KSHU1);
		else
			if(chipVer == CHIP_VER_E2 )
			  vcore = SEL_PREFIX_VCORE_E2(LP5, KSHU1);
			else
			  vcore = SEL_PREFIX_VCORE(LP5, KSHU1);	
	#endif
		break;
	case SRAM_SHU2: // 5500
	#ifdef VOLTAGE_SEL
		vcore = vcore_voltage_select(KSHU2);
	#else
		if (mt_get_dram_type() == TYPE_LPDDR5X)
			if(chipVer == CHIP_VER_E2 )
			  vcore = SEL_PREFIX_VCORE_E2(LP5X, KSHU2);
			else
			  vcore = SEL_PREFIX_VCORE(LP5X, KSHU2);
		else
			if(chipVer == CHIP_VER_E2 )
			  vcore = SEL_PREFIX_VCORE_E2(LP5, KSHU2);
			else  
			  vcore = SEL_PREFIX_VCORE(LP5, KSHU2);
	#endif
		break;
	case SRAM_SHU3: //4266
	#ifdef VOLTAGE_SEL
		vcore = vcore_voltage_select(KSHU3);
	#else
		if (mt_get_dram_type() == TYPE_LPDDR5X)
			if(chipVer == CHIP_VER_E2 )
			  vcore = SEL_PREFIX_VCORE_E2(LP5X, KSHU3);
			else
			  vcore = SEL_PREFIX_VCORE(LP5X, KSHU3);
		else
			if(chipVer == CHIP_VER_E2 )
			  vcore = SEL_PREFIX_VCORE_E2(LP5, KSHU3);
			else
			  vcore = SEL_PREFIX_VCORE(LP5, KSHU3);
	#endif
		break;
	case SRAM_SHU4: //3094
	#ifdef VOLTAGE_SEL
		vcore = vcore_voltage_select(KSHU4);
	#else
		if (mt_get_dram_type() == TYPE_LPDDR5X)
			if(chipVer == CHIP_VER_E2 )
			  vcore = SEL_PREFIX_VCORE_E2(LP5X, KSHU4);
			else
			  vcore = SEL_PREFIX_VCORE(LP5X, KSHU4);
		else
			if(chipVer == CHIP_VER_E2 )
			  vcore = SEL_PREFIX_VCORE_E2(LP5, KSHU4);
			else
			  vcore = SEL_PREFIX_VCORE(LP5, KSHU4); 
	#endif
		break;
	case SRAM_SHU5: //2667
	#ifdef VOLTAGE_SEL
		vcore = vcore_voltage_select(KSHU5);
	#else
		if (mt_get_dram_type() == TYPE_LPDDR5X)
			if(chipVer == CHIP_VER_E2 )
			  vcore = SEL_PREFIX_VCORE_E2(LP5X, KSHU5);
			else
			  vcore = SEL_PREFIX_VCORE(LP5X, KSHU5);
		else
			if(chipVer == CHIP_VER_E2 )
			  vcore = SEL_PREFIX_VCORE_E2(LP5, KSHU5);
			else
			  vcore = SEL_PREFIX_VCORE(LP5, KSHU5);
	#endif
		break;
	case SRAM_SHU6: //2133
	#ifdef VOLTAGE_SEL
		vcore = vcore_voltage_select(KSHU6);
	#else
		if (mt_get_dram_type() == TYPE_LPDDR5X)
			if(chipVer == CHIP_VER_E2 )
			   vcore = SEL_PREFIX_VCORE_E2(LP5X, KSHU6);
			else
			   vcore = SEL_PREFIX_VCORE(LP5X, KSHU6);  
		else
			if(chipVer == CHIP_VER_E2 )
			   vcore = SEL_PREFIX_VCORE_E2(LP5, KSHU6);
			else	 
			   vcore = SEL_PREFIX_VCORE(LP5, KSHU6);
	#endif
		break;
	case SRAM_SHU7: //1866
	#ifdef VOLTAGE_SEL
		vcore = vcore_voltage_select(KSHU7);
	#else
		if (mt_get_dram_type() == TYPE_LPDDR5X)
			if(chipVer == CHIP_VER_E2 )
			  vcore = SEL_PREFIX_VCORE_E2(LP5X, KSHU7);
			else
			  vcore = SEL_PREFIX_VCORE(LP5X, KSHU7);
		else
			if(chipVer == CHIP_VER_E2 )
			  vcore = SEL_PREFIX_VCORE_E2(LP5, KSHU7);
			else
			  vcore = SEL_PREFIX_VCORE(LP5, KSHU7);
	#endif
		break;
	case SRAM_SHU8: //754
	#ifdef VOLTAGE_SEL
		vcore = vcore_voltage_select(KSHU8);
	#else
		if (mt_get_dram_type() == TYPE_LPDDR5X)
			if(chipVer == CHIP_VER_E2 )
			  vcore = SEL_PREFIX_VCORE_E2(LP5X, KSHU8);
			else
			  vcore = SEL_PREFIX_VCORE(LP5X, KSHU8);
		else
			if(chipVer == CHIP_VER_E2 )
			  vcore = SEL_PREFIX_VCORE_E2(LP5, KSHU8);
			else
			  vcore = SEL_PREFIX_VCORE(LP5, KSHU8);
	#endif
		break;
	default:
		return;
	}
#endif
	if (vcore)
		dramc_set_vcore_voltage(vcore);

#if !__ETT__
	if (is_second_pmic_pp_swap()) {
		switch (vGet_Current_SRAMIdx(p)) {
		case SRAM_SHU0:
#ifdef VOLTAGE_SEL
			vmddr = vmddr_voltage_select(KSHU0);
#else
			vmddr = SEL_PREFIX_DVS_VMDDR(KSHU0);
#endif
			break;
		case SRAM_SHU1:
#ifdef VOLTAGE_SEL
			vmddr = vmddr_voltage_select(KSHU1);
#else
			vmddr = SEL_PREFIX_DVS_VMDDR(KSHU1);
#endif
			break;
		case SRAM_SHU2:
#ifdef VOLTAGE_SEL
			vmddr = vmddr_voltage_select(KSHU2);
#else
			vmddr = SEL_PREFIX_DVS_VMDDR(KSHU2);
#endif
			break;
		case SRAM_SHU3:
#ifdef VOLTAGE_SEL
			vmddr = vmddr_voltage_select(KSHU3);
#else
			vmddr = SEL_PREFIX_DVS_VMDDR(KSHU3);
#endif
			break;
		case SRAM_SHU4:
#ifdef VOLTAGE_SEL
			vmddr = vmddr_voltage_select(KSHU4);
#else
			vmddr = SEL_PREFIX_DVS_VMDDR(KSHU4);
#endif
			break;
		case SRAM_SHU5:
#ifdef VOLTAGE_SEL
			vmddr = vmddr_voltage_select(KSHU5);
#else
			vmddr = SEL_PREFIX_DVS_VMDDR(KSHU5);
#endif
			break;
		case SRAM_SHU6:
#ifdef VOLTAGE_SEL
			vmddr = vmddr_voltage_select(KSHU6);
#else
			vmddr = SEL_PREFIX_DVS_VMDDR(KSHU6);
#endif
			break;
		case SRAM_SHU7:
#ifdef VOLTAGE_SEL
			vmddr = vmddr_voltage_select(KSHU7);
#else
			vmddr = SEL_PREFIX_DVS_VMDDR(KSHU7);
#endif
			break;
		case SRAM_SHU8:
#ifdef VOLTAGE_SEL
			vmddr = vmddr_voltage_select(KSHU8);
#else
			vmddr = SEL_PREFIX_DVS_VMDDR(KSHU8);
#endif
			break;
		default:
			break;
		}
	}
#endif

	if (vmddr)
		dramc_set_vmddr_voltage(vmddr);

#if defined(DRAM_HQA)
	if (vio18)
		dramc_set_vio18_voltage(vio18);

	if (vdd2)
		dramc_set_vdd2_voltage(vdd2);

	if (vddq){
		dramc_crit("dramc_set_vddq_voltage vddq = %d\n", vddq);
		dramc_set_vddq_voltage(vddq);
	}

	if (vdd2H)
		dramc_set_vdd2H_voltage(vdd2H);

	if (vdd2L)
		dramc_set_vdd2L_voltage(vdd2L);
#endif

#ifdef FOR_HQA_REPORT_USED
    switch (vGet_Current_SRAMIdx(p)) {
        case SRAM_SHU0: //6384 for lp5, 7500 for lp5x
        case SRAM_SHU1: //6384
		case SRAM_SHU2: //5500
		case SRAM_SHU3: //4266
		case SRAM_SHU4: //3094
		case SRAM_SHU5: //2667
		case SRAM_SHU6: //2133
		case SRAM_SHU7: //1866
		case SRAM_SHU8: //754
			gHQA_Test_Freq_Vcore_Level = 0; //only 1 freq
            break;
        default:
            print("[HQA] undefined shuffle level for Vcore (SHU%d)\r\n", vGet_Current_SRAMIdx(p));
#if __ETT__
            ASSERT(0);
#endif
            break;
    }
#endif

#ifndef DDR_INIT_TIME_PROFILING
	print("Read voltage for %d, %d\n", p->frequency, vGet_Current_SRAMIdx(p));
	print("Vio18 = %d\n", dramc_get_vio18_voltage());
	print("Vcore = %d\n", dramc_get_vcore_voltage());
	print("Vddq = %d\n", dramc_get_vddq_voltage());
	print("Vmddr = %d\n", dramc_get_vmddr_voltage());
	if (p->dram_type == TYPE_LPDDR4X) {
		print("Vdd2 = %d\n", dramc_get_vdd2_voltage());
	} else {
		print("Vdd2H = %d\n", dramc_get_vdd2H_voltage());
		print("Vdd2L = %d\n", dramc_get_vdd2L_voltage());
	}
#endif
#endif
}

U32 vGetVoltage(DRAMC_CTX_T *p, U32 get_voltage_type)
{
#if (defined(DRAM_HQA) || __ETT__) && DRAMC_CTRL_BY_SOC
    if (get_voltage_type==0)
        return dramc_get_vcore_voltage();

    if (get_voltage_type==1)
        return dramc_get_vdd2_voltage();

    if (get_voltage_type==2)
        return dramc_get_vddq_voltage();

    if (get_voltage_type==3)
           return dramc_get_vio18_voltage();

    if (get_voltage_type==4)
             return dramc_get_vmddr_voltage();

    if (get_voltage_type==5)
             return dramc_get_vdd2H_voltage();

    if (get_voltage_type==6)
             return dramc_get_vdd2L_voltage();

#endif

    return 0;
}

void DramcDpmSramEnable(DRAMC_CTX_T *p)
{
    /* DPM SRAM */
    vIO32WriteFldAlign(SPM_REG_DPM0_PWR_CON, 0, DPM0_PWR_CON_DPM0_SRAM_PDN);
    vIO32WriteFldAlign(SPM_REG_DPM0_PWR_CON, 1, DPM0_PWR_CON_DPM0_SRAM_SLEEP_B);
    mcDELAY_US(2);
    vIO32WriteFldAlign(SPM_REG_DPM0_PWR_CON, 1, DPM0_PWR_CON_DPM0_SRAM_ISOINT_B);
    mcDELAY_US(2);
    vIO32WriteFldAlign(SPM_REG_DPM0_PWR_CON, 0, DPM0_PWR_CON_DPM0_SRAM_CKISO);
    mcDELAY_US(1);

#if CHANNEL_NUM > 1
    vIO32WriteFldAlign(SPM_REG_DPM1_PWR_CON, 0, DPM1_PWR_CON_DPM1_SRAM_PDN);
    vIO32WriteFldAlign(SPM_REG_DPM1_PWR_CON, 1, DPM1_PWR_CON_DPM1_SRAM_SLEEP_B);
    mcDELAY_US(2);
    vIO32WriteFldAlign(SPM_REG_DPM1_PWR_CON, 1, DPM1_PWR_CON_DPM1_SRAM_ISOINT_B);
    mcDELAY_US(2);
    vIO32WriteFldAlign(SPM_REG_DPM1_PWR_CON, 0, DPM1_PWR_CON_DPM1_SRAM_CKISO);
    mcDELAY_US(1);

#if CHANNEL_NUM > 2
    vIO32WriteFldAlign(SPM_REG_DPM2_PWR_CON, 0, DPM2_PWR_CON_DPM2_SRAM_PDN);
    vIO32WriteFldAlign(SPM_REG_DPM2_PWR_CON, 1, DPM2_PWR_CON_DPM2_SRAM_SLEEP_B);
    mcDELAY_US(2);
    vIO32WriteFldAlign(SPM_REG_DPM2_PWR_CON, 1, DPM2_PWR_CON_DPM2_SRAM_ISOINT_B);
    mcDELAY_US(2);
    vIO32WriteFldAlign(SPM_REG_DPM2_PWR_CON, 0, DPM2_PWR_CON_DPM2_SRAM_CKISO);
    mcDELAY_US(1);

    vIO32WriteFldAlign(SPM_REG_DPM3_PWR_CON, 0, DPM3_PWR_CON_DPM3_SRAM_PDN);
    vIO32WriteFldAlign(SPM_REG_DPM3_PWR_CON, 1, DPM3_PWR_CON_DPM3_SRAM_SLEEP_B);
    mcDELAY_US(2);
    vIO32WriteFldAlign(SPM_REG_DPM3_PWR_CON, 1, DPM3_PWR_CON_DPM3_SRAM_ISOINT_B);
    mcDELAY_US(2);
    vIO32WriteFldAlign(SPM_REG_DPM3_PWR_CON, 0, DPM3_PWR_CON_DPM3_SRAM_CKISO);
    mcDELAY_US(1);
#endif
#endif

    /* SHU SRAM */
    vIO32WriteFldAlign(SPM_REG_DPYD0_PWR_CON, 0, DPYD0_PWR_CON_DPYD0_SRAM_PDN);
    vIO32WriteFldAlign(SPM_REG_DPYD0_PWR_CON, 1, DPYD0_PWR_CON_DPYD0_SRAM_SLEEP_B);
    mcDELAY_US(2);
    vIO32WriteFldAlign(SPM_REG_DPYD0_PWR_CON, 1, DPYD0_PWR_CON_DPYD0_SRAM_ISOINT_B);
    vIO32WriteFldAlign(SPM_REG_DPYD0_PWR_CON, 0, DPYD0_PWR_CON_DPYD0_SRAM_CKISO);

#if CHANNEL_NUM > 1
    vIO32WriteFldAlign(SPM_REG_DPYD1_PWR_CON, 0, DPYD1_PWR_CON_DPYD1_SRAM_PDN);
    vIO32WriteFldAlign(SPM_REG_DPYD1_PWR_CON, 1, DPYD1_PWR_CON_DPYD1_SRAM_SLEEP_B);
    mcDELAY_US(2);
    vIO32WriteFldAlign(SPM_REG_DPYD1_PWR_CON, 1, DPYD1_PWR_CON_DPYD1_SRAM_ISOINT_B);
    vIO32WriteFldAlign(SPM_REG_DPYD1_PWR_CON, 0, DPYD1_PWR_CON_DPYD1_SRAM_CKISO);

#if CHANNEL_NUM > 2
    vIO32WriteFldAlign(SPM_REG_DPYD2_PWR_CON, 0, DPYD2_PWR_CON_DPYD2_SRAM_PDN);
    vIO32WriteFldAlign(SPM_REG_DPYD2_PWR_CON, 1, DPYD2_PWR_CON_DPYD2_SRAM_SLEEP_B);
    mcDELAY_US(2);
    vIO32WriteFldAlign(SPM_REG_DPYD2_PWR_CON, 1, DPYD2_PWR_CON_DPYD2_SRAM_ISOINT_B);
    vIO32WriteFldAlign(SPM_REG_DPYD2_PWR_CON, 0, DPYD2_PWR_CON_DPYD2_SRAM_CKISO);

    vIO32WriteFldAlign(SPM_REG_DPYD3_PWR_CON, 0, DPYD3_PWR_CON_DPYD3_SRAM_PDN);
    vIO32WriteFldAlign(SPM_REG_DPYD3_PWR_CON, 1, DPYD3_PWR_CON_DPYD3_SRAM_SLEEP_B);
    mcDELAY_US(2);
    vIO32WriteFldAlign(SPM_REG_DPYD3_PWR_CON, 1, DPYD3_PWR_CON_DPYD3_SRAM_ISOINT_B);
    vIO32WriteFldAlign(SPM_REG_DPYD3_PWR_CON, 0, DPYD3_PWR_CON_DPYD3_SRAM_CKISO);
#endif
#endif
}
#if __ETT__
int dramc_top_get_vmddr_voltage(int shuf)
{
	int vmddr = 0;
    if (is_second_pmic_pp_swap()) {
		switch (shuf) {
			case SRAM_SHU0:
#ifdef VOLTAGE_SEL
				vmddr = vmddr_voltage_select(KSHU0);
#else
				vmddr = SEL_PREFIX_DVS_VMDDR(KSHU0);
#endif
				break;
			case SRAM_SHU1:
#ifdef VOLTAGE_SEL
				vmddr = vmddr_voltage_select(KSHU1);
#else
				vmddr = SEL_PREFIX_DVS_VMDDR(KSHU1);
#endif
				break;
			case SRAM_SHU2:
#ifdef VOLTAGE_SEL
				vmddr = vmddr_voltage_select(KSHU2);
#else
				vmddr = SEL_PREFIX_DVS_VMDDR(KSHU2);
#endif
				break;
			case SRAM_SHU3:
#ifdef VOLTAGE_SEL
				vmddr = vmddr_voltage_select(KSHU3);
#else
				vmddr = SEL_PREFIX_DVS_VMDDR(KSHU3);
#endif
				break;
			case SRAM_SHU4:
#ifdef VOLTAGE_SEL
				vmddr = vmddr_voltage_select(KSHU4);
#else
				vmddr = SEL_PREFIX_DVS_VMDDR(KSHU4);
#endif
				break;
			case SRAM_SHU5:
#ifdef VOLTAGE_SEL
				vmddr = vmddr_voltage_select(KSHU5);
#else
				vmddr = SEL_PREFIX_DVS_VMDDR(KSHU5);
#endif
				break;
			case SRAM_SHU6:
#ifdef VOLTAGE_SEL
				vmddr = vmddr_voltage_select(KSHU6);
#else
				vmddr = SEL_PREFIX_DVS_VMDDR(KSHU6);
#endif
				break;
			case SRAM_SHU7:
#ifdef VOLTAGE_SEL
				vmddr = vmddr_voltage_select(KSHU7);
#else
				vmddr = SEL_PREFIX_DVS_VMDDR(KSHU7);
#endif
				break;
			case SRAM_SHU8:
#ifdef VOLTAGE_SEL
				vmddr = vmddr_voltage_select(KSHU8);
#else
				vmddr = SEL_PREFIX_DVS_VMDDR(KSHU8);
#endif
				break;
			default:
				break;
			}
		} else {
			switch (hqa_vmddr_class)
			{
				case 1:
					vmddr = _SEL_VMDDR(HV);
					break;
				 case 2:
					vmddr = _SEL_VMDDR(NV);
					break;
				case 3:
					vmddr = _SEL_VMDDR(LV);
					break;
			}
		}
	return vmddr;
}

int vmddr_voltage_runtime_select(DRAM_KSHU kshu)
{
	int vmddr = 0;

	if (hqa_vmddr_class == LEVEL_LV) {
		switch (kshu) {
		case KSHU0:
			vmddr = HQA_VMDDR_LV_RT_KSHU0;
			break;
		case KSHU1:
			vmddr = HQA_VMDDR_LV_RT_KSHU1;
			break;
		case KSHU2:
			vmddr = HQA_VMDDR_LV_RT_KSHU2;
			break;
		case KSHU3:
			vmddr = HQA_VMDDR_LV_RT_KSHU3;
			break;
		case KSHU4:
			vmddr = HQA_VMDDR_LV_RT_KSHU4;
			break;
		case KSHU5:
			vmddr = HQA_VMDDR_LV_RT_KSHU5;
			break;
		case KSHU6:
			vmddr = HQA_VMDDR_LV_RT_KSHU6;
			break;
		case KSHU7:
			vmddr = HQA_VMDDR_LV_RT_KSHU7;
			break;
		case KSHU8:
			vmddr = HQA_VMDDR_LV_RT_KSHU8;
			break;
		default:
			break;
		}
	} else if (hqa_vmddr_class == LEVEL_HV) {
		switch (kshu) {
		case KSHU0:
			vmddr = HQA_VMDDR_HV_RT_KSHU0;
			break;
		case KSHU1:
			vmddr = HQA_VMDDR_HV_RT_KSHU1;
			break;
		case KSHU2:
			vmddr = HQA_VMDDR_HV_RT_KSHU2;
			break;
		case KSHU3:
			vmddr = HQA_VMDDR_HV_RT_KSHU3;
			break;
		case KSHU4:
			vmddr = HQA_VMDDR_HV_RT_KSHU4;
			break;
		case KSHU5:
			vmddr = HQA_VMDDR_HV_RT_KSHU5;
			break;
		case KSHU6:
			vmddr = HQA_VMDDR_HV_RT_KSHU6;
			break;
		case KSHU7:
			vmddr = HQA_VMDDR_HV_RT_KSHU7;
			break;
		case KSHU8:
			vmddr = HQA_VMDDR_HV_RT_KSHU8;
			break;
		default:
			break;
		}
	} else {
		switch (kshu) {
		case KSHU0:
			vmddr = HQA_VMDDR_NV_RT_KSHU0;
			break;
		case KSHU1:
			vmddr = HQA_VMDDR_NV_RT_KSHU1;
			break;
		case KSHU2:
			vmddr = HQA_VMDDR_NV_RT_KSHU2;
			break;
		case KSHU3:
			vmddr = HQA_VMDDR_NV_RT_KSHU3;
			break;
		case KSHU4:
			vmddr = HQA_VMDDR_NV_RT_KSHU4;
			break;
		case KSHU5:
			vmddr = HQA_VMDDR_NV_RT_KSHU5;
			break;
		case KSHU6:
			vmddr = HQA_VMDDR_NV_RT_KSHU6;
			break;
		case KSHU7:
			vmddr = HQA_VMDDR_NV_RT_KSHU7;
			break;
		case KSHU8:
			vmddr = HQA_VMDDR_NV_RT_KSHU8;
			break;
		default:
			break;
		}
	}

	return vmddr;
}

int dramc_top_get_rt_vmddr_voltage(int shuf)
{

	int vmddr = 0;
    if (is_second_pmic_pp_swap()) {
		vmddr = vmddr_voltage_runtime_select(shuf);
	} else {
		switch (hqa_vmddr_class)
		{
			case 1:
				vmddr = _SEL_VMDDR(HV);
				break;
			 case 2:
				vmddr = _SEL_VMDDR(NV);
				break;
			case 3:
				vmddr = _SEL_VMDDR(LV);
				break;
		}
	}
	return vmddr;
}
#else
int vmddr_voltage_runtime_select(DRAM_KSHU kshu)
{
	int vmddr = 0;
#ifdef LVCORE_LVDRAM
		switch (kshu) {
		case KSHU0:
			vmddr = HQA_VMDDR_LV_RT_KSHU0;
			break;
		case KSHU1:
			vmddr = HQA_VMDDR_LV_RT_KSHU1;
			break;
		case KSHU2:
			vmddr = HQA_VMDDR_LV_RT_KSHU2;
			break;
		case KSHU3:
			vmddr = HQA_VMDDR_LV_RT_KSHU3;
			break;
		case KSHU4:
			vmddr = HQA_VMDDR_LV_RT_KSHU4;
			break;
		case KSHU5:
			vmddr = HQA_VMDDR_LV_RT_KSHU5;
			break;
		case KSHU6:
			vmddr = HQA_VMDDR_LV_RT_KSHU6;
			break;
		case KSHU7:
			vmddr = HQA_VMDDR_LV_RT_KSHU7;
			break;
		case KSHU8:
			vmddr = HQA_VMDDR_LV_RT_KSHU8;
			break;
		default:
			break;
		}
#elif defined(HVCORE_HVDRAM) 
		switch (kshu) {
		case KSHU0:
			vmddr = HQA_VMDDR_HV_RT_KSHU0;
			break;
		case KSHU1:
			vmddr = HQA_VMDDR_HV_RT_KSHU1;
			break;
		case KSHU2:
			vmddr = HQA_VMDDR_HV_RT_KSHU2;
			break;
		case KSHU3:
			vmddr = HQA_VMDDR_HV_RT_KSHU3;
			break;
		case KSHU4:
			vmddr = HQA_VMDDR_HV_RT_KSHU4;
			break;
		case KSHU5:
			vmddr = HQA_VMDDR_HV_RT_KSHU5;
			break;
		case KSHU6:
			vmddr = HQA_VMDDR_HV_RT_KSHU6;
			break;
		case KSHU7:
			vmddr = HQA_VMDDR_HV_RT_KSHU7;
			break;
		case KSHU8:
			vmddr = HQA_VMDDR_HV_RT_KSHU8;
			break;
		default:
			break;
		}
#else
		switch (kshu) {
		case KSHU0:
			vmddr = HQA_VMDDR_NV_RT_KSHU0;
			break;
		case KSHU1:
			vmddr = HQA_VMDDR_NV_RT_KSHU1;
			break;
		case KSHU2:
			vmddr = HQA_VMDDR_NV_RT_KSHU2;
			break;
		case KSHU3:
			vmddr = HQA_VMDDR_NV_RT_KSHU3;
			break;
		case KSHU4:
			vmddr = HQA_VMDDR_NV_RT_KSHU4;
			break;
		case KSHU5:
			vmddr = HQA_VMDDR_NV_RT_KSHU5;
			break;
		case KSHU6:
			vmddr = HQA_VMDDR_NV_RT_KSHU6;
			break;
		case KSHU7:
			vmddr = HQA_VMDDR_NV_RT_KSHU7;
			break;
		case KSHU8:
			vmddr = HQA_VMDDR_NV_RT_KSHU8;
			break;
		default:
			break;
		}
#endif

	return vmddr;
}
int dramc_top_get_rt_vmddr_voltage(int shuf)
{

	int vmddr = 0;
    if (is_second_pmic_pp_swap()) {
		vmddr = vmddr_voltage_runtime_select(shuf);
	} else {
#ifdef LVCORE_LVDRAM
		vmddr = _SEL_VMDDR(HV);
#elif defined(HVCORE_HVDRAM) 
		vmddr = _SEL_VMDDR(NV);
#else
		vmddr = _SEL_VMDDR(LV);
#endif		
	}
	return vmddr;
}
#endif
