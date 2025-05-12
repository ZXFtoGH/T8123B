//=============================================================================
//  Include Files                                                                                                                                                                                
//=============================================================================
#include "dramc_common.h"
#include "dramc_int_global.h"
#include "x_hal_io.h"
#include "emi.h"


//=============================================================================
//  Global Variables                                                                                                                                                                         
//=============================================================================
int global_which_test = 0;

#define mcSHOW_DBG_MSG_LPS mcSHOW_ERR_MSG
//#define MEASURE_POWER_WITH_ETT
#define TEST_CNR_OF_LOW_POWER_SCENARIO 1000
#define ETT_COMMAND_FOR_PASR_EXTEND 0
#define CHAN_MODE 3 //1_chan: 1 2_chan: 3 
#define LOW_POWER_STRESS_TEST_WITH_TA2 1

int LewisLogEnable = 0;
void Func_Emi_Off(DRAMC_CTX_T *p)
{
    int i = 0;
    U32 u4value = 0;
    //U32 u4SrAck = 3;//LP4: CHA CHB; LP3: CHA

    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0, CHAN_MODE, LPIF_LOW_POWER_CFG_0_EMI_CLK_OFF_REQ);
    for(i = 2; i > 0; i--)
    {
        mcDELAY_US(1);
        u4value = u4IO32ReadFldAlign(DDRPHY_MD32_REG_LPIF_STATUS_4, LPIF_STATUS_4_EMI_CLK_OFF_REQ_ACK);
        if((CHAN_MODE & u4value) == CHAN_MODE)
        {
            break;
        }
    }

    if(i == 0) //no issue Ack =>status = 1
    {
        mcSHOW_DBG_MSG_LPS(("!Wait no ack(Enter SR)[0x%X]\n", u4value));
        vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0, 0, LPIF_LOW_POWER_CFG_0_EMI_CLK_OFF_REQ);
    }
    return;
}


void Func_Emi_On(DRAMC_CTX_T *p)
{
    int i = 0;
    U32 u4value = 0;
    //U32 u4SrAck = 3;//LP4: CHA CHB; LP3: CHA

    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0, 0, LPIF_LOW_POWER_CFG_0_EMI_CLK_OFF_REQ);
    
    for(i = 2; i > 0; i--)
    {
        mcDELAY_US(1);
        u4value = u4IO32ReadFldAlign(DDRPHY_MD32_REG_LPIF_STATUS_4, LPIF_STATUS_4_EMI_CLK_OFF_REQ_ACK);
        if((CHAN_MODE & u4value) == 0)
        {
            break;
        }
    }
    if(i == 0) //no issue Ack =>status = 1
    {
        mcSHOW_DBG_MSG_LPS(("!Wait no ack(Leave SR)[0x%X]\n", u4value));
    }
}


#define RX_DLY_TRACK_WORKAROUND
#ifdef RX_DLY_TRACK_WORKAROUND
static unsigned int r_rx_dly_track_en_b0_bak, r_rx_dly_track_en_b1_bak;
static unsigned int r_rk0_rx_dly_ris_track_gate_ena_b0_bak, r_rk0_rx_dly_fal_track_gate_ena_b0_bak, r_rk0_dvs_mode_b0_bak;
static unsigned int r_rk0_rx_dly_ris_track_gate_ena_b1_bak, r_rk0_rx_dly_fal_track_gate_ena_b1_bak, r_rk0_dvs_mode_b1_bak;
static unsigned int r_rk1_rx_dly_ris_track_gate_ena_b0_bak, r_rk1_rx_dly_fal_track_gate_ena_b0_bak, r_rk1_dvs_mode_b0_bak;
static unsigned int r_rk1_rx_dly_ris_track_gate_ena_b1_bak, r_rk1_rx_dly_fal_track_gate_ena_b1_bak, r_rk1_dvs_mode_b1_bak;

static void RX_DLY_TRACK_EN_HIGH(DRAMC_CTX_T *p) {
    unsigned int rank_bak;
    unsigned int bc_bak;

    bc_bak = GetDramcBroadcast();
    DramcBroadcastOnOff(DRAMC_BROADCAST_ON);
    
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B0_RXDVS0), r_rx_dly_track_en_b0_bak, B0_RXDVS0_R_RX_DLY_TRACK_ENA_B0);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B1_RXDVS0), r_rx_dly_track_en_b1_bak, B1_RXDVS0_R_RX_DLY_TRACK_ENA_B1);

    rank_bak = u1GetRank(p);
    vSetRank(p, RANK_0);

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_RK_B0_RXDVS2),
        P_Fld(r_rk0_rx_dly_ris_track_gate_ena_b0_bak, RK_B0_RXDVS2_R_RK0_RX_DLY_RIS_TRACK_GATE_ENA_B0) |
        P_Fld(r_rk0_rx_dly_fal_track_gate_ena_b0_bak, RK_B0_RXDVS2_R_RK0_RX_DLY_FAL_TRACK_GATE_ENA_B0) |
        P_Fld(r_rk0_dvs_mode_b0_bak, RK_B0_RXDVS2_R_RK0_DVS_MODE_B0));

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_RK_B1_RXDVS2),
        P_Fld(r_rk0_rx_dly_ris_track_gate_ena_b1_bak, RK_B1_RXDVS2_R_RK0_RX_DLY_RIS_TRACK_GATE_ENA_B1) |
        P_Fld(r_rk0_rx_dly_fal_track_gate_ena_b1_bak, RK_B1_RXDVS2_R_RK0_RX_DLY_FAL_TRACK_GATE_ENA_B1) |
        P_Fld(r_rk0_dvs_mode_b1_bak, RK_B1_RXDVS2_R_RK0_DVS_MODE_B1));

    vSetRank(p, RANK_1);

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_RK_B0_RXDVS2),
        P_Fld(r_rk1_rx_dly_ris_track_gate_ena_b0_bak, RK_B0_RXDVS2_R_RK0_RX_DLY_RIS_TRACK_GATE_ENA_B0) |
        P_Fld(r_rk1_rx_dly_fal_track_gate_ena_b0_bak, RK_B0_RXDVS2_R_RK0_RX_DLY_FAL_TRACK_GATE_ENA_B0) |
        P_Fld(r_rk1_dvs_mode_b0_bak, RK_B0_RXDVS2_R_RK0_DVS_MODE_B0));

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_RK_B1_RXDVS2),
        P_Fld(r_rk1_rx_dly_ris_track_gate_ena_b1_bak, RK_B1_RXDVS2_R_RK0_RX_DLY_RIS_TRACK_GATE_ENA_B1) |
        P_Fld(r_rk1_rx_dly_fal_track_gate_ena_b1_bak, RK_B1_RXDVS2_R_RK0_RX_DLY_FAL_TRACK_GATE_ENA_B1) |
        P_Fld(r_rk1_dvs_mode_b1_bak, RK_B1_RXDVS2_R_RK0_DVS_MODE_B1));

    vSetRank(p, rank_bak);

    DramcBroadcastOnOff(bc_bak);
}

