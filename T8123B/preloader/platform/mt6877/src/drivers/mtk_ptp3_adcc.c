/**
 * @file    mtk_ptp3_adcc_dis.c
 * @brief   Driver for Advance Duty Cycle Control
 *
 */
#include <mtk_ptp3_adcc.h>
#include <typedefs.h>
#include <platform.h>
#include <wdt_v2.h>
#if MTK_DOE_CONFIG_ENV_SUPPORT
#include <dconfig_env.h>
#endif

#define ADCC_TRUE 1
#define ADCC_FALSE 0

/************************************************
 * adcc_cfg mapping
 * DCC	0	disable all
 * DCC	1	PLL control
 * DCC	2	FLL control
 * DCC	3	Ext-FLL-auto-calib	Ext-auto-calib with Target = 0x80
 * DCC	4	Ext-ADCC-auto-calib	Ext-auto-calib with Target = 0x80
 * DCC	5	Ext-FLL shaper_15	FLL shaper calin=15
 * DCC	6	Ext-FLL shaper_16	FLL shaper calin=16
 * DCC	7	Ext-ADCC shaper_15	ADCC shaper calin=15
 * DCC	8	Ext-ADCC shaper_16	ADCC shaper calin=16
 * DCC	9	Ext-FLL shaper_0	ADCC shaper calin=0
 ************************************************/
#define DUTY_50		0x80
#define MDATA		0x12
#define CHECKSUM_S	0x7FF
#define CHECKSUM	0x7FFF
#define DCC_CFG		0

