/**
 * @file    mtk_ptp3_adcc_pre.c
 * @brief   Driver for Advance Duty Cycle Control
 *
 */
#include "mtk_ptp3_adcc.h"
#include "mtk_ptp3_cpu_pwr.h" 
#include <typedefs.h>
#include <platform.h>
#include <pll.h>
#include <pmic.h>
#include <wdt_v2.h>
#include <delsel.h>
#include <regulator/mtk_regulator.h>
#include <ram_console_common.h>
#if MTK_DOE_CONFIG_ENV_SUPPORT
#include <dconfig_env.h>
#endif

extern int adcc_start(unsigned int);

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
#define MDATA		0x10
#define CHECKSUM	0x7FF
#define DCC_CFG		3

static const struct adcc_class adcc_s[NR_ADCC_CPU] = {
	[0] = { //DUMMY
		.set.DcTarget = 0,
		.set.DcdSelect = 0,
		.set.SW_nFlag = 0,
		.set.adcc_cfg = 0,
		.set.checksum = 0,

		.PLL.CalOut = 0,
		.PLL.CalDone = 0,
		.PLL.integrator = 0,
		.PLL.Ack = 0,
		.PLL.adcc_cfg = 0,
		.PLL.checksum = 0,

		.FLL.CalOut = 0,
		.FLL.CalDone = 0,
		.FLL.integrator = 0,
		.FLL.Ack = 0,
		.FLL.adcc_cfg = 0,
		.FLL.checksum = 0,
	},

	[1] = { //COMPLEX0
		.set.DcTarget = DUTY_50,
		.set.DcdSelect = MDATA,
		.set.SW_nFlag = 0,
		.set.adcc_cfg = DCC_CFG,
		.set.checksum = CHECKSUM,

		.PLL.CalOut = 0x1E,
		.PLL.CalDone = 1,
		.PLL.integrator = 0,
		.PLL.Ack = 0,
		.PLL.adcc_cfg = DCC_CFG,
		.PLL.checksum = CHECKSUM,

		.FLL.CalOut = 0,
		.FLL.CalDone = 1,
		.FLL.integrator = 0,
		.FLL.Ack = 0,
		.FLL.adcc_cfg = DCC_CFG,
		.FLL.checksum = CHECKSUM,
	},

	[2] = { //COMPLEX1
		.set.DcTarget = DUTY_50,
		.set.DcdSelect = MDATA,
		.set.SW_nFlag = 0,
		.set.adcc_cfg = DCC_CFG,
		.set.checksum = CHECKSUM,

		.PLL.CalOut = 0x1E,
		.PLL.CalDone = 1,
		.PLL.integrator = 0,
		.PLL.Ack = 0,
		.PLL.adcc_cfg = DCC_CFG,
		.PLL.checksum = CHECKSUM,

		.FLL.CalOut = 0,
		.FLL.CalDone = 1,
		.FLL.integrator = 0,
		.FLL.Ack = 0,
		.FLL.adcc_cfg = DCC_CFG,
		.FLL.checksum = CHECKSUM,
	},

	[3] = { //MPSYS
		.set.DcTarget = DUTY_50,
		.set.DcdSelect = MDATA,
		.set.SW_nFlag = 0,
		.set.adcc_cfg = DCC_CFG,
		.set.checksum = CHECKSUM,

		.PLL.CalOut = 0x1E,
		.PLL.CalDone = 1,
		.PLL.integrator = 0,
		.PLL.Ack = 0,
		.PLL.adcc_cfg = DCC_CFG,
		.PLL.checksum = CHECKSUM,

		.FLL.CalOut = 0,
		.FLL.CalDone = 1,
		.FLL.integrator = 0,
		.FLL.Ack = 0,
		.FLL.adcc_cfg = DCC_CFG,
		.FLL.checksum = CHECKSUM,
	},
	[4] = { //CPU4
		.set.DcTarget = DUTY_50,
		.set.DcdSelect = MDATA,
		.set.SW_nFlag = 0,
		.set.adcc_cfg = DCC_CFG,
		.set.checksum = CHECKSUM,

		.PLL.CalOut = 0x1E,
		.PLL.CalDone = 1,
		.PLL.integrator = 0,
		.PLL.Ack = 0,
		.PLL.adcc_cfg = DCC_CFG,
		.PLL.checksum = CHECKSUM,

		.FLL.CalOut = 0,
		.FLL.CalDone = 1,
		.FLL.integrator = 0,
		.FLL.Ack = 0,
		.FLL.adcc_cfg = DCC_CFG,
		.FLL.checksum = CHECKSUM,
	},

	[5] = { //CPU5
		.set.DcTarget = DUTY_50,
		.set.DcdSelect = MDATA,
		.set.SW_nFlag = 0,
		.set.adcc_cfg = DCC_CFG,
		.set.checksum = CHECKSUM,

		.PLL.CalOut = 0x1E,
		.PLL.CalDone = 1,
		.PLL.integrator = 0,
		.PLL.Ack = 0,
		.PLL.adcc_cfg = DCC_CFG,
		.PLL.checksum = CHECKSUM,

		.FLL.CalOut = 0,
		.FLL.CalDone = 1,
		.FLL.integrator = 0,
		.FLL.Ack = 0,
		.FLL.adcc_cfg = DCC_CFG,
		.FLL.checksum = CHECKSUM,
	},

	[6] = { //CPU6
		.set.DcTarget = DUTY_50,
		.set.DcdSelect = MDATA,
		.set.SW_nFlag = 0,
		.set.adcc_cfg = DCC_CFG,
		.set.checksum = CHECKSUM,

		.PLL.CalOut = 0x1E,
		.PLL.CalDone = 1,
		.PLL.integrator = 0,
		.PLL.Ack = 0,
		.PLL.adcc_cfg = DCC_CFG,
		.PLL.checksum = CHECKSUM,

		.FLL.CalOut = 0,
		.FLL.CalDone = 1,
		.FLL.integrator = 0,
		.FLL.Ack = 0,
		.FLL.adcc_cfg = DCC_CFG,
		.FLL.checksum = CHECKSUM,
	},

	[7] = { //CPU7
		.set.DcTarget = DUTY_50,
		.set.DcdSelect = MDATA,
		.set.SW_nFlag = 0,
		.set.adcc_cfg = DCC_CFG,
		.set.checksum = CHECKSUM,

		.PLL.CalOut = 0x1E,
		.PLL.CalDone = 1,
		.PLL.integrator = 0,
		.PLL.Ack = 0,
		.PLL.adcc_cfg = DCC_CFG,
		.PLL.checksum = CHECKSUM,

		.FLL.CalOut = 0,
		.FLL.CalDone = 1,
		.FLL.integrator = 0,
		.FLL.Ack = 0,
		.FLL.adcc_cfg = DCC_CFG,
		.FLL.checksum = CHECKSUM,
	},
};

