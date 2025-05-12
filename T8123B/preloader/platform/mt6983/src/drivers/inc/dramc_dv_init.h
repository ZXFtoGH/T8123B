#ifndef _DRAMC_DV_INIT_H_
#define _DRAMC_DV_INIT_H_

#include "dramc_common.h"
#include "dramc_int_global.h"
#include "x_hal_io.h"
#if FOR_DV_SIMULATION_USED
#include "svdpi.h"
#include "sv_c_data_traffic.h"
#endif

#define SA_CONFIG_EN 1

/**///    //=========================================================
/**///    //DRAM CONFIG ELEMENT COLLECTION
/**///    //=========================================================
//typedef enum { DDR3, DDR4, LPDDR3, LPDDR4, LPDDR5, PSRAM } DRAM_TYPE_T;
//typedef enum {LPDDR, LPDDR2, PCDDR3, LPDDR3, LPDDR4, LPDDR5, PSRAM} DRAM_TYPE_T;
typedef enum {LPDDR, LPDDR2, PCDDR3, LPDDR3, LPDDR4, LPDDR5, PCDDR4, PSRAM } DRAM_TYPE_T;
typedef enum {BG4BK4, BK8, BK16, BKORG_RFU}   e_BKORG;
typedef enum {DIS_both, EN_t, EN_both, EN_c}  e_RDQSWCK;//MR20
typedef enum {LP45_NORM, LP45_BYTE, LP45_MIX} e_lp45_type;
/**///
#define DV_CONFIG_EN 1
#define SA_CALIBRATION_EN   1
//=========================================================
//Build Top configuration
//=========================================================
#define DFS_GROUP_NUM 10
#define CH_NUM        2
#define RK_NUM_PER_CH 2
#define DONT_CARE_VALUE 0
#define PULL_UP   1
#define PULL_DOWN 1

/**///
/**///
/**///
typedef struct Gating_config
{
    U8 GAT_TRACK_EN         ;
    U8 RX_GATING_MODE       ;
    U8 RX_GATING_TRACK_MODE ;
    U8 SELPH_MODE           ;
    U8 PICG_EARLY_EN        ;
    U8 VALID_LAT_VALUE      ;
}Gating_confg_T;
/**///
/**///
/**///    //=========================================================
/**///    //DV configuration connection
/**///    //=========================================================
/**///    #if DV_CONFIG_EN==1
/**///    typedef struct DRAMC_DVFS_GROUP_transfer
/**///    {
/**///        U8                CKR         ; //LPDDR5 CKR could be 4 and 2  other memory type should be 1
/**///        U8                DQSIEN_MODE ; //ANA DQSG mode config  LPDDR4 = 1, LPDDR5 with other modes
/**///        U8                DQ_P2S_RATIO; //16-1 8-1 4-1   LPDDR5 could support 16-1 mode
/**///        U8                RESERVED_8BIT;
/**///        U32               data_rate   ;
/**///    }DRAMC_DVFS_GROUP_transfer_T;
/**///
/**///    typedef struct DV_configuration
/**///    {
/**///        U8  EX_ROW_EN_1      ;
/**///        U8  EX_ROW_EN_0      ;
/**///        U8  BYTE_MODE_1      ;
/**///        U8  BYTE_MODE_0      ;
/**///        U8  LP4YEN          ;
/**///        U8  LP4_WR_PST       ;
/**///        U8  LP4_OTF          ;
/**///        U8  NEW_8X_MODE      ;
/**///        U8  LP45_APHY_COMB_EN;
/**///        U8  DLL_IDLE_MODE    ;
/**///        U8  NEW_RANK_MODE    ;
/**///        U8  DLL_ASYNC_EN     ;
/**///        U8  MD32_EN          ;
/**///        U8  SRAM_EN          ;
/**///        U8  GP_NUM           ;
/**///    } DV_new_config_T;
/**///    #endif
/**///
/**///
//=========================================================
//DDR3 DRAM config
//=========================================================
typedef struct PC3_DRAM_CONFIG
{
    U8  CWL         ;
    U8  RTT_NORM    ;
    U8  CL          ;
    U8  AL          ;
    U8  BL          ;
    U8  RBT         ;
    U8  WR          ;
} PC3_DRAM_CONFIG_T;

