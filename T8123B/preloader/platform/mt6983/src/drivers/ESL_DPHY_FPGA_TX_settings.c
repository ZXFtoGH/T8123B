// For ESL use DPHY settings only
// Use these setting to cover APHY setting
// Not consider about shuffle RG, due to no this scenario
#include "dramc_dv_init.h"

void ESL_DPHY_FPGA_TX_settings(DRAMC_CTX_T *p)
{
  mcSHOW_DBG_MSG("=================================== \n");
  mcSHOW_DBG_MSG("Start %s ESL_DPHY_FPGA_TX_settings \n", (LPDDR5_EN_S?"LPDDR5":"LPDDR4"));
  mcSHOW_DBG_MSG("=================================== \n");

  mcSHOW_DBG_MSG("Current FREQ=%0d, DQ_P2S_RATIO=%0d.\n", DFS(0)->data_rate, DFS(0)->DQ_P2S_RATIO);

  mcSHOW_DBG_MSG("=================================== \n");
  mcSHOW_DBG_MSG("End   %s ESL_DPHY_FPGA_TX_settings \n", (LPDDR5_EN_S?"LPDDR5":"LPDDR4"));
  mcSHOW_DBG_MSG("=================================== \n");
}



