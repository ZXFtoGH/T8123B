#include "mtk_dcm_busdvt.h"
#include <typedefs.h>
#include <platform.h>
#include <mtk_dcm_busdvt.h>

void dcm_dump_regs(void)
{
    dcm_pr_info("\n******** dcm dump register ********\n");

    REG_DUMP(MCUSYS_TOP_MP_ADB_DCM_CFG0);
    REG_DUMP(MCUSYS_TOP_ADB_FIFO_DCM_EN);
    REG_DUMP(MCUSYS_TOP_MP0_DCM_CFG0);
    REG_DUMP(MCUSYS_TOP_QDCM_CONFIG0);
    REG_DUMP(MCUSYS_TOP_QDCM_CONFIG1);
    REG_DUMP(MCUSYS_TOP_QDCM_CONFIG2);
    REG_DUMP(MCUSYS_TOP_QDCM_CONFIG3);
    REG_DUMP(MCUSYS_TOP_L3GIC_ARCH_CG_CONFIG);
    REG_DUMP(MCUSYS_TOP_CBIP_CABGEN_3TO1_CONFIG);
    REG_DUMP(MCUSYS_TOP_CBIP_CABGEN_2TO1_CONFIG);
    REG_DUMP(MCUSYS_TOP_CBIP_CABGEN_4TO2_CONFIG);
    REG_DUMP(MCUSYS_TOP_CBIP_CABGEN_1TO2_CONFIG);
    REG_DUMP(MCUSYS_TOP_CBIP_CABGEN_2TO5_CONFIG);
    REG_DUMP(MCUSYS_TOP_CBIP_P2P_CONFIG0);
    REG_DUMP(MCUSYS_CPC_CPC_DCM_Enable);
    REG_DUMP(MP_CPU4_TOP_PTP3_CPU_PCSM_SW_PCHANNEL);
    REG_DUMP(MP_CPU4_TOP_STALL_DCM_CONF);
    REG_DUMP(MP_CPU5_TOP_PTP3_CPU_PCSM_SW_PCHANNEL);
    REG_DUMP(MP_CPU5_TOP_STALL_DCM_CONF);
    REG_DUMP(MP_CPU6_TOP_PTP3_CPU_PCSM_SW_PCHANNEL);
    REG_DUMP(MP_CPU6_TOP_STALL_DCM_CONF);
    REG_DUMP(MP_CPU7_TOP_PTP3_CPU_PCSM_SW_PCHANNEL);
    REG_DUMP(MP_CPU7_TOP_STALL_DCM_CONF);
    REG_DUMP(INFRA_BUS_DCM_CTRL);
    REG_DUMP(P2P_RX_CLK_ON);
    REG_DUMP(INFRA_AXIMEM_IDLE_BIT_EN_0);
    REG_DUMP(VDNR_DCM_TOP_INFRA_PAR_BUS_u_INFRA_PAR_BUS_CTRL_0);
    REG_DUMP(VDNR_DCM_TOP_INFRA_PAR_BUS_u_INFRA_PAR_BUS_CTRL_1);
    REG_DUMP(VDNR_DCM_TOP_INFRA_PAR_BUS_u_INFRA_PAR_BUS_CTRL_2);
    REG_DUMP(VDNR_DCM_TOP_PERI_PAR_BUS_u_PERI_PAR_BUS_CTRL_0);
    REG_DUMP(VDNR_DCM_TOP_PERI_PAR_BUS_u_PERI_PAR_BUS_CTRL_1);
    REG_DUMP(VDNR_DCM_TOP_VLP_PAR_BUS_u_VLP_PAR_BUS_CTRL_0);
}

static int dcm_infra(int on)
{
    dcm_infracfg_ao_aximem_bus_dcm(on);
    dcm_infracfg_ao_infra_bus_dcm(on);
    dcm_infracfg_ao_infra_rx_p2p_dcm(on);
    dcm_infra_ao_bcrm_infra_bus_dcm(on);
    dcm_infra_ao_bcrm_infra_bus_fmem_sub_dcm(on);
    return 0;
}

static int dcm_peri(int on)
{
    dcm_peri_ao_bcrm_peri_bus_dcm(on);
    return 0;
}

static int dcm_busdvt(int on)
{
    dcm_pr_info("\n******** before busdvt dcm enable ********\n");

    dcm_infracfg_ao_aximem_bus_dcm(on);
    dcm_infracfg_ao_infra_bus_dcm(on);
    dcm_infra_ao_bcrm_infra_bus_dcm(on);
    dcm_infra_ao_bcrm_infra_bus_fmem_sub_dcm(on);

    dcm_pr_info("\n******** after busdvt dcm enable ********\n");
    dcm_dump_regs();
    return 0;
}

int mtk_dcm_init(void)
{
    int ret = 0;
#if !defined(DCM_BRINGUP)
    ret = dcm_infra(1);
    ret |= dcm_peri(1);

#if defined(MTK_DCM_BUSDVT)
    ret |= dcm_busdvt(1);
#endif /* defined(MTK_DCM_BUSDVT) */
#endif /* !defined(DCM_BRINGUP) */
    return ret;
}
