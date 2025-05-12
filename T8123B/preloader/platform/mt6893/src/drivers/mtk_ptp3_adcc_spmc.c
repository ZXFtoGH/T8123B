/*
 * Copyright (c) [2021], MediaTek Inc. All rights reserved.
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws.
 * The information contained herein is confidential and proprietary to
 * MediaTek Inc. and/or its licensors.
 * Except as otherwise provided in the applicable licensing terms with
 * MediaTek Inc. and/or its licensors, any reproduction, modification, use or
 * disclosure of MediaTek Software, and information contained herein, in whole
 * or in part, shall be strictly prohibited.
*/

#include <mtk_ptp3_adcc_spmc.h>
#include <mt6893.h>
#include <platform.h>
#include <typedefs.h>

#define adcc_read(addr)			(*(volatile unsigned int *)(addr))
#define adcc_write(addr, val)	(*(volatile unsigned int *)(addr) = (val))

#define assert ASSERT
#define ERROR HOTPLUG_INFO

extern int aarch64_adcc_start_slave(unsigned int);

#define ADCC_DVT 0

#if ADCC_DVT
extern int aarch64_adcc_udi_start_slave(unsigned int);
#endif

unsigned int adcc_slave_cpu_base = 0;

/* start m-ATE */
void boot_secondary_core17(void)
{
	unsigned int i = 0;

	for(i = 1; i < 8; i++) {
		boot_secondary(i);
	}
}
void boot_secondary_core13(void)
{
	boot_secondary(1);
	boot_secondary(2);
	boot_secondary(3);
}
void boot_secondary_core47(void)
{
	boot_secondary(4);
	boot_secondary(5);
	boot_secondary(6);
	boot_secondary(7);
}
/* end of mATE */

static inline void set_cpu_retention_control(void)
{
/* refer to prometheus trm */
#if 1
    /* WFI_RET_CTRL, [6:4] */
	asm volatile (
		"mrc p15, 0, r0, c15, c2, 7\n\t"
		"orr r0, r0, #0x10\n\t"
		"mcr p15, 0, r0, c15, c2, 7\n\t"
		:::"r0", "cc"
	);
	/* WFE_RET_CTRL, [9:7] */
	asm volatile (
		"mrc p15, 0, r0, c15, c2, 7\n\t"
		"orr r0, r0, #0x80\n\t"
		"mcr p15, 0, r0, c15, c2, 7\n\t"
		:::"r0", "cc"
	);
#endif

}

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

/* SPMC related registers */
#define SPM_POWERON_CONFIG_EN		(SPM_BASE + 0x000)
/* bit-fields of SPM_POWERON_CONFIG_EN */
#define PROJECT_CODE				0xB16
#define BCLK_CG_EN					(1 << 0)

/* === SPMC related registers */
#define SPM_MCUSYS_PWR_CON			(MCUCFG_BASE + 0xd200) /*(SPM_BASE + 0xd200) */
#define SPM_MP0_CPUTOP_PWR_CON		(MCUCFG_BASE + 0xd204) /*(SPM_BASE + 0xd204) */
#define SPM_MP0_CPU0_PWR_CON		(MCUCFG_BASE + 0xd208) /*(SPM_BASE + 0xd208) */
#define SPM_MP0_CPU1_PWR_CON		(MCUCFG_BASE + 0xd20c) /*(SPM_BASE + 0xd20c) */
#define SPM_MP0_CPU2_PWR_CON		(MCUCFG_BASE + 0xd210) /*(SPM_BASE + 0xd210) */
#define SPM_MP0_CPU3_PWR_CON		(MCUCFG_BASE + 0xd214) /*(SPM_BASE + 0xd214) */
#define SPM_MP0_CPU4_PWR_CON		(MCUCFG_BASE + 0xd218) /*(SPM_BASE + 0xd218) */
#define SPM_MP0_CPU5_PWR_CON		(MCUCFG_BASE + 0xd21c) /*(SPM_BASE + 0xd21c) */
#define SPM_MP0_CPU6_PWR_CON		(MCUCFG_BASE + 0xd220) /*(SPM_BASE + 0xd220) */
#define SPM_MP0_CPU7_PWR_CON		(MCUCFG_BASE + 0xd224) /*(SPM_BASE + 0xd224) */

/* bit fields of SPM_*_PWR_CON */
#define PWR_ON_ACK					(1 << 31)
#define VPROC_EXT_OFF				(1 << 7)
#define DORMANT_EN					(1 << 6)
#define RESETPWRON_CONFIG			(1 << 5)
#define PWR_CLK_DIS					(1 << 4)
#define PWR_ON						(1 << 2)
#define PWR_RST_B					(1 << 0)

#define MCUCFG_CPC_FLOW_CTRL_CFG	(MCUCFG_BASE + 0xA814)

/*CPC_FLOW_CTRL_CFG*/
#define CPU_PWR_ON_SEQ_DIS				(1<<1)
#define CPU_PWR_ON_PRIORITY				(1<<2)
#define CPC_AUTO_DORMANT_EN				(1<<5)
#define SSPM_CORE_PWR_ON_EN				(1<<7)
#define SSPM_CORE_PWR_OFF_EN			(1<<8)
#define SSPM_CPUSYS_PWR_ON_EN			(1<<9)
#define SSPM_CPUSYS_PWR_OFF_EN			(1<<10)
#define SSPM_MCUSYS_PWR_ON_EN			(1<<11)
#define SSPM_MCUSYS_PWR_OFF_EN			(1<<12)
#define SSPM_ALL_PWR_CTRL_EN			(1<<13)
#define DORMANT_WAIT_EN					(1<<14)
#define CPC_CTRL_ENABLE					(1<<16)
#define RESET_ON_KEEP_ON				(1<<17)
#define RESET_ALL_ON					(1<<18)
#define RESET_ON_REFER_VPROC_EXT_OFF	(1<<19)
#define RESET_PWR_ON_EN					(1<<20)
#define GIC_WAKEUP_IGNORE				(1<<21)

