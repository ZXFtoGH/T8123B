#ifndef MCUPM_H
#define MCUPM_H

#define MCUPM_DBG_MODE			0

#if MCUPM_DBG_MODE
#define mcupm_dbg(f, ...)		pal_log_info("[MCUPM] " f, ##__VA_ARGS__)
#else
#define mcupm_dbg(f, ...)		do{}while(0)
#endif

#define MCUPM_MEM_SIZE			0x00100000  // 1M
#define MCUPM_MEM_ALIGN			0x00010000  // 64K (minimal size for EMI MPU)
#define MCUPM_MEM_LIMIT			0x80000000
#define MCUPM_IMG_SZ			0x000C0000  // 768K
#define MCUPM_TCM_SZ			0x0000C000  //  48K

#define PT_ID_MCUPM_DM			0
#define PT_ID_MCUPM_PM			1
#define PT_ID_MCUPM_XFILE		99

#define MCUPM_SW_RSTN			0x10300000
#define MCUPM_CFGREG_DBG_CON		(MCUPM_SW_RSTN + 0x08)
#define MCUPM_CFGREG_MON_PC		(MCUPM_SW_RSTN + 0x40)
#define MCUPM_CFGREG_AHB_STATUS		(MCUPM_SW_RSTN + 0x44)
#define MCUPM_CFGREG_AHB_M0_ADDR	(MCUPM_SW_RSTN + 0x48)
#define MCUPM_CFGREG_AHB_M1_ADDR	(MCUPM_SW_RSTN + 0x4C)

//MCUPM use SRAM as GPR
#define MCUPM_SRAM_SIZE			0xC000		//48K Bytes
#define SRAM_32KB_SIZE			0x8000		//32K Bytes
#define SRAM_32KB_AP_BASE		0x1031C000
#define SRAM_RESERVED_20B		0x00000014	//20 Bytes
#define SRAM_GPR_SIZE_4B		0x4		//4Bytes
#define SRAM_GPR_TOTAL_SIZE		100		//4Bytes
#define MBOX_TABLE_TOTAL_SIZE		800
#define GPR_BASE_ADDR(x)		(MCUPM_SRAM_SIZE - \
					MBOX_TABLE_TOTAL_SIZE - \
					SRAM_GPR_TOTAL_SIZE + \
					(x * SRAM_GPR_SIZE_4B)) - \
					SRAM_32KB_SIZE + \
					SRAM_32KB_AP_BASE

#define MCUPM_SRAM_GPR0			GPR_BASE_ADDR(0)
#define MCUPM_SRAM_GPR1			GPR_BASE_ADDR(1)
#define MCUPM_SRAM_GPR2			GPR_BASE_ADDR(2)
#define MCUPM_SRAM_GPR3			GPR_BASE_ADDR(3)
#define MCUPM_SRAM_GPR4			GPR_BASE_ADDR(4)
#define MCUPM_SRAM_GPR5			GPR_BASE_ADDR(5)
#define MCUPM_SRAM_GPR6			GPR_BASE_ADDR(6)
#define MCUPM_SRAM_GPR7			GPR_BASE_ADDR(7)
#define MCUPM_SRAM_GPR8			GPR_BASE_ADDR(8)
#define MCUPM_SRAM_GPR9			GPR_BASE_ADDR(9)
#define MCUPM_SRAM_GPR10		GPR_BASE_ADDR(10)
#define MCUPM_SRAM_GPR11		GPR_BASE_ADDR(11)
#define MCUPM_SRAM_GPR12		GPR_BASE_ADDR(12)
#define MCUPM_SRAM_GPR13		GPR_BASE_ADDR(13)
#define MCUPM_SRAM_GPR14		GPR_BASE_ADDR(14)
#define MCUPM_SRAM_GPR15		GPR_BASE_ADDR(15)
#define MCUPM_SRAM_GPR16		GPR_BASE_ADDR(16)

#define SRAM_REINIT_BASE                0x1031FB40
#define SRAM_CPUDVFS_INIT_PTBL          SRAM_REINIT_BASE
#define SRAM_CPUDVFS_INIT_VOLT_E        (SRAM_REINIT_BASE + 48)

#define ROUNDUP(a,b)		(((a) + ((b)-1)) & ~((b)-1))

struct mcupm_info_t {
    unsigned int mcupm_dm_ofs; /* for mcupm data memory */
    unsigned int mcupm_dm_sz;
    unsigned int rd_ofs;      /* for mcupm ramdump */
    unsigned int rd_sz;
    unsigned int xfile_addr;  /* for LKdump */
    unsigned int xfile_sz;
};

#endif /* MCUPM_H */
