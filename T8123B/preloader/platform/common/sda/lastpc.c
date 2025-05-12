#include "lastpc.h"
#include <pal_log.h>
#include <typedefs.h>

#define LASTPC_TAG	"LASTPC"
#define LASTPC_PRINT(fmt, ...)	pal_log_err(LASTPC_TAG " " #fmt "\n", ##__VA_ARGS__);

void lastpc_init(void)
{
	unsigned int ctrl = DRV_Reg32(CPC_FLOW_CTRL_CFG);

	DRV_WriteReg32(CPC_FLOW_CTRL_CFG, (ctrl|RESET_ON_KEEP_EN));
	dsb();
}

void lastpc_show(void)
{
	unsigned long long lastpc = 0;
	unsigned int dfd_triggered = 0;
	unsigned int dfd_status;

	/*
	 * only get core0 lastpc for debug nested reboot issue.
	 */

	DRV_WriteReg32(CPU0_DBG_SEL, SEL_TYPE_PC_H);
	lastpc = DRV_Reg32(CPU0_DBG_MON);
	DRV_WriteReg32(CPU0_DBG_SEL, SEL_TYPE_PC_L);
	lastpc = (lastpc << 32)|DRV_Reg32(CPU0_DBG_MON);

	dfd_status = DRV_Reg32(DFD_STATUS_RETURN);
	dfd_triggered = ((dfd_status & DFD_START) && (dfd_status & DFD_FINISH)) ? 1 : 0;

	/*
	 * if both ddr_reserved_mode and dfd_triggered are true, means that
	 * dfd has beed triggered during last reset.
	 */

	if (dfd_triggered) {
		LASTPC_PRINT("latch value may be corrupted due to DFD_muc is triggered,");
		LASTPC_PRINT("please refer to DFD_mcu.");
		LASTPC_PRINT("CPU0 pc=0x%llx", lastpc);
	} else {
		LASTPC_PRINT("CPU0 pc=0x%llx", lastpc);
	}
}
