#ifndef __MTK_BRISKET2_H__
#define __MTK_BRISKET2_H__

/**
 * @file    mtk_brisket2.h
 * @brief   Driver header for Credit-based di/dt
 *
 */
#include "mtk_ptp3_common.h"

/************************************************
 * BIT Operation and REG r/w
 ************************************************/
#undef  BIT
#define BIT(_bit_)                    (unsigned int)(1 << (_bit_))
#define BITS(_bits_, _val_)           ((((unsigned int) -1 >> (31 - ((1) ? _bits_))) \
& ~((1U << ((0) ? _bits_)) - 1)) & ((_val_)<<((0) ? _bits_)))
#define BITMASK(_bits_)               (((unsigned int) -1 >> (31 - ((1) ? _bits_))) & ~((1U << ((0) ? _bits_)) - 1))
#define GET_BITS_VAL(_bits_, _val_)   (((_val_) & (BITMASK(_bits_))) >> ((0) ? _bits_))

#define brisket2_shift(val, bits, shift) \
	((val & (((unsigned int)0x1 << bits) - 1)) << shift)

#ifdef PetrusP_ready
#ifdef FT_SOC_SIM /* For FT only, prohibited to use in ATF */
#define brisket2_read(addr)			(*(volatile unsigned int  *)(addr))
#define brisket2_write(addr,  val)	((*(volatile unsigned int *)(addr)) = (unsigned int)val)
#else
#ifdef SLT1_DRV /* For SLT1 only, prohibited to use in ATF */
#define brisket2_read(addr)			(*(volatile unsigned int  *)(addr))
#define brisket2_write(addr,  val)	((*(volatile unsigned int *)(addr)) = (unsigned int)val)
#else
#define brisket2_read(addr)			(*(volatile unsigned int  *)(addr))
#define brisket2_write(addr, val)	((*(volatile unsigned int *)(addr)) = (unsigned int)val)
#endif /*end of SLT1_DRV */
#endif /*end of FT_SOC_SIM */

#define brisket2_write_bits(addr, bits, shift, val) \
	brisket2_write(addr, ((brisket2_read(addr) & ~(brisket2_shift((unsigned int)0xffffffff, \
	bits, shift))) | brisket2_shift(val, bits, shift)))

#define brisket2_read_bits(addr, bits, shift) \
	((brisket2_read(addr) >> shift) & (((unsigned int)0x1 << bits) - 1))
#else
#define brisket2_reg_access(fmt, args...)	INFO("[%s():%d]" fmt, __func__, __LINE__, ##args)
#define brisket2_write(addr,  val) \
	brisket2_reg_access("[REG_W] addr(0x%08x) val(%d)\n", addr, val)
#define brisket2_read(addr) \
	brisket2_reg_access("[REG_R] addr(0x%08x)\n", addr)
#define brisket2_write_bits(addr, bits, shift, val) \
	brisket2_reg_access("[REG_W_bits] addr(0x%08x) bits(%d) shift(%d) val(%d)\n", addr, bits, shift, val)
#define brisket2_read_bits(addr, bits, shift) \
	brisket2_reg_access("[REG_R_bits] addr(0x%08x) bits(%d) shift(%d)\n", addr, bits, shift)
#endif

#define brisket2_set_bit(addr, set) brisket2_write(addr, (brisket2_read(addr) | (set)))
#define brisket2_clr_bit(addr, clr) brisket2_write(addr, (brisket2_read(addr) & ~(clr)))

/************************************************
 * Need maintain for each project
 ************************************************/
/* Core ID control by project */
#define NR_BRISKET2_CPU 4
#define BRISKET2_CPU_START_ID 4
#define BRISKET2_CPU_END_ID 7

/************************************************
 * Register addr, offset, bits range
 ************************************************/
/* BRISKET2 base_addr */
#define BRISKET2_CPU4_CONFIG_REG 0x0C532000
#define BRISKET2_CPU5_CONFIG_REG 0x0C532800
#define BRISKET2_CPU6_CONFIG_REG 0x0C533000
#define BRISKET2_CPU7_CONFIG_REG 0x0C533800

