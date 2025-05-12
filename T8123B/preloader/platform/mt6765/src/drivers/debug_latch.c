#include <typedefs.h>


#define AP_TRACKER_BASE		0x10208000

#define BUS_DBG_CON		0x000
#define BUS_DBG_TIMER_CON0	0x004
#define BUS_DBG_TIMER_CON1	0x008

#define BUS_TRACKER_CON_IRQ_CLR (7)
#define BUS_TRACKER_CON_SW_RST	(16)


#define SYSTRACKER_CTRL		0x100012fc

void systracker_init(void)
{
	/* do the sw reset before config & enable tracker */
	DRV_WriteReg32(AP_TRACKER_BASE + BUS_DBG_CON,
			DRV_Reg32(AP_TRACKER_BASE + BUS_DBG_CON) | (0x1 << BUS_TRACKER_CON_IRQ_CLR));
	DRV_WriteReg32(AP_TRACKER_BASE + BUS_DBG_CON,
			DRV_Reg32(AP_TRACKER_BASE + BUS_DBG_CON) & ~(0x1 << BUS_TRACKER_CON_IRQ_CLR));
	DRV_WriteReg32(AP_TRACKER_BASE + BUS_DBG_CON,
			DRV_Reg32(AP_TRACKER_BASE + BUS_DBG_CON) | (0x1 << BUS_TRACKER_CON_SW_RST));

	/* set ap tracker timeout
	 * timeout = clock_in_mhz * 1000 / 16 * timeout_in_ms */
	/* timeout: 100ms(1st. stage), 2000ms (2nd. stage)
	 *  AP clock: 156MHz
	 */
	DRV_WriteReg32(AP_TRACKER_BASE + BUS_DBG_TIMER_CON0, 0xee098);
	DRV_WriteReg32(AP_TRACKER_BASE + BUS_DBG_TIMER_CON1, 0x1298be0);

	/* enable ap tracker */
	/* bit[0] - BUS_DBG_EN
	 * bit[1] - TIMEOUT_EN
	 * bit[2] - SLV_ERR_EN
	 * bit[13] - HALT_ON_TIMEOUT_EN
	 * bit[14] - BUS_OT_WEN_CTRL
	 * bit[16] - SLV_ERROR_CAPTURE_EN */

	DRV_WriteReg32(SYSTRACKER_CTRL, 0x00006007);
}
