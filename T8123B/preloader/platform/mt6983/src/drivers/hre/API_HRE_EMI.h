#ifndef _API_HRE_EMI_H_
#define _API_HRE_EMI_H_

#define INIT_HRE_BK_CONFIG                      1
#define INIT_HRE_RS_CONFIG                      2
#define SAVE_HRE_BK_CONFIG                      3
#define SAVE_HRE_RS_CONFIG                      4

#define NORTH_EMI_REG_HRE_ADDR_BASE 0x10344000
#define NORTH_EMI_REG_HRE_ADDR_MIN 0x0
void NORTH_EMI_REG_HRE_BKRS(unsigned int select);

#define NORTH_EMI_MPU_REG_HRE_ADDR_BASE 0x10346000
#define NORTH_EMI_MPU_REG_HRE_ADDR_MIN 0x0
void NORTH_EMI_MPU_REG_HRE_BKRS(unsigned int select);

#define NORTH_SLC_REG_HRE_ADDR_BASE 0x10348000
#define NORTH_SLC_REG_HRE_ADDR_MIN 0x0
void NORTH_SLC_REG_HRE_BKRS(unsigned int select);

#define SOUTH_EMI_REG_HRE_ADDR_BASE 0x10345000
#define SOUTH_EMI_REG_HRE_ADDR_MIN 0x0
void SOUTH_EMI_REG_HRE_BKRS(unsigned int select);

#define SOUTH_EMI_MPU_REG_HRE_ADDR_BASE 0x10347000
#define SOUTH_EMI_MPU_REG_HRE_ADDR_MIN 0x0
void SOUTH_EMI_MPU_REG_HRE_BKRS(unsigned int select);

#define SOUTH_SLC_REG_HRE_ADDR_BASE 0x10349000
#define SOUTH_SLC_REG_HRE_ADDR_MIN 0x0
void SOUTH_SLC_REG_HRE_BKRS(unsigned int select);

#endif
