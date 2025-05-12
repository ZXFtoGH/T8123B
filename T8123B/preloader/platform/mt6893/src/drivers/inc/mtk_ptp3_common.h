#ifndef __MTK_PTP3_H__
#define __MTK_PTP3_H__

/**
 * @file    mtk_ptp3_common.h
 * @brief   Driver header for ptp3 feature
 *
 */

/************************************************
 * Environment Control (SLT/FT/Android)
 ************************************************/
/* If in FT evn, enable FT_SOC_SIM */
/* #define FT_SOC_SIM */

/* If in SLT1 evn, enable SLT1_DRV */
/* #define SLT1_DRV */

/************************************************
 * Central control
 ************************************************/
#define PetrusP_ready

/************************************************
 * Platform Header Control
 ************************************************/
#ifdef FT_SOC_SIM
#else
#ifdef SLT1_DRV
#else
#endif /*end of SLT1_DRV */
#endif /*end of FT_SOC_SIM */

/************************************************
 * ADDR BASE
 ************************************************/
#define SYSAPB_BASE (0x40000000)
#define MCUSYS_CFG_BASE (0x0C530000)

/************************************************
 * BIT Operation and REG r/w
 ************************************************/
#undef  BIT
#define BIT(_bit_)                    (unsigned int)(1 << (_bit_))
#define BITS(_bits_, _val_)           ((((unsigned int) -1 >> (31 - ((1) ? _bits_))) \
& ~((1U << ((0) ? _bits_)) - 1)) & ((_val_)<<((0) ? _bits_)))
#define BITMASK(_bits_)               (((unsigned int) -1 >> (31 - ((1) ? _bits_))) & ~((1U << ((0) ? _bits_)) - 1))
#define GET_BITS_VAL(_bits_, _val_)   (((_val_) & (BITMASK(_bits_))) >> ((0) ? _bits_))

#ifdef FT_SOC_SIM /* For FT only, prohibited to use in ATF */
#define ptp3_read(addr)			(*(volatile unsigned int  *)(addr))
#define ptp3_write(addr,  val)	((*(volatile unsigned int *)(addr)) = (unsigned int)val)
#else
#ifdef SLT1_DRV /* For SLT1 only, prohibited to use in ATF */
#define ptp3_read(addr)			(*(volatile unsigned int  *)(addr))
#define ptp3_write(addr,  val)	((*(volatile unsigned int *)(addr)) = (unsigned int)val)
#else
#define ptp3_read(addr)			(*(volatile unsigned int  *)(addr))
#define ptp3_write(addr,  val)	((*(volatile unsigned int *)(addr)) = (unsigned int)val)
#endif /*end of SLT1_DRV */
#endif /*end of FT_SOC_SIM */


#define ptp3_shift(val, bits, shift) \
	((val & (((unsigned int)0x1 << bits) - 1)) << shift)

#define ptp3_write_bits(addr, bits, shift, val) \
	ptp3_write(addr, ((ptp3_read(addr) & ~(ptp3_shift((unsigned int)0xffffffff, \
	bits, shift))) | ptp3_shift(val, bits, shift)))

#define ptp3_read_bits(addr, bits, shift) \
	((ptp3_read(addr) >> shift) & (((unsigned int)0x1 << bits) - 1))

#define ptp3_set_bit(addr, set) ptp3_write(addr, (ptp3_read(addr) | (set)))
#define ptp3_clr_bit(addr, clr) ptp3_write(addr, (ptp3_read(addr) & ~(clr)))

/************************************************
 * config enum
 ************************************************/
enum PTP3_FEATURE {
	PTP3_FEATURE_PTP3,
	PTP3_FEATURE_BRISKET2,
	PTP3_FEATURE_ADCC,
	PTP3_FEATURE_FLL,
	PTP3_FEATURE_CTT,
	PTP3_FEATURE_DRCC,
	PTP3_FEATURE_CINST,
	PTP3_FEATURE_DT,
	PTP3_FEATURE_PDP,
	PTP3_FEATURE_IGLRE,

	NR_PTP3_FEATURE,
};

enum PTP3_POWER_CTRL {
	PTP3_POWER_CTRL_MCUSYS,
	PTP3_POWER_CTRL_CLUSTER,
	PTP3_POWER_CTRL_CORE,

	NR_PTP3_POWER_CTRL,
};

/************************************
 * prototype
 ************************************/
/* init trigger for ptp3 feature */
extern void ptp3_core_init(unsigned int core);
extern void ptp3_core_unInit(unsigned int core);

/* recevieve para from smc */
extern int ptp3_smc_ctrl(
	enum PTP3_FEATURE feature,
	unsigned int x2,
	unsigned int x3,
	unsigned int x4);

#endif /* end of __MTK_PTP3_H__ */

