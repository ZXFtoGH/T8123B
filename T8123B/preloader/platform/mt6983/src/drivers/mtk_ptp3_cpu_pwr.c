#include "mtk_ptp3_cpu_pwr.h"
#include "mtk_ptp3_adcc.h"

/************************************************
 * BIT Operation and REG r/w
 ************************************************/
inline void mmio_clrbits_32(unsigned int addr, unsigned int clear)
{
	adcc_write(addr, adcc_read(addr) & ~clear);
}

inline void mmio_setbits_32(unsigned int addr, unsigned int set)
{
	adcc_write(addr, adcc_read(addr) | set);
}

static inline void mmio_clrsetbits_32(unsigned int addr,
				unsigned int clear,
				unsigned int set)
{
	adcc_write(addr, (adcc_read(addr) & ~clear) | set);
}

/* CPC_FLOW_CTRL_CFG Definition */
#define RVBARADDR_ONKEEPON_SEL		(MT_RVBARADDR_BASE + 0x388)
#define CPC_FLOW_CTRL_CFG			(MT_CPC_CFG + 0x0114)
#define CPC_FLOW_CTRL_EN			(1 << 16)
#define MTK_SPMC_PWR_CTRL_EN		(1U << 13)

#ifdef MVP_SIM
#define MP0_SPMC_PWR_CTRL			(0x0C00FFE8)
#else
#define MP0_SPMC_PWR_ON_CPUx		(1U << 2)
#define MP0_SPMC_PWR_ON_ACK_CPUx	(1U << 31)
#endif

#define IS_VALID_CPUID(_cpuid)		((_cpuid >= 0 && _cpuid < 8))

static void core_bootup_address_set(int cpu,
		struct cpu_pwr_ctrl *pwr_ctrl,uintptr_t entry)
{

	/* Set bootup address */
	adcc_write(pwr_ctrl->rvbaraddr_l, entry);

	adcc_debug("[%s:%d] - core_%d RVBATADDR :0x%x\n",
			__func__, __LINE__, cpu,
			adcc_read(pwr_ctrl->rvbaraddr_l));
}

static void core_ppu_pwsr_set(int cpu, struct cpu_pwr_ctrl *pwr_ctrl,
			unsigned int mode, unsigned int policy)
{
	unsigned int val;

	val = adcc_read(pwr_ctrl->ppu_pwpr);
	val &= ~(PPU_PWPR_MASK | PPU_PWPR_DYNAMIC_MODE);

	if (mode & PPU_PWPR_DYNAMIC_MODE)
		val |= (PPU_PWPR_DYNAMIC_MODE |
		       (policy & PPU_PWPR_MASK));
	else
		val |= (policy & PPU_PWPR_MASK);

	adcc_write(pwr_ctrl->ppu_pwpr, val);

	adcc_debug("[%s:%d] - core_%d PWPR:0x%x, \n",
			__func__, __LINE__, cpu,
			adcc_read(pwr_ctrl->ppu_pwpr));

}

