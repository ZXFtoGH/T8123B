#include "drm_define.h"
#if (FOR_DV_SIMULATION_USED == 0)
#include "dramc_top.h"
#endif
#include "dramc_common.h"
#include "dramc_int_global.h"
#include "x_hal_io.h"
#define IPMV21_DRM_WRKAROUND_STORE__DSM_EN__SHU_LEVEL__SHU_LEVEL_SRAM

#define EXIT_DRM_WITH_SAME_FREQ 1 //1:DRM out with the same freq
#ifdef DDR_RESERVE_MODE
#define mcSHOW_DRM_MSG mcSHOW_DBG_MSG
#define CHAN_MODE 3 //1_chan: 1 2_chan: 3 //NEED_DE_NOTICE

#if ((__ETT__==0) && (FOR_DV_SIMULATION_USED==0))
#define SPM_BASE_ADDRESS 0x10006000

#define SPM_POWER_ON_VAL0 (SPM_BASE_ADDRESS + 0x004)
    #define SPM_POWER_ON_VAL0_SC_PHYPLL_MODE_SW          Fld(1,28)
#endif
#define DRM_DUMP_RG 0

extern DRAMC_CTX_T DramCtx_LPDDR4;
static U32 u4DramType = 0;
static U32 u4ReserveRegBackupAddress[] =
{
    (DRAMC_REG_HMR4),//MR4
    (DRAMC_REG_HMR4 + SHIFT_TO_CHB_ADDR),//0x0220
#if (CHANNEL_NUM > 2)
    (DRAMC_REG_HMR4 + SHIFT_TO_CHC_ADDR),//0x0220
    (DRAMC_REG_HMR4 + SHIFT_TO_CHD_ADDR),//0x0220
#endif

    (DRAMC_REG_DRAMC_PD_CTRL),//PHY clk
    (DRAMC_REG_DRAMC_PD_CTRL + SHIFT_TO_CHB_ADDR),//0x01FC
#if (CHANNEL_NUM > 2)
    (DRAMC_REG_DRAMC_PD_CTRL + SHIFT_TO_CHC_ADDR),//0x01FC
    (DRAMC_REG_DRAMC_PD_CTRL + SHIFT_TO_CHD_ADDR),//0x01FC
#endif

    (DDRPHY_REG_B0_DQ5),//Verf en
    (DDRPHY_REG_B0_DQ5 + SHIFT_TO_CHB_ADDR),//0x0188
#if (CHANNEL_NUM > 2)
    (DDRPHY_REG_B0_DQ5 + SHIFT_TO_CHC_ADDR),//0x0188
    (DDRPHY_REG_B0_DQ5 + SHIFT_TO_CHD_ADDR),//0x0188
#endif

    (DDRPHY_REG_B1_DQ5),
    (DDRPHY_REG_B1_DQ5 + SHIFT_TO_CHB_ADDR),//0x0308
#if (CHANNEL_NUM > 2)
    (DDRPHY_REG_B1_DQ5 + SHIFT_TO_CHC_ADDR),//0x0308
    (DDRPHY_REG_B1_DQ5 + SHIFT_TO_CHD_ADDR),//0x0308
#endif

    (DDRPHY_REG_B0_RXDVS0),//RX tracking
    (DDRPHY_REG_B0_RXDVS0 + SHIFT_TO_CHB_ADDR),//0x0164
#if (CHANNEL_NUM > 2)
    (DDRPHY_REG_B0_RXDVS0 + SHIFT_TO_CHC_ADDR),//0x0164
    (DDRPHY_REG_B0_RXDVS0 + SHIFT_TO_CHD_ADDR),//0x0164
#endif

    (DDRPHY_REG_B1_RXDVS0),
    (DDRPHY_REG_B1_RXDVS0 + SHIFT_TO_CHB_ADDR),//0x02E4
#if (CHANNEL_NUM > 2)
    (DDRPHY_REG_B1_RXDVS0 + SHIFT_TO_CHC_ADDR),//0x02E4
    (DDRPHY_REG_B1_RXDVS0 + SHIFT_TO_CHD_ADDR),//0x02E4
#endif

    (DDRPHY_REG_RK_B0_RXDVS2),
    (DDRPHY_REG_RK_B0_RXDVS2 + SHIFT_TO_CHB_ADDR),//0x0068
#if (CHANNEL_NUM > 2)
    (DDRPHY_REG_RK_B0_RXDVS2 + SHIFT_TO_CHC_ADDR),//0x0068
    (DDRPHY_REG_RK_B0_RXDVS2 + SHIFT_TO_CHD_ADDR),//0x0068
#endif

    (DDRPHY_REG_RK_B1_RXDVS2),
    (DDRPHY_REG_RK_B1_RXDVS2 + SHIFT_TO_CHB_ADDR),//0x01E8
#if (CHANNEL_NUM > 2)
    (DDRPHY_REG_RK_B1_RXDVS2 + SHIFT_TO_CHC_ADDR),//0x01E8
    (DDRPHY_REG_RK_B1_RXDVS2 + SHIFT_TO_CHD_ADDR),//0x01E8
#endif

    (DDRPHY_AO_RANK_OFFSET + DDRPHY_REG_RK_B0_RXDVS2),
    (DDRPHY_AO_RANK_OFFSET + DDRPHY_REG_RK_B0_RXDVS2 + SHIFT_TO_CHB_ADDR),  //0x00E8
#if (CHANNEL_NUM > 2)
    (DDRPHY_AO_RANK_OFFSET + DDRPHY_REG_RK_B0_RXDVS2 + SHIFT_TO_CHC_ADDR),  //0x00E8
    (DDRPHY_AO_RANK_OFFSET + DDRPHY_REG_RK_B0_RXDVS2 + SHIFT_TO_CHD_ADDR),  //0x00E8
#endif

    (DDRPHY_AO_RANK_OFFSET + DDRPHY_REG_RK_B1_RXDVS2),
    (DDRPHY_AO_RANK_OFFSET + DDRPHY_REG_RK_B1_RXDVS2 + SHIFT_TO_CHB_ADDR),  //0x0268
#if (CHANNEL_NUM > 2)
    (DDRPHY_AO_RANK_OFFSET + DDRPHY_REG_RK_B1_RXDVS2 + SHIFT_TO_CHC_ADDR),  //0x0268
    (DDRPHY_AO_RANK_OFFSET + DDRPHY_REG_RK_B1_RXDVS2 + SHIFT_TO_CHD_ADDR),  //0x0268
#endif

    (DRAMC_REG_DUMMY_RD),//Dummy read
    (DRAMC_REG_DUMMY_RD + SHIFT_TO_CHB_ADDR),//0x0110
#if (CHANNEL_NUM > 2)
    (DRAMC_REG_DUMMY_RD + SHIFT_TO_CHC_ADDR),//0x0110
    (DRAMC_REG_DUMMY_RD + SHIFT_TO_CHD_ADDR),//0x0110
#endif

    (DDRPHY_REG_MISC_CTRL1),//TX tracking reload from AO
    (DDRPHY_REG_MISC_CTRL1 + SHIFT_TO_CHB_ADDR),//TX tracking reload from AO
#if (CHANNEL_NUM > 2)
    (DDRPHY_REG_MISC_CTRL1 + SHIFT_TO_CHC_ADDR),//TX tracking reload from AO
    (DDRPHY_REG_MISC_CTRL1 + SHIFT_TO_CHD_ADDR),//TX tracking reload from AO
#endif

    (DDRPHY_MD32_REG_LPIF_FSM_CFG_1),
#if (CHANNEL_NUM > 2)
    (DDRPHY_MD32_REG_LPIF_FSM_CFG_1 + SHIFT_TO_CHB_ADDR),
#endif
};

void MD32_init(void)
{
    DRAMC_CTX_T *p;
    p = &DramCtx_LPDDR4;

    mcSHOW_DRM_MSG(("[mem.c]CFC setting for MD32 NAO start\n"));
////  *((UINT32P)(MD32DR_CH0_BASE + 0x47034)) |= ( 0x1);//legacy
//  *((UINT32P)(MD32DR_CH0_BASE + 0x47020)) |= ( 0x10000000);
//  *((UINT32P)(MD32DR_CH0_BASE + 0x47004)) &= (~0xF003);
//  *((UINT32P)(MD32DR_CH0_BASE + 0x47008)) &= (~0xC0000);
//  *((UINT32P)(MD32DR_CH0_BASE + 0x4700C)) &= (~0x70);
//  *((UINT32P)(MD32DR_CH0_BASE + 0x4703C)) &= (~0xC03000);
    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0, 0x0,LPIF_LOW_POWER_CFG_0_MEM_CK_OFF);          //0x47004[15:14]
    mcDELAY_XUS(1);

    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_FSM_CFG_1, 0, LPIF_FSM_CFG_1_LPIF_LEGACY_CONTROL);              //0x47034[0], EED_DE_NOTICE
    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_FSM_CTRL_0, 0x1,LPIF_FSM_CTRL_0_LP_INTF_OUT_ENABLE);            //0x47020[28]
    mcSHOW_DRM_MSG(("[mem.c]CFC setting for MD32 NAO end\n"));
}

int MD32_init2(void)
{
    DRAMC_CTX_T *p;
    p = &DramCtx_LPDDR4;
    U32     shu_lev,     shu_lev_sram,     DSM;
    mcSHOW_DRM_MSG(("[mem.c]MD32_init2 start\n"));
#ifdef IPMV21_DRM_WRKAROUND_STORE__DSM_EN__SHU_LEVEL__SHU_LEVEL_SRAM
    shu_lev        = u4IO32ReadFldAlign(DRAMC_REG_SA_RESERVE, SA_RESERVE_DRM_SHU_LEVEL);
    shu_lev_sram   = u4IO32ReadFldAlign(DRAMC_REG_SA_RESERVE, SA_RESERVE_DRM_SHU_SRAM_LEVEL);
    DSM            = u4IO32ReadFldAlign(DRAMC_REG_SA_RESERVE, SA_RESERVE_DRM_DSM);
#else
    if((u4IO32Read4B(DDRPHY_MD32_REG_LPIF_DBG_LATCH7) | u4IO32Read4B(DDRPHY_MD32_REG_LPIF_DBG_LATCH8))== 0)
    {
        vIO32WriteFldAlign_All(DDRPHY_MD32_REG_SSPM_CFGREG_SW_RSTN, 1, SSPM_CFGREG_SW_RSTN_SW_RSTN); //0x40000[0]
        mcDELAY_XUS(30);
        shu_lev          = u4IO32ReadFldAlign(DDRPHY_MD32_REG_LPIF_STATUS_10,  LPIF_STATUS_10_DRAMC_DR_SHU_LEVEL);       //0x470C8
        shu_lev_sram     = u4IO32ReadFldAlign(DDRPHY_MD32_REG_LPIF_STATUS_10,  LPIF_STATUS_10_DRAMC_DR_SHU_SRAM_LEVEL);  //0x470C8
        DSM              = u4IO32ReadFldAlign(DDRPHY_MD32_REG_LPIF_STATUS_14,  LPIF_STATUS_14_DRAMC_DPY_DSM_EN);         //0x470D8
        mcSHOW_DRM_MSG(("[mem.c]MD32_init2 status:           shu_lev=0x%x, shu_lev_sram=0x%x, DSM=0x%x\n",     shu_lev,     shu_lev_sram,     DSM));
        vIO32WriteFldAlign_All(DDRPHY_MD32_REG_SSPM_CFGREG_GPR7, DSM, SSPM_CFGREG_GPR7_GPR7);        //0x40114[0]
        vIO32WriteFldAlign_All(DDRPHY_MD32_REG_SSPM_CFGREG_SW_RSTN, 0, SSPM_CFGREG_SW_RSTN_SW_RSTN); //0x40000[0]
        vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0, 3, LPIF_LOW_POWER_CFG_0_DDRPHY_FB_CK_EN);      //0x47004[17:16], if release DDRPHY_MD32_REG_SSPM_CFGREG_SW_RSTN
        vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_3, 0, LPIF_LOW_POWER_CFG_3_DPY_DSM_EN);           //0x47038[13:12], if release DDRPHY_MD32_REG_SSPM_CFGREG_SW_RSTN
        vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_FSM_CFG_1, 1, LPIF_FSM_CFG_1_LPIF_OUTPUT_PATH_FROM_SW);         //0x47034[4]    , if release DDRPHY_MD32_REG_SSPM_CFGREG_SW_RSTN
    }
    else
    {
        shu_lev      = u4IO32ReadFldAlign(DDRPHY_MD32_REG_LPIF_DBG_LATCH8, LPIF_DBG_LATCH8_DRAMC_DR_SHU_LEVEL);      //0x473A0
        shu_lev_sram = u4IO32ReadFldAlign(DDRPHY_MD32_REG_LPIF_DBG_LATCH8, LPIF_DBG_LATCH8_DRAMC_DR_SHU_SRAM_LEVEL); //0x473A0
        DSM          = u4IO32ReadFldAlign(DDRPHY_MD32_REG_LPIF_DBG_LATCH9, LPIF_DBG_LATCH9_DRAMC_DPY_DSM_EN);        //0x470A4
        mcSHOW_DRM_MSG(("[mem.c]MD32_init2 DBG latch status: shu_lev=0x%x, shu_lev_sram=0x%x, DSM=0x%x\n",      shu_lev,     shu_lev_sram,     DSM));
        vIO32WriteFldAlign_All(DDRPHY_MD32_REG_SSPM_CFGREG_GPR7, DSM, SSPM_CFGREG_GPR7_GPR7);        //0x40114[0]
    }
#endif
    mcSHOW_DRM_MSG(("[mem.c]MD32_init2 end\n"));
    return (shu_lev | (shu_lev_sram <<4));
}

void CS_toggle()
{
    DRAMC_CTX_T *p;
    p = &DramCtx_LPDDR4;
    mcSHOW_DRM_MSG(("[mem.c]CS toggle start\n"));
    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0, 3, LPIF_LOW_POWER_CFG_0_DDRPHY_FB_CK_EN);      //0x47004[17:16]
    mcDELAY_XUS(4);        //DELAY_US(4);

    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_3, 3, LPIF_LOW_POWER_CFG_3_DPY_CA_PULL_DN_EN);    //0x47038[23:22]
    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_3, 3, LPIF_LOW_POWER_CFG_3_DPY_CS_PULL_DN_EN);    //0x47038[19:18]

    vIO32WriteFldAlign_All(DDRPHY_REG_CA_CMD2, 0x0, CA_CMD2_RG_TX_ARCS_OE_TIE_SEL_CA);//OE DIS, DDRPHY_AO+0x47C[14]
    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_3, 3, LPIF_LOW_POWER_CFG_3_DPY_CS_PULL_UP_EN);    //0x47038[17:16]
    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_3, 0, LPIF_LOW_POWER_CFG_3_DPY_CS_PULL_UP_EN);    //0x47038[17:16]
    vIO32WriteFldAlign_All(DDRPHY_REG_CA_CMD2, 0x1, CA_CMD2_RG_TX_ARCS_OE_TIE_SEL_CA);//OE EN, DDRPHY_AO+0x47C[14]

    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_3, 0, LPIF_LOW_POWER_CFG_3_DPY_CS_PULL_DN_EN);    //0x47038[19:18]
    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_3, 0, LPIF_LOW_POWER_CFG_3_DPY_CA_PULL_DN_EN);    //0x47038[23:22]

    mcSHOW_DRM_MSG(("[mem.c]CS toggle end\n"));
}

void DRM_exit_DSM()
{
    DRAMC_CTX_T *p;
    p = &DramCtx_LPDDR4;
    mcSHOW_DRM_MSG(("[mem.c]DRM_exit_DSM start\n"));

    while ( u4IO32ReadFldAlign(DDRPHY_REG_MISC_DDR_RESERVE_STATE ,                    MISC_DDR_RESERVE_STATE_WDT_SM) !=9);//0x1520[3:0]
#if (CHANNEL_NUM > 1)
    while ( u4IO32ReadFldAlign(DDRPHY_REG_MISC_DDR_RESERVE_STATE + SHIFT_TO_CHB_ADDR, MISC_DDR_RESERVE_STATE_WDT_SM) !=9);
#endif
#if (CHANNEL_NUM > 2)
    while ( u4IO32ReadFldAlign(DDRPHY_REG_MISC_DDR_RESERVE_STATE + SHIFT_TO_CHC_ADDR, MISC_DDR_RESERVE_STATE_WDT_SM) !=9);
    while ( u4IO32ReadFldAlign(DDRPHY_REG_MISC_DDR_RESERVE_STATE + SHIFT_TO_CHD_ADDR, MISC_DDR_RESERVE_STATE_WDT_SM) !=9);
#endif
//    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_3, 0x3, LPIF_LOW_POWER_CFG_3_DPY_DSM_EN);//0x47038[13:12]
//    mcDELAY_XUS(1);

    CS_toggle();
    mcSHOW_DRM_MSG(("[mem.c]wait 190us\n"));
    mcDELAY_XUS(190);    //DELAY_US(190);

    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_DDR_RESERVE2, 0, MISC_DDR_RESERVE2_DRM_SM_HOLD_CTRL);//0x6f4[31:16]
//    mcDELAY_XUS(1);
//    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_3, 0x0, LPIF_LOW_POWER_CFG_3_DPY_DSM_EN);//0x47038[13:12]

    mcSHOW_DRM_MSG(("[mem.c]DRM_exit_DSM end\n"));
}

void Check_MD32_LP5_DSM() //Insert after proj C release wdt_dramc_sref
{
    DRAMC_CTX_T *p;
    p = &DramCtx_LPDDR4;
    U32 DSM = 0;
    mcSHOW_DRM_MSG(("[mem.c]Check_MD32_LP5_DSM start\n"));

#ifdef IPMV21_DRM_WRKAROUND_STORE__DSM_EN__SHU_LEVEL__SHU_LEVEL_SRAM
    DSM = u4IO32ReadFldAlign(DRAMC_REG_SA_RESERVE, SA_RESERVE_DRM_DSM);
#else
    DSM = u4IO32ReadFldAlign(DDRPHY_MD32_REG_SSPM_CFGREG_GPR7, SSPM_CFGREG_GPR7_GPR7) & 0x1; //MD32DR_CH0_BASE + 0x40114
#endif

    mcSHOW_DRM_MSG(("DSM=0x%X\n", DSM));

    if(DSM) DRM_exit_DSM() ;
    else    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_DDR_RESERVE2, 0x0, MISC_DDR_RESERVE2_DRM_SM_HOLD_CTRL); //DDRPHY_AO_BASE_ADDRESS + 0x6f4[31:16] = 0x200
    mcDELAY_XUS(1);    //DELAY_US(190);

    mcSHOW_DRM_MSG(("[mem.c]Check_MD32_LP5_DSM end\n"));
}

void LP_Mode_DRM_Setting(void)
{
    DRAMC_CTX_T * p;
    U32 temp;
    p = &DramCtx_LPDDR4;
    U32 backup_broadcast = GetDramcBroadcast();
//    U32 fgOpenLoop = (p->pDFSTable->shuffleIdx == SRAM_SHU7?1:0); // u4IO32ReadFldAlign(DDRPHY_REG_SHU_MISC_CLK_CTRL0, SHU_MISC_CLK_CTRL0_M_CK_OPENLOOP_MODE_SEL);//DDRPHY_AO_BASE_ADDRESS + 0x0D40[4]
//    if(fgOpenLoop == 0x1)
//    {
//        vIO32WriteFldAlign_All(DDRPHY_REG_SHU_MISC_CLK_CTRL0, 1, SHU_MISC_CLK_CTRL0_M_CK_OPENLOOP_MODE_SEL);
//        mcDELAY_XUS(1);
//        vIO32WriteFldAlign_All(DDRPHY_REG_MISC_CG_CTRL0, 1, MISC_CG_CTRL0_RESERVED_MISC_CG_CTRL0_BIT3_1);//DDRPHY_AO_BASE_ADDRESS + 0x04EC[3:1]
//        vIO32WriteFldAlign_All(DDRPHY_REG_MISC_CG_CTRL0, 0, MISC_CG_CTRL0_RESERVED_MISC_CG_CTRL0_BIT3_1);
//        vIO32WriteFldAlign_All(DDRPHY_REG_MISC_CG_CTRL0, 1, MISC_CG_CTRL0_RESERVED_MISC_CG_CTRL0_BIT7);//DDRPHY_AO_BASE_ADDRESS + 0x04EC[7]
//    }

    temp = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_MISC_CLK_CTRL0), SHU_MISC_CLK_CTRL0_M_CK_OPENLOOP_MODE_SEL);

    if (temp) {
        mcSHOW_DRM_MSG(("DRAMC in open loop mode(26M to MCK)\n"));
        vIO32WriteFldAlign_All(DDRPHY_REG_MISC_CG_CTRL0, 0x0, MISC_CG_CTRL0_RESERVED_MISC_CG_CTRL0_BIT7);
        vIO32WriteFldAlign_All(DDRPHY_REG_MISC_CG_CTRL0, 0x1, MISC_CG_CTRL0_RESERVED_MISC_CG_CTRL0_BIT3_1);
        vIO32WriteFldAlign_All(DDRPHY_REG_MISC_CG_CTRL0, 0x0, MISC_CG_CTRL0_RESERVED_MISC_CG_CTRL0_BIT3_1);
//        vIO32WriteFldAlign_All(DDRPHY_REG_SHU_MISC_CLK_CTRL0, 0x0, SHU_MISC_CLK_CTRL0_M_CK_OPENLOOP_MODE_SEL);
//!        vIO32WriteFldAlign_All(DDRPHY_REG_MISC_DVFSCTL2, 0x1, MISC_DVFSCTL2_RG_DVFS_MEM_CK_OP_SEL_SOURCE);
    }

    mcSHOW_DRM_MSG(("LP_Mode_DRM_Setting(26M to MCK)\n"));

    vIO32WriteFldMulti_All(DDRPHY_REG_MISC_CLK_CTRL,
                     P_Fld(0x1, MISC_CLK_CTRL_DVFS_CLK_MEM_SEL) | // by "sel_source"
                     P_Fld(0x0, MISC_CLK_CTRL_DVFS_MEM_CK_MUX_SEL_MODE) | // HW mode by shuffle
                     P_Fld(0x1, MISC_CLK_CTRL_DVFS_MEM_CK_MUX_SEL)); // 4-to-1 mux from PLLCK if DVFS_MEM_CK_MUX_SEL_MODE = 2

