#ifndef __MTK_ADCC_H__
#define __MTK_ADCC_H__

/**
 * @file    mtk_ptp3_adcc.h
 * @brief   Driver header for Advance Duty Cycle Control
 *
 */

/************************************************
 * Central control
 ************************************************/
/* #define PetrusP_ready */
/* #define ADCC_DEBUG */
#define CALIBEATION_EN

#define ADCC_DISABLE 0

/* 0: Perloader/SLT, 1: ATF */
#define ADCC_ATF 0
#define mATE 0

#if ADCC_ATF
#include <debug.h>
#include <mmio.h>
#include "mtk_ptp3_brisket2.h"
#endif

/************************************************
 * BIT Operation and REG r/w
 ************************************************/
#undef  BIT
#define BIT(_bit_)                    (unsigned int)(1 << (_bit_))
#define BITS(_bits_, _val_)           ((((unsigned int) -1 >> (31 - ((1) ? _bits_))) \
& ~((1U << ((0) ? _bits_)) - 1)) & ((_val_)<<((0) ? _bits_)))
#define BITMASK(_bits_)               (((unsigned int) -1 >> (31 - ((1) ? _bits_))) & ~((1U << ((0) ? _bits_)) - 1))
#define GET_BITS_VAL(_bits_, _val_)   (((_val_) & (BITMASK(_bits_))) >> ((0) ? _bits_))

#define adcc_shift(val, bits, shift) \
	((val & (((unsigned int)0x1 << bits) - 1)) << shift)

#define adcc_shift_lsb(val, bits, shift) \
	((val & (((unsigned int)0x1 << bits) - 1) << shift) >> shift)

#if ADCC_ATF
#define adcc_read(addr)			mmio_read_32((uintptr_t)addr)
#define adcc_write(addr, val)	mmio_write_32((uintptr_t)addr,  val)
#else
#define adcc_read(addr)			(*(volatile unsigned int *)(addr))
#define adcc_write(addr, val)	(*(volatile unsigned int *)(addr) = (unsigned long)(val))
#endif

#define adcc_write_bits(addr, bits, shift, val) \
	adcc_write(addr, ((adcc_read(addr) & ~(adcc_shift((unsigned int)0xffffffff, \
	bits, shift))) | adcc_shift(val, bits, shift)))

#define adcc_read_bits(addr, bits, shift) \
	((adcc_read(addr) >> shift) & (((unsigned int)0x1 << bits) - 1))


#define adcc_set_bit(addr, set) adcc_write(addr, (adcc_read(addr) | (set)))
#define adcc_clr_bit(addr, clr) adcc_write(addr, (adcc_read(addr) & ~(clr)))

/************************************************
 * Need maintain for each project
 ************************************************/
/* Core ID control by project */
#define NR_ADCC_CPU			4
#define ADCC_CPU_START_ID	4
#define ADCC_CPU_END_ID		7

/************************************************
 * Register addr, offset, bits range
 ************************************************/
/* BRISKET2 base_addr */
#define ADCC_CPU_CONFIG_REG		0x0C530000
#define ADCC_CPU4_CONFIG_REG	0x0C532000
#define ADCC_CPU5_CONFIG_REG	0x0C532800
#define ADCC_CPU6_CONFIG_REG	0x0C533000
#define ADCC_CPU7_CONFIG_REG	0x0C533800

/* ADDC control register range */
#define ADCC_OFFSET_V101	0x400
#define ADCC_OFFSET_V109	0x420
#define ADCC_OFFSET_V110	0x424
#define ADCC_OFFSET_V112	0x42C
#define ADCC_OFFSET_V114	0x434
#define ADCC_OFFSET_V120	0x44C
#define ADCC_OFFSET_DCD		0x21c

#define ADCC_EFUSE_FLAG		0x11C10B30
#define ADCC_EFUSE_DATA		0x11C10B3C
#define ADCC_SYSRAM_BASE	0x0010E000

#define SET_OFFSET 0x0
#define PLL_OFFSET 0x4
#define FLL_OFFSET 0x8
#define DBG_OFFSET 0xC