static void RX_DLY_TRACK_EN_LOW(DRAMC_CTX_T *p) {
    unsigned int bc_bak;
    unsigned int rank_bak;

    bc_bak = GetDramcBroadcast();
    DramcBroadcastOnOff(DRAMC_BROADCAST_ON);

    r_rx_dly_track_en_b0_bak = u4IO32ReadFldAlign(DDRPHY_REG_B0_RXDVS0, B0_RXDVS0_R_RX_DLY_TRACK_ENA_B0);
    r_rx_dly_track_en_b1_bak = u4IO32ReadFldAlign(DDRPHY_REG_B1_RXDVS0, B1_RXDVS0_R_RX_DLY_TRACK_ENA_B1);

    vIO32WriteFldAlign(DDRPHY_REG_B0_RXDVS0, 0, B0_RXDVS0_R_RX_DLY_TRACK_ENA_B0);
    vIO32WriteFldAlign(DDRPHY_REG_B1_RXDVS0, 0, B1_RXDVS0_R_RX_DLY_TRACK_ENA_B1);

    rank_bak = u1GetRank(p);
    vSetRank(p, RANK_0);

    r_rk0_rx_dly_ris_track_gate_ena_b0_bak = u4IO32ReadFldAlign(DDRPHY_REG_RK_B0_RXDVS2, RK_B0_RXDVS2_R_RK0_RX_DLY_RIS_TRACK_GATE_ENA_B0);
    r_rk0_rx_dly_fal_track_gate_ena_b0_bak = u4IO32ReadFldAlign(DDRPHY_REG_RK_B0_RXDVS2, RK_B0_RXDVS2_R_RK0_RX_DLY_FAL_TRACK_GATE_ENA_B0);
    r_rk0_dvs_mode_b0_bak = u4IO32ReadFldAlign(DDRPHY_REG_RK_B0_RXDVS2, RK_B0_RXDVS2_R_RK0_DVS_MODE_B0);

    vIO32WriteFldMulti(DDRPHY_REG_RK_B0_RXDVS2,
        P_Fld(0, RK_B0_RXDVS2_R_RK0_RX_DLY_RIS_TRACK_GATE_ENA_B0) |
        P_Fld(0, RK_B0_RXDVS2_R_RK0_RX_DLY_FAL_TRACK_GATE_ENA_B0) |
        P_Fld(0, RK_B0_RXDVS2_R_RK0_DVS_MODE_B0));

    r_rk0_rx_dly_ris_track_gate_ena_b1_bak = u4IO32ReadFldAlign(DDRPHY_REG_RK_B1_RXDVS2, RK_B1_RXDVS2_R_RK0_RX_DLY_RIS_TRACK_GATE_ENA_B1);
    r_rk0_rx_dly_fal_track_gate_ena_b1_bak = u4IO32ReadFldAlign(DDRPHY_REG_RK_B1_RXDVS2, RK_B1_RXDVS2_R_RK0_RX_DLY_FAL_TRACK_GATE_ENA_B1);
    r_rk0_dvs_mode_b1_bak = u4IO32ReadFldAlign(DDRPHY_REG_RK_B1_RXDVS2, RK_B1_RXDVS2_R_RK0_DVS_MODE_B1);

    vIO32WriteFldMulti(DDRPHY_REG_RK_B1_RXDVS2,
        P_Fld(0, RK_B1_RXDVS2_R_RK0_RX_DLY_RIS_TRACK_GATE_ENA_B1) |
        P_Fld(0, RK_B1_RXDVS2_R_RK0_RX_DLY_FAL_TRACK_GATE_ENA_B1) |
        P_Fld(0, RK_B1_RXDVS2_R_RK0_DVS_MODE_B1));

    vSetRank(p, RANK_1);

    r_rk1_rx_dly_ris_track_gate_ena_b0_bak = u4IO32ReadFldAlign(DDRPHY_REG_RK_B0_RXDVS2, RK_B0_RXDVS2_R_RK0_RX_DLY_RIS_TRACK_GATE_ENA_B0);
    r_rk1_rx_dly_fal_track_gate_ena_b0_bak = u4IO32ReadFldAlign(DDRPHY_REG_RK_B0_RXDVS2, RK_B0_RXDVS2_R_RK0_RX_DLY_FAL_TRACK_GATE_ENA_B0);
    r_rk1_dvs_mode_b0_bak = u4IO32ReadFldAlign(DDRPHY_REG_RK_B0_RXDVS2, RK_B0_RXDVS2_R_RK0_DVS_MODE_B0);

    vIO32WriteFldMulti(DDRPHY_REG_RK_B0_RXDVS2,
        P_Fld(0, RK_B0_RXDVS2_R_RK0_RX_DLY_RIS_TRACK_GATE_ENA_B0) |
        P_Fld(0, RK_B0_RXDVS2_R_RK0_RX_DLY_FAL_TRACK_GATE_ENA_B0) |
        P_Fld(0, RK_B0_RXDVS2_R_RK0_DVS_MODE_B0));

    r_rk1_rx_dly_ris_track_gate_ena_b1_bak = u4IO32ReadFldAlign(DDRPHY_REG_RK_B1_RXDVS2, RK_B1_RXDVS2_R_RK0_RX_DLY_RIS_TRACK_GATE_ENA_B1);
    r_rk1_rx_dly_fal_track_gate_ena_b1_bak = u4IO32ReadFldAlign(DDRPHY_REG_RK_B1_RXDVS2, RK_B1_RXDVS2_R_RK0_RX_DLY_FAL_TRACK_GATE_ENA_B1);
    r_rk1_dvs_mode_b1_bak = u4IO32ReadFldAlign(DDRPHY_REG_RK_B1_RXDVS2, RK_B1_RXDVS2_R_RK0_DVS_MODE_B1);

    vIO32WriteFldMulti(DDRPHY_REG_RK_B1_RXDVS2,
        P_Fld(0, RK_B1_RXDVS2_R_RK0_RX_DLY_RIS_TRACK_GATE_ENA_B1) |
        P_Fld(0, RK_B1_RXDVS2_R_RK0_RX_DLY_FAL_TRACK_GATE_ENA_B1) |
        P_Fld(0, RK_B1_RXDVS2_R_RK0_DVS_MODE_B1));

    DramcBroadcastOnOff(bc_bak);
}
#endif


unsigned int mck_openloop_mode_sel_bak;
static void OpenLoopMode_1(DRAMC_CTX_T *p)
{
    unsigned int ch_bak, shu_bak, bc_bak;
    U32 tmp;
    if (mck_openloop_mode_sel_bak) 
    {
        shu_bak = p->ShuRGAccessIdx;

        bc_bak = GetDramcBroadcast();
        DramcBroadcastOnOff(DRAMC_BROADCAST_ON);

        tmp = u4IO32ReadFldAlign(DDRPHY_MD32_REG_LPIF_STATUS_10, LPIF_STATUS_10_DRAMC_DR_SHU_LEVEL);

        p->ShuRGAccessIdx = (tmp) ? DRAM_DFS_REG_SHU1 : DRAM_DFS_REG_SHU0;

        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_MISC_CLK_CTRL0), mck_openloop_mode_sel_bak, SHU_MISC_CLK_CTRL0_M_CK_OPENLOOP_MODE_SEL);
        tmp = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_REG_SHU_MISC_CLK_CTRL0));

        DramcBroadcastOnOff(bc_bak);
        p->ShuRGAccessIdx = shu_bak;
    }
}