#if (FOR_DV_SIMULATION_USED == 1)
    ANA_top_config_T *a_cfg = A_T;
    DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
    vSetPHY2ChannelMapping(p, CHANNEL_A);
    if(a_cfg->ASYNC_MODE == 0)//8139, Panther
    {
	vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CKMUX_SEL  )  , P_Fld(   1             , MISC_CKMUX_SEL_FMEM_CK_MUX ));
	vSetPHY2ChannelMapping(p, CHANNEL_B);
	vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CKMUX_SEL  )  , P_Fld(   3             , MISC_CKMUX_SEL_FMEM_CK_MUX ));

        //only set at dsim, TBA can't & need not.
        //#define INFRACFG_AO_MEM_BASE (0x10002000)
        //#define TOP_CK_ANCHOR_CFG	((UINT32P)(INFRACFG_AO_MEM_BASE+0x038))
        //Please confirm addr
        //*TOP_CK_ANCHOR_CFG   |= ((0x1 << 28) | (0x1 << 30));
    }
    else if(a_cfg->ASYNC_MODE == 2)
    {
	vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CKMUX_SEL  )  , P_Fld(   0             , MISC_CKMUX_SEL_FMEM_CK_MUX ));
	vSetPHY2ChannelMapping(p, CHANNEL_B);
	vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CKMUX_SEL  )  , P_Fld(   2             , MISC_CKMUX_SEL_FMEM_CK_MUX ));
    }
    else//Le_Pin, Fi_geac, Mon_trose
    {
	vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CKMUX_SEL  )  , P_Fld(   0             , MISC_CKMUX_SEL_FMEM_CK_MUX ));
	vSetPHY2ChannelMapping(p, CHANNEL_B);
	vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CKMUX_SEL  )  , P_Fld(   0             , MISC_CKMUX_SEL_FMEM_CK_MUX ));
    }
    vSetPHY2ChannelMapping(p, CHANNEL_A);
    DramcBroadcastOnOff(backup_broadcast);
#else//TBD: need SA change
	vIO32WriteFldAlign_All(DDRPHY_REG_MISC_CKMUX_SEL, 0 , MISC_CKMUX_SEL_FMEM_CK_MUX);
#endif
    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_CG_CTRL0, 1, MISC_CG_CTRL0_CLK_MEM_SEL);//Just sync MP setting, If DVFS_CLK_MEM_SEL = 0;DDRPHY_AO_BASE_ADDRESS + 0x04EC[5:4]
    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_CG_CTRL0, 1, MISC_CG_CTRL0_W_CHG_MEM);//DDRPHY_AO_BASE_ADDRESS + 0x04EC[0]

    mcSHOW_DRM_MSG(("DELAY 1 us\n"));
    mcDELAY_XUS(1);

    mcSHOW_DRM_MSG(("disable memory clock change\n"));
    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_CG_CTRL0, 0, MISC_CG_CTRL0_W_CHG_MEM);

    if (temp) {
        vIO32WriteFldAlign_All(DDRPHY_REG_MISC_CG_CTRL0,  1, MISC_CG_CTRL0_RG_FREERUN_MCK_CG);
        mcSHOW_DRM_MSG(("DRAMC in open loop mode 8X(26M to MCK)\n"));
        vIO32WriteFldAlign_All(DDRPHY_REG_MISC_CG_CTRL10, 1, MISC_CG_CTRL10_CLK_MEM_SEL_MCK8X);//DDRPHY_AO_BASE_ADDRESS + 0x06FC[0]
        vIO32WriteFldAlign_All(DDRPHY_REG_MISC_CG_CTRL10, 1, MISC_CG_CTRL10_W_CHG_MEM_MCK8X);//DDRPHY_AO_BASE_ADDRESS + 0x06FC[0]

        mcSHOW_DRM_MSG(("DELAY 1 us\n"));
        mcDELAY_XUS(1);

        mcSHOW_DRM_MSG(("disable memory clock change\n"));
        vIO32WriteFldAlign_All(DDRPHY_REG_MISC_CG_CTRL10, 0, MISC_CG_CTRL10_W_CHG_MEM_MCK8X);
        vIO32WriteFldAlign_All(DDRPHY_REG_MISC_CG_CTRL0, 0x1, MISC_CG_CTRL0_RESERVED_MISC_CG_CTRL0_BIT7);
    }
}

void ASVA5_8_New_Mode_1(void)
{
    U32 u4WbrBackup = 0;
    U32 u4value = 0;
    DRAMC_CTX_T * p;
    p = &DramCtx_LPDDR4;
    BOOL bSupport2Rank = (u4IO32ReadFldAlign(DRAMC_REG_SA_RESERVE, SA_RESERVE_SINGLE_RANK) == 0) ? TRUE : FALSE;

    Init_Dram_Ctx_By_Type(p,mt_get_dram_type());
    vSetChannelNumber(p);
    vSetRankNumber(p);

    MD32_init();

    mcSHOW_DRM_MSG(("ASVA5_8_New_Mode_1\n"));

#ifdef IMPEDANCE_TRACKING_ENABLE
    mcSHOW_DRM_MSG(("Disable IMP tracking\n"));
    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_IMPCAL, 0, MISC_IMPCAL_IMPCAL_HW);
#endif

#if ENABLE_TX_TRACKING
    mcSHOW_DRM_MSG(("Disable DQSOSC\n"));
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_RK_DQSOSC), 0, RK_DQSOSC_DQSOSCR_RK0EN);
    if ((bSupport2Rank == TRUE))
    {
        vSetRank(p, RANK_1);
        vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_RK_DQSOSC), 0, RK_DQSOSC_DQSOSCR_RK0EN);
    }
    vSetRank(p, RANK_0);
#endif
    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_LP_CTRL,1,MISC_LP_CTRL_RG_ARDMSUS_10);
    vIO32WriteFldAlign_All(DDRPHY_REG_B0_LP_CTRL0 ,1,  B0_LP_CTRL0_RG_ARDMSUS_10_B0);
    vIO32WriteFldAlign_All(DDRPHY_REG_B1_LP_CTRL0 ,1,  B1_LP_CTRL0_RG_ARDMSUS_10_B1);
    vIO32WriteFldAlign_All(DDRPHY_REG_CA_LP_CTRL0 ,1,  CA_LP_CTRL0_RG_ARDMSUS_10_CA);

    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_LP_CTRL,0,MISC_LP_CTRL_RG_ARDMSUS_10_LP_SEL);
    vIO32WriteFldAlign_All(DDRPHY_REG_B0_LP_CTRL0 ,0,  B0_LP_CTRL0_RG_ARDMSUS_10_B0_LP_SEL);
    vIO32WriteFldAlign_All(DDRPHY_REG_B1_LP_CTRL0 ,0,  B1_LP_CTRL0_RG_ARDMSUS_10_B1_LP_SEL);
    vIO32WriteFldAlign_All(DDRPHY_REG_CA_LP_CTRL0 ,0,  CA_LP_CTRL0_RG_ARDMSUS_10_CA_LP_SEL);

    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_DDR_RESERVE,0,MISC_DDR_RESERVE_WDT_LITE_EN);
    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_DDR_RESERVE,1,MISC_DDR_RESERVE_WDT_SM_CLR);
}

void ASVA5_8_CSCA_Pull_Down_EN(void);
void ASVA5_8_CSCA_Pull_Down_DIS(void);
void ASVA5_8_New_Mode_2(void)
{
    U32 u4WbrBackup = 0;
    U32 u4value = 0;
    DRAMC_CTX_T * p;
    p = &DramCtx_LPDDR4;

    mcSHOW_DRM_MSG(("ASVA5_8_New_Mode_2\n"));

    ASVA5_8_CSCA_Pull_Down_EN();

    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_LP_CTRL,1,MISC_LP_CTRL_RG_ARDMSUS_10_LP_SEL);
    vIO32WriteFldAlign_All(DDRPHY_REG_B0_LP_CTRL0 ,1,  B0_LP_CTRL0_RG_ARDMSUS_10_B0_LP_SEL);
    vIO32WriteFldAlign_All(DDRPHY_REG_B1_LP_CTRL0 ,1,  B1_LP_CTRL0_RG_ARDMSUS_10_B1_LP_SEL);
    vIO32WriteFldAlign_All(DDRPHY_REG_CA_LP_CTRL0 ,1,  CA_LP_CTRL0_RG_ARDMSUS_10_CA_LP_SEL);

    ASVA5_8_CSCA_Pull_Down_DIS();
}

void ASVA5_8_New_Mode_3(void)
{
    U32 u4WbrBackup = 0;
    U32 u4value = 0;
    DRAMC_CTX_T * p;
    p = &DramCtx_LPDDR4;

    mcSHOW_DRM_MSG(("ASVA5_8_New_Mode_3\n"));

    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_DDR_RESERVE,0,MISC_DDR_RESERVE_WDT_SM_CLR);
    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_DDR_RESERVE,1,MISC_DDR_RESERVE_WDT_LITE_EN);
}

void ASVA5_8_CSCA_Pull_Down_EN(void)
{
    U32 u4WbrBackup = 0;
    U32 u4value = 0;
    DRAMC_CTX_T * p;
    p = &DramCtx_LPDDR4;

    mcSHOW_DRM_MSG(("ASVA5_8_CSCA_Pull_Down_EN\n"));
    vIO32WriteFldAlign_All(DDRPHY_REG_CA_CMD7, 1, CA_CMD7_RG_TX_ARCS0_PULL_DN); //DDRPHY_AO_BASE_ADDRESS + 0x0490[4]
    vIO32WriteFldAlign_All(DDRPHY_REG_CA_CMD7, 1, CA_CMD7_RG_TX_ARCS1_PULL_DN); //DDRPHY_AO_BASE_ADDRESS + 0x0490[8]
    vIO32WriteFldAlign_All(DDRPHY_REG_CA_CMD7, 1, CA_CMD7_RG_TX_ARCMD_PULL_DN); //DDRPHY_AO_BASE_ADDRESS + 0x0490[6]
    vIO32WriteFldAlign_All(DDRPHY_REG_CA_LP_CTRL0, 0, CA_LP_CTRL0_RG_TX_ARCS_PULL_DN_LP_SEL);//DDRPHY_AO_BASE_ADDRESS + 0x0460[3]
    vIO32WriteFldAlign_All(DDRPHY_REG_CA_LP_CTRL0, 0, CA_LP_CTRL0_RG_TX_ARCA_PULL_DN_LP_SEL);//DDRPHY_AO_BASE_ADDRESS + 0x0460[1]
}

void ASVA5_8_CSCA_Pull_Down_DIS(void)
{
    U32 u4WbrBackup = 0;
    U32 u4value = 0;
    DRAMC_CTX_T * p;
    p = &DramCtx_LPDDR4;

    mcSHOW_DRM_MSG(("ASVA5_8_CSCA_Pull_Down_DIS\n"));
//    vIO32WriteFldAlign_All(DDRPHY_REG_CA_CMD7, 1, CA_CMD7_RG_TX_ARCS0_PULL_DN);
//    vIO32WriteFldAlign_All(DDRPHY_REG_CA_CMD7, 1, CA_CMD7_RG_TX_ARCS1_PULL_DN);
//    vIO32WriteFldAlign_All(DDRPHY_REG_CA_CMD7, 1, CA_CMD7_RG_TX_ARCMD_PULL_DN);
    vIO32WriteFldAlign_All(DDRPHY_REG_CA_LP_CTRL0, 1, CA_LP_CTRL0_RG_TX_ARCS_PULL_DN_LP_SEL);//DDRPHY_AO_BASE_ADDRESS + 0x0460[3]
    vIO32WriteFldAlign_All(DDRPHY_REG_CA_LP_CTRL0, 1, CA_LP_CTRL0_RG_TX_ARCA_PULL_DN_LP_SEL);//DDRPHY_AO_BASE_ADDRESS + 0x0460[1]
}

void ASVA5_8_IPMV2(void)
{
    DRAMC_CTX_T * p;
    U32 temp;
    p = &DramCtx_LPDDR4;

    if(u4IO32ReadFldAlign(DDRPHY_REG_MISC_DDR_RESERVE, MISC_DDR_RESERVE_WDT_LITE_EN)==0)
    {
        vIO32WriteFldAlign_All(DDRPHY_REG_MISC_DDR_RESERVE , 0,     MISC_DDR_RESERVE_WDT_SM_CLR);
        vIO32WriteFldAlign_All(DDRPHY_REG_MISC_DDR_RESERVE3, 0xFF, MISC_DDR_RESERVE3_DRM_SM_PASS_CTRL);
        mcDELAY_XUS(1);
        vIO32WriteFldAlign_All(DDRPHY_REG_MISC_DDR_RESERVE , 1,     MISC_DDR_RESERVE_WDT_LITE_EN);
    }
        vIO32WriteFldAlign_All(DDRPHY_REG_MISC_DDR_RESERVE3, 0xFF, MISC_DDR_RESERVE3_DRM_SM_PASS_CTRL);

    mcSHOW_DRM_MSG(("[mem.c]DRM_ASVA5_8_IPMV2 start\n"));

    while ( u4IO32ReadFldAlign(DDRPHY_REG_MISC_DDR_RESERVE_STATE ,                    MISC_DDR_RESERVE_STATE_WDT_SM) !=8);//0x1520[3:0]
#if (CHANNEL_NUM > 1)
    while ( u4IO32ReadFldAlign(DDRPHY_REG_MISC_DDR_RESERVE_STATE + SHIFT_TO_CHB_ADDR, MISC_DDR_RESERVE_STATE_WDT_SM) !=8);
#endif
#if (CHANNEL_NUM > 2)
    while ( u4IO32ReadFldAlign(DDRPHY_REG_MISC_DDR_RESERVE_STATE + SHIFT_TO_CHC_ADDR, MISC_DDR_RESERVE_STATE_WDT_SM) !=8);
    while ( u4IO32ReadFldAlign(DDRPHY_REG_MISC_DDR_RESERVE_STATE + SHIFT_TO_CHD_ADDR, MISC_DDR_RESERVE_STATE_WDT_SM) !=8);
#endif
    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_DDR_RESERVE3, 0x0, MISC_DDR_RESERVE3_DRM_SM_PASS_CTRL);

    ASVA5_8_CSCA_Pull_Down_EN();

    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_DDR_RESERVE2, 0x200, MISC_DDR_RESERVE2_DRM_SM_HOLD_CTRL);//0x6f4[31:16]

    ASVA5_8_CSCA_Pull_Down_DIS();
    mcSHOW_DRM_MSG(("[mem.c]DRM_ASVA5_8_IPMV2 end\n"));

}

void wait_SREFx(void)
{
    DRAMC_CTX_T * p;
    U32 temp;
    p = &DramCtx_LPDDR4;
    mcSHOW_DRM_MSG(("[mem.c]wait_SREFx start\n"));

    while ( u4IO32ReadFldAlign(DRAMC_REG_MISC_STATUSA ,                    MISC_STATUSA_SREF_STATE) !=0);//DRAMC_NAO_BASE_ADDRESS + 0x0080[16]
#if (CHANNEL_NUM > 1)
    while ( u4IO32ReadFldAlign(DRAMC_REG_MISC_STATUSA + SHIFT_TO_CHB_ADDR, MISC_STATUSA_SREF_STATE) !=0);
#endif
#if (CHANNEL_NUM > 2)
    while ( u4IO32ReadFldAlign(DRAMC_REG_MISC_STATUSA + SHIFT_TO_CHC_ADDR, MISC_STATUSA_SREF_STATE) !=0);
    while ( u4IO32ReadFldAlign(DRAMC_REG_MISC_STATUSA + SHIFT_TO_CHD_ADDR, MISC_STATUSA_SREF_STATE) !=0);
#endif

    mcSHOW_DRM_MSG(("[mem.c]wait_SREFx end\n"));
}

void Set_Spm_Poweron_Config_En(DRAMC_CTX_T * p)//NEED_DE_NOTICE, for CTP load
{
#if 0 //If use CTP load(legacy mode low power flow)
    vIO32WriteFldMulti(SPM_POWERON_CONFIG_EN, P_Fld(0x1, POWERON_CONFIG_EN_BCLK_CG_EN)//SPM power on
                | P_Fld(0xB16, POWERON_CONFIG_EN_PROJECT_CODE));
    vIO32WriteFldAlign(SPM_PCM_PWR_IO_EN, 0, PCM_PWR_IO_EN_FULL);
    vIO32WriteFldAlign(SPM_DRAMC_DPY_CLK_SW_CON_SEL, 0xffffffff, DRAMC_DPY_CLK_SW_CON_SEL_FULL);
    vIO32WriteFldAlign(SPM_DRAMC_DPY_CLK_SW_CON_SEL2, 0xffffffff, DRAMC_DPY_CLK_SW_CON_SEL2_FULL);
    vIO32WriteFldMulti(SPM_SPM_POWER_ON_VAL0, P_Fld(0x1, SPM_POWER_ON_VAL0_SC_DPY_2ND_DLL_EN)
                | P_Fld(0x1, SPM_POWER_ON_VAL0_SC_DPY_DLL_EN)
                | P_Fld(0x1, SPM_POWER_ON_VAL0_SC_DPY_DLL_CK_EN)
                | P_Fld(0x1, SPM_POWER_ON_VAL0_SC_DPY_VREF_EN)
                | P_Fld(0x1, SPM_POWER_ON_VAL0_SC_PHYPLL_EN));
    vIO32WriteFldAlign(SPM_SPM_S1_MODE_CH, 0x3, SPM_S1_MODE_CH_SPM_S1_MODE_CH);//SR request by side band
#endif
    return;
}

void dump_SR(DRAMC_CTX_T * p, U32 u4DramType)
{
    DRAM_CHANNEL_T bkChannel = vGetPHY2ChannelMapping(p);
    U8 u1channel = CHANNEL_A;
    U32 u4value = 0;

    for(u1channel = CHANNEL_A; u1channel < p->support_channel_num; u1channel++)
    {
        vSetPHY2ChannelMapping(p, u1channel);
        u4value = u4IO32ReadFldAlign(DRAMC_REG_MISC_STATUSA, MISC_STATUSA_SREF_STATE);
        mcSHOW_ERR_MSG(("###CH%d SREF_STATE [0x%X]\n", u1channel, u4value));
    }
    vSetPHY2ChannelMapping(p, bkChannel);
    return;
}

#if DRM_DUMP_RG
void dump_drm_rg(DRAMC_CTX_T * p, u32 u4ConfShuLevel)
{
    U32 u4RegIdx = 0;
    U32 u4RegValue = 0;
    U32 u4RankIdx = 0;

    U32 u4DRMRgAddrCommon[] =
    {
        DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0,
        DDRPHY_MD32_REG_LPIF_FSM_CFG_1,
        DDRPHY_MD32_REG_LPIF_FSM_CTRL_0,
        DDRPHY_MD32_REG_SSPM_CFGREG_SW_RSTN,
        DDRPHY_MD32_REG_SSPM_CFGREG_GPR7,
        DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_3,
        DDRPHY_REG_CA_CMD2,
        DDRPHY_REG_MISC_DDR_RESERVE2,
        DDRPHY_REG_MISC_CG_CTRL0,
        DDRPHY_REG_MISC_CG_CTRL10,
        DDRPHY_REG_MISC_IMPCAL,
        DDRPHY_REG_MISC_LP_CTRL,
        DDRPHY_REG_B0_LP_CTRL0,
        DDRPHY_REG_B1_LP_CTRL0,
        DDRPHY_REG_CA_LP_CTRL0,
        DDRPHY_REG_MISC_DDR_RESERVE,
        DDRPHY_REG_CA_CMD7,
        DRAMC_REG_SWCMD_EN,
        DRAMC_REG_DRAMCTRL,
        DRAMC_REG_SHU_DCM_CTRL0,
        DRAMC_REG_ZQ_SET1,
        DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_1,
        DDRPHY_REG_MISC_CG_CTRL5,
        DDRPHY_REG_B0_RXDVS0,
        DDRPHY_REG_B1_RXDVS0,
        DRAMC_REG_HMR4,
        DRAMC_REG_DRAMC_PD_CTRL,
        DDRPHY_REG_B0_DQ5,
        DDRPHY_REG_B1_DQ5,
        DDRPHY_REG_CA_CMD5,
        DDRPHY_REG_B0_DQ6,
        DDRPHY_REG_B1_DQ6,
        DDRPHY_REG_CA_CMD6,
        DDRPHY_REG_B0_DQ3,
        DDRPHY_REG_B0_DLL_ARPI0,
        DDRPHY_REG_B1_DQ3,
        DDRPHY_REG_B1_DLL_ARPI0,
        DDRPHY_REG_CA_CMD3,
        DDRPHY_REG_CA_DLL_ARPI0,
        DDRPHY_REG_PHYPLL2,
        DDRPHY_REG_MISC_DVFSCTL2,
        DDRPHY_REG_PHYPLL0,
        DDRPHY_REG_CLRPLL0,
        DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_2,
        DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_4,
        DDRPHY_MD32_REG_LPIF_MISC_CTL_1,
        DDRPHY_REG_MISC_RG_DFS_CTRL,
        DRAMC_REG_RX_SET0,
        DDRPHY_MD32_REG_SSPM_CFGREG_DBG_LATCH_SW,
        SPM_POWER_ON_VAL0,
        DDRPHY_REG_MISC_CLK_CTRL,
        DDRPHY_REG_MISC_CG_CTRL0,
        DRAMC_REG_DUMMY_RD,
        DDRPHY_REG_MISC_CTRL3,
        DRAMC_REG_DQSOSCR,
        DDRPHY_REG_MISC_CKMUX_SEL,
    };


    U32 u4DRMRgAddrShu[] =
    {
        DDRPHY_REG_SHU_B0_DQ11,
        DDRPHY_REG_SHU_B1_DQ11,
        DDRPHY_REG_MISC_SHU_CTRL0,
        DDRPHY_REG_MISC_SHU_CG_CTRL0,
        DDRPHY_REG_SHU_B0_DLL1,
        DDRPHY_REG_SHU_B1_DLL1,
        DDRPHY_REG_SHU_CA_DLL1,
        DDRPHY_REG_SHU_B0_DQ1,
        DDRPHY_REG_SHU_B1_DQ1,
        DDRPHY_REG_SHU_CA_CMD1,
        DDRPHY_REG_B0_SHU_MIDPI_CTRL,
        DDRPHY_REG_B1_SHU_MIDPI_CTRL,
        DDRPHY_REG_SHU_B0_DLL_ARPI2,
        DDRPHY_REG_SHU_B1_DLL_ARPI2,
        DDRPHY_REG_SHU_CA_DLL_ARPI2,
        DDRPHY_REG_MISC_SHU_STBCAL,
    };


    U32 u4DRMRgAddrRk[] =
    {
        DRAMC_REG_RK_DQSOSC,
        DDRPHY_REG_RK_B0_RXDVS2,
        DDRPHY_REG_RK_B1_RXDVS2,
    };

    mcSHOW_DBG_MSG(("----Dump DRM RG ----\n"));

    //Common
    for (u4RegIdx = 0; u4RegIdx < ARRAY_SIZE(u4DRMRgAddrCommon); u4RegIdx++)
    {
        u4RegValue = u4IO32Read4B(u4DRMRgAddrCommon[u4RegIdx]);
        mcSHOW_DBG_MSG(("Reg(0x%X) = 0x%X\n", u4DRMRgAddrCommon[u4RegIdx], u4RegValue));
    }
    //SHU RG
    for (u4RegIdx = 0; u4RegIdx < ARRAY_SIZE(u4DRMRgAddrShu); u4RegIdx++)
    {
        u4RegValue = u4IO32Read4B(u4DRMRgAddrShu[u4RegIdx] + u4ConfShuLevel * 0x700);
        mcSHOW_DBG_MSG(("SHU Reg(0x%X) = 0x%X\n", u4DRMRgAddrShu[u4RegIdx] + u4ConfShuLevel * 0x700, u4RegValue));
    }
    //Rank RG
    for (u4RankIdx = 0; u4RankIdx < p->support_rank_num; u4RankIdx++)
    {
        u4RegValue = u4IO32Read4B(u4DRMRgAddrRk[u4RegIdx] + u4RankIdx * DDRPHY_AO_RANK_OFFSET);
        mcSHOW_DBG_MSG(("RK Reg(0x%X) = 0x%X\n", u4DRMRgAddrRk[u4RegIdx] + u4RankIdx * DDRPHY_AO_RANK_OFFSET, u4RegValue));
    }
}
#endif

