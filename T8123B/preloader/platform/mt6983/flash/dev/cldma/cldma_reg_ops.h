#ifndef __CLDMA_REG_OPS_H__
#define __CLDMA_REG_OPS_H__

#include <driver/register_act_util.h>

/*************************************************************************************
  * Special option setting
 *************************************************************************************/

#define CLDMA_DRV_DBG_ENABLE	/* Enable cldma.c code debug log */
//#define CLDMA_DEBUG_EN	/* Enable cldma_hw code debug log */

/*************************************************************************************
  *BASE_ADDR
 *************************************************************************************/

/* Reference info ------------------
 *  @CODA: CLDMA_INDMA_AO	CLDMA0_AO_INDMA_AO_AP  [0x1004_9000]<host Tx>  CLDMA0_AO_INDMA_AO_MD  [0x1004_A000]<sAP Tx>  UL backup register for start and curr, enhance mode
 *  @CODA: CLDMA_INDMA_PD	CLDMA0_INDMA_PD_AP     [0x1021_D000]<host Tx>  CLDMA0_INDMA_PD_MD     [0x1021_E000]<sAP Tx>
 *  @CODA: CLDMA_MISC_AO	CLDMA0_AO_MISC_AO_AP   [0x1004_9800]<host IRQ> CLDMA0_AO_MISC_AO_MD   [0x1004_A800]<sAP IRQ>
 *  @CODA: CLDMA_MISC_PD	CLDMA0_MSIC_PD_AP      [0x1021_D800]<host IRQ> CLDMA0_MISC_PD_MD      [0x1021_E800]<sAP IRQ>
 *  @CODA: CLDMA_OUTDMA_AO	CLDMA0_AO_OUTDMA_AO_AP [0x1004_9400]<host Rx>  CLDMA0_AO_OUTDMA_AO_MD [0x1004_A400]<sAP Rx>
 *  @CODA: CLDMA_OUTDMA_PD	CLDMA0_OUTDMA_PD_AP    [0x1021_D400]<host Rx>  CLDMA0_OUTDMA_PD_MD    [0x1021_E400]<sAP Rx>
 * End ----------------------------- */

/* ---------- sAP section ------------------------------------------ */

/* CLDMA register */
#define CLDMA_SAP_AO_BASE				0x1004A000
#define CLDMA_SAP_PD_BASE				0x1021E000
#define CLDMA_HS_AO_BASE				0x10049000
#define CLDMA_HS_PD_BASE				0x1021D000


/* power down domain for sAP */
#define CLDMA_PD_UL_CFG_BASE_ADDR		(CLDMA_SAP_PD_BASE) //TX 0x1021E000
#define CLDMA_PD_DL_CFG_BASE_ADDR		(CLDMA_SAP_PD_BASE + 0x400) //RX 0x1021E400
#define CLDMA_PD_MISC_CFG_BASE_ADDR		(CLDMA_SAP_PD_BASE + 0x800) //CFG 0x1021E800

/* always on domain for sAP */
#define CLDMA_AO_UL_CFG_BASE_ADDR		(CLDMA_SAP_AO_BASE) //TX 0x1004A000
#define CLDMA_AO_DL_CFG_BASE_ADDR		(CLDMA_SAP_AO_BASE + 0x400) //RX 0x1004A400
#define CLDMA_AO_MISC_CFG_BASE_ADDR		(CLDMA_SAP_AO_BASE + 0x800) //CFG 0x1004A800


/* ---------- Host section ----------------------------------------- */
/* power down domain for Host */
#define HS_CLDMA_PD_UL_CFG_BASE_ADDR	(CLDMA_HS_PD_BASE) //TX 0x1021D000
#define HS_CLDMA_PD_DL_CFG_BASE_ADDR	(CLDMA_HS_PD_BASE + 0x400) //RX 0x1021D400

/* always on domain for Host */
#define HS_CLDMA_AO_UL_CFG_BASE_ADDR	(CLDMA_HS_AO_BASE) //TX 0x10049000
#define HS_CLDMA_AO_DL_CFG_BASE_ADDR	(CLDMA_HS_AO_BASE + 0x400) //RX 0x10049400


/*********************************************************************************************
  *CLDMA_PD_DL_CFG
 *********************************************************************************************/
