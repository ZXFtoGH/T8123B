#ifndef _SRCLKEN_RC_H_
#define _SRCLKEN_RC_H_

/***********************************************************************************
** Definitions
************************************************************************************/

#define MTK_SRCLKEN_RC_BRINGUP		(0)
#define MTK_SRCLKEN_RC_SUPPORT
#ifdef MTK_SRCLKEN_RC_SUPPORT
#define MTK_SRCLKEN_RC_FULL_SET		(1)
#endif
#define SRCLKEN_DBG			(1)


#define AP_BASE				(0x1000C000)
#define RC_BASE				(0x1000F800)
#define RC_STATE_BASE			(0x1000F900)

#if !defined(PMIF_SPI_BASE)
#define PMIF_SPI_BASE			(0x10026000)
#endif /* !defined(PMIF_SPI_BASE) */

#if !defined(PMIF_SPMI_BASE)
#define PMIF_SPMI_BASE			(0x10027000)
#endif /* !defined(PMIC_SPMI_BASE) */

#if !defined(PMIF_SPMI2_BASE)
#define PMIF_SPMI2_BASE			(0x10024000)
#endif /* !defined(PMIF_SPMI2_BASE) */

#if !defined(SPM_BASE)
#define SPM_BASE			(0x10006000)
#endif /* !defined(SPM_BASE) */

/* PMIF Register*/
#define PMIFSPI_INF_EN (PMIF_SPI_BASE + 0x0024)
	#define PMIFSPI_INF_EN_SRCLKEN_RC_HW_MSK 0x1
	#define PMIFSPI_INF_EN_SRCLKEN_RC_HW_SHFT 4

#define PMIFSPI_OTHER_INF_EN (PMIF_SPI_BASE + 0x0028)
	#define OTHER_INF_DXCO0_EN_MSK 0x1
	#define OTHER_INF_DXCO0_EN_SHFT 0
	#define OTHER_INF_DXCO1_EN_MSK 0x1
	#define OTHER_INF_DXCO1_EN_SHFT 1

#define PMIFSPI_DCXO_CMD_ADDR0 (PMIF_SPI_BASE + 0x005C)
	#define DCXO_CMD_ADDR0_0_MSK 0xffff
	#define DCXO_CMD_ADDR0_0_SHFT 0
	#define DCXO_CMD_ADDR0_1_MSK 0xffff
	#define DCXO_CMD_ADDR0_1_SHFT 16

#define PMIFSPI_DCXO_CMD_WDATA0 (PMIF_SPI_BASE + 0x0060)
	#define DCXO_CMD_WDATA0_0_MSK 0xffff
	#define DCXO_CMD_WDATA0_0_SHFT 0
	#define DCXO_CMD_WDATA0_1_MSK 0xffff
	#define DCXO_CMD_WDATA0_1_SHFT 16

#define PMIFSPI_DCXO_CMD_ADDR1 (PMIF_SPI_BASE + 0x0064)
	#define DCXO_CMD_ADDR1_0_MSK 0xffff
	#define DCXO_CMD_ADDR1_0_SHFT 0
	#define DCXO_CMD_ADDR1_1_MSK 0xffff
	#define DCXO_CMD_ADDR1_1_SHFT 16

#define PMIFSPI_DCXO_CMD_WDATA1 (PMIF_SPI_BASE + 0x0068)
	#define DCXO_CMD_WDATA1_0_MSK 0xffff
	#define DCXO_CMD_WDATA1_0_SHFT 0
	#define DCXO_CMD_WDATA1_1_MSK 0xffff
	#define DCXO_CMD_WDATA1_1_SHFT 16

#define PMIFSPI_ARB_EN (PMIF_SPI_BASE + 0x0150)
	#define PMIFSPI_ARB_EN_SRCLKEN_RC_HW_MSK 0x1
	#define PMIFSPI_ARB_EN_SRCLKEN_RC_HW_SHFT 4
	#define PMIFSPI_ARB_EN_DCXO_CONN_MSK 0x1
	#define PMIFSPI_ARB_EN_DCXO_CONN_SHFT 15
	#define PMIFSPI_ARB_EN_DCXO_NFC_MSK 0x1
	#define PMIFSPI_ARB_EN_DCXO_NFC_SHFT 16

#define PMIFSPI_SLEEP_PROTECTION_CRL (PMIF_SPI_BASE + 0x03F0)
	#define PMIFSPI_SPM_SLEEP_REQ_SEL_MSK 0x3
	#define PMIFSPI_SPM_SLEEP_REQ_SEL_SHFT 0
	#define PMIFSPI_SCP_SLEEP_REQ_SEL_MSK 0x3
	#define PMIFSPI_SCP_SLEEP_REQ_SEL_SHFT 9