void dump_last_dramc(DRAMC_CTX_T * p)
{
    DRAM_CHANNEL_T bkChannel = vGetPHY2ChannelMapping(p);
    U8 u1channel = CHANNEL_A;
    U32 u4value = 0;

    mcSHOW_DRM_MSG(("@@@ p->support_channel_num=%d \n",p->support_channel_num));
    for(u1channel = CHANNEL_A; u1channel < p->support_channel_num; u1channel++)
    {
        vSetPHY2ChannelMapping(p, u1channel);
        u4value = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_WDT_DBG_SIGNAL));
        mcSHOW_ERR_MSG(("@ DDR reserve CH%d last_dramc[0x%X]\n", u1channel, u4value));

        u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_WDT_DBG_SIGNAL), WDT_DBG_SIGNAL_LATCH_DRAMC_GATING_ERROR_FROM_AO);
        mcSHOW_ERR_MSG(("@ GATING_ERROR[%d]\n", u4value));

        u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_WDT_DBG_SIGNAL), WDT_DBG_SIGNAL_LATCH_RD_TEST_DM_CMP_CPT2_RK0_FROM_AO);
        mcSHOW_ERR_MSG(("@ RD_TEST_DM_CMP_CPT2_RK0[%d]\n", u4value));

        u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_WDT_DBG_SIGNAL), WDT_DBG_SIGNAL_LATCH_RD_TEST_DM_CMP_CPT2_RK1_FROM_AO);
        mcSHOW_ERR_MSG(("@ RD_TEST_DM_CMP_CPT2_RK1[%d]\n", u4value));

        u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_WDT_DBG_SIGNAL), WDT_DBG_SIGNAL_LATCH_RD_TEST_DM_CMP_ERR2_RK0_FROM_AO);
        mcSHOW_ERR_MSG(("@ RD_TEST_DM_CMP_ERR2_RK0[%d] <-Ignore (w/o test)\n", u4value));

        u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_WDT_DBG_SIGNAL), WDT_DBG_SIGNAL_LATCH_RD_TEST_DM_CMP_ERR2_RK1_FROM_AO);
        mcSHOW_ERR_MSG(("@ RD_TEST_DM_CMP_ERR2_RK1[%d] <-Ignore (w/o test)\n", u4value));

        u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_WDT_DBG_SIGNAL), WDT_DBG_SIGNAL_LATCH_RD_TEST_DLE_CNT_OK2_RK0_FROM_AO);
        mcSHOW_ERR_MSG(("@ RD_TEST_DLE_CNT_OK2_RK0[%d]\n", u4value));

        u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_WDT_DBG_SIGNAL), WDT_DBG_SIGNAL_LATCH_RD_TEST_DLE_CNT_OK2_RK1_FROM_AO);
        mcSHOW_ERR_MSG(("@ RD_TEST_DLE_CNT_OK2_RK1[%d]\n", u4value));

        u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_WDT_DBG_SIGNAL), WDT_DBG_SIGNAL_LATCH_RDWR_TEST_DM_CMP_CPT2_RK0_FROM_AO);
        mcSHOW_ERR_MSG(("@ RDWR_TEST_DM_CMP_CPT2_RK0[%d]\n", u4value));

        u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_WDT_DBG_SIGNAL), WDT_DBG_SIGNAL_LATCH_RDWR_TEST_DM_CMP_CPT2_RK1_FROM_AO);
        mcSHOW_ERR_MSG(("@ RDWR_TEST_DM_CMP_CPT2_RK1[%d]\n", u4value));

        u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_WDT_DBG_SIGNAL), WDT_DBG_SIGNAL_LATCH_RDWR_TEST_DM_CMP_ERR2_RK0_FROM_AO);
        mcSHOW_ERR_MSG(("@ RDWR_TEST_DM_CMP_ERR2_RK0[%d]\n", u4value));

        u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_WDT_DBG_SIGNAL), WDT_DBG_SIGNAL_LATCH_RDWR_TEST_DM_CMP_ERR2_RK1_FROM_AO);
        mcSHOW_ERR_MSG(("@ RDWR_TEST_DM_CMP_ERR2_RK1[%d]\n", u4value));

        u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_WDT_DBG_SIGNAL), WDT_DBG_SIGNAL_LATCH_RDWR_TEST_DLE_CNT_OK2_RK0_FROM_AO);
        mcSHOW_ERR_MSG(("@ RDWR_TEST_DLE_CNT_OK2_RK0[%d]\n", u4value));

        u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_WDT_DBG_SIGNAL), WDT_DBG_SIGNAL_LATCH_RDWR_TEST_DLE_CNT_OK2_RK1_FROM_AO);
        mcSHOW_ERR_MSG(("@ RDWR_TEST_DLE_CNT_OK2_RK1[%d]\n", u4value));

        u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MISC_STATUSA),MISC_STATUSA_SREF_STATE);
        mcSHOW_ERR_MSG(("@ Check DRAMC enter Self-Refresh-> MISC_STATUSA_SREF_STATE=[0x%x]\n", u4value));
    }
    vSetPHY2ChannelMapping(p, bkChannel);
    return;
}

void dump_gating_error_rg(DRAMC_CTX_T * p)
{
    DRAM_CHANNEL_T bkChannel = vGetPHY2ChannelMapping(p);
    U8 u1channel = CHANNEL_A;
    U32 u4value = 0;

    for(u1channel = CHANNEL_A; u1channel < p->support_channel_num; u1channel++)
    {
        vSetPHY2ChannelMapping(p, u1channel);
        u4value = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_REG_MISC_STBERR_ALL));
        mcSHOW_ERR_MSG(("###DDR reserve CH%d DDRPHY_REG_MISC_STBERR_ALL [0x%X]\n", u1channel, u4value));
        u4value = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_REG_MISC_RD_DET_ERR_FLAG_B0));
        mcSHOW_ERR_MSG(("###DDR reserve CH%d DDRPHY_REG_MISC_RD_DET_ERR_FLAG_B0 [0x%X]\n", u1channel, u4value));
        u4value = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_REG_MISC_RD_DET_ERR_FLAG_B1));
        mcSHOW_ERR_MSG(("###DDR reserve CH%d DDRPHY_REG_MISC_RD_DET_ERR_FLAG_B1 [0x%X]\n", u1channel, u4value));

        u4value = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_REG_SHU_RK_B0_DQSIEN_DLY));
        mcSHOW_ERR_MSG(("###DDR reserve CH%d DDRPHY_REG_SHU_RK_B0_DQSIEN_DLY RK0[0x%X]\n", u1channel, u4value));
        u4value = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_REG_SHU_RK_B0_DQSIEN_DLY + DDRPHY_AO_RANK_OFFSET));
        mcSHOW_ERR_MSG(("###DDR reserve CH%d DDRPHY_REG_SHU_RK_B0_DQSIEN_DLY RK1[0x%X]\n", u1channel, u4value));
        u4value = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_REG_SHU_RK_B1_DQSIEN_DLY));
        mcSHOW_ERR_MSG(("###DDR reserve CH%d DDRPHY_REG_SHU_RK_B1_DQSIEN_DLY RK0[0x%X]\n", u1channel, u4value));
        u4value = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_REG_SHU_RK_B1_DQSIEN_DLY + DDRPHY_AO_RANK_OFFSET));
        mcSHOW_ERR_MSG(("###DDR reserve CH%d DDRPHY_REG_SHU_RK_B1_DQSIEN_DLY RK1[0x%X]\n", u1channel, u4value));
    }
    vSetPHY2ChannelMapping(p, bkChannel);
    return;
}

void dump_gating_error_rg_log(DRAMC_CTX_T * p, U8 u1Cnt)
{
	mcSHOW_ERR_MSG(("###DDR reserve dump_gating_error_rg_log[0x%X]###\n",u1Cnt));
	dump_gating_error_rg(p);
    return;
}

void Dramc_DDR_Reserved_Mode_setting(void)
{
    U32 u4WbrBackup = 0;
    U32 u4value = 0;
    U32 backup_broadcast = GetDramcBroadcast();
    DRAMC_CTX_T * p;
    p = &DramCtx_LPDDR4;
    BOOL bSupport2Rank;
    U32 u4SramShuLevel = 0;
    U32 u4ConfShuLevel = 0;
    U32 MIDPI_CA = 0;
    U32 MIDPI_B0 = 0;
    U32 MIDPI_B1 = 0;
    unsigned int temp;
    U8 backup_channel;
    DRAM_CHANNEL_T chIdx = CHANNEL_A;

    #ifdef IPMV21_DRM_WRKAROUND_STORE__DSM_EN__SHU_LEVEL__SHU_LEVEL_SRAM
    MD32_init();
    u4ConfShuLevel = u4IO32ReadFldAlign(DRAMC_REG_SA_RESERVE, SA_RESERVE_DRM_SHU_LEVEL);
    u4SramShuLevel = u4IO32ReadFldAlign(DRAMC_REG_SA_RESERVE, SA_RESERVE_DRM_SHU_SRAM_LEVEL);
    #else
    u4ConfShuLevel = MD32_init2();
    u4SramShuLevel = (u4ConfShuLevel >>4) & 0xF;
    u4ConfShuLevel = u4ConfShuLevel       & 0x3;
    MD32_init();
    #endif
    DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);

    backup_channel = vGetPHY2ChannelMapping(p);
    Init_Dram_Ctx_By_Type(p,mt_get_dram_type());
    vSetChannelNumber(p);
    vSetRankNumber(p);

    bSupport2Rank = (u4IO32ReadFldAlign(DRAMC_REG_SA_RESERVE, SA_RESERVE_SINGLE_RANK) == 0) ? TRUE : FALSE;
    dump_last_dramc(p);
#if DRM_DUMP_RG
    dump_drm_rg(p, u4ConfShuLevel);
#endif


    MIDPI_CA =  u4IO32Read4B(DDRPHY_REG_SHU_CA_CMD1);
    MIDPI_B0 =  u4IO32Read4B(DDRPHY_REG_SHU_B0_DQ1) ;
    MIDPI_B1 =  u4IO32Read4B(DDRPHY_REG_SHU_B1_DQ1) ;
//    MIDPI_B2 =  u4IO32Read4B(DDRPHY_REG_SHU_B2_DQ1) ;

    vIO32WriteFldAlign_All(DRAMC_REG_SWCMD_EN, 0x0, SWCMD_EN_RTSWCMDEN);//DRAMC_AO_BASE_ADDRESS + 0x0124[25], walk around for PC4 RTSWCMD trigger before reset

    mcSHOW_DRM_MSG(("@@@ WDT_SM=[%d]\n", u4IO32ReadFldAlign(DDRPHY_REG_MISC_DDR_RESERVE_STATE, MISC_DDR_RESERVE_STATE_WDT_SM) ));

    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_DDR_RESERVE2, 0x300, MISC_DDR_RESERVE2_DRM_SM_HOLD_CTRL);//0x6f4[31:16]
    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_DDR_RESERVE3, 0x7F, MISC_DDR_RESERVE3_DRM_SM_PASS_CTRL);
    mcDELAY_XUS(1);
    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_DDR_RESERVE3, 0x0,  MISC_DDR_RESERVE3_DRM_SM_PASS_CTRL);//CFC IPM V2.4 0x6f8[15: 0]

    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0, 0x0,LPIF_LOW_POWER_CFG_0_EMI_CLK_OFF_REQ);     //0x47004[13:12]

    mcSHOW_DRM_MSG(("@@@ conf_shuffle_level[%d] sram shuffle level[%d]\n", u4ConfShuLevel, u4SramShuLevel));

    mcSHOW_DRM_MSG(("To 26MHz\n"));
    vIO32WriteFldMulti_All(DDRPHY_REG_MISC_CG_CTRL0,
                  P_Fld(0, MISC_CG_CTRL0_W_CHG_MEM)
                | P_Fld(0, MISC_CG_CTRL0_CLK_MEM_SEL));//[5:4] mem_ck mux: 2'b00: 26MHz, [0]: change memory clock

    vIO32WriteFldMulti_All(DDRPHY_REG_MISC_CLK_CTRL, P_Fld(2, MISC_CLK_CTRL_DVFS_MEM_CK_MUX_SEL_MODE) );

    vIO32WriteFldMulti_All(DDRPHY_REG_MISC_CLK_CTRL, P_Fld(0, MISC_CLK_CTRL_DVFS_MEM_CK_MUX_SEL) );//[1]: 0:SW 1:HW

    vIO32WriteFldMulti_All(DDRPHY_REG_MISC_CG_CTRL0, P_Fld(1, MISC_CG_CTRL0_W_CHG_MEM));

    mcSHOW_DRM_MSG(("DELAY 1 us\n"));
    mcDELAY_XUS(1);

    mcSHOW_DRM_MSG(("disable memory clock change\n"));
    vIO32WriteFldMulti_All(DDRPHY_REG_MISC_CG_CTRL0, P_Fld(0, MISC_CG_CTRL0_W_CHG_MEM));

    temp = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_MISC_CLK_CTRL0), SHU_MISC_CLK_CTRL0_M_CK_OPENLOOP_MODE_SEL);

    if (temp) {
        mcSHOW_DRM_MSG(("DRAMC in open loop mode\n"));
        vIO32WriteFldAlign_All(DDRPHY_REG_MISC_CG_CTRL0, 0x0, MISC_CG_CTRL0_RESERVED_MISC_CG_CTRL0_BIT7);
//        vIO32WriteFldAlign_All(DDRPHY_REG_SHU_MISC_CLK_CTRL0, 0x0, SHU_MISC_CLK_CTRL0_M_CK_OPENLOOP_MODE_SEL);
        vIO32WriteFldAlign_All(DDRPHY_REG_MISC_CG_CTRL0, 0x1, MISC_CG_CTRL0_RESERVED_MISC_CG_CTRL0_BIT3_1);
        vIO32WriteFldAlign_All(DDRPHY_REG_MISC_CG_CTRL0, 0x0, MISC_CG_CTRL0_RESERVED_MISC_CG_CTRL0_BIT3_1);	
    }

#if (FOR_DV_SIMULATION_USED == 0)
#if EXIT_DRM_WITH_SAME_FREQ
    mcSHOW_DRM_MSG(("@@@ Out DRM with the same freq.\n"));
    vSetDFSTable(p, get_FreqTbl_by_SRAMIndex(p, u4SramShuLevel));
    DDRPhyFreqSel(p, p->pDFSTable->freq_sel);
#else
    p->frequency = u2DFSGetHighestFreq(p);
    vSetDFSTable(p, get_FreqTbl_by_SRAMIndex(p, SRAM_SHU0));
#endif
    mcSHOW_DRM_MSG(("@@@ Out DRM with DDR[%d]\n", p->frequency << 1));
#else
//    if(p->support_channel_num !=2) mcSHOW_DRM_MSG(("Set DRAMC Dual Channel\n"));//FOR_DV_SIMULATION_USE, never delet
//    p->support_channel_num = 2;//FOR_DV_SIMULATION_USE, never delet
#endif


    mcSHOW_DRM_MSG(("Start Dramc_DDR_Reserved_Mode_setting \n"));

    mcSHOW_DRM_MSG(("Enable BLCOK_ALE + DIS FAKEUP2 + CKEFIXON\n"));
    vIO32WriteFldAlign_All(DRAMC_REG_DRAMCTRL, 1,  DRAMCTRL_ALEBLOCK);
    vIO32WriteFldAlign_All(DRAMC_REG_SHU_DCM_CTRL0, 0,  SHU_DCM_CTRL0_FASTWAKE2);
//    vIO32WriteFldAlign_All(DRAMC_REG_CKECTRL, 1,  CKECTRL_CKEFIXON);
//    vIO32WriteFldAlign_All(DRAMC_REG_CKECTRL, 1,  CKECTRL_CKE1FIXON);

//!    mcSHOW_DRM_MSG(("APB clk To MCK\n"));//  ****Need to do before trigger DRM
////  vIO32WriteFldAlign_All(DDRPHY_REG_MISC_CKMUX_SEL, 0x3, MISC_CKMUX_SEL_FMEM_CK_MUX);

//!    vIO32WriteFldMulti_All(DDRPHY_REG_MISC_CG_CTRL0,
//!              P_Fld(0x0, MISC_CG_CTRL0_W_CHG_MEM)
//!            | P_Fld(0x0, MISC_CG_CTRL0_RESERVED_MISC_CG_CTRL0_BIT3_1)
//!            | P_Fld(0x0, MISC_CG_CTRL0_CLK_MEM_SEL));


//    u4value = u4IO32ReadFldAlign(DRAMC_REG_SHUSTATUS, SHUSTATUS_SHUFFLE_LEVEL);//from conf: 0 or 1 //==TODO
//    mcSHOW_ERR_MSG(("### shuffle level[%d]\n", u4value));

#ifdef HW_GATING
    mcSHOW_DRM_MSG(("Disable HW Gating tracking for gating tracking fifo mode, 0x0514, 0x04E8, 0x0D14\n"));
    DramcHWGatingOnOff(p, 0);
#endif

//    u4DramType = u4IO32ReadFldAlign(DRAMC_REG_ARBCTL, ARBCTL_RSV_DRAM_TYPE);//==TODO
//    mcSHOW_DRM_MSG(("### dram type =%x \n", p->dram_type));

#ifdef IMPEDANCE_TRACKING_ENABLE
    mcSHOW_DRM_MSG(("Disable IMP tracking\n"));
    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_IMPCAL, 0, MISC_IMPCAL_IMPCAL_HW);
#endif

#if ENABLE_TX_TRACKING
    mcSHOW_DRM_MSG(("Disable DQSOSC\n"));
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_RK_DQSOSC), 0, RK_DQSOSC_DQSOSCR_RK0EN);
    if ((bSupport2Rank == TRUE))
    {
        vSetRank(p, RANK_1);
        vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_RK_DQSOSC), 0, RK_DQSOSC_DQSOSCR_RK0EN);
    }
    vSetRank(p, RANK_0);
#endif

#ifdef ZQCS_ENABLE_LP4
    mcSHOW_DRM_MSG(("Disable HW ZQ\n"));
    vIO32WriteFldAlign_All(DRAMC_REG_ZQ_SET1, 0, ZQ_SET1_ZQCALDISB);
#endif

    mcSHOW_DRM_MSG(("Enable MD32 RG mode\n"));
    //*** WARNING***SA please change below write cmd to make sure write 2 MD32(Pe_trus)
    //*** WARNING***SA please change below write cmd to make sure write 1 MD32(Mar_gaux)
    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_FSM_CFG_1, 0, LPIF_FSM_CFG_1_LPIF_LEGACY_CONTROL); //NEED_DE_NOTICE

    mcSHOW_DRM_MSG(("Disable MD32 SPM mode\n"));
    //*** WARNING***SA please change below write cmd to make sure write 2 MD32(Pe_trus)
    //*** WARNING***SA please change below write cmd to make sure write 1 MD32(Mar_gaux)
    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_FSM_CFG_1, 1, LPIF_FSM_CFG_1_LPIF_OUTPUT_PATH_FROM_SW); //NEED_DE_NOTICE

#if ENABLE_TX_TRACKING
    mcSHOW_DRM_MSG(("Disable TX tracking\n"));
    //*** WARNING***SA please check vIO32WriteFldAlign_All cmd will write 2 MD32
    //*** WARNING***SA please check There are more write MD32 cmd below
    //*** WARNING***SA please change below write cmd to make sure write 2 MD32(Pe_trus)
    //*** WARNING***SA please change below write cmd to make sure write 1 MD32(Mar_gaux)
    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_1, CHAN_MODE, LPIF_LOW_POWER_CFG_1_TX_TRACKING_DIS);
#endif

#if ENABLE_RX_TRACKING
    mcSHOW_DRM_MSG(("Disable RX tracking\n"));
    //*** WARNING***SA please change below write cmd to make sure write 2 MD32(Pe_trus)
    //*** WARNING***SA please change below write cmd to make sure write 1 MD32(Mar_gaux)
    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_1, 0, LPIF_LOW_POWER_CFG_1_DPHY_RXDLY_TRACK_EN);
