/**
 * @file    mtk_ptp3_adcc.c
 * @brief   Driver for Advance Duty Cycle Control
 *
 */
#include <mtk_ptp3_adcc.h>

#if ADCC_ATF
#include <arch_helpers.h>
#include <platform.h>
#include "mtk_ptp3_common.h"
#else
#include <typedefs.h>
#include <platform.h>
#include <pll.h>
#include <pmic.h>
#include <pmic_wrap_init.h>
#include <wdt_v2.h>
#include <mtk_ptp3_adcc_spmc.h>
#include <delsel.h>
#include <regulator/mtk_regulator.h>
#ifdef MT6893_H
	#include "mt6359.h"
	#include "mt6315-regulator.h"
#endif
#include <ram_console_common.h>
#if MTK_DOE_CONFIG_ENV_SUPPORT
#include <dconfig_env.h>
#endif

#endif

#include "timer.h"

/************************************************
 * Debug print
 ************************************************/
#define ADCC_TRUE 1
#define ADCC_FALSE 0
#define ADCC_TAG	 "[ADCC]"
#ifdef FT_SOC_SIM /* For FT only, prohibited to use in ATF */
#define adcc_err(fmt, args...)	printf(ADCC_TAG"[ERROR][%s():%d]" fmt, __func__, __LINE__, ##args)
#define adcc_msg(fmt, args...)	printf(ADCC_TAG"[INFO][%s():%d]" fmt, __func__, __LINE__, ##args)
#define adcc_log	printf
#else
#ifdef SLT1_DRV /* For SLT1 only, prohibited to use in ATF */
#define adcc_err(fmt, args...)	printf(ADCC_TAG"[ERROR][%s():%d]" fmt, __func__, __LINE__, ##args)
#define adcc_msg(fmt, args...)	printf(ADCC_TAG"[INFO][%s():%d]" fmt, __func__, __LINE__, ##args)
#define adcc_log	printf
#else
#define adcc_err(fmt, args...)	ERROR(ADCC_TAG"[ERROR][%s():%d]" fmt, __func__, __LINE__, ##args)
#define adcc_msg(fmt, args...)	INFO(ADCC_TAG"[INFO][%s():%d]" fmt, __func__, __LINE__, ##args)
#define adcc_log	INFO
#endif /*end of SLT1_DRV */
#endif /*end of FT_SOC_SIM */

#ifdef ADCC_DEBUG
#ifdef FT_SOC_SIM /* For FT only, prohibited to use in ATF */
#define adcc_debug(fmt, args...)	printf(ADCC_TAG"[DEBUG][%s():%d]" fmt, __func__, __LINE__, ##args)
#else
#ifdef SLT1_DRV /* For SLT1 only, prohibited to use in ATF */
#define adcc_debug(fmt, args...)	printf(ADCC_TAG"[DEBUG][%s():%d]" fmt, __func__, __LINE__, ##args)
#else
#define adcc_debug(fmt, args...)	INFO(ADCC_TAG"[DEBUG][%s():%d]" fmt, __func__, __LINE__, ##args)
#endif /*end of SLT1_DRV */
#endif /*end of FT_SOC_SIM */
#else
#define adcc_debug(fmt, args...)
#endif

/************************************************
 * Static Variable
 ************************************************/

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