static void ChangeToMCK_1(DRAMC_CTX_T *p, unsigned int mode)
{
    unsigned int ch_bak, shu_bak, bc_bak;
    U32 tmp;

    shu_bak = p->ShuRGAccessIdx;
    if (mck_openloop_mode_sel_bak) 
    {

        vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0, mode, LPIF_LOW_POWER_CFG_0_DDRPHY_FB_CK_EN);

        mcDELAY_US(1);

        bc_bak = GetDramcBroadcast();
        DramcBroadcastOnOff(DRAMC_BROADCAST_ON);

        tmp = u4IO32ReadFldAlign(DDRPHY_MD32_REG_LPIF_STATUS_10, LPIF_STATUS_10_DRAMC_DR_SHU_LEVEL);

        p->ShuRGAccessIdx = (tmp) ? DRAM_DFS_REG_SHU1 : DRAM_DFS_REG_SHU0;

        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_MISC_CLK_CTRL0), mck_openloop_mode_sel_bak, SHU_MISC_CLK_CTRL0_M_CK_OPENLOOP_MODE_SEL);

        tmp = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_REG_SHU_MISC_CLK_CTRL0));

        DramcBroadcastOnOff(bc_bak);
        p->ShuRGAccessIdx = shu_bak;

        vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0, 0, LPIF_LOW_POWER_CFG_0_DDRPHY_FB_CK_EN);
        
        mcDELAY_US(1);
    }
}


static void ChangeToMCK_2(DRAMC_CTX_T *p, unsigned int mode)
{
    unsigned int ch_bak, bc_bak;
    U32 tmp;

    ch_bak = vGetPHY2ChannelMapping(p);
    vSetPHY2ChannelMapping(p, CHANNEL_A);

    if (mode == 0x3) {
        bc_bak = GetDramcBroadcast();
        DramcBroadcastOnOff(DRAMC_BROADCAST_ON);
    } else if (mode == 0x2) {
        vSetPHY2ChannelMapping(p, CHANNEL_B);
    }

    if (mck_openloop_mode_sel_bak) {
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CG_CTRL0), 0x1, MISC_CG_CTRL0_RESERVED_MISC_CG_CTRL0_BIT3_1);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CG_CTRL0), 0x0, MISC_CG_CTRL0_RESERVED_MISC_CG_CTRL0_BIT3_1);

        tmp = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CG_CTRL0));

        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CG_CTRL0), 0x1, MISC_CG_CTRL0_RESERVED_MISC_CG_CTRL0_BIT7);
    }

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CG_CTRL0), 0x1, MISC_CG_CTRL0_CLK_MEM_SEL);

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CLK_CTRL), 0x0, MISC_CLK_CTRL_DVFS_MEM_CK_MUX_SEL_MODE);

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CG_CTRL0), 0x1, MISC_CG_CTRL0_W_CHG_MEM);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CG_CTRL0), 0x0, MISC_CG_CTRL0_W_CHG_MEM);

    mcDELAY_US(1);

    DramcBroadcastOnOff(bc_bak);
    vSetPHY2ChannelMapping(p, ch_bak);
}


static void ChangeToMCK_3(DRAMC_CTX_T *p, unsigned int mode)
{
    unsigned int ch_bak, bc_bak;
    U32 tmp;

    ch_bak = vGetPHY2ChannelMapping(p);
    vSetPHY2ChannelMapping(p, CHANNEL_A);

    if (mode == 0x3) {
        bc_bak = GetDramcBroadcast();
        DramcBroadcastOnOff(DRAMC_BROADCAST_ON);
    } else if (mode == 0x2) {
        vSetPHY2ChannelMapping(p, CHANNEL_B);
    }
   
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CG_CTRL0),
            P_Fld(0x1, MISC_CG_CTRL0_RG_CG_PHY_OFF_DIABLE) |
            P_Fld(0x1, MISC_CG_CTRL0_RG_CG_RX_COMB0_OFF_DISABLE) |
            P_Fld(0x1, MISC_CG_CTRL0_RG_CG_RX_COMB1_OFF_DISABLE));

    tmp = u4IO32ReadFldAlign(DDRPHY_MD32_REG_LPIF_STATUS_10, LPIF_STATUS_10_DRAMC_DR_SHU_LEVEL);
    p->ShuRGAccessIdx = (tmp) ? DRAM_DFS_REG_SHU1 : DRAM_DFS_REG_SHU0;

    tmp = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_STBCAL), MISC_SHU_STBCAL_STBCALEN);

    if (tmp) {
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_STBCAL), 0x0, MISC_SHU_STBCAL_STBCALEN);
        mcDELAY_US(1);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_STBCAL), 0x1, MISC_SHU_STBCAL_STBCALEN);
    }

    tmp = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_STBCAL), MISC_SHU_STBCAL_STB_SELPHCALEN);

    if (tmp) {
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_STBCAL), 0x0, MISC_SHU_STBCAL_STB_SELPHCALEN);
        mcDELAY_US(1);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_STBCAL), 0x1, MISC_SHU_STBCAL_STB_SELPHCALEN);
    }

#ifdef RX_DLY_TRACK_WORKAROUND
    RX_DLY_TRACK_EN_HIGH(p);
    mcDELAY_US(1);
#endif

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CG_CTRL0),
            P_Fld(0x0, MISC_CG_CTRL0_RG_CG_PHY_OFF_DIABLE) |
            P_Fld(0x0, MISC_CG_CTRL0_RG_CG_RX_COMB0_OFF_DISABLE) |
            P_Fld(0x0, MISC_CG_CTRL0_RG_CG_RX_COMB1_OFF_DISABLE));

    u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CG_CTRL0));

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DRAMCTRL), 0x0, DRAMCTRL_ALEBLOCK);

    if (mode == 0x3) {
        DramcBroadcastOnOff(bc_bak);
    }

    vSetPHY2ChannelMapping(p, ch_bak);
}


static void ChangeTo26M(DRAMC_CTX_T *p, unsigned int mode)
{
    unsigned int ch_bak, shu_bak, bc_bak;
    U32 tmp;

    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0, mode, LPIF_LOW_POWER_CFG_0_DDRPHY_FB_CK_EN);

    ch_bak = vGetPHY2ChannelMapping(p);
    vSetPHY2ChannelMapping(p, CHANNEL_A);
    shu_bak = p->ShuRGAccessIdx;

    if (mode == 0x3) {
        bc_bak = GetDramcBroadcast();
        DramcBroadcastOnOff(DRAMC_BROADCAST_ON);
    } else if (mode == 0x2) {
        vSetPHY2ChannelMapping(p, CHANNEL_B);
    }

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DRAMCTRL), 0x1, DRAMCTRL_ALEBLOCK);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CG_CTRL0), 0x0, MISC_CG_CTRL0_CLK_MEM_SEL);

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CLK_CTRL),
        P_Fld(0x0, MISC_CLK_CTRL_DVFS_MEM_CK_MUX_SEL) |
        P_Fld(0x2, MISC_CLK_CTRL_DVFS_MEM_CK_MUX_SEL_MODE));

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CG_CTRL0), 0x1, MISC_CG_CTRL0_W_CHG_MEM);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CG_CTRL0), 0x0, MISC_CG_CTRL0_W_CHG_MEM);

    tmp = u4IO32ReadFldAlign(DDRPHY_MD32_REG_LPIF_STATUS_10, LPIF_STATUS_10_DRAMC_DR_SHU_LEVEL);

    p->ShuRGAccessIdx = (tmp) ? DRAM_DFS_REG_SHU1 : DRAM_DFS_REG_SHU0;

    mck_openloop_mode_sel_bak = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_MISC_CLK_CTRL0), SHU_MISC_CLK_CTRL0_M_CK_OPENLOOP_MODE_SEL);
    tmp = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_REG_SHU_MISC_CLK_CTRL0));

    if (mck_openloop_mode_sel_bak) {
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CG_CTRL0), 0x0, MISC_CG_CTRL0_RESERVED_MISC_CG_CTRL0_BIT7);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_MISC_CLK_CTRL0), 0x0, SHU_MISC_CLK_CTRL0_M_CK_OPENLOOP_MODE_SEL);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CG_CTRL0), 0x1, MISC_CG_CTRL0_RESERVED_MISC_CG_CTRL0_BIT3_1);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CG_CTRL0), 0x0, MISC_CG_CTRL0_RESERVED_MISC_CG_CTRL0_BIT3_1);

        tmp = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CG_CTRL0));
    }

    if (mode == 0x3) {
        DramcBroadcastOnOff(bc_bak);
    }

    p->ShuRGAccessIdx = shu_bak;
    vSetPHY2ChannelMapping(p, ch_bak);

    mcDELAY_US(1);
    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0, 0, LPIF_LOW_POWER_CFG_0_DDRPHY_FB_CK_EN);
}