#endif

    mcSHOW_DRM_MSG(("For DDR800semi WA, CA_PI clock CG free-run, 0x04EC[12], 0x04EC[10], 0x0500[22]\n"));
    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_CG_CTRL0, 0x1, MISC_CG_CTRL0_RG_CG_CMD_OFF_DISABLE); //0x04EC[12]
    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_CG_CTRL0, 0x1, MISC_CG_CTRL0_RG_CG_PHY_OFF_DIABLE);  //0x04EC[10]
    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_CG_CTRL5, 0x0, MISC_CG_CTRL5_R_CA_PI_DCM_EN);        //0x0500[22]
    //lynx added
    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_CG_CTRL5, 0x0, MISC_CG_CTRL5_R_DQ0_PI_DCM_EN);        //0x0500[22]
    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_CG_CTRL5, 0x0, MISC_CG_CTRL5_R_DQ1_PI_DCM_EN);        //0x0500[22]
    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_CG_CTRL5, 0x0, MISC_CG_CTRL5_R_CA_DLY_DCM_EN);        //0x0500[22]
    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_CG_CTRL5, 0x0, MISC_CG_CTRL5_R_DQ0_DLY_DCM_EN);       //0x0500[22]
    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_CG_CTRL5, 0x0, MISC_CG_CTRL5_R_DQ1_DLY_DCM_EN);       //0x0500[22]

    mcSHOW_DRM_MSG(("Backup regs\n"));
    DramcBackupRegisters(p, u4ReserveRegBackupAddress, sizeof(u4ReserveRegBackupAddress)/sizeof(U32));

#if ENABLE_RX_TRACKING
    mcSHOW_DRM_MSG(("Close RX tracking\n"));
    mcSHOW_DRM_MSG(("Close RX tracking by conf RG, fix 1:4 mode enter DDR reserve mode fail. PIC: Lynx\n"));
    vIO32WriteFldAlign_All(DDRPHY_REG_B0_RXDVS0, 0x0, B0_RXDVS0_R_RX_DLY_TRACK_ENA_B0);
    vIO32WriteFldAlign_All(DDRPHY_REG_B1_RXDVS0, 0x0, B1_RXDVS0_R_RX_DLY_TRACK_ENA_B1);

    mcSHOW_DRM_MSG(("clear rx tracking FSM - start\n"));
    vIO32WriteFldAlign_All(DDRPHY_REG_B0_RXDVS0, 1, B0_RXDVS0_R_RX_DLY_TRACK_CLR_B0);
    vIO32WriteFldAlign_All(DDRPHY_REG_B1_RXDVS0, 1, B1_RXDVS0_R_RX_DLY_TRACK_CLR_B1);
    mcSHOW_DRM_MSG(("clear rx tracking FSM - end\n"));

    vIO32WriteFldAlign_All(DDRPHY_REG_RK_B0_RXDVS2, 0x0, RK_B0_RXDVS2_R_RK0_DVS_MODE_B0);
    vIO32WriteFldAlign_All(DDRPHY_REG_RK_B1_RXDVS2, 0x0, RK_B1_RXDVS2_R_RK0_DVS_MODE_B1);
    vIO32WriteFldAlign_All(DDRPHY_AO_RANK_OFFSET + DDRPHY_REG_RK_B0_RXDVS2, 0x0, RK_B0_RXDVS2_R_RK0_DVS_MODE_B0);
    vIO32WriteFldAlign_All(DDRPHY_AO_RANK_OFFSET + DDRPHY_REG_RK_B1_RXDVS2, 0x0, RK_B1_RXDVS2_R_RK0_DVS_MODE_B1);
#endif


#ifdef TEMP_SENSOR_ENABLE
    mcSHOW_DRM_MSG(("Disable MR4\n"));
    vIO32WriteFldAlign_All(DRAMC_REG_HMR4, 1, HMR4_REFRDIS);
    vIO32WriteFldAlign_All(DRAMC_REG_HMR4, 0, HMR4_SPDR_MR4_OPT); //Resume from S0 trigger HW MR4 function disable.
#endif

    mcSHOW_DRM_MSG(("Disable Dummy Read\n"));
    vIO32WriteFldMulti_All(DRAMC_REG_DUMMY_RD, P_Fld(0x0, DUMMY_RD_DQSG_DMYWR_EN)
            | P_Fld(0x0, DUMMY_RD_DQSG_DMYRD_EN)
            | P_Fld(0x0, DUMMY_RD_SREF_DMYRD_EN)
            | P_Fld(0x0, DUMMY_RD_DUMMY_RD_EN)
            | P_Fld(0x0, DUMMY_RD_DMY_RD_DBG)
            | P_Fld(0x0, DUMMY_RD_DMY_WR_DBG));

    mcSHOW_DRM_MSG(("Disable DDRPHY dynamic clock gating\n"));
    vIO32WriteFldAlign_All(DRAMC_REG_DRAMC_PD_CTRL, 0, DRAMC_PD_CTRL_PHYCLKDYNGEN);//disable DDRPHY dynamic clock gating


#if (FOR_DV_SIMULATION_USED == 1)
    mcSHOW_DRM_MSG(("@@@FOR_DV_SIMULATION_USED@@@\n"));
    mcSHOW_DRM_MSG(("@@@FOR_DV_SIMULATION_USED@@@\n"));
    mcSHOW_DRM_MSG(("@@@FOR_DV_SIMULATION_USED@@@\n"));


  #if ENABLE_RX_TRACKING
    mcSHOW_DRM_MSG((" @Darren, Fix rx tracking cause DDR3733 TA2 fail via ddr reserved mode\n"));
    vIO32WriteFldAlign_All(DDRPHY_REG_SHU_B0_DQ11, 0x0, SHU_B0_DQ11_RG_RX_ARDQ_DVS_EN_B0);
    vIO32WriteFldAlign_All(DDRPHY_REG_SHU_B1_DQ11, 0x0, SHU_B1_DQ11_RG_RX_ARDQ_DVS_EN_B1);
  #endif

#ifdef HW_GATING
    mcSHOW_DRM_MSG(("Disable HW Gating tracking for gating tracking fifo mode, 0x0514, 0x04E8, 0x0D14\n"));
    DramcHWGatingOnOff(p, 0);
  #endif

  #if ENABLE_TX_TRACKING
    vIO32WriteFldAlign_All(DRAMC_REG_DRAMC_PD_CTRL, 0x0, DRAMC_PD_CTRL_DCMEN2);

    //SW force reload from AO to NAO, TX tracking only track PI; ARUIDQ_SW = 1, UI will use AO conf
    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_SHU_CTRL0, 0, MISC_SHU_CTRL0_R_DMARPIDQ_SW);
    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_SHU_CTRL0, 1, MISC_SHU_CTRL0_R_DMARPIDQ_SW);
    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_SHU_CTRL0, 0, MISC_SHU_CTRL0_R_DMARPIDQ_SW);

    //vIO32WriteFldAlign_All(DRAMC_REG_DQSOSCR, 0, DQSOSCR_ARUIDQ_SW);
    //vIO32WriteFldAlign_All(DRAMC_REG_DQSOSCR, 1, DQSOSCR_ARUIDQ_SW);
    //vIO32WriteFldAlign_All(DRAMC_REG_DQSOSCR, 0, DQSOSCR_ARUIDQ_SW);
#endif

/*  mcSHOW_DRM_MSG(("Pipe line dcm off\n"));
    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_SHU_CG_CTRL0, u4IO32Read4B(DDRPHY_REG_MISC_SHU_CG_CTRL0) | 0x33000000, MISC_SHU_CG_CTRL0_R_PHY_MCK_CG_CTRL);


    vIO32WriteFldMulti_All(DDRPHY_REG_MISC_CLK_CTRL , P_Fld(0, MISC_CLK_CTRL_DVFS_CLK_MEM_SEL) );//[1]: 0:SW 1:HW

    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_CG_CTRL0, 1, MISC_CG_CTRL0_W_CHG_MEM);//change clock freq

    mcSHOW_DRM_MSG(("DELAY 1 us\n"));
    mcDELAY_XUS(1);

    mcSHOW_DRM_MSG(("disable memory clock change\n"));
    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_CG_CTRL0, 0, MISC_CG_CTRL0_W_CHG_MEM);*/

    //===========================================================================================================
    //PLL close sequence: (Need to sync with Maoauo)
    //DLL_PHDET_EN_* = 0
    //==> CG = 1
    //==> MIDPI_EN = 0(async)
    //==> RG_ARPI_RESETB_* = 0
    //==> MCK8X_EN(source of clk gating) = 0
    //==> PLL_EN = 0 PIC: Ying-Yu

    //
    mcSHOW_DRM_MSG(("RG_*PHDET_EN = 0 (DLL off)\n"));
#if 0
#if ENABLE_MCK8X_MODE
    //vIO32WriteFldAlign_All(DDRPHY_SHU_CA_DLL0, 0x0, SHU_CA_DLL0_RG_ARDLL_PHDET_EN_CA_SHU);
    //vIO32WriteFldAlign_All(DDRPHY_SHU_B0_DLL0, 0x0, SHU_B0_DLL0_RG_ARDLL_PHDET_EN_B0_SHU);
    //vIO32WriteFldAlign_All(DDRPHY_SHU_B1_DLL0, 0x0, SHU_B1_DLL0_RG_ARDLL_PHDET_EN_B1_SHU);
#else
    vIO32WriteFldAlign_All(DDRPHY_CA_DLL_ARPI2, 0x0, CA_DLL_ARPI2_RG_ARDLL_PHDET_EN_CA);
    vIO32WriteFldAlign_All(DDRPHY_B0_DLL_ARPI2, 0x0, B0_DLL_ARPI2_RG_ARDLL_PHDET_EN_B0);
    vIO32WriteFldAlign_All(DDRPHY_B1_DLL_ARPI2, 0x0, B1_DLL_ARPI2_RG_ARDLL_PHDET_EN_B1);
#endif
#endif
    vIO32WriteFldAlign_All(DDRPHY_REG_SHU_B0_DLL1, 0x0, SHU_B0_DLL1_RG_ARDLL_PHDET_EN_B0);
    vIO32WriteFldAlign_All(DDRPHY_REG_SHU_B1_DLL1, 0x0, SHU_B1_DLL1_RG_ARDLL_PHDET_EN_B1);
    vIO32WriteFldAlign_All(DDRPHY_REG_SHU_CA_DLL1, 0x0, SHU_CA_DLL1_RG_ARDLL_PHDET_EN_CA);

    mcSHOW_DRM_MSG(("RG_*VREF_EN=0\n"));
    vIO32WriteFldAlign_All(DDRPHY_REG_B0_DQ5, 0x0, B0_DQ5_RG_RX_ARDQ_VREF_EN_B0);
    vIO32WriteFldAlign_All(DDRPHY_REG_B1_DQ5, 0x0, B1_DQ5_RG_RX_ARDQ_VREF_EN_B1);
    vIO32WriteFldAlign_All(DDRPHY_REG_CA_CMD5, 0x0, CA_CMD5_RG_RX_ARCMD_VREF_EN);

    mcSHOW_DRM_MSG(("ASVA 2-1 Multi-phase Ready : disable clk CG after DLL disable\n"));
    vIO32WriteFldMulti_All(DDRPHY_REG_MISC_CTRL3         ,
                  P_Fld(0x1, MISC_CTRL3_ARPI_MPDIV_CG_DQ_OPT)
                | P_Fld(0x1, MISC_CTRL3_ARPI_MPDIV_CG_CA_OPT));

    mcSHOW_DRM_MSG(("*PI_CG = 1, RG_*MPDIV_CG = 1\n"));
    vIO32WriteFldMulti_All(DDRPHY_REG_SHU_B0_DLL_ARPI2,//
                  P_Fld(0x1, SHU_B0_DLL_ARPI2_RG_ARPI_CG_MCK_B0)
                | P_Fld(0x1, SHU_B0_DLL_ARPI2_RG_ARPI_CG_MCK_FB2DLL_B0)
                | P_Fld(0x1, SHU_B0_DLL_ARPI2_RG_ARPI_CG_MCTL_B0)
                | P_Fld(0x1, SHU_B0_DLL_ARPI2_RG_ARPI_CG_FB_B0)
                | P_Fld(0x1, SHU_B0_DLL_ARPI2_RG_ARPI_CG_DQS_B0)
                | P_Fld(0x1, SHU_B0_DLL_ARPI2_RG_ARPI_CG_DQM_B0)
                | P_Fld(0x1, SHU_B0_DLL_ARPI2_RG_ARPI_CG_DQ_B0)
                | P_Fld(0x1, SHU_B0_DLL_ARPI2_RG_ARPI_CG_DQSIEN_B0)
                | P_Fld(0x1, SHU_B0_DLL_ARPI2_RG_ARPI_MPDIV_CG_B0));
    vIO32WriteFldMulti_All(DDRPHY_REG_SHU_B1_DLL_ARPI2,//
                  P_Fld(0x1, SHU_B1_DLL_ARPI2_RG_ARPI_CG_MCK_B1)
                | P_Fld(0x1, SHU_B1_DLL_ARPI2_RG_ARPI_CG_MCK_FB2DLL_B1)
                | P_Fld(0x1, SHU_B1_DLL_ARPI2_RG_ARPI_CG_MCTL_B1)
                | P_Fld(0x1, SHU_B1_DLL_ARPI2_RG_ARPI_CG_FB_B1)
                | P_Fld(0x1, SHU_B1_DLL_ARPI2_RG_ARPI_CG_DQS_B1)
                | P_Fld(0x1, SHU_B1_DLL_ARPI2_RG_ARPI_CG_DQM_B1)
                | P_Fld(0x1, SHU_B1_DLL_ARPI2_RG_ARPI_CG_DQ_B1)
                | P_Fld(0x1, SHU_B1_DLL_ARPI2_RG_ARPI_CG_DQSIEN_B1)
                | P_Fld(0x1, SHU_B1_DLL_ARPI2_RG_ARPI_MPDIV_CG_B1));
    vIO32WriteFldMulti_All(DDRPHY_REG_SHU_CA_DLL_ARPI2,//
                  P_Fld(0x1, SHU_CA_DLL_ARPI2_RG_ARPI_CG_MCK_CA)
                | P_Fld(0x1, SHU_CA_DLL_ARPI2_RG_ARPI_CG_MCK_FB2DLL_CA)
                | P_Fld(0x1, SHU_CA_DLL_ARPI2_RG_ARPI_CG_MCTL_CA)
                | P_Fld(0x1, SHU_CA_DLL_ARPI2_RG_ARPI_CG_FB_CA)
                | P_Fld(0x1, SHU_CA_DLL_ARPI2_RG_ARPI_CG_CS)
                | P_Fld(0x1, SHU_CA_DLL_ARPI2_RG_ARPI_CG_CLK)
                | P_Fld(0x1, SHU_CA_DLL_ARPI2_RG_ARPI_CG_CMD)
                | P_Fld(0x1, SHU_CA_DLL_ARPI2_RG_ARPI_CG_CLKIEN)
                | P_Fld(0x1, SHU_CA_DLL_ARPI2_RG_ARPI_MPDIV_CG_CA));

    mcSHOW_DRM_MSG(("RG_*MIDPI_EN = 0\n"));
    vIO32WriteFldAlign_All(DDRPHY_REG_SHU_B0_DQ1, 0x0, SHU_B0_DQ1_RG_ARPI_MIDPI_EN_B0);//874
    vIO32WriteFldAlign_All(DDRPHY_REG_SHU_B1_DQ1, 0x0, SHU_B1_DQ1_RG_ARPI_MIDPI_EN_B1);//9F4
//    vIO32WriteFldAlign_All(DDRPHY_REG_SHU_B2_DQ1, 0x0, SHU_B2_DQ1_RG_ARPI_MIDPI_EN_B2);
    vIO32WriteFldAlign_All(DDRPHY_REG_SHU_CA_CMD1, 0x0, SHU_CA_CMD1_RG_ARPI_MIDPI_EN_CA);

    vIO32WriteFldAlign_All(DDRPHY_REG_SHU_B0_DQ1, 0x0, SHU_B0_DQ1_RG_ARPI_MIDPI_CKDIV4_EN_B0);//874
    vIO32WriteFldAlign_All(DDRPHY_REG_SHU_B1_DQ1, 0x0, SHU_B1_DQ1_RG_ARPI_MIDPI_CKDIV4_EN_B1);//9F4
//    vIO32WriteFldAlign_All(DDRPHY_REG_SHU_B2_DQ1, 0x0, SHU_B2_DQ1_RG_ARPI_MIDPI_CKDIV4_EN_B2);
    vIO32WriteFldAlign_All(DDRPHY_REG_SHU_CA_CMD1, 0x0, SHU_CA_CMD1_RG_ARPI_MIDPI_CKDIV4_EN_CA);

//    vIO32WriteFldAlign_All(DDRPHY_REG_SHU_B0_DQ1, 0x0, SHU_B0_DQ1_RG_ARPI_MIDPI_8PH_DLY_B0);//874
//    vIO32WriteFldAlign_All(DDRPHY_REG_SHU_B1_DQ1, 0x0, SHU_B1_DQ1_RG_ARPI_MIDPI_8PH_DLY_B1);//9F4
////    vIO32WriteFldAlign_All(DDRPHY_REG_SHU_B2_DQ1, 0x0, SHU_B2_DQ1_RG_ARPI_MIDPI_8PH_DLY_B2);
//    vIO32WriteFldAlign_All(DDRPHY_REG_SHU_CA_CMD1, 0x0, SHU_CA_CMD1_RG_ARPI_MIDPI_8PH_DLY_CA);
//
//    vIO32WriteFldAlign_All(DDRPHY_REG_SHU_B0_DQ1, 0x0, SHU_B0_DQ1_RG_ARPI_8PHASE_XLATCH_FORCE_B0);//874
//    vIO32WriteFldAlign_All(DDRPHY_REG_SHU_B1_DQ1, 0x0, SHU_B1_DQ1_RG_ARPI_8PHASE_XLATCH_FORCE_B1);//9F4
////    vIO32WriteFldAlign_All(DDRPHY_REG_SHU_B2_DQ1, 0x0, SHU_B2_DQ1_RG_ARPI_8PHASE_XLATCH_FORCE_B2);
//    vIO32WriteFldAlign_All(DDRPHY_REG_SHU_CA_CMD1, 0x0, SHU_CA_CMD1_RG_ARPI_8PHASE_XLATCH_FORCE_CA);

    mcSHOW_DRM_MSG(("RG_*BIAS_EN = 0\n"));
    vIO32WriteFldAlign_All(DDRPHY_REG_B0_DQ6, 0x0, B0_DQ6_RG_RX_ARDQ_BIAS_EN_B0);
    vIO32WriteFldAlign_All(DDRPHY_REG_B1_DQ6, 0x0, B1_DQ6_RG_RX_ARDQ_BIAS_EN_B1);
    vIO32WriteFldAlign_All(DDRPHY_REG_CA_CMD6, 0x0, CA_CMD6_RG_RX_ARCMD_BIAS_EN);

    mcSHOW_DRM_MSG(("RG_*RESETB = 0\n"));
    vIO32WriteFldAlign_All(DDRPHY_REG_B0_DQ3, 0x0, B0_DQ3_RG_ARDQ_RESETB_B0);
    vIO32WriteFldAlign_All(DDRPHY_REG_B0_DLL_ARPI0, 0x0, B0_DLL_ARPI0_RG_ARPI_RESETB_B0);
    vIO32WriteFldAlign_All(DDRPHY_REG_B1_DQ3, 0x0, B1_DQ3_RG_ARDQ_RESETB_B1);
    vIO32WriteFldAlign_All(DDRPHY_REG_B1_DLL_ARPI0, 0x0, B1_DLL_ARPI0_RG_ARPI_RESETB_B1);
    vIO32WriteFldAlign_All(DDRPHY_REG_CA_CMD3, 0x0, CA_CMD3_RG_ARCMD_RESETB);
    vIO32WriteFldAlign_All(DDRPHY_REG_CA_DLL_ARPI0, 0x0, CA_DLL_ARPI0_RG_ARPI_RESETB_CA);
    vIO32WriteFldAlign_All(DDRPHY_REG_PHYPLL2, 0x0, PHYPLL2_RG_RPHYPLL_RESETB);//Since there is only 1 PLL, only control CHA //Follow Petrus_DDR_reserved C code_20190123.docx

    mcSHOW_DRM_MSG(("RG_*MCK8X_EN = 0   \n"));
#if 0
#if ENABLE_MCK8X_MODE
    //vIO32WriteFldAlign(DDRPHY_SHU_PLL22, 0, SHU_PLL22_RG_RPHYPLL_ADA_MCK8X_EN_SHU);
    //vIO32WriteFldAlign(DDRPHY_PLL4, 0, PLL4_RG_RPHYPLL_AD_MCK8X_EN);//RG_*MCK8X_EN=0; Since there is only 1 PLL, only to control CHA
#else
    vIO32WriteFldMulti(DDRPHY_REG_PHYPLL2, P_Fld(0, PHYPLL2_RG_RPHYPLL_ADA_MCK8X_EN)
                    | P_Fld(0, PHYPLL2_RG_RPHYPLL_AD_MCK8X_EN));//RG_*MCK8X_EN=0; Since there is only 1 PLL, only to control CHA
#endif
#endif
    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_DVFSCTL2, 0, MISC_DVFSCTL2_RG_ADA_MCK8X_EN_SHUFFLE);//Follow Petrus_DDR_reserved C code_20190123.docx
    vIO32WriteFldAlign_All(DDRPHY_REG_PHYPLL2, 0, PHYPLL2_RG_RPHYPLL_AD_MCK8X_EN);//RG_*MCK8X_EN=0; Since there is only 1 PLL, only to control CHA //Follow Petrus_DDR_reserved C code_20190123.docx
    vIO32WriteFldAlign_All(DDRPHY_REG_PHYPLL2, 0, PHYPLL2_RG_RPHYPLL_ADA_MCK8X_EN);//RG_*MCK8X_EN=0; Since there is only 1 PLL, only to control CHA //Follow Petrus_DDR_reserved C code_20190123.docx
    vIO32WriteFldAlign_All(DDRPHY_REG_SHU_PLL2, 0, SHU_PLL2_RG_RPHYPLL_ADA_MCK8X_EN_SHU);  //refer to MISC_DVFSCTRL2


    mcSHOW_DRM_MSG(("PLL_EN = 0\n"));
    vIO32WriteFldAlign_All(DDRPHY_REG_PHYPLL0, 0x0, PHYPLL0_RG_RPHYPLL_EN);//RG_*PLL_EN=0; Since there is only 1 PLL, only control CHA //Follow Petrus_DDR_reserved C code_20190123.docx
    vIO32WriteFldAlign_All(DDRPHY_REG_CLRPLL0, 0x0, CLRPLL0_RG_RCLRPLL_EN);//RG_*PLL_EN=0; Since there is only 1 PLL, only control CHA //Follow Petrus_DDR_reserved C code_20190123.docx

