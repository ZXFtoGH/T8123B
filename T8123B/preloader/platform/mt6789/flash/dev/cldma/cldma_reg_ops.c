#include "cldma_reg_ops.h"


#ifdef CLDMA_DEBUG_EN
#define DBG_LOG(...)	do{print(__VA_ARGS__);}while(0)
#else
#define DBG_LOG(...)
#endif

void cldma_reg_set_domain_id(void)
{
   unsigned int val = 0;

    val = (DOMAIN_ID_PCIE << DOMAIN_ID_LEFT_HAND_SHIFT) |
        (DOMAIN_ID_SAP << DOMAIN_ID_RIGHT_HAND_SHIFT) |
        (DOMAIN_ID_PCIE << DOMAIN_ID_PCIE_SHIFT) |
        (DOMAIN_ID_MD << DOMAIN_ID_MD_SHIFT);
    DRV_WriteReg32(val, REG_CLDMA_DOMAIN_ID);
}


void cldma_clr_tx_fsm_err(void)
{
	unsigned int reg;

	reg = DRV_Reg32(REG_CLDMA_UL_ERR);
	if (reg & 1) {// Bit 0, LDMU_ERR_STATUS
		reg |= 1 << 1; // LDMU_ERR_EXIT, forse exit ERROR state
		DRV_WriteReg32(REG_CLDMA_UL_ERR, reg);
	}
}

unsigned int cldma_get_tx_fsm_err(void)
{
	return DRV_Reg32(REG_CLDMA_UL_ERR) & 0x1; // Bit0: LDMU_ER_STATUS
}

void cldma_clr_rx_fsm_err(void)
{
	unsigned int reg;

	reg = DRV_Reg32(REG_CLDMA_DL_ERR);
	if (reg & 1) {// Bit 0, LDMU_ERR_STATUS
		reg |= 1 << 1; // LDMU_ERR_EXIT, forse exit ERROR state
		DRV_WriteReg32(REG_CLDMA_DL_ERR, reg);
	}
}

unsigned int cldma_get_rx_fsm_err(void)
{
	return DRV_Reg32(REG_CLDMA_DL_ERR) & 0x1; // Bit0: LDMU_ER_STATUS
}

void cldma_clear_tx_int_status(void)
{
	DRV_WriteReg32(REG_CLDMA_L2TISAR0, DRV_Reg32(REG_CLDMA_L2TISAR0));
	DRV_WriteReg32(REG_CLDMA_L2TISAR1, DRV_Reg32(REG_CLDMA_L2TISAR1));
	DRV_WriteReg32(REG_CLDMA_L3TISAR0, DRV_Reg32(REG_CLDMA_L3TISAR0));
	DRV_WriteReg32(REG_CLDMA_L3TISAR1, DRV_Reg32(REG_CLDMA_L3TISAR1));
	DRV_WriteReg32(REG_CLDMA_L3TISAR2, DRV_Reg32(REG_CLDMA_L3TISAR2));
}

void cldma_clear_rx_int_status(void)
{
	DRV_WriteReg32(REG_CLDMA_L2RISAR0, DRV_Reg32(REG_CLDMA_L2RISAR0));
	DRV_WriteReg32(REG_CLDMA_L2RISAR1, DRV_Reg32(REG_CLDMA_L2RISAR1));
	DRV_WriteReg32(REG_CLDMA_L3RISAR0, DRV_Reg32(REG_CLDMA_L3RISAR0));
	DRV_WriteReg32(REG_CLDMA_L3RISAR1, DRV_Reg32(REG_CLDMA_L3RISAR1));
}

void cldma_clear_interrupt_status(void)
{
	cldma_clear_tx_int_status();
	cldma_clear_rx_int_status();
}

void cldma_stop_all_queues(void)
{
	DRV_WriteReg32(REG_CLDMA_UL_STOP_CMD_WP, 0xFF);
	DRV_WriteReg32(REG_CLDMA_DL_STOP_WP, 0xFF);
}

void cldma_tx_start_addr(void *gpd)
{
	DRV_WriteReg32(REG_CLDMA_UL_START_ADDRL_0, (unsigned int)gpd);
	DRV_WriteReg32(REG_CLDMA_UL_START_ADDRH_0, 0);
}

void cldma_start_tx_queue(void)
{
	DRV_WriteReg32(REG_CLDMA_UL_START_CMD_WP, 0x1);
}


void cldma_stop_tx_queue(void)
{
	DRV_WriteReg32(REG_CLDMA_UL_STOP_CMD_WP, 0x1);
}


unsigned int cldma_tx_active_status(void)
{
	return (DRV_Reg32(REG_CLDMA_UL_STATUS) & 0x1);
}