/*
 * per_cpu/cluster helper
 */
struct per_cpu_reg {
	int cluster_addr;
	int cpu_stride;
};

volatile int leave_busy_loop_flag[8];
volatile unsigned int cpu_enter_wfe[8];
volatile unsigned int cpu_reset[8];
volatile int cpu_power_down_flag[8];
extern __slave_init();

#define per_cpu(cluster, cpu, reg) (reg[cluster].cluster_addr + (cpu << reg[cluster].cpu_stride))
#define per_cluster(cluster, reg) (reg[cluster].cluster_addr)

/* per_cpu registers for SPM_MCU?_PWR_CON */
static const struct per_cpu_reg SPM_CPU_PWR[] = {
	[0] = { .cluster_addr = SPM_MP0_CPU0_PWR_CON, .cpu_stride = 2 },
	/*[1] = { .cluster_addr = SPM_MCU8_PWR_CON, .cpu_stride = 2 },*/
};

/* per_cluster registers for SPM_MCU?_PWR_CON */
static const struct per_cpu_reg SPM_CLUSTER_PWR[] = {
	[0] = { .cluster_addr = SPM_MP0_CPUTOP_PWR_CON, },
	/*[1] = { .cluster_addr = SPM_MCU7_PWR_CON, },*/
};


/* === MCUCFG related registers */
/* aa64naa32 */
#define MCUCFG_MP0_CLUSTER_CFG5			(MCUCFG_BASE + 0xc8e4)
/* reset vectors */
#define MCUCFG_MP0_CLUSTER_CFG8			(MCUCFG_BASE + 0xc900)
#define MCUCFG_MP0_CLUSTER_CFG10		(MCUCFG_BASE + 0xc908)
#define MCUCFG_MP0_CLUSTER_CFG12		(MCUCFG_BASE + 0xc910)
#define MCUCFG_MP0_CLUSTER_CFG14		(MCUCFG_BASE + 0xc918)
#define MCUCFG_MP0_CLUSTER_CFG16		(MCUCFG_BASE + 0xc920)
#define MCUCFG_MP0_CLUSTER_CFG18		(MCUCFG_BASE + 0xc928)
#define MCUCFG_MP0_CLUSTER_CFG20		(MCUCFG_BASE + 0xc930)
#define MCUCFG_MP0_CLUSTER_CFG22		(MCUCFG_BASE + 0xc938)

/**** per_cpu registers for MCUCFG_MP0_CLUSTER_CFG? */
static const struct per_cpu_reg MCUCFG_BOOTADDR[] = {
	[0] = { .cluster_addr = MCUCFG_MP0_CLUSTER_CFG8, .cpu_stride = 3 },
};

/**** per_cpu registers for MCUCFG_MP0_CLUSTER_CFG5 */
static const struct per_cpu_reg MCUCFG_INITARCH[] = {
	[0] = { .cluster_addr = MCUCFG_MP0_CLUSTER_CFG5 },
};

/* === CPC control */
#define MCUCFG_CPC_FLOW_CTRL_CFG		(MCUCFG_BASE + 0xa814)
#define MCUCFG_CPC_SPMC_PWR_STATUS		MCUCFG_BASE + 0xa840)

/* bit fields of CPC_FLOW_CTRL_CFG */
#define CPC_CTRL_ENABLE					(1 << 16)
#define SSPM_ALL_PWR_CTRL_EN			(1 << 13) /* for cpu-hotplug */

/* bit fields of CPC_SPMC_PWR_STATUS */
#define CORE_SPMC_PWR_ON_ACK			(0xfff << 0)

/* === APB Module infracfg_ao */
#define INFRA_TOPAXI_PROTECTEN			(INFRACFG_AO_BASE + 0x0220)
#define INFRA_TOPAXI_PROTECTEN_STA0		(INFRACFG_AO_BASE + 0x0224)
#define INFRA_TOPAXI_PROTECTEN_STA1		(INFRACFG_AO_BASE + 0x0228)
#define INFRA_TOPAXI_PROTECTEN_SET		(INFRACFG_AO_BASE + 0x02a0)
#define INFRA_TOPAXI_PROTECTEN_CLR		(INFRACFG_AO_BASE + 0x02a4)
#define INFRA_TOPAXI_PROTECTEN_1		(INFRACFG_AO_BASE + 0x0250)
#define INFRA_TOPAXI_PROTECTEN_STA0_1	(INFRACFG_AO_BASE + 0x0254)
#define INFRA_TOPAXI_PROTECTEN_STA1_1	(INFRACFG_AO_BASE + 0x0258)
#define INFRA_TOPAXI_PROTECTEN_1_SET	(INFRACFG_AO_BASE + 0x02a8)
#define INFRA_TOPAXI_PROTECTEN_1_CLR	(INFRACFG_AO_BASE + 0x02ac)

/* bit fields of INFRA_TOPAXI_PROTECTEN */
#define MP0_SPMC_PROT_STEP1_0_MASK		((1 << 12))
#define MP0_SPMC_PROT_STEP1_1_MASK		((1 << 26) | (1 << 12))


