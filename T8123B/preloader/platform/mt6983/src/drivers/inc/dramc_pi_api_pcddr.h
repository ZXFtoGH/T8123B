#ifndef _PI_API_PCDDR_H
#define _PI_API_PCDDR_H

#include "dramc_dv_init.h"

/*----------------------------------------------------------------------
  Add PCDDR specific define & typedef & function proto-type
 ----------------------------------------------------------------------*/

#define DDR4266_FREQ    (2133)
#define DDR3733_FREQ    (1866)
#define DDR3200_FREQ    (1600)
#define DDR2933_FREQ    (1466)
#define DDR2667_FREQ    (1333)
#define DDR2400_FREQ    (1200)
#define DDR2133_FREQ    (1066)
#define DDR1866_FREQ    (933)
#define DDR1600_FREQ    (800)
#define DDR1333_FREQ    (666)
#define DDR1200_FREQ    (600)
#define DDR1066_FREQ    (533)
#define DDR800_FREQ     (400)

#define DDR4_MRFSP_TERM_FREQ DDR2400_FREQ

/*
  Impedance Defult Value
  Author: Hongbo Li (MTK13176)   Date: (4/7/2021)
-> Designer's defult (WL Lee):
->          DRVP   DRVN   ODTP
-> DDR4   14      14         6

*/
#define DDR4_DEFULT_ODTP    6
#define DDR4_DEFULT_ODTN    0
#define DDR4_DEFULT_DRVN    14
#define DDR4_DEFULT_DRVP    14

#if !FOR_DV_SIMULATION_USED
#define DDR_ENABLE_ZQ_CAL               1
#define DDR_ENABLE_SW_IMPED_CAL         1
#define DDR_ENABLE_MIOCK_JMETER_CAL     1
#define DDR_ENABLE_8PHASE_CAL           0
#define DDR_ENABLE_DUTY_CAL             0
#define DDR_ENABLE_RX_INPUT_BUF_CAL     0
#define DDR_ENABLE_WRITE_LEVELING_CAL   1
#define DDR_ENABLE_DUTY_CYC_MONITOR_CAL 1
#define DDR_ENABLE_GATING_CAL           1
#define DDR_ENABLE_DATLAT_CAL           1
#define DDR_ENABLE_RX_RDDQC_CAL         1
#define DDR_ENABLE_RX_PERBIT_CAL        1
#define DDR_ENABLE_TX_PERBIT_CAL        1  // Please enable with write leveling
#define DDR_ENABLE_TX_OE_CAL            0
#define DDR_ENABLE_RX_DVS_CAL           1
#define DDR_ENABLE_RUNTIME_CONFIG       1
#else
#define DDR_ENABLE_ZQ_CAL               1
#define DDR_ENABLE_SW_IMPED_CAL         1
#define DDR_ENABLE_MIOCK_JMETER_CAL     1
#define DDR_ENABLE_8PHASE_CAL           1
#define DDR_ENABLE_DUTY_CAL             0
#define DDR_ENABLE_RX_INPUT_BUF_CAL     0
#define DDR_ENABLE_WRITE_LEVELING_CAL   1
#define DDR_ENABLE_DUTY_CYC_MONITOR_CAL 0
#define DDR_ENABLE_GATING_CAL           1
#define DDR_ENABLE_DATLAT_CAL           1
#define DDR_ENABLE_RX_RDDQC_CAL         1
#define DDR_ENABLE_RX_PERBIT_CAL        1
#define DDR_ENABLE_TX_PERBIT_CAL        1  // Please enable with write leveling
#define DDR_ENABLE_TX_OE_CAL            0
#define DDR_ENABLE_RX_DVS_CAL           0
#define DDR_ENABLE_RUNTIME_CONFIG       0  // @Darren for DV sim
#endif

#define DDR_TX_K_DQM_WITH_WDBI          0
#define DDR_RX_DELAY_PRE_CAL            1

/* Feature */
#define DDR_ENABLE_READ_DBI                         0
#define DDR_ENABLE_WRITE_DBI                        0
#define DDR_ENABLE_PER_BANK_REFRESH                 0
#define DDR_TDQSCK_PRECALCULATION_FOR_DVFS          0
#define DDR_DRAMC_MODIFIED_REFRESH_MODE             0
#define DDR_DRAMC_CKE_DEBOUNCE                      0
#define DDR_ENABLE_TX_WDQS                          0
#define DDR_APPLY_DQDQM_DUTY_CALIBRATION            1
#define DDR_DFS_ENABLE                              0