#endif

#if EXIT_DRM_WITH_SAME_FREQ
    mcSHOW_DRM_MSG(("Check if Load previous shuffle setting from SRAM by DMA \n"));
    if(u4ConfShuLevel)//from CLRPLL
    {
        mcSHOW_DRM_MSG(("from CLRPLL \n"));
    #ifdef IPMV21_DRM_WRKAROUND_STORE__DSM_EN__SHU_LEVEL__SHU_LEVEL_SRAM //conf AO only, IPM V2.1, V2.3
        DramcSaveToShuffleSRAM(p, DRAM_DFS_REG_SHU1, u4SramShuLevel);//To avoid DRAMC violoate SPEC while from Soff/S0 to ON
        mcSHOW_DRM_MSG(("DRM_SHU_SRAM_WA conf1 to shu_sram done\n"));
    #endif
        LoadShuffleSRAMtoDramc(p, u4SramShuLevel, DRAM_DFS_REG_SHU0); //NEED_DE_NOTICE
        mcSHOW_DRM_MSG(("DRM_SHU_SRAM_WA shu_sram to conf0 done\n"));
    MIDPI_CA =  u4IO32Read4B(DDRPHY_REG_SHU_CA_CMD1);//0x0E74
    MIDPI_B0 =  u4IO32Read4B(DDRPHY_REG_SHU_B0_DQ1) ;//0x0EF4
    MIDPI_B1 =  u4IO32Read4B(DDRPHY_REG_SHU_B1_DQ1) ;//0x0F74
//    MIDPI_B2 =  u4IO32Read4B(DDRPHY_REG_SHU_B2_DQ1) ;//0x0FF4
    }
#else
    #ifdef IPMV21_DRM_WRKAROUND_STORE__DSM_EN__SHU_LEVEL__SHU_LEVEL_SRAM //conf AO only, IPM V2.1, V2.3
    if(u4SramShuLevel==0)
    {
        if(u4ConfShuLevel)//from CLRPLL
        {
            DramcSaveToShuffleSRAM(p, DRAM_DFS_REG_SHU1, u4SramShuLevel);//To avoid DRAMC violoate SPEC while from Soff/S0 to ON
            mcSHOW_DRM_MSG(("DRM_SHU_SRAM_WA conf1 to shu_sram done\n"));
            LoadShuffleSRAMtoDramc(p, u4SramShuLevel, DRAM_DFS_REG_SHU0); //NEED_DE_NOTICE
            mcSHOW_DRM_MSG(("DRM_SHU_SRAM_WA shu_sram to conf0 done\n"));
    MIDPI_CA =  u4IO32Read4B(DDRPHY_REG_SHU_CA_CMD1);//0x0E74
    MIDPI_B0 =  u4IO32Read4B(DDRPHY_REG_SHU_B0_DQ1) ;//0x0EF4
    MIDPI_B1 =  u4IO32Read4B(DDRPHY_REG_SHU_B1_DQ1) ;//0x0F74
//    MIDPI_B2 =  u4IO32Read4B(DDRPHY_REG_SHU_B2_DQ1) ;//0x0FF4
        }
    }
    else
    {
        mcSHOW_DRM_MSG(("Load shuffle 0 setting from SRAM by DMA \n"));
        LoadShuffleSRAMtoDramc(p, 0, DRAM_DFS_REG_SHU0); //NEED_DE_NOTICE
    MIDPI_CA =  u4IO32Read4B(DDRPHY_REG_SHU_CA_CMD1);//0x0E74
    MIDPI_B0 =  u4IO32Read4B(DDRPHY_REG_SHU_B0_DQ1) ;//0x0EF4
    MIDPI_B1 =  u4IO32Read4B(DDRPHY_REG_SHU_B1_DQ1) ;//0x0F74
//    MIDPI_B2 =  u4IO32Read4B(DDRPHY_REG_SHU_B2_DQ1) ;//0x0FF4
    }
    #else
    mcSHOW_DRM_MSG(("Load shuffle 0 setting from SRAM by DMA \n"));
    LoadShuffleSRAMtoDramc(p, 0, DRAM_DFS_REG_SHU0); //NEED_DE_NOTICE
    MIDPI_CA =  u4IO32Read4B(DDRPHY_REG_SHU_CA_CMD1);//0x0E74
    MIDPI_B0 =  u4IO32Read4B(DDRPHY_REG_SHU_B0_DQ1) ;//0x0EF4
    MIDPI_B1 =  u4IO32Read4B(DDRPHY_REG_SHU_B1_DQ1) ;//0x0F74
//    MIDPI_B2 =  u4IO32Read4B(DDRPHY_REG_SHU_B2_DQ1) ;//0x0FF4
    #endif
#endif

    //mcSHOW_DRM_MSG(("Pipe line dcm off\n")); //Dummy code, since the value is the same with golden setting
    //u4value = u4IO32ReadFldAlign(DDRPHY_REG_MISC_SHU_CG_CTRL0, MISC_SHU_CG_CTRL0_R_PHY_MCK_CG_CTRL);
    //vIO32WriteFldAlign_All(DDRPHY_REG_MISC_SHU_CG_CTRL0, u4value | 0x33000000, MISC_SHU_CG_CTRL0_R_PHY_MCK_CG_CTRL);

#if ENABLE_RX_TRACKING
    mcSHOW_DRM_MSG((" @Darren, Fix rx tracking cause DDR3733 TA2 fail via ddr reserved mode\n"));
    vIO32WriteFldAlign_All(DDRPHY_REG_SHU_B0_DQ11, 0x0, SHU_B0_DQ11_RG_RX_ARDQ_DVS_EN_B0);
    vIO32WriteFldAlign_All(DDRPHY_REG_SHU_B1_DQ11, 0x0, SHU_B1_DQ11_RG_RX_ARDQ_DVS_EN_B1);
#endif

#ifdef HW_GATING
    mcSHOW_DRM_MSG(("Disable HW Gating tracking for gating tracking fifo mode, 0x0514, 0x04E8, 0x0D14\n"));
    DramcHWGatingOnOff(p, 0);
#endif

#if ENABLE_TX_TRACKING
    vIO32WriteFldAlign_All(DRAMC_REG_DRAMC_PD_CTRL, 0x0, DRAMC_PD_CTRL_DCMEN2);

    //SW force reload from AO to NAO, TX tracking only track PI; ARUIDQ_SW = 1, UI will use AO conf
    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_SHU_CTRL0, 0, MISC_SHU_CTRL0_R_DMARPIDQ_SW);
    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_SHU_CTRL0, 1, MISC_SHU_CTRL0_R_DMARPIDQ_SW);
    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_SHU_CTRL0, 0, MISC_SHU_CTRL0_R_DMARPIDQ_SW);

    //vIO32WriteFldAlign_All(DRAMC_REG_DQSOSCR, 0, DQSOSCR_ARUIDQ_SW);
    //vIO32WriteFldAlign_All(DRAMC_REG_DQSOSCR, 1, DQSOSCR_ARUIDQ_SW);
    //vIO32WriteFldAlign_All(DRAMC_REG_DQSOSCR, 0, DQSOSCR_ARUIDQ_SW);
#endif

/*  mcSHOW_DRM_MSG(("Pipe line dcm off\n"));
    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_SHU_CG_CTRL0, u4IO32Read4B(DDRPHY_REG_MISC_SHU_CG_CTRL0) | 0x33000000, MISC_SHU_CG_CTRL0_R_PHY_MCK_CG_CTRL);


    vIO32WriteFldMulti_All(DDRPHY_REG_MISC_CLK_CTRL , P_Fld(0, MISC_CLK_CTRL_DVFS_CLK_MEM_SEL) );//[1]: 0:SW 1:HW

    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_CG_CTRL0, 1, MISC_CG_CTRL0_W_CHG_MEM);//change clock freq

    mcSHOW_DRM_MSG(("DELAY 1 us\n"));
    mcDELAY_XUS(1);

    mcSHOW_DRM_MSG(("disable memory clock change\n"));
    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_CG_CTRL0, 0, MISC_CG_CTRL0_W_CHG_MEM);*/

    //===========================================================================================================
    //PLL close sequence: (Need to sync with Maoauo)
    //DLL_PHDET_EN_* = 0
    //==> CG = 1
    //==> MIDPI_EN = 0(async)
    //==> RG_ARPI_RESETB_* = 0
    //==> MCK8X_EN(source of clk gating) = 0
    //==> PLL_EN = 0 PIC: Ying-Yu

    //
    mcSHOW_DRM_MSG(("RG_*PHDET_EN = 0 (DLL off)\n"));
#if 0
#if ENABLE_MCK8X_MODE
    //vIO32WriteFldAlign_All(DDRPHY_SHU_CA_DLL0, 0x0, SHU_CA_DLL0_RG_ARDLL_PHDET_EN_CA_SHU);
    //vIO32WriteFldAlign_All(DDRPHY_SHU_B0_DLL0, 0x0, SHU_B0_DLL0_RG_ARDLL_PHDET_EN_B0_SHU);
    //vIO32WriteFldAlign_All(DDRPHY_SHU_B1_DLL0, 0x0, SHU_B1_DLL0_RG_ARDLL_PHDET_EN_B1_SHU);
#else
    vIO32WriteFldAlign_All(DDRPHY_CA_DLL_ARPI2, 0x0, CA_DLL_ARPI2_RG_ARDLL_PHDET_EN_CA);
    vIO32WriteFldAlign_All(DDRPHY_B0_DLL_ARPI2, 0x0, B0_DLL_ARPI2_RG_ARDLL_PHDET_EN_B0);
    vIO32WriteFldAlign_All(DDRPHY_B1_DLL_ARPI2, 0x0, B1_DLL_ARPI2_RG_ARDLL_PHDET_EN_B1);
#endif
#endif
    vIO32WriteFldAlign_All(DDRPHY_REG_SHU_B0_DLL1, 0x0, SHU_B0_DLL1_RG_ARDLL_PHDET_EN_B0);
    vIO32WriteFldAlign_All(DDRPHY_REG_SHU_B1_DLL1, 0x0, SHU_B1_DLL1_RG_ARDLL_PHDET_EN_B1);
    vIO32WriteFldAlign_All(DDRPHY_REG_SHU_CA_DLL1, 0x0, SHU_CA_DLL1_RG_ARDLL_PHDET_EN_CA);

    mcSHOW_DRM_MSG(("RG_*VREF_EN=0\n"));
    vIO32WriteFldAlign_All(DDRPHY_REG_B0_DQ5, 0x0, B0_DQ5_RG_RX_ARDQ_VREF_EN_B0);
    vIO32WriteFldAlign_All(DDRPHY_REG_B1_DQ5, 0x0, B1_DQ5_RG_RX_ARDQ_VREF_EN_B1);
    vIO32WriteFldAlign_All(DDRPHY_REG_CA_CMD5, 0x0, CA_CMD5_RG_RX_ARCMD_VREF_EN);

    mcSHOW_DRM_MSG(("ASVA 2-1 Multi-phase Ready : disable clk CG after DLL disable\n"));
    vIO32WriteFldMulti_All(DDRPHY_REG_MISC_CTRL3         ,
                  P_Fld(0x1, MISC_CTRL3_ARPI_MPDIV_CG_DQ_OPT)
                | P_Fld(0x1, MISC_CTRL3_ARPI_MPDIV_CG_CA_OPT));

    mcSHOW_DRM_MSG(("*PI_CG = 1, RG_*MPDIV_CG = 1\n"));
    vIO32WriteFldMulti_All(DDRPHY_REG_SHU_B0_DLL_ARPI2,//
                  P_Fld(0x1, SHU_B0_DLL_ARPI2_RG_ARPI_CG_MCK_B0)
                | P_Fld(0x1, SHU_B0_DLL_ARPI2_RG_ARPI_CG_MCK_FB2DLL_B0)
                | P_Fld(0x1, SHU_B0_DLL_ARPI2_RG_ARPI_CG_MCTL_B0)
                | P_Fld(0x1, SHU_B0_DLL_ARPI2_RG_ARPI_CG_FB_B0)
                | P_Fld(0x1, SHU_B0_DLL_ARPI2_RG_ARPI_CG_DQS_B0)
                | P_Fld(0x1, SHU_B0_DLL_ARPI2_RG_ARPI_CG_DQM_B0)
                | P_Fld(0x1, SHU_B0_DLL_ARPI2_RG_ARPI_CG_DQ_B0)
                | P_Fld(0x1, SHU_B0_DLL_ARPI2_RG_ARPI_CG_DQSIEN_B0)
                | P_Fld(0x1, SHU_B0_DLL_ARPI2_RG_ARPI_MPDIV_CG_B0));
    vIO32WriteFldMulti_All(DDRPHY_REG_SHU_B1_DLL_ARPI2,//
                  P_Fld(0x1, SHU_B1_DLL_ARPI2_RG_ARPI_CG_MCK_B1)
                | P_Fld(0x1, SHU_B1_DLL_ARPI2_RG_ARPI_CG_MCK_FB2DLL_B1)
                | P_Fld(0x1, SHU_B1_DLL_ARPI2_RG_ARPI_CG_MCTL_B1)
                | P_Fld(0x1, SHU_B1_DLL_ARPI2_RG_ARPI_CG_FB_B1)
                | P_Fld(0x1, SHU_B1_DLL_ARPI2_RG_ARPI_CG_DQS_B1)
                | P_Fld(0x1, SHU_B1_DLL_ARPI2_RG_ARPI_CG_DQM_B1)
                | P_Fld(0x1, SHU_B1_DLL_ARPI2_RG_ARPI_CG_DQ_B1)
                | P_Fld(0x1, SHU_B1_DLL_ARPI2_RG_ARPI_CG_DQSIEN_B1)
                | P_Fld(0x1, SHU_B1_DLL_ARPI2_RG_ARPI_MPDIV_CG_B1));
    vIO32WriteFldMulti_All(DDRPHY_REG_SHU_CA_DLL_ARPI2,//
                  P_Fld(0x1, SHU_CA_DLL_ARPI2_RG_ARPI_CG_MCK_CA)
                | P_Fld(0x1, SHU_CA_DLL_ARPI2_RG_ARPI_CG_MCK_FB2DLL_CA)
                | P_Fld(0x1, SHU_CA_DLL_ARPI2_RG_ARPI_CG_MCTL_CA)
                | P_Fld(0x1, SHU_CA_DLL_ARPI2_RG_ARPI_CG_FB_CA)
                | P_Fld(0x1, SHU_CA_DLL_ARPI2_RG_ARPI_CG_CS)
                | P_Fld(0x1, SHU_CA_DLL_ARPI2_RG_ARPI_CG_CLK)
                | P_Fld(0x1, SHU_CA_DLL_ARPI2_RG_ARPI_CG_CMD)
                | P_Fld(0x1, SHU_CA_DLL_ARPI2_RG_ARPI_CG_CLKIEN)
                | P_Fld(0x1, SHU_CA_DLL_ARPI2_RG_ARPI_MPDIV_CG_CA));

    mcSHOW_DRM_MSG(("RG_*MIDPI_EN = 0\n"));
    vIO32WriteFldAlign_All(DDRPHY_REG_SHU_B0_DQ1, 0x0, SHU_B0_DQ1_RG_ARPI_MIDPI_EN_B0);//874
    vIO32WriteFldAlign_All(DDRPHY_REG_SHU_B1_DQ1, 0x0, SHU_B1_DQ1_RG_ARPI_MIDPI_EN_B1);//9F4
//    vIO32WriteFldAlign_All(DDRPHY_REG_SHU_B2_DQ1, 0x0, SHU_B2_DQ1_RG_ARPI_MIDPI_EN_B2);
    vIO32WriteFldAlign_All(DDRPHY_REG_SHU_CA_CMD1, 0x0, SHU_CA_CMD1_RG_ARPI_MIDPI_EN_CA);

    vIO32WriteFldAlign_All(DDRPHY_REG_SHU_B0_DQ1, 0x0, SHU_B0_DQ1_RG_ARPI_MIDPI_CKDIV4_EN_B0);//874
    vIO32WriteFldAlign_All(DDRPHY_REG_SHU_B1_DQ1, 0x0, SHU_B1_DQ1_RG_ARPI_MIDPI_CKDIV4_EN_B1);//9F4
//    vIO32WriteFldAlign_All(DDRPHY_REG_SHU_B2_DQ1, 0x0, SHU_B2_DQ1_RG_ARPI_MIDPI_CKDIV4_EN_B2);
    vIO32WriteFldAlign_All(DDRPHY_REG_SHU_CA_CMD1, 0x0, SHU_CA_CMD1_RG_ARPI_MIDPI_CKDIV4_EN_CA);

//    vIO32WriteFldAlign_All(DDRPHY_REG_SHU_B0_DQ1, 0x0, SHU_B0_DQ1_RG_ARPI_MIDPI_8PH_DLY_B0);//874
//    vIO32WriteFldAlign_All(DDRPHY_REG_SHU_B1_DQ1, 0x0, SHU_B1_DQ1_RG_ARPI_MIDPI_8PH_DLY_B1);//9F4
////    vIO32WriteFldAlign_All(DDRPHY_REG_SHU_B2_DQ1, 0x0, SHU_B2_DQ1_RG_ARPI_MIDPI_8PH_DLY_B2);
//    vIO32WriteFldAlign_All(DDRPHY_REG_SHU_CA_CMD1, 0x0, SHU_CA_CMD1_RG_ARPI_MIDPI_8PH_DLY_CA);
//
//    vIO32WriteFldAlign_All(DDRPHY_REG_SHU_B0_DQ1, 0x0, SHU_B0_DQ1_RG_ARPI_8PHASE_XLATCH_FORCE_B0);//874
//    vIO32WriteFldAlign_All(DDRPHY_REG_SHU_B1_DQ1, 0x0, SHU_B1_DQ1_RG_ARPI_8PHASE_XLATCH_FORCE_B1);//9F4
////    vIO32WriteFldAlign_All(DDRPHY_REG_SHU_B2_DQ1, 0x0, SHU_B2_DQ1_RG_ARPI_8PHASE_XLATCH_FORCE_B2);
//    vIO32WriteFldAlign_All(DDRPHY_REG_SHU_CA_CMD1, 0x0, SHU_CA_CMD1_RG_ARPI_8PHASE_XLATCH_FORCE_CA);

    mcSHOW_DRM_MSG(("RG_*BIAS_EN = 0\n"));
    vIO32WriteFldAlign_All(DDRPHY_REG_B0_DQ6, 0x0, B0_DQ6_RG_RX_ARDQ_BIAS_EN_B0);
    vIO32WriteFldAlign_All(DDRPHY_REG_B1_DQ6, 0x0, B1_DQ6_RG_RX_ARDQ_BIAS_EN_B1);
    vIO32WriteFldAlign_All(DDRPHY_REG_CA_CMD6, 0x0, CA_CMD6_RG_RX_ARCMD_BIAS_EN);

    mcSHOW_DRM_MSG(("RG_*RESETB = 0\n"));
    vIO32WriteFldAlign_All(DDRPHY_REG_B0_DQ3, 0x0, B0_DQ3_RG_ARDQ_RESETB_B0);
    vIO32WriteFldAlign_All(DDRPHY_REG_B0_DLL_ARPI0, 0x0, B0_DLL_ARPI0_RG_ARPI_RESETB_B0);
    vIO32WriteFldAlign_All(DDRPHY_REG_B1_DQ3, 0x0, B1_DQ3_RG_ARDQ_RESETB_B1);
    vIO32WriteFldAlign_All(DDRPHY_REG_B1_DLL_ARPI0, 0x0, B1_DLL_ARPI0_RG_ARPI_RESETB_B1);
    vIO32WriteFldAlign_All(DDRPHY_REG_CA_CMD3, 0x0, CA_CMD3_RG_ARCMD_RESETB);
    vIO32WriteFldAlign_All(DDRPHY_REG_CA_DLL_ARPI0, 0x0, CA_DLL_ARPI0_RG_ARPI_RESETB_CA);
    vIO32WriteFldAlign_All(DDRPHY_REG_PHYPLL2, 0x0, PHYPLL2_RG_RPHYPLL_RESETB);//Since there is only 1 PLL, only control CHA //Follow Petrus_DDR_reserved C code_20190123.docx

    mcSHOW_DRM_MSG(("RG_*MCK8X_EN = 0   \n"));
#if 0
#if ENABLE_MCK8X_MODE
    //vIO32WriteFldAlign(DDRPHY_SHU_PLL22, 0, SHU_PLL22_RG_RPHYPLL_ADA_MCK8X_EN_SHU);
    //vIO32WriteFldAlign(DDRPHY_PLL4, 0, PLL4_RG_RPHYPLL_AD_MCK8X_EN);//RG_*MCK8X_EN=0; Since there is only 1 PLL, only to control CHA
#else
    vIO32WriteFldMulti(DDRPHY_REG_PHYPLL2, P_Fld(0, PHYPLL2_RG_RPHYPLL_ADA_MCK8X_EN)
                    | P_Fld(0, PHYPLL2_RG_RPHYPLL_AD_MCK8X_EN));//RG_*MCK8X_EN=0; Since there is only 1 PLL, only to control CHA