void cldma_clear_tx_done_status(void)
{
	DRV_WriteReg32(REG_CLDMA_L2TISAR0, DRV_Reg32(REG_CLDMA_L2TISAR0)); 
}


unsigned int cldma_tx_int_status(void)
{
	unsigned int errcode = 0;
	unsigned int error = 0;

	error = DRV_Reg32(REG_CLDMA_L2TISAR0) ;
	errcode = ((error & CLDMA_TX0_INT_DONE) << TX_DONE_INT) |
			(((error >> CLDMA_TX0_QE_INT_SHIFT) &1)<< TX_QE_INT) |
			(((error >> CLDMA_TX0_ERR_SHIFT) &1)<< TX_ERR) |
			(((error >> CLDMA_TX0_ACTIVE_START_ERR_SHIFT) &1)  <<TX_ACTIVE_START_ERR);

	DBG_LOG("cldma_tx_int_status: L2TISAR0[0x%x]=0x%x\n",
		REG_CLDMA_L2TISAR0, DRV_Reg32(REG_CLDMA_L2TISAR0));

	error = DRV_Reg32(REG_CLDMA_L2TISAR1) ;
	errcode = errcode | (((error >> CLDMA_RX0_INDMA_START_CMD_SHIFT) &1)  << TX_RX_INDMA_START_CMD);

	DBG_LOG("cldma_tx_int_status: L2TISAR1[0x%x]=0x%x\n",
		REG_CLDMA_L2TISAR1, DRV_Reg32(REG_CLDMA_L2TISAR1));

	error = DRV_Reg32(REG_CLDMA_L3TISAR0) ;
	errcode = errcode |
			(((error >> CLDMA_TX0_GPD_CSERR_SHIFT) &1) << TX_GPD_CS_ERR) |
			 (((error >> CLDMA_TX0_BD_CSERR_SHIFT) &1)  << TX_BD_CS_ERR) |
			 (((error >> CLDMA_TX0_GPD_LENERR_SHIFT) &1) << TX_GPD_LEN_ERR) |
			 (((error >> CLDMA_TX0_BD_LENERR_SHIFT) &1) << TX_BD_LEN_ERR);

	DBG_LOG("cldma_tx_int_status: L3TISAR0[0x%x]=0x%x\n",
		REG_CLDMA_L3TISAR0, DRV_Reg32(REG_CLDMA_L3TISAR0));
	error = DRV_Reg32(REG_CLDMA_L3TISAR1) ;
	errcode = errcode |
			(((error >> CLDMA_TX0_GPD_64KERR_SHIFT) &1)  << TX_GPD_64K_ERR) |
			(((error >> CLDMA_TX0_BD_64KERR_SHIFT) &1)  << TX_BD_64K_ERR) |
			(((error >> CLDMA_TX0_GPD_DATA_LEN_MIS_ERR_SHIFT) &1) << TX_GPD_DATA_LEN_MIS_ERR) |
			(((error >> CLDMA_TX0_SPD_ZERO_PKT_NUM_ERR_SHIFT) &1) << TX_SPD_ZERO_PKT_NUM_ERR);

	DBG_LOG("cldma_tx_int_status: L3TISAR1[0x%x]=0x%x, errorcode=0x%x\n",
		REG_CLDMA_L3TISAR1, DRV_Reg32(REG_CLDMA_L3TISAR1), errcode);

	error = DRV_Reg32(REG_CLDMA_L3TISAR2) ;
	errcode = errcode |
			(((error >> CLDMA_TX0_SPD_64K_LEN_ERR_SHIFT) &1)  << TX_SPD_64K_LEN_ERR) |
			(((error >> CLDMA_TX0_ILLEGAL_MEMORY_ACCESS_SHIFT) &1)  << TX_ILLEGAL_MEM_ACCESS_ERR) |
			(((error >> CLDMA_TX0_SPD_HEADER_ROOM_LEN_ERR_SHIFT) &1) << TX_SPD_HEADER_ROOM_LEN_ERR);

	DBG_LOG("cldma_tx_int_status: L3TISAR2[0x%x]=0x%x, errorcode=0x%x\n",
		REG_CLDMA_L3TISAR2, DRV_Reg32(REG_CLDMA_L3TISAR2), errcode);

	return errcode;
}

void cldma_rx_start_addr(void *gpd)
{
	DRV_WriteReg32(REG_CLDMA_DL_START_ADDRL_0, gpd);
	DRV_WriteReg32(REG_CLDMA_DL_START_ADDRH_0, 0);
}

