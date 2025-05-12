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

#define adcc_read(addr)			(*(volatile unsigned int *)(addr))
#define adcc_write(addr, val)	(*(volatile unsigned int *)(addr) = (unsigned long)(val))

#define adcc_write_bits(addr, bits, shift, val) \
	adcc_write(addr, ((adcc_read(addr) & ~(adcc_shift((unsigned int)0xffffffff, \
	bits, shift))) | adcc_shift(val, bits, shift)))

#define adcc_read_bits(addr, bits, shift) \
	((adcc_read(addr) >> shift) & (((unsigned int)0x1 << bits) - 1))

/************************************************
 * Need maintain for each project
 ************************************************/
/* Core ID control by project */
#define NR_ADCC_CPU			8
#define ADCC_CPU_START_ID	0
#define ADCC_CPU_END_ID		7

/* BRISKET2 base_addr */
#define ADCC_CPU_CONFIG_REG		0x0C530000

/* ADDC control register range */
#define ADCC_SYSRAM_BASE	0X00115000

#define GET_SET_ADDR(ii)	(ADCC_SYSRAM_BASE + ((ii) * 16))
#define GET_PLL_ADDR(ii)	(ADCC_SYSRAM_BASE + ((ii) * 16) + 0x4)
#define GET_FLL_ADDR(ii)	(ADCC_SYSRAM_BASE + ((ii) * 16) + 0x8)
#define GET_DBG_ADDR(ii)	(ADCC_SYSRAM_BASE + ((ii) * 16) + 0xc)

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

extern unsigned int adcc_pre_init(void);
#endif /* end of __MTK_ADCC_H__ */