#endif
#endif
//!remove    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_DVFSCTL2, 0, MISC_DVFSCTL2_RG_ADA_MCK8X_EN_SHUFFLE);//Follow Petrus_DDR_reserved C code_20190123.docx
    vIO32WriteFldAlign_All(DDRPHY_REG_PHYPLL2, 0, PHYPLL2_RG_RPHYPLL_AD_MCK8X_EN);//RG_*MCK8X_EN=0; Since there is only 1 PLL, only to control CHA //Follow Petrus_DDR_reserved C code_20190123.docx
    vIO32WriteFldAlign_All(DDRPHY_REG_PHYPLL2, 0, PHYPLL2_RG_RPHYPLL_ADA_MCK8X_EN);//RG_*MCK8X_EN=0; Since there is only 1 PLL, only to control CHA //Follow Petrus_DDR_reserved C code_20190123.docx
    vIO32WriteFldAlign_All(DDRPHY_REG_SHU_PLL2, 0, SHU_PLL2_RG_RPHYPLL_ADA_MCK8X_EN_SHU);  //refer to MISC_DVFSCTRL2


    mcSHOW_DRM_MSG(("PLL_EN = 0\n"));
    vIO32WriteFldAlign_All(DDRPHY_REG_PHYPLL0, 0x0, PHYPLL0_RG_RPHYPLL_EN);//RG_*PLL_EN=0; Since there is only 1 PLL, only control CHA //Follow Petrus_DDR_reserved C code_20190123.docx
    vIO32WriteFldAlign_All(DDRPHY_REG_CLRPLL0, 0x0, CLRPLL0_RG_RCLRPLL_EN);//RG_*PLL_EN=0; Since there is only 1 PLL, only control CHA //Follow Petrus_DDR_reserved C code_20190123.docx
    //===========================================================================================================

    MPLLInit();

    //marked by kaihsin on May 7th, not necessary to PDN DDRPHY
    //*((UINT32P)(0x1000631c )) &= ~(0x1 << 2); //ddrphy_pwr_on=0
    //*((UINT32P)(0x100063b8 )) &= ~(0x1 << 2);
    mcSHOW_DRM_MSG(("DELAY 1 us\n"));
    mcDELAY_XUS(1);
    //marked by kaihsin on May 7th, not necessary to PDN DDRPHY
    //*((UINT32P)(0x1000631c )) |= (0x1 << 2); //ddrphy_pwr_on=1
    //*((UINT32P)(0x100063b8 )) |= (0x1 << 2);
    mcSHOW_DRM_MSG(("DELAY 1 us\n"));
    mcDELAY_XUS(1);

    //PLL open sequence
    //PLL_EN = 1
    //==> some of MIDPI*_EN = 1(async)
    //==> RG_ARPI_RESETB_* = 1(async, open first)
    //==> MCK8X_EN(source of clk gating) = 1
    //==> CG = 0
    //==> DLL_PHDET_EN_* = 1 PIC: Ying-Yu

    mcSHOW_DRM_MSG(("RG_*RESETB = 1\n"));
    vIO32WriteFldAlign_All(DDRPHY_REG_B0_DQ3, 0x1, B0_DQ3_RG_ARDQ_RESETB_B0);
    vIO32WriteFldAlign_All(DDRPHY_REG_B1_DQ3, 0x1, B1_DQ3_RG_ARDQ_RESETB_B1);
    vIO32WriteFldAlign_All(DDRPHY_REG_CA_CMD3, 0x1, CA_CMD3_RG_ARCMD_RESETB);
    vIO32WriteFldAlign_All(DDRPHY_REG_PHYPLL2, 0x1, PHYPLL2_RG_RPHYPLL_RESETB);//Since there is only 1 PLL, only control CHA //Follow Petrus_DDR_reserved C code_20190123.docx

    //*** WARNING***SA please change below write cmd to make sure write 2 MD32(Pe_trus)
    //*** WARNING***SA please change below write cmd to make sure write 1 MD32(Mar_gaux)
    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_2, 0x0, LPIF_LOW_POWER_CFG_2_MPLL_S_OFF);

    //*** WARNING***SA please change below write cmd to make sure write 2 MD32(Pe_trus)
    //*** WARNING***SA please change below write cmd to make sure write 1 MD32(Mar_gaux)
    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_MISC_CTL_1, 0x0, LPIF_MISC_CTL_1_MPLL_OFF);


    mcSHOW_DRM_MSG(("DELAY 20 us\n"));
    mcDELAY_XUS(20);

    mcSHOW_DRM_MSG(("Change DVFS to SPM mode\n"));
    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_RG_DFS_CTRL, 1, MISC_RG_DFS_CTRL_SPM_DVFS_CONTROL_SEL);//change DVFS to RG mode
    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_RG_DFS_CTRL, 1, MISC_RG_DFS_CTRL_RG_PHYPLL_SHU_EN);//toggle shu_en to sync shu_level, need MISC_SPM_CTRL1_SPM_DVFS_CONTROL_SEL = 1
    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_RG_DFS_CTRL, 1, MISC_RG_DFS_CTRL_RG_PHYPLL2_SHU_EN);//toggle shu_en to sync shu_level
    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_RG_DFS_CTRL, 0, MISC_RG_DFS_CTRL_RG_PHYPLL_SHU_EN);//toggle shu_en to sync shu_level
    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_RG_DFS_CTRL, 0, MISC_RG_DFS_CTRL_RG_PHYPLL2_SHU_EN);//toggle shu_en to sync shu_level
    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_RG_DFS_CTRL, 0, MISC_RG_DFS_CTRL_SPM_DVFS_CONTROL_SEL);//change DVFS to SPM mode

    mcSHOW_DRM_MSG(("MPDIV_CG = 0 FB_CG = 0 \n"));
    vIO32WriteFldMulti_All(DDRPHY_REG_SHU_B0_DLL_ARPI2, P_Fld(0x0, SHU_B0_DLL_ARPI2_RG_ARPI_CG_FB_B0)
                | P_Fld(0x0, SHU_B0_DLL_ARPI2_RG_ARPI_MPDIV_CG_B0));
    vIO32WriteFldMulti_All(DDRPHY_REG_SHU_B1_DLL_ARPI2, P_Fld(0x0, SHU_B1_DLL_ARPI2_RG_ARPI_CG_FB_B1)
                | P_Fld(0x0, SHU_B1_DLL_ARPI2_RG_ARPI_MPDIV_CG_B1));
    vIO32WriteFldMulti_All(DDRPHY_REG_SHU_CA_DLL_ARPI2, P_Fld(0x0, SHU_CA_DLL_ARPI2_RG_ARPI_CG_FB_CA)
                | P_Fld(0x0, SHU_CA_DLL_ARPI2_RG_ARPI_MPDIV_CG_CA));

    mcSHOW_DRM_MSG(("RG_*PLL_EN = 1\n"));
    vIO32WriteFldAlign_All(DDRPHY_REG_PHYPLL0, 0x1, PHYPLL0_RG_RPHYPLL_EN);//RG_*PLL_EN=1; Since there is only 1 PLL, only control CHA //Follow Petrus_DDR_reserved C code_20190123.docx
    vIO32WriteFldAlign_All(DDRPHY_REG_CLRPLL0, 0x1, CLRPLL0_RG_RCLRPLL_EN);//RG_*PLL_EN=1; Since there is only 1 PLL, only control CHA //Follow Petrus_DDR_reserved C code_20190123.docx
//    vIO32WriteFldAlign_All(DDRPHY_REG_SHU_CA_DLL_ARPI2,0x0, SHU_CA_DLL_ARPI2_RG_ARPI_CG_MCTL_CA);//IPM V2.1 only

    mcSHOW_DRM_MSG(("DELAY 20 us\n"));
    mcDELAY_XUS(20);//Wait 20us for MEMPLL

    mcSHOW_DRM_MSG(("ASVA 2-1 Multi-phase Ready : enable clk CG before DLL enable\n"));
    vIO32WriteFldMulti_All(DDRPHY_REG_MISC_CTRL3         ,
                  P_Fld(0x0, MISC_CTRL3_ARPI_MPDIV_CG_DQ_OPT)
                | P_Fld(0x0, MISC_CTRL3_ARPI_MPDIV_CG_CA_OPT));

    mcSHOW_DRM_MSG(("MPDIV_CG = 1 FB_CG = 1 \n"));
    vIO32WriteFldMulti_All(DDRPHY_REG_SHU_B0_DLL_ARPI2, P_Fld(0x1, SHU_B0_DLL_ARPI2_RG_ARPI_CG_FB_B0)
                | P_Fld(0x1, SHU_B0_DLL_ARPI2_RG_ARPI_MPDIV_CG_B0));
    vIO32WriteFldMulti_All(DDRPHY_REG_SHU_B1_DLL_ARPI2, P_Fld(0x1, SHU_B1_DLL_ARPI2_RG_ARPI_CG_FB_B1)
                | P_Fld(0x1, SHU_B1_DLL_ARPI2_RG_ARPI_MPDIV_CG_B1));
    vIO32WriteFldMulti_All(DDRPHY_REG_SHU_CA_DLL_ARPI2, P_Fld(0x1, SHU_CA_DLL_ARPI2_RG_ARPI_CG_FB_CA)
                | P_Fld(0x1, SHU_CA_DLL_ARPI2_RG_ARPI_MPDIV_CG_CA));

    mcSHOW_DRM_MSG(("RG_*VREF_EN = 1 (Vref is only used in read, DQ(B0, B1)is use for RX) \n"));
    vIO32WriteFldAlign_All(DDRPHY_REG_B0_DQ5, 0x1, B0_DQ5_RG_RX_ARDQ_VREF_EN_B0);
    vIO32WriteFldAlign_All(DDRPHY_REG_B1_DQ5, 0x1, B1_DQ5_RG_RX_ARDQ_VREF_EN_B1);

    mcSHOW_DRM_MSG(("DELAY 1 us\n"));
    mcDELAY_XUS(1);

    mcSHOW_DRM_MSG(("MIDPI*_EN = 1\n"));
//    vIO32Write4B_All(DDRPHY_REG_SHU_CA_CMD1, MIDPI_CA);
    temp = MIDPI_CA & 1;
	vIO32WriteFldAlign(DDRPHY_REG_SHU_CA_CMD1, temp, SHU_CA_CMD1_RG_ARPI_MIDPI_EN_CA);
    temp = (MIDPI_CA & 4)>>2;
    vIO32WriteFldAlign(DDRPHY_REG_SHU_CA_CMD1, temp, SHU_CA_CMD1_RG_ARPI_MIDPI_CKDIV4_EN_CA);
    mcSHOW_DRM_MSG(("DELAY 1 us\n"));
    mcDELAY_XUS(10);

//    vIO32Write4B_All(DDRPHY_REG_SHU_B0_DQ1,  MIDPI_B0);
////    vIO32Write4B_All(DDRPHY_REG_SHU_B2_DQ1,  MIDPI_B2);
//    vIO32Write4B_All(DDRPHY_REG_SHU_B1_DQ1,  MIDPI_B1);
    temp = MIDPI_CA & 1;
	vIO32WriteFldAlign_All(DDRPHY_REG_SHU_CA_CMD1, temp, SHU_CA_CMD1_RG_ARPI_MIDPI_EN_CA);
    temp = (MIDPI_CA & 4)>>2;
    vIO32WriteFldAlign_All(DDRPHY_REG_SHU_CA_CMD1, temp, SHU_CA_CMD1_RG_ARPI_MIDPI_CKDIV4_EN_CA);
    temp = MIDPI_B1 & 1;
    vIO32WriteFldAlign_All(DDRPHY_REG_SHU_B0_DQ1, temp, SHU_B0_DQ1_RG_ARPI_MIDPI_EN_B0);//874
    vIO32WriteFldAlign_All(DDRPHY_REG_SHU_B1_DQ1, temp, SHU_B1_DQ1_RG_ARPI_MIDPI_EN_B1);//9F4
    temp = (MIDPI_B1 & 4)>>2;
    vIO32WriteFldAlign_All(DDRPHY_REG_SHU_B0_DQ1, temp, SHU_B0_DQ1_RG_ARPI_MIDPI_CKDIV4_EN_B0);//874
    vIO32WriteFldAlign_All(DDRPHY_REG_SHU_B1_DQ1, temp, SHU_B1_DQ1_RG_ARPI_MIDPI_CKDIV4_EN_B1);//9F4
//    vIO32WriteFldAlign_All(DDRPHY_REG_SHU_B2_DQ1, 0x0, SHU_B2_DQ1_RG_ARPI_MIDPI_EN_B2);
//    vIO32WriteFldAlign_All(DDRPHY_REG_SHU_B2_DQ1, 0x0, SHU_B2_DQ1_RG_ARPI_MIDPI_CKDIV4_EN_B2);

    mcSHOW_DRM_MSG(("DELAY 10 us\n"));
    mcDELAY_XUS(10);

    vIO32WriteFldAlign_All(DDRPHY_REG_B0_SHU_MIDPI_CTRL, 0x1, B0_SHU_MIDPI_CTRL_RG_RX_ARDQS_DQSIEN_GLITCH_FREE_EN_B0);
    vIO32WriteFldAlign_All(DDRPHY_REG_B1_SHU_MIDPI_CTRL, 0x1, B1_SHU_MIDPI_CTRL_RG_RX_ARDQS_DQSIEN_GLITCH_FREE_EN_B1);

    mcSHOW_DRM_MSG(("RG_ARPI_RESETB_* = 1\n"));
    vIO32WriteFldAlign_All(DDRPHY_REG_B0_DLL_ARPI0, 0x1, B0_DLL_ARPI0_RG_ARPI_RESETB_B0);
    vIO32WriteFldAlign_All(DDRPHY_REG_B1_DLL_ARPI0, 0x1, B1_DLL_ARPI0_RG_ARPI_RESETB_B1);
    vIO32WriteFldAlign_All(DDRPHY_REG_CA_DLL_ARPI0, 0x1, CA_DLL_ARPI0_RG_ARPI_RESETB_CA);

    mcSHOW_DRM_MSG(("DELAY 1 us\n"));
    mcDELAY_XUS(1);

    mcSHOW_DRM_MSG(("RG_*MCK8X_EN = 1   \n"));
#if 0
#if ENABLE_MCK8X_MODE
    //O32WriteFldAlign(DDRPHY_SHU_PLL22, 1, SHU_PLL22_RG_RPHYPLL_ADA_MCK8X_EN_SHU);
    //O32WriteFldAlign(DDRPHY_PLL4, 1, PLL4_RG_RPHYPLL_AD_MCK8X_EN);//RG_*MCK8X_EN=1; Since there is only 1 PLL, only to control CHA
#else
    vIO32WriteFldMulti(DDRPHY_REG_PHYPLL2, P_Fld(1, PHYPLL2_RG_RPHYPLL_ADA_MCK8X_EN)
                    | P_Fld(1, PHYPLL2_RG_RPHYPLL_AD_MCK8X_EN));//RG_*MCK8X_EN=1; Since there is only 1 PLL, only to control CHA
#endif
#endif
//!remove    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_DVFSCTL2, 1, MISC_DVFSCTL2_RG_ADA_MCK8X_EN_SHUFFLE); //Follow Petrus_DDR_reserved C code_20190123.docx
    vIO32WriteFldAlign_All(DDRPHY_REG_PHYPLL2, 1, PHYPLL2_RG_RPHYPLL_AD_MCK8X_EN);//RG_*MCK8X_EN=0; Since there is only 1 PLL, only to control CHA //Follow Petrus_DDR_reserved C code_20190123.docx
    vIO32WriteFldAlign_All(DDRPHY_REG_PHYPLL2, 1, PHYPLL2_RG_RPHYPLL_ADA_MCK8X_EN);//RG_*MCK8X_EN=0; Since there is only 1 PLL, only to control CHA //Follow Petrus_DDR_reserved C code_20190123.docx
    vIO32WriteFldAlign_All(DDRPHY_REG_SHU_PLL2, 1, SHU_PLL2_RG_RPHYPLL_ADA_MCK8X_EN_SHU);  //refer to MISC_DVFSCTRL2

    mcSHOW_DRM_MSG(("DELAY 1 us\n"));
    mcDELAY_XUS(1);

    mcSHOW_DRM_MSG(("*PI_CG = 0, RG_*MPDIV_CG = 0\n"));
    vIO32WriteFldMulti_All(DDRPHY_REG_SHU_B0_DLL_ARPI2,
                  P_Fld(0x0, SHU_B0_DLL_ARPI2_RG_ARPI_CG_MCK_B0)
                | P_Fld(0x0, SHU_B0_DLL_ARPI2_RG_ARPI_CG_MCK_FB2DLL_B0)
                | P_Fld(0x0, SHU_B0_DLL_ARPI2_RG_ARPI_CG_MCTL_B0)
                | P_Fld(0x0, SHU_B0_DLL_ARPI2_RG_ARPI_CG_FB_B0)
                | P_Fld(0x0, SHU_B0_DLL_ARPI2_RG_ARPI_CG_DQS_B0)
                | P_Fld(0x0, SHU_B0_DLL_ARPI2_RG_ARPI_CG_DQM_B0)
                | P_Fld(0x0, SHU_B0_DLL_ARPI2_RG_ARPI_CG_DQ_B0)
                | P_Fld(0x0, SHU_B0_DLL_ARPI2_RG_ARPI_CG_DQSIEN_B0)
                | P_Fld(0x0, SHU_B0_DLL_ARPI2_RG_ARPI_MPDIV_CG_B0));
    vIO32WriteFldMulti_All(DDRPHY_REG_SHU_B1_DLL_ARPI2,
                  P_Fld(0x0, SHU_B1_DLL_ARPI2_RG_ARPI_CG_MCK_B1)
                | P_Fld(0x0, SHU_B1_DLL_ARPI2_RG_ARPI_CG_MCK_FB2DLL_B1)
                | P_Fld(0x0, SHU_B1_DLL_ARPI2_RG_ARPI_CG_MCTL_B1)
                | P_Fld(0x0, SHU_B1_DLL_ARPI2_RG_ARPI_CG_FB_B1)
                | P_Fld(0x0, SHU_B1_DLL_ARPI2_RG_ARPI_CG_DQS_B1)
                | P_Fld(0x0, SHU_B1_DLL_ARPI2_RG_ARPI_CG_DQM_B1)
                | P_Fld(0x0, SHU_B1_DLL_ARPI2_RG_ARPI_CG_DQ_B1)
                | P_Fld(0x0, SHU_B1_DLL_ARPI2_RG_ARPI_CG_DQSIEN_B1)
                | P_Fld(0x0, SHU_B1_DLL_ARPI2_RG_ARPI_MPDIV_CG_B1));
    vIO32WriteFldMulti_All(DDRPHY_REG_SHU_CA_DLL_ARPI2,
                  P_Fld(0x0, SHU_CA_DLL_ARPI2_RG_ARPI_CG_MCK_CA)
                | P_Fld(0x0, SHU_CA_DLL_ARPI2_RG_ARPI_CG_MCK_FB2DLL_CA)
                | P_Fld(0x0, SHU_CA_DLL_ARPI2_RG_ARPI_CG_MCTL_CA)
                | P_Fld(0x0, SHU_CA_DLL_ARPI2_RG_ARPI_CG_FB_CA)
                | P_Fld(0x0, SHU_CA_DLL_ARPI2_RG_ARPI_CG_CS)
                | P_Fld(0x0, SHU_CA_DLL_ARPI2_RG_ARPI_CG_CLK)
                | P_Fld(0x0, SHU_CA_DLL_ARPI2_RG_ARPI_CG_CMD)
                | P_Fld(0x0, SHU_CA_DLL_ARPI2_RG_ARPI_CG_CLKIEN)
                | P_Fld(0x0, SHU_CA_DLL_ARPI2_RG_ARPI_MPDIV_CG_CA));

    mcSHOW_DRM_MSG(("RG_*BIAS_EN = 1\n"));
    vIO32WriteFldAlign_All(DDRPHY_REG_B0_DQ6, 0x1, B0_DQ6_RG_RX_ARDQ_BIAS_EN_B0);
    vIO32WriteFldAlign_All(DDRPHY_REG_B1_DQ6, 0x1, B1_DQ6_RG_RX_ARDQ_BIAS_EN_B1);
    vIO32WriteFldAlign_All(DDRPHY_REG_CA_CMD6, 0x1, CA_CMD6_RG_RX_ARCMD_BIAS_EN);

    mcSHOW_DRM_MSG(("DELAY 1 us\n"));
    mcDELAY_XUS(1);

//    vIO32WriteFldMulti_All(DDRPHY_REG_MISC_CLK_CTRL , P_Fld(1, MISC_CLK_CTRL_DVFS_CLK_MEM_SEL) );//[1]: 0:SW 1:HW

//    vIO32WriteFldMulti_All(DDRPHY_REG_MISC_CG_CTRL0, P_Fld(0, MISC_CG_CTRL0_W_CHG_MEM)
//                | P_Fld(1, MISC_CG_CTRL0_CLK_MEM_SEL));//[5:4] mem_ck mux: 2'b01: memory clock, [0]: change memory clock

//    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_CG_CTRL0, 1, MISC_CG_CTRL0_W_CHG_MEM);//change clock freq

//    mcSHOW_DRM_MSG(("DELAY 1 us\n"));
//    mcDELAY_XUS(1);

//    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_CG_CTRL0, 0, MISC_CG_CTRL0_W_CHG_MEM);//disable memory clock change
    LP_Mode_DRM_Setting();
    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_2, 0x0,LPIF_LOW_POWER_CFG_2_DPY_MODE_SW);         //0x4700C[5:4]
    mcDELAY_XUS(1);

//    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0, 0x0,LPIF_LOW_POWER_CFG_0_EMI_CLK_OFF_REQ);     //0x47004[13:12]
//    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0, 0x0,LPIF_LOW_POWER_CFG_0_MEM_CK_OFF);          //0x47004[15:14]

//    mcDELAY_XUS(1);
    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0, 0x0,LPIF_LOW_POWER_CFG_0_DMSUS_OFF);           //0x47004[1:0]
    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_1, 0x0,LPIF_LOW_POWER_CFG_1_TX_TRACKING_DIS);     //0x47008[19:18]
    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_2, 0x0,LPIF_LOW_POWER_CFG_2_EMI_SLEEP_PROT_EN);   //0x4700C[6]
    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_4, 0x0,LPIF_LOW_POWER_CFG_4_DPY_BLOCK_CTO_ALE);   //0x4703C[23:22]
    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_4, 0x0,LPIF_LOW_POWER_CFG_4_DPY_ALL_TRACKING_OFF);//0x4703C[13:12]


    mcSHOW_DRM_MSG(("force top feedback MCK not divide\n"));
    //vIO32WriteFldAlign(SPM_DRAMC_DPY_CLK_SW_CON, 3, DRAMC_DPY_CLK_SW_CON_SW_DDRPHY_FB_CK_EN);
    //vIO32WriteFldAlign(SPM_SPM_POWER_ON_VAL0, 1, SPM_POWER_ON_VAL0_SC_DDRPHY_FB_CK_EN);

    //*** WARNING***SA please change below write cmd to make sure write 2 MD32(Pe_trus)
    //*** WARNING***SA please change below write cmd to make sure write 1 MD32(Mar_gaux)
    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0, CHAN_MODE, LPIF_LOW_POWER_CFG_0_DDRPHY_FB_CK_EN);//==TODO

    //DLL
