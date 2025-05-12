#include "dramc_dv_init.h"
//========================================================
//Do this function need config channel_id -- Can not broadcast
//========================================================
void DRAMC_DMA_CONF_to_SRAM(DRAMC_CTX_T *p,U8 GP_id,U8 CONF_id)
{
    U8 SRAM_DONE;
    U8 APB_DONE;
    U8 DMA_interval = 0;

    mcSHOW_DBG_MSG("[DRAMC_DMA_CONF_to_SRAM] Source CONF:%1d, Target SRAM group:%1d @Channle:%1d\n",CONF_id,GP_id,vGetPHY2ChannelMapping(p));
    //Enable SRAM software Mode
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SRAM_DMA0), P_Fld(    1    ,   MISC_SRAM_DMA0_SW_MODE          )\
                                                                | P_Fld(    1    ,   MISC_SRAM_DMA0_SW_STEP_EN_MODE  )\
                                                                | P_Fld(    1    ,   MISC_SRAM_DMA0_SRAM_WR_MODE     )\
                                                                | P_Fld(    0    ,   MISC_SRAM_DMA0_APB_WR_MODE      ));

    //FIRE=0 and set source CONF and Tartget SRAM group_id
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SRAM_DMA0), P_Fld(    0    ,   MISC_SRAM_DMA0_SW_DMA_FIRE      )\
                                                                | P_Fld(  CONF_id,   MISC_SRAM_DMA0_SW_SHU_LEVEL_APB )\
                                                                | P_Fld(  GP_id  ,   MISC_SRAM_DMA0_SW_SHU_LEVEL_SRAM));
    //trigger DMA 
    mcSHOW_DBG_MSG("[DRAMC_DMA_CONF_to_SRAM] Trigger DMA->\n");
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SRAM_DMA0), 1, MISC_SRAM_DMA0_SW_DMA_FIRE      );
    
    //wait result
    do
    {
        mcDELAY_XUS(1);
        DMA_interval = DMA_interval + 1;
        mcSHOW_DBG_MSG("[DRAMC_DMA_CONF_to_SRAM] DMA on-going %1d us...\n",DMA_interval);
        APB_DONE  = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DMA_DEBUG0), MISC_DMA_DEBUG0_APB_SHU_DONE );
        SRAM_DONE = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DMA_DEBUG0), MISC_DMA_DEBUG0_SRAM_SHU_DONE );
    } while(!((SRAM_DONE==1) && (APB_DONE==1)));
    mcSHOW_DBG_MSG("[DRAMC_DMA_CONF_to_SRAM] DMA Done.\n");

    //Stop 
    mcSHOW_DBG_MSG("[DRAMC_DMA_CONF_to_SRAM] Stop DMA\n");
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SRAM_DMA0), 0, MISC_SRAM_DMA0_SW_DMA_FIRE      );

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SRAM_DMA0), 0, MISC_SRAM_DMA0_SW_MODE          );

    //For FIX SHU4 and SHU10????--mark temp need do later
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SRAM_DMA0), 0, MISC_SRAM_DMA0_APB_SLV_SEL      );
}

void DRAMC_DMA_CONF_to_SRAM_sudo()
{

}