//S-idle --> S1 {DLL off and CG[mem_ck mck_8x MPDIV] enable}
static void FUNC_DDRPHY_DLL_OFF_INFRA_CG_EN_ENTER(DRAMC_CTX_T *p, int intoS0)
{
    //Pause frequency hopping; {no freq hopping}
    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_2, 1, LPIF_LOW_POWER_CFG_2_FHC_PAUSE_MPLL);

#ifdef CHECK_CONF_FOR_DEBUG
    mcSHOW_DBG_MSG_LPS(("[Enter]CHECK_CONF_FOR_DEBUG\n"));
    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_CG_CTRL0, 0, MISC_CG_CTRL0_CLK_MEM_SEL);//Switch clk to 26MHz
    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_CG_CTRL0, 1, MISC_CG_CTRL0_W_CHG_MEM);
    mcDELAY_US(1);
    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_CG_CTRL0, 0, MISC_CG_CTRL0_W_CHG_MEM);
    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0, CHAN_MODE, LPIF_LOW_POWER_CFG_0_DDRPHY_FB_CK_EN);
#endif

    if(intoS0)
    {
        RX_DLY_TRACK_EN_LOW(p);
        mcDELAY_US(1);
    }
    //DDRPHY switch to SPM control
    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_2, CHAN_MODE, LPIF_LOW_POWER_CFG_2_DPY_MODE_SW);

    //IO enter suspend{tie to specific value, SR state}
    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0, CHAN_MODE, LPIF_LOW_POWER_CFG_0_DMSUS_OFF);

    //MEM clk off
    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0, CHAN_MODE, LPIF_LOW_POWER_CFG_0_MEM_CK_OFF);

    //DLL disable
    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0, 0, LPIF_LOW_POWER_CFG_0_DPY_2ND_DLL_EN);
    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0, 0, LPIF_LOW_POWER_CFG_0_DPY_DLL_EN);
    
    if(intoS0)
    {
        ChangeTo26M(p, CHAN_MODE);
    }
    
    //All PI disable & CG enable{Aphy cg enable, Not do PI delay Calculation}
    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0, 0, LPIF_LOW_POWER_CFG_0_DPY_DLL_CK_EN);
#if ENABLE_MCK8X_MODE
    //SC_MIDPI_EN = 0(NEW 8X seq)
    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_3, 0, LPIF_LOW_POWER_CFG_3_DPY_MIDPI_EN);
    //SC_PI_RESETB = 0(NEW 8X seq)
    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_3, 0, LPIF_LOW_POWER_CFG_3_DPY_PI_RESETB_EN);
    //SC_MCK8X_EN = 0(NEW 8X seq)
    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_3, 0, LPIF_LOW_POWER_CFG_3_DPY_MCK8X_EN);
#endif
    //VREF disable, MCK8X CG enable and  MIDPI disable{MIDPI: PI of DLL}
    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0, 0, LPIF_LOW_POWER_CFG_0_DPY_VREF_EN);

    return;
}


//S1 --> S-idle {Exit DLL off and CG[mem_ck mck_8x MPDIV] enable}
static void FUNC_DDRPHY_DLL_OFF_INFRA_CG_EN_EXIT(DRAMC_CTX_T *p, int fromS0)
{
    if(fromS0)
    {
        ChangeToMCK_1(p, CHAN_MODE);
    }
    //VREF enable, MCK8X CG disable and MIDPI enable
    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0, CHAN_MODE, LPIF_LOW_POWER_CFG_0_DPY_VREF_EN);
#if ENABLE_MCK8X_MODE
    //SC_MIDPI_EN = 1(NEW 8X seq)
    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_3, CHAN_MODE, LPIF_LOW_POWER_CFG_3_DPY_MCK8X_EN);
    //SC_PI_RESETB = 1(NEW 8X seq)
    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_3, CHAN_MODE, LPIF_LOW_POWER_CFG_3_DPY_PI_RESETB_EN);
    //SC_MCK8X_EN = 1(NEW 8X seq)
    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_3, CHAN_MODE, LPIF_LOW_POWER_CFG_3_DPY_MIDPI_EN);
#endif
    //All PI enable & CG disable
    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0, CHAN_MODE, LPIF_LOW_POWER_CFG_0_DPY_DLL_CK_EN);

    //Set TOP feedback MCK to full frequency
    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0, CHAN_MODE, LPIF_LOW_POWER_CFG_0_DDRPHY_FB_CK_EN);
    if(fromS0)
    {
        ChangeToMCK_2(p, CHAN_MODE);
    }
    else
    {
        mcDELAY_US(1);
    }

    //1st DLL enable
    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0, CHAN_MODE, LPIF_LOW_POWER_CFG_0_DPY_DLL_EN);
    //wait 70 MCK
    mcDELAY_US(1);
    //2nd DLL enable
    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0, CHAN_MODE, LPIF_LOW_POWER_CFG_0_DPY_2ND_DLL_EN);
    //wait 90 MCK
    mcDELAY_US(1);

    //DDRPHY switch to SW control
    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_2, 0, LPIF_LOW_POWER_CFG_2_DPY_MODE_SW);

#ifdef CHECK_CONF_FOR_DEBUG
    mcSHOW_DBG_MSG_LPS(("CHECK_CONF_FOR_DEBUG swith to mem ck\n"));
    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_CG_CTRL0, 1, MISC_CG_CTRL0_CLK_MEM_SEL);//Switch clk to mem_ck
    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_CG_CTRL0, 1, MISC_CG_CTRL0_W_CHG_MEM);
    mcDELAY_US(1);
    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_CG_CTRL0, 0, MISC_CG_CTRL0_W_CHG_MEM);
#endif

    //mem_ck CG disable
    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0, 0, LPIF_LOW_POWER_CFG_0_MEM_CK_OFF);
    //Wait 150ns
    mcDELAY_US(1);

    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_3, CHAN_MODE, LPIF_LOW_POWER_CFG_3_DPY_CS_PULL_DN_EN);
    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_3, CHAN_MODE, LPIF_LOW_POWER_CFG_3_DPY_CA_PULL_DN_EN);

    //IO exit suspend
    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0, 0, LPIF_LOW_POWER_CFG_0_DMSUS_OFF);

    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_3, CHAN_MODE, LPIF_LOW_POWER_CFG_3_DPY_PICG_FREE);
    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_3, 0, LPIF_LOW_POWER_CFG_3_DPY_PICG_FREE);
    
    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_3, 0, LPIF_LOW_POWER_CFG_3_DPY_CS_PULL_DN_EN);
    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_3, 0, LPIF_LOW_POWER_CFG_3_DPY_CA_PULL_DN_EN);

    if(fromS0)
    {
        ChangeToMCK_3(p, CHAN_MODE);
    }
    //Release frequency hopping
    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_2, 0, LPIF_LOW_POWER_CFG_2_FHC_PAUSE_MPLL);

