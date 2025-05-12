#ifndef _API_HRE_PERI_DEVAPC_H_
#define _API_HRE_PERI_DEVAPC_H_

#define INIT_HRE_BK_CONFIG                      1
#define INIT_HRE_RS_CONFIG                      2
#define SAVE_HRE_BK_CONFIG                      3
#define SAVE_HRE_RS_CONFIG                      4

#define PERI_DEVAPC_HRE_ADDR_BASE 0x1C3B0000
#define PERI_DEVAPC_HRE_ADDR_MIN 0x0
void PERI_DEVAPC_HRE_BKRS(unsigned int select);

#endif