/* MCUSYS ARMPLLDIV offset */
#define MCUSYS_OFFSET_CPU_PLLDIV_CFG1 0xA2A4
#define MCUSYS_OFFSET_CPU_PLLDIV_CFG2 0xA2A8
#define MCUSYS_OFFSET_CPU_PLLDIV_CFG3 0xA2AC
#define MCUSYS_OFFSET_CPU_PLLDIV_CFG4 0xA2B0

/* BRISKET2 control register offset */
#define BRISKET2_OFFSET_V101 0x400
#define BRISKET2_OFFSET_V102 0x404
#define BRISKET2_OFFSET_V103 0x408
#define BRISKET2_OFFSET_V104 0x40C
#define BRISKET2_OFFSET_V105 0x410
#define BRISKET2_OFFSET_V106 0x414
#define BRISKET2_OFFSET_V107 0x418
#define BRISKET2_OFFSET_V108 0x41C
#define BRISKET2_OFFSET_V109 0x420
#define BRISKET2_OFFSET_V110 0x424
#define BRISKET2_OFFSET_V111 0x428
#define BRISKET2_OFFSET_V112 0x42C
#define BRISKET2_OFFSET_V113 0x430
#define BRISKET2_OFFSET_V114 0x434
#define BRISKET2_OFFSET_V115 0x438
#define BRISKET2_OFFSET_V116 0x43C
#define BRISKET2_OFFSET_V117 0x440
#define BRISKET2_OFFSET_V118 0x444
#define BRISKET2_OFFSET_V119 0x448
#define BRISKET2_OFFSET_V120 0x44C
#define BRISKET2_OFFSET_V121 0x450
#define BRISKET2_OFFSET_V122 0x454
#define BRISKET2_OFFSET_V123 0x458
#define BRISKET2_OFFSET_V124 0x45C
#define BRISKET2_OFFSET_V125 0x460
#define BRISKET2_OFFSET_V126 0x464
#define BRISKET2_OFFSET_V127 0x468
#define BRISKET2_OFFSET_V128 0x46C
#define BRISKET2_OFFSET_V129 0x470
#define BRISKET2_OFFSET_V130 0x474
#define BRISKET2_OFFSET_V131 0x478
#define BRISKET2_OFFSET_V132 0x47C
#define BRISKET2_OFFSET_V133 0x480
#define BRISKET2_OFFSET_V134 0x484
#define BRISKET2_OFFSET_V135 0x488
#define BRISKET2_OFFSET_V136 0x48C
#define BRISKET2_OFFSET_V137 0x490
#define BRISKET2_OFFSET_V138 0x494
#define BRISKET2_OFFSET_V139 0x498
#define BRISKET2_OFFSET_V140 0x49C
#define BRISKET2_OFFSET_V141 0x5A0
#define BRISKET2_OFFSET_V142 0x5A4

/* MCUSYS_BITS */
#define MCUSYS_BITS_CPU_PLLDIV_SafeFreqEn_1 1
#define MCUSYS_BITS_CPU_PLLDIV_SafeFreqEn_2 1
#define MCUSYS_BITS_CPU_PLLDIV_SafeFreqEn_3 1
#define MCUSYS_BITS_CPU_PLLDIV_SafeFreqEn_4 1
#define MCUSYS_BITS_CPU_PLLDIV_SafeFreqBypass_1 1
#define MCUSYS_BITS_CPU_PLLDIV_SafeFreqBypass_2 1
#define MCUSYS_BITS_CPU_PLLDIV_SafeFreqBypass_3 1
#define MCUSYS_BITS_CPU_PLLDIV_SafeFreqBypass_4 1

/* BRISKET2_BITS */
#define BRISKET2_BITS_SamplerEn 1
#define BRISKET2_BITS_DrccGateEn 1
#define BRISKET2_BITS_ConfigComplete 1
#define BRISKET2_BITS_FllEn 1
#define BRISKET2_BITS_FllClkOutSelect 1
#define BRISKET2_BITS_FllSlowReqEn 1
#define BRISKET2_BITS_FllSlowReqGateEn 1
#define BRISKET2_BITS_CttEn 1
#define BRISKET2_BITS_TestMode 4
#define BRISKET2_BITS_GlobalEventEn 1
#define BRISKET2_BITS_SafeFreqReqOverride 1