#define PMIFSPI_MODE_CRL (PMIF_SPI_BASE + 0x0408)
	#define PMIFSPI_MD_CTL_PMIF_RDY_MSK 0x1
	#define PMIFSPI_MD_CTL_PMIF_RDY_SHFT 9
	#define PMIFSPI_MD_CTL_SRCLK_EN_MSK 0x1
	#define PMIFSPI_MD_CTL_SRCLK_EN_SHFT 10
	#define PMIFSPI_MD_CTL_SRVOL_EN_MSK 0x1
	#define PMIFSPI_MD_CTL_SRVOL_EN_SHFT 11

#define PMIFSPMI_SLEEP_PROTECTION_CRL (PMIF_SPMI_BASE + 0x03F0)
	#define PMIFSPMI_SPM_SLEEP_REQ_SEL_MSK 0x3
	#define PMIFSPMI_SPM_SLEEP_REQ_SEL_SHFT 0
	#define PMIFSPMI_SCP_SLEEP_REQ_SEL_MSK 0x3
	#define PMIFSPMI_SCP_SLEEP_REQ_SEL_SHFT 9

#define PMIFSPMI_MODE_CRL (PMIF_SPMI_BASE + 0x0408)
	#define PMIFSPMI_MD_CTL_PMIF_RDY_MSK 0x1
	#define PMIFSPMI_MD_CTL_PMIF_RDY_SHFT 9
	#define PMIFSPMI_MD_CTL_SRCLK_EN_MSK 0x1
	#define PMIFSPMI_MD_CTL_SRCLK_EN_SHFT 10
	#define PMIFSPMI_MD_CTL_SRVOL_EN_MSK 0x1
	#define PMIFSPMI_MD_CTL_SRVOL_EN_SHFT 11

#define PMIFSPMI2_SLEEP_PROTECTION_CRL (PMIF_SPMI2_BASE + 0x03F0)
	#define PMIFSPMI2_SPM_SLEEP_REQ_SEL_MSK 0x3
	#define PMIFSPMI2_SPM_SLEEP_REQ_SEL_SHFT 0
	#define PMIFSPMI2_SCP_SLEEP_REQ_SEL_MSK 0x3
	#define PMIFSPMI2_SCP_SLEEP_REQ_SEL_SHFT 9

#define PMIFSPMI2_MODE_CRL (PMIF_SPMI2_BASE + 0x0408)
	#define PMIFSPMI2_MD_CTL_PMIF_RDY_MSK 0x1
	#define PMIFSPMI2_MD_CTL_PMIF_RDY_SHFT 9
	#define PMIFSPMI2_MD_CTL_SRCLK_EN_MSK 0x1
	#define PMIFSPMI2_MD_CTL_SRCLK_EN_SHFT 10
	#define PMIFSPMI2_MD_CTL_SRVOL_EN_MSK 0x1
	#define PMIFSPMI2_MD_CTL_SRVOL_EN_SHFT 11

/* SPM Register */
#define ULPOSC_CON			(SPM_BASE + 0x0440)
    #define ULPOSC_EN_SHFT		0
    #define ULPOSC_RST_SHFT		1
    #define ULPOSC_CG_EN_SHFT		2
    #define ULPOSC_CLK_SEL_SHFT		3

#define SRCLKEN_RC_CFG			(RC_BASE + 0x0000)
    #define SW_RESET_MSK		0x1
    #define SW_RESET_SHFT		0
    #define CG_32K_EN_MSK		0x1
    #define CG_32K_EN_SHFT		1
    #define CG_FCLK_EN_MSK		0x1
    #define CG_FCLK_EN_SHFT		2
	#define CG_FCLK_FR_EN_MSK	0x1
	#define CG_FCLK_FR_EN_SHFT	3
	#define MUX_FCLK_FR_MSK		0x1
	#define MUX_FCLK_FR_SHFT	4
	#define RC_32K_DCM_MSK 		0x1
	#define RC_32K_DCM_SHFT		8