/* cpu spmc cfg */
#define MCUCFG_MP0_CPU0_SPMC_SW_CFG		(MCUCFG_BASE +  0x200)
#define MCUCFG_MP0_CPU1_SPMC_SW_CFG		(MCUCFG_BASE +  0xA00)
#define MCUCFG_MP0_CPU2_SPMC_SW_CFG		(MCUCFG_BASE + 0x1200)
#define MCUCFG_MP0_CPU3_SPMC_SW_CFG		(MCUCFG_BASE + 0x1A00)
#define MCUCFG_MP0_CPU4_SPMC_SW_CFG		(MCUCFG_BASE + 0x2200)
#define MCUCFG_MP0_CPU5_SPMC_SW_CFG		(MCUCFG_BASE + 0x2A00)
#define MCUCFG_MP0_CPU6_SPMC_SW_CFG		(MCUCFG_BASE + 0x3200)
#define MCUCFG_MP0_CPU7_SPMC_SW_CFG		(MCUCFG_BASE + 0x3A00)

/**** per_cpu registers for MCUCFG_MP0_CPUx_SPMC_SW_CFG */
static const struct per_cpu_reg MCUCFG_CPU_SPMC_CFG[] = {
	[0] = { .cluster_addr = MCUCFG_MP0_CPU0_SPMC_SW_CFG, .cpu_stride = 11 },
};
/* bit fields of MCUCFG_MP0_CPUx_SPMC_SW_CFG */
#define SW_SPARK_EN					(1 << 0)
#define SW_PCHANNEL_HANDSHAKE		(1 << 1)
#define SW_DFD_OVERRIDE_DIS			(1 << 2)
#define SW_PWR_ON_OVERRIDE_EN		(1 << 3)
#define SW_PWR_ON					(1 << 4)
#define SW_FSM_OVERIDE				(1 << 5)
#define SW_LOGIC_PRE1_PDB			(1 << 6)
#define SW_LOGIC_PRE2_PDB			(1 << 7)
#define SW_LOGIC_PDB				(1 << 8)
#define SW_ISO						(1 << 9)
#define SW_SRAM_ISOINTB				(1 << 10)
#define SW_CKISO					(1 << 11)
#define SW_CLK_DIS					(1 << 12)
#define SW_SRAM_SLEEPB_EN			(1 << 13)
#define SW_SRAM_PD_EN				(1 << 14)
#define SW_HOT_PLUG_RESET			(1 << 15)
#define SW_WARM_RESET				(1 << 16)
#define SW_NO_SPARK2LDO				(1 << 17)
#define SW_DBG_RECOV_WARM_RESET		(1 << 18)

/* cpu spmc pwr status0 */
#define MCUCFG_MP0_CPU0_SPMC_PWR_STATUS0	(MCUCFG_BASE +  0x210)
#define MCUCFG_MP0_CPU1_SPMC_PWR_STATUS0	(MCUCFG_BASE +  0xA10)
#define MCUCFG_MP0_CPU2_SPMC_PWR_STATUS0	(MCUCFG_BASE + 0x1210)
#define MCUCFG_MP0_CPU3_SPMC_PWR_STATUS0	(MCUCFG_BASE + 0x1A10)
#define MCUCFG_MP0_CPU4_SPMC_PWR_STATUS0	(MCUCFG_BASE + 0x2210)
#define MCUCFG_MP0_CPU5_SPMC_PWR_STATUS0	(MCUCFG_BASE + 0x2A10)
#define MCUCFG_MP0_CPU6_SPMC_PWR_STATUS0	(MCUCFG_BASE + 0x3210)
#define MCUCFG_MP0_CPU7_SPMC_PWR_STATUS0	(MCUCFG_BASE + 0x3A10)

/**** per_cpu registers for MCUCFG_MP0_CPUx_SPMC_SW_CFG */
static const struct per_cpu_reg MCUCFG_CPU_SPMC_PWR_STATUS0[] = {
	[0] = { .cluster_addr = MCUCFG_MP0_CPU0_SPMC_PWR_STATUS0, .cpu_stride = 11 },
};
/* bit fields of MCUCFG_MP0_CPUx_SPMC_SW_CFG */
#define CPU_PWR_ON_ACK				(0x1 << 0)
#define RET_ON_ACK					(0x1 << 1)
#define LOGIC_PRE1_PDBO_ALL_ON_ACK	(0x1 << 2)
#define LOGIC_PRE2_PDBO_ALL_ON_ACK	(0x1 << 3)
#define LOGIC_PDBO_ALL_ON_ACK		(0x1 << 4)
#define LOGIC_PDBO_ALL_OFF_ACK		(0x1 << 5)
#define FSM_STATE_OUT				(0x7F << 6)
#define SW_PD_SEQ_DONE				(0x1 << 13)
#define SW_SLEEPB_SEQ_DONE			(0x1 << 14)
#define CUR_PSTATE					(0x3F << 15)

/* cpu spmc cfg */
#define MCUCFG_MP0_CPU0_SPMC_SW_PCHANNEL	(MCUCFG_BASE +  0x208)
#define MCUCFG_MP0_CPU1_SPMC_SW_PCHANNEL	(MCUCFG_BASE +  0xA08)
#define MCUCFG_MP0_CPU2_SPMC_SW_PCHANNEL	(MCUCFG_BASE + 0x1208)
#define MCUCFG_MP0_CPU3_SPMC_SW_PCHANNEL	(MCUCFG_BASE + 0x1A08)
#define MCUCFG_MP0_CPU4_SPMC_SW_PCHANNEL	(MCUCFG_BASE + 0x2208)
#define MCUCFG_MP0_CPU5_SPMC_SW_PCHANNEL	(MCUCFG_BASE + 0x2A08)
#define MCUCFG_MP0_CPU6_SPMC_SW_PCHANNEL	(MCUCFG_BASE + 0x3208)
#define MCUCFG_MP0_CPU7_SPMC_SW_PCHANNEL	(MCUCFG_BASE + 0x3A08)