#if 0
#if DLL_ASYNC_MODE
#if ENABLE_MCK8X_MODE
    //vIO32WriteFldAlign_All(DDRPHY_SHU_CA_DLL0, 0x1, SHU_CA_DLL0_RG_ARDLL_PHDET_EN_CA_SHU);//Since CHA CA and CHB CA are DLL master
    //mcDELAY_XUS(1);
    //vIO32WriteFldAlign_All(DDRPHY_SHU_B0_DLL0, 0x1, SHU_B0_DLL0_RG_ARDLL_PHDET_EN_B0_SHU);//2nd DLL enable
    //vIO32WriteFldAlign_All(DDRPHY_SHU_B1_DLL0, 0x1, SHU_B1_DLL0_RG_ARDLL_PHDET_EN_B1_SHU);//2nd DLL enable
    //mcDELAY_XUS(1);
#else
    vIO32WriteFldAlign_All(DDRPHY_CA_DLL_ARPI2, 0x1, CA_DLL_ARPI2_RG_ARDLL_PHDET_EN_CA);//Since CHA CA and CHB CA are DLL master
    mcDELAY_XUS(1);
    vIO32WriteFldAlign_All(DDRPHY_B0_DLL_ARPI2, 0x1, B0_DLL_ARPI2_RG_ARDLL_PHDET_EN_B0);//2nd DLL enable
    vIO32WriteFldAlign_All(DDRPHY_B1_DLL_ARPI2, 0x1, B1_DLL_ARPI2_RG_ARDLL_PHDET_EN_B1);//2nd DLL enable
    mcDELAY_XUS(1);
#endif
#else //DLL_ASYNC_MODE
#if ENABLE_MCK8X_MODE
    //vIO32WriteFldAlign(DDRPHY_SHU_CA_DLL0, 0x1, SHU_CA_DLL0_RG_ARDLL_PHDET_EN_CA_SHU);//Since CHA CA is DLL master
    //mcDELAY_XUS(1);
    //vIO32WriteFldAlign(DDRPHY_SHU_CA_DLL0 + SHIFT_TO_CHB_ADDR, 0x1, SHU_CA_DLL0_RG_ARDLL_PHDET_EN_CA_SHU);//2nd DLL enable
    //vIO32WriteFldAlign_All(DDRPHY_SHU_B0_DLL0, 0x1, SHU_B0_DLL0_RG_ARDLL_PHDET_EN_B0_SHU);//2nd DLL enable
    //vIO32WriteFldAlign_All(DDRPHY_SHU_B1_DLL0, 0x1, SHU_B1_DLL0_RG_ARDLL_PHDET_EN_B1_SHU);//2nd DLL enable
    //mcDELAY_XUS(1);
#else
    vIO32WriteFldAlign(DDRPHY_CA_DLL_ARPI2, 0x1, CA_DLL_ARPI2_RG_ARDLL_PHDET_EN_CA);//Since CHA CA is DLL master
    mcDELAY_XUS(1);
    vIO32WriteFldAlign(DDRPHY_CA_DLL_ARPI2 + SHIFT_TO_CHB_ADDR, 0x1, CA_DLL_ARPI2_RG_ARDLL_PHDET_EN_CA);//2nd DLL enable
    vIO32WriteFldAlign_All(DDRPHY_B0_DLL_ARPI2, 0x1, B0_DLL_ARPI2_RG_ARDLL_PHDET_EN_B0);//2nd DLL enable
    vIO32WriteFldAlign_All(DDRPHY_B1_DLL_ARPI2, 0x1, B1_DLL_ARPI2_RG_ARDLL_PHDET_EN_B1);//2nd DLL enable
    mcDELAY_XUS(1);
#endif
#endif
#endif

    //
    mcSHOW_DRM_MSG(("RG_*PHDET_EN = 1 (DLL on)\n"));
    vIO32WriteFldAlign(DDRPHY_REG_SHU_CA_DLL1, 0x1, SHU_CA_DLL1_RG_ARDLL_PHDET_EN_CA);
#if (CHANNEL_NUM > 2)
    vIO32WriteFldAlign(DDRPHY_REG_SHU_CA_DLL1 + SHIFT_TO_CHC_ADDR, 0x1, SHU_CA_DLL1_RG_ARDLL_PHDET_EN_CA);
#endif

    mcSHOW_DRM_MSG(("DELAY 1 us\n"));
    mcDELAY_XUS(1);

    vIO32WriteFldAlign(DDRPHY_REG_SHU_CA_DLL1 + SHIFT_TO_CHB_ADDR, 0x1, SHU_CA_DLL1_RG_ARDLL_PHDET_EN_CA);
#if (CHANNEL_NUM > 2)
    vIO32WriteFldAlign(DDRPHY_REG_SHU_CA_DLL1 + SHIFT_TO_CHD_ADDR, 0x1, SHU_CA_DLL1_RG_ARDLL_PHDET_EN_CA);
#endif

    mcSHOW_DRM_MSG(("DELAY 1 us\n"));
    mcDELAY_XUS(1);

    vIO32WriteFldAlign_All(DDRPHY_REG_SHU_B0_DLL1, 0x1, SHU_B0_DLL1_RG_ARDLL_PHDET_EN_B0);
    vIO32WriteFldAlign_All(DDRPHY_REG_SHU_B1_DLL1, 0x1, SHU_B1_DLL1_RG_ARDLL_PHDET_EN_B1);

    mcSHOW_DRM_MSG(("DELAY 1 us\n"));
    mcDELAY_XUS(1);

    //
   //
    mcSHOW_DRM_MSG(("Free run clock for PHY/RDATA_CNT RESET after self refresh\n"));
    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_CG_CTRL0, 0x1, MISC_CG_CTRL0_RG_CG_PHY_OFF_DIABLE);
    //
    mcSHOW_DRM_MSG(("Top feedback MCK to divided frequency\n"));
    //vIO32WriteFldAlign(SPM_DRAMC_DPY_CLK_SW_CON, 0, DRAMC_DPY_CLK_SW_CON_SW_DDRPHY_FB_CK_EN);
    //vIO32WriteFldAlign(SPM_SPM_POWER_ON_VAL0, 0, SPM_POWER_ON_VAL0_SC_DDRPHY_FB_CK_EN);

    //*** WARNING***SA please change below write cmd to make sure write 2 MD32(Pe_trus)
    //*** WARNING***SA please change below write cmd to make sure write 1 MD32(Mar_gaux)
    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0, 0, LPIF_LOW_POWER_CFG_0_DDRPHY_FB_CK_EN);

    vIO32WriteFldAlign_All(DRAMC_REG_RX_SET0, 1, RX_SET0_RDATRST);//R_DMRDATRST = 1
    vIO32WriteFldAlign_All(DRAMC_REG_RX_SET0, 0, RX_SET0_RDATRST);//R_DMRDATRST = 0

    mcSHOW_DRM_MSG(("Switching PHYPLL enable path from DDRPHY to SPM control by setting SPM SW\n"));
    //vIO32WriteFldAlign(SPM_SPM_POWER_ON_VAL0, 1, SPM_POWER_ON_VAL0_SC_PHYPLL_MODE_SW);
    //vIO32WriteFldAlign(SPM_DRAMC_DPY_CLK_SW_CON2, 1, DRAMC_DPY_CLK_SW_CON2_SW_PHYPLL_MODE_SW);

    //*** WARNING***SA please change below write cmd to make sure write 2 MD32(Pe_trus)
    //*** WARNING***SA please change below write cmd to make sure write 1 MD32(Mar_gaux)
    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0, CHAN_MODE, LPIF_LOW_POWER_CFG_0_PHYPLL_MODE_SW);//==TODO


    mcSHOW_DRM_MSG(("DELAY 1 us\n"));
    mcDELAY_XUS(1);

//     mcSHOW_DRM_MSG(("RG_*PLL_EN=1\n"));
    vIO32WriteFldAlign_All(DDRPHY_REG_PHYPLL0, 0x0, PHYPLL0_RG_RPHYPLL_EN);//disable DDRPHY PHYPLL enable, RG_*PLL_EN=0; Since there is only 1 PLL, only control CHA //Follow Petrus_DDR_reserved C code_20190123.docx
    vIO32WriteFldAlign_All(DDRPHY_REG_CLRPLL0, 0x0, CLRPLL0_RG_RCLRPLL_EN);//disable DDRPHY CLRPLL enable, RG_*PLL_EN=0; Since there is only 1 PLL, only control CHA //Follow Petrus_DDR_reserved C code_20190123.docx

//    vIO32WriteFldMulti_All(DDRPHY_REG_MISC_CLK_CTRL , P_Fld(1, MISC_CLK_CTRL_DVFS_CLK_MEM_SEL) );//[1]: 0:SW 1:HW

//    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_CG_CTRL0, 1, MISC_CG_CTRL0_W_CHG_MEM);//change clock freq

//    mcSHOW_DRM_MSG(("DELAY 1 us\n"));
//    mcDELAY_XUS(1);

//    mcSHOW_DRM_MSG(("disable memory clock change\n"));
//    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_CG_CTRL0, 0, MISC_CG_CTRL0_W_CHG_MEM);//
//    mcSHOW_DRM_MSG(("Disable CKE FIX ON\n"));
//    vIO32WriteFldAlign_All(DRAMC_REG_CKECTRL, 0,  CKECTRL_CKEFIXON);
//    vIO32WriteFldAlign_All(DRAMC_REG_CKECTRL, 0,  CKECTRL_CKE1FIXON);

#if ENABLE_TX_REBASE_ODT_WA
    TxReadBaseODTWA(p, SRAM_SHU0);//for low power enter DRM fail
#endif

//    ASVA5_8_CSCA_Pull_Down_EN();
    DramcBroadcastOnOff(backup_broadcast);
    //DDRPhyFreqMeter();
    mcSHOW_DRM_MSG(("End of Dramc_DDR_Reserved_Mode_setting \n"));

}

    // *** WARNING*** SA please change below  *** WARNING***
    // *** WARNING*** SA please change below  *** WARNING***
    // *** WARNING*** SA please change below  *** WARNING***

void Dramc_DDR_Reserved_Mode_BeforeSR(void)
{
    DRAMC_CTX_T * p = &DramCtx_LPDDR4; //done in ddr reserve mode setting
    mcSHOW_DRM_MSG(("Start Dramc_DDR_Reserved_Mode_BeforeSR, dram type =%x \n", p->dram_type));
    vIO32WriteFldMulti_All(DRAMC_REG_DQSOSCR, P_Fld(0x1,DQSOSCR_TXUPDMODE));
    mcDELAY_XUS(1);
    vIO32WriteFldMulti_All(DRAMC_REG_DQSOSCR, P_Fld(0x0,DQSOSCR_TXUPDMODE));

#if ENABLE_RX_TRACKING
    mcSHOW_DRM_MSG(("Enable RX tracking\n"));
    mcSHOW_DRM_MSG(("Enable RX tracking by conf RG, fix 1:4 mode enter DDR reserve mode fail. PIC: Lynx\n"));
    vIO32WriteFldAlign_All(DDRPHY_REG_B0_RXDVS0, 0x1, B0_RXDVS0_R_RX_DLY_TRACK_ENA_B0);//0x04E4[31]
    vIO32WriteFldAlign_All(DDRPHY_REG_B1_RXDVS0, 0x1, B1_RXDVS0_R_RX_DLY_TRACK_ENA_B1);//0x0564[31]

    mcSHOW_DRM_MSG(("Enable rx tracking FSM - start\n"));
    vIO32WriteFldAlign_All(DDRPHY_REG_B0_RXDVS0, 0, B0_RXDVS0_R_RX_DLY_TRACK_CLR_B0);//0x04E4[30]
    vIO32WriteFldAlign_All(DDRPHY_REG_B1_RXDVS0, 0, B1_RXDVS0_R_RX_DLY_TRACK_CLR_B1);//0x0564[30]
    mcSHOW_DRM_MSG(("Enable rx tracking FSM - end\n"));

    vIO32WriteFldAlign_All(DDRPHY_REG_RK_B0_RXDVS2, 0x2, RK_B0_RXDVS2_R_RK0_DVS_MODE_B0);//0x00E8[31:30]
    vIO32WriteFldAlign_All(DDRPHY_REG_RK_B1_RXDVS2, 0x2, RK_B1_RXDVS2_R_RK0_DVS_MODE_B1);//0x0168[31:30]
    vIO32WriteFldAlign_All(DDRPHY_AO_RANK_OFFSET + DDRPHY_REG_RK_B0_RXDVS2, 0x2, RK_B0_RXDVS2_R_RK0_DVS_MODE_B0);//0x02E8[31:30]
    vIO32WriteFldAlign_All(DDRPHY_AO_RANK_OFFSET + DDRPHY_REG_RK_B1_RXDVS2, 0x2, RK_B1_RXDVS2_R_RK0_DVS_MODE_B1);//0x0368[31:30]

    //*** WARNING***SA please change below write cmd to make sure write 2 MD32(Pe_trus)
    //*** WARNING***SA please change below write cmd to make sure write 1 MD32(Mar_gaux)
    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_1, CHAN_MODE, LPIF_LOW_POWER_CFG_1_DPHY_RXDLY_TRACK_EN);//0x47008[11:10]
#endif
    mcSHOW_DRM_MSG(("End Dramc_DDR_Reserved_Mode_BeforeSR, dram type =%x \n", p->dram_type));
}

static U32 TA2_Test_After_DRM(DRAMC_CTX_T * p)//Not change test range of DRM
{
    DRAM_CHANNEL_T channel_bak = p->channel;
    DRAM_RANK_T rank_bak = p->rank;
    U32 bit_error = 0;

    TA2_Test_Run_Time_HW_Write(p, ENABLE);
    bit_error = TA2_Test_Run_Time_HW_Status(p);

    if(bit_error)
    dump_gating_error_rg(p);

    p->channel = channel_bak;
    p->rank = rank_bak;
    return bit_error;
}

#define USE_TA2_IN_DDR_RESERVE_MODE //Just for debug
#define USE_TA2_TO_DECIDE_DRM_RESULT 1
extern u32 g_ddr_reserve_success; //In platform.c
void Dramc_DDR_Reserved_Mode_AfterSR(void)
{
    U32 u4value = 0;
    U32 u4WbrBackup = 0;
    DRAMC_CTX_T * p = &DramCtx_LPDDR4; //done in ddr reserve mode setting
    U32 u4LP3_MR2RLWL = 0;
    U32 u4LP4_MR13FSP = 0;
    U32 u4LP4_MR2RLWL = 0;
    U32 backup_broadcast = GetDramcBroadcast();
    U8 u1channel = CHANNEL_A;
    DRAM_CHANNEL_T eOriChannel = vGetPHY2ChannelMapping(p);
    BOOL bSupport2Rank = (u4IO32ReadFldAlign(DRAMC_REG_SA_RESERVE, SA_RESERVE_SINGLE_RANK) == 0) ? TRUE : FALSE; 
    //BOOL bSupport2Rank = TRUE;
    U8 u1set_mrsrk = (bSupport2Rank == TRUE) ? 0x3 : RANK_0; /* MRS two ranks simutaniously */
    U8 u1ShuLevel;
    U8 u1OPEN;
    U8 u1SOPEN;


//    if(p->support_channel_num !=2) mcSHOW_DRM_MSG(("Set DRAMC Dual Channel\n"));//FOR_DV_SIMULATION_USE, never delet
//    p->support_channel_num = 2;//FOR_DV_SIMULATION_USE, never delet
    mcSHOW_DRM_MSG(("Start Dramc_DDR_Reserved_Mode_AfterSR, dram type =%x \n", p->dram_type));



#if (FOR_DV_SIMULATION_USED == 1)
  if(DUT_p.DRMODE==1)// 1: new mode, 0: old mode
  {
    ASVA5_8_IPMV2();
    if (is_lp5_family(p)) Check_MD32_LP5_DSM();
    else vIO32WriteFldAlign_All(DDRPHY_REG_MISC_DDR_RESERVE2, 0x0, MISC_DDR_RESERVE2_DRM_SM_HOLD_CTRL); //0x6f4[31:16] = 0x200
    wait_SREFx();
  }
#else
  #if (DDR_RESERVE_NEW_MODE==1)
    ASVA5_8_IPMV2();
    if (is_lp5_family(p)) Check_MD32_LP5_DSM();
    else vIO32WriteFldAlign_All(DDRPHY_REG_MISC_DDR_RESERVE2, 0x0, MISC_DDR_RESERVE2_DRM_SM_HOLD_CTRL); //0x6f4[31:16] = 0x200
    wait_SREFx();
  #else
  #endif
#endif

    DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);

#if 0
    vIO32WriteFldAlign_All(DRAMC_REG_DRAMC_PD_CTRL, 1, DRAMC_PD_CTRL_PHYCLKDYNGEN);//enable DDRPHY dynamic clock gating

    mcSHOW_DRM_MSG(("Disable BLCOK_ALE + EN FAKEUP2 + hold\n"));

    UINT32 bkDRAMCTRL_ALEBLOCK;
    bkDRAMCTRL_ALEBLOCK=u4IO32ReadFldAlign(DRAMC_REG_DRAMCTRL,DRAMCTRL_ALEBLOCK);
    vIO32WriteFldAlign_All(DRAMC_REG_DRAMCTRL, 0,  DRAMCTRL_ALEBLOCK);
    int i=0;
    for(i = 0; i < 10; i++)
    {
        TA2_Test_After_DRM(p);
    }
    vIO32WriteFldAlign_All(DRAMC_REG_DRAMCTRL, bkDRAMCTRL_ALEBLOCK, DRAMCTRL_ALEBLOCK);
#endif

#if ENABLE_TX_TRACKING
    mcSHOW_DRM_MSG(("Enable TX tracking\n"));
    //"*** WARNING***SA please change below write cmd to make sure write 2 MD32(Pe_trus)
    //*** WARNING***SA please change below write cmd to make sure write 1 MD32(Mar_gaux)
    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_1, 0, LPIF_LOW_POWER_CFG_1_TX_TRACKING_DIS);

    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_RK_DQSOSC), 1, RK_DQSOSC_DQSOSCR_RK0EN);
    if (bSupport2Rank == TRUE)
    {
        vSetRank(p, RANK_1);
        vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_RK_DQSOSC), 1, RK_DQSOSC_DQSOSCR_RK0EN);
    }
    vSetRank(p, RANK_0);
#endif

#if !EXIT_DRM_WITH_SAME_FREQ
    u4LP4_MR13FSP = u4IO32ReadFldAlign(DRAMC_REG_SHU_HWSET_VRCG, SHU_HWSET_VRCG_HWSET_VRCG_OP);//Use the second MRW{MR13 VRCG=0} of DFS as reference to meet SPEC tFC
    u4LP4_MR2RLWL = u4IO32ReadFldAlign(DRAMC_REG_SHU_HWSET_MR2, SHU_HWSET_MR2_HWSET_MR2_OP);

    //CKE high
    CKEFixOnOff(p, TO_ALL_RANK, CKE_FIXON, TO_ALL_CHANNEL);//tVRCG_DISABLE only allow Deselect command
    for(u1channel = CHANNEL_A; u1channel < p->support_channel_num; u1channel++)
    {
        vSetPHY2ChannelMapping(p, u1channel);

        DramcModeRegWriteByRank(p, u1set_mrsrk, 13, u4LP4_MR13FSP | (0x1 << 3));
        mcDELAY_XUS(1);//delay for tFC_Long = 250 ns
        DramcModeRegWriteByRank(p, u1set_mrsrk, 13, u4LP4_MR13FSP);//set VRCG = 0
        DramcModeRegWriteByRank(p, u1set_mrsrk,  2, u4LP4_MR2RLWL);
        DramcModeRegWriteByRank(p, u1set_mrsrk, 17, 0);//R1 lost data WA, set R0 R1 both for safe{On charging mode, system may issue MR17=0xFF in R1}
    }
    vSetPHY2ChannelMapping(p, eOriChannel);
    CKEFixOnOff(p, TO_ALL_RANK, CKE_DYNAMIC, TO_ALL_CHANNEL);
#endif

#ifdef HW_GATING
  #if EXIT_DRM_WITH_SAME_FREQ
    u1ShuLevel = u4IO32ReadFldAlign(DDRPHY_REG_DVFS_STATUS, DVFS_STATUS_OTHER_SHU_GP);
    u1OPEN     = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD6 + (SHU_GRP_DDRPHY_OFFSET * 0)), SHU_CA_CMD6_RG_ARPI_OPEN_EN_CA);
    u1SOPEN    = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD6 + (SHU_GRP_DDRPHY_OFFSET * 0)), SHU_CA_CMD6_RG_ARPI_SOPEN_EN_CA);
    mcSHOW_DRM_MSG(("Open/Sopen Mode=(%d,%d), shuffle group=%d\n",u1OPEN,u1SOPEN, u1ShuLevel));
    if (u1OPEN || u1SOPEN)
    {
        mcSHOW_DRM_MSG(("Disable HW Gating tracking for gating tracking fifo mode"));
        DramcHWGatingOnOff(p,0);
        //Enable_Gating_Tracking(p,u1ShuLevel);
        vIO32WriteFldMulti_All(DDRPHY_REG_MISC_SHU_STBCAL + (SHU_GRP_DDRPHY_OFFSET * 0),
            P_Fld(0x0, MISC_SHU_STBCAL_STBCALEN) |
            P_Fld(0x0, MISC_SHU_STBCAL_STB_SELPHCALEN));
    }
    else
    {
        mcSHOW_DRM_MSG(("Enable HW Gating tracking for gating tracking fifo mode"));
        DramcHWGatingOnOff(p,1);
        //Enable_Gating_Tracking(p,u1ShuLevel);
        vIO32WriteFldMulti_All(DDRPHY_REG_MISC_SHU_STBCAL + (SHU_GRP_DDRPHY_OFFSET * 0),
            P_Fld(0x1, MISC_SHU_STBCAL_STBCALEN) |
            P_Fld(0x1, MISC_SHU_STBCAL_STB_SELPHCALEN));
    }
  #else
    mcSHOW_DRM_MSG(("Enable HW Gating tracking for gating tracking fifo mode\n"));
    DramcHWGatingOnOff(p,1);
    Enable_Gating_Tracking(p,0);
  #endif
#endif

    mcSHOW_DRM_MSG(("Restore regs\n"));
    DramcRestoreRegisters(p, u4ReserveRegBackupAddress, sizeof(u4ReserveRegBackupAddress)/sizeof(U32));


    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_CG_CTRL0, 0x0, MISC_CG_CTRL0_RG_CG_PHY_OFF_DIABLE);


