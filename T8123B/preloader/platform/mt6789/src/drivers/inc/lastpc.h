#ifndef __LASTPC_H__
#define __LASTPC_H__

void lastpc_init(void);

#define CPC_FLOW_CTRL_CFG  0x0C53A814
#define RESET_ON_KEEP_EN   (1 << 17)

#define CPU0_DBG_SEL       0x0C530220
#define CPU0_DBG_MON       0x0C530224

#define SEL_TYPE_PC_L      0xC
#define SEL_TYPE_PC_H      0xD

#define DFD_STATUS_RETURN  0x0C53E0A4
#define DFD_START          (1 << 0)
#define DFD_FINISH         (1 << 1)

#endif