static const struct adcc_class adcc_s[NR_ADCC_CPU] = {
	[0] = {
		.DcTarget = DUTY_50,
		.DcdSelect = MDATA,
		.SW_nFlag = 0,
		.adcc_cfg = DCC_CFG,
		.SET_checksum = CHECKSUM_S,

		.PLL_CalOut = 2,
		.PLL_CalDone = 1,
		.PLL_integrator = 0,
		.PLL_Ack = 0,
		.PLL_checksum = CHECKSUM,

		.FLL_CalOut = 0,
		.FLL_CalDone = 1,
		.FLL_integrator = 0,
		.FLL_Ack = 0,
		.FLL_checksum = CHECKSUM,
	},

	[1] = {
		.DcTarget = DUTY_50,
		.DcdSelect = MDATA,
		.SW_nFlag = 0,
		.adcc_cfg = DCC_CFG,
		.SET_checksum = CHECKSUM_S,

		.PLL_CalOut = 2,
		.PLL_CalDone = 1,
		.PLL_integrator = 0,
		.PLL_Ack = 0,
		.PLL_checksum = CHECKSUM,

		.FLL_CalOut = 0,
		.FLL_CalDone = 1,
		.FLL_integrator = 0,
		.FLL_Ack = 0,
		.FLL_checksum = CHECKSUM,
	},

	[2] = {
		.DcTarget = DUTY_50,
		.DcdSelect = MDATA,
		.SW_nFlag = 0,
		.adcc_cfg = DCC_CFG,
		.SET_checksum = CHECKSUM_S,

		.PLL_CalOut = 2,
		.PLL_CalDone = 1,
		.PLL_integrator = 0,
		.PLL_Ack = 0,
		.PLL_checksum = CHECKSUM,

		.FLL_CalOut = 0,
		.FLL_CalDone = 1,
		.FLL_integrator = 0,
		.FLL_Ack = 0,
		.FLL_checksum = CHECKSUM,
	},

	[3] = {
		.DcTarget = DUTY_50,
		.DcdSelect = MDATA,
		.SW_nFlag = 0,
		.adcc_cfg = DCC_CFG,
		.SET_checksum = CHECKSUM_S,

		.PLL_CalOut = 2,
		.PLL_CalDone = 1,
		.PLL_integrator = 0,
		.PLL_Ack = 0,
		.PLL_checksum = CHECKSUM,

		.FLL_CalOut = 0,
		.FLL_CalDone = 1,
		.FLL_integrator = 0,
		.FLL_Ack = 0,
		.FLL_checksum = CHECKSUM,
	},
	[4] = {
		.DcTarget = DUTY_50,
		.DcdSelect = MDATA,
		.SW_nFlag = 0,
		.adcc_cfg = DCC_CFG,
		.SET_checksum = CHECKSUM_S,

		.PLL_CalOut = 2,
		.PLL_CalDone = 1,
		.PLL_integrator = 0,
		.PLL_Ack = 0,
		.PLL_checksum = CHECKSUM,

		.FLL_CalOut = 0,
		.FLL_CalDone = 1,
		.FLL_integrator = 0,
		.FLL_Ack = 0,
		.FLL_checksum = CHECKSUM,
	},

	[5] = {
		.DcTarget = DUTY_50,
		.DcdSelect = MDATA,
		.SW_nFlag = 0,
		.adcc_cfg = DCC_CFG,
		.SET_checksum = CHECKSUM_S,

		.PLL_CalOut = 2,
		.PLL_CalDone = 1,
		.PLL_integrator = 0,
		.PLL_Ack = 0,
		.PLL_checksum = CHECKSUM,

		.FLL_CalOut = 0,
		.FLL_CalDone = 1,
		.FLL_integrator = 0,
		.FLL_Ack = 0,
		.FLL_checksum = CHECKSUM,
	},

	[6] = {
		.DcTarget = DUTY_50,
		.DcdSelect = MDATA,
		.SW_nFlag = 0,
		.adcc_cfg = DCC_CFG,
		.SET_checksum = CHECKSUM_S,

		.PLL_CalOut = 2,
		.PLL_CalDone = 1,
		.PLL_integrator = 0,
		.PLL_Ack = 0,
		.PLL_checksum = CHECKSUM,

		.FLL_CalOut = 0,
		.FLL_CalDone = 1,
		.FLL_integrator = 0,
		.FLL_Ack = 0,
		.FLL_checksum = CHECKSUM,
	},

	[7] = {
		.DcTarget = DUTY_50,
		.DcdSelect = MDATA,
		.SW_nFlag = 0,
		.adcc_cfg = DCC_CFG,
		.SET_checksum = CHECKSUM_S,

		.PLL_CalOut = 2,
		.PLL_CalDone = 1,
		.PLL_integrator = 0,
		.PLL_Ack = 0,
		.PLL_checksum = CHECKSUM,

		.FLL_CalOut = 0,
		.FLL_CalDone = 1,
		.FLL_integrator = 0,
		.FLL_Ack = 0,
		.FLL_checksum = CHECKSUM,
	},
};

static struct adcc_class adcc[NR_ADCC_CPU];

void adcc_dump(void)
{
	unsigned int temp, cpu;

	for (cpu = ADCC_CPU_START_ID; cpu <= ADCC_CPU_END_ID; cpu++) {
		print("[ADCC][CPU%d]", cpu);
		print(" Shaper:0x%x,", adcc[cpu].adcc_cfg);
		print(" SW_nFlag:0x%x,", adcc[cpu].SW_nFlag);
		print(" DcdSelect:0x%x,", adcc[cpu].DcdSelect);
		print(" DcTarget:0x%x,", adcc[cpu].DcTarget);

		print(" FllCalDone:0x%x,", adcc[cpu].FLL_CalDone);
		print(" FllCalin:0x%x,", adcc[cpu].FLL_CalOut);

		if (adcc[cpu].FLL_Ack == 1) {
			temp = adcc[cpu].FLL_integrator;
			print(" FLL_integrator:%d,", temp);
			if (temp >= 512)
				print(" FllDuty:%d,",
					(5000-(((temp-512)*10000)/512)));
			else
				print(" FllDuty:%d,",
					(5000+(((512-temp)*10000)/512)));
		} else
			print(" FLL_integrator:0, FllDuty:0,");

		print(" PllCalDone:0x%x,", adcc[cpu].PLL_CalDone);
		print(" PllCalin:0x%x,", adcc[cpu].PLL_CalOut);

		if (adcc[cpu].PLL_Ack == 1) {
			temp = adcc[cpu].PLL_integrator;
			print(" PLL_integrator:%d,", temp);
			if (temp >= 512)
				print(" PllDuty:%d,",
					(5000-(((temp-512)*10000)/512)));
			else
				print(" PllDuty:%d,",
					(5000+(((512-temp)*10000)/512)));
		} else
			print(" PLL_integrator:0, PllDuty:0,");
	print(" \n");
	}

}