/* MCUSYS_SHIFT */
#define MCUSYS_SHIFT_CPU_PLLDIV_SafeFreqEn_1 12
#define MCUSYS_SHIFT_CPU_PLLDIV_SafeFreqEn_2 12
#define MCUSYS_SHIFT_CPU_PLLDIV_SafeFreqEn_3 12
#define MCUSYS_SHIFT_CPU_PLLDIV_SafeFreqEn_4 12
#define MCUSYS_SHIFT_CPU_PLLDIV_SafeFreqBypass_1 13
#define MCUSYS_SHIFT_CPU_PLLDIV_SafeFreqBypass_2 13
#define MCUSYS_SHIFT_CPU_PLLDIV_SafeFreqBypass_3 13
#define MCUSYS_SHIFT_CPU_PLLDIV_SafeFreqBypass_4 13

/* BRISKET2_SHIFT */
#define BRISKET2_SHIFT_SamplerEn 13
#define BRISKET2_SHIFT_DrccGateEn 12
#define BRISKET2_SHIFT_ConfigComplete 11
#define BRISKET2_SHIFT_FllEn 10
#define BRISKET2_SHIFT_FllClkOutSelect 9
#define BRISKET2_SHIFT_FllSlowReqEn 8
#define BRISKET2_SHIFT_FllSlowReqGateEn 7
#define BRISKET2_SHIFT_CttEn 6
#define BRISKET2_SHIFT_TestMode 2
#define BRISKET2_SHIFT_GlobalEventEn 1
#define BRISKET2_SHIFT_SafeFreqReqOverride 0

/* BRISKET2 Cfg metadata offset */
#define BRISKET2_CFG_OFFSET_VALUE 0
#define BRISKET2_CFG_OFFSET_OPTION 20
#define BRISKET2_CFG_OFFSET_CPU 28

/* BRISKET2 Cfg metadata bitmask */
#define BRISKET2_CFG_BITMASK_VALUE 0xFFFF
#define BRISKET2_CFG_BITMASK_OPTION 0xFF00000
#define BRISKET2_CFG_BITMASK_CPU 0xF0000000

/************************************************
 * config enum
 ************************************************/
enum BRISKET2_NODE {
	BRISKET2_NODE_LIST_READ,
	BRISKET2_NODE_LIST_WRITE,
	BRISKET2_NODE_RW_REG_READ,
	BRISKET2_NODE_RW_REG_WRITE,

	NR_BRISKET2_NODE
};

enum BRISKET2_RW_GROUP {
	BRISKET2_RW_GROUP_V101,

	NR_BRISKET2_RW_GROUP,
};

enum BRISKET2_LIST {
	BRISKET2_LIST_SamplerEn,
	BRISKET2_LIST_DrccGateEn,
	BRISKET2_LIST_ConfigComplete,
	BRISKET2_LIST_FllEn,
	BRISKET2_LIST_FllClkOutSelect,
	BRISKET2_LIST_FllSlowReqEn,
	BRISKET2_LIST_FllSlowReqGateEn,
	BRISKET2_LIST_CttEn,
	BRISKET2_LIST_TestMode,
	BRISKET2_LIST_GlobalEventEn,
	BRISKET2_LIST_SafeFreqReqOverride,
	BRISKET2_LIST_Cfg,
	BRISKET2_LIST_PollingEn,
	BRISKET2_LIST_SafeFreqEn,
	BRISKET2_LIST_SafeFreqBypass,

	NR_BRISKET2_LIST,
};

enum BRISKET2_PATH {
	BRISKET2_PATH_DISABLE,
	BRISKET2_PATH_FLL_ONLY,
	BRISKET2_PATH_DEFAULT_MP,
	BRISKET2_PATH_FLL_LEGACY_DRCC,
	BRISKET2_PATH_LEGACY_DRCC_ONLY,
	BRISKET2_PATH_FPC_PATCFG,

	NR_BRISKET2_PATH,
};


/************************************************
 * function prototype
 ************************************************/
unsigned int brisket2_get_SafeFreqEn(unsigned int core);
unsigned int brisket2_set_SafeFreqEn(unsigned int core, unsigned int value);
extern void brisket2_init(void);
/************************************/

#endif /* end of __MTK_BRISKET2_H__ */