#define REG_CLDMA_DL_ERR			((volatile unsigned int *)(CLDMA_PD_DL_CFG_BASE_ADDR + 0x0100))
#define REG_CLDMA_DL_START_WP			((volatile unsigned int *)(CLDMA_PD_DL_CFG_BASE_ADDR + 0x01BC))
#define REG_CLDMA_DL_RESUME_WP			((volatile unsigned int *)(CLDMA_PD_DL_CFG_BASE_ADDR + 0x01C0))
#define REG_CLDMA_DL_STOP_WP			((volatile unsigned int *)(CLDMA_PD_DL_CFG_BASE_ADDR + 0x01C4))


/*********************************************************************************************
  *CLDMA_PD_UL_CFG
 *********************************************************************************************/
/* sAP section ----- */
#define REG_CLDMA_UL_START_ADDRL_0		((volatile unsigned int *)(CLDMA_PD_UL_CFG_BASE_ADDR + 0x0004))
#define REG_CLDMA_UL_START_ADDRH_0		((volatile unsigned int *)(CLDMA_PD_UL_CFG_BASE_ADDR + 0x0008))

#define REG_CLDMA_UL_CURRENT_ADDRL_0    	((volatile unsigned int *)(CLDMA_PD_UL_CFG_BASE_ADDR + 0x0044))
#define REG_CLDMA_UL_CURRENT_ADDRH_0    	((volatile unsigned int *)(CLDMA_PD_UL_CFG_BASE_ADDR + 0x0048))

#define REG_CLDMA_UL_STATUS             	((volatile unsigned int *)(CLDMA_PD_UL_CFG_BASE_ADDR + 0x0084)) // que active or not
#define REG_CLDMA_UL_START_CMD_WP       	((volatile unsigned int *)(CLDMA_PD_UL_CFG_BASE_ADDR + 0x0088))
#define REG_CLDMA_UL_RESUME_CMD_WP		((volatile unsigned int *)(CLDMA_PD_UL_CFG_BASE_ADDR + 0x008C))	// Not used currently --
#define REG_CLDMA_UL_STOP_CMD_WP        	((volatile unsigned int *)(CLDMA_PD_UL_CFG_BASE_ADDR + 0x0090))
#define REG_CLDMA_UL_ERR               	 	((volatile unsigned int *)(CLDMA_PD_UL_CFG_BASE_ADDR + 0x0094))
#define REG_CLDMA_UL_CFG                	((volatile unsigned int *)(CLDMA_PD_UL_CFG_BASE_ADDR + 0x0098)) // Not used currently --

#define REG_CLDMA_UL_LAST_UPDATE_ADDRL_0	((volatile unsigned int *)(CLDMA_PD_UL_CFG_BASE_ADDR + 0x015C))	// Not used currently
#define REG_CLDMA_UL_LAST_UPDATE_ADDRH_0	((volatile unsigned int *)(CLDMA_PD_UL_CFG_BASE_ADDR + 0x0160))	// Not used currently

/* Host section ----- */
#define HS_REG_CLDMA_UL_START_ADDRL_0		((volatile unsigned int *)(HS_CLDMA_PD_UL_CFG_BASE_ADDR + 0x0004))
#define HS_REG_CLDMA_UL_START_ADDRH_0		((volatile unsigned int *)(HS_CLDMA_PD_UL_CFG_BASE_ADDR + 0x0008))

#define HS_REG_CLDMA_UL_CURRENT_ADDRL_0    	((volatile unsigned int *)(HS_CLDMA_PD_UL_CFG_BASE_ADDR + 0x0044))
#define HS_REG_CLDMA_UL_CURRENT_ADDRH_0    	((volatile unsigned int *)(HS_CLDMA_PD_UL_CFG_BASE_ADDR + 0x0048))

#define HS_REG_CLDMA_UL_STATUS			((volatile unsigned int *)(HS_CLDMA_PD_UL_CFG_BASE_ADDR + 0x0084))


/*********************************************************************************************
  *CLDMA_PD_MISC_CFG
 *********************************************************************************************/
#define REG_CLDMA_CODA_VERSION			((volatile unsigned int *)(CLDMA_PD_MISC_CFG_BASE_ADDR + 0x0000))
/* Tx part ----- */
#define REG_CLDMA_L2TISAR0			((volatile unsigned int *)(CLDMA_PD_MISC_CFG_BASE_ADDR + 0x0010))
#define REG_CLDMA_L2TISAR1			((volatile unsigned int *)(CLDMA_PD_MISC_CFG_BASE_ADDR + 0x0014))
#define REG_CLDMA_L3TISAR0			((volatile unsigned int *)(CLDMA_PD_MISC_CFG_BASE_ADDR + 0x0030))
#define REG_CLDMA_L3TISAR1			((volatile unsigned int *)(CLDMA_PD_MISC_CFG_BASE_ADDR + 0x0034))
#define REG_CLDMA_L3TISAR2			((volatile unsigned int *)(CLDMA_PD_MISC_CFG_BASE_ADDR + 0x00c0))