//=========================================================
//DDR4 DRAM config
//=========================================================
typedef struct PC4_DRAM_CONFIG
{
    U8  CWL         ;
    U8  RTT_NORM    ;
    U8  CL          ;
    U8  AL          ;
    U8  BL          ;
    U8  RBT         ;
    U8  WR          ;
    U8  TCCDL       ;
} PC4_DRAM_CONFIG_T;

//LPDDR4 DRAM config
//=========================================================
typedef struct LP4_DRAM_CONFIG
{
    U8  BYTE_MODE[2]; //diff rank
    U8  EX_ROW_EN[2]; //diff rank --density over 10G should 1
    U8  MR_WL    ;
    U8  MR_RL    ;
    U8  nWR    ;
    U8  BL       ;
    U8  RPST     ;
    U8  RD_PRE   ;
    U8  WR_PRE   ;
    U8  WR_PST_FSPx;
    U8  WR_PST_FSPy;
    U8  DBI_WR   ;
    U8  DBI_RD   ;
    //U8  DMI      ;    //No use  default enable
    U8  OTF      ;
    U8  LP4YEN  ;
    U8  WORK_FSP ;
} LP4_DRAM_CONFIG_T;
/**///
/**///
/**///
//=========================================================
//LPDDR5 DRAM config
//=========================================================
typedef struct LP5_DRAM_CONFIG
{
    U8          BYTE_MODE[2] ;
    U8          EX_ROW_EN[2] ;
    U8          MR_WL        ;
	U8          MR_RL        ;
	U8          nWR          ;
	U8          BL           ;
	U8          CK_Mode      ;
//	U8          RPST         ;
//	U8          RD_PRE       ;
//	U8          WR_PRE       ;
//	U8          WR_PST       ;
	U8          DBI_WR       ;
	U8          DBI_RD       ;
//	U8          DMI          ;
	U8          OTF          ;
    U8          WCK_PST      ;
    U8          RDQS_PRE     ;
    U8          RDQS_PST     ;
    U8          CA_ODT       ;
    U8          DQ_ODT       ;
    U8          NT_ODT       ;
    U8          CKR          ;
    U8          WCK_ON       ;
    U8          WCK_FM       ;
    U8          WCK_ODT      ;
    U8          DVFSQ        ;
    U8          DVFSC        ;
    e_RDQSWCK   RDQSmode[2]  ;
    U8          WCKmode[2]   ;
    U8          RECC         ;
    U8          WECC         ;
    e_BKORG     BankMode     ;
    U8          WORK_FSP     ;
    U8          DFEQU        ;
    U8          DFEQL        ;
    U8          NT_DQ_ODT    ;
    U8          ODTD_CA      ;
    U8          ODTD_CK      ;
    U8          SOC_ODT      ;
    U8          OptREF       ;
} LP5_DRAM_CONFIG_T;
/**///
//=========================================================
//Analog PHY config
//=========================================================
typedef struct ANA_top_function_config
{
    U8   ASYNC_MODE       ;
    U8   DLL_ASYNC_EN     ;
    U8   ALL_SLAVE_EN     ;
    U8   NEW_RANK_MODE    ;
    U8   DLL_IDLE_MODE    ;
    U8   LP45_APHY_COMB_EN;
    U8   TX_ODT_DIS       ;
    U8   NEW_8X_MODE      ;
    U8   LP4_WDQS_MODE    ; //LPDDR4 Jedec SPEC WDQS mode 1 Do not support Mode 2 -- need following ASVA1-7 ODT setting
}ANA_top_config_T;
/**///
/**///
typedef struct ANA_DVFS_core_config
{
    U8   CKR;
    U8   DQ_P2S_RATIO;
    //U8   LP5_1600_DQ_P2S_MODE;
    U8   CA_P2S_RATIO;
    U8   DQ_CA_OPEN;
    U8   DQ_SEMI_OPEN;
    U8   CA_SEMI_OPEN;
    U8   CA_FULL_RATE;
    U8   DQ_CKDIV4_EN;
    U8   CA_CKDIV4_EN;
    U8   CA_PREDIV_EN;
    U8   CA_PH8_DLY;
    U8   DQ_PH8_DLY;
    U8   SEMI_OPEN_CA_PICK_MCK_RATIO;
    U8   DQ_AAMCK_DIV;
    U8   CA_AAMCK_DIV;
    U8   CA_ADMCK_DIV;
    U8   DQ_TRACK_CA_EN;
    U32  PLL_FREQ;
    U8   DQ_UI_PI_RATIO;
    U8   CA_UI_PI_RATIO;
    U32  DQ_MCKIO;
    U32  CA_MCKIO;
} ANA_DVFS_CORE_T;
/**///
/**///
//=========================================================
//DVFS group configuration
//=========================================================
typedef struct DRAMC_DVFS_GROUP_CONFIG
{
    U32               data_rate         ;
    U8                APHY_CLOCK_MODE   ;
    U8                APHY_CLOCK_MODE_EN;
    U8                DQSIEN_MODE       ; //ANA DQSG mode config  LPDDR4 = 1, LPDDR5 with other modes
    U8                DQ_P2S_RATIO      ; //16-1 8-1 4-1   LPDDR5 could support 16-1 mode
    U8                CKR               ; //LPDDR5 CKR could be 4 and 2, other memory type should be 1
    U8                TX_IMPAT_LAT_DRAMOBF  ; //DRAMOBF function releated latency.
    U8                RX_IMPAT_LAT_DRAMOBF  ; //DRAMOBF function releated latency.
    U8                DIG_IMPAT_LAT_DRAMOBF ; //DRAMOBF function releated latency.
    U32               TX_DQ_Dline           ; //Dline.
}DRAMC_DVFS_GROUP_CONFIG_T;