static const struct adcc_class adcc_s[NR_ADCC_CPU] = {
	[0] = {
		.DcTarget = DUTY_50,
		.DcdSelect = MDATA,
		.SW_nFlag = 0,
		.adcc_cfg = 3,
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
		.adcc_cfg = 3,
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
		.adcc_cfg = 3,
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
		.adcc_cfg = 3,
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
struct adcc_class adcc[NR_ADCC_CPU];

static const unsigned int ADCC_CPU_BASE[NR_ADCC_CPU] = {
	ADCC_CPU4_CONFIG_REG,
	ADCC_CPU5_CONFIG_REG,
	ADCC_CPU6_CONFIG_REG,
	ADCC_CPU7_CONFIG_REG
};

static const unsigned int ADCC_STORE_BASE[NR_ADCC_CPU] = {
	ADCC_CPU4_SET_STORE,
	ADCC_CPU5_SET_STORE,
	ADCC_CPU6_SET_STORE,
	ADCC_CPU7_SET_STORE
};

/************************************************
 * Static Function
 ************************************************/
unsigned int adcc_core_reorder(unsigned int core)
{
	return core - ADCC_CPU_START_ID;
}

static int adcc_dcc_en(unsigned int core)
{
	unsigned int adcc_core;

	if ((core < ADCC_CPU_START_ID) || (core > ADCC_CPU_END_ID))
		return ADCC_FALSE;

	adcc_core = adcc_core_reorder(core);

	if (adcc[adcc_core].PLL_CalDone != 1)
		return ADCC_FALSE;

	/* FllDccEn =0 */
	adcc_write_bits(ADCC_CPU_BASE[adcc_core] + ADCC_OFFSET_V114,
	FllDccEn_bits,
	FllDccEn_shift,
	0);
	/* DccClkShaperCalin */
	adcc_write_bits(ADCC_CPU_BASE[adcc_core] + ADCC_OFFSET_V109,
	DccClkShaperCalin_bits,
	DccClkShaperCalin_shift,
	adcc[adcc_core].PLL_CalOut);
	/* DccControlForce = 1 */
	adcc_write_bits(ADCC_CPU_BASE[adcc_core] + ADCC_OFFSET_V109,
	DccControlForce_bits,
	DccControlForce_shift,
	1);

	return ADCC_TRUE;

}

static int adcc_fll_en(unsigned int core)
{
	unsigned int adcc_core;

	if ((core < ADCC_CPU_START_ID) || (core > ADCC_CPU_END_ID))
		return ADCC_FALSE;

	adcc_core = adcc_core_reorder(core);

	if (adcc[adcc_core].FLL_CalDone != 1)
		return ADCC_FALSE;

	/*FllDccClkShaperCalin */
	adcc_write_bits(ADCC_CPU_BASE[adcc_core] + ADCC_OFFSET_V114,
	FllDccClkShaperCalin_bits,
	FllDccClkShaperCalin_shift,
	adcc[adcc_core].FLL_CalOut);

	/* FllDccEn =1 */
	adcc_write_bits(ADCC_CPU_BASE[adcc_core] + ADCC_OFFSET_V114,
	FllDccEn_bits,
	FllDccEn_shift,
	1);

	/*DccControlForce = 1 */
	adcc_write_bits(ADCC_CPU_BASE[adcc_core] + ADCC_OFFSET_V109,
	DccControlForce_bits,
	DccControlForce_shift,
	1);

	return ADCC_TRUE;
}

int adcc_set_Shaper(unsigned int core, unsigned int Shaper)
{
	unsigned int adcc_core;

	if ((core < ADCC_CPU_START_ID) || (core > ADCC_CPU_END_ID))
		return ADCC_FALSE;

	adcc_core = adcc_core_reorder(core);

	adcc[adcc_core].adcc_cfg = Shaper;

	adcc_write_bits(ADCC_STORE_BASE[adcc_core],
	ADCC_cfg_bits,
	ADCC_cfg_shift,
	adcc[adcc_core].adcc_cfg);

	return ADCC_TRUE;
}

int adcc_set_Calin(unsigned int core, unsigned int Calin)
{
	unsigned int adcc_core;

	if ((core < ADCC_CPU_START_ID) || (core > ADCC_CPU_END_ID))
		return ADCC_FALSE;

	adcc_core = adcc_core_reorder(core);

	if (adcc[adcc_core].adcc_cfg == 1) {
		adcc[adcc_core].PLL_CalOut = Calin;
		/* preloader DVT only */
		adcc_write_bits(ADCC_STORE_BASE[adcc_core] + PLL_OFFSET,
		ADCC_CalOut_bits,
		ADCC_CalOut_shift,
		adcc[adcc_core].PLL_CalOut);
	} else if (adcc[adcc_core].adcc_cfg == 2) {
		adcc[adcc_core].FLL_CalOut = Calin;
		/* preloader DVT only */
		adcc_write_bits(ADCC_STORE_BASE[adcc_core] + FLL_OFFSET,
		ADCC_CalOut_bits,
		ADCC_CalOut_shift,
		adcc[adcc_core].FLL_CalOut);
	}

	return ADCC_TRUE;
}

int adcc_set_DcdSelect(unsigned int core, unsigned int DcdSelect)
{
	unsigned int adcc_core;

	if ((core < ADCC_CPU_START_ID) || (core > ADCC_CPU_END_ID))
		return ADCC_FALSE;

	adcc_core = adcc_core_reorder(core);

	adcc[adcc_core].DcdSelect = DcdSelect;
	adcc_write_bits(ADCC_STORE_BASE[adcc_core],
		ADCC_DcdSelect_bits,
		ADCC_DcdSelect_shift,
	adcc[adcc_core].DcdSelect);

	adcc[adcc_core].SW_nFlag = 0;
	adcc_write_bits(ADCC_STORE_BASE[adcc_core],
		ADCC_SW_nFlag_bits,
		ADCC_SW_nFlag_shift,
	adcc[adcc_core].SW_nFlag);

	return ADCC_TRUE;
}

int adcc_set_DcTarget(unsigned int core, unsigned int DcTarget)
{
	unsigned int adcc_core;

	if ((core < ADCC_CPU_START_ID) || (core > ADCC_CPU_END_ID))
		return ADCC_FALSE;

	adcc_core = adcc_core_reorder(core);

	adcc[adcc_core].DcTarget = DcTarget;
	adcc_write_bits(ADCC_STORE_BASE[adcc_core],
		ADCC_DcTarget_bits,
		ADCC_DcTarget_shift,
	adcc[adcc_core].DcTarget);

	adcc[adcc_core].SW_nFlag = 0;
	adcc_write_bits(ADCC_STORE_BASE[adcc_core],
		ADCC_SW_nFlag_bits,
		ADCC_SW_nFlag_shift,
	adcc[adcc_core].SW_nFlag);

	return ADCC_TRUE;
}

static int adcc_reg_read(unsigned int core, unsigned int index)
{
	unsigned int adcc_core, ret = 0;

	if ((core < ADCC_CPU_START_ID) || (core > ADCC_CPU_END_ID))
		return ADCC_FALSE;

	adcc_core = adcc_core_reorder(core);

	switch (index) {
	case 0:
		ret = adcc_read(ADCC_STORE_BASE[adcc_core]);
		break;
	case 1:
		ret = adcc_read(ADCC_STORE_BASE[adcc_core] + PLL_OFFSET);
		break;
	case 2:
		ret = adcc_read(ADCC_STORE_BASE[adcc_core] + FLL_OFFSET);
		break;
	case 3:
		ret = adcc_read(ADCC_EFUSE_FLAG);
		break;
	case 4:
		ret = adcc_read(ADCC_CPU_BASE[adcc_core] + ADCC_OFFSET_V109);
		break;
	case 5:
		ret = adcc_read(ADCC_CPU_BASE[adcc_core] + ADCC_OFFSET_V110);
		break;
	case 6:
		ret = adcc_read(ADCC_CPU_BASE[adcc_core] + ADCC_OFFSET_V114);
		break;
	case 7:
		ret = adcc_read(ADCC_CPU_BASE[adcc_core] + ADCC_OFFSET_DCD);
		break;
	case 8:
		ret = adcc[adcc_core].set;
		break;
	case 9:
		ret = adcc[adcc_core].PLL;
		break;
	case 10:
		ret = adcc[adcc_core].FLL;
		break;
	case 11:
		ret = adcc_read(ADCC_EFUSE_DATA);
		break;
	case 12:
		ret = adcc_read(ADCC_STORE_BASE[adcc_core] + DBG_OFFSET);
		break;
	default:
		break;
	}

	return ret;
}


/************************************************
 * non-Static Function
 ************************************************/
unsigned int adcc_control(unsigned int attr, unsigned int core, unsigned int value)
{
	/* check for core if illegal */
	if ((core < ADCC_CPU_START_ID) || (core > ADCC_CPU_END_ID)) {
		adcc_err("core(%d) is illegal\n", core);
		return ADCC_FALSE;
	}

	adcc_debug("attr(%d) core(0x%08x) value(0x%08x)\n", attr, core, value);

	switch (attr) {
	case ADCC_ENABLE:
		adcc_init(core);
		return adcc_en(core, value);
	case ADCC_SET_SHAPER:
		return adcc_set_Shaper(core, value);
	case ADCC_SET_CALIN:
		return adcc_set_Calin(core, value);
	case ADCC_SET_DCDSELECT:
		return adcc_set_DcdSelect(core, value);
	case ADCC_SET_DCTARGET:
		return adcc_set_DcTarget(core, value);
	case ADCC_DUMP_INFO:
		return adcc_reg_read(core, value);
	default:
		break;
	}

	return ADCC_TRUE;
}

unsigned int adcc_init(unsigned int core)
{
	unsigned int adcc_core;

	if ((core >= ADCC_CPU_START_ID) && (core <= ADCC_CPU_END_ID)) {
		adcc_core = adcc_core_reorder(core);

		adcc[adcc_core].set = adcc_read(ADCC_STORE_BASE[adcc_core]);
		if (adcc[adcc_core].SET_checksum != CHECKSUM_S)
			adcc[adcc_core].adcc_cfg = 0;

		adcc[adcc_core].PLL = adcc_read(ADCC_STORE_BASE[adcc_core] + PLL_OFFSET);
		if (adcc[adcc_core].PLL_checksum != CHECKSUM)
			adcc[adcc_core].adcc_cfg = 0;

		adcc[adcc_core].FLL = adcc_read(ADCC_STORE_BASE[adcc_core] + FLL_OFFSET);
		if (adcc[adcc_core].FLL_checksum != CHECKSUM)
			adcc[adcc_core].adcc_cfg = 0;

		return adcc[adcc_core].adcc_cfg;
	}
	return ADCC_FALSE;
}

unsigned int adcc_en(unsigned int core, unsigned int enable)
{
	unsigned int adcc_core;

	if ((core < ADCC_CPU_START_ID) || (core > ADCC_CPU_END_ID))
		return ADCC_FALSE;

	adcc_core = adcc_core_reorder(core);

	switch (enable) {
	case ADCC_DEFAULT:
		/* DccClkShaperCalin = 0x2 */
		adcc_write_bits(ADCC_CPU_BASE[adcc_core] + ADCC_OFFSET_V109,
			DccClkShaperCalin_bits,
			DccClkShaperCalin_shift,
			2);

		/*FllDccClkShaperCalin = 0x0 */
		adcc_write_bits(ADCC_CPU_BASE[adcc_core] + ADCC_OFFSET_V114,
			FllDccClkShaperCalin_bits,
			FllDccClkShaperCalin_shift,
			0);

		/* FllDccEn =0 */
		adcc_write_bits(ADCC_CPU_BASE[adcc_core] + ADCC_OFFSET_V114,
			FllDccEn_bits,
			FllDccEn_shift,
			0);

		adcc_set_Shaper(core, ADCC_DEFAULT);
		adcc[adcc_core].PLL_CalOut = 2;
		adcc[adcc_core].PLL_CalDone = 1;
		adcc[adcc_core].FLL_CalOut = 0;
		adcc[adcc_core].FLL_CalDone = 1;
		break;
	case ADCC_PLL:
		adcc_set_Shaper(core, ADCC_PLL);
		adcc_dcc_en(core);
		break;
	case ADCC_FLL:
		adcc_set_Shaper(core, ADCC_FLL);
		adcc_fll_en(core);
		break;
	case ADCC_FLL_AUTO:
		adcc_set_Shaper(core, ADCC_FLL_AUTO);
		adcc_fll_en(core);
		break;
	case ADCC_PLL_AUTO:
		adcc_set_Shaper(core, ADCC_PLL_AUTO);
		adcc_dcc_en(core);
		break;
	case ADCC_FLL_15:
		adcc_set_Shaper(core, ADCC_FLL_15);
		adcc[adcc_core].FLL_CalDone = 1;
		adcc[adcc_core].FLL_CalOut = 15;
		adcc_fll_en(core);
		break;
	case ADCC_FLL_16:
		adcc_set_Shaper(core, ADCC_FLL_16);
		adcc[adcc_core].FLL_CalDone = 1;
		adcc[adcc_core].FLL_CalOut = 16;
		adcc_fll_en(core);
		break;
	case ADCC_PLL_15:
		adcc_set_Shaper(core, ADCC_PLL_15);
		adcc[adcc_core].PLL_CalDone = 1;
		adcc[adcc_core].PLL_CalOut = 15;
		adcc_dcc_en(core);
		break;
	case ADCC_PLL_16:
		adcc_set_Shaper(core, ADCC_PLL_16);
		adcc[adcc_core].PLL_CalDone = 1;
		adcc[adcc_core].PLL_CalOut = 16;
		adcc_dcc_en(core);
		break;
	case ADCC_FLL_DEFAULT:
		/* DccClkShaperCalin = 0x2 */
		adcc_write_bits(ADCC_CPU_BASE[adcc_core] + ADCC_OFFSET_V109,
			DccClkShaperCalin_bits,
			DccClkShaperCalin_shift,
			2);

		/*FllDccClkShaperCalin = 0x0 */
		adcc_write_bits(ADCC_CPU_BASE[adcc_core] + ADCC_OFFSET_V114,
			FllDccClkShaperCalin_bits,
			FllDccClkShaperCalin_shift,
			0);

		/* FllDccEn =1 */
		adcc_write_bits(ADCC_CPU_BASE[adcc_core] + ADCC_OFFSET_V114,
			FllDccEn_bits,
			FllDccEn_shift,
			1);

		adcc_set_Shaper(core, ADCC_FLL_DEFAULT);
		adcc[adcc_core].PLL_CalOut = 2;
		adcc[adcc_core].PLL_CalDone = 1;
		adcc[adcc_core].FLL_CalOut = 0;
		adcc[adcc_core].FLL_CalDone = 1;
	default:
		break;
	}
		return ADCC_TRUE;
}

/**********************************
 * pre-loader only
 **********************************/
#ifdef MT6893_H

/* PLL */
#define C03_ARMPLL_CON1_PHY					0x1000C20C /* core 03 */
#define C4_ARMPLL_CON1_PHY					0x1000C22C /* core 4 */
#define C5_ARMPLL_CON1_PHY					0x1000C23C /* core 5 */
#define C6_ARMPLL_CON1_PHY					0x1000C24C /* core 6 */
#define C7_ARMPLL_CON1_PHY					0x1000C21C /* core 7 */
#define CCI_ARMPLL_CON1_PHY					0x1000C25C /* CCI */

/* [10:9] muxsel: switch to PLL speed ==> 0: 26M,  1: armpll, 2:  Mainpll, 3:  Unipll */
/* [21:17] divsel: CPU clock divide by 1, ==> 8: div1, A: div2, B: div4, 1D: div6 */
#define C03_ARMPLL_DIV_PHY					0x0C53A2A0 /* core 03 */
#define C4_ARMPLL_DIV_PHY					0x0C53A2A4 /* core 4 */
#define C5_ARMPLL_DIV_PHY					0x0C53A2A8 /* core 5 */
#define C6_ARMPLL_DIV_PHY					0x0C53A2AC /* core 6 */
#define C7_ARMPLL_DIV_PHY					0x0C53A2B0 /* core 7 */
#define CCI_ARMPLL_DIV_PHY					0x0C53A2E0 /* CCI */

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
#define FREQ_DIV(f_mhz)						(0x8) /*((f_mhz <= 374) ? 0xb : ((f_mhz <= 749) ? 0xa : 0x8)) */

#define FREQ_SET_DCM_DIV_Q(dcm, div, f_mhz)	adcc_write(dcm, FREQ_DCM(f_mhz)),\
											adcc_write(div, (adcc_read(div) & 0xFFC1F9FF) | (FREQ_DIV(f_mhz) << 17) | 0x200) //force select armpll

#define FREQ_SET_DCM_DIV_LOG(dcm, div, f_mhz)	adcc_debug("%s, Freq = %d, DCM[0x%x] = 0x%x, DIV[0x%x] = 0x%x.\n",\
												(((dcm == C03_ARMPLL_CON1_PHY)) ? "Core0-3" :\
												(((dcm ==  C4_ARMPLL_CON1_PHY)) ? "Core4" :\
												(((dcm ==  C5_ARMPLL_CON1_PHY)) ? "Core5" :\
												(((dcm ==  C6_ARMPLL_CON1_PHY)) ? "Core6" :\
												(((dcm ==  C7_ARMPLL_CON1_PHY)) ? "Core7" : "CCI    "))))),\
												f_mhz, dcm, FREQ_DCM(f_mhz), div, (adcc_read(div) & 0xFFC1FFFF) | (FREQ_DIV(f_mhz) << 17))

#else

#define	PMIC_VPROC_SET(cls_n, volt_uv)
#define	PMIC_VSRAM_SET(cls_n, volt_uv)
#define	PMIC_VPROC_GET(cls_n)
#define	PMIC_VSRAM_GET(cls_n)

/* PLL */
#define C03_ARMPLL_CON1_PHY					0x1000C20C /* core 03 */
#define C4_ARMPLL_CON1_PHY					0x1000C22C /* core 4 */
#define C5_ARMPLL_CON1_PHY					0x1000C23C /* core 5 */
#define C6_ARMPLL_CON1_PHY					0x1000C24C /* core 6 */
#define C7_ARMPLL_CON1_PHY					0x1000C21C /* core 7 */
#define CCI_ARMPLL_CON1_PHY					0x1000C25C /* CCI */

/* [10:9] muxsel: switch to PLL speed ==> 0: 26M,  1: armpll, 2:  Mainpll, 3:  Unipll */
/* [21:17] divsel: CPU clock divide by 1, ==> 8: div1, A: div2, B: div4, 1D: div6 */
#define C03_ARMPLL_DIV_PHY					0x0C53A2A0 /* core 03 */
#define C4_ARMPLL_DIV_PHY					0x0C53A2A4 /* core 4 */
#define C5_ARMPLL_DIV_PHY					0x0C53A2A8 /* core 5 */
#define C6_ARMPLL_DIV_PHY					0x0C53A2AC /* core 6 */
#define C7_ARMPLL_DIV_PHY					0x0C53A2B0 /* core 7 */
#define CCI_ARMPLL_DIV_PHY					0x0C53A2E0 /* CCI */

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
#define FREQ_DIV(f_mhz)						(0x8) /*((f_mhz <= 374) ? 0xb : ((f_mhz <= 749) ? 0xa : 0x8)) */

#define FREQ_SET_DCM_DIV_Q(dcm, div, f_mhz)	adcc_write(dcm, FREQ_DCM(f_mhz)),\
											adcc_write(div, (adcc_read(div) & 0xFFC1F9FF) | (FREQ_DIV(f_mhz) << 17) | 0x200) //force select armpll

#define FREQ_SET_DCM_DIV_LOG(dcm, div, f_mhz)	adcc_debug("%s, Freq = %d, DCM[0x%x] = 0x%x, DIV[0x%x] = 0x%x.\n",\
												(((dcm == C03_ARMPLL_CON1_PHY)) ? "Core0-3" :\
												(((dcm ==  C4_ARMPLL_CON1_PHY)) ? "Core4" :\
												(((dcm ==  C5_ARMPLL_CON1_PHY)) ? "Core5" :\
												(((dcm ==  C6_ARMPLL_CON1_PHY)) ? "Core6" :\
												(((dcm ==  C7_ARMPLL_CON1_PHY)) ? "Core7" : "CCI    "))))),\
												f_mhz, dcm, FREQ_DCM(f_mhz), div, (adcc_read(div) & 0xFFC1FFFF) | (FREQ_DIV(f_mhz) << 17))

