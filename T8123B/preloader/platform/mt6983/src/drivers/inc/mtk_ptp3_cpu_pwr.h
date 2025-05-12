#ifndef MTK_PTP3_CPU_PWR
#define MTK_PTP3_CPU_PWR


/* #define MVP_SIM */

#define MT_CPC_CFG			(0x0C040000)
#define MT_CPC_CFG_SIZE		(0x10000)

#define MT_RVBARADDR_BASE	(0x0C000000)
#define MT_RVBARADDR_SIZE	(0x10000)

#define MT_UTILITYBUS_BASE	(0x0C800000)
#define MT_UTILITYBUS_SIZE	(0x800000)

#define CPC_SPMC_BASE		(0x1C001000)

/* PPU PWPR definition */
#define PPU_PWPR_MASK		(0xf)
#define PPU_PWPR_MODE_MASK	(0x1)
#define PPU_PWPR_OFF		(0)
#define PPU_PWPR_MEM_RET	(2)
#define PPU_PWPR_FULL_RET	(5)
#define PPU_PWPR_MEM_OFF	(6)
#define PPU_PWPR_FUN_RET	(7)
#define PPU_PWPR_ON		(8)
#define PPU_PWPR_WARM_RESET	(10)
#define PPU_PWPR_DYNAMIC_MODE	(1U << 8)

/* PPU PWSR definition */
#define PPU_PWSR_STATE_ON	(1U << 3)


#define PPU_PWPR_SET(_val, _policy) \
	({ _val = (_val & PPU_PWPR_MASK) |\
		  (_policy & PPU_PWPR_MASK);\
	_val; })


#define CPU_PM_FN (MTK_CPUPM_FN_INIT | \
		   MTK_CPUPM_FN_PWR_STATE_VALID | \
		   MTK_CPUPM_FN_PWR_ON_CORE_PREPARE | \
		   MTK_CPUPM_FN_PWR_DWN_CORE_PREPARE | \
		   MTK_CPUPM_FN_RESUME_CORE | \
		   MTK_CPUPM_FN_SUSPEND_CORE)


#define PER_CPU_PWR_DATA(_p, _cl, _c) ({\
	_p.rvbaraddr_l = CORE_RVBRADDR_##_cl##_##_c##_L;\
	_p.ppu_pwpr = CORE_PPU_PWPR_##_cl##_##_c;\
	_p.ppu_pwsr = CORE_PPU_PWSR_##_cl##_##_c;\
	_p.cpc_spmc = CORE_CPC_SPMC_##_cl##_##_c; })


#define PER_CPU_PWR_CTRL(_val, _cpu) ({\
	switch (_cpu) {\
	case 0:	PER_CPU_PWR_DATA(_val, 0, 0); break;\
	case 1:	PER_CPU_PWR_DATA(_val, 0, 1); break;\
	case 2:	PER_CPU_PWR_DATA(_val, 0, 2); break;\
	case 3:	PER_CPU_PWR_DATA(_val, 0, 3); break;\
	case 4:	PER_CPU_PWR_DATA(_val, 0, 4); break;\
	case 5:	PER_CPU_PWR_DATA(_val, 0, 5); break;\
	case 6:	PER_CPU_PWR_DATA(_val, 0, 6); break;\
	case 7:	PER_CPU_PWR_DATA(_val, 0, 7); break;\
	default: break;\
	} })


/*
 * Definition about bootup address for each core
 * CORE_RVBRADDR_clusterid_cpuid
 */