//=========================================================
//DRAMC Subsystem config
//=========================================================
typedef struct DRAMC_SUBSYS_CONFIG
{
    U8                        GP_NUM               ;
    U8                        SRAM_EN              ;
    U8                        MD32_EN              ;
    ANA_top_config_T          *a_cfg               ;
    ANA_DVFS_CORE_T           *a_opt               ;
    PC3_DRAM_CONFIG_T         *pc3_init            ;
    PC4_DRAM_CONFIG_T         *pc4_init            ;
    LP4_DRAM_CONFIG_T         *lp4_init            ;
    LP5_DRAM_CONFIG_T         *lp5_init            ;
    DRAMC_DVFS_GROUP_CONFIG_T *DFS_GP[DFS_GROUP_NUM];
}DRAMC_SUBSYS_CONFIG_T;


/**///
typedef struct HW_FUNC_enable_T {
  U8         HW_DQSG_EN                    ;
  U8         HW_DQSOSC_EN                  ;
  U8         HW_DMYRD_EN                   ;
  U8         HW_REFR_EN                    ;
  U8         HW_REF_EN                     ;
  U8         HW_IMPCAL_EN                  ;
  U8         HW_ZQ_EN                      ;
}
#if FOR_DV_SIMULATION_USED
__attribute__((packed))
#endif
HW_FUNC_enable_T;