static struct adcc_class adcc[NR_ADCC_CPU];

unsigned int adcc_init(unsigned int cpu)
{
	if ((cpu >= ADCC_CPU_START_ID) && (cpu <= ADCC_CPU_END_ID)) {

		adcc[cpu].set.reg = adcc_read(GET_SET_ADDR(cpu));
		if (adcc[cpu].set.checksum != CHECKSUM)
			adcc[cpu].set.adcc_cfg = 0;

		adcc[cpu].PLL.reg = adcc_read(GET_PLL_ADDR(cpu));
		if (adcc[cpu].PLL.checksum != CHECKSUM)
			adcc[cpu].PLL.adcc_cfg = 0;

		adcc[cpu].FLL.reg = adcc_read(GET_FLL_ADDR(cpu));
		if (adcc[cpu].FLL.checksum != CHECKSUM)
			adcc[cpu].FLL.adcc_cfg = 0;

		return adcc[cpu].FLL.adcc_cfg;
	}
	return ADCC_FALSE;
}

void adcc_dump(void)
{
	unsigned int temp, pwr, cpu;

	for (pwr = 0; pwr < NR_ADCC_PWR; pwr++) {
		switch (pwr) {
		case 0:
			cpu = 3;
			adcc_log("[ADCC][MPSYS]");
			break;
		case 1:
			cpu = 1;
			adcc_log("[ADCC][COMPLEX0]");
			break;
		case 2:
			cpu = 2;
			adcc_log("[ADCC][COMPLEX1]");
			break;
		default:
			cpu = pwr+1;
			adcc_log("[ADCC][CPU%d]", cpu);
			break;
		}
		adcc_init(cpu);
		adcc_log(" Shaper:0x%x,", adcc[cpu].set.adcc_cfg);
		adcc_log(" SW_nFlag:0x%x,", adcc[cpu].set.SW_nFlag);
		adcc_log(" DcdSelect:0x%x,", adcc[cpu].set.DcdSelect);
		adcc_log(" DcTarget:0x%x,", adcc[cpu].set.DcTarget);

		adcc_log(" FllCalDone:0x%x,", adcc[cpu].FLL.CalDone);
		adcc_log(" FllCalin:0x%x,", adcc[cpu].FLL.CalOut);

		if (adcc[cpu].FLL.Ack == 1) {
			temp = adcc[cpu].FLL.integrator;
			adcc_log(" FLL_integrator:%d,", temp);
			if (temp >= 512)
				adcc_log(" FllDuty:%d",
					(5000-(((temp-512)*10000)/512)));
			else
				adcc_log(" FllDuty:%d",
					(5000+(((512-temp)*10000)/512)));
		} else
			adcc_log(" FLL_integrator:0, FllDuty:0");
/*
		adcc_log(", PllCalDone:0x%x,", adcc[cpu].PLL.CalDone);
		adcc_log(" PllCalin:0x%x,", adcc[cpu].PLL.CalOut);

		if (adcc[cpu].PLL.Ack == 1) {
			temp = adcc[cpu].PLL.integrator;
			adcc_log(" PLL_integrator:%d,", temp);
			if (temp >= 512)
				adcc_log(" PllDuty:%d, ",
					(5000-(((temp-512)*10000)/512)));
			else
				adcc_log(" PllDuty:%d, ",
					(5000+(((512-temp)*10000)/512)));
		} else
			adcc_log(" PLL_integrator:0, PllDuty:0, ");
*/
	adcc_log(", debug:0x%x", adcc_read(GET_DBG_ADDR(cpu)));
	adcc_log("\n");
	}
}