#define CORE_RVBRADDR_0_0_L		(MT_RVBARADDR_BASE + 0x00)
#define CORE_RVBRADDR_0_0_H		(MT_RVBARADDR_BASE + 0x04)
#define CORE_RVBRADDR_0_1_L		(MT_RVBARADDR_BASE + 0x08)
#define CORE_RVBRADDR_0_1_H		(MT_RVBARADDR_BASE + 0x0c)
#define CORE_RVBRADDR_0_2_L		(MT_RVBARADDR_BASE + 0x10)
#define CORE_RVBRADDR_0_2_H		(MT_RVBARADDR_BASE + 0x14)
#define CORE_RVBRADDR_0_3_L		(MT_RVBARADDR_BASE + 0x18)
#define CORE_RVBRADDR_0_3_H		(MT_RVBARADDR_BASE + 0x1C)
#define CORE_RVBRADDR_0_4_L		(MT_RVBARADDR_BASE + 0x20)
#define CORE_RVBRADDR_0_4_H		(MT_RVBARADDR_BASE + 0x24)
#define CORE_RVBRADDR_0_5_L		(MT_RVBARADDR_BASE + 0x28)
#define CORE_RVBRADDR_0_5_H		(MT_RVBARADDR_BASE + 0x2c)
#define CORE_RVBRADDR_0_6_L		(MT_RVBARADDR_BASE + 0x30)
#define CORE_RVBRADDR_0_6_H		(MT_RVBARADDR_BASE + 0x34)
#define CORE_RVBRADDR_0_7_L		(MT_RVBARADDR_BASE + 0x38)
#define CORE_RVBRADDR_0_7_H		(MT_RVBARADDR_BASE + 0x3C)

/*
 * Definition about bootup address for each core
 * PPU_PWPR_clusterid_cpuid
 */
#define CORE_PPU_PWPR_0_0		(MT_UTILITYBUS_BASE + 0x080000)
#define CORE_PPU_PWPR_0_1		(MT_UTILITYBUS_BASE + 0x180000)
#define CORE_PPU_PWPR_0_2		(MT_UTILITYBUS_BASE + 0x280000)
#define CORE_PPU_PWPR_0_3		(MT_UTILITYBUS_BASE + 0x380000)
#define CORE_PPU_PWPR_0_4		(MT_UTILITYBUS_BASE + 0x480000)
#define CORE_PPU_PWPR_0_5		(MT_UTILITYBUS_BASE + 0x580000)
#define CORE_PPU_PWPR_0_6		(MT_UTILITYBUS_BASE + 0x680000)
#define CORE_PPU_PWPR_0_7		(MT_UTILITYBUS_BASE + 0x780000)

/*
 * Definition about bootup address for each core
 * PPU_PWSR_clusterid_cpuid
 */
#define CORE_PPU_PWSR_0_0		(MT_UTILITYBUS_BASE + 0x080008)
#define CORE_PPU_PWSR_0_1		(MT_UTILITYBUS_BASE + 0x180008)
#define CORE_PPU_PWSR_0_2		(MT_UTILITYBUS_BASE + 0x280008)
#define CORE_PPU_PWSR_0_3		(MT_UTILITYBUS_BASE + 0x380008)
#define CORE_PPU_PWSR_0_4		(MT_UTILITYBUS_BASE + 0x480008)
#define CORE_PPU_PWSR_0_5		(MT_UTILITYBUS_BASE + 0x580008)
#define CORE_PPU_PWSR_0_6		(MT_UTILITYBUS_BASE + 0x680008)
#define CORE_PPU_PWSR_0_7		(MT_UTILITYBUS_BASE + 0x780008)

/*
 * MP0_SPMC_PWR_ON_CPUx
 */
#define CORE_CPC_SPMC_0_0		(CPC_SPMC_BASE + 0x268)
#define CORE_CPC_SPMC_0_1		(CPC_SPMC_BASE + 0x26C)
#define CORE_CPC_SPMC_0_2		(CPC_SPMC_BASE + 0x270)
#define CORE_CPC_SPMC_0_3		(CPC_SPMC_BASE + 0x274)
#define CORE_CPC_SPMC_0_4		(CPC_SPMC_BASE + 0x278)
#define CORE_CPC_SPMC_0_5		(CPC_SPMC_BASE + 0x27C)
#define CORE_CPC_SPMC_0_6		(CPC_SPMC_BASE + 0x280)
#define CORE_CPC_SPMC_0_7		(CPC_SPMC_BASE + 0x284)


struct cpu_pwr_ctrl {
	unsigned int rvbaraddr_l;
	unsigned int rvbaraddr_h;
	unsigned int ppu_pwpr;
	unsigned int ppu_pwsr;
	unsigned int cpc_spmc;
};
typedef unsigned long uintptr_t;
extern int adcc_cpu_pwr_drv_init(void);
extern int adcc_cpu_pwr_drv_restore(void);
extern int adcc_cpupm_cpu_pwr_on_prepare(unsigned int cpu, uintptr_t entry);
extern void adcc_cpupm_cpu_pwr_dwn_prepare(unsigned int cpu);
#endif