/**///
typedef struct DUT_shuf_config_T {
  U32        TX_DQ_Dline                   ;
  U8         FAKE_SEL_NEW_MODE             ;
  U8         RDLE128_PACKING_EN            ;
  U8         WCK_MCK_RANK_DIFF             ;
  U8         WCK_PI_RANK_DIFF              ;
  U8         DCM_SHUF_EN                   ;
  U8         LP5_DVFSQ_USE                 ;
  U8         LP5_DVFSC_USE                 ;
  U8         WCK_FM                        ;
  U8         BGRPARBEN                     ;
  U8         CKE_DBE_CNT                   ;
  U8         FASTWAKE2                     ;
  U8         DMPGTIM                       ;
  U8         ADVPREEN                      ;
  U8         LP5_SEP_ACT                   ;
  U8         SE_MODE                       ;
  U8         DVFS_CLK_MODE_SEL             ;
  U8         LP5_WR_DBI                    ;
  U8         LP4YEN                        ;
  U8         BC4OTF                        ;
  U8         TXECC_EN                      ;
  U8         RXECC_EN                      ;
  U8         FAKE_1TO16_MODE               ;
  U8         LP5_BGOTF                     ;
  U8         LP5_CAS_MODE                  ;
  U8         LP5_RDQS_SE_EN                ;
  U8         DQSIEN_MODE                   ;
  U8         APHY_CLOCK_MODE_EN            ;
  U8         APHY_CLOCK_MODE               ;
  U8         CKR_RATIO                     ;
  U8         DQ_P2S_RATIO                  ;
  U32        data_rate                     ;
}
#if FOR_DV_SIMULATION_USED
__attribute__((packed))
#endif
DUT_shuf_config_T;
typedef struct SA_Sim_Ctl_T {
    U32 sim_datlat;
    U32 sim_tx;
    U32 sim_rx;
    U32 sim_rddqc;
    U32 sim_dqsg;
    U32 sim_wl;
    U32 sim_cbt;
    U32 sim_duty;
    U32 sim_8ph;
    U32 sim_jm;
    U32 sim_sw_imp;
    U32 sim_init;
}
#if FOR_DV_SIMULATION_USED
__attribute__((packed))
#endif
SA_Sim_Ctl_T;