#define RC_CENTRAL_CFG1			(RC_BASE + 0x0004)
    #define SRCLKEN_RC_EN_MSK		0x1
    #define SRCLKEN_RC_EN_SHFT		0
    #define RCEN_ISSUE_M_MSK		0x1
    #define RCEN_ISSUE_M_SHFT		1
    #define RC_SPI_ACTIVE_MSK		0x1
    #define RC_SPI_ACTIVE_SHFT		2
    #define SRCLKEN_RC_EN_SEL_MSK	0x1
    #define SRCLKEN_RC_EN_SEL_SHFT	3
    #define VCORE_SETTLE_T_MSK		0x7
    #define VCORE_SETTLE_T_SHFT		5
    #define ULPOSC_SETTLE_T_MSK		0xf
    #define ULPOSC_SETTLE_T_SHFT	8
    #define NON_DCXO_SETTLE_T_MSK	0x3ff
    #define NON_DCXO_SETTLE_T_SHFT	12
    #define DCXO_SETTLE_T_MSK		0x3ff
    #define DCXO_SETTLE_T_SHFT		22

#define RC_CENTRAL_CFG2			(RC_BASE + 0x0008)
    #define SRCVOLTEN_CTRL_MSK		0xf
    #define SRCVOLTEN_CTRL_SHFT		0
    #define VREQ_CTRL_MSK		0xf
    #define VREQ_CTRL_SHFT		4
    #define SRCVOLTEN_VREQ_SEL_MSK	0x1
    #define SRCVOLTEN_VREQ_SEL_SHFT	8
    #define SRCVOLTEN_VREQ_M_MSK	0x1
    #define SRCVOLTEN_VREQ_M_SHFT	9
    #define FORCE_SRCVOLTEN_OFF_MSK	0x1
    #define FORCE_SRCVOLTEN_OFF_SHFT	10
    #define FORCE_SRCVOLTEN_ON_MSK	0x1
    #define FORCE_SRCVOLTEN_ON_SHFT	11
    #define ULPOSC_CTRL_M_MSK		0xf
    #define ULPOSC_CTRL_M_SHFT		12
    #define FORCE_VCORE_RDY_MSK		0x1
    #define FORCE_VCORE_RDY_SHFT	16
    #define FORCE_ULPOSC2ON_MSK		0x1
    #define FORCE_ULPOSC2ON_SHFT	17
    #define FORCE_ULPOSC_CLK_EN_MSK	0x1
    #define FORCE_ULPOSC_CLK_EN_SHFT	18
    #define FORCE_ULPOSC_ON_MSK		0x1
    #define FORCE_ULPOSC_ON_SHFT	19
    #define DIS_ULPOSC_RDY_CHK_MSK	0x1
    #define DIS_ULPOSC_RDY_CHK_SHFT	20
    #define PWRAP_SLP_CTRL_M_MSK	0xf
    #define PWRAP_SLP_CTRL_M_SHFT	21
    #define PWRAP_SLP_MUX_SEL_MSK	0x1
    #define PWRAP_SLP_MUX_SEL_SHFT	25
    #define FORCE_PWRAP_ON_MSK		0x1
    #define FORCE_PWRAP_ON_SHFT		26
    #define FORCE_PWRAP_AWK_MSK  	0x1
    #define FORCE_PWRAP_AWK_SHFT	27
    #define NON_DCXO_REQ_FORCEON_MSK	0x1
    #define NON_DCXO_REQ_FORCEON_SHFT	28
    #define NON_DCXO_REQ_FORCEOFF_MSK	0x1
    #define NON_DCXO_REQ_FORCEOFF_SHFT	29
    #define DCXO_REQ_FORCEON_MSK	0x1
    #define DCXO_REQ_FORCEON_SHFT	30
    #define DCXO_REQ_FORCEOFF_MSK	0x1
    #define DCXO_REQ_FORCEOFF_SHFT	31
 
#define RC_CMD_ARB_CFG			(RC_BASE + 0x000C)
    #define SW_RC_EN_MSK		0x1fff
    #define SW_RC_EN_SHFT		0
    #define SW_RCEN_EN_MSK		0x1fff
    #define SW_RCEN_EN_SHFT		13
    #define SW_DCXO_M_EN_MSK		0x1
    #define SW_DCXO_M_EN_SHFT		28
    #define SW_DCXO_M_MSK		0x7
    #define SW_DCXO_M_SHFT		29

#define RC_PMIC_RCEN_ADDR		(RC_BASE + 0x0010)
#define RC_PMIC_RCEN_SET_CLR_ADDR	(RC_BASE + 0x0014)
#define RC_DCXO_FPM_CFG			(RC_BASE + 0x0018)
    #define DCXO_FPM_CTRL_M_MSK		0xf
    #define DCXO_FPM_CTRL_M_SHFT	0
    #define SRCVOLTEN_FPM_MSK_B_MSK	0x1
    #define SRCVOLTEN_FPM_MSK_B_SHFT	4
    #define SUB_SRCLKEN_FPM_MSK_B_MSK	0x1fff
    #define SUB_SRCLKEN_FPM_MSK_B_SHFT	16