#endif


#if MTK_DOE_CONFIG_ENV_SUPPORT
#define DOE_UPDATE(_item)	\
{	\
	doe_##_item = dconfig_getenv(#_item);	\
	if (doe_##_item)	\
		_item = atoi(doe_##_item);\
	adcc_msg(#_item " %x\n", _item);\
}
#endif


static const struct mtk_regulator empty_regulator;
static unsigned int adcc_set_BCPU_voltage(int vproc1_vol)
{
	int ret = 0;
	struct mtk_regulator reg_vsram_proc1 = empty_regulator;
	struct mtk_regulator reg_vproc1 = empty_regulator;
	/* int vproc1_vol = 900000; */
	int vsram_proc1_vol;

	/*--Get regulator handle--*/
	if (mtk_regulator_get("vsram_proc1", &reg_vsram_proc1))
		ret |= (1 << 1);

	if (mtk_regulator_get("vproc1", &reg_vproc1))
		ret |= (1 << 7);

	if (ret) {
		adcc_msg("mtk_regulator_get failed.(0x%x)\n", ret);
		return ADCC_FALSE;
	}

	/*--Set voltage--*/
	if (vproc1_vol + 100000 > 1000000)
			vsram_proc1_vol = 1000000;
	else if (vproc1_vol + 100000 < 750000)
		vsram_proc1_vol = 750000;
	else
		vsram_proc1_vol = vproc1_vol + 100000;

	if (mtk_regulator_set_voltage(&reg_vsram_proc1, vsram_proc1_vol, vsram_proc1_vol))
		ret |= (1 << 0);
	if (mtk_regulator_set_voltage(&reg_vproc1, vproc1_vol, vproc1_vol))
		ret |= (1 << 2);
	if (ret) {
		adcc_msg("mtk_regulator_set_voltage failed.(0x%x)\n", ret);
		return ADCC_FALSE;
	}

	/*--Get voltage--*/
	adcc_debug("vsram_proc1 = %d uV\n", mtk_regulator_get_voltage(&reg_vsram_proc1));
	adcc_debug("vproc1 = %d uV\n", mtk_regulator_get_voltage(&reg_vproc1));

	return ADCC_TRUE;
}

static unsigned int set_freq(unsigned int core, unsigned int freq)
{
	switch (core) {
	case 4:
		if (freq >= 1800000)
			cache_delsel_init(MT_CPU_DVFS_L, freq);
		FREQ_SET_DCM_DIV_Q(C4_ARMPLL_CON1_PHY, C4_ARMPLL_DIV_PHY, (freq/1000));
		FREQ_SET_DCM_DIV_LOG(C4_ARMPLL_CON1_PHY, C4_ARMPLL_DIV_PHY, (freq/1000));
		if (freq == 730000)
			cache_delsel_init(MT_CPU_DVFS_L, freq);
		break;
	case 5:
		if (freq >= 1800000)
			cache_delsel_init(MT_CPU_DVFS_L, freq);
		FREQ_SET_DCM_DIV_Q(C5_ARMPLL_CON1_PHY, C5_ARMPLL_DIV_PHY, (freq/1000));
		FREQ_SET_DCM_DIV_LOG(C5_ARMPLL_CON1_PHY, C5_ARMPLL_DIV_PHY, (freq/1000));
		if (freq == 730000)
			cache_delsel_init(MT_CPU_DVFS_L, freq);
		break;
	case 6:
		if (freq >= 1800000)
			cache_delsel_init(MT_CPU_DVFS_L, freq);
		FREQ_SET_DCM_DIV_Q(C6_ARMPLL_CON1_PHY, C6_ARMPLL_DIV_PHY, (freq/1000));
		FREQ_SET_DCM_DIV_LOG(C6_ARMPLL_CON1_PHY, C6_ARMPLL_DIV_PHY, (freq/1000));
		if (freq == 730000)
			cache_delsel_init(MT_CPU_DVFS_L, freq);
		break;
	case 7:
		if (freq >= 1800000)
			cache_delsel_init(MT_CPU_DVFS_B, freq);
		FREQ_SET_DCM_DIV_Q(C7_ARMPLL_CON1_PHY, C7_ARMPLL_DIV_PHY, (freq/1000));
		FREQ_SET_DCM_DIV_LOG(C7_ARMPLL_CON1_PHY, C7_ARMPLL_DIV_PHY, (freq/1000));
		if (freq == 730000)
			cache_delsel_init(MT_CPU_DVFS_B, freq);
		break;
	default:
		return ADCC_FALSE;
	}

	return ADCC_TRUE;
}

unsigned int adcc_pre_init(void)
{
#if (ADCC_DISABLE == 0)
	unsigned int core, adcc_core, cnt;
#if MTK_DOE_CONFIG_ENV_SUPPORT
	unsigned int enable;
	unsigned int ADCC_En = 0xFFFFFFFF;
	char *doe_ADCC_En;
#endif

	/* cold boot re-calibration */
	adcc_debug("cold(0)/warm(2) boot check: %d\n", g_rgu_status);
	if (ram_console_is_abnormal_boot() == 0) {
		if (g_rgu_status == 0) {
		/* cold boot */
			for (core = ADCC_CPU_START_ID; core <= ADCC_CPU_END_ID; core++) {
				adcc_core = adcc_core_reorder(core);
				adcc[adcc_core] = adcc_s[adcc_core];
				adcc_write(ADCC_STORE_BASE[adcc_core], adcc[adcc_core].set);
				adcc_write(ADCC_STORE_BASE[adcc_core] + PLL_OFFSET, adcc[adcc_core].PLL);
				adcc_write(ADCC_STORE_BASE[adcc_core] + FLL_OFFSET, adcc[adcc_core].FLL);
				adcc_write(ADCC_STORE_BASE[adcc_core] + DBG_OFFSET, 0);
			}
		} else {
		/* data corrupt re-calibration */
			for (core = ADCC_CPU_START_ID; core <= ADCC_CPU_END_ID; core++) {
				adcc_core = adcc_core_reorder(core);
				adcc[adcc_core].set = adcc_read(ADCC_STORE_BASE[adcc_core]);
				if (adcc[adcc_core].SET_checksum != CHECKSUM_S) {
					adcc[adcc_core].set = adcc_s[adcc_core].set;
					adcc_write(ADCC_STORE_BASE[adcc_core], adcc[adcc_core].set);
				}
				adcc[adcc_core].PLL = adcc_read(ADCC_STORE_BASE[adcc_core] + PLL_OFFSET);
				if (adcc[adcc_core].PLL_checksum != CHECKSUM) {
					adcc[adcc_core].PLL = adcc_s[adcc_core].PLL;
					adcc_write(ADCC_STORE_BASE[adcc_core] + PLL_OFFSET, adcc[adcc_core].PLL);
				}
				adcc[adcc_core].FLL = adcc_read(ADCC_STORE_BASE[adcc_core] + FLL_OFFSET);
				if (adcc[adcc_core].FLL_checksum != CHECKSUM) {
					adcc[adcc_core].FLL = adcc_s[adcc_core].FLL;
					adcc_write(ADCC_STORE_BASE[adcc_core] + FLL_OFFSET, adcc[adcc_core].FLL);
				}
			}
		}

#if MTK_DOE_CONFIG_ENV_SUPPORT
	DOE_UPDATE(ADCC_En);
	if (ADCC_En != 0xFFFFFFFF) {
		for (core = ADCC_CPU_START_ID; core <= ADCC_CPU_END_ID; core++) {
			adcc_core = adcc_core_reorder(core);
			enable = (ADCC_En >> (core * 4)) & 0xF;
			adcc[adcc_core].adcc_cfg = enable;
			adcc_write_bits(ADCC_STORE_BASE[adcc_core],
				ADCC_cfg_bits,
				ADCC_cfg_shift,
				adcc[adcc_core].adcc_cfg);
		}
	}
#endif

	cnt = 0;
	for (core = ADCC_CPU_START_ID; core <= ADCC_CPU_END_ID; core++) {
		adcc_core = adcc_core_reorder(core);
#ifndef CALIBEATION_EN
		adcc[adcc_core].SW_nFlag = 1;
		adcc_write_bits(ADCC_STORE_BASE[adcc_core], ADCC_SW_nFlag_bits, ADCC_SW_nFlag_shift, 1);
#endif
		cnt = cnt + adcc[adcc_core].SW_nFlag;
		if (cnt == NR_ADCC_CPU)
			return ADCC_TRUE;
	}

	/* disable DREQ */
	adcc_write(0x0C53AD8C, 0x0);
	adcc_debug("0x0C53AD8C = %d (expected: 0)\n", adcc_read(0x0C53AD8C));

	if(adcc_set_BCPU_voltage(900000) == ADCC_FALSE)
		return ADCC_FALSE;

	spmc_init(0);
	for (core = ADCC_CPU_START_ID; core <= ADCC_CPU_END_ID; core++) {
		adcc_core = adcc_core_reorder(core);
		set_freq(core, 1800000);
		boot_secondary(core);
	}

	for (core = ADCC_CPU_START_ID; core <= ADCC_CPU_END_ID; core++) {
		cnt = 0;
		while (!adcc_read_bits(ADCC_STORE_BASE[adcc_core], ADCC_SW_nFlag_bits, ADCC_SW_nFlag_shift)) {
			if (cnt++ > 2000000) {
				adcc_msg("CPU%d Auto-calibation fail.\n", core);
				return ADCC_FALSE;
			}
		}
		/* adcc_msg("cnt=%d\n", cnt); */
		adcc[adcc_core].SW_nFlag = 1;
		adcc_debug("CPU(%d) SET: 0x%x, PLL: 0x%x, FLL: 0x%x\n", core,
					adcc_reg_read(core, 0),
					adcc_reg_read(core, 1),
					adcc_reg_read(core, 2));
		adcc_debug("CPU%d Auto-calibation done.\n", core);
	}

	for (core = ADCC_CPU_START_ID; core <= ADCC_CPU_END_ID; core++) {
		adcc_core = adcc_core_reorder(core);
		power_down_cpu(core);
		set_freq(core, 730000);
	}

	adcc_set_BCPU_voltage(650000);
	adcc_dump();
	}

#endif
	return ADCC_TRUE;

}

unsigned int adcc_cfg(unsigned int set, unsigned int value)
{
	unsigned int enable = 0, dump_set;
	unsigned int core;

	if (set == 1) {
		for (core = ADCC_CPU_START_ID; core <= ADCC_CPU_END_ID; core++) {
			enable = (value >> (core * 4)) & 0xF;
			adcc_init(core);
			adcc_en(core, enable);
		}
	} else {
		for (core = ADCC_CPU_START_ID; core <= ADCC_CPU_END_ID; core++) {
			dump_set = adcc_reg_read(core, 8);
			enable = enable | ((GET_BITS_VAL(20:17, dump_set) & 0xF) << (core * 4));
		}
		adcc_log("[CPU][ADCC]adcc_cfg: %x", enable);
	}

	return enable;

}

void adcc_dump(void)
{
	unsigned int core, temp, adcc_core;
	unsigned int dump_set, dump_PLL, dump_FLL;

	for (core = ADCC_CPU_START_ID; core <= ADCC_CPU_END_ID; core++) {
		dump_set = adcc_reg_read(core, 0);
		dump_PLL = adcc_reg_read(core, 1);
		dump_FLL = adcc_reg_read(core, 2);

		adcc_log("[ADCC][CPU%d]", core);
		adcc_log(" 0x%x,", GET_BITS_VAL(20:17, dump_set));
		adcc_log(" 0x%x,", GET_BITS_VAL(16:16, dump_set));
		adcc_log(" 0x%x,", GET_BITS_VAL(15:8, dump_set));
		adcc_log(" 0x%x,", GET_BITS_VAL(7:0, dump_set));

		adcc_log(" 0x%x,", GET_BITS_VAL(5:5, dump_FLL));
		adcc_log(" 0x%x,", GET_BITS_VAL(4:0, dump_FLL));

		if (GET_BITS_VAL(16:16, dump_FLL) == 1) {
			temp = GET_BITS_VAL(15:6, dump_FLL);
			adcc_log(" %d,", temp);
			if (temp >= 512)
				adcc_log(" %d,",
					(5000+(((temp-512)*10000)/512)));
			else
				adcc_log(" %d,",
					(5000-(((512-temp)*10000)/512)));
		} else {
			adcc_log(" 0, 0,");
		}

		adcc_log(" 0x%x,", GET_BITS_VAL(5:5, dump_PLL));
		adcc_log(" 0x%x,", GET_BITS_VAL(4:0, dump_PLL));

		if (GET_BITS_VAL(16:16, dump_PLL) == 1) {
			temp = GET_BITS_VAL(15:6, dump_PLL);
			adcc_log(" %d,", temp);
			if (temp >= 512)
				adcc_log(" %d,",
					(5000+(((temp-512)*10000)/512)));
			else
				adcc_log(" %d,",
					(5000-(((512-temp)*10000)/512)));
		} else {
			adcc_log(" 0, 0,");
		}

		adcc_log(" 0x%x,", adcc_reg_read(core, 11));
		adcc_log(" 0x%x,", adcc_reg_read(core, 3));
		adcc_log(" 0x%x\n", adcc_reg_read(core, 12));
	}
}

void adcc_dump_reg(void)
{
	unsigned int core;

	for (core = ADCC_CPU_START_ID; core <= ADCC_CPU_END_ID; core++) {
		adcc_log("[ADCC][CPU%d] SET:0x%x,", core, adcc_reg_read(core, 0));
		adcc_log(" PLL:0x%x,", adcc_reg_read(core, 1));
		adcc_log(" FLL:0x%x,", adcc_reg_read(core, 2));
		adcc_log(" EFV:0x%x,", adcc_reg_read(core, 3));
		adcc_log(" EFD:0x%x,", adcc_reg_read(core, 11));
		adcc_log(" 109:0x%x,", adcc_reg_read(core, 4));
		adcc_log(" 110:0x%x,", adcc_reg_read(core, 5));
		adcc_log(" 114:0x%x,", adcc_reg_read(core, 6));
		adcc_log(" DCD:0x%x,", adcc_reg_read(core, 7));
		adcc_log(" DOS:0x%x,", adcc_reg_read(core, 8));
		adcc_log(" DOP:0x%x,", adcc_reg_read(core, 9));
		adcc_log(" DOF:0x%x\n", adcc_reg_read(core, 10));
	}
}