typedef struct DUT_top_set_T {
  DRAMC_CTX_T * ctx                        ; //cc: for TBA, shall add 2 dmy int field since pointer in TBA is 32bit
  #if (fcFOR_CHIP_ID == fcLepin)
  U8         PICHON_PINMUX                 ;
  U8         fcPAVIE                       ;
  #endif
  #if (fcFOR_CHIP_ID == fcFigeac) || (fcFOR_CHIP_ID == fcLepin)
  U8         PINMUX_DSC                    ;
  U8         PINMUX_MCP                    ; 
  #endif
  U8         TX_DQ_Dline_EN                ;
  U8         DRMODE                        ;
  U8         DDR_RESERVE_EN                ;
  U8         ACT_setAB_MODE                ;
  U8         DRAMOBF_EN                    ;
  U8         OPT_REF_EN                    ;
  U8         RFM_EN                        ;
  U32        dvfs_transit_point            ;
  U8         DRAMC_SHU_SRAM_MTCMOS_SEL     ;
  U8         DVFS_AUTO_HWSET_EN            ;
  U8         DVFS_AUTO_UNMUTE_EN           ;
  U32        TWCK2DQO_LF_R1_B1             ;
  U32        TWCK2DQO_LF_R1_B0             ;
  U32        TWCK2DQO_LF_R0_B1             ;
  U32        TWCK2DQO_LF_R0_B0             ;
  U32        TWCK2DQI_LF_R1_B1             ;
  U32        TWCK2DQI_LF_R1_B0             ;
  U32        TWCK2DQI_LF_R0_B1             ;
  U32        TWCK2DQI_LF_R0_B0             ;
  U32        TWCK2DQO_HF_R1_B1             ;
  U32        TWCK2DQO_HF_R1_B0             ;
  U32        TWCK2DQO_HF_R0_B1             ;
  U32        TWCK2DQO_HF_R0_B0             ;
  U32        TWCK2DQI_HF_R1_B1             ;
  U32        TWCK2DQI_HF_R1_B0             ;
  U32        TWCK2DQI_HF_R0_B1             ;
  U32        TWCK2DQI_HF_R0_B0             ;
  U32        TDQSCK_R1_B1                  ;
  U32        TDQSCK_R1_B0                  ;
  U32        TDQSCK_R0_B1                  ;
  U32        TDQSCK_R0_B0                  ;
  U32        TDQS2DQ_R1_B1                 ;
  U32        TDQS2DQ_R1_B0                 ;
  U32        TDQS2DQ_R0_B1                 ;
  U32        TDQS2DQ_R0_B0                 ;
  U8         DEF_DDR4_LP4_EN_TBA           ;
  U8         DEF_FPGA                      ;
  U8         DEF_DIGPHY                    ;
  U8         OCC_CLKCHK                    ;
  U8         DV_FAST_UP                    ;
  U8         LP5_READ_BASE_EN              ;
  U8         WCK_MCK_BYTE_DIFF             ;
  #if (fcFOR_CHIP_ID == fcLepin)
  U8         VB_ADV                        ;
  #endif
  U8         PINMUX_BYTE_SWAP_CHB          ;
  U8         PINMUX_BYTE_SWAP_CHA          ;
  U8         DMPINMUX_CHB                  ;
  U8         DMPINMUX_CHA                  ;
  U8         BYPASS_TX_PIPE_MODE           ;
  U8         ASYNC_MODE                    ;
  U8         REAL_FREQ_OPT                 ;
  U8         RDSEL_TRACK_EN                ;
  U8         DCM_NONSHUF_EN                ;
  U8         GP_NUM                        ;
  U8         TX_PIPE_BYPASS_EN             ;
  U8         RX_PIPE_BYPASS_EN             ;
  U8         SRAM_EN                       ;
  U8         MD32_EN                       ;
  U8         FORCE_DBI_OFF                 ;
  U8         DVFS_RTMRW_EN                 ;
  U8         SW_PHY_INIT_WITH_MD32         ;
  U8         LP_MTCMOS_CONTROL_SEL         ;
  U8         new_uP_spm_mode               ;
  U8         EX_ROW_EN_RK1                 ;
  U8         EX_ROW_EN_RK0                 ;
  U8         RANK_SWAP                     ;
  U8         NO_QUEUEFLUSH_EN              ;
  U8         REQQUE_DEPTH                  ;
  U8         old_dcm_mode                  ;
  U8         WAIT_DLE_EXT_DLY              ;
  U8         RX_DCM_EXT_DLY                ;
  U8         DPHY_DCM_MODE                 ;
  U8         SW_ZQCAL_OPT                  ;
  U8         DLL_IDLE_MODE                 ;
  U8         TXP_WORKAROUND_OPT            ;
  U8         WPST1P5T_OPT                  ;
  U8         AUTOPRE                       ;
  U8         LP5WRAPEN                     ;
  U8         NEW_RANK_MODE                 ;
  U8         VALID_LAT_VALUE               ;
  U8         TRACK_UP_MODE                 ;
  U8         SELPH_MODE                    ;
  U8         ACDERATEEN                    ;
  U8         BGPIPE_EN                     ;
  U8         TREFBWIG_IGNORE               ;
  U8         TX_OE_EXT_OPT                 ;
  U8         RG_SPM_MODE                   ;
  U8         RXTRACK_PBYTE_OPT             ;
  U8         DDR400_OPEN_LOOP_MODE         ;
  U8         NEW_8X_MODE                   ;
  U8         SINGLE_RANK                   ;
  U8         SINGLE_TOP                    ;
  U8         ESL_OR_RANDOM                 ;
  U32        LP45_TYPE                     ;
  U32        memory_type                   ;
}
#if FOR_DV_SIMULATION_USED
__attribute__((packed))
#endif
DUT_top_set_T;