#define MP0_ARMPLL_CON1_PHY					0x0c03040c //core 0-3(CPUL)
#define MP1_ARMPLL_CON1_PHY					0x0c03080c //core 4-6(CPUM)
#define MP2_ARMPLL_CON1_PHY					0x0c030c0c //core 7(CPUB)
#define C03_ARMPLL_CON1_PHY					MP0_ARMPLL_CON1_PHY
#define C46_ARMPLL_CON1_PHY					MP1_ARMPLL_CON1_PHY
#define C7_ARMPLL_CON1_PHY					MP2_ARMPLL_CON1_PHY
#define CCI_ARMPLL_CON1_PHY					0x0c03000c //CCI

// [9:8] muxsel: switch to PLL speed ==> 0: 26M,  1: armpll, 2:  Mainpll, 3:  Unipll
// [10]  muxsel: switch toggle
// [21:17] divsel: CPU clock divide by 1, ==> 8: div1, A: div2, B: div4, 1D: div6
#define MP0_ARMPLL_DIV_PHY					0x0c00010c //CPU_L
#define MP1_ARMPLL_DIV_PHY					0x0c000118 //CPU_M
#define MP2_ARMPLL_DIV_PHY					0x0c000124 //CPU_B
#define C03_ARMPLL_DIV_PHY					MP0_ARMPLL_DIV_PHY
#define C46_ARMPLL_DIV_PHY					MP1_ARMPLL_DIV_PHY
#define C7_ARMPLL_DIV_PHY					MP2_ARMPLL_DIV_PHY
#define CCI_ARMPLL_DIV_PHY					0x0c000100 //CCI