#define ADCC_CPU0_SET_STORE	(ADCC_SYSRAM_BASE + 0x00)
#define ADCC_CPU0_PLL_STORE	(ADCC_SYSRAM_BASE + 0x04)
#define ADCC_CPU0_FLL_STORE	(ADCC_SYSRAM_BASE + 0x08)
#define ADCC_CPU1_SET_STORE	(ADCC_SYSRAM_BASE + 0x10)
#define ADCC_CPU1_PLL_STORE	(ADCC_SYSRAM_BASE + 0x14)
#define ADCC_CPU1_FLL_STORE	(ADCC_SYSRAM_BASE + 0x18)
#define ADCC_CPU2_SET_STORE	(ADCC_SYSRAM_BASE + 0x20)
#define ADCC_CPU2_PLL_STORE	(ADCC_SYSRAM_BASE + 0x24)
#define ADCC_CPU2_FLL_STORE	(ADCC_SYSRAM_BASE + 0x28)
#define ADCC_CPU3_SET_STORE	(ADCC_SYSRAM_BASE + 0x30)
#define ADCC_CPU3_PLL_STORE	(ADCC_SYSRAM_BASE + 0x34)
#define ADCC_CPU3_FLL_STORE	(ADCC_SYSRAM_BASE + 0x38)
#define ADCC_CPU4_SET_STORE	(ADCC_SYSRAM_BASE + 0x40)
#define ADCC_CPU4_PLL_STORE	(ADCC_SYSRAM_BASE + 0x44)
#define ADCC_CPU4_FLL_STORE	(ADCC_SYSRAM_BASE + 0x48)
#define ADCC_CPU5_SET_STORE	(ADCC_SYSRAM_BASE + 0x50)
#define ADCC_CPU5_PLL_STORE	(ADCC_SYSRAM_BASE + 0x54)
#define ADCC_CPU5_FLL_STORE	(ADCC_SYSRAM_BASE + 0x58)
#define ADCC_CPU6_SET_STORE	(ADCC_SYSRAM_BASE + 0x60)
#define ADCC_CPU6_PLL_STORE	(ADCC_SYSRAM_BASE + 0x64)
#define ADCC_CPU6_FLL_STORE	(ADCC_SYSRAM_BASE + 0x68)
#define ADCC_CPU7_SET_STORE	(ADCC_SYSRAM_BASE + 0x70)
#define ADCC_CPU7_PLL_STORE	(ADCC_SYSRAM_BASE + 0x74)
#define ADCC_CPU7_FLL_STORE	(ADCC_SYSRAM_BASE + 0x78)


/* PLL/FLL STORE */
#define ADCC_Ack_shift 16             /* 16:16 */
#define ADCC_Ack_bits 16
#define ADCC_Duty_shift 6             /* 15:6 */
#define ADCC_Duty_bits 10
#define ADCC_CalDone_shift 5          /* 5:5 */
#define ADCC_CalDone_bits 1
#define ADCC_CalOut_shift 0           /* 4:0 */
#define ADCC_CalOut_bits 5

/* SET STORE */
#define ADCC_cfg_shift 17
#define ADCC_cfg_bits 4
#define ADCC_SW_nFlag_shift 16         /* 16:16 */
#define ADCC_SW_nFlag_bits 1
#define ADCC_DcdSelect_shift 8        /* 15:8 */
#define ADCC_DcdSelect_bits 8
#define ADCC_DcTarget_shift 0         /* 7:0 */
#define ADCC_DcTarget_bits 8


/* DRFC_V101_REG */
#define SafeFreqReqOverride_shift 0   /* 0:0 */
#define SafeFreqReqOverride_bits 1

/* DRFC_V109_REG */
#define DccControlCalDone_shift 24    /* 24:24 */
#define DccControlCalDone_bits 1
#define DccControlStatus_shift 23     /* 23:23 */
#define DccControlStatus_bits 1
#define DccControlCalout_shift 18     /* 22:18 */
#define DccControlCalout_bits 5
#define DccControlDcdSelect_shift 17  /* 17:17 */
#define DccControlDcdSelect_bits 1
#define DccControlDcTarget_shift 9    /* 16:9 */
#define DccControlDcTarget_bits 8
#define DccControlCalRate_shift 8     /* 8:8 */
#define DccControlCalRate_bits 1
#define DccControlForce_shift 7       /* 7:7 */
#define DccControlForce_bits 1
#define DccControlDetEnable_shift 6   /* 6:6 */
#define DccControlDetEnable_bits 1
#define DccControlCalibrate_shift 5   /* 5:5 */
#define DccControlCalibrate_bits 1
#define DccClkShaperCalin_shift 0     /* 4:0 */
#define DccClkShaperCalin_bits 5