/**** per_cpu registers for MCUCFG_MP0_CPUx_SPMC_SW_PCHANNEL*/
static const struct per_cpu_reg MCUCFG_CPU_SPMC_PCHANNEL[] = {
	[0] = { .cluster_addr = MCUCFG_MP0_CPU0_SPMC_SW_PCHANNEL, .cpu_stride = 11 },
};
/* bit fields of MCUCFG_MP0_CPUx_SPMC_SW_PCHANNEL */
#define SW_PSTATE				(0x3F << 0)
#define SW_PREQ					(0x1 << 6)
#define SW_PACTIVE				(0x1FF << 7)
#define SW_PACCEPT				(0x1 << 16)
#define SW_PDENY				(0x1 << 17)
#define SW_IGNORE_PACTIVE		(0x1 << 18)
#define SW_PACCEPT_OVERRIDE		(0x1 << 19)

/* === SPMC related registers */
#define SMP_MCUSYS_IDLE_STA		(SPM_BASE + 0x230)
#define PICACHU_WFE_STATUS		(0x0C53AE04)

/* bit-fields of MCUSYS_IDLE_STA */
#define MP0_CLUSTER_IDLE_TO_PWR_OFF	(1 << 1)
#define MP0_CPU_IDLE_TO_PWR_OFF		16

#define SPM_PWR_STATUS				(SPM_BASE + 0x174)

int spm_get_powerstate(uint32_t mask)
{
	return (adcc_read(SPM_PWR_STATUS) & mask);
}

int spm_get_cpu_powerstate(unsigned int cluster, unsigned int cpu)
{
	return (adcc_read(SPM_PWR_STATUS)&(1<<cpu));
}

int boot_secondary(unsigned int cpu)
{
	cpu_power_down_flag[cpu] = 0;
	PRINTF_SPMC("Power on core %d :\n", cpu);
	spm_poweron_cpu(0, cpu);

	return 0;
}

int power_down_cpu(unsigned int cpu)
{
	cpu_power_down_flag[cpu] = 1;
	PRINTF_SPMC("Power off core %d:\n", cpu);
	spm_poweroff_cpu(0, cpu);

	return 0;
}


#define MP0_AXI_CONFIG				(MCUCFG_BASE + 0x02C)
#define MPx_AXI_CONFIG_ACINACTM		(1<<4)

void enable_scu(unsigned long mpidr)
{
	unsigned int axi_config = 0;
	unsigned int acinactm = 0;

	switch (mpidr) {
	case 0x000:
		axi_config = MP0_AXI_CONFIG;
		acinactm = MPx_AXI_CONFIG_ACINACTM;
		break;
	default:
		ERROR("enable_scu: mpidr not exists\n");
		assert(0);
	}
	mmio_clrbits_32(axi_config, acinactm);
}

void psci_entrypoint_udi(void)
{

	set_CPUPWRCTLR_CORE_PWRDN_EN();
	__asm__("dsb sy");
	__asm__("isb");
	__asm__("wfi");

}

const unsigned int psci_arr_wfi[] = {
	0xd5033f9f,
	0xd5033fdf,
	0xd503207f,
	0x17fffffd,
};

#define MP0_CLUSTER_CFG5					(MCUCFG_BASE + 0xC8E4)

#define MP0_MISC_CONFIG_BOOT_ADDR_L(cpu)	(MCUCFG_BASE + 0xc900 + ((cpu) * 8))
#define MP0_MISC_CONFIG_BOOT_ADDR_H(cpu)	(MCUCFG_BASE + 0xc904 + ((cpu) * 8))

#define MCUCFG_MP0_CLUSTER_CFG8				(MCUCFG_BASE + 0xc900)
#define MCUCFG_MP0_CLUSTER_CFG10			(MCUCFG_BASE + 0xc908)
#define MCUCFG_MP0_CLUSTER_CFG12			(MCUCFG_BASE + 0xc910)
#define MCUCFG_MP0_CLUSTER_CFG14			(MCUCFG_BASE + 0xc918)
#define MCUCFG_MP0_CLUSTER_CFG16			(MCUCFG_BASE + 0xc920)
#define MCUCFG_MP0_CLUSTER_CFG18			(MCUCFG_BASE + 0xc928)
#define MCUCFG_MP0_CLUSTER_CFG20			(MCUCFG_BASE + 0xc930)
#define MCUCFG_MP0_CLUSTER_CFG22			(MCUCFG_BASE + 0xc938)


void mt_smp_prepare_cpus(void)
{
#if 1
	int i;

	/* Set all cpus into AArch64 */
	adcc_write(MP0_CLUSTER_CFG5, adcc_read(MP0_CLUSTER_CFG5) | 0x00FF0000);
	for (i = 0; i < 8; i++) {
		if (adcc_slave_cpu_base == 0)
			adcc_write(MP0_MISC_CONFIG_BOOT_ADDR_L(i), (unsigned long)aarch64_adcc_start_slave);
#if ADCC_DVT
		else if (adcc_slave_cpu_base == 1)
			adcc_write(MP0_MISC_CONFIG_BOOT_ADDR_L(i), (unsigned long)aarch64_adcc_udi_start_slave);
#endif
		else
			adcc_write(MP0_MISC_CONFIG_BOOT_ADDR_L(i), (unsigned long)psci_arr_wfi);

		adcc_write(MP0_MISC_CONFIG_BOOT_ADDR_H(i), 0);
	}
#else
	/* Set all cpus into AArch32 */
	adcc_write(MP0_CLUSTER_CFG5, adcc_read(MP0_CLUSTER_CFG5) & 0xFF00FFFF);
	/* enable bootrom power down mode */
    adcc_write(BOOTROM_SEC_CTRL, adcc_read(BOOTROM_SEC_CTRL) | SW_ROM_PD);
	adcc_write(BOOTROM_BOOT_ADDR, (unsigned long) psci_entrypoint_udi);
#endif
}