#if TDQSCK_PRECALCULATION_FOR_DVFS
    //Joe@20171030: In If use SPM_DRAMC_DPY_CLK_SW_CON2, you need to set SPM_DRAMC_DPY_CLK_SW_CON_SEL2 to 0.So we use VAL0 
    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_1, CHAN_MODE, LPIF_LOW_POWER_CFG_1_DPHY_PRECAL_UP);
    //wait 80ns
    mcDELAY_US(1);
    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_1, 0, LPIF_LOW_POWER_CFG_1_DPHY_PRECAL_UP);
#endif

    //Set TOP feedback MCK to divided frequency
    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0, 0, LPIF_LOW_POWER_CFG_0_DDRPHY_FB_CK_EN);

    //Release self-refresh request
    Func_Emi_On(p);//868 only 1 channel

    return;
}


#define PWR_EN_VALUE CHAN_MODE //1: one channel 3: 2 channel
int spm_mtcmos_ctrl_dpy_ch0_apmcu(DRAMC_CTX_T *p, int state)
{
    int err = 0;
    if (state == 0) 
    {   //Power Down
        vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_MISC_CTL_1, PWR_EN_VALUE, LPIF_MISC_CTL_1_PWR_ISO);
        vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_MISC_CTL_1, 0x0, LPIF_MISC_CTL_1_PWR_RST_B);
        vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_MISC_CTL_1, 0x0, LPIF_MISC_CTL_1_PWR_ON_2ND);
        vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_MISC_CTL_1, 0x0, LPIF_MISC_CTL_1_PWR_ON);
    }
    else 
    {   //Power On
        vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_MISC_CTL_1, PWR_EN_VALUE, LPIF_MISC_CTL_1_PWR_ON);
        mcDELAY_US(1);
        vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_MISC_CTL_1, PWR_EN_VALUE, LPIF_MISC_CTL_1_PWR_ON_2ND);
        vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_MISC_CTL_1, 0x0, LPIF_MISC_CTL_1_PWR_ISO);
        mcDELAY_US(1);
        vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_MISC_CTL_1, PWR_EN_VALUE, LPIF_MISC_CTL_1_PWR_RST_B);
    }
    return err;
}


//S1--> S0{PLL disable, PHY MTCMOS off }
static void FUNC_DDRPHY_INFRA_MTCMOS_OFF_ENTER(DRAMC_CTX_T *p)
{
    //PHYPLL off
    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0, 0x0, LPIF_LOW_POWER_CFG_0_PHYPLL_EN);

    //SRAM off
    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_MISC_CTL_1, 0xff, LPIF_MISC_CTL_1_PWR_SRAM_PDN);

    //MPLL off
    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_2, 0x1, LPIF_LOW_POWER_CFG_2_MPLLOUT_OFF);
    mcDELAY_US(1);
    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_2, 0x1, LPIF_LOW_POWER_CFG_2_MPLL_S_OFF);
    mcDELAY_US(1);
    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_MISC_CTL_1, 0x1, LPIF_MISC_CTL_1_MPLL_OFF);
    mcDELAY_US(1);

    /*TINFO="DRAM : DPY power down, DRAMC_DPY_CLK_SW_CON_SEL2[30:28] = 3'b111, means turn off CHA, BCD will also turn off  "*/ 
    spm_mtcmos_ctrl_dpy_ch0_apmcu(p, 0);
    return;
}


//TODO: S1--> S0{PLL enable, PHY MTCMOS off exit}
static void FUNC_DDRPHY_INFRA_MTCMOS_OFF_EXIT(DRAMC_CTX_T *p)
{
    spm_mtcmos_ctrl_dpy_ch0_apmcu(p, 1);
    //vIO32WriteFldAlign(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_2, 0, LPIF_LOW_POWER_CFG_2_RESERVED_X8_7);//LewisToDo, MPLLOUT_OFF
    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_MISC_CTL_1, 0, LPIF_MISC_CTL_1_MPLL_OFF);//LewisToDo

    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_2, 0, LPIF_LOW_POWER_CFG_2_MPLL_S_OFF);
    mcDELAY_US(20);
    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_2, 0, LPIF_LOW_POWER_CFG_2_MPLLOUT_OFF);
    
    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_MISC_CTL_1, 0, LPIF_MISC_CTL_1_PWR_SRAM_PDN);
    mcDELAY_US(1);

    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0, PWR_EN_VALUE, LPIF_LOW_POWER_CFG_0_DDRPHY_FB_CK_EN);
    mcDELAY_US(1);
    //vIO32WriteFldAlign(SPM_SPM_POWER_ON_VAL0, 0, SPM_POWER_ON_VAL0_SC_MPLLOUT_OFF);//LewisToDo

    //Joe@20171030: In , If use SPM_DRAMC_DPY_CLK_SW_CON2, you need to set SPM_DRAMC_DPY_CLK_SW_CON_SEL2 to 0.So we use VAL0 
    /*TINFO="DRAM : set sc_dpy_bclk_en = 1"*/
    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_1, PWR_EN_VALUE, LPIF_LOW_POWER_CFG_1_DPY_BCLK_ENABLE);

    OpenLoopMode_1(p);

    mcDELAY_US(1);
    /*TINFO="DRAM : set sc_shu_restore_en = 1"*/
    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_1, PWR_EN_VALUE, LPIF_LOW_POWER_CFG_1_SHU_RESTORE);

    //wait 80ns
    mcDELAY_US(1);
    /*TINFO="DRAM : set sc_shu_restore_en = 0"*/
    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_1, 0, LPIF_LOW_POWER_CFG_1_SHU_RESTORE);

    //wait 195ns
    mcDELAY_US(1);
    /*TINFO="DRAM : set sc_dpy_bclk_en = 0"*/
    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_1, 0, LPIF_LOW_POWER_CFG_1_DPY_BCLK_ENABLE);

    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0, 0, LPIF_LOW_POWER_CFG_0_DDRPHY_FB_CK_EN);
    mcDELAY_US(1);
    //PHYPLL on
    ///TODO: (Step 10) PHYPLL on{PHYPLL  || CLRPLL}
    /* HW will use this bit AND normal path signal(PHYPLL2_MODE_SW_PCM, PHYPLL_MODE_SW_PCM) to decide which PLL is on/off */
    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0, PWR_EN_VALUE, LPIF_LOW_POWER_CFG_0_PHYPLL_EN);

    //wait 20us
    ///TODO: (Step 11) wait 20us
    mcDELAY_US(20);
    
    return;
}


void Func_Apsrc_Wakeup(DRAMC_CTX_T *p, int fromS0)
{
    mcSHOW_DBG_MSG_LPS(("[Start]Func_Apsrc_Wakeup\n"));
    if(fromS0 == 1)
    {
        FUNC_DDRPHY_INFRA_MTCMOS_OFF_EXIT(p);
        mcDELAY_US(1);
    }
    FUNC_DDRPHY_DLL_OFF_INFRA_CG_EN_EXIT(p, fromS0);
    mcDELAY_US(1);
    mcSHOW_DBG_MSG_LPS(("[Stop]Func_Apsrc_Wakeup\n"));
    return;
}


