#ifndef _API_HRE_SMPU_H_
#define _API_HRE_SMPU_H_

#define INIT_HRE_BK_CONFIG                      1
#define INIT_HRE_RS_CONFIG                      2
#define SAVE_HRE_BK_CONFIG                      3
#define SAVE_HRE_RS_CONFIG                      4

#define NORTH_SMPU_HRE_ADDR_BASE 0x1034A000
#define NORTH_SMPU_HRE_ADDR_MIN 0x0
void NORTH_SMPU_HRE_BKRS(unsigned int select);

#define SOUTH_SMPU_HRE_ADDR_BASE 0x1034B000
#define SOUTH_SMPU_HRE_ADDR_MIN 0x0
void SOUTH_SMPU_HRE_BKRS(unsigned int select);

#endif
