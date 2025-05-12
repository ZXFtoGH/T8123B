#include <typedefs.h>

#define INFRA_TRACKER_BASE	0x10314000
#define PERI_TRACKER_BASE	0x10218000
#define BUS_TRACE_MONITOR_BASE	0x0d040000

#define BUS_DBG_CON			0x000
#define BUS_DBG_TIMER_CON0	0x004
#define BUS_DBG_TIMER_CON1	0x008

#define BUS_TRACE_CON_1		0x900
#define BUS_TRACE_CON_AO_1	0x9fc
#define BUS_TRACE_CON_2		0xa00
#define BUS_TRACE_CON_AO_2	0xafc

#define BUS_TRACE_EN		(16)

#define SYSTRACKER_CTRL		0x100012fc

void bustracker_init(void)
{
	/* set infra/peri tracker timeout,
	 * timeout = clock_in_mhz * 1000 / 16 * timeout_in_ms */
	/* timeout: 200ms
	 * infra tracker clock: 156MHz
	 * peri tracker clock: 78MHz */
	DRV_WriteReg32(INFRA_TRACKER_BASE + BUS_DBG_TIMER_CON0, 0x001dc130);
	DRV_WriteReg32(INFRA_TRACKER_BASE + BUS_DBG_TIMER_CON1, 0x001dc130);
	DRV_WriteReg32(PERI_TRACKER_BASE + BUS_DBG_TIMER_CON0, 0x000ee098);
	DRV_WriteReg32(PERI_TRACKER_BASE + BUS_DBG_TIMER_CON1, 0x000ee098);

	/* enable infra/peri tracer due to tracker/tracer share the same enable bit */
	DRV_WriteReg32(BUS_TRACE_MONITOR_BASE + BUS_TRACE_CON_1, (1 << BUS_TRACE_EN));
	DRV_WriteReg32(BUS_TRACE_MONITOR_BASE + BUS_TRACE_CON_2, (1 << BUS_TRACE_EN));

	/* enable infra/peri tracker */
	/* bit[0] - BUS_DBG_EN
	 * bit[1] - TIMEOUT_EN
	 * bit[2] - SLV_ERR_EN
	 * bit[13] - HALT_ON_TIMEOUT_EN
	 * bit[14] - BUS_OT_WEN_CTRL */
	DRV_WriteReg32(BUS_TRACE_MONITOR_BASE + BUS_TRACE_CON_AO_1, 0x6007);
	DRV_WriteReg32(BUS_TRACE_MONITOR_BASE + BUS_TRACE_CON_AO_2, 0x6007);
}

void systracker_init(void)
{
	DRV_WriteReg32(SYSTRACKER_CTRL, 0x00006007);
}