void Func_Apsrc_Sleep (DRAMC_CTX_T *p, int intoS0)
{
    mcSHOW_DBG_MSG_LPS(("[Start]Func_Apsrc_Sleep\n"));
    mcDELAY_US(12);
    Func_Emi_Off(p);
//while(1);
    //------------
    // S-idle-->S1-->S0
    //------------
    FUNC_DDRPHY_DLL_OFF_INFRA_CG_EN_ENTER(p, intoS0);
    if(intoS0 == 1)
    {
        FUNC_DDRPHY_INFRA_MTCMOS_OFF_ENTER(p);
        mcDELAY_US(1);
    }
    mcSHOW_DBG_MSG_LPS(("[Stop]Func_Apsrc_Sleep\n"));
    return;
}


void Enter_Precharge_All(DRAMC_CTX_T *p)
{
    DRAM_CHANNEL_T eOriChannel = vGetPHY2ChannelMapping(p);
    DRAM_RANK_T eOriRank = u1GetRank(p);
    U8 u1ChIdx = 0;
    U8 u1RankIdx = 0;
    U32 u4BackupRg = 0;
    
    for(u1ChIdx = 0; u1ChIdx<(U8)p->support_channel_num; u1ChIdx++)
    {
        vSetPHY2ChannelMapping(p, u1ChIdx);  
        u4BackupRg = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_SWCMD_CTRL0));    
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SWCMD_EN), 1, SWCMD_EN_SPREA_EN);//IPMV2 for Precharge
        for(u1RankIdx=0; u1RankIdx<(U8)p->support_rank_num; u1RankIdx++)
        {
            vSetRank(p, u1RankIdx); 
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SWCMD_CTRL0), u1RankIdx, SWCMD_CTRL0_MRSRK);
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SWCMD_EN), 1, SWCMD_EN_PREAEN);
            mcDELAY_US(2);
            while(u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMDRESP), SPCMDRESP_PREA_RESPONSE) == 0)
            {
                mcSHOW_DBG_MSG_LPS(("!Not enter precharge all...\n"));
            }
            mcSHOW_DBG_MSG_LPS(("CHAN %s Rank[%d] Enter precharge all ...\n", (u1ChIdx==CHANNEL_A) ? ("A"):("B"), 
                (u1RankIdx==RANK_0) ? 0:1));
        }
        vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SWCMD_CTRL0), u4BackupRg);
    }
    vSetRank(p, (U8)eOriRank);
    vSetPHY2ChannelMapping(p, (U8)eOriChannel);     

    return;
}


int enter_pasr_dpd_config_base(DRAMC_CTX_T *p, unsigned char segment_rank0, unsigned char segment_rank1)
{
	unsigned int rank_pasr_segment[2];
	unsigned int iRankIdx = 0, iChannelIdx = 0;
	unsigned int u4value_mr4 = 0;
	unsigned int u4value_zq = 0;
	unsigned int u4value_dcm = 0;
	unsigned int u4value_cke = 0;
	
	rank_pasr_segment[0] = segment_rank0 & 0xFF; /* for rank0 */
	rank_pasr_segment[1] = segment_rank1 & 0xFF; /* for rank1 */
	mcSHOW_DBG_MSG_LPS(("[DRAMC0] PASR r0 = 0x%x  r1 = 0x%x\n", rank_pasr_segment[0], rank_pasr_segment[1]));

	for (iChannelIdx = 0; iChannelIdx < p->support_channel_num; iChannelIdx++) 
    {
        //Backup 
        u4value_mr4 = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_HMR4));
        u4value_zq = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_ZQ_SET1));
        u4value_dcm = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL));
        u4value_cke = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_CKECTRL));

        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_HMR4), 1, HMR4_REFRDIS);/* Disable MR4 */
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_ZQ_SET1), 0, ZQ_SET1_ZQCALDISB);/* Disable ZQCS */
        mcDELAY_US(2);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL), 1, DRAMC_PD_CTRL_MIOCKCTRLOFF);/* MIOCKCTRLOFF = 1 */
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL), 0, DRAMC_PD_CTRL_DCMEN2);/* DCMEN2 = 0 */
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL), 0, DRAMC_PD_CTRL_PHYCLKDYNGEN);/* PHYCLKDYNGEN = 0 */

        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_CKECTRL), P_Fld(0x0, CKECTRL_CKE1FIXOFF)
                    | P_Fld(0x0, CKECTRL_CKEFIXOFF)
                    | P_Fld(0x1, CKECTRL_CKE1FIXON)                    
                    | P_Fld(0x1, CKECTRL_CKEFIXON));/* CKE0 CKE1 fix on no matter the setting of CKE2RANK */  

		for (iRankIdx = 0; iRankIdx < p->support_rank_num; iRankIdx++) 
        {
            DramcModeRegWriteByRank(p, iRankIdx, 17, rank_pasr_segment[0]);
		}

        //Restore
        vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_HMR4), u4value_mr4);
        vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_ZQ_SET1), u4value_zq);
        vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL), u4value_dcm);
        vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_CKECTRL), u4value_cke);
	}
 
	return 0;
}


int enter_pasr_dpd_config(DRAMC_CTX_T *p, unsigned char segment_rank0, unsigned char segment_rank1)
{
	int istatus = 0;
	istatus = enter_pasr_dpd_config_base(p, segment_rank0, segment_rank1);
	return istatus;
}


static void Suspend_Low_Power_Scenario(DRAMC_CTX_T *p)
{
    int intoS0 = 1;
    U32 u32Result;

    switch(global_which_test)
    {
        case LOW_POWER_SCENARIO_ONLY_SELF_REFRESH:
            Enter_Precharge_All(p);
            Func_Emi_Off(p);
            mcSHOW_DBG_MSG_LPS(("Low Power Scenario only SR\n"));
            break;
        case LOW_POWER_SCENARIO_PRECHARGE_ALL:
            Enter_Precharge_All(p);
            break;
        case LOW_POWER_SCENARIO_S0:
            mcSHOW_DBG_MSG_LPS(("Low Power Scenario S0\n"));
            Func_Apsrc_Sleep(p, intoS0);
            break;

        case LOW_POWER_SCENARIO_S1:
            mcSHOW_DBG_MSG_LPS(("Low Power Scenario S1\n"));
            intoS0 = 0;
            Func_Apsrc_Sleep(p, intoS0);
            break;
        case LOW_POWER_SCENARIO_PASR:
            mcSHOW_DBG_MSG_LPS(("Low Power Scenario PASR\n"));
            enter_pasr_dpd_config(p, 0, 0xFF);//Enable_PASR(p);
            Func_Apsrc_Sleep(p, intoS0);    
            break;
#if ETT_COMMAND_FOR_PASR_EXTEND
        case LOW_POWER_SCENARIO_PASR_1BANK:
            mcSHOW_DBG_MSG_LPS(("Low Power Scenario PASR 1 Bank(Both ranks)\n"));
            enter_pasr_dpd_config(p, 0x01, 0x01);
            Func_Apsrc_Sleep(p, intoS0);    
            break;
        case LOW_POWER_SCENARIO_PASR_2BANK:
            mcSHOW_DBG_MSG_LPS(("Low Power Scenario PASR 2 Banks(Both ranks)\n"));
            enter_pasr_dpd_config(p, 0x03, 0x03);
            Func_Apsrc_Sleep(p, intoS0);    
            break;
        case LOW_POWER_SCENARIO_PASR_4BANK:
            mcSHOW_DBG_MSG_LPS(("Low Power Scenario PASR 4 Banks(Both ranks)\n"));
            enter_pasr_dpd_config(p, 0x0F, 0x0F);
            Func_Apsrc_Sleep(p, intoS0);    
            break;
        case LOW_POWER_SCENARIO_PASR_8BANK:
            mcSHOW_DBG_MSG_LPS(("Low Power Scenario PASR 8 Banks(Both ranks)\n"));
            enter_pasr_dpd_config(p, 0xFF, 0xFF);
            Func_Apsrc_Sleep(p, intoS0);    
            break;
#endif
#if 1
        case LOW_POWER_SCENARIO_FAKE_ENGINE_READ:    
            Fake_Engine_Presetting();
            mcSHOW_DBG_MSG_LPS(("Low Power Scenario: All memory Read\n"));
            Fake_Engine_Read(p);
            break;
        case LOW_POWER_SCENARIO_FAKE_ENGINE_WRITE:    
            Fake_Engine_Presetting();
            mcSHOW_DBG_MSG_LPS(("Low Power Scenario: All memory Write\n"));
            Fake_Engine_Write(p);
            break;
        case LOW_POWER_SCENARIO_FAKE_ENGINE_READ_WRITE:    
            Fake_Engine_Presetting();
            mcSHOW_DBG_MSG_LPS(("Low Power Scenario: All memory Read + Write\n"));
            Fake_Engine_Read_Write(p);
            break;
        case LOW_POWER_SCENARIO_FAKE_ENGINE_BW:
            Fake_Engine_Presetting();
            mcSHOW_DBG_MSG_LPS(("Low Power Scenario: All memory Read/Write Bandwidth\n"));
            Fake_Engine_Bandwidth();
            break;
#endif
        default:
            mcSHOW_DBG_MSG_LPS(("!Error Coding flow in Suspend_Low_Power_Scenario\n"));
            while(1);
    }
}