/* Runtime Related */
#define DDR_HW_GATING                               0
#define DDR_ENABLE_TX_TRACKING                      0
#define DDR_ENABLE_RX_TRACKING                      0
#define DDR_ZQCS_ENABLE                             0
#define DDR_IMPEDANCE_TRACKING_ENABLE               0
#define DDR_TEMP_SENSOR_ENABLE                      0
#define DDR_RDSEL_TRACKING_EN                       0
#define DDR_ENABLE_DFS_RUNTIME_MRW                  0
#define DDR_ENABLE_RODT_TRACKING                    0
#define DDR_GATING_ADJUST_TXDLY_FOR_TRACKING        0
#define DDR_FEATURE_RDDQC_K_DMI                     0
#define DDR_CLK_FREE_FUN_FOR_DRAMC_PSEL             0
#define DDR_HW_SAVE_FOR_SR                          0
#define DDR_SPM_CONTROL_AFTERK                      0
#define DDR_DUMMY_READ_FOR_TRACKING                 0
#define DDR_ENABLE_RX_DCM_DPHY                      0 //Set 0 will lead DCM on/off error

#define DV_SIMULATION_PC4   1
#define DV_SIMULATION_PC3   1
#define DQS_NUMBER_LP4      2
#define DQ_DATA_WIDTH_DDR   16


typedef struct {
    U16 mr00;
    U16 mr01;
    U16 mr02;
    U16 mr03;
    U16 mr04;
    U16 mr05;
    U16 mr06;
} ddr_mr_val_t;

typedef enum
{
	SERIAL_MODE = 0,
	PARALLEL_MODE,
	STAGGERED_MODE,
	RESERVED_MODE,
} dram_mpr_mode_t;

struct tx_dly_for_all {
    U8 mck;
    U8 ui;
    U8 pi;

    U8 mck_oe;
    U8 ui_oe;
};

struct tx_dly_per_byte {
    U8 dq_mck[DQS_NUMBER];
    U8 dq_ui[DQS_NUMBER];
    U8 dq_pi[DQS_NUMBER];

    U8 dqm_mck[DQS_NUMBER];
    U8 dqm_ui[DQS_NUMBER];
    U8 dqm_pi[DQS_NUMBER];

    U8 dq_mck_oe[DQS_NUMBER];
    U8 dq_ui_oe[DQS_NUMBER];
    U8 dqm_mck_oe[DQS_NUMBER];
    U8 dqm_ui_oe[DQS_NUMBER];
};


#define DDR_RDDQC_PINMUX_WORKAROUND 0
#define DDR_MPRR_MODE SERIAL_MODE


/* Global variables */
EXTERN const U8 PCDDR4_O1_Mapping_POP[CHANNEL_NUM][16];
EXTERN const U8 PCDDR3_O1_Mapping_POP[CHANNEL_NUM][16];
EXTERN ddr_mr_val_t gMRVal[CHANNEL_NUM][RANK_MAX];
EXTERN U16 u2gdelay_cell_ps_all[DRAM_DFS_SRAM_MAX][CHANNEL_NUM];///TODO: to be removed by Francis
EXTERN U32 u4gVcore[DRAM_DFS_SRAM_MAX];
EXTERN U16 u2gdelay_cell_ps;

/* Function Protos */
EXTERN void DDR3_MRS(DRAMC_CTX_T *p, U16 reg_addr, U32 reg_op,  U8 rank);
EXTERN void DDR4_MRS(DRAMC_CTX_T *p, U16 reg_addr, U32 reg_op, U8 rank);
EXTERN void PC3_dram_init_single_rank(DRAMC_CTX_T *p,PC3_DRAM_CONFIG_T *tr,U8 rank);
EXTERN void PC4_dram_init_single_rank(DRAMC_CTX_T *p,PC4_DRAM_CONFIG_T *tr,U8 rank);
EXTERN void DIG_CONFIG_SHUF_PC4_TX_PREAMBLE(DRAMC_CTX_T *p, int ch_id, int group_id);


#endif // _PI_API_H