int spmc_init(unsigned int val)
{
#if CONFIG_SPMC_MODE == 1
	PRINTF_SPMC("SPM: enable SPMC mode\n");
#else
	PRINTF_SPMC("SPM: enable CPC mode\n");
#endif
	adcc_write(SPM_POWERON_CONFIG_EN, PROJECT_CODE | BCLK_CG_EN);

	adcc_slave_cpu_base = val;
	mt_smp_prepare_cpus();

	/* TINFO=SPMC_INIT: release PWR_RST_B of Core 1-7 */
	mmio_setbits_32(per_cpu(0, 1, SPM_CPU_PWR), PWR_RST_B);
	mmio_setbits_32(per_cpu(0, 2, SPM_CPU_PWR), PWR_RST_B);
	mmio_setbits_32(per_cpu(0, 3, SPM_CPU_PWR), PWR_RST_B);
	mmio_setbits_32(per_cpu(0, 4, SPM_CPU_PWR), PWR_RST_B);
	mmio_setbits_32(per_cpu(0, 5, SPM_CPU_PWR), PWR_RST_B);
	mmio_setbits_32(per_cpu(0, 6, SPM_CPU_PWR), PWR_RST_B);
	mmio_setbits_32(per_cpu(0, 7, SPM_CPU_PWR), PWR_RST_B);

	/* INFO=SPMC_INIT: clear resetpwron of mcusys/cluster/core0 */
	mmio_clrbits_32(SPM_MCUSYS_PWR_CON, RESETPWRON_CONFIG);
	mmio_clrbits_32(SPM_MP0_CPUTOP_PWR_CON, RESETPWRON_CONFIG);
	mmio_clrbits_32(per_cpu(0, 0, SPM_CPU_PWR), RESETPWRON_CONFIG);

#if CONFIG_SPMC_MODE == 0
	/* enable CPC */
	mmio_setbits_32(MCUCFG_CPC_FLOW_CTRL_CFG, CPC_CTRL_ENABLE);
#endif

	return 0;
}

#if CONFIG_SPMC_MODE == 0
/**
 * Power on a core with specified cluster and core index
 * with CPC
 *
 * @cluster: the cluster ID of the CPU which to be powered on
 * @cpu: the CPU ID of the CPU which to be powered on
 */
static void spm_poweron_cpu_cpc(unsigned int cluster, unsigned int cpu)
{
	unsigned int cnt = 0;

	/* info CPC that CPU hotplug on */
	mmio_setbits_32(MCUCFG_CPC_FLOW_CTRL_CFG, SSPM_ALL_PWR_CTRL_EN);

	/* Set mp0_spmc_pwr_on_cpuX = 1 */
	mmio_setbits_32(per_cpu(cluster, cpu, SPM_CPU_PWR), PWR_ON);

	/* wait for power on ack */
	while (!spm_get_cpu_powerstate(cluster, cpu)) {

		if (cnt++ > 2000000) {
			print("spm_poweron_cpu_cpc fail for cpu %d, SPM_PWR_STATUS = 0x%x\n", cpu, adcc_read(SPM_PWR_STATUS));
			break;
		}
	}

	/* info CPC that CPU hotplug off */
	mmio_clrbits_32(MCUCFG_CPC_FLOW_CTRL_CFG, SSPM_ALL_PWR_CTRL_EN);
}

/**
 * Power off a core with specified cluster and core index
 * with CPC
 *
 * @cluster: the cluster ID of the CPU which to be powered off
 * @cpu: the CPU ID of the CPU which to be powered off
 */
static void spm_poweroff_cpu_cpc(unsigned int cluster, unsigned int cpu)
{
	unsigned int cnt = 0 ;

	/* Set mp0_spmc_pwr_on_cpuX = 0 */
	mmio_clrbits_32(per_cpu(cluster, cpu, SPM_CPU_PWR), PWR_ON);

	/* wait for power on ack */
	while (spm_get_cpu_powerstate(cluster, cpu)) {
		if (cnt++ > 2000000) {
			print("spm_poweroff_cpu_cpc fail for cpu %d, SPM_PWR_STATUS = 0x%x\n", cpu, adcc_read(SPM_PWR_STATUS));
			break;
		}
	}
}

/**
 * Power off a cluster with specified index
 * with CPC
 *
 * @cluster: the cluster index which to be powered off
 */
static void spm_poweroff_cluster_cpc(unsigned int cluster)
{
	/* CPU hotplug do not power off cluster for ARMv8.2 */
}

/**
 * Power on a cluster with specified index
 * with CPC
 *
 * @cluster: the cluster index which to be powered on
 */
static void spm_poweron_cluster_cpc(unsigned int cluster)
{
	/* CPU hotplug do not power on cluster for ARMv8.2 */
}
#endif /* CONFIG_SPMC_MODE == 0 */

/**
 * Power on a core with specified cluster and core index
 *
 * @cluster: the cluster ID of the CPU which to be powered on
 * @cpu: the CPU ID of the CPU which to be powered on
 */
void spm_poweron_cpu(unsigned int cluster, unsigned int cpu)
{
	PRINTF_SPMC("spmc: power on core %d.%d\n", cluster, cpu);

#if CONFIG_SPMC_MODE == 0
	spm_poweron_cpu_cpc(cluster, cpu);

	PRINTF_SPMC("spmc: power on core %d.%d successfully\n", cluster, cpu);
	return;
#endif

	/* TINFO="Start to turn on MP0_CPU0_SPMC" */
	/* TINFO="Set VPROC_EXT_OFF = 0" */
	mmio_clrbits_32(per_cpu(cluster, cpu, SPM_CPU_PWR), VPROC_EXT_OFF);

	/* TINFO="Set PWR_ON = 1" */
	mmio_setbits_32(per_cpu(cluster, cpu, SPM_CPU_PWR), PWR_ON);

	/* TINFO="Wait until MP0_CPU0_SPMC_PWR_STA_MASK = 1" */
	while (!spm_get_cpu_powerstate(cluster, cpu)) {
	}

	/* TINFO="Set PWR_RST_B = 1" */
	mmio_setbits_32(per_cpu(cluster, cpu, SPM_CPU_PWR), PWR_RST_B);

	/* TINFO="Finish to turn on MP0_CPU0_SPMC" */
	PRINTF_SPMC("spmc: power on core %d.%d successfully\n", cluster, cpu);
}