#define RC_CENTRAL_CFG3			(RC_BASE + 0x001C)
    #define TO_LPM_SETTLE_EN_MSK	0x1
    #define TO_LPM_SETTLE_EN_SHFT	0
    #define BLK_SCP_DXCO_MD_TARGET_MSK 	0x1
    #define BLK_SCP_DXCO_MD_TARGET_SHFT	1
    #define BLK_COANT_DXCO_MD_TARGET_MSK 	0x1
    #define BLK_COANT_DXCO_MD_TARGET_SHFT	2
	#define TO_BBLPM_SETTLE_EN_MSK 0x1
	#define TO_BBLPM_SETTLE_EN_SHFT 0x3
	#define TO_BBLPM_SETTLE_ND_EN_MSK 0x1
	#define TO_BBLPM_SETTLE_ND_EN_SHFT 0x4
    #define TO_LPM_SETTLE_T_MSK		0x2ff
    #define TO_LPM_SETTLE_T_SHFT	12

#define RC_MXX_SRCLKEN_CFG		(RC_BASE + 0x0020)
    #define DCXO_SETTLE_BLK_EN_MSK	0x1
    #define DCXO_SETTLE_BLK_EN_SHFT	1
    #define BYPASS_CMD_EN_MSK		0x1
    #define BYPASS_CMD_EN_SHFT		2
    #define SW_SRCLKEN_RC_MSK		0x1
    #define SW_SRCLKEN_RC_SHFT		3
    #define SW_SRCLKEN_FPM_MSK		0x1
    #define SW_SRCLKEN_FPM_SHFT		4
    #define SW_SRCLKEN_BBLPM_MSK	0x1
    #define SW_SRCLKEN_BBLPM_SHFT	5
    #define XO_SOC_LINK_EN_MSK		0x1
    #define XO_SOC_LINK_EN_SHFT		6
    #define REQ_ACK_LOW_IMD_EN_MSK	0x1
    #define REQ_ACK_LOW_IMD_EN_SHFT	7
    #define SRCLKEN_TRACK_M_EN_MSK	0x1
    #define SRCLKEN_TRACK_M_EN_SHFT	8
    #define CNT_PRD_STEP_MSK		0x3
    #define CNT_PRD_STEP_SHFT		10
    #define XO_STABLE_PRD_MSK		0x3ff
    #define XO_STABLE_PRD_SHFT		12
    #define DCXO_STABLE_PRD_MSK		0x3ff
    #define DCXO_STABLE_PRD_SHFT	22

#define RC_DEBUG_TRACE			(RC_BASE + 0x0054)
	#define TACE_EN_MSK 0x1
	#define TACE_EN_SHFT 0

#define RC_CENTRAL_CFG4			(RC_BASE + 0x0058)
	#define KEEP_RC_SPI_ACTIVE_MSK 0x1ff
	#define KEEP_RC_SPI_ACTIVE_SHFT 0
	#define PWRAP_VLD_FORCE_MAK 0x1
	#define PWRAP_VLD_FORCE_SHFT 16
	#define SLEEP_VLD_MODE_MAK 0x1
	#define SLEEP_VLD_MODE_SHFT 17
	#define SCP_SLEEP_REQ_MODE_MAK 0x1
	#define SCP_SLEEP_REQ_MODE_SHFT 18
	#define SLEEP_REQ_MODE_MSK 0x1
	#define SLEEP_REQ_MODE_SHFT 20

#define RC_DEBUG_CFG		(RC_BASE + 0x0064)
	#define TRACE_MODE_EN_MSK 0x1
	#define TRACE_MODE_EN_SHFT 24
	#define DBG_STOP_PROT_EN_MSK 0x1
	#define DBG_STOP_PROT_EN_SHFT 28

#define SUBSYS_INTF_CFG			(RC_BASE + 0x00BC)
    #define SRCLKEN_FPM_MASK_B_MSK	0x1fff
    #define SRCLKEN_FPM_MASK_B_SHFT	0
    #define SRCLKEN_BBLPM_MASK_B_MSK	0x1fff
    #define SRCLKEN_BBLPM_MASK_B_SHFT	16

