#ifndef _MTK_SES_H_
#define _MTK_SES_H_

#define SES_EN	0
#define DSU_EN	0
#define SES_NUM	9

#define ses_print					print
#define ses_read(addr)				DRV_Reg32(addr)
#define ses_write(addr,  val)		DRV_WriteReg32(addr,  val)

#define ses_shift(val, bits, shift) \
	((val & (((unsigned int)0x1 << bits) - 1)) << shift)

#define ses_write_bits(addr, bits, shift, val) \
	ses_write(addr, ((ses_read(addr) & ~(ses_shift((unsigned int)0xffffffff, \
	bits, shift))) | ses_shift(val, bits, shift)))

#define ses_read_bits(addr, bits, shift) \
	((ses_read(addr) >> shift) & (((unsigned int)0x1 << bits) - 1))

#define AMUX_OUT	0x11F30040

/* eFuse */
#define CPU0_EFUSE	(0x11C10B30)
#define CPU1_EFUSE	(0x11C10B34)
#define CPU2_EFUSE	(0x11C10B38)
#define CPU3_EFUSE	(0x11C10B3C)
#define CPU4_EFUSE	(0x11C10B40)
#define CPU5_EFUSE	(0x11C10B44)
#define CPU6_EFUSE	(0x11C10B48)
#define CPU7_EFUSE	(0x11C10B4C)
#define DSU_EFUSE	(0x11C10B50)
#define SESBG_EFUSE	(0x11C10B54)

/* SES control register */
#define SES_REG_BASE_ADDR	(0x0C530000)

#define SESV6_BG_CTRL		(SES_REG_BASE_ADDR + 0xAC40)
#define DSU_SESV6_AO_REG0	(SES_REG_BASE_ADDR + 0xAC50)
#define DSU_SESV6_AO_REG1	(SES_REG_BASE_ADDR + 0xAC54)
#define DSU_SESV6_AO_REG2	(SES_REG_BASE_ADDR + 0xAC58)
#define DSU_SESV6_AO_REG3	(SES_REG_BASE_ADDR + 0xAC5C)
#define DSU_SESV6_AO_REG4	(SES_REG_BASE_ADDR + 0xAC60)
#define CPU0_SESV6_AO_REG0	(SES_REG_BASE_ADDR + 0xB010)
#define CPU0_SESV6_AO_REG1	(SES_REG_BASE_ADDR + 0xB014)
#define CPU0_SESV6_AO_REG2	(SES_REG_BASE_ADDR + 0xB018)
#define CPU0_SESV6_AO_REG3	(SES_REG_BASE_ADDR + 0xB01C)
#define CPU0_SESV6_AO_REG4	(SES_REG_BASE_ADDR + 0xB020)
#define CPU1_SESV6_AO_REG0	(SES_REG_BASE_ADDR + 0xB210)
#define CPU1_SESV6_AO_REG1	(SES_REG_BASE_ADDR + 0xB214)
#define CPU1_SESV6_AO_REG2	(SES_REG_BASE_ADDR + 0xB218)
#define CPU1_SESV6_AO_REG3	(SES_REG_BASE_ADDR + 0xB21C)
#define CPU1_SESV6_AO_REG4	(SES_REG_BASE_ADDR + 0xB220)
#define CPU2_SESV6_AO_REG0	(SES_REG_BASE_ADDR + 0xB410)
#define CPU2_SESV6_AO_REG1	(SES_REG_BASE_ADDR + 0xB414)
#define CPU2_SESV6_AO_REG2	(SES_REG_BASE_ADDR + 0xB418)
#define CPU2_SESV6_AO_REG3	(SES_REG_BASE_ADDR + 0xB41C)
#define CPU2_SESV6_AO_REG4	(SES_REG_BASE_ADDR + 0xB420)
#define CPU3_SESV6_AO_REG0	(SES_REG_BASE_ADDR + 0xB610)
#define CPU3_SESV6_AO_REG1	(SES_REG_BASE_ADDR + 0xB614)
#define CPU3_SESV6_AO_REG2	(SES_REG_BASE_ADDR + 0xB618)
#define CPU3_SESV6_AO_REG3	(SES_REG_BASE_ADDR + 0xB61C)
#define CPU3_SESV6_AO_REG4	(SES_REG_BASE_ADDR + 0xB620)
#define CPU4_SESV6_AO_REG0	(SES_REG_BASE_ADDR + 0xB810)
#define CPU4_SESV6_AO_REG1	(SES_REG_BASE_ADDR + 0xB814)
#define CPU4_SESV6_AO_REG2	(SES_REG_BASE_ADDR + 0xB818)
#define CPU4_SESV6_AO_REG3	(SES_REG_BASE_ADDR + 0xB81C)
#define CPU4_SESV6_AO_REG4	(SES_REG_BASE_ADDR + 0xB820)
#define CPU5_SESV6_AO_REG0	(SES_REG_BASE_ADDR + 0xBA10)
#define CPU5_SESV6_AO_REG1	(SES_REG_BASE_ADDR + 0xBA14)
#define CPU5_SESV6_AO_REG2	(SES_REG_BASE_ADDR + 0xBA18)
#define CPU5_SESV6_AO_REG3	(SES_REG_BASE_ADDR + 0xBA1C)
#define CPU5_SESV6_AO_REG4	(SES_REG_BASE_ADDR + 0xBA20)
#define CPU6_SESV6_AO_REG0	(SES_REG_BASE_ADDR + 0xBC10)
#define CPU6_SESV6_AO_REG1	(SES_REG_BASE_ADDR + 0xBC14)
#define CPU6_SESV6_AO_REG2	(SES_REG_BASE_ADDR + 0xBC18)
#define CPU6_SESV6_AO_REG3	(SES_REG_BASE_ADDR + 0xBC1C)
#define CPU6_SESV6_AO_REG4	(SES_REG_BASE_ADDR + 0xBC20)
#define CPU7_SESV6_AO_REG0	(SES_REG_BASE_ADDR + 0xBE10)
#define CPU7_SESV6_AO_REG1	(SES_REG_BASE_ADDR + 0xBE14)
#define CPU7_SESV6_AO_REG2	(SES_REG_BASE_ADDR + 0xBE18)
#define CPU7_SESV6_AO_REG3	(SES_REG_BASE_ADDR + 0xBE1C)
#define CPU7_SESV6_AO_REG4	(SES_REG_BASE_ADDR + 0xBE20)

extern int ses_init(void);
extern int ses_enable(unsigned int onOff, unsigned int ses_node);
extern void ses_AMUXOUT(void);
#endif