static void Resume_Low_Power_Scenario(DRAMC_CTX_T *p)
{
    int fromS0 = 1;

    if(global_which_test == LOW_POWER_SCENARIO_S1)
    {
        fromS0 = 0;
    }
    switch(global_which_test)
    {
        case LOW_POWER_SCENARIO_ONLY_SELF_REFRESH:
            Func_Emi_On(p);
            break;
        case LOW_POWER_SCENARIO_S0: 
        case LOW_POWER_SCENARIO_S1:
            Func_Apsrc_Wakeup(p, fromS0);
            break;
        case LOW_POWER_SCENARIO_PASR:    
            Func_Apsrc_Wakeup(p, fromS0);
            enter_pasr_dpd_config(p, 0, 0);//Disable_PASR(p);
            break;
        default:
            mcSHOW_DBG_MSG_LPS(("!Error Coding flow in Suspend_Low_Power_Scenario\n"));
            while(1);
    }
}


void Access_Memory_Test(DRAMC_CTX_T *p)
{
#if LOW_POWER_STRESS_TEST_WITH_TA2
    TA2_Test_Run_Time_HW(p);
#else
    Write_Ranks_With_1024bytes(p);
    Read_And_Compare_Ranks_With_1024bytes(p);
#endif    
}

//refer to page 41~44 of new SPM protocol.pptx
//static void Low_Power_Scenarios_Init(DRAMC_CTX_T *p)
void Low_Power_Scenarios_Init(DRAMC_CTX_T *p)
{
    vIO32WriteFldMulti(DDRPHY_MD32_REG_LPIF_FSM_CFG_1, P_Fld(0x0, LPIF_FSM_CFG_1_LPIF_LEGACY_CONTROL)
                        | P_Fld(0x0, LPIF_FSM_CFG_1_LPIF_LEGACY_CONTROL_2ND)
                        | P_Fld(0x0, LPIF_FSM_CFG_1_LPIF_LEGACY_CONTROL_FOR_PWR)                    
                        | P_Fld(0x0, LPIF_FSM_CFG_1_LPIF_LEGACY_CONTROL_FOR_PWR_2ND)
                        | P_Fld(0x1, LPIF_FSM_CFG_1_LPIF_OUTPUT_PATH_FROM_SW)
                        | P_Fld(0x1, LPIF_FSM_CFG_1_LPIF_OUTPUT_PATH_FROM_SW_2ND)
                        | P_Fld(0x1, LPIF_FSM_CFG_1_LPIF_POWER_CONTROL_SEL)
                        | P_Fld(0x1, LPIF_FSM_CFG_1_LPIF_POWER_CONTROL_SEL_2ND)
                        );

    vIO32WriteFldMulti(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0, P_Fld(0x3, LPIF_LOW_POWER_CFG_0_PHYPLL_EN)
                        | P_Fld(0x3, LPIF_LOW_POWER_CFG_0_DPY_DLL_EN)
                        | P_Fld(0x3, LPIF_LOW_POWER_CFG_0_DPY_2ND_DLL_EN)                    
                        | P_Fld(0x3, LPIF_LOW_POWER_CFG_0_DPY_DLL_CK_EN)
                        | P_Fld(0x3, LPIF_LOW_POWER_CFG_0_DPY_VREF_EN)
                        | P_Fld(0x3, LPIF_LOW_POWER_CFG_0_PHYPLL_MODE_SW));

    vIO32WriteFldMulti(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_3, P_Fld(0x3, LPIF_LOW_POWER_CFG_3_DPY_MCK8X_EN)
                        | P_Fld(0x3, LPIF_LOW_POWER_CFG_3_DPY_MIDPI_EN)
                        | P_Fld(0x3, LPIF_LOW_POWER_CFG_3_DPY_PI_RESETB_EN));

    vIO32WriteFldMulti(DDRPHY_MD32_REG_LPIF_FSM_OUT_CTRL_0, P_Fld(0x0, LPIF_FSM_OUT_CTRL_0_LOG_OPT_DR_SHORT_QUEUE)
                        | P_Fld(0x0, LPIF_FSM_OUT_CTRL_0_LOG_OPT_DR_SHU_EN)
                        | P_Fld(0x0, LPIF_FSM_OUT_CTRL_0_LOG_OPT_PHYPLL2_MODE_SW)                    
                        | P_Fld(0x0, LPIF_FSM_OUT_CTRL_0_LOG_OPT_PHYPLL2_SHU_EN)
                        | P_Fld(0x1, LPIF_FSM_OUT_CTRL_0_LOG_OPT_PHYPLL_MODE_SW) // 11
                        | P_Fld(0x0, LPIF_FSM_OUT_CTRL_0_LOG_OPT_PHYPLL_SHU_EN)
                        | P_Fld(0x0, LPIF_FSM_OUT_CTRL_0_LOG_OPT_DR_GATE_RETRY_EN)
                        | P_Fld(0x0, LPIF_FSM_OUT_CTRL_0_LOG_OPT_DDRPHY_FB_CK_EN)
                        | P_Fld(0x0, LPIF_FSM_OUT_CTRL_0_LOG_OPT_MEM_CK_OFF)
                        | P_Fld(0x0, LPIF_FSM_OUT_CTRL_0_LOG_OPT_EMI_CLK_OFF_REQ)
                        | P_Fld(0x1, LPIF_FSM_OUT_CTRL_0_LOG_OPT_DPY_VREF_EN) // 5
                        | P_Fld(0x1, LPIF_FSM_OUT_CTRL_0_LOG_OPT_DPY_DLL_CK_EN) // 4  
                        | P_Fld(0x1, LPIF_FSM_OUT_CTRL_0_LOG_OPT_DPY_2ND_DLL_EN) // 3
                        | P_Fld(0x1, LPIF_FSM_OUT_CTRL_0_LOG_OPT_DPY_DLL_EN) //  2
                        | P_Fld(0x1, LPIF_FSM_OUT_CTRL_0_LOG_OPT_PHYPLL_EN) // 1
                        | P_Fld(0x0, LPIF_FSM_OUT_CTRL_0_LOG_OPT_DMSUS_OFF)
                        | P_Fld(0x0, LPIF_FSM_OUT_CTRL_0_LOG_OPT_DR_SRAM_LOAD)
                        | P_Fld(0x0, LPIF_FSM_OUT_CTRL_0_LOG_OPT_DR_SHU_SRAM_LEVEL)
                        | P_Fld(0x0, LPIF_FSM_OUT_CTRL_0_LOG_OPT_TX_TRACKING_RETRY_EN)
                        | P_Fld(0x0, LPIF_FSM_OUT_CTRL_0_LOG_OPT_TX_TRACKING_DIS)                    
                        | P_Fld(0x0, LPIF_FSM_OUT_CTRL_0_LOG_OPT_DMYRD_EN)
                        | P_Fld(0x0, LPIF_FSM_OUT_CTRL_0_LOG_OPT_DMYRD_INTV_SEL)
                        | P_Fld(0x0, LPIF_FSM_OUT_CTRL_0_LOG_OPT_DMY_EN_MOD_SEL)
                        | P_Fld(0x0, LPIF_FSM_OUT_CTRL_0_LOG_OPT_DPHY_RXDLY_TRACK_EN)
                        | P_Fld(0x0, LPIF_FSM_OUT_CTRL_0_LOG_OPT_DPHY_PRECAL_UP)
                        | P_Fld(0x0, LPIF_FSM_OUT_CTRL_0_LOG_OPT_SHU_RESTORE)
                        | P_Fld(0x0, LPIF_FSM_OUT_CTRL_0_LOG_OPT_DPY_BCLK_ENABLE)
                        | P_Fld(0x0, LPIF_FSM_OUT_CTRL_0_LOG_OPT_DR_SHU_LEVEL)                    
                        | P_Fld(0x0, LPIF_FSM_OUT_CTRL_0_LOG_OPT_DR_SHU_LEVEL_SRAM_LATCH));

    vIO32WriteFldMulti(DDRPHY_MD32_REG_LPIF_FSM_OUT_CTRL_1, P_Fld(0x3, LPIF_FSM_OUT_CTRL_1_LOG_OPT_FHC_PAUSE_MEM)
                        | P_Fld(0x0, LPIF_FSM_OUT_CTRL_1_LOG_OPT_FHC_PAUSE_MPLL)
                        | P_Fld(0x3, LPIF_FSM_OUT_CTRL_1_LOG_OPT_MPLL_S_OFF)                    
                        | P_Fld(0x3, LPIF_FSM_OUT_CTRL_1_LOG_OPT_DPY_MODE_SW)
                        | P_Fld(0x0, LPIF_FSM_OUT_CTRL_1_LOG_OPT_DVFS_MEM_CK_MUX_SEL)
                        | P_Fld(0x0, LPIF_FSM_OUT_CTRL_1_LOG_OPT_DVFS_MEM_CK_MUX_UPDATE)
                        | P_Fld(0x1, LPIF_FSM_OUT_CTRL_1_LOG_OPT_DPY_PI_RESETB_EN) 
                        | P_Fld(0x1, LPIF_FSM_OUT_CTRL_1_LOG_OPT_DPY_MIDPI_EN)
                        | P_Fld(0x1, LPIF_FSM_OUT_CTRL_1_LOG_OPT_DPY_MCK8X_EN));

#if 0
    vIO32WriteFldMulti(DDRPHY_MD32_REG_LPIF_MISC_CTL_1, P_Fld(0x3, LPIF_MISC_CTL_1_PWR_RST_B)
                        | P_Fld(0x0, LPIF_MISC_CTL_1_PWR_ISO)
                        | P_Fld(0x3, LPIF_MISC_CTL_1_PWR_ON)                    
                        | P_Fld(0x3, LPIF_MISC_CTL_1_PWR_ON_2ND)
                        | P_Fld(0x0, LPIF_MISC_CTL_1_PWR_CLK_DIS)
                        | P_Fld(0x0, LPIF_MISC_CTL_1_PWR_SRAM_PDN)
                        | P_Fld(0x0, LPIF_MISC_CTL_1_MPLL_OFF));
#endif
    
}