#define RC_PI_PO_STA		(RC_STATE_BASE + 0x0010)
#define RC_MXX_REQ_STA_0		(RC_STATE_BASE + 0x0014)
	#define FPM_ACK_MSK 0x1
	#define FPM_ACK_SHFT 1
	#define BBLPM_ACK_MSK		0x1
	#define BBLPM_ACK_SHFT		3

#define RC_SPI_STA_0			(RC_STATE_BASE + 0x000C)

/*#define TEST_UFS				(UFS_HCI_BASE + 0x0144)*/

typedef struct {
    u32    osc_div;
    u32    cali_val;
} ulposc_con_t;

enum {
    ULPOSC_DIV_1 = 0,
    ULPOSC_DIV_4 = 1
};

enum chn_id {
	CHN_SUSPEND = 0,
	CHN_RF = 1,
	CHN_DEEPIDLE = 2,
	CHN_MD= 3,
	CHN_GPS = 4,
	CHN_BT = 5,
	CHN_WIFI = 6,
	CHN_MCU = 7,
	CHN_COANT = 8,
	CHN_NFC = 9,
	CHN_UFS = 10,
	CHN_SCP = 11,
	CHN_RESERVE = 12,
	MAX_CHN_NUM,
};

enum {
	SRCLKENAO_MODE,
	VREQ_MODE,
};

enum {
	MERGE_OR_MODE		= 0x0,
	BYPASS_MODE		= 0x1,
	MERGE_AND_MODE 		= 0x1 << 1,
	BYPASS_RC_MODE 		= 0x2 << 1,
	BYPASS_OR_MODE		= 0x3,
	BYPASS_OTHER_MODE 	= 0x3 << 1,
	ASYNC_MODE		= 0x1 << 3,
};

enum {
	RC_32K = 0,
	RC_ULPOSC1,
};

enum rc_ctrl_m {
	HW_MODE = 0,
	SW_MODE = 1,
	INIT_MODE = 0xff,
};

enum rc_ctrl_r {
	NO_REQ = 0,
	FPM_REQ = 1 << SW_SRCLKEN_FPM_SHFT,
	BBLPM_REQ = 1 << SW_SRCLKEN_BBLPM_SHFT,
};

enum pmic_interface {
	PMIF_VLD_RDY = 0,
	PMIF_SLP_REQ,
	PMIF_MAX,
};

enum {
	SRLCKEN_RC_BRINGUP = 0,
	SRCLKEN_RC_DISABLE,
	SRCLKEN_RC_ENABLE,
	SRCLKEN_RC_SKIP,
};

struct subsys_rc_con {
	enum chn_id id;
	unsigned int dcxo_prd;
	unsigned int xo_prd;
	unsigned int cnt_step;
	unsigned int track_en;
	unsigned int req_ack_imd_en;
	unsigned int xo_soc_link_en;
	unsigned int sw_bblpm;
	unsigned int sw_fpm;
	unsigned int sw_rc;
	unsigned int bypass_cmd;
	unsigned int dcxo_settle_blk_en;
};

#if 0
struct subsys_rc_ctrl {
	enum chn_id id;
	enum rc_ctrl_m mode;
	enum rc_ctrl_r req;
};

struct rc_subsys_con {
	unsigned int dcxo_prd;
	unsigned int xo_prd;
	unsigned int cnt_step;
	bool track_en;
	bool req_ack_imd_en;
	bool xo_soc_link_en;
	bool sw_bblpm;
	bool sw_fpm;
	bool sw_rc;
	bool bypass_settle;
};
#endif

#define RC_SUBSYS_SET(_dcxo_prd, _xo_prd, _cnt_step, _track_en, _req_ack_imd_en,	\
		_xo_soc_link_en, _sw_bblpm, _sw_fpm, _sw_rc, _bypass_settle) {	\
			.dcxo_prd = _dcxo_prd,		\
			.xo_prd = _xo_prd,		\
			.cnt_step = _cnt_step,		\
			.track_en = _track_en,		\
			.req_ack_imd_en = _req_ack_imd_en,	\
			.xo_soc_link_en = _xo_soc_link_en,	\
			.sw_bblpm = _sw_bblpm,		\
			.sw_fpm = _sw_fpm,		\
			.sw_rc = _sw_rc,		\
			.bypass_settle = _bypass_settle,	\
		}

extern void rc_ctrl_mode_switch_hw(enum chn_id id);
extern void rc_ctrl_mode_switch_sw(enum chn_id id);
extern int srclken_rc_init(void);
#endif	/* _SRCLKEN_RC_H_ */