int adcc_cpupm_cpu_pwr_on_prepare(unsigned int cpu, uintptr_t entry)
{
	struct cpu_pwr_ctrl pwr_ctrl = {0, 0, 0, 0, 0};
	unsigned int _val;
	unsigned int cnt = 0 ;

	adcc_debug("[%s:%d] - core_%d_pwr_on_prepare\n", __func__, __LINE__, cpu);

	/* fix reboot issue after DA */
	if (adcc_read(RVBARADDR_ONKEEPON_SEL) == 0x1) {
		adcc_debug("1. ONKEEPON_SEL=%x, CPC_FLOW_CTRL_CFG=%x\n",
		adcc_read(RVBARADDR_ONKEEPON_SEL),
		adcc_read(CPC_FLOW_CTRL_CFG));
		adcc_write(RVBARADDR_ONKEEPON_SEL, 0x1);
		adcc_debug("2. ONKEEPON_SEL=%x, CPC_FLOW_CTRL_CFG=%x\n",
		adcc_read(RVBARADDR_ONKEEPON_SEL),
		adcc_read(CPC_FLOW_CTRL_CFG));
		if (adcc_read(RVBARADDR_ONKEEPON_SEL) == 0x1)
			return 1;
	}

	PER_CPU_PWR_CTRL(pwr_ctrl, cpu);
	core_bootup_address_set(cpu, &pwr_ctrl, entry);
	core_ppu_pwsr_set(cpu, &pwr_ctrl, PPU_PWPR_DYNAMIC_MODE, PPU_PWPR_OFF);

	_val = adcc_read(pwr_ctrl.ppu_pwsr) & PPU_PWSR_STATE_ON;

	if (_val) {
		adcc_msg("CPU_%d had been power on !!\n", cpu);
	} else {
		/* info CPC that CPU hotplug on */
		mmio_setbits_32(CPC_FLOW_CTRL_CFG, MTK_SPMC_PWR_CTRL_EN);
		adcc_debug("[cpu_%d] SPMC PWR_CTRL - enable: 0x%x\n", cpu, adcc_read(CPC_FLOW_CTRL_CFG));
		

		/* Set mp0_spmc_pwr_on_cpuX = 1 */
#ifdef MVP_SIM
		_val = (adcc_read(MP0_SPMC_PWR_CTRL) | (1u << cpu));
		adcc_write(MP0_SPMC_PWR_CTRL, _val);
		adcc_debug("CPC power: 0x%x\n", adcc_read(MP0_SPMC_PWR_CTRL));
#else
		mmio_setbits_32(pwr_ctrl.cpc_spmc, MP0_SPMC_PWR_ON_CPUx);
#endif
		/* wait for power on ack */
		while (!(adcc_read(pwr_ctrl.ppu_pwsr) & PPU_PWSR_STATE_ON)) {
			if (cnt++ > 2000000) {
				adcc_msg("cpu_pwr_on fail: CPU%d PPU_PWSR_STATE_ON = 0x%x\n",
				cpu, adcc_read(pwr_ctrl.ppu_pwsr));
				break;
			}
		}
		/* info CPC that CPU hotplug off */
		mmio_clrbits_32(CPC_FLOW_CTRL_CFG, MTK_SPMC_PWR_CTRL_EN);
		adcc_debug("[cpu_%d] SPMC PWR_CTRL - disable - 0x%x\n", cpu, adcc_read(CPC_FLOW_CTRL_CFG));
	}
	return 0;
}

void adcc_cpupm_cpu_pwr_dwn_prepare(unsigned int cpu)
{
#ifndef MVP_SIM
	unsigned int cnt = 0 ;
	struct cpu_pwr_ctrl pwr_ctrl = {0, 0, 0, 0, 0};

	PER_CPU_PWR_CTRL(pwr_ctrl, cpu);

	/* Set mp0_spmc_pwr_on_cpuX = 0 */
	mmio_clrbits_32(pwr_ctrl.cpc_spmc, MP0_SPMC_PWR_ON_CPUx);

	/* wait for power off ack */
	while ((adcc_read(pwr_ctrl.cpc_spmc) & MP0_SPMC_PWR_ON_ACK_CPUx)) {
		if (cnt++ > 2000000) {
			adcc_msg("cpu_pwr_dwn fail: SPM_CPU%d_PWR_CON = 0x%x\n", cpu, adcc_read(pwr_ctrl.cpc_spmc));
			break;
		}
	}
#endif
}

int adcc_cpu_pwr_drv_init(void)
{
	unsigned int val;

	adcc_debug(" CPC init\n");
	val = adcc_read(CPC_FLOW_CTRL_CFG);

	val |= CPC_FLOW_CTRL_EN;
	adcc_write(CPC_FLOW_CTRL_CFG, val);

	adcc_debug(" CPC CTRL FLOW: 0x%x\n", adcc_read(CPC_FLOW_CTRL_CFG));

	return 0;
}

int adcc_cpu_pwr_drv_restore(void)
{
	mmio_clrbits_32(CPC_FLOW_CTRL_CFG, CPC_FLOW_CTRL_EN);
	adcc_debug(" CPC CTRL FLOW: 0x%x\n", adcc_read(CPC_FLOW_CTRL_CFG));

	return 0;
}


