#if (FOR_DV_SIMULATION_USED == 0)
#include "dramc_top.h"
#endif
#include "dramc_common.h"
#include "x_hal_io.h"
//#define DDR_RESERVE_MODE

#define EXIT_DRM_WITH_SAME_FREQ 1 //1:DRM out with the same freq
#ifdef DDR_RESERVE_MODE
#define mcSHOW_DRM_MSG mcSHOW_DBG_MSG
#define CHAN_MODE 3 //1_chan: 1 2_chan: 3 //NEED_DE_NOTICE

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

void LP_Mode_DRM_Setting(void)
{
    DRAMC_CTX_T * p;
    p = &DramCtx_LPDDR4;
    #if ENABLE_DDR400_OPEN_LOOP_MODE_OPTION
    U32 fgOpenLoop = (vGet_DDR_Loop_Mode(p) == OPEN_LOOP_MODE?1:0);
    #else
    U32 fgOpenLoop = 0;
    #endif

    mcSHOW_DRM_MSG(("LP_Mode_DRM_Setting(26M to MCK)\n"));

    if(fgOpenLoop == 0x1)
    {
        vIO32WriteFldAlign_All(DDRPHY_REG_SHU_MISC_CLK_CTRL0, 1, SHU_MISC_CLK_CTRL0_M_CK_OPENLOOP_MODE_SEL);
        mcDELAY_US(1);
        vIO32WriteFldAlign_All(DDRPHY_REG_MISC_CG_CTRL0, 1, MISC_CG_CTRL0_RESERVED_MISC_CG_CTRL0_BIT3_1);//DDRPHY_AO_BASE_ADDRESS + 0x04EC[3:1]
        vIO32WriteFldAlign_All(DDRPHY_REG_MISC_CG_CTRL0, 0, MISC_CG_CTRL0_RESERVED_MISC_CG_CTRL0_BIT3_1);
        vIO32WriteFldAlign_All(DDRPHY_REG_MISC_CG_CTRL0, 1, MISC_CG_CTRL0_RESERVED_MISC_CG_CTRL0_BIT7);//DDRPHY_AO_BASE_ADDRESS + 0x04EC[7]
    }

    vIO32WriteFldMulti_All(DDRPHY_REG_MISC_CLK_CTRL,
                     P_Fld(0x1, MISC_CLK_CTRL_DVFS_CLK_MEM_SEL) | // by "sel_source"
                     P_Fld(0x0, MISC_CLK_CTRL_DVFS_MEM_CK_MUX_SEL_MODE) | // HW mode by shuffle
                     P_Fld(0x1, MISC_CLK_CTRL_DVFS_MEM_CK_MUX_SEL)); // 4-to-1 mux from PLLCK if DVFS_MEM_CK_MUX_SEL_MODE = 2

    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_CG_CTRL0, 1, MISC_CG_CTRL0_CLK_MEM_SEL);//Just sync MP setting, If DVFS_CLK_MEM_SEL = 0;DDRPHY_AO_BASE_ADDRESS + 0x04EC[5:4]
    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_CG_CTRL0, 1, MISC_CG_CTRL0_W_CHG_MEM);//DDRPHY_AO_BASE_ADDRESS + 0x04EC[0]

    mcSHOW_DRM_MSG(("DELAY 1 us\n"));
    mcDELAY_US(1);

    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_CG_CTRL0, 0, MISC_CG_CTRL0_W_CHG_MEM);
}

void ASVA5_8_New_Mode_1(void)
{
    U32 u4WbrBackup = 0;
    U32 u4value = 0;
    DRAMC_CTX_T * p;
    p = &DramCtx_LPDDR4;
    BOOL bSupport2Rank = (u4IO32ReadFldAlign(DRAMC_REG_SA_RESERVE, SA_RESERVE_SINGLE_RANK) == 0) ? TRUE : FALSE; 

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
    vIO32WriteFldAlign_All(DDRPHY_REG_CA_CMD7, 1, CA_CMD7_RG_TX_ARCS0_PULL_DN);
    vIO32WriteFldAlign_All(DDRPHY_REG_CA_CMD7, 1, CA_CMD7_RG_TX_ARCS1_PULL_DN);
    vIO32WriteFldAlign_All(DDRPHY_REG_CA_CMD7, 1, CA_CMD7_RG_TX_ARCMD_PULL_DN);
    vIO32WriteFldAlign_All(DDRPHY_REG_CA_LP_CTRL0, 0, CA_LP_CTRL0_RG_TX_ARCS_PULL_DN_LP_SEL);
    vIO32WriteFldAlign_All(DDRPHY_REG_CA_LP_CTRL0, 0, CA_LP_CTRL0_RG_TX_ARCA_PULL_DN_LP_SEL);
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
    vIO32WriteFldAlign_All(DDRPHY_REG_CA_LP_CTRL0, 1, CA_LP_CTRL0_RG_TX_ARCS_PULL_DN_LP_SEL);
    vIO32WriteFldAlign_All(DDRPHY_REG_CA_LP_CTRL0, 1, CA_LP_CTRL0_RG_TX_ARCA_PULL_DN_LP_SEL);
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

void dump_gating_error_rg(DRAMC_CTX_T * p, U32 u4DramType)
{
    DRAM_CHANNEL_T bkChannel = vGetPHY2ChannelMapping(p);
    U8 u1channel = CHANNEL_A;
    U32 u4value = 0;

    for(u1channel = CHANNEL_A; u1channel < p->support_channel_num; u1channel++)
    {
        vSetPHY2ChannelMapping(p, u1channel);
        u4value = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_REG_MISC_STBERR_ALL));
        mcSHOW_ERR_MSG(("###DDR reserve CH%d DDRPHY_REG_MISC_STBERR_ALL [0x%X]\n", u1channel, u4value));
    }
    vSetPHY2ChannelMapping(p, bkChannel);
    return;
}