/* Rx part ----- */
#define REG_CLDMA_L2RISAR0			((volatile unsigned int *)(CLDMA_PD_MISC_CFG_BASE_ADDR + 0x0050))
#define REG_CLDMA_L2RISAR1			((volatile unsigned int *)(CLDMA_PD_MISC_CFG_BASE_ADDR + 0x0054))
#define REG_CLDMA_L3RISAR0			((volatile unsigned int *)(CLDMA_PD_MISC_CFG_BASE_ADDR + 0x0070))
#define REG_CLDMA_L3RISAR1			((volatile unsigned int *)(CLDMA_PD_MISC_CFG_BASE_ADDR + 0x0074))

#define REG_CLDMA_AP_IP_BUSY			((volatile unsigned int *)(CLDMA_PD_MISC_CFG_BASE_ADDR + 0x00b4))


/*********************************************************************************************
  *CLDMA_AO_DL_CFG
 *********************************************************************************************/
/* sAP section ----- */
#define REG_CLDMA_DL_CFG			((volatile unsigned int *)(CLDMA_AO_DL_CFG_BASE_ADDR + 0x0004))
#define REG_CLDMA_DL_START_ADDRL_0		((volatile unsigned int *)(CLDMA_AO_DL_CFG_BASE_ADDR + 0x0078))
#define REG_CLDMA_DL_START_ADDRH_0		((volatile unsigned int *)(CLDMA_AO_DL_CFG_BASE_ADDR + 0x007C))
#define REG_CLDMA_DL_CURRENT_ADDRL_0		((volatile unsigned int *)(CLDMA_AO_DL_CFG_BASE_ADDR + 0x00B8))
#define REG_CLDMA_DL_CURRENT_ADDRH_0		((volatile unsigned int *)(CLDMA_AO_DL_CFG_BASE_ADDR + 0x00BC))
#define REG_CLDMA_DL_STATUS			((volatile unsigned int *)(CLDMA_AO_DL_CFG_BASE_ADDR + 0x00F8)) // Que active or not
#define REG_CLDMA_DL_LAST_UPDATE_ADDRL_0	((volatile unsigned int *)(CLDMA_AO_DL_CFG_BASE_ADDR + 0x0100))
#define REG_CLDMA_DL_LAST_UPDATE_ADDRH_0	((volatile unsigned int *)(CLDMA_AO_DL_CFG_BASE_ADDR + 0x0104))

/* Host section ----- */
#define HS_REG_CLDMA_DL_START_ADDRL_0		((volatile unsigned int *)(HS_CLDMA_AO_DL_CFG_BASE_ADDR + 0x0078))
#define HS_REG_CLDMA_DL_START_ADDRH_0         	((volatile unsigned int *)(HS_CLDMA_AO_DL_CFG_BASE_ADDR + 0x007C))
#define HS_REG_CLDMA_DL_CURRENT_ADDRL_0		((volatile unsigned int *)(HS_CLDMA_AO_DL_CFG_BASE_ADDR + 0x00B8))
#define HS_REG_CLDMA_DL_CURRENT_ADDRH_0		((volatile unsigned int *)(HS_CLDMA_AO_DL_CFG_BASE_ADDR + 0x00BC))
#define HS_REG_CLDMA_DL_STATUS			((volatile unsigned int *)(HS_CLDMA_AO_DL_CFG_BASE_ADDR + 0x00F8))


/*********************************************************************************************
  *CLDMA_AO_UL_CFG
 *********************************************************************************************/
#define REG_CLDMA_UL_START_ADDR_BKL_0		((volatile unsigned int *)(CLDMA_AO_UL_CFG_BASE_ADDR + 0x0004))
#define REG_CLDMA_UL_START_ADDR_BKH_0		((volatile unsigned int *)(CLDMA_AO_UL_CFG_BASE_ADDR + 0x0008))


/*********************************************************************************************
  *CLDMA_AO_MISC_CFG
 *********************************************************************************************/