/**
 * Power off a core with specified cluster and core index
 *
 * @cluster: the cluster ID of the CPU which to be powered off
 * @cpu: the CPU ID of the CPU which to be powered off
 */
void spm_poweroff_cpu(unsigned int cluster, unsigned int cpu)
{
	PRINTF_SPMC("spmc: power off core %d.%d\n", cluster, cpu);

#if CONFIG_SPMC_MODE == 0
	spm_poweroff_cpu_cpc(cluster, cpu);

	PRINTF_SPMC("spmc: power off core %d.%d successfully\n", cluster, cpu);
	return;
#endif

	/* TINFO="Start to turn off MP0_CPU0_SPMC" */
	/* TINFO="Set PWR_RST_B = 0" */
	mmio_clrbits_32(per_cpu(cluster, cpu, SPM_CPU_PWR), PWR_RST_B);

	/* TINFO="Set PWR_ON = 0" */
	mmio_clrbits_32(per_cpu(cluster, cpu, SPM_CPU_PWR), PWR_ON);

	/* TINFO="Wait until MP0_CPU0_SPMC_PWR_STA_MASK = 0" */
	while (spm_get_cpu_powerstate(cluster, cpu))
		;

	/* TINFO="Set VPROC_EXT_OFF = 1" */
	mmio_setbits_32(per_cpu(cluster, cpu, SPM_CPU_PWR), VPROC_EXT_OFF);

	/* TINFO="Finish to turn off MP0_CPU0_SPMC" */
	PRINTF_SPMC("spmc: power off core %d.%d successfully\n", cluster, cpu);
}

#if 0
/**
 * Power off a cluster with specified index
 *
 * @cluster: the cluster index which to be powered off
 */
void spm_poweroff_cluster(unsigned int cluster)
{
	INFO("spmc: power off cluster %d\n", cluster);

#if CONFIG_SPMC_MODE == 0
	spm_poweroff_cluster_cpc(cluster);

	INFO("spmc: power off cluster %d successfully\n", cluster);
	return;
#endif

	/* TINFO="Start to turn off MP0_SPMC" */
	/* TINFO="Set bus protect - step1 : 0" */
	adcc_write(INFRA_TOPAXI_PROTECTEN_SET, MP0_SPMC_PROT_STEP1_0_MASK);

	while (!(adcc_read(INFRA_TOPAXI_PROTECTEN_STA1) &
		 MP0_SPMC_PROT_STEP1_0_MASK))
		;

	/* TINFO="Set bus protect - step1 : 1" */
	adcc_write(INFRA_TOPAXI_PROTECTEN_1_SET, MP0_SPMC_PROT_STEP1_1_MASK);

	while (!(adcc_read(INFRA_TOPAXI_PROTECTEN_STA1_1) &
		 MP0_SPMC_PROT_STEP1_1_MASK))
		;

	/* TINFO="Set PWR_RST_B = 0" */
	mmio_clrbits_32(per_cluster(cluster, SPM_CLUSTER_PWR), PWR_RST_B);

	/* TINFO="Set PWR_ON = 0" */
	mmio_clrbits_32(per_cluster(cluster, SPM_CLUSTER_PWR), PWR_ON);

	/* TINFO="Wait until MP0_SPMC_PWR_STA_MASK = 0" */
	while (spm_get_cluster_powerstate(cluster))
		;

	/* TINFO="Set VPROC_EXT_OFF = 1" */
	mmio_setbits_32(per_cluster(cluster, SPM_CLUSTER_PWR), VPROC_EXT_OFF);

	/* TINFO="Finish to turn off MP0_SPMC" */
	INFO("spmc: power off cluster %d successfully\n", cluster);
}

/**
 * Power on a cluster with specified index
 *
 * @cluster: the cluster index which to be powered on
 */
void spm_poweron_cluster(unsigned int cluster)
{
	INFO("spmc: power on cluster %d\n", cluster);

#if CONFIG_SPMC_MODE == 0
	spm_poweron_cluster_cpc(cluster);

	INFO("spmc: power on cluster %d successfully\n", cluster);
	return;
#endif

	/* TINFO="Start to turn on MP0_SPMC" */
	/* TINFO="Set VPROC_EXT_OFF = 0" */
	mmio_clrbits_32(per_cluster(cluster, SPM_CLUSTER_PWR), VPROC_EXT_OFF);

	/* TINFO="Set PWR_ON = 1" */
	mmio_setbits_32(per_cluster(cluster, SPM_CLUSTER_PWR), PWR_ON);

	/* TINFO="Wait until MP0_SPMC_PWR_STA_MASK = 1" */
	while (!spm_get_cluster_powerstate(cluster))
		;

	/* TINFO="Set PWR_RST_B = 1" */
	mmio_setbits_32(per_cluster(cluster, SPM_CLUSTER_PWR), PWR_RST_B);

	/* TINFO="Release bus protect - step1 : 0" */
	adcc_write(INFRA_TOPAXI_PROTECTEN_CLR, MP0_SPMC_PROT_STEP1_0_MASK);

	/* TINFO="Release bus protect - step1 : 1" */
	adcc_write(INFRA_TOPAXI_PROTECTEN_1_CLR, MP0_SPMC_PROT_STEP1_1_MASK);

	/* TINFO="Finish to turn on MP0_SPMC" */
	INFO("spmc: power on cluster %d successfully\n", cluster);
}
#endif