/* DRFC_V110_REG */
#define DccStatusIntegrator_shift 6        /* 15:6 */
#define DccStatusIntegrator_bits 10
#define DccStatusCalClkEnableAck_shift 5   /* 5:5 */
#define DccStatusCalClkEnableAck_bits 1
#define DccStatusLoopCounter_shift 0       /* 4:0 */
#define DccStatusLoopCounter_bits 5

/* DRFC_V112_REG */
#define FllInFreqOverrideVal_shift 0       /* 7:0 */
#define FllInFreqOverrideVal_bits 8

/* DRFC_V114_REG */
#define FllDccEn_shift 10                  /* 10:10 */
#define FllDccEn_bits 1
#define FllDccClkShaperCalin_shift 5       /* 9:5 */
#define FllDccClkShaperCalin_bits 5

/* DRFC_V120_REG */
#define FllFreqLockDet_shift 1             /* 1:1 */
#define FllFreqLockDet_bits 1

/* DCD_CON_REG */
#define dcd2drfc_calend_shift 27           /* 27:27 */
#define dcd2drfc_calend_bits 1
#define dcd2drfc_calsign_shift 26          /* 26:26 */
#define dcd2drfc_calsign_bits 1
#define integrator_shift 16                /* 25:16 */
#define integrator_bits 10
#define dctarget_shift 8                   /* 15:8 */
#define dctarget_bits 8
#define dcd_select_shift 4                 /* 7:4 */
#define dcd_select_bits 4
#define calibrate_shift 1                  /* 1:1 */
#define calibrate_bits 1
#define detenable_shift 0                  /* 0:0 */
#define detenable_bits 1

/************************************************
 * config & struct
 ************************************************/

struct adcc_class {
	/* PLL, FLL, SET STORE */
		union {
			struct {
				unsigned int DcTarget:8;        /* [7:0] */
				unsigned int DcdSelect:8;       /* [15:8] */
				unsigned int SW_nFlag:1;        /* [16:16] */
				unsigned int adcc_cfg:4;        /* [20:17] */
				unsigned int SET_checksum:11;   /* [31:21] */
			};
			unsigned int set;
		};

		union {
			struct {
				unsigned int PLL_CalOut:5;      /* [4:0] */
				unsigned int PLL_CalDone:1;     /* [5:5] */
				unsigned int PLL_integrator:10; /* [15:6] */
				unsigned int PLL_Ack:1;         /* [16:16] */
				unsigned int PLL_checksum:15;   /* [31:17] */
			};
			unsigned int PLL;
		};

		union {
			struct {
				unsigned int FLL_CalOut:5;      /* [4:0] */
				unsigned int FLL_CalDone:1;     /* [5:5] */
				unsigned int FLL_integrator:10; /* [15:6] */
				unsigned int FLL_Ack:1;         /* [16:16] */
				unsigned int FLL_checksum:15;   /* [31:17] */
			};
			unsigned int FLL;
		};
};

enum ADCC_INDEX {
	ADCC_ENABLE,
	ADCC_SET_SHAPER,
	ADCC_SET_CALIN,
	ADCC_SET_DCDSELECT,
	ADCC_SET_DCTARGET,
	ADCC_DUMP_INFO,

	NR_ADCC_INDEX
};

enum ADCC_CONFIG {
	ADCC_DEFAULT,
	ADCC_PLL,
	ADCC_FLL,
	ADCC_FLL_AUTO,
	ADCC_PLL_AUTO,
	ADCC_FLL_15,
	ADCC_FLL_16,
	ADCC_PLL_15,
	ADCC_PLL_16,
	ADCC_FLL_DEFAULT,

	NR_ADCC_CONFIG
};

extern unsigned int adcc_init(unsigned int core);
extern unsigned int adcc_en(unsigned int core, unsigned int enable);
extern unsigned int adcc_control(unsigned int attr, unsigned int core, unsigned int value);
extern unsigned int adcc_pre_init(void);
extern unsigned int adcc_cfg(unsigned int set, unsigned int value);
extern void adcc_dump(void);
extern void adcc_dump_reg(void);

extern int adcc_set_Shaper(unsigned int core, unsigned int Shaper);
extern int adcc_set_Calin(unsigned int core, unsigned int Calin);
extern int adcc_set_DcdSelect(unsigned int core, unsigned int DcdSelect);
extern int adcc_set_DcTarget(unsigned int core, unsigned int DcTarget);
extern unsigned int adcc_core_reorder(unsigned int core);

#endif /* end of __MTK_ADCC_H__ */