#define REG_CLDMA_L2RIMR0			((volatile unsigned int *)(CLDMA_AO_MISC_CFG_BASE_ADDR + 0x0058))
#define REG_CLDMA_L2RIMR1			((volatile unsigned int *)(CLDMA_AO_MISC_CFG_BASE_ADDR + 0x005C))
#define REG_CLDMA_L2RIMCR0			((volatile unsigned int *)(CLDMA_AO_MISC_CFG_BASE_ADDR + 0x0060))
#define REG_CLDMA_L2RIMCR1			((volatile unsigned int *)(CLDMA_AO_MISC_CFG_BASE_ADDR + 0x0064))
#define REG_CLDMA_L2RIMSR0			((volatile unsigned int *)(CLDMA_AO_MISC_CFG_BASE_ADDR + 0x0068))
#define REG_CLDMA_L2RIMSR1			((volatile unsigned int *)(CLDMA_AO_MISC_CFG_BASE_ADDR + 0x006C))
#define REG_CLDMA_ADDR_REMAP_FROM		((volatile unsigned int *)(CLDMA_AO_MISC_CFG_BASE_ADDR + 0x0144))
#define REG_CLDMA_ADDR_REMAP_TO			((volatile unsigned int *)(CLDMA_AO_MISC_CFG_BASE_ADDR + 0x0148))
#define REG_CLDMA_BUSY_MASK			((volatile unsigned int *)(CLDMA_AO_MISC_CFG_BASE_ADDR + 0x0154))
#define REG_CLDMA_INT_SAP_MASK			((volatile unsigned int *)(CLDMA_AO_MISC_CFG_BASE_ADDR + 0x0158))
#define REG_CLDMA_INT_MD_MASK			((volatile unsigned int *)(CLDMA_AO_MISC_CFG_BASE_ADDR + 0x015C))
#define REG_CLDMA_DOMAIN_ID			((volatile unsigned int *)(CLDMA_AO_MISC_CFG_BASE_ADDR + 0x016C))
#define REG_CLDMA_DOMAIN_ID_LOCK		((volatile unsigned int *)(CLDMA_AO_MISC_CFG_BASE_ADDR + 0x0170))


/*********************************************************************************************
 * macro define
 *********************************************************************************************/
/* current domain id  value isn't assigned to subsystem, need trace and update it before tapeout */
/* domain id */
#define DOMAIN_ID_SAP    			0
#define DOMAIN_ID_PCIE  			5
#define DOMAIN_ID_MD     			7
 
#define  DOMAIN_ID_LEFT_HAND_MASK		(0xFF)
#define  DOMAIN_ID_RIGHT_HAND_MASK		(0xFF << 8)
#define  DOMAIN_ID_PCIE_MASK                	(0xFF << 16)
#define  DOMAIN_ID_MD_MASK                  	(0xFF << 24)

#define  DOMAIN_ID_LEFT_HAND_SHIFT		0
#define  DOMAIN_ID_RIGHT_HAND_SHIFT		8
#define  DOMAIN_ID_PCIE_SHIFT              	16
#define  DOMAIN_ID_MD_SHIFT                 	24


/* ERRINFO -- REG_CLDMA_DL_ERR bitmap */
#define CLDMA_RX_LDMU_ERR_SHIFT 		0


/* L2TISAR0 bitmap */
#define CLDMA_TX0_INT_DONE 	 		0x1
#define CLDMA_TX0_QE_INT_SHIFT 	 		8
#define CLDMA_TX0_ERR_SHIFT 	 		16	// Should check L3 status for more details
#define CLDMA_TX0_ACTIVE_START_ERR_SHIFT  	24
/* L2TISAR1 bitmap */
#define CLDMA_TX0_ACTIVE_LD_TC_ERR_SHIFT 	0	// Traffic control phase out
#define CLDMA_TX0_SPD_PKT_NUM_MIS_ERR_SHIFT 	8
#define CLDMA_RX0_INDMA_START_CMD_SHIFT		16	// Notify RX that TX start command write
/* L3TISAR0 bitmap */
#define CLDMA_TX0_GPD_CSERR_SHIFT 		0	// Phase out
#define CLDMA_TX0_BD_CSERR_SHIFT 		8	// Phase out
#define CLDMA_TX0_GPD_LENERR_SHIFT 		16
#define CLDMA_TX0_BD_LENERR_SHIFT 		24
/* L3TISAR1 bitmap */
#define CLDMA_TX0_GPD_64KERR_SHIFT		0
#define CLDMA_TX0_BD_64KERR_SHIFT		8
#define CLDMA_TX0_GPD_DATA_LEN_MIS_ERR_SHIFT	16
#define CLDMA_TX0_SPD_ZERO_PKT_NUM_ERR_SHIFT	24
/* L3TISAR2 bitmap */
#define CLDMA_TX0_SPD_64K_LEN_ERR_SHIFT		0
#define CLDMA_TX0_ILLEGAL_MEMORY_ACCESS_SHIFT	8
#define CLDMA_TX0_SPD_HEADER_ROOM_LEN_ERR_SHIFT	16