//=========================================================
//HW DQSOSC config
//=========================================================
typedef struct DRAMC_HW_DQSOSC_CTRL
{
    U32 HW_MR                       ;
    U8  DQSOSCENDIS                 ;
    U8  DQSOSCRDIS                  ;
    U8  RK0_MRR_BYTE_SWAP[CHANNEL_NUM]           ;
    U8  RK1_MRR_BYTE_SWAP[CHANNEL_NUM]           ;
    U8  DQSOSCR_RK0EN               ;
    U8  DQSOSCR_RK1EN               ;
    U8  RK0_BYTE_MODE               ;
    U8  RK1_BYTE_MODE               ;
    U32 DQSOSCENCNT                 ;
    U8  R_DMARPIDQ_SW               ;
    U8  DQSOSC_CALEN                ;
    U16 DQSOSC_PRDCNT               ;
    U8  DQSOSCRCNT                  ;
    U8  WCK2DQ_OSC_FM_GROUP         ;
    U8  R_DMDQS2DQ_FILT_OPT         ;
    U32 DQS2DQ_FILT_PITHRD          ;
    U8  DQS2DQ_WARN_OPT             ;
    U32 DQS2DQ_WARN_PITHRD          ;
    U8  TX_RETRY_SHU_RESP_OPT       ;
    U8  TX_RETRY_SRF_RESP_OPT       ;
    U8  TX_RETRY_BLOCK_ALE_MASK     ;
    U8  SHU_LEVEL_MODE_SEL          ;
    U8  SHU_PRELOAD_TX_HW           ;
    U8  SHU_PRELOAD_TX_START        ;
    U8  SW_UP_TX_NOW_CASE           ;
    U8  MPC_BLOCKALE_OPT            ;
    U8  TXUPDMODE                   ;
    U8  MANUTXUPD                   ;
    U8  TDQS2DQ_UPD_BLOCKING        ;
    U8  TDQS2DQ_UPD_BLOCK_CNT       ;
    U8  TX_SW_FORCE_UPD_SEL         ;
    U8  SUBCLK_CTRL_TX_TRACKING     ;
    U8  HWDQSOSC_MPC_BY_RTSWCMD     ;
    U8  HWDQSOSC_MRR_BY_RTSWCMD     ;
    U8  TX_TRACK_PERIOD_NEW         ;
    U8  TXUPD_W2R_SEL               ;
    U16 DQSOSC_BASE_B0_RK[2]        ;
    U16 DQSOSC_BASE_B1_RK[2]        ;
    U16 DQSOSCTHRD_INC_B0_RK[2]     ;
    U16 DQSOSCTHRD_DEC_B0_RK[2]     ;
    U16 DQSOSCTHRD_INC_B1_RK[2]     ;
    U16 DQSOSCTHRD_DEC_B1_RK[2]     ;
} DRAMC_HW_DQSOSC_CTRL_T;

/**///
/**///
/**///
extern Gating_confg_T Gat_p;
extern DRAM_TYPE_T MEM_TYPE;
extern PC3_DRAM_CONFIG_T PC3_INIT;
extern PC4_DRAM_CONFIG_T PC4_INIT;
extern LP4_DRAM_CONFIG_T LP4_INIT;
extern LP5_DRAM_CONFIG_T LP5_INIT;
extern ANA_top_config_T ana_top_p;
extern ANA_DVFS_CORE_T ANA_option;
extern DRAMC_DVFS_GROUP_CONFIG_T DFS_TOP[DFS_GROUP_NUM];
extern DRAMC_SUBSYS_CONFIG_T DV_p;
extern DRAMC_CTX_T *DramcConfig;
extern DUT_shuf_config_T  DUTShufConfigGlobal[10];
extern DUT_top_set_T  DUTTopSetGlobal;
extern HW_FUNC_enable_T HWFUNCEnableGlobal;
extern DRAMC_HW_DQSOSC_CTRL_T HW_DQSOSC_CTRL;
#if FOR_DV_SIMULATION_USED
extern SA_Sim_Ctl_T SASimCtlGlobal;
#endif