//               CLK  POS_DIV  CLK_DIV
//1500 ~ 3800 =   x1    /1        /1
// 750 ~ 1499 =   x2    /2        /1
// 375 ~  749 =   x4    /4        /1
// 188 ~  374 =   x8    /8        /1
//bit[31]: chnage, bit[26:24]: pos_div , Div1 = OR 0x00000000, Div2 = OR 0x01000000, Div4 = OR 0x02000000, Div8 = OR 0x03000000
#define FREQ_DCM(f_mhz)						((f_mhz <=  374) ? (((f_mhz << 16) / 13) | 0x03000000 | 0x80000000) : \
											((f_mhz <=  749) ? (((f_mhz << 15) / 13) | 0x02000000 | 0x80000000) : \
											((f_mhz <= 1499) ? (((f_mhz << 14) / 13) | 0x01000000 | 0x80000000) : \
															   (((f_mhz << 13) / 13) | 0x00000000 | 0x80000000))))
/* [21:17] divsel: CPU clock divide by 1, ==> 8: div1, A: div2, B: div4, 1D: div6 */
#define FREQ_DIV(f_mhz)						(0x0) /*((f_mhz <= 374) ? 0xb : ((f_mhz <= 749) ? 0xa : 0x8)) */

#define FREQ_SET_DCM_DIV_Q(dcm, div, f_mhz)	adcc_write(dcm, FREQ_DCM(f_mhz)),\
											adcc_write(div, (adcc_read(div) & 0xFFC1F9FF) | (FREQ_DIV(f_mhz) << 17) | 0x000)

#define FREQ_SET_DCM_DIV_LOG(dcm, div, f_mhz)	adcc_debug("%s, Freq = %d, DCM[0x%x] = 0x%x, DIV[0x%x] = 0x%x.\n",\
												((dcm == C03_ARMPLL_CON1_PHY) ? "Core0-3" :\
												((dcm == C46_ARMPLL_CON1_PHY) ? "Core4-6" :\
												((dcm ==  C7_ARMPLL_CON1_PHY) ? "Core7" : "CCI    "))),\
												f_mhz, dcm, FREQ_DCM(f_mhz), div, (adcc_read(div) & 0xFFC1FFFF) | (FREQ_DIV(f_mhz) << 17))

