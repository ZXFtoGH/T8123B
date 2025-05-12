#ifndef _MT_DELSEL_H_
#define _MT_DELSEL_H_

#include "platform.h"

#define H300_2600000KHz    2600000
#define H300_2000000KHz    2000000
#define H300_800000KHz      800000
#define DELSEL_EMA0_6bits    0x8
#define DELSEL_EMA0_12bits   0x208
#define DELSEL_EMA0_7bits    0x0
#define DELSEL_EMA1_6bits    0x9
#define DELSEL_EMA1_12bits   0x209
#define DELSEL_EMA1_7bits    0x11
#define DELSEL_EMA2_6bits    0xA
#define DELSEL_EMA2_12bits   0x20A
#define DELSEL_EMA2_7bits    0x22

#define H240_2300000KHz    2300000
#define H240_1700000KHz    1700000
#define H240_730000KHz      730000
#define DELSEL_SEL0_6bits    0x0
#define DELSEL_SEL0_12bits   0xB24
#define DELSEL_SEL2_6bits    0x12
#define DELSEL_SEL2_12bits   0xB24
#define DELSEL_SEL4_6bits    0x24
#define DELSEL_SEL4_12bits   0xB24

#define CPU_TYPE2_RAM_DELSEL0_CFG    (MCUCFG_BASE + 0xCE20) /* 1AB */
#define CPU_TYPE2_RAM_DELSEL1_CFG    (MCUCFG_BASE + 0xCE24) /* 1AB */
#define CPU_TYPE2_RAM_DELSEL2_CFG    (MCUCFG_BASE + 0xCE28) /* 1AB */
#define CPU_TYPE2_RAM_DELSEL3_CFG    (MCUCFG_BASE + 0xCE2C) /* 1AB */
#define CPU_TYPE2_RAM_DELSEL4_CFG    (MCUCFG_BASE + 0xCE30) /* 1AB */
#define CPU_TYPE2_RAM_DELSEL5_CFG    (MCUCFG_BASE + 0xCE34) /* 1AB */
#define CPU_TYPE2_RAM_DELSEL6_CFG    (MCUCFG_BASE + 0xCE38) /* 1AB */
#define CPU_TYPE2_RAM_DELSEL7_CFG    (MCUCFG_BASE + 0xCE3C) /* 1AB */
#define CPU_TYPE2_RAM_DELSEL8_CFG    (MCUCFG_BASE + 0xCE40) /* 1AB */
#define CPU_TYPE2_RAM_DELSEL9_CFG    (MCUCFG_BASE + 0xCE44) /* 1AB */

#define CPU_TYPE1_RAM_DELSEL0_CFG    (MCUCFG_BASE + 0xCD20) /* 3BL */
#define CPU_TYPE1_RAM_DELSEL1_CFG    (MCUCFG_BASE + 0xCD24) /* 3BL */
#define CPU_TYPE1_RAM_DELSEL2_CFG    (MCUCFG_BASE + 0xCD28) /* 3BL */
#define CPU_TYPE1_RAM_DELSEL3_CFG    (MCUCFG_BASE + 0xCD2C) /* 3BL */
#define CPU_TYPE1_RAM_DELSEL4_CFG    (MCUCFG_BASE + 0xCD30) /* 3BL */
#define CPU_TYPE1_RAM_DELSEL5_CFG    (MCUCFG_BASE + 0xCD34) /* 3BL */
#define CPU_TYPE1_RAM_DELSEL6_CFG    (MCUCFG_BASE + 0xCD38) /* 3BL */
#define CPU_TYPE1_RAM_DELSEL7_CFG    (MCUCFG_BASE + 0xCD3C) /* 3BL */
#define CPU_TYPE1_RAM_DELSEL8_CFG    (MCUCFG_BASE + 0xCD40) /* 3BL */
#define CPU_TYPE1_RAM_DELSEL9_CFG    (MCUCFG_BASE + 0xCD44) /* 3BL */

#define MT_CPU_DVFS_L	1
#define MT_CPU_DVFS_B	2
extern void cache_delsel_init(unsigned int cluster_id, unsigned int target_khz);

#endif