/***************************
 *                         *
 *       SPMC3.0 DVT       *
 *                         *
 ***************************
 */
void cpc_hotplug_poweron_cpu(unsigned int cluster, unsigned int cpu)
{
	/* info CPC that CPU hotplug on */
	mmio_setbits_32(MCUCFG_CPC_FLOW_CTRL_CFG, SSPM_ALL_PWR_CTRL_EN);

	/* Set mp0_spmc_pwr_on_cpuX = 1 */
	mmio_setbits_32(per_cpu(cluster, cpu, SPM_CPU_PWR), PWR_ON);

	/* wait for power on ack */
	while (!spm_get_cpu_powerstate(cluster, cpu))
		;

	/* info CPC that CPU hotplug off */
	mmio_clrbits_32(MCUCFG_CPC_FLOW_CTRL_CFG, SSPM_ALL_PWR_CTRL_EN);
}

/**
 * Power off a core with specified cluster and core index
 * with CPC
 *
 * @cluster: the cluster ID of the CPU which to be powered off
 * @cpu: the CPU ID of the CPU which to be powered off
 */
void cpc_hotplug_poweroff_cpu(unsigned int cluster, unsigned int cpu)
{
	/* Set mp0_spmc_pwr_on_cpuX = 0 */
	mmio_clrbits_32(per_cpu(cluster, cpu, SPM_CPU_PWR), PWR_ON);

	/* wait for power on ack */
	/*while (spm_get_cpu_powerstate(cluster, cpu))
		; */
}
void cpc_mcdi_poweron_cpu(unsigned int cluster, unsigned int cpu)
{
	/* ipi_cluster_cpu(); */
}

void sspm_hotplug_auto_poweron_cpu(unsigned int cluster, unsigned int cpu)
{
	INFO("[sspm][hotplug][auto] power on core %d.%d\n", cluster, cpu);
	/* TINFO="Start to turn on MP0_CPU0_SPMC" */
	/* TINFO="Set VPROC_EXT_OFF = 0" */
	mmio_clrbits_32(per_cpu(cluster, cpu, SPM_CPU_PWR), VPROC_EXT_OFF);

	/* TINFO="Set PWR_ON = 1" */
	mmio_setbits_32(per_cpu(cluster, cpu, SPM_CPU_PWR), PWR_ON);

	/* TINFO="Wait until MP0_CPU0_SPMC_PWR_STA_MASK = 1" */
	while (!spm_get_cpu_powerstate(cluster, cpu))
		;

	/* TINFO="Set PWR_RST_B = 1" */
	/*mmio_setbits_32(per_cpu(cluster, cpu, SPM_CPU_PWR), PWR_RST_B);*/

	/* TINFO="Finish to turn on MP0_CPU0_SPMC" */
	INFO("[sspm][hotplug][auto] power on core %d.%d successfully\n", cluster, cpu);
}

/**
 * Power off a core with specified cluster and core index
 *
 * @cluster: the cluster ID of the CPU which to be powered off
 * @cpu: the CPU ID of the CPU which to be powered off
 */
void sspm_hotplug_auto_poweroff_cpu(unsigned int cluster, unsigned int cpu)
{
	INFO("[sspm][hotplug][auto] power off core %d.%d\n", cluster, cpu);
	/* TINFO="Start to turn off MP0_CPU0_SPMC" */
	/* TINFO="Set PWR_RST_B = 0" */
	/*mmio_clrbits_32(per_cpu(cluster, cpu, SPM_CPU_PWR), PWR_RST_B);*/

/* Set "core_spmc_pchannel_handshake" = 1 */
	mmio_setbits_32(per_cpu(cluster, cpu, MCUCFG_CPU_SPMC_CFG), SW_PCHANNEL_HANDSHAKE);

	/* TINFO="Set PWR_ON = 0" */
	mmio_clrbits_32(per_cpu(cluster, cpu, SPM_CPU_PWR), PWR_ON);

	/* TINFO="Wait until MP0_CPU0_SPMC_PWR_STA_MASK = 0" */
	/*while (spm_get_cpu_powerstate(cluster, cpu))
		;*/

	/* TINFO="Set VPROC_EXT_OFF = 1" */
	/*mmio_setbits_32(per_cpu(cluster, cpu, SPM_CPU_PWR), VPROC_EXT_OFF);*/

	/* TINFO="Finish to turn off MP0_CPU0_SPMC" */
	INFO("[sspm][hotplug][auto] power off core %d.%d successfully\n", cluster, cpu);
}