void cldma_start_rx_queue(void)
{
	DRV_WriteReg32(REG_CLDMA_DL_START_WP, 0x1);
}

void cldma_stop_rx_queue(void)
{
	DRV_WriteReg32(REG_CLDMA_DL_STOP_WP, 0x1);
}


unsigned int cldma_rx_active_status(void)
{
	return (DRV_Reg32(REG_CLDMA_DL_STATUS) & 0x1);
}


void cldma_clear_rx_done_status(void)
{
	DRV_WriteReg32(REG_CLDMA_L2RISAR0, DRV_Reg32(REG_CLDMA_L2RISAR0));
}


unsigned int cldma_rx_int_status(void)
{
	unsigned int errcode = 0;
	unsigned int error = 0;

	error = DRV_Reg32(REG_CLDMA_L2RISAR0) ;
	errcode = ((error & CLDMA_RX0_INT_DONE) << RX_DONE_INT) |
			(((error >> CLDMA_RX0_ERR_SHIFT)&1)  << RX_ERR)|
			(((error >> CLDMA_RX0_QE_SHIFT) &1) << RX_QE_INT)|
			(((error >> CLDMA_RX0_ACTIVE_START_ERR_SHIFT) &1) << RX_ACTIVE_START_ERR);

	DBG_LOG("cldma_rx_int_status: L2RISAR0[0x%x]=0x%x\n",
		REG_CLDMA_L2RISAR0, DRV_Reg32(REG_CLDMA_L2RISAR0));

	error = DRV_Reg32(REG_CLDMA_L2RISAR1) ;
	errcode = errcode |
			(((error >> CLDMA_RX0_INACTIVE_ERR_SHIFT) &1) << RX_INACTIVE_ERR);

	DBG_LOG("cldma_rx_int_status: L2RISAR1[0x%x]=0x%x\n",
		REG_CLDMA_L2RISAR1, DRV_Reg32(REG_CLDMA_L2RISAR1));

	error = DRV_Reg32(REG_CLDMA_L3RISAR0) ;
	errcode = errcode |
			(((error >> CLDMA_RX0_GPD_CSERR_SHIFT) &1)  << RX_GPD_CSERR)|
			(((error >> CLDMA_RX0_BD_CSERR_SHIFT) &1) << RX_BD_CSERR) |
			(((error >> CLDMA_RX0_GPD_LENERR_SHIFT)&1) << RX_GPD_LENERR) |
			(((error >> CLDMA_RX0_BD_LENERR_SHIFT) &1)<< RX_BD_LENERR);

	DBG_LOG("cldma_rx_int_status: L3RISAR0[0x%x]=0x%x\n",
		REG_CLDMA_L3RISAR0, DRV_Reg32(REG_CLDMA_L3RISAR0));

	error = DRV_Reg32(REG_CLDMA_L3RISAR1) ;
	errcode = errcode |
			 (((error >> CLDMA_RX0_ALLEN_ERR_SHIFT) &1)  << RX_ALLEN_ERR) |
			 (((error >> CLDMA_RX0_FIFO_LEN_MIS_ERR_SHIFT) &1)  << RX_FIFO_LEN_MIS_ERR) |
			 (((error >> CLDMA_RX0_ILLEGAL_MEMORY_ACCESS_ERR_SHIFT) &1) << RX_ILLEGAL_MEMORY_ACCESS_ERR) |
			 (((error >> CLDMA_RBIDX_ERR_SHIFT) &1) << RX_RBIDX_ERR);

	DBG_LOG("cldma_rx_int_status: L3RISAR1[0x%x]=0x%x\n",
		REG_CLDMA_L3RISAR1, DRV_Reg32(REG_CLDMA_L3RISAR1));

	error = DRV_Reg32(REG_CLDMA_DL_ERR);
	errcode = errcode |
			 (((error >> CLDMA_RX_LDMU_ERR_SHIFT)&1)  << RX_LDMU_ERR);

	DBG_LOG("cldma_rx_int_status: DL_ERR[0x%x]=0x%x, errorcode=0x%x\n",
		REG_CLDMA_DL_ERR, DRV_Reg32(REG_CLDMA_DL_ERR), errcode);

	DBG_LOG("cldma_rx_int_status: DL_CURRENT_ADDRL[0x%x]=0x%x, DL_CURRENT_ADDRH[0x%x]=0x%x\n",
		REG_CLDMA_DL_CURRENT_ADDRL_0, DRV_Reg32(REG_CLDMA_DL_CURRENT_ADDRL_0),
		REG_CLDMA_DL_CURRENT_ADDRH_0, DRV_Reg32(REG_CLDMA_DL_CURRENT_ADDRH_0));

	return errcode;
}



