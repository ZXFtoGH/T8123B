#ifndef __MTK_ADCC_H__
#define __MTK_ADCC_H__

/**
 * @file    mtk_ptp3_adcc.h
 * @brief   Driver header for Advance Duty Cycle Control
 *
 */

#include "typedefs.h"
#include "print.h"
/************************************************
 * Central control
 ************************************************/
/* #define ADCC_DEBUG_FLAG */
#define CALIBEATION_EN
/* #define FT_SOC_SIM */

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
#define adcc_err(fmt, args...)	print(ADCC_TAG"[ERROR][%s():%d]" fmt, __func__, __LINE__, ##args)
#define adcc_msg(fmt, args...)	print(ADCC_TAG"[INFO][%s():%d]" fmt, __func__, __LINE__, ##args)
#define adcc_log	print
#endif /*end of SLT1_DRV */
#endif /*end of FT_SOC_SIM */

#ifdef ADCC_DEBUG_FLAG
#ifdef FT_SOC_SIM /* For FT only, prohibited to use in ATF */
#define adcc_debug(fmt, args...)	printf(ADCC_TAG"[DEBUG][%s():%d]" fmt, __func__, __LINE__, ##args)
#else
#ifdef SLT1_DRV /* For SLT1 only, prohibited to use in ATF */
#define adcc_debug(fmt, args...)	printf(ADCC_TAG"[DEBUG][%s():%d]" fmt, __func__, __LINE__, ##args)
#else
#define adcc_debug(fmt, args...)	print(ADCC_TAG"[DEBUG][%s():%d]" fmt, __func__, __LINE__, ##args)
#endif /*end of SLT1_DRV */
#endif /*end of FT_SOC_SIM */
#else
#define adcc_debug(fmt, args...)
#endif


/************************************************
 * BIT Operation and REG r/w
 ************************************************/
#define adcc_read(addr)			(*(volatile unsigned int *)(unsigned long)(addr))
#define adcc_write(addr, val)	(*(volatile unsigned int *)(unsigned long)(addr) = (unsigned long)(val))


#define adcc_shift(val, bits, shift) \
	((val & (((unsigned int)0x1 << bits) - 1)) << shift)

#define adcc_shift_lsb(val, bits, shift) \
	((val & (((unsigned int)0x1 << bits) - 1) << shift) >> shift)

#define adcc_write_bits(addr, bits, shift, val) \
	adcc_write(addr, ((adcc_read(addr) & ~(adcc_shift((unsigned int)0xffffffff, \
	bits, shift))) | adcc_shift(val, bits, shift)))

#define adcc_read_bits(addr, bits, shift) \
	((adcc_read(addr) >> shift) & (((unsigned int)0x1 << bits) - 1))


/************************************************
 * Need maintain for each project
 ************************************************/
#define NR_ADCC_CPU			8
#define ADCC_CPU_START_ID	1
#define ADCC_BCPU_START_ID	4
#define ADCC_CPU_END_ID		7

/* ADDC control register range */
#define ADCC_SYSRAM_BASE	0X00115000
//#define ADCC_SYSRAM_BASE	0X40115000

#define GET_SET_ADDR(ii)	(ADCC_SYSRAM_BASE + ((ii) * 16))
#define GET_PLL_ADDR(ii)	(ADCC_SYSRAM_BASE + ((ii) * 16) + 0x4)
#define GET_FLL_ADDR(ii)	(ADCC_SYSRAM_BASE + ((ii) * 16) + 0x8)
#define GET_DBG_ADDR(ii)	(ADCC_SYSRAM_BASE + ((ii) * 16) + 0xc)
#define GET_DB1_ADDR(ii)	(ADCC_SYSRAM_BASE + ((ii) * 256))
#define GET_DB2_ADDR(ii)	(ADCC_SYSRAM_BASE + ((ii) * 256) + 0x4)
#define GET_DB3_ADDR(ii)	(ADCC_SYSRAM_BASE + ((ii) * 256) + 0x8)

#define ADCC_SW_nFlag_shift 16         /* 16:16 */
#define ADCC_SW_nFlag_bits 1
#define ADCC_cfg_shift 17              /* 20:17 */
#define ADCC_cfg_bits 4

/************************************************
 * config & struct
 ************************************************/

union set_u {
	struct {
		unsigned int DcTarget:8;        /* [7:0] */
		unsigned int DcdSelect:8;       /* [15:8] */
		unsigned int SW_nFlag:1;        /* [16:16] */
		unsigned int adcc_cfg:4;        /* [20:17] */
		unsigned int checksum:11;   /* [31:21] */
	};
	unsigned long long reg;
};


union dcc_u {
	struct {
		unsigned int CalOut:5;      /* [4:0] */
		unsigned int CalDone:1;     /* [5:5] */
		unsigned int integrator:10; /* [15:6] */
		unsigned int Ack:1;         /* [16:16] */
		unsigned int adcc_cfg:4;    /* [20:17] */
		unsigned int checksum:11;   /* [31:21] */
	};
	unsigned long long reg;
};


struct adcc_class {
	/* PLL, FLL, SET STORE */
	union set_u set;
	union dcc_u PLL;
	union dcc_u FLL;
};

enum ADCC_PWR {
	ADCC_PWR_MPSYS,
	ADCC_PWR_COMPLEX0,
	ADCC_PWR_COMPLEX1,
	ADCC_PWR_CPU4,
	ADCC_PWR_CPU5,
	ADCC_PWR_CPU6,
	ADCC_PWR_CPU7,

	NR_ADCC_PWR
};


extern unsigned int adcc_pre_init(void);
extern void adcc_dump(void);
#endif /* end of __MTK_ADCC_H__ */

