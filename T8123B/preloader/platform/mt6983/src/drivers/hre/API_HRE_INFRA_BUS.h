#ifndef _API_HRE_INFRA_BUS_H_
#define _API_HRE_INFRA_BUS_H_

#define INIT_HRE_BK_CONFIG                      1
#define INIT_HRE_RS_CONFIG                      2
#define SAVE_HRE_BK_CONFIG                      3
#define SAVE_HRE_RS_CONFIG                      4

#define INFRA_BUS_HRE_ADDR_BASE 0x1032C000
#define INFRA_BUS_HRE_ADDR_MIN 0x0
void INFRA_BUS_HRE_BKRS(unsigned int select);

#endif