#if MTK_DOE_CONFIG_ENV_SUPPORT
#define DOE_UPDATE(_item)	\
{	\
	doe_##_item = dconfig_getenv(#_item);	\
	if (doe_##_item)	\
		_item = atoi(doe_##_item);\
	adcc_msg(#_item " %x\n", _item);\
}
#endif

enum adcc_cpu_dvfs_id {
	ADCC_CPU_DVFS_LL,
	ADCC_CPU_DVFS_L,
	ADCC_CPU_DVFS_B,

	NR_ADCC_CPU_DVFS,
};

/* cpu default Volt/Freq */
unsigned int cpul_default_volt = 0;
unsigned int cpum_default_volt = 0;
unsigned int cpub_default_volt = 0;
unsigned int cpum_default_freq = 0;
unsigned int cpub_default_freq = 0;

void mate_set_armpll_cpul(unsigned int freq)
{
	unsigned int temp;

	if ((freq < 180) || (freq >= 4096))
		return;

	/* switch to MAINPLL first */
	adcc_debug("switch to 26M\n");
	DRV_WriteReg32(CPU_PLLDIV_0_CFG0, (DRV_Reg32(CPU_PLLDIV_0_CFG0) & 0xFFFFFCFF) | (0x00 << 8));  // [9:8] muxsel: switch to 26M
	DRV_WriteReg32(CPU_PLLDIV_0_CFG0, (DRV_Reg32(CPU_PLLDIV_0_CFG0) & 0xFFFFFBFF) | (0x01 << 10)); // [10] muxsel: toggle

	/* set CPU to new freq */
	adcc_debug("ARMPLL_LL switch to OPP %d\n", freq);
	DRV_WriteReg32(CPU_L_PLL1U_PLL_CON1, FREQ_DCM(freq));

	/* switch back to ARMPLL */
	adcc_debug("switch to ARMPLL LL\n");
	DRV_WriteReg32(CPU_PLLDIV_0_CFG0, (DRV_Reg32(CPU_PLLDIV_0_CFG0) & 0xFFFFFCFF) | (0x01 << 8));  // [9:8] muxsel: switch to PLL speed
	DRV_WriteReg32(CPU_PLLDIV_0_CFG0, (DRV_Reg32(CPU_PLLDIV_0_CFG0) & 0xFFFFFBFF) | (0x01 << 10)); // [10] muxsel: toggle

	adcc_debug("CPU_L_PLL1U_PLL_CON1: 0x%x\n", DRV_Reg32(CPU_L_PLL1U_PLL_CON1));
}

void mate_set_armpll_cpum(unsigned int freq)
{
	unsigned int temp;

	if ((freq < 180) || (freq >= 4096))
		return;

	if (freq == 1800)
		cache_delsel_init(ADCC_CPU_DVFS_L, freq * 1000);

	/* switch to MAINPLL first */
	adcc_debug("switch to 26M\n");
	DRV_WriteReg32(CPU_PLLDIV_1_CFG0, (DRV_Reg32(CPU_PLLDIV_1_CFG0) & 0xFFFFFCFF) | (0x00 << 8));  // [9:8] muxsel: switch to 26M
	DRV_WriteReg32(CPU_PLLDIV_1_CFG0, (DRV_Reg32(CPU_PLLDIV_1_CFG0) & 0xFFFFFBFF) | (0x01 << 10)); // [10] muxsel: toggle

	/* set CPU to new freq */
	adcc_debug("ARMPLL_LL switch to OPP %d\n", freq);
	DRV_WriteReg32(CPU_M_PLL1U_PLL_CON1, FREQ_DCM(freq));

	/* switch back to ARMPLL */
	adcc_debug("switch to ARMPLL LL\n");
	DRV_WriteReg32(CPU_PLLDIV_1_CFG0, (DRV_Reg32(CPU_PLLDIV_1_CFG0) & 0xFFFFFCFF) | (0x01 << 8));  // [9:8] muxsel: switch to PLL speed
	DRV_WriteReg32(CPU_PLLDIV_1_CFG0, (DRV_Reg32(CPU_PLLDIV_1_CFG0) & 0xFFFFFBFF) | (0x01 << 10)); // [10] muxsel: toggle

	if (freq == cpum_default_freq/1000)
		cache_delsel_init(ADCC_CPU_DVFS_L, freq * 1000);

	adcc_debug("CPU_M_PLL1U_PLL_CON1: 0x%x\n", DRV_Reg32(CPU_M_PLL1U_PLL_CON1));
}

void mate_set_armpll_cpub(unsigned int freq)
{
	unsigned int temp;

	if ((freq < 180) || (freq >= 4096))
		return;

	if (freq == 1800)
			cache_delsel_init(ADCC_CPU_DVFS_B, freq * 1000);

	/* switch to MAINPLL first */
	adcc_debug("switch to 26M\n");
	DRV_WriteReg32(CPU_PLLDIV_2_CFG0, (DRV_Reg32(CPU_PLLDIV_2_CFG0) & 0xFFFFFCFF) | (0x00 << 8));  // [9:8] muxsel: switch to 26M
	DRV_WriteReg32(CPU_PLLDIV_2_CFG0, (DRV_Reg32(CPU_PLLDIV_2_CFG0) & 0xFFFFFBFF) | (0x01 << 10)); // [10] muxsel: toggle

	/* set CPU to new freq */
	adcc_debug("ARMPLL_LL switch to OPP %d\n", freq);
	DRV_WriteReg32(CPU_B_PLL1U_PLL_CON1, FREQ_DCM(freq));

	/* switch back to ARMPLL */
	adcc_debug("switch to ARMPLL LL\n");
	DRV_WriteReg32(CPU_PLLDIV_2_CFG0, (DRV_Reg32(CPU_PLLDIV_2_CFG0) & 0xFFFFFCFF) | (0x01 << 8));  // [9:8] muxsel: switch to PLL speed
	DRV_WriteReg32(CPU_PLLDIV_2_CFG0, (DRV_Reg32(CPU_PLLDIV_2_CFG0) & 0xFFFFFBFF) | (0x01 << 10)); // [10] muxsel: toggle

	adcc_debug("CPU_B_PLL1U_PLL_CON1: 0x%x\n", DRV_Reg32(CPU_B_PLL1U_PLL_CON1));
}

void mate_set_armpll_cci(unsigned int freq)
{
	unsigned int temp;

	if ((freq < 180) || (freq >= 4096))
		return;

	/* switch to MAINPLL first */
	adcc_debug("switch to 26M\n");
	DRV_WriteReg32(BUS_PLLDIV_CFG0, (DRV_Reg32(BUS_PLLDIV_CFG0) & 0xFFFFFCFF) | (0x00 << 8));  // [9:8] muxsel: switch to 26M
	DRV_WriteReg32(BUS_PLLDIV_CFG0, (DRV_Reg32(BUS_PLLDIV_CFG0) & 0xFFFFFBFF) | (0x01 << 10)); // [10] muxsel: toggle

	/* set CPU to new freq */
	adcc_debug("ARMPLL_LL switch to OPP %d\n", freq);
	DRV_WriteReg32(CCI_PLL1U_PLL_CON1, FREQ_DCM(freq));

	/* switch back to ARMPLL */
	adcc_debug("switch to ARMPLL LL\n");
	DRV_WriteReg32(BUS_PLLDIV_CFG0, (DRV_Reg32(BUS_PLLDIV_CFG0) & 0xFFFFFCFF) | (0x01 << 8));  // [9:8] muxsel: switch to PLL speed
	DRV_WriteReg32(BUS_PLLDIV_CFG0, (DRV_Reg32(BUS_PLLDIV_CFG0) & 0xFFFFFBFF) | (0x01 << 10)); // [10] muxsel: toggle

	adcc_debug("CCI_PLL1U_PLL_CON1: 0x%x\n", DRV_Reg32(CCI_PLL1U_PLL_CON1));
}


static const struct mtk_regulator empty_regulator;
static unsigned int adcc_set_voltage(unsigned int buck_id,
						unsigned int vproc_vol,
						unsigned int flag)
{
	unsigned int ret = 0;
	unsigned int vsram_proc_vol = 0;
	struct mtk_regulator reg_vsram_cpu = empty_regulator;
	struct mtk_regulator reg_vcpu = empty_regulator;

	/*--Get regulator handle--*/
	switch (buck_id) {
	case ADCC_CPU_DVFS_LL:
		if (mtk_regulator_get("vsram_cpul", &reg_vsram_cpu))
			ret |= (1 << 0);
		if (mtk_regulator_get("vcpul", &reg_vcpu))
			ret |= (1 << 1);
		break;
	case ADCC_CPU_DVFS_L:
		if (mtk_regulator_get("vsram_cpum", &reg_vsram_cpu))
			ret |= (1 << 2);
		if (mtk_regulator_get("vcpum", &reg_vcpu))
			ret |= (1 << 3);
		break;
	case ADCC_CPU_DVFS_B:
		if (mtk_regulator_get("vsram_cpub", &reg_vsram_cpu))
			ret |= (1 << 4);
		if (mtk_regulator_get("vcpub", &reg_vcpu))
			ret |= (1 << 5);
		break;
	default:
		break;
	}

	if (ret) {
		adcc_msg("mtk_regulator_get failed.(0x%x)\n", ret);
		return ADCC_FALSE;
	}

	/*--Set voltage--*/
	if (vproc_vol + 100000 > 1000000)
			vsram_proc_vol = 1000000;
	else if (vproc_vol + 100000 < 750000)
		vsram_proc_vol = 750000;
	else
		vsram_proc_vol = vproc_vol + 100000;

	switch (buck_id) {
	case ADCC_CPU_DVFS_LL:
		if (flag == 1)
			cpul_default_volt = mtk_regulator_get_voltage(&reg_vcpu);
		if (mtk_regulator_set_voltage(&reg_vsram_cpu, vsram_proc_vol, vsram_proc_vol))
			ret |= (1 << 0);
		if (mtk_regulator_set_voltage(&reg_vcpu, vproc_vol, vproc_vol))
			ret |= (1 << 1);
		break;
	case ADCC_CPU_DVFS_L:
		if (flag == 1)
			cpum_default_volt = mtk_regulator_get_voltage(&reg_vcpu);
		if (mtk_regulator_set_voltage(&reg_vsram_cpu, vsram_proc_vol, vsram_proc_vol))
			ret |= (1 << 2);
		if (mtk_regulator_set_voltage(&reg_vcpu, vproc_vol, vproc_vol))
			ret |= (1 << 3);
		break;
	case ADCC_CPU_DVFS_B:
		if (flag == 1)
			cpub_default_volt = mtk_regulator_get_voltage(&reg_vcpu);
		if (mtk_regulator_set_voltage(&reg_vsram_cpu, vsram_proc_vol, vsram_proc_vol))
			ret |= (1 << 4);
		if (mtk_regulator_set_voltage(&reg_vcpu, vproc_vol, vproc_vol))
			ret |= (1 << 5);
		break;
	default:
		break;
	}

	if (ret) {
		adcc_msg("mtk_regulator_set_voltage failed.(0x%x)\n", ret);
		return ADCC_FALSE;
	}

	/*--Get voltage--*/
	adcc_debug("vsram_cpu(%d)= %d uV\n", buck_id, mtk_regulator_get_voltage(&reg_vsram_cpu));
	adcc_debug("vproc_cpu(%d)= %d uV\n", buck_id, mtk_regulator_get_voltage(&reg_vcpu));

	return ADCC_TRUE;
}

unsigned int adcc_pre_init(void)
{
	unsigned int cpu, cnt;
#if MTK_DOE_CONFIG_ENV_SUPPORT
	unsigned int enable, pwr;
	unsigned int ADCC_En = 0xFFFFFFFF;
	char *doe_ADCC_En;
#endif
	/* cold boot re-calibration */
	adcc_debug("cold(0)/warm(2) boot check: %d\n", g_rgu_status);
	adcc_debug("mtk_is_rst_from_da = %d\n", mtk_is_rst_from_da());
	if (ram_console_is_abnormal_boot() == 0) {
		if (mtk_is_rst_from_da())
			return ADCC_FALSE;

		if (g_rgu_status == 0) {
		/* cold boot */
			for (cpu = ADCC_CPU_START_ID; cpu <= ADCC_CPU_END_ID; cpu++) {
				adcc[cpu] = adcc_s[cpu];
				adcc_write(GET_SET_ADDR(cpu), adcc[cpu].set.reg);
				adcc_write(GET_PLL_ADDR(cpu), adcc[cpu].PLL.reg);
				adcc_write(GET_FLL_ADDR(cpu), adcc[cpu].FLL.reg);
				adcc_write(GET_DBG_ADDR(cpu), 0x0);
			}
		} else {
		/* data corrupt re-calibration */
			for (cpu = ADCC_CPU_START_ID; cpu <= ADCC_CPU_END_ID; cpu++) {
				adcc[cpu].set.reg = adcc_read(GET_SET_ADDR(cpu));
				if (adcc[cpu].set.checksum != CHECKSUM) {
					adcc[cpu].set.reg = adcc_s[cpu].set.reg;
					adcc_write(GET_SET_ADDR(cpu), adcc[cpu].set.reg);
				}
				adcc[cpu].PLL.reg = adcc_read(GET_PLL_ADDR(cpu));
				if (adcc[cpu].PLL.checksum != CHECKSUM) {
					adcc[cpu].PLL.reg = adcc_s[cpu].PLL.reg;
					adcc_write(GET_PLL_ADDR(cpu), adcc[cpu].PLL.reg);
				}
				adcc[cpu].FLL.reg = adcc_read(GET_FLL_ADDR(cpu));
				if (adcc[cpu].FLL.checksum != CHECKSUM) {
					adcc[cpu].FLL.reg = adcc_s[cpu].FLL.reg;
					adcc_write(GET_FLL_ADDR(cpu), adcc[cpu].FLL.reg);
				}
				adcc_write(GET_DBG_ADDR(cpu), 0x0);
			}
		}

#if MTK_DOE_CONFIG_ENV_SUPPORT
	DOE_UPDATE(ADCC_En);
	if (ADCC_En != 0xFFFFFFFF) {
		for (pwr = 0; pwr < NR_ADCC_PWR; pwr++) {
			switch (pwr) {
			case 0:
				cpu = 3;
				break;
			case 1:
				cpu = 1;
				break;
			case 2:
				cpu = 2;
				break;
			default:
				cpu = pwr+1;
				break;
			}
			adcc[cpu].set.adcc_cfg = enable;
			adcc_write_bits(GET_SET_ADDR(cpu),
				ADCC_cfg_bits,
				ADCC_cfg_shift,
				adcc[cpu].set.adcc_cfg);
		}
	}
#endif

	cnt = ADCC_BCPU_START_ID;
	for (cpu = ADCC_BCPU_START_ID; cpu <= ADCC_CPU_END_ID; cpu++) {

#ifndef CALIBEATION_EN
		adcc[cpu].set.SW_nFlag = 1;
		adcc_write_bits(GET_SET_ADDR(cpu), ADCC_SW_nFlag_bits, ADCC_SW_nFlag_shift, 1);
#endif
		cnt = cnt + adcc[cpu].set.SW_nFlag;
		if (cnt == NR_ADCC_CPU)
			goto OUT2;
	}

#ifdef CALIBEATION_EN
	if (adcc_set_voltage(ADCC_CPU_DVFS_L, 900000, 1) == ADCC_FALSE)
		return ADCC_FALSE;

	if (adcc_set_voltage(ADCC_CPU_DVFS_B, 900000, 1) == ADCC_FALSE)
		return ADCC_FALSE;

	/* get default freq*/
	cpum_default_freq = mt_get_cpum_freq();
	cpum_default_freq = (cpum_default_freq/1000) +
						((cpum_default_freq % 1000) > 500 ? 1:0);
	cpub_default_freq = mt_get_cpub_freq();
	cpub_default_freq = (cpub_default_freq/1000) +
						((cpub_default_freq % 1000) > 500 ? 1:0);

	adcc_cpu_pwr_drv_init();
	mate_set_armpll_cpum(1800);
	mate_set_armpll_cpub(1800);

	for (cpu = ADCC_BCPU_START_ID; cpu <= ADCC_CPU_END_ID; cpu++) {
		if (adcc_cpupm_cpu_pwr_on_prepare(cpu, (unsigned long)adcc_start))
			goto NEXT_GO;
	}

	for (cpu = ADCC_BCPU_START_ID; cpu <= ADCC_CPU_END_ID; cpu++) {
		cnt = 0;
		while (!adcc_read_bits(GET_SET_ADDR(cpu), ADCC_SW_nFlag_bits, ADCC_SW_nFlag_shift)) {
			if (cnt++ > 2000000) {
				adcc_msg("ADCC CPU%d Auto-calibration fail.\n", cpu);
				goto OUT;
			}
		}
		/* adcc_msg("cnt=%d\n", cnt); */
		adcc[cpu].set.SW_nFlag = 1;
		adcc_debug("ADCC CPU(%d) SET: 0x%x, PLL: 0x%x, FLL: 0x%x\n", cpu,
					adcc_read(GET_SET_ADDR(cpu)),
					adcc_read(GET_PLL_ADDR(cpu)),
					adcc_read(GET_FLL_ADDR(cpu)));
		adcc_debug("ADCC CPU%d Auto-calibration done.\n", cpu);
	}

	adcc_log("[ADCC] Auto-calibration done.\n");

OUT:
	for (cpu = ADCC_BCPU_START_ID; cpu <= ADCC_CPU_END_ID; cpu++) {
		adcc_cpupm_cpu_pwr_dwn_prepare(cpu);
	}

NEXT_GO:
	adcc_cpu_pwr_drv_restore();
	mate_set_armpll_cpum(cpum_default_freq);
	mate_set_armpll_cpub(cpub_default_freq);

	adcc_dump();

	adcc_set_voltage(ADCC_CPU_DVFS_L, cpum_default_volt, 0);
	adcc_set_voltage(ADCC_CPU_DVFS_B, cpub_default_volt, 0);

#endif /* CALIBEATION_EN */
	} /* end of normal_boot */

OUT2:
	/* SafeFreq disable */
	adcc_write_bits(0x0C1C003C, 1, 10, 0);
	adcc_write_bits(0x0C1D003C, 1, 10, 0);
	adcc_write_bits(0x0C1E003C, 1, 10, 0);
	adcc_write_bits(0x0C1F003C, 1, 10, 0);

	return ADCC_TRUE;
}