void Dramc_DDR_Reserved_Mode_setting(void)
{
    U32 u4WbrBackup = 0;
    U32 u4value = 0;
    U32 backup_broadcast = GetDramcBroadcast();
    DRAMC_CTX_T * p;
    p = &DramCtx_LPDDR4;
    BOOL bSupport2Rank = (u4IO32ReadFldAlign(DRAMC_REG_SA_RESERVE, SA_RESERVE_SINGLE_RANK) == 0) ? TRUE : FALSE; 
    U32 u4SramShuLevel = 0;
    U32 u4ConfShuLevel = 0;
    unsigned int temp;

    #if __ETT__
    u4ConfShuLevel = u4IO32ReadFldAlign(DDRPHY_MD32_REG_LPIF_STATUS_10, LPIF_STATUS_10_DRAMC_DR_SHU_LEVEL);
    u4ConfShuLevel = u4ConfShuLevel & 0x3;
    u4SramShuLevel = u4IO32ReadFldAlign(DDRPHY_MD32_REG_LPIF_STATUS_10, LPIF_STATUS_10_DRAMC_DR_SHU_SRAM_LEVEL);
    u4SramShuLevel = u4SramShuLevel & 0xf;
    #else
    u4ConfShuLevel = u4IO32ReadFldAlign(DDRPHY_MD32_REG_LPIF_DBG_LATCH8, LPIF_DBG_LATCH8_DRAMC_DR_SHU_LEVEL);
    u4ConfShuLevel = u4ConfShuLevel & 0x3;
    u4SramShuLevel = u4IO32ReadFldAlign(DDRPHY_MD32_REG_LPIF_DBG_LATCH8, LPIF_DBG_LATCH8_DRAMC_DR_SHU_SRAM_LEVEL);
    u4SramShuLevel = u4SramShuLevel & 0xf;
    #endif
	
    mcSHOW_DRM_MSG(("@@@ conf_shuffle_level[%d] sram shuffle level[%d]\n", u4ConfShuLevel, u4SramShuLevel));
#if EXIT_DRM_WITH_SAME_FREQ 
    mcSHOW_DRM_MSG(("@@@ Out DRM with the same freq.\n"));   
    vSetDFSTable(p, get_FreqTbl_by_SRAMIndex(p, u4SramShuLevel));
    DDRPhyFreqSel(p, p->pDFSTable->freq_sel);
#else
    p->frequency = u2DFSGetHighestFreq(p);
    vSetDFSTable(p, get_FreqTbl_by_SRAMIndex(p, SRAM_SHU0));
#endif
    mcSHOW_DRM_MSG(("@@@ Out DRM with DDR[%d]\n", p->frequency << 1));
    vSetChannelNumber(p);
    vSetRankNumber(p);
//    if(p->support_channel_num !=2) mcSHOW_DRM_MSG(("Set DRAMC Dual Channel\n"));//FOR_DV_SIMULATION_USE, never delet
//    p->support_channel_num = 2;//FOR_DV_SIMULATION_USE, never delet


    mcSHOW_DRM_MSG(("Start Dramc_DDR_Reserved_Mode_setting \n"));
    DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);

    mcSHOW_DRM_MSG(("Enable BLCOK_ALE\n"));
    vIO32WriteFldAlign_All(DRAMC_REG_DRAMCTRL, 1,  DRAMCTRL_ALEBLOCK);

    mcSHOW_DRM_MSG(("APB clk To MCK\n"));//  ****Need to do before trigger DRM
////  vIO32WriteFldAlign_All(DDRPHY_REG_MISC_CKMUX_SEL, 0x3, MISC_CKMUX_SEL_FMEM_CK_MUX);

    vIO32WriteFldMulti_All(DDRPHY_REG_MISC_CG_CTRL0,
              P_Fld(0x0, MISC_CG_CTRL0_W_CHG_MEM)
            | P_Fld(0x0, MISC_CG_CTRL0_RESERVED_MISC_CG_CTRL0_BIT3_1)
            | P_Fld(0x0, MISC_CG_CTRL0_CLK_MEM_SEL));


//    u4value = u4IO32ReadFldAlign(DRAMC_REG_SHUSTATUS, SHUSTATUS_SHUFFLE_LEVEL);//from conf: 0 or 1 //==TODO
//    mcSHOW_ERR_MSG(("### shuffle level[%d]\n", u4value));

#ifdef HW_GATING
    mcSHOW_DRM_MSG(("Disable HW Gating tracking for gating tracking fifo mode, 0x0514, 0x04E8, 0x0D14\n"));
    DramcHWGatingOnOff(p, 0);
#endif

//    u4DramType = u4IO32ReadFldAlign(DRAMC_REG_ARBCTL, ARBCTL_RSV_DRAM_TYPE);//==TODO
//    mcSHOW_DRM_MSG(("### dram type =%x \n", u4DramType));

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
    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_CG_CTRL5, 0x0, MISC_CG_CTRL5_R_DQ0_DLY_DCM_EN);        //0x0500[22]
    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_CG_CTRL5, 0x0, MISC_CG_CTRL5_R_DQ1_DLY_DCM_EN);        //0x0500[22]

    mcSHOW_DRM_MSG(("Backup regs\n"));
    DramcBackupRegisters(p, u4ReserveRegBackupAddress, sizeof(u4ReserveRegBackupAddress)/sizeof(U32));

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

    mcSHOW_DRM_MSG(("To 26MHz\n"));
    vIO32WriteFldMulti_All(DDRPHY_REG_MISC_CLK_CTRL , P_Fld(0, MISC_CLK_CTRL_DVFS_CLK_MEM_SEL) );//[1]: 0:SW 1:HW
    vIO32WriteFldMulti_All(DDRPHY_REG_MISC_CG_CTRL0,
                  P_Fld(0, MISC_CG_CTRL0_W_CHG_MEM)
                | P_Fld(0, MISC_CG_CTRL0_CLK_MEM_SEL));//[5:4] mem_ck mux: 2'b00: 26MHz, [0]: change memory clock
    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_CG_CTRL0, 1, MISC_CG_CTRL0_W_CHG_MEM);//change clock freq

    temp = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_MISC_CLK_CTRL0), SHU_MISC_CLK_CTRL0_M_CK_OPENLOOP_MODE_SEL);
	
    if (temp) {
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CG_CTRL0), 0x0, MISC_CG_CTRL0_RESERVED_MISC_CG_CTRL0_BIT7);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_MISC_CLK_CTRL0), 0x0, SHU_MISC_CLK_CTRL0_M_CK_OPENLOOP_MODE_SEL);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CG_CTRL0), 0x1, MISC_CG_CTRL0_RESERVED_MISC_CG_CTRL0_BIT3_1);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CG_CTRL0), 0x0, MISC_CG_CTRL0_RESERVED_MISC_CG_CTRL0_BIT3_1);	
    }	

    mcSHOW_DRM_MSG(("DELAY 1 us\n"));
    mcDELAY_US(1);

    mcSHOW_DRM_MSG(("disable memory clock change\n"));
    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_CG_CTRL0, 0, MISC_CG_CTRL0_W_CHG_MEM);