/**///
#define A_T DV_p.a_cfg
#define A_D DV_p.a_opt
#define M_PC3 DV_p.pc3_init
#define M_PC4 DV_p.pc4_init
#define M_LP4 DV_p.lp4_init
#define M_LP5 DV_p.lp5_init
#define DFS(i) DV_p.DFS_GP[i]
#define LPDDR5_EN_S ((MEM_TYPE==LPDDR5) ? 1 : 0)
#define LPDDR4_EN_S ((MEM_TYPE==LPDDR4) ? 1 : 0)
#define PCDDR3_EN_S ((MEM_TYPE==PCDDR3) ? 1 : 0)
#define PCDDR4_EN_S ((MEM_TYPE==PCDDR4) ? 1 : 0)
/**///
#define  DUT_p DUTTopSetGlobal
#define  DUT_shu_p DUTShufConfigGlobal
#define  HW_p HWFUNCEnableGlobal

/**///
/**///
#if FOR_DV_SIMULATION_USED==1
EXTERN void fdisplay_begin(const char * xxx);
EXTERN void fdisplay_end(void);
EXTERN void register_write(int address, int data);
EXTERN void register_read(int address, int * data);
EXTERN void delay_us(u32 delta);
EXTERN void delay_ns(u32 delta);
EXTERN void timestamp_show();
EXTERN void get_time_val();
EXTERN void build_api_initial();
EXTERN void register_write_c(u32 address, u32 data);
EXTERN u32 register_read_c(u32 address);
EXTERN u32 get_time_val_c();
EXTERN void conf_to_sram_sudo(int ch_id , u8 group_id, int conf_id);
EXTERN void DvSetTopDebugVif_WckDelayEN(int ch_id, u8 group_id, int EN, int byte_id);
//================ added by Lingyun Wu 11.14 =====================
EXTERN void broadcast_on(void);
EXTERN void broadcast_off(void);
//================ added by Lingyun Wu 11.14 =====================
EXTERN void mygetscope();
EXTERN void mysetscope();
#endif
/**///
/**///
/**///    #if DV_CONFIG_EN
/**///    extern void get_dfs_configuration_from_DV_random(DRAMC_DVFS_GROUP_transfer_T *tr, int group_id);
/**///    extern void get_top_configuration_from_DV_random(DV_new_config_T *tr);
/**///    #endif
//DRAM LP4 initial configuration
extern U8 LP4_DRAM_INIT_RLWL_MRfield_config(U32 data_rate);
/**///
/**///
/**///
extern U8 LP4_DRAM_INIT_nWR_MRfield_config(U32 data_rate);

extern void MD32_initializaton(DRAMC_CTX_T *p);
extern void DRAMC_SUBSYS_PRE_CONFIG(DRAMC_CTX_T *p, DRAMC_SUBSYS_CONFIG_T *tr);
extern void SetClkFreeRun(DRAMC_CTX_T *p, U8 EN);
//extern void CKE_FIX_ON(DRAMC_CTX_T *p, U8 EN, U8 rank);
extern void CKE_FIX_OFF(DRAMC_CTX_T *p, U8 EN, U8 rank);
extern void PC3_DRAM_config(U32 data_rate, PC3_DRAM_CONFIG_T *tr);
extern void PC4_DRAM_config(U32 data_rate, PC4_DRAM_CONFIG_T *tr);
extern void LP4_DRAM_config(U32 data_rate, LP4_DRAM_CONFIG_T *tr, U8 FORCE_DBI_OFF);
extern void LP5_DRAM_config(DRAMC_DVFS_GROUP_CONFIG_T *dfs_tr, LP5_DRAM_CONFIG_T *tr, U8 group_id);
extern void ANA_TOP_FUNCTION_CFG(DRAMC_CTX_T *p, ANA_top_config_T *tr,U16 data_rate,U8 APHY_CLOCK_MODE);
extern void ANA_CLK_DIV_config( ANA_DVFS_CORE_T *tr,DRAMC_DVFS_GROUP_CONFIG_T *dfs);
extern void ANA_sequence_shuffle_colletion(DRAMC_CTX_T *p,ANA_DVFS_CORE_T *tr);
extern void ANA_Config_shuffle(DRAMC_CTX_T *p,ANA_top_config_T *a_cfg,U8 group_id);
extern void DDRPHY_PICG_Config(DRAMC_CTX_T *p, U8 MISC_CG_EN);
extern void RX_PATH_auto_gen_and_set(DRAMC_CTX_T *p,U32 ch_id, U8 group_id);
extern void TX_PATH_auto_gen_and_set(DRAMC_CTX_T *p,U32 ch_id, U8 group_id);
extern U32 Get_RL_by_MR_LP4(U8 BYTE_MODE_EN,U8 DBI_EN, U8 MR_RL_field_value);
extern U32 Get_RL_LP5_DVFSC_EN( U8 MR_RL_field_value, U8 DBI_EN, U8 BYTE_MODE_EN,U8 CKR);
extern U32 Get_RL_LP5_DVFSC_DIS( U8 MR_RL_field_value, U8 DBI_EN, U8 BYTE_MODE_EN, U8 RECC_EN, U8 RDC_EN, U8 CKR);
extern void DIG_CONFIG_SHUF_LP5_SE_MODE(DRAMC_CTX_T *p, int ch_id, U8 group_id);
extern void DIG_CONFIG_SHUF_LP4_SE_MODE(DRAMC_CTX_T *p, int ch_id, U8 group_id);