#if MTK_DOE_CONFIG_ENV_SUPPORT
#define DOE_UPDATE(_item)	\
{	\
	doe_##_item = dconfig_getenv(#_item);	\
	if (doe_##_item)	\
		_item = atoi(doe_##_item);\
	print(#_item " %x\n", _item);\
}
#endif

unsigned int adcc_pre_init(void)
{
	unsigned int cpu, cnt;
#if MTK_DOE_CONFIG_ENV_SUPPORT
	unsigned int enable;
	unsigned int ADCC_En = 0xFFFFFFFF;
	char *doe_ADCC_En;
#endif

	/* cold boot re-calibration */
	/* adcc_debug("cold(0)/warm(2) boot check: %d\n", g_rgu_status); */
	if (ram_console_is_abnormal_boot() == 0) {
		if (g_rgu_status == 0) {
		/* cold boot */
			for (cpu = ADCC_CPU_START_ID; cpu <= ADCC_CPU_END_ID; cpu++) {
				adcc[cpu] = adcc_s[cpu];
				adcc_write(GET_SET_ADDR(cpu), adcc[cpu].set);
				adcc_write(GET_PLL_ADDR(cpu), adcc[cpu].PLL);
				adcc_write(GET_FLL_ADDR(cpu), adcc[cpu].FLL);
				adcc_write(GET_DBG_ADDR(cpu), 0x0);
			}
		} else {
		/* data corrupt re-calibration */
			for (cpu = ADCC_CPU_START_ID; cpu <= ADCC_CPU_END_ID; cpu++) {
				adcc[cpu].set = adcc_read(GET_SET_ADDR(cpu));
				if (adcc[cpu].SET_checksum != CHECKSUM_S) {
					adcc[cpu].set = adcc_s[cpu].set;
					adcc_write(GET_SET_ADDR(cpu), adcc[cpu].set);
				}
				adcc[cpu].PLL = adcc_read(GET_PLL_ADDR(cpu));
				if (adcc[cpu].PLL_checksum != CHECKSUM) {
					adcc[cpu].PLL = adcc_s[cpu].PLL;
					adcc_write(GET_PLL_ADDR(cpu), adcc[cpu].PLL);
				}
				adcc[cpu].FLL = adcc_read(GET_FLL_ADDR(cpu));
				if (adcc[cpu].FLL_checksum != CHECKSUM) {
					adcc[cpu].FLL = adcc_s[cpu].FLL;
					adcc_write(GET_FLL_ADDR(cpu), adcc[cpu].FLL);
				}
			}
		}

#if MTK_DOE_CONFIG_ENV_SUPPORT
	DOE_UPDATE(ADCC_En);
	if (ADCC_En != 0xFFFFFFFF) {
		for (cpu = ADCC_CPU_START_ID; cpu <= ADCC_CPU_END_ID; cpu++) {
			enable = (ADCC_En >> (cpu * 4)) & 0xF;
			adcc[cpu].adcc_cfg = enable;
			adcc_write_bits(GET_SET_ADDR(cpu),
				ADCC_cfg_bits,
				ADCC_cfg_shift,
				adcc[cpu].adcc_cfg);
		}
	}
#endif
}

	/* adcc_dump(); */
	return ADCC_TRUE;
}