#if 0
    mcSHOW_DRM_MSG(("Load shuffle 0 setting from SRAM by DMA \n"));
    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_SRAM_DMA0, 0, MISC_SRAM_DMA0_SW_DMA_FIRE);
    vIO32WriteFldMulti_All(DDRPHY_REG_MISC_SRAM_DMA0, P_Fld(1, MISC_SRAM_DMA0_SW_MODE)
                | P_Fld(1, MISC_SRAM_DMA0_SW_STEP_EN_MODE));
    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_SRAM_DMA0, 0, MISC_SRAM_DMA0_SRAM_WR_MODE);
    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_SRAM_DMA0, 1, MISC_SRAM_DMA0_APB_WR_MODE);
    vIO32WriteFldMulti_All(DDRPHY_REG_MISC_SRAM_DMA0, P_Fld(0, MISC_SRAM_DMA0_SW_SHU_LEVEL_APB)
                | P_Fld(0, MISC_SRAM_DMA0_SW_SHU_LEVEL_SRAM));
    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_SRAM_DMA0, 1, MISC_SRAM_DMA0_SW_DMA_FIRE);


    mcSHOW_DRM_MSG(("Wait sram_done APB_done\n"));
    while((u4IO32ReadFldAlign(DDRPHY_REG_MISC_DMA_DEBUG0, MISC_DMA_DEBUG0_APB_DONE ) != 1) ||
          (u4IO32ReadFldAlign(DDRPHY_REG_MISC_DMA_DEBUG0, MISC_DMA_DEBUG0_SRAM_DONE) != 1));
    while((u4IO32ReadFldAlign(DDRPHY_REG_MISC_DMA_DEBUG0 + SHIFT_TO_CHB_ADDR, MISC_DMA_DEBUG0_APB_DONE ) != 1) ||
          (u4IO32ReadFldAlign(DDRPHY_REG_MISC_DMA_DEBUG0 + SHIFT_TO_CHB_ADDR, MISC_DMA_DEBUG0_SRAM_DONE) != 1));
    //*** WARNING*** SA Need to use below setting of CHC, CHD *** WARNING***
    //*** WARNING***      Pelase Check C code for detail      *** WARNING***
//  *** WARNING*** SA Need to use below setting of CHC, CHD
//    while((u4IO32ReadFldAlign(DDRPHY_REG_MISC_DMA_DEBUG0 + SHIFT_TO_CHC_ADDR, MISC_DMA_DEBUG0_APB_DONE ) != 1) ||
//          (u4IO32ReadFldAlign(DDRPHY_REG_MISC_DMA_DEBUG0 + SHIFT_TO_CHC_ADDR, MISC_DMA_DEBUG0_SRAM_DONE) != 1));
//    while((u4IO32ReadFldAlign(DDRPHY_REG_MISC_DMA_DEBUG0 + SHIFT_TO_CHD_ADDR, MISC_DMA_DEBUG0_APB_DONE ) != 1) ||
//          (u4IO32ReadFldAlign(DDRPHY_REG_MISC_DMA_DEBUG0 + SHIFT_TO_CHD_ADDR, MISC_DMA_DEBUG0_SRAM_DONE) != 1));





    vIO32WriteFldMulti_All(DDRPHY_REG_MISC_SRAM_DMA0, P_Fld(0, MISC_SRAM_DMA0_SW_DMA_FIRE)
                | P_Fld(0, MISC_SRAM_DMA0_SW_MODE)
                | P_Fld(0, MISC_SRAM_DMA0_SW_STEP_EN_MODE));
#endif

#if EXIT_DRM_WITH_SAME_FREQ
    mcSHOW_DRM_MSG(("Load previous shuffle setting from SRAM by DMA \n"));
    if(u4ConfShuLevel)//from CLRPLL
    {
        mcSHOW_DRM_MSG(("from CLRPLL \n"));
        LoadShuffleSRAMtoDramc(p, u4SramShuLevel, DRAM_DFS_REG_SHU0); //NEED_DE_NOTICE
#if ENABLE_SRAM_DMA_WA
        mcSHOW_DRM_MSG((" + ENABLE_SRAM_DMA_WA \n"));
        SRAMShuRestoreToDPHYWA(p, u4SramShuLevel, PHYPLL_MODE); //NEED_DE_NOTICE
#endif
    }
#else
    mcSHOW_DRM_MSG(("Load shuffle 0 setting from SRAM by DMA \n"));
    LoadShuffleSRAMtoDramc(p, 0, DRAM_DFS_REG_SHU0); //NEED_DE_NOTICE
#if ENABLE_SRAM_DMA_WA
    mcSHOW_DRM_MSG((" + ENABLE_SRAM_DMA_WA \n"));
    SRAMShuRestoreToDPHYWA(p, 0, PHYPLL_MODE); //NEED_DE_NOTICE
#endif
#endif

    //mcSHOW_DRM_MSG(("Pipe line dcm off\n")); //Dummy code, since the value is the same with golden setting
    //u4value = u4IO32ReadFldAlign(DDRPHY_REG_MISC_SHU_CG_CTRL0, MISC_SHU_CG_CTRL0_R_PHY_MCK_CG_CTRL);
    //vIO32WriteFldAlign_All(DDRPHY_REG_MISC_SHU_CG_CTRL0, u4value | 0x33000000, MISC_SHU_CG_CTRL0_R_PHY_MCK_CG_CTRL);

    mcSHOW_DRM_MSG((" @Darren, Fix rx tracking cause DDR3733 TA2 fail via ddr reserved mode\n"));
    vIO32WriteFldAlign_All(DDRPHY_REG_SHU_B0_DQ11, 0x0, SHU_B0_DQ11_RG_RX_ARDQ_DVS_EN_B0);
    vIO32WriteFldAlign_All(DDRPHY_REG_SHU_B1_DQ11, 0x0, SHU_B1_DQ11_RG_RX_ARDQ_DVS_EN_B1);

#ifdef HW_GATING
    mcSHOW_DRM_MSG(("Disable HW Gating tracking for gating tracking fifo mode, 0x0514, 0x04E8, 0x0D14\n"));
    DramcHWGatingOnOff(p, 0);
#endif

