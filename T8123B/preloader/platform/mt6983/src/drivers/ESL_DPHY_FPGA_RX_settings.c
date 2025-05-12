// For ESL use DPHY settings only
// Use these setting to cover APHY setting
// Not consider about shuffle RG, due to no this scenario
#include "dramc_dv_init.h"

void ESL_DPHY_FPGA_RX_settings(DRAMC_CTX_T *p)
{
  mcSHOW_DBG_MSG("=================================== \n");
  mcSHOW_DBG_MSG("Start %s ESL_DPHY_FPGA_RX_settings \n", (LPDDR5_EN_S?"LPDDR5":"LPDDR4"));
  mcSHOW_DBG_MSG("=================================== \n");

  mcSHOW_DBG_MSG("Current FREQ=%0d, DQ_P2S_RATIO=%0d.\n", DFS(0)->data_rate, DFS(0)->DQ_P2S_RATIO);
//DPHY gating control mode

	vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_STBCAL), 0, MISC_SHU_STBCAL_DQSIEN_DQSSTB_MODE);
  vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_STBCAL), 0, MISC_SHU_STBCAL_STB_SELPHCALEN);

  mcSHOW_DBG_MSG("=================================== \n");
  mcSHOW_DBG_MSG("End   %s ESL_DPHY_FPGA_RX_settings \n", (LPDDR5_EN_S?"LPDDR5":"LPDDR4"));
  mcSHOW_DBG_MSG("=================================== \n");
}