#ifdef IMPEDANCE_TRACKING_ENABLE
    mcSHOW_DRM_MSG(("Enable IMP tracking, \n"));
    //SPM only handle CHA IMPCAL_IMPCAL_HW, DDR2400 into DDR reserve mode(IMPCAL_IMPCAL_HW CHA = 0 CHB = 1)
    //CHB NAO have been reset to 0, need do hand shake with CHA, but CHA IMP tracking is off --> dead lock
    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_IMPCAL, 1, MISC_IMPCAL_IMPCAL_HW);
#endif

#ifdef ZQCS_ENABLE_LP4
  if (TRUE != is_lp5_family(p))
  {
    mcSHOW_DRM_MSG(("Enable HW ZQ\n"));
    vIO32WriteFldAlign_All(DRAMC_REG_ZQ_SET1, 1, ZQ_SET1_ZQCALDISB);
  }
#endif

    //u4value = u4IO32ReadFldAlign(DDRPHY_REG_MISC_SHU_CG_CTRL0, MISC_SHU_CG_CTRL0_R_PHY_MCK_CG_CTRL);
    //vIO32WriteFldAlign_All(DDRPHY_REG_MISC_SHU_CG_CTRL0,u4value & ~(0x22000000),  MISC_SHU_CG_CTRL0_R_PHY_MCK_CG_CTRL);

    mcSHOW_DRM_MSG(("Enable DDRPHY dynamic clock gating\n"));
    vIO32WriteFldAlign_All(DRAMC_REG_DRAMC_PD_CTRL, 1, DRAMC_PD_CTRL_PHYCLKDYNGEN);//enable DDRPHY dynamic clock gating

    mcSHOW_DRM_MSG(("Disable BLCOK_ALE + EN FAKEUP2 + hold\n"));
    vIO32WriteFldAlign_All(DRAMC_REG_DRAMCTRL, 0,  DRAMCTRL_ALEBLOCK);
    vIO32WriteFldAlign_All(DRAMC_REG_SHU_DCM_CTRL0, 1,  SHU_DCM_CTRL0_FASTWAKE2);
    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_DDR_RESERVE2, 0x300,  MISC_DDR_RESERVE2_DRM_SM_HOLD_CTRL);

    mcSHOW_DRM_MSG(("For DDR800semi WA, CA_PI clock CG free-run, 0x04EC[12], 0x04EC[10], 0x0500[22]\n"));
    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_CG_CTRL0, 0x0, MISC_CG_CTRL0_RG_CG_CMD_OFF_DISABLE);  //0x04EC[12]
//    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_CG_CTRL0, 0x0, MISC_CG_CTRL0_RG_CG_PHY_OFF_DIABLE);   //0x04EC[10]
    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_CG_CTRL5, 0x1, MISC_CG_CTRL5_R_CA_PI_DCM_EN);         //0x0500[22]
    //lynx added
    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_CG_CTRL5, 0x1, MISC_CG_CTRL5_R_DQ0_PI_DCM_EN);        //0x0500[21]
    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_CG_CTRL5, 0x1, MISC_CG_CTRL5_R_DQ1_PI_DCM_EN);        //0x0500[20]
    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_CG_CTRL5, 0x1, MISC_CG_CTRL5_R_CA_DLY_DCM_EN);        //0x0500[18]
    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_CG_CTRL5, 0x1, MISC_CG_CTRL5_R_DQ0_DLY_DCM_EN);       //0x0500[17]
    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_CG_CTRL5, 0x1, MISC_CG_CTRL5_R_DQ1_DLY_DCM_EN);       //0x0500[16]

    mcSHOW_DRM_MSG(("End Dramc_DDR_Reserved_Mode_AfterSR, dram type =%x \n", p->dram_type));
#if (FOR_DV_SIMULATION_USED == 1)
    mcSHOW_DRM_MSG(("Set DRM MP for next sim =%x \n", p->dram_type));//IPM V2.1=IPM V2.3
    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_SSPM_CFGREG_DBG_LATCH_SW, 1,  SSPM_CFGREG_DBG_LATCH_SW_R_DRM_LATCH_EN);//DDRPHY_MD32_BASE_ADDRESS + 0x401A8[1]
    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_DDR_RESERVE2, 0x300,  MISC_DDR_RESERVE2_DRM_SM_HOLD_CTRL);  //IPM V2.1 DDRPHY MP
    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_DDR_RESERVE3, 0x0,    MISC_DDR_RESERVE3_DRM_SM_PASS_CTRL);  //IPM V2.1 DDRPHY MP
#endif
    DramcBroadcastOnOff(backup_broadcast);

    //DramcRegDump(p);
#ifdef USE_TA2_IN_DDR_RESERVE_MODE
    {
        mcSHOW_DBG_MSG(("\n***TA2 test after exit DRM***\n"));
        int i = 0;
        U32 bit_error = 0;

        for(i = 0; i < 10; i++)
        {
            bit_error |= TA2_Test_After_DRM(p);
        }

#if USE_TA2_TO_DECIDE_DRM_RESULT
#if (!__ETT__)
        g_ddr_reserve_success = (bit_error == 0) ? (1) : (0); //Overwrite DRM result to avoid run re-K even access memory OK after DRM
#endif
#endif
    }
#endif

#if DRM_DUMP_RG
    dump_drm_rg(p, 0);
#endif
}

//----------------------------------------
// Auto Gen Code -- START
//----------------------------------------
void patch_to_before_k(DRAMC_CTX_T *p)
{
	vIO32Write4B_All(DRAMC_REG_TEST2_A0,0x100);
	vIO32Write4B_All(DRAMC_REG_TEST2_A3,0x2000880);
	vIO32Write4B_All(DRAMC_REG_TEST2_A4,0x4000110D);
	vIO32Write4B_All(DRAMC_REG_TX_TRACKING_SET0,0x0);
	vIO32Write4B_All(DRAMC_REG_DVFS_CTRL0,0x8300780);
	vIO32Write4B_All(DRAMC_REG_HMR4,0x940);
	vIO32Write4B_All(DRAMC_REG_TX_SET0,0x10);
	vIO32Write4B_All(DRAMC_REG_DQSOSCR,0x98E3160);
	vIO32Write4B_All(DRAMC_REG_REFCTRL0,0x5712000);
	vIO32Write4B_All(DRAMC_REG_REFCTRL1,0x3003116D);
	vIO32Write4B_All(DRAMC_REG_REF_BOUNCE1,0xFFFF0404);
	vIO32Write4B_All(DRAMC_REG_REF_BOUNCE2,0x1F);
	vIO32Write4B_All(DRAMC_REG_REFCTRL2,0x41F);
	vIO32Write4B_All(DRAMC_REG_CBT_WLEV_CTRL5,0x6F100);
	vIO32Write4B_All(DRAMC_REG_RK_DQSOSC,0x0);
	vIO32Write4B_All(DRAMC_REG_SELFREF_HWSAVE_FLAG,0x0);
	vIO32Write4B_All(DDRPHY_REG_B0_RXDVS0,0x3);
	vIO32Write4B_All(DDRPHY_REG_B0_PHY3,0x0);
	vIO32Write4B_All(DDRPHY_REG_B1_RXDVS0,0x3);
	vIO32Write4B_All(DDRPHY_REG_B1_PHY3,0x0);
	vIO32Write4B_All(DDRPHY_REG_CA_LP_CTRL0,0x1);
	vIO32Write4B_All(DDRPHY_REG_CA_CMD6,0x34040);
	vIO32Write4B_All(DDRPHY_REG_MISC_STBCAL2,0x1D0004);
	vIO32Write4B_All(DDRPHY_REG_MISC_CLK_CTRL,0x0);
	vIO32Write4B_All(DDRPHY_REG_MISC_PRE_TDQSCK1,0x0);
	vIO32Write4B_All(DDRPHY_REG_MISC_DDR_RESERVE,0x1040404);
	vIO32Write4B_All(DDRPHY_REG_MISC_IMPCAL,0x4003C00);
	vIO32Write4B_All(DDRPHY_REG_MISC_SRAM_DMA0,0x731000);
	vIO32Write4B_All(DDRPHY_REG_MISC_CTRL1,0x1000000);
	vIO32Write4B_All(DDRPHY_REG_MISC_RX_AUTOK_CFG0,0x0);
	vIO32Write4B_All(DDRPHY_REG_MISC_RX_AUTOK_CFG1,0x3);
	vIO32Write4B_All(DRAMC_REG_SHURK_DQSOSC,0x0);
	vIO32Write4B_All(DRAMC_REG_SHURK_DQSOSC_THRD_B0,0x10001);
	vIO32Write4B_All(DRAMC_REG_SHURK_DQSOSC_THRD_B1,0x10001);
	vIO32Write4B_All(DRAMC_REG_SHU_ACTIM4,0x28);
	vIO32Write4B_All(DRAMC_REG_SHU_ACTIM5,0x0);
	vIO32Write4B_All(DRAMC_REG_SHU_ACTIM6,0x4C00001F);
	vIO32Write4B_All(DRAMC_REG_SHU_AC_TIME_05T,0x0);
	vIO32Write4B_All(DRAMC_REG_SHU_TX_SET0,0x38000020);
	vIO32Write4B_All(DRAMC_REG_SHU_DQSOSC_SET0,0x200F1);
	vIO32Write4B_All(DRAMC_REG_SHU_DQSOSCR,0x1E0000);
	vIO32Write4B_All(DRAMC_REG_SHU_FREQ_RATIO_SET0,0x0);
	vIO32Write4B_All(DRAMC_REG_SHU_FREQ_RATIO_SET1,0x0);
	vIO32Write4B_All(DRAMC_REG_SHU_REF0,0x7);
	vIO32Write4B_All(DDRPHY_REG_SHU_B0_DQ11,0x0);
	vIO32Write4B_All(DDRPHY_REG_SHU_B0_RANK_SELPH_UI_DLY,0x0);
	vIO32Write4B_All(DDRPHY_REG_SHU_B1_DQ11,0x0);
	vIO32Write4B_All(DDRPHY_REG_SHU_B1_RANK_SELPH_UI_DLY,0x0);
	vIO32Write4B_All(DDRPHY_REG_SHU_CA_CMD10,0x0);
	vIO32Write4B_All(DDRPHY_REG_SHU_CA_CMD11,0x0);
	vIO32Write4B_All(DDRPHY_REG_MISC_SHU_RX_CG_CTRL,0x20007);
}
//----------------------------------------
// Auto Gen Code -- END
//----------------------------------------

void Patch_ReK(DRAMC_CTX_T *p)
{
#ifdef HW_GATING
    mcSHOW_DRM_MSG(("Disable HW Gating tracking for gating tracking fifo mode, 0x0514, 0x04E8, 0x0D14\n"));
    DramcHWGatingOnOff(p, 0);
#endif

#ifdef IMPEDANCE_TRACKING_ENABLE
    mcSHOW_DRM_MSG(("Disable IMP tracking\n"));
    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_IMPCAL, 0, MISC_IMPCAL_IMPCAL_HW);
    vIO32WriteFldAlign_All(DRAMC_REG_RK_DQSOSC, 0, RK_DQSOSC_DQSOSCR_RK0EN);
#endif

#ifdef ZQCS_ENABLE_LP4
    mcSHOW_DRM_MSG(("Disable HW ZQ\n"));
    vIO32WriteFldAlign_All(DRAMC_REG_ZQ_SET1, 0, ZQ_SET1_ZQCALDISB);
#endif

#if ENABLE_TX_TRACKING
    mcSHOW_DRM_MSG(("Disable TX tracking\n"));
    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_1, CHAN_MODE, LPIF_LOW_POWER_CFG_1_TX_TRACKING_DIS);
#endif

#if ENABLE_RX_TRACKING
    mcSHOW_DRM_MSG(("Disable RX tracking\n"));
    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_1, 0, LPIF_LOW_POWER_CFG_1_DPHY_RXDLY_TRACK_EN);

    mcSHOW_DRM_MSG(("Close RX tracking\n"));
    mcSHOW_DRM_MSG(("Close RX tracking by conf RG, fix 1:4 mode enter DDR reserve mode fail. PIC: Lynx\n"));
    vIO32WriteFldAlign_All(DDRPHY_REG_B0_RXDVS0, 0x0, B0_RXDVS0_R_RX_DLY_TRACK_ENA_B0);
    vIO32WriteFldAlign_All(DDRPHY_REG_B1_RXDVS0, 0x0, B1_RXDVS0_R_RX_DLY_TRACK_ENA_B1);

    mcSHOW_DRM_MSG(("clear rx tracking FSM - start\n"));
    vIO32WriteFldAlign_All(DDRPHY_REG_B0_RXDVS0, 1, B0_RXDVS0_R_RX_DLY_TRACK_CLR_B0);
    vIO32WriteFldAlign_All(DDRPHY_REG_B1_RXDVS0, 1, B1_RXDVS0_R_RX_DLY_TRACK_CLR_B1);
    mcSHOW_DRM_MSG(("clear rx tracking FSM - end\n"));

    vIO32WriteFldAlign_All(DDRPHY_REG_RK_B0_RXDVS2, 0x0, RK_B0_RXDVS2_R_RK0_DVS_MODE_B0);
    vIO32WriteFldAlign_All(DDRPHY_REG_RK_B1_RXDVS2, 0x0, RK_B1_RXDVS2_R_RK0_DVS_MODE_B1);
    vIO32WriteFldAlign_All(DDRPHY_AO_RANK_OFFSET + DDRPHY_REG_RK_B0_RXDVS2, 0x0, RK_B0_RXDVS2_R_RK0_DVS_MODE_B0);
    vIO32WriteFldAlign_All(DDRPHY_AO_RANK_OFFSET + DDRPHY_REG_RK_B1_RXDVS2, 0x0, RK_B1_RXDVS2_R_RK0_DVS_MODE_B1);
//    mcSHOW_DRM_MSG((" @Darren, Fix rx tracking cause DDR3733 TA2 fail via ddr reserved mode\n"));
//    vIO32WriteFldAlign_All(DDRPHY_REG_B0_DQ5, 0x0, B0_DQ5_RG_RX_ARDQS0_DVS_EN_B0);
//    vIO32WriteFldAlign_All(DDRPHY_REG_B1_DQ5, 0x0, B1_DQ5_RG_RX_ARDQS0_DVS_EN_B1);
    mcSHOW_DRM_MSG((" @Lynx, update RG name start from IPM V2.x\n"));
    vIO32WriteFldAlign_All(DDRPHY_REG_SHU_B0_DQ11, 0x0, SHU_B0_DQ11_RG_RX_ARDQ_DVS_EN_B0);
    vIO32WriteFldAlign_All(DDRPHY_REG_SHU_B1_DQ11, 0x0, SHU_B1_DQ11_RG_RX_ARDQ_DVS_EN_B1);
#endif

#ifdef TEMP_SENSOR_ENABLE 
    mcSHOW_DRM_MSG(("Disable MR4\n"));
    vIO32WriteFldAlign_All(DRAMC_REG_HMR4, 1, HMR4_REFRDIS);
    vIO32WriteFldAlign_All(DRAMC_REG_HMR4, 0, HMR4_SPDR_MR4_OPT); //Resume from S0 trigger HW MR4 function disable.
#endif

    mcSHOW_DRM_MSG(("Disable Dummy Read\n"));
    vIO32WriteFldMulti_All(DRAMC_REG_DUMMY_RD, P_Fld(0x0, DUMMY_RD_DQSG_DMYWR_EN)
            | P_Fld(0x0, DUMMY_RD_DQSG_DMYRD_EN)
            | P_Fld(0x0, DUMMY_RD_SREF_DMYRD_EN)
            | P_Fld(0x0, DUMMY_RD_DUMMY_RD_EN)
            | P_Fld(0x0, DUMMY_RD_DMY_RD_DBG)
            | P_Fld(0x0, DUMMY_RD_DMY_WR_DBG));

    EnableDramcPhyDCM(p, DCM_OFF); //Let CLK always free-run

    //WA for re-K
    vIO32WriteFldAlign_All(DDRPHY_REG_PHYPLL0, 0x1, PHYPLL0_RG_RPHYPLL_EN);//conf PLL_EN set to 1
    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0, 0, LPIF_LOW_POWER_CFG_0_PHYPLL_MODE_SW);//PLL control back to RG
    vIO32WriteFldAlign_All(SPM_POWER_ON_VAL0, 0, SPM_POWER_ON_VAL0_SC_PHYPLL_MODE_SW);//PLL control back to RG

    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_CLK_CTRL, 0, MISC_CLK_CTRL_DVFS_CLK_MEM_SEL);//[1]: 0:SW 1:HW => Set 0 for Init DRAM to switch to 26MHz

//    vIO32WriteFldAlign_All(DRAMC_REG_DVFS_CTRL0, 1, DVFS_CTRL0_R_DMDVFSMRW_EN);//must be, could not switch to ddr1600
//    vIO32WriteFldAlign_All(DRAMC_REG_DVFS_CTRL0, 0, DVFS_CTRL0_DVFS_NOQUEFLUSH_EN);//must be, could not switch to ddr1600
//    vIO32WriteFldAlign_All(DRAMC_REG_DQSOSCR, 1, DQSOSCR_ARUIDQ_SW);//must be, TA2 fail

    patch_to_before_k(p);
}

void Before_Init_DRAM_While_Reserve_Mode_fail(DRAM_DRAM_TYPE_T dram_type)
{
    DRAMC_CTX_T * p = &DramCtx_LPDDR4;
    mcSHOW_DBG_MSG(("\n\tReserve mode fail\tBefore_Init_DRAM_While_Reserve_Mode_fail\n"));

    Patch_ReK(p);
}

#if __ETT__
#if DRM_FAIL_RE_INTDRAMC_TEST
int fgDRMFailReInitDramTest = 0;
#endif
void ETT_DRM(DRAMC_CTX_T *p)
{
    U32 u4SramShuLevel;
    U32 u4ConfShuLevel;
#if DRM_FAIL_RE_INTDRAMC_TEST
    EMI_SETTINGS *emi_set = &g_default_emi_setting;
    int ret=0;
#endif

#if DRM_FAIL_RE_INTDRAMC_TEST
    if(fgDRMFailReInitDramTest)
    {
        return;
    }
#endif

    int i = 0;
    U32 bit_error = 0;

    for(i = 0; i < 10; i++)
    {
        bit_error |= TA2_Test_After_DRM(p);
    }

    //After system reset shu level value will latch to RG:DDRPHY_MD32_REG_LPIF_DBG_LATCH8
    u4ConfShuLevel = u4IO32ReadFldAlign(DRAMC_REG_SA_RESERVE, SA_RESERVE_DRM_SHU_LEVEL);
    u4SramShuLevel = u4IO32ReadFldAlign(DRAMC_REG_SA_RESERVE, SA_RESERVE_DRM_SHU_SRAM_LEVEL);
    vSetDFSTable(p, get_FreqTbl_by_SRAMIndex(p, u4SramShuLevel));
    DDRPhyFreqSel(p, p->pDFSTable->freq_sel);
    mcSHOW_DRM_MSG(("@@@ In DRM with DDR[%d] conf_shuffle_level[%d] sram_shuffle_level[%d]\n", p->frequency << 1, u4ConfShuLevel, u4SramShuLevel));

    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0, CHAN_MODE, LPIF_LOW_POWER_CFG_0_EMI_CLK_OFF_REQ);//DRAM into SR

    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0, CHAN_MODE,LPIF_LOW_POWER_CFG_0_DMSUS_OFF);//DMSUS set 1

    Dramc_DDR_Reserved_Mode_setting();

    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0, 0 ,LPIF_LOW_POWER_CFG_0_DMSUS_OFF);//DMSUS set 0

    Dramc_DDR_Reserved_Mode_BeforeSR();

    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0, 0 ,LPIF_LOW_POWER_CFG_0_EMI_CLK_OFF_REQ);//DRAM exit SR

    Dramc_DDR_Reserved_Mode_AfterSR();

	mcSHOW_DBG_MSG(("\nPush any key to continue...\n\n"));
	while ( UART_Get_Command() == 0);

#if DRM_FAIL_RE_INTDRAMC_TEST
	fgDRMFailReInitDramTest = 1;
#if DRM_RESET_CONF_HW
	mcSHOW_DBG_MSG(("DDRPHY_REG_PHYPLL0=0x%x...\n", u4IO32Read4B(DDRPHY_REG_PHYPLL0)));
	mcSHOW_DBG_MSG(("DRAMC_REG_DDRCOMMON0=0x%x...\n", u4IO32Read4B(DRAMC_REG_DDRCOMMON0)));
	vIO32WriteFldAlign(DDRPHY_MD32_REG_LPIF_FSM_CTRL_0, 0, LPIF_FSM_CTRL_0_RESERVED_X20_31_31);
	vIO32WriteFldAlign_All(DDRPHY_MD32_REG_SSPM_CFGREG_SW_RSTN, 0 ,SSPM_CFGREG_SW_RSTN_DRAMC_CONF_RST_B);
	vIO32WriteFldAlign_All(DDRPHY_MD32_REG_SSPM_CFGREG_SW_RSTN, 1, SSPM_CFGREG_SW_RSTN_DRAMC_CONF_RST_B);
	vIO32WriteFldAlign(DDRPHY_MD32_REG_LPIF_FSM_CTRL_0, 1, LPIF_FSM_CTRL_0_RESERVED_X20_31_31);
	mcSHOW_DBG_MSG(("DDRPHY_REG_PHYPLL0=0x%x...\n", u4IO32Read4B(DDRPHY_REG_PHYPLL0)));
	mcSHOW_DBG_MSG(("DRAMC_REG_DDRCOMMON0=0x%x...\n", u4IO32Read4B(DRAMC_REG_DDRCOMMON0)));
	mcSHOW_DBG_MSG(("\nPush any key to continue...\n\n"));
	while ( UART_Get_Command() == 0);
#else
	Patch_ReK(p);
	mcSHOW_DBG_MSG(("\nPush any key to continue...\n\n"));
	while ( UART_Get_Command() == 0);
#endif
	ret = Init_DRAM((emi_set->type & 0xF), emi_set->dram_cbt_mode_extern, NULL, NORMAL_USED);

	for(i = 0; i < 10; i++)
	{
		bit_error |= TA2_Test_After_DRM(p);
	}

    mcSHOW_DBG_MSG(("\nPush any key to continue...\n\n"));
	while ( UART_Get_Command() == 0);
#endif
}
#endif
#endif