extern void DIG_HW_ATTRIBUTE_INIT(DRAMC_CTX_T *p);
extern void DIG_HW_NONSHUF_SWITCH(DRAMC_CTX_T *p,U8 ch_id);
extern void DIG_HW_NONSHUF_CFG(DRAMC_CTX_T *p,U8 ch_id);
extern void DIG_HW_SHUF_CFG(DRAMC_CTX_T *p,U8 ch_id,U8 group_id);
extern void DIG_HW_SHUF_SWITCH(DRAMC_CTX_T *p,U8 ch_id,U8 group_id);
extern void ESL_DPHY_FPGA_TX_settings(DRAMC_CTX_T *p);
extern void ESL_DPHY_FPGA_RX_settings(DRAMC_CTX_T *p);
extern void DRAMC_DMA_CONF_to_SRAM(DRAMC_CTX_T *p,U8 GP_id,U8 CONF_id);
extern void DRAMC_HW_FUNC_DQSOSC_config(DRAMC_CTX_T *p, DRAMC_HW_DQSOSC_CTRL_T *tr, U8 group_id);
extern void DIG_HW_NONSHUF_DQSOSC_CFG(DRAMC_CTX_T *p, DRAMC_HW_DQSOSC_CTRL_T *tr);
extern void DIG_HW_SHUF_DQSOSC_CFG(DRAMC_CTX_T *p, DRAMC_HW_DQSOSC_CTRL_T *tr);
extern void DIG_HW_SHUF_DQSOSC_SWITCH(DRAMC_CTX_T *p, U8 EN);
extern void DIG_CONFIG_SHUF_DCM(DRAMC_CTX_T *p, U8 ch_id, U8 group_id, U8 DCM_SHUF_EN);
extern void DIG_CONFIG_NONSHUF_DCM(DRAMC_CTX_T *p,U8 DCM_NONSHUF_EN);
extern void DramcHWFuncSet(DRAMC_CTX_T *p, HW_FUNC_enable_T *cfg_dv);
extern void DramcDUTShuSet(DRAMC_CTX_T *p, DUT_shuf_config_T *cfg_dv, U8 group_id);
extern void DramcDUTTopSet(DRAMC_CTX_T *p, DUT_top_set_T *cfg_dv);
extern void DramcDutTopUpdate(DRAMC_CTX_T *p);
extern U16 Round_Operation(U16 A, U16 B);

#if FOR_DV_SIMULATION_USED
extern void set_cbt_status(int ch_id, int on_off);
extern void set_curr_freq_group(int freq_group);
extern void DPI_entry_DDR3_AC_Timing_setting(int frequency, U8 group_id, int dramc_dram_ratio);
extern void DPI_entry_DDR4_AC_Timing_setting(int frequency, U8 group_id, int dramc_dram_ratio);
extern void fsdb_dump_start();
#endif
/**///
#endif   // _DRAMC_DV_INIT_H_