void Low_Power_Scenarios_Test(DRAMC_CTX_T *p)
{
    int i = 0;
    int testCnt = 0;

//LewisLogEnable = 1;
    Low_Power_Scenarios_Init(p);//LewisToDo: Wait for Chen-Hsiang bring up value of MD32 RG && conf
    testCnt = TEST_CNR_OF_LOW_POWER_SCENARIO;

    for(i=0; i<testCnt; i++)
    {
        mcSHOW_DBG_MSG_LPS(("**** Start Low Power scenarios DDR[%d]****\n", p->frequency * 2));
        mcSHOW_DBG_MSG_LPS(("**** Test[%d] ****\n", i+1));


        Access_Memory_Test(p);

        /* Suspend */        
        mcSHOW_DBG_MSG_LPS(("**** Suspend\n"));
        Suspend_Low_Power_Scenario(p);

#ifdef MEASURE_POWER_WITH_ETT
        if(i == 1)
        {
            while(1);
        }
#endif

#if 0
        if(global_which_test == LOW_POWER_SCENARIO_PASR)
        {
            mcSHOW_DBG_MSG_LPS(("**** Delay %d ms\n", global_refresh_delay));
            mcDELAY_MS(global_refresh_delay);
        }
#endif        

#if TEST_LOW_POWER_WITH_1_SEC_DELAY
        mcSHOW_DBG_MSG_LPS(("Delay 1 second\n"));
        mcDELAY_MS(1000);
#endif
        /* Resume */
        mcSHOW_DBG_MSG_LPS(("**** Resume\n"));
        Resume_Low_Power_Scenario(p);

        //DFSTestProgram(p, 0);

        Access_Memory_Test(p);

        mcSHOW_DBG_MSG_LPS(("**** Stop Power scenarios test End ****\n"));
        mcSHOW_DBG_MSG_LPS(("\n\n\n\n\n\n"));
    }
}

static void Low_Power_Scenarios_Test_All(DRAMC_CTX_T *p)
{
    int iLastTest = (p->support_rank_num == RANK_DUAL) ?  LOW_POWER_SCENARIO_PASR : LOW_POWER_SCENARIO_S0;
    
    for(global_which_test = LOW_POWER_SCENARIO_S1; global_which_test <= iLastTest; global_which_test++)
    {
        Low_Power_Scenarios_Test(p);
    }
    global_which_test = LOW_POWER_SCENARIO_ALL;
}


void Enter_Suspend(DRAMC_CTX_T *p)
{
    mcSHOW_DBG_MSG_LPS(("* Enter Suspend for Power Measurement\n"));
    mcSHOW_DBG_MSG_LPS(("* Freq DDR[%d]****\n", psCurrDramCtx->frequency * 2));

    Suspend_Low_Power_Scenario(psCurrDramCtx);
}