#if ENABLE_TX_TRACKING
    vIO32WriteFldAlign_All(DRAMC_REG_DRAMC_PD_CTRL, 0x0, DRAMC_PD_CTRL_DCMEN2);

    //SW force reload from AO to NAO, TX tracking only track PI; ARUIDQ_SW = 1, UI will use AO conf
    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_CTRL1, 0, MISC_CTRL1_R_DMARPIDQ_SW);
    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_CTRL1, 1, MISC_CTRL1_R_DMARPIDQ_SW);
    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_CTRL1, 0, MISC_CTRL1_R_DMARPIDQ_SW);

    //vIO32WriteFldAlign_All(DRAMC_REG_DQSOSCR, 0, DQSOSCR_ARUIDQ_SW);
    //vIO32WriteFldAlign_All(DRAMC_REG_DQSOSCR, 1, DQSOSCR_ARUIDQ_SW);
    //vIO32WriteFldAlign_All(DRAMC_REG_DQSOSCR, 0, DQSOSCR_ARUIDQ_SW);
#endif

/*  mcSHOW_DRM_MSG(("Pipe line dcm off\n"));
    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_SHU_CG_CTRL0, u4IO32Read4B(DDRPHY_REG_MISC_SHU_CG_CTRL0) | 0x33000000, MISC_SHU_CG_CTRL0_R_PHY_MCK_CG_CTRL);


    vIO32WriteFldMulti_All(DDRPHY_REG_MISC_CLK_CTRL , P_Fld(0, MISC_CLK_CTRL_DVFS_CLK_MEM_SEL) );//[1]: 0:SW 1:HW

    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_CG_CTRL0, 1, MISC_CG_CTRL0_W_CHG_MEM);//change clock freq

    mcSHOW_DRM_MSG(("DELAY 1 us\n"));
    mcDELAY_US(1);

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
    vIO32WriteFldAlign_All(DDRPHY_REG_SHU_CA_CMD1, 0x0, SHU_CA_CMD1_RG_ARPI_MIDPI_EN_CA);

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


    mcSHOW_DRM_MSG(("PLL_EN = 0\n"));
    vIO32WriteFldAlign_All(DDRPHY_REG_PHYPLL0, 0x0, PHYPLL0_RG_RPHYPLL_EN);//RG_*PLL_EN=0; Since there is only 1 PLL, only control CHA //Follow Petrus_DDR_reserved C code_20190123.docx
    vIO32WriteFldAlign_All(DDRPHY_REG_CLRPLL0, 0x0, CLRPLL0_RG_RCLRPLL_EN);//RG_*PLL_EN=0; Since there is only 1 PLL, only control CHA //Follow Petrus_DDR_reserved C code_20190123.docx
    //===========================================================================================================

    MPLLInit();

    //marked by kaihsin on May 7th, not necessary to PDN DDRPHY
    //*((UINT32P)(0x1000631c )) &= ~(0x1 << 2); //ddrphy_pwr_on=0
    //*((UINT32P)(0x100063b8 )) &= ~(0x1 << 2);
    mcSHOW_DRM_MSG(("DELAY 1 us\n"));
    mcDELAY_US(1);
    //marked by kaihsin on May 7th, not necessary to PDN DDRPHY
    //*((UINT32P)(0x1000631c )) |= (0x1 << 2); //ddrphy_pwr_on=1
    //*((UINT32P)(0x100063b8 )) |= (0x1 << 2);
    mcSHOW_DRM_MSG(("DELAY 1 us\n"));
    mcDELAY_US(1);

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
    mcDELAY_US(20);

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

    mcSHOW_DRM_MSG(("DELAY 20 us\n"));
    mcDELAY_US(20);//Wait 20us for MEMPLL

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
    mcDELAY_US(1);

    mcSHOW_DRM_MSG(("MIDPI*_EN = 1\n"));
    vIO32WriteFldAlign_All(DDRPHY_REG_SHU_B0_DQ1, 0x1, SHU_B0_DQ1_RG_ARPI_MIDPI_EN_B0);
    vIO32WriteFldAlign_All(DDRPHY_REG_SHU_B1_DQ1, 0x1, SHU_B1_DQ1_RG_ARPI_MIDPI_EN_B1);
    vIO32WriteFldAlign_All(DDRPHY_REG_SHU_CA_CMD1, 0x1, SHU_CA_CMD1_RG_ARPI_MIDPI_EN_CA);

    mcSHOW_DRM_MSG(("DELAY 10 us\n"));
    mcDELAY_US(10);

    mcSHOW_DRM_MSG(("RG_ARPI_RESETB_* = 1\n"));
    vIO32WriteFldAlign_All(DDRPHY_REG_B0_DLL_ARPI0, 0x1, B0_DLL_ARPI0_RG_ARPI_RESETB_B0);
    vIO32WriteFldAlign_All(DDRPHY_REG_B1_DLL_ARPI0, 0x1, B1_DLL_ARPI0_RG_ARPI_RESETB_B1);
    vIO32WriteFldAlign_All(DDRPHY_REG_CA_DLL_ARPI0, 0x1, CA_DLL_ARPI0_RG_ARPI_RESETB_CA);

    mcSHOW_DRM_MSG(("DELAY 1 us\n"));
    mcDELAY_US(1);

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
    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_DVFSCTL2, 1, MISC_DVFSCTL2_RG_ADA_MCK8X_EN_SHUFFLE); //Follow Petrus_DDR_reserved C code_20190123.docx
    vIO32WriteFldAlign_All(DDRPHY_REG_PHYPLL2, 1, PHYPLL2_RG_RPHYPLL_AD_MCK8X_EN);//RG_*MCK8X_EN=0; Since there is only 1 PLL, only to control CHA //Follow Petrus_DDR_reserved C code_20190123.docx

    mcSHOW_DRM_MSG(("DELAY 1 us\n"));
    mcDELAY_US(1);

    mcSHOW_DRM_MSG(("*PI_CG = 0, RG_*MPDIV_CG = 0\n"));
    vIO32WriteFldMulti_All(DDRPHY_REG_SHU_B0_DLL_ARPI2, P_Fld(0x0, SHU_B0_DLL_ARPI2_RG_ARPI_CG_MCK_B0)
                | P_Fld(0x0, SHU_B0_DLL_ARPI2_RG_ARPI_CG_MCK_FB2DLL_B0)
                | P_Fld(0x0, SHU_B0_DLL_ARPI2_RG_ARPI_CG_MCTL_B0)
                | P_Fld(0x0, SHU_B0_DLL_ARPI2_RG_ARPI_CG_FB_B0)
                | P_Fld(0x0, SHU_B0_DLL_ARPI2_RG_ARPI_CG_DQS_B0)
                | P_Fld(0x0, SHU_B0_DLL_ARPI2_RG_ARPI_CG_DQM_B0)
                | P_Fld(0x0, SHU_B0_DLL_ARPI2_RG_ARPI_CG_DQ_B0)
                | P_Fld(0x0, SHU_B0_DLL_ARPI2_RG_ARPI_CG_DQSIEN_B0)
                | P_Fld(0x0, SHU_B0_DLL_ARPI2_RG_ARPI_MPDIV_CG_B0));
    vIO32WriteFldMulti_All(DDRPHY_REG_SHU_B1_DLL_ARPI2, P_Fld(0x0, SHU_B1_DLL_ARPI2_RG_ARPI_CG_MCK_B1)
                | P_Fld(0x0, SHU_B1_DLL_ARPI2_RG_ARPI_CG_MCK_FB2DLL_B1)
                | P_Fld(0x0, SHU_B1_DLL_ARPI2_RG_ARPI_CG_MCTL_B1)
                | P_Fld(0x0, SHU_B1_DLL_ARPI2_RG_ARPI_CG_FB_B1)
                | P_Fld(0x0, SHU_B1_DLL_ARPI2_RG_ARPI_CG_DQS_B1)
                | P_Fld(0x0, SHU_B1_DLL_ARPI2_RG_ARPI_CG_DQM_B1)
                | P_Fld(0x0, SHU_B1_DLL_ARPI2_RG_ARPI_CG_DQ_B1)
                | P_Fld(0x0, SHU_B1_DLL_ARPI2_RG_ARPI_CG_DQSIEN_B1)
                | P_Fld(0x0, SHU_B1_DLL_ARPI2_RG_ARPI_MPDIV_CG_B1));
    vIO32WriteFldMulti_All(DDRPHY_REG_SHU_CA_DLL_ARPI2, P_Fld(0x0, SHU_CA_DLL_ARPI2_RG_ARPI_CG_MCK_CA)//not include 11th bit(CA_DLL_ARPI2_RG_ARPI_CG_CLKIEN) LewisToDo
                | P_Fld(0x0, SHU_CA_DLL_ARPI2_RG_ARPI_CG_MCK_FB2DLL_CA)
                | P_Fld(0x0, SHU_CA_DLL_ARPI2_RG_ARPI_CG_MCTL_CA)
                | P_Fld(0x0, SHU_CA_DLL_ARPI2_RG_ARPI_CG_FB_CA)
                | P_Fld(0x0, SHU_CA_DLL_ARPI2_RG_ARPI_CG_CS)
                | P_Fld(0x0, SHU_CA_DLL_ARPI2_RG_ARPI_CG_CLK)
                | P_Fld(0x0, SHU_CA_DLL_ARPI2_RG_ARPI_CG_CMD)
                | P_Fld(0x0, SHU_CA_DLL_ARPI2_RG_ARPI_MPDIV_CG_CA));

    mcSHOW_DRM_MSG(("RG_*BIAS_EN = 1\n"));
    vIO32WriteFldAlign_All(DDRPHY_REG_B0_DQ6, 0x1, B0_DQ6_RG_RX_ARDQ_BIAS_EN_B0);
    vIO32WriteFldAlign_All(DDRPHY_REG_B1_DQ6, 0x1, B1_DQ6_RG_RX_ARDQ_BIAS_EN_B1);
    vIO32WriteFldAlign_All(DDRPHY_REG_CA_CMD6, 0x1, CA_CMD6_RG_RX_ARCMD_BIAS_EN);

    mcSHOW_DRM_MSG(("DELAY 1 us\n"));
    mcDELAY_US(1);

//    vIO32WriteFldMulti_All(DDRPHY_REG_MISC_CLK_CTRL , P_Fld(1, MISC_CLK_CTRL_DVFS_CLK_MEM_SEL) );//[1]: 0:SW 1:HW

//    vIO32WriteFldMulti_All(DDRPHY_REG_MISC_CG_CTRL0, P_Fld(0, MISC_CG_CTRL0_W_CHG_MEM)
//                | P_Fld(1, MISC_CG_CTRL0_CLK_MEM_SEL));//[5:4] mem_ck mux: 2'b01: memory clock, [0]: change memory clock

//    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_CG_CTRL0, 1, MISC_CG_CTRL0_W_CHG_MEM);//change clock freq

//    mcSHOW_DRM_MSG(("DELAY 1 us\n"));
//    mcDELAY_US(1);

//    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_CG_CTRL0, 0, MISC_CG_CTRL0_W_CHG_MEM);//disable memory clock change
    LP_Mode_DRM_Setting();


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
    //mcDELAY_US(1);
    //vIO32WriteFldAlign_All(DDRPHY_SHU_B0_DLL0, 0x1, SHU_B0_DLL0_RG_ARDLL_PHDET_EN_B0_SHU);//2nd DLL enable
    //vIO32WriteFldAlign_All(DDRPHY_SHU_B1_DLL0, 0x1, SHU_B1_DLL0_RG_ARDLL_PHDET_EN_B1_SHU);//2nd DLL enable
    //mcDELAY_US(1);
#else
    vIO32WriteFldAlign_All(DDRPHY_CA_DLL_ARPI2, 0x1, CA_DLL_ARPI2_RG_ARDLL_PHDET_EN_CA);//Since CHA CA and CHB CA are DLL master
    mcDELAY_US(1);
    vIO32WriteFldAlign_All(DDRPHY_B0_DLL_ARPI2, 0x1, B0_DLL_ARPI2_RG_ARDLL_PHDET_EN_B0);//2nd DLL enable
    vIO32WriteFldAlign_All(DDRPHY_B1_DLL_ARPI2, 0x1, B1_DLL_ARPI2_RG_ARDLL_PHDET_EN_B1);//2nd DLL enable
    mcDELAY_US(1);
#endif
#else //DLL_ASYNC_MODE
#if ENABLE_MCK8X_MODE
    //vIO32WriteFldAlign(DDRPHY_SHU_CA_DLL0, 0x1, SHU_CA_DLL0_RG_ARDLL_PHDET_EN_CA_SHU);//Since CHA CA is DLL master
    //mcDELAY_US(1);
    //vIO32WriteFldAlign(DDRPHY_SHU_CA_DLL0 + SHIFT_TO_CHB_ADDR, 0x1, SHU_CA_DLL0_RG_ARDLL_PHDET_EN_CA_SHU);//2nd DLL enable
    //vIO32WriteFldAlign_All(DDRPHY_SHU_B0_DLL0, 0x1, SHU_B0_DLL0_RG_ARDLL_PHDET_EN_B0_SHU);//2nd DLL enable
    //vIO32WriteFldAlign_All(DDRPHY_SHU_B1_DLL0, 0x1, SHU_B1_DLL0_RG_ARDLL_PHDET_EN_B1_SHU);//2nd DLL enable
    //mcDELAY_US(1);
#else
    vIO32WriteFldAlign(DDRPHY_CA_DLL_ARPI2, 0x1, CA_DLL_ARPI2_RG_ARDLL_PHDET_EN_CA);//Since CHA CA is DLL master
    mcDELAY_US(1);
    vIO32WriteFldAlign(DDRPHY_CA_DLL_ARPI2 + SHIFT_TO_CHB_ADDR, 0x1, CA_DLL_ARPI2_RG_ARDLL_PHDET_EN_CA);//2nd DLL enable
    vIO32WriteFldAlign_All(DDRPHY_B0_DLL_ARPI2, 0x1, B0_DLL_ARPI2_RG_ARDLL_PHDET_EN_B0);//2nd DLL enable
    vIO32WriteFldAlign_All(DDRPHY_B1_DLL_ARPI2, 0x1, B1_DLL_ARPI2_RG_ARDLL_PHDET_EN_B1);//2nd DLL enable
    mcDELAY_US(1);
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
    mcDELAY_US(1);

    vIO32WriteFldAlign(DDRPHY_REG_SHU_CA_DLL1 + SHIFT_TO_CHB_ADDR, 0x1, SHU_CA_DLL1_RG_ARDLL_PHDET_EN_CA);
#if (CHANNEL_NUM > 2)
    vIO32WriteFldAlign(DDRPHY_REG_SHU_CA_DLL1 + SHIFT_TO_CHD_ADDR, 0x1, SHU_CA_DLL1_RG_ARDLL_PHDET_EN_CA);
#endif

    mcSHOW_DRM_MSG(("DELAY 1 us\n"));
    mcDELAY_US(1);

    vIO32WriteFldAlign_All(DDRPHY_REG_SHU_B0_DLL1, 0x1, SHU_B0_DLL1_RG_ARDLL_PHDET_EN_B0);
    vIO32WriteFldAlign_All(DDRPHY_REG_SHU_B1_DLL1, 0x1, SHU_B1_DLL1_RG_ARDLL_PHDET_EN_B1);

    mcSHOW_DRM_MSG(("DELAY 1 us\n"));
    mcDELAY_US(1);

    //
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
    mcDELAY_US(1);


//     mcSHOW_DRM_MSG(("RG_*PLL_EN=1\n"));
    vIO32WriteFldAlign_All(DDRPHY_REG_PHYPLL0, 0x0, PHYPLL0_RG_RPHYPLL_EN);//disable DDRPHY PHYPLL enable, RG_*PLL_EN=0; Since there is only 1 PLL, only control CHA //Follow Petrus_DDR_reserved C code_20190123.docx
    vIO32WriteFldAlign_All(DDRPHY_REG_CLRPLL0, 0x0, CLRPLL0_RG_RCLRPLL_EN);//disable DDRPHY CLRPLL enable, RG_*PLL_EN=0; Since there is only 1 PLL, only control CHA //Follow Petrus_DDR_reserved C code_20190123.docx

//    vIO32WriteFldMulti_All(DDRPHY_REG_MISC_CLK_CTRL , P_Fld(1, MISC_CLK_CTRL_DVFS_CLK_MEM_SEL) );//[1]: 0:SW 1:HW

//    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_CG_CTRL0, 1, MISC_CG_CTRL0_W_CHG_MEM);//change clock freq

//    mcSHOW_DRM_MSG(("DELAY 1 us\n"));
//    mcDELAY_US(1);

//    mcSHOW_DRM_MSG(("disable memory clock change\n"));
//    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_CG_CTRL0, 0, MISC_CG_CTRL0_W_CHG_MEM);//

#if ENABLE_TX_REBASE_ODT_WA
    TxReadBaseODTWA(p, SRAM_SHU0);//for low power enter DRM fail
#endif

//    ASVA5_8_CSCA_Pull_Down_EN();
    DramcBroadcastOnOff(backup_broadcast);
    //DDRPhyFreqMeter();
    mcSHOW_DRM_MSG(("End of Dramc_DDR_Reserved_Mode_setting \n"));

}

static U32 TA2_Test_After_DRM(DRAMC_CTX_T * p)//Not change test range of DRM
{
    DRAM_CHANNEL_T channel_bak = p->channel;
    DRAM_RANK_T rank_bak = p->rank;
    U32 bit_error = 0;

    TA2_Test_Run_Time_HW_Write(p, ENABLE);
    bit_error = TA2_Test_Run_Time_HW_Status(p);

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

//    if(p->support_channel_num !=2) mcSHOW_DRM_MSG(("Set DRAMC Dual Channel\n"));//FOR_DV_SIMULATION_USE, never delet
//    p->support_channel_num = 2;//FOR_DV_SIMULATION_USE, never delet
    mcSHOW_DRM_MSG(("Start Dramc_DDR_Reserved_Mode_AfterSR, dram type =%x \n", u4DramType));

    DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);	
#if !EXIT_DRM_WITH_SAME_FREQ     
    u4LP4_MR13FSP = u4IO32ReadFldAlign(DRAMC_REG_SHU_HWSET_VRCG, SHU_HWSET_VRCG_HWSET_VRCG_OP);//Use the second MRW{MR13 VRCG=0} of DFS as reference to meet SPEC tFC
    u4LP4_MR2RLWL = u4IO32ReadFldAlign(DRAMC_REG_SHU_HWSET_MR2, SHU_HWSET_MR2_HWSET_MR2_OP);

    //CKE high
    CKEFixOnOff(p, CKE_WRITE_TO_ALL_RANK, CKE_FIXON, CKE_WRITE_TO_ALL_CHANNEL);//tVRCG_DISABLE only allow Deselect command
    for(u1channel = CHANNEL_A; u1channel < p->support_channel_num; u1channel++)
    {
        vSetPHY2ChannelMapping(p, u1channel);

        DramcModeRegWriteByRank(p, u1set_mrsrk, 13, u4LP4_MR13FSP | (0x1 << 3));
        mcDELAY_US(1);//delay for tFC_Long = 250 ns
        DramcModeRegWriteByRank(p, u1set_mrsrk, 13, u4LP4_MR13FSP);//set VRCG = 0
        DramcModeRegWriteByRank(p, u1set_mrsrk,  2, u4LP4_MR2RLWL);
        DramcModeRegWriteByRank(p, u1set_mrsrk, 17, 0);//R1 lost data WA, set R0 R1 both for safe{On charging mode, system may issue MR17=0xFF in R1}
    }
    vSetPHY2ChannelMapping(p, eOriChannel);
    CKEFixOnOff(p, CKE_WRITE_TO_ALL_RANK, CKE_DYNAMIC, CKE_WRITE_TO_ALL_CHANNEL);
    
#endif

#ifdef HW_GATING
    mcSHOW_DRM_MSG(("Enable HW Gating tracking for gating tracking fifo mode\n"));
    DramcHWGatingOnOff(p,1);
    Enable_Gating_Tracking(p,0);
#endif

    mcSHOW_DRM_MSG(("Restore regs\n"));
    DramcRestoreRegisters(p, u4ReserveRegBackupAddress, sizeof(u4ReserveRegBackupAddress)/sizeof(U32));


    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_CG_CTRL0, 0x0, MISC_CG_CTRL0_RG_CG_PHY_OFF_DIABLE);

//    mcDELAY_US(4);
//    mcDELAY_NS(119);
#if ENABLE_RX_TRACKING
    mcSHOW_DRM_MSG(("Enable RX tracking\n"));
    //*** WARNING***SA please change below write cmd to make sure write 2 MD32(Pe_trus)
    //*** WARNING***SA please change below write cmd to make sure write 1 MD32(Mar_gaux)
    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_1, CHAN_MODE, LPIF_LOW_POWER_CFG_1_DPHY_RXDLY_TRACK_EN);
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

#ifdef IMPEDANCE_TRACKING_ENABLE
    mcSHOW_DRM_MSG(("Enable IMP tracking, \n"));
    //SPM only handle CHA IMPCAL_IMPCAL_HW, DDR2400 into DDR reserve mode(IMPCAL_IMPCAL_HW CHA = 0 CHB = 1)
    //CHB NAO have been reset to 0, need do hand shake with CHA, but CHA IMP tracking is off --> dead lock
    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_IMPCAL, 1, MISC_IMPCAL_IMPCAL_HW);
#endif

#ifdef ZQCS_ENABLE_LP4
    mcSHOW_DRM_MSG(("Enable HW ZQ\n"));
    vIO32WriteFldAlign_All(DRAMC_REG_ZQ_SET1, 1, ZQ_SET1_ZQCALDISB);
#endif

    //u4value = u4IO32ReadFldAlign(DDRPHY_REG_MISC_SHU_CG_CTRL0, MISC_SHU_CG_CTRL0_R_PHY_MCK_CG_CTRL);
    //vIO32WriteFldAlign_All(DDRPHY_REG_MISC_SHU_CG_CTRL0,u4value & ~(0x22000000),  MISC_SHU_CG_CTRL0_R_PHY_MCK_CG_CTRL);

    mcSHOW_DRM_MSG(("Enable DDRPHY dynamic clock gating\n"));
    vIO32WriteFldAlign_All(DRAMC_REG_DRAMC_PD_CTRL, 1, DRAMC_PD_CTRL_PHYCLKDYNGEN);//enable DDRPHY dynamic clock gating

    mcSHOW_DRM_MSG(("Disable BLCOK_ALE\n"));
    vIO32WriteFldAlign_All(DRAMC_REG_DRAMCTRL, 0,  DRAMCTRL_ALEBLOCK);

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
    mcSHOW_DRM_MSG(("End Dramc_DDR_Reserved_Mode_AfterSR, dram type =%x \n", u4DramType));
    return;
}


//----------------------------------------
// Auto Gen Code -- START
//----------------------------------------
void patch_to_before_k(DRAMC_CTX_T *p)
{
    vIO32Write4B_All(DRAMC_REG_DDRCOMMON0, 0x80101);
    vIO32Write4B_All(DRAMC_REG_SA_RESERVE, 0x0);
    vIO32Write4B_All(DRAMC_REG_TEST2_A0, 0x100);
    vIO32Write4B_All(DRAMC_REG_TEST2_A2, 0x200);
    vIO32Write4B_All(DRAMC_REG_TEST2_A3, 0x12000880);
    vIO32Write4B_All(DRAMC_REG_TEST2_A4, 0x4000110D);
    vIO32Write4B_All(DRAMC_REG_SWCMD_CTRL0, 0xD18);
    vIO32Write4B_All(DRAMC_REG_SREF_DPD_CTRL, 0x589780);
    vIO32Write4B_All(DRAMC_REG_ACTIMING_CTRL, 0x200047);
    vIO32Write4B_All(DRAMC_REG_ZQ_SET1, 0x0);
    vIO32Write4B_All(DRAMC_REG_TX_TRACKING_SET0, 0x620000);
    vIO32Write4B_All(DRAMC_REG_DRAMC_PD_CTRL, 0x68);
    vIO32Write4B_All(DRAMC_REG_DCM_CTRL0, 0x20004);
    vIO32Write4B_All(DRAMC_REG_CKECTRL, 0x89504450);
    vIO32Write4B_All(DRAMC_REG_DVFS_CTRL0, 0x380720);
    vIO32Write4B_All(DRAMC_REG_SHUCTRL1, 0x1A);
    vIO32Write4B_All(DRAMC_REG_CMD_DEC_CTRL0, 0x110);
    vIO32Write4B_All(DRAMC_REG_HMR4, 0x304);
    vIO32Write4B_All(DRAMC_REG_TX_SET0, 0x1400050);
    vIO32Write4B_All(DRAMC_REG_TX_CG_SET0, 0x1800001F);
    vIO32Write4B_All(DRAMC_REG_RX_CG_SET0, 0xC0000000);
    vIO32Write4B_All(DRAMC_REG_DQSOSCR, 0x90E0080);
    vIO32Write4B_All(DRAMC_REG_MISCTL0, 0x81084003);
    vIO32Write4B_All(DRAMC_REG_CLKAR, 0x9347FFF);
    vIO32Write4B_All(DRAMC_REG_REFCTRL0, 0x25714000);
    vIO32Write4B_All(DRAMC_REG_REFCTRL1, 0x3202D);
    vIO32Write4B_All(DRAMC_REG_REF_BOUNCE1, 0xFFFF0504);
    vIO32Write4B_All(DRAMC_REG_REF_BOUNCE2, 0x1F);
    vIO32Write4B_All(DRAMC_REG_REFPEND1, 0x33355555);
    vIO32Write4B_All(DRAMC_REG_SCSMCTRL_CG, 0xC0000000);
    vIO32Write4B_All(DRAMC_REG_REFCTRL2, 0x41F);
    vIO32Write4B_All(DRAMC_REG_DCM_SUB_CTRL, 0x0);
    vIO32Write4B_All(DRAMC_REG_CBT_WLEV_CTRL5, 0x6F100);
    vIO32Write4B_All(DRAMC_REG_SELFREF_HWSAVE_FLAG, 0x0);
    vIO32Write4B_All(DDRPHY_REG_PHYPLL0, 0x80000000);
    vIO32Write4B_All(DDRPHY_REG_B0_DQ4, 0x24246E6E);
    vIO32Write4B_All(DDRPHY_REG_B0_PHY3, 0x0);
    vIO32Write4B_All(DDRPHY_REG_B1_DQ4, 0x24246E6E);
    vIO32Write4B_All(DDRPHY_REG_B1_PHY3, 0x0);
    vIO32Write4B_All(DDRPHY_REG_CA_LP_CTRL0, 0x1FFEA31E);
    vIO32Write4B_All(DDRPHY_REG_CA_CMD6, 0x200000);
    vIO32Write4B_All(DDRPHY_REG_MISC_STBCAL2, 0x401D0404);
    vIO32Write4B_All(DDRPHY_REG_MISC_CG_CTRL0, 0x200BFF10);
    vIO32Write4B_All(DDRPHY_REG_MISC_CG_CTRL2, 0x80600B6E);
    vIO32Write4B_All(DDRPHY_REG_MISC_CG_CTRL5, 0x0);
    vIO32Write4B_All(DDRPHY_REG_MISC_CG_CTRL7, 0x10800);
    vIO32Write4B_All(DDRPHY_REG_MISC_CLK_CTRL, 0x0);
    vIO32Write4B_All(DDRPHY_REG_MISC_PRE_TDQSCK1, 0x0);
    vIO32Write4B_All(DDRPHY_REG_MISC_RG_DFS_CTRL, 0x41);
    vIO32Write4B_All(DDRPHY_REG_MISC_DDR_RESERVE, 0x104040F);
    vIO32Write4B_All(DDRPHY_REG_MISC_IMPCAL, 0x40A3C28);
    vIO32Write4B_All(DDRPHY_REG_MISC_SRAM_DMA0, 0x735000);
    vIO32Write4B_All(DDRPHY_REG_MISC_DUTYSCAN1, 0x4808);
    vIO32Write4B_All(DDRPHY_REG_MISC_CTRL1, 0x8100328C);
    vIO32Write4B_All(DDRPHY_REG_MISC_CTRL3, 0x1D001100);
    vIO32Write4B_All(DDRPHY_REG_MISC_CTRL4, 0x0);
    vIO32Write4B_All(DDRPHY_REG_MISC_RX_AUTOK_CFG0, 0x8);

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
#endif

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
    mcSHOW_DRM_MSG((" @Darren, Fix rx tracking cause DDR3733 TA2 fail via ddr reserved mode\n"));
    vIO32WriteFldAlign_All(DDRPHY_REG_B0_DQ5, 0x0, B0_DQ5_RG_RX_ARDQS0_DVS_EN_B0);
    vIO32WriteFldAlign_All(DDRPHY_REG_B1_DQ5, 0x0, B1_DQ5_RG_RX_ARDQS0_DVS_EN_B1);

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
    vIO32WriteFldAlign_All(DDRPHY_REG_PHYPLL0, 0x1, PHYPLL0_RG_RPHYPLL_EN);//Use conf RG to enalbe PHYPLL
    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0, 0, LPIF_LOW_POWER_CFG_0_PHYPLL_MODE_SW);//PLL control back to RG
    vIO32WriteFldAlign_All(SPM_DRAMC_DPY_CLK_SW_CON_2, 0, SPM_DRAMC_DPY_CLK_SW_CON_2_SW_PHYPLL_MODE_SW);//PLL control back to RG
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
//int MustStop = 0;
void ETT_DRM(DRAMC_CTX_T *p) 
{
    U32 u4SramShuLevel;
    U32 u4ConfShuLevel;

    //After system reset shu level value will latch to RG:DDRPHY_MD32_REG_LPIF_DBG_LATCH8
    u4ConfShuLevel = u4IO32ReadFldAlign(DDRPHY_MD32_REG_LPIF_STATUS_10, LPIF_STATUS_10_DRAMC_DR_SHU_LEVEL);
    u4ConfShuLevel = u4ConfShuLevel & 0x3;
    u4SramShuLevel = u4IO32ReadFldAlign(DDRPHY_MD32_REG_LPIF_STATUS_10, LPIF_STATUS_10_DRAMC_DR_SHU_SRAM_LEVEL);
    u4SramShuLevel = u4SramShuLevel & 0xf;
    vSetDFSTable(p, get_FreqTbl_by_SRAMIndex(p, u4SramShuLevel));
    DDRPhyFreqSel(p, p->pDFSTable->freq_sel);
    mcSHOW_DRM_MSG(("@@@ In DRM with DDR[%d] conf_shuffle_level[%d] sram_shuffle_level[%d]\n", p->frequency << 1, u4ConfShuLevel, u4SramShuLevel));

    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0, CHAN_MODE, LPIF_LOW_POWER_CFG_0_EMI_CLK_OFF_REQ);//DRAM into SR

    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0, CHAN_MODE,LPIF_LOW_POWER_CFG_0_DMSUS_OFF);//DMSUS set 1

    Dramc_DDR_Reserved_Mode_setting();

    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0, 0 ,LPIF_LOW_POWER_CFG_0_DMSUS_OFF);//DMSUS set 0

    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0, 0 ,LPIF_LOW_POWER_CFG_0_EMI_CLK_OFF_REQ);//DRAM exit SR

    Dramc_DDR_Reserved_Mode_AfterSR();

//    Patch_ReK(p);
//    MustStop = 1;
//    Init_DRAM(TYPE_LPDDR4X, CBT_R0_R1_BYTE, NULL, NORMAL_USED);//CBT_R0_R1_BYTE since DRAM is H9HQ16AFAMMDAR
}
#endif
#endif

