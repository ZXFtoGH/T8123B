#ifndef _MT_DELSEL_H_
#define _MT_DELSEL_H_

#include "platform.h"

#define BCPU_2540000KHz         2540000 // set1
#define BCPU_1940000KHz         1940000 // set4
#define BCPU_940000KHz          940000  // set5

#define MCPU_2240000KHz         2240000 // set1
#define MCPU_1650000KHz         1650000 // set4
#define MCPU_1040000KHz         1040000 // set5

#define SET1_DELSEL21_32bits    0x0090000
#define SET1_DELSEL43_32bits    0xB000009
#define SET1_DELSEL65_32bits    0x1B0009
#define SET2_DELSEL21_32bits    0x0090009
#define SET2_DELSEL43_32bits    0xB000009
#define SET2_DELSEL65_32bits    0x1B0009
#define SET3_DELSEL21_32bits    0x0090009
#define SET3_DELSEL43_32bits    0xB090009
#define SET3_DELSEL65_32bits    0x1B0009
#define SET4_DELSEL21_32bits    0x0120012
#define SET4_DELSEL43_32bits    0xB1B0012
#define SET4_DELSEL65_32bits    0x1B0012
#define SET5_DELSEL21_32bits    0x01B001B
#define SET5_DELSEL43_32bits    0xB1B001B
#define SET5_DELSEL65_32bits    0x1B001B
#define SET6_DELSEL21_32bits    0x01B001B
#define SET6_DELSEL43_32bits    0xBDB001B
#define SET6_DELSEL65_32bits    0x1B001B

#define MCUSYS_PAR_WRAP      (0x0C000000)
#define CORE_TYPE1_DELSEL_CONFIG0    (MCUSYS_PAR_WRAP + 0x01A8) /* MCPU */
//15	0	rg_core_type1_delsel_1
//31	16	rg_core_type1_delsel_2
#define CORE_TYPE1_DELSEL_CONFIG1    (MCUSYS_PAR_WRAP + 0x01AC) /* MCPU */
//15	0	rg_core_type1_delsel_3
//31	16	rg_core_type1_delsel_4
#define CORE_TYPE1_DELSEL_CONFIG2    (MCUSYS_PAR_WRAP + 0x01B0) /* MCPU */
//15	0	rg_core_type1_delsel_5
//31	16	rg_core_type1_delsel_6
#define CORE_TYPE2_DELSEL_CONFIG0    (MCUSYS_PAR_WRAP + 0x01B8) /* BCPU */
//15	0	rg_core_type1_delsel_1
//31	16	rg_core_type1_delsel_2
#define CORE_TYPE2_DELSEL_CONFIG1    (MCUSYS_PAR_WRAP + 0x01BC) /* BCPU */
//15	0	rg_core_type1_delsel_3
//31	16	rg_core_type1_delsel_4
#define CORE_TYPE2_DELSEL_CONFIG2    (MCUSYS_PAR_WRAP + 0x01C0) /* BCPU */
//15	0	rg_core_type1_delsel_5
//31	16	rg_core_type1_delsel_6

#define MT_CPU_DVFS_L	1 //MCPU
#define MT_CPU_DVFS_B	2 //BCPU
extern void cache_delsel_init(unsigned int cluster_id, unsigned int target_khz);

#endif