enum cldma_tx_err_code_bit {
	TX_DONE_INT = 0,
	TX_QE_INT = 1,
	TX_ERR = 2,
	TX_ACTIVE_START_ERR = 3,
	//TX_ACTIVE_LD_TC_ERR = 2,  // Traffic control phase out
	//TX_SPD_PKT_NUM_MIS_ERR = 4, // SPD not used
	TX_RX_INDMA_START_CMD = 4,
	TX_GPD_CS_ERR = 5,
	TX_BD_CS_ERR = 6,
	TX_GPD_LEN_ERR = 7,
	TX_BD_LEN_ERR = 8,
	TX_GPD_64K_ERR = 9,
	TX_BD_64K_ERR = 10,
	TX_GPD_DATA_LEN_MIS_ERR = 11,
	TX_SPD_ZERO_PKT_NUM_ERR = 12,
	TX_SPD_64K_LEN_ERR = 13,
	TX_ILLEGAL_MEM_ACCESS_ERR = 14,
	TX_SPD_HEADER_ROOM_LEN_ERR = 15,
};


/* L2RISAR0 bitmap */
#define CLDMA_RX0_INT_DONE  	 		0x1
#define CLDMA_RX0_QE_SHIFT 	 		8
#define CLDMA_RX0_ERR_SHIFT 	 		16	// Should check L3RISAR1
#define CLDMA_RX0_ACTIVE_START_ERR_SHIFT 	24

/* L2RISAR1 bitmap */
#define CLDMA_RX0_INACTIVE_ERR_SHIFT		0

/* L3RISAR0 bitmap */
#define CLDMA_RX0_GPD_CSERR_SHIFT		0	// Phase out
#define CLDMA_RX0_BD_CSERR_SHIFT 		8	// Phase out
#define CLDMA_RX0_GPD_LENERR_SHIFT		16
#define CLDMA_RX0_BD_LENERR_SHIFT		24

/* L3RISAR1 bitmap */
#define CLDMA_RX0_ALLEN_ERR_SHIFT		0
#define CLDMA_RX0_FIFO_LEN_MIS_ERR_SHIFT 	8	// Phase out
#define CLDMA_RX0_ILLEGAL_MEMORY_ACCESS_ERR_SHIFT 	16
#define CLDMA_RBIDX_ERR_SHIFT 			31	// Phase out


enum {
	RX_DONE_INT = 0,
	RX_ERR = 1,
	RX_QE_INT = 2,
	RX_ACTIVE_START_ERR = 3,
	RX_INACTIVE_ERR = 4,
	RX_GPD_CSERR = 5,
	RX_BD_CSERR = 6,
	RX_GPD_LENERR = 7,
	RX_BD_LENERR = 8,
	RX_ALLEN_ERR = 9,
	RX_FIFO_LEN_MIS_ERR = 10,
	RX_ILLEGAL_MEMORY_ACCESS_ERR = 11,
	RX_RBIDX_ERR = 12,
	RX_LDMU_ERR = 13,
};


/*********************************************************************************************
 * cldma operate function
 *********************************************************************************************/
void cldma_reg_set_domain_id(void);
void cldma_rx_start_addr(void *gpd);
void cldma_tx_start_addr(void *gpd);
void cldma_start_tx_queue(void);
void cldma_stop_tx_queue(void);
void cldma_start_rx_queue(void);
void cldma_stop_rx_queue(void);
unsigned int cldma_tx_active_status(void);
unsigned int cldma_rx_active_status(void);
void cldma_clear_tx_int_status(void);
void cldma_clear_rx_int_status(void);
void cldma_clr_tx_fsm_err(void);
unsigned int cldma_get_tx_fsm_err(void);
void cldma_clr_rx_fsm_err(void);
unsigned int cldma_get_rx_fsm_err(void);
void cldma_clear_interrupt_status(void);
void cldma_stop_all_queues(void);
void cldma_clear_rx_done_status(void);
unsigned int cldma_tx_int_status(void);
unsigned int cldma_rx_int_status(void);

 #if 0
 




U32 cldma_tx_active_status(void);
U8 cldma_tx_done(void);
void cldma_clear_tx_done_status(void);
U32 cldma_tx_status(void);








U8 cldma_rx_done(void);

U32 cldma_rx_status(void);



#endif


#endif