void sspm_mcdi_auto_poweron_cpu(unsigned int cluster, unsigned int cpu)
{
	INFO("[sspm][mcdi][auto] power on core %d.%d\n", cluster, cpu);
	/* TINFO="Start to turn on MP0_CPU0_SPMC" */
	/* TINFO="Set VPROC_EXT_OFF = 0" */
	mmio_clrbits_32(per_cpu(cluster, cpu, SPM_CPU_PWR), VPROC_EXT_OFF);

	/* Generate rising edge of "mp0_spmc_pwr_on_cpux"to init ON */
	mmio_clrbits_32(per_cpu(cluster, cpu, SPM_CPU_PWR), PWR_ON);
	mmio_setbits_32(per_cpu(cluster, cpu, SPM_CPU_PWR), PWR_ON);

	/* TINFO="Wait until MP0_CPU0_SPMC_PWR_STA_MASK = 1" */
	while (!spm_get_cpu_powerstate(cluster, cpu))
		;

	/* TINFO="Set PWR_RST_B = 1" */
	/*mmio_setbits_32(per_cpu(cluster, cpu, SPM_CPU_PWR), PWR_RST_B);*/

	/* TINFO="Finish to turn on MP0_CPU0_SPMC" */
	INFO("[sspm][mcdi][auto] power on core %d.%d successfully\n", cluster, cpu);

}
void sspm_mcdi_auto_poweroff_cpu(unsigned int cluster, unsigned int cpu)
{
	INFO("[sspm][mcdi][auto] power off core %d.%d\n", cluster, cpu);
	while (spm_get_cpu_powerstate(cluster, cpu))
		;
	INFO("[sspm][mcdi][auto] power off core %d.%d\n", cluster, cpu);
}
void sspm_noauto_poweron_cpu(unsigned int cluster, unsigned int cpu)
{
	INFO("[sspm][hotplug][noauto] power on core %d.%d\n", cluster, cpu);
	/* TINFO="Start to turn on MP0_CPU0_SPMC" */
	/* TINFO="Set VPROC_EXT_OFF = 0" */
	mmio_clrbits_32(per_cpu(cluster, cpu, SPM_CPU_PWR), VPROC_EXT_OFF);

	/* TINFO="Set PWR_ON = 1" */
	mmio_setbits_32(per_cpu(cluster, cpu, SPM_CPU_PWR), PWR_ON);

	/* TINFO="Wait until MP0_CPU0_SPMC_PWR_STA_MASK = 1" */
	while (!spm_get_cpu_powerstate(cluster, cpu))
		;

	/* TINFO="Set PWR_RST_B = 1" */
	mmio_setbits_32(per_cpu(cluster, cpu, SPM_CPU_PWR), PWR_RST_B);

	/* TINFO="Finish to turn on MP0_CPU0_SPMC" */
	INFO("[sspm][hotplug][noauto] power on core %d.%d successfully\n", cluster, cpu);

}

void sspm_noauto_poweroff_cpu(unsigned int cluster, unsigned int cpu)
{
/* Wait mp0_cpu_idle_to_pwr_off[*]=1 */
	while (!spm_get_cpu_idle_to_pwr_off(cluster, cpu))
		;
/* Set "core_spmc_pchannel_handshake" = 1 */
	mmio_setbits_32(per_cpu(cluster, cpu, MCUCFG_CPU_SPMC_CFG), SW_PCHANNEL_HANDSHAKE);
/* Set "mp0_spmc_pwr_on_cpux"= 0 to init OFF */
	mmio_clrbits_32(per_cpu(cluster, cpu, SPM_CPU_PWR), PWR_ON);
/* Wait "mp0_spmc_pwr_ack_cpux"= 0 for power off finish */
	while (spm_get_cpu_powerstate(cluster, cpu))
		;
/* Set "core_spmc_pchannel_handshake" = 0 */
	mmio_clrbits_32(per_cpu(cluster, cpu, MCUCFG_CPU_SPMC_CFG), SW_PCHANNEL_HANDSHAKE);
}

void set_cpu_ignore_pactive(unsigned int cluster, unsigned int cpu)
{
	mmio_setbits_32(per_cpu(cluster, cpu, MCUCFG_CPU_SPMC_PCHANNEL), SW_IGNORE_PACTIVE);
}
void clear_cpu_ignore_pactive(unsigned int cluster, unsigned int cpu)
{
	mmio_clrbits_32(per_cpu(cluster, cpu, MCUCFG_CPU_SPMC_PCHANNEL), SW_IGNORE_PACTIVE);
}

#if 0
int spm_get_cpu_idle_to_pwr_off(unsigned int cluster, unsigned int cpu)
{
	uint32_t mask;

	/*assert(cluster == 0);*/

	/*
	 * Mp0_cpu_idle_to_pwr_off[7:0]: 0x10006230[23:16]
	 * Mp_cluster_idle_to_pwr_off: 0x10006230[1]
	 */
	mask = 1 << (MP0_CPU_IDLE_TO_PWR_OFF + cpu);

	INFO("adcc_read(SMP_MCUSYS_IDLE_STA) = 0x%x\n", adcc_read(SMP_MCUSYS_IDLE_STA));

	return (adcc_read(SMP_MCUSYS_IDLE_STA) & mask);
}
#endif

int spm_get_cpu_idle_to_pwr_off(unsigned int cluster, unsigned int cpu)
{
	uint32_t mask;

	/*assert(cluster == 0);*/

	/*
	 * Mp0_cpu_idle_to_pwr_off[7:0]: 0x10006230[23:16]
	 * Mp_cluster_idle_to_pwr_off: 0x10006230[1]
	 */
	mask = 1 << cpu;

	INFO("adcc_read(PICACHU_WFE_STATUS) = 0x%x\n", adcc_read(PICACHU_WFE_STATUS));

	return (adcc_read(PICACHU_WFE_STATUS) & mask);
}

/* core power off
 * Set CPUPWRCTLR.CORE_PWRDN_EN = 1
 * CPUPWRCTLR[0]=1
 * ISB&DSB
 * WFI
 */
void set_CPUPWRCTLR_CORE_PWRDN_EN(void)
{
#ifdef AARCH64
	asm volatile (
		"mrs x0, S3_0_c15_c2_7\n\t"
		"orr x0, x0, #1\n\t"
		"msr S3_0_c15_c2_7, x0\n\t"
		:::"x0", "cc"
		);
#else
	asm volatile (
		"mrc p15, 0, r0, c15, c2, 7\n\t"
		"orr r0, r0, #1\n\t"
		"mcr p15, 0, r0, c15, c2, 7\n\t"
		:::"r0", "cc"
		);
#endif
}




