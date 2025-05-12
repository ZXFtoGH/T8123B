#include <typedefs.h>

#define LASTBUS_INFRA		0x10023000
#define LASTBUS_PERI		0x1002b000
#define LASTBUS_PERI_2		0x1002e000
#define LASTBUS_SUBINFRA	0x10025000

#define AP_TRACKER_BASE		0x10208000
#define INFRA_TRACKER_BASE	0x10314000
#define BUS_TRACE_MONITOR_BASE	0x0d040000

#define BUS_DBG_CON		0x000
#define BUS_DBG_TIMER_CON0	0x004
#define BUS_DBG_TIMER_CON1	0x008

#define BUS_TRACE_CON		0x800
#define BUS_TRACE_CON_AO	0x8fc
#define BUS_TRACE_CON_1		0x900
#define BUS_TRACE_CON_AO_1	0x9fc

#define BUS_TRACE_EN		(16)

#define BUS_TRACKER_CON_IRQ_CLR (7)
#define BUS_TRACKER_CON_SW_RST	(16)

void lastbus_init(void)
{
	/* mt6983 has 1 dummy check point which caused timeout */
	/* let's mask it first */
	DRV_WriteReg32(LASTBUS_INFRA + 0x14, 0x200);
	/* ok, do the normal init with max timeout */
	DRV_WriteReg32(LASTBUS_INFRA, 0xffff0008);
	DRV_WriteReg32(LASTBUS_INFRA, 0xffff000c);

	/* other monitors can use normal enable flow */
	DRV_WriteReg32(LASTBUS_PERI, 0xffff0008);
	DRV_WriteReg32(LASTBUS_PERI, 0xffff000c);
	DRV_WriteReg32(LASTBUS_PERI_2, 0xffff0008);
	DRV_WriteReg32(LASTBUS_PERI_2, 0xffff000c);
	DRV_WriteReg32(LASTBUS_SUBINFRA, 0xffff0008);
	DRV_WriteReg32(LASTBUS_SUBINFRA, 0xffff000c);
}

void systracker_init(void)
{

	/* do the sw reset before config & enable tracker */
	DRV_WriteReg32(AP_TRACKER_BASE + BUS_DBG_CON,
			DRV_Reg32(AP_TRACKER_BASE + BUS_DBG_CON) | (0x1 << BUS_TRACKER_CON_IRQ_CLR));
	DRV_WriteReg32(AP_TRACKER_BASE + BUS_DBG_CON,
			DRV_Reg32(AP_TRACKER_BASE + BUS_DBG_CON) | (0x1 << BUS_TRACKER_CON_SW_RST));
	DRV_WriteReg32(INFRA_TRACKER_BASE + BUS_DBG_CON,
			DRV_Reg32(INFRA_TRACKER_BASE + BUS_DBG_CON) | (0x1 << BUS_TRACKER_CON_IRQ_CLR));
	DRV_WriteReg32(INFRA_TRACKER_BASE + BUS_DBG_CON,
			DRV_Reg32(INFRA_TRACKER_BASE + BUS_DBG_CON) | (0x1 << BUS_TRACKER_CON_SW_RST));

	/* set infra tracker timeout
	 * timeout = clock_in_mhz * 1000 / 16 * timeout_in_ms */
	/* timeout: 1ms(1st. stage), 2000ms (2nd. stage)
	 * infra tracker clock: 156MHz; AP clock: 312MHz
	 */
	//DRV_WriteReg32(AP_TRACKER_BASE + BUS_DBG_TIMER_CON0, 0x456D70);
	DRV_WriteReg32(AP_TRACKER_BASE + BUS_DBG_TIMER_CON0, 0x4C2C);
	DRV_WriteReg32(AP_TRACKER_BASE + BUS_DBG_TIMER_CON1, 0x2b64660);
	//DRV_WriteReg32(INFRA_TRACKER_BASE + BUS_DBG_TIMER_CON0, 0x1DC130);
	DRV_WriteReg32(INFRA_TRACKER_BASE + BUS_DBG_TIMER_CON0, 0x2616);
	DRV_WriteReg32(INFRA_TRACKER_BASE + BUS_DBG_TIMER_CON1, 0x1298BE0);

	/* enable ap/infra tracker */
	/* bit[0] - BUS_DBG_EN
	 * bit[1] - TIMEOUT_EN
	 * bit[2] - SLV_ERR_EN
	 * bit[13] - HALT_ON_TIMEOUT_EN
	 * bit[14] - BUS_OT_WEN_CTRL
	 * bit[16] - SLV_ERROR_CAPTURE_EN */
	/* AP tracker */
	DRV_WriteReg32(BUS_TRACE_MONITOR_BASE + BUS_TRACE_CON_AO, 0x16003);
	/* Infra tracker, disable slverr */
	DRV_WriteReg32(BUS_TRACE_MONITOR_BASE + BUS_TRACE_CON_AO_1, 0x16003);

	/* enable infra tracer due to tracker/tracer share the same enable bit */
	DRV_WriteReg32(BUS_TRACE_MONITOR_BASE + BUS_TRACE_CON_1, (1 << BUS_TRACE_EN));
}
