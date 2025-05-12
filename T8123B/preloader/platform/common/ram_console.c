#include <pal_log.h>
#include <string.h>
#include "dram_buffer.h"
#include "ram_console.h"
#include "wdt_v2.h"

#define RAM_CONSOLE_SIG (0x43474244) /* DBGC */
#define MOD "RAM_CONSOLE"


struct ram_console_buffer {
	u32 sig;
	/* for size comptible */
	u32 off_pl;
	u32 off_lpl; /* last preloader */
	u32 sz_pl;
	u32 off_lk;
	u32 off_llk; /* last lk */
	u32 sz_lk;
	u32 padding[4]; /* size = 4 * 16 = 64 byte */
	u32 off_linux;
	u32 off_console;
	u32 padding2[3];
};

struct reboot_reason_pl {
	u32 wdt_status;
	u32 last_func[RAM_CONSOLE_PL_SIZE];
};

struct reboot_reason_kernel {
	u32 fiq_step;
	u32 exp_type; /* 0xaeedeadX: X=1 (HWT), X=2 (KE), X=3 (nested panic) */
	u32 others[0];
};

static struct ram_console_buffer *ram_console = NULL;
static int ram_console_size;
/* AEE exception info */
static unsigned char aee_exp_type;
static unsigned int aee_wdt_status;

#if CFG_AEE_LK_MEMADDR
#define RC_UNINIT_RGU_VALUE 0xaeedead
static struct reboot_reason_pl rr_pl_tmp = { /* recored data before ram_console init done */
	/* indicate that ram_console_is_abnormal_boot() is avaliable */
	.wdt_status = RC_UNINIT_RGU_VALUE
};
#endif

#define ALIGN(x, size) ((x + size - 1) & ~(size - 1))

static void ram_console_fatal(const char *str)
{
	print("%s FATAL:%s\n", MOD, str);
	while(1)
		;
}

#define bootarg g_dram_buf->bootarg
void ram_console_mblock_reserve(void)
{
	u64 addr_res;

	addr_res = mblock_alloc(RAM_CONSOLE_DRAM_SIZE, 0x10000, (RAM_CONSOLE_DRAM_ADDR + RAM_CONSOLE_DRAM_SIZE), RAM_CONSOLE_DRAM_ADDR, 0, "aee_debug_kinfo");
	if (addr_res != RAM_CONSOLE_DRAM_ADDR)
		ram_console_fatal("reserve dram memory failed");
	addr_res = mblock_alloc(PSTORE_SIZE, 0x10000, (PSTORE_ADDR + PSTORE_SIZE), PSTORE_ADDR, 0, "pstore");
	if (addr_res != PSTORE_ADDR)
		ram_console_fatal("reserve pstore memory failed");
	addr_res = mblock_alloc(MRDUMP_MINI_HEADER_SIZE, 0x10000, (MRDUMP_MINI_HEADER_ADDR + MRDUMP_MINI_HEADER_SIZE), MRDUMP_MINI_HEADER_ADDR, 0, "minirdump");
	if (addr_res != MRDUMP_MINI_HEADER_ADDR)
		ram_console_fatal("reserve minirdump memory failed");
}

#ifndef RE_BOOT_BY_WDT_SW
#define RE_BOOT_BY_WDT_SW 2
#endif
#define RE_BOOT_NORMAL_BOOT 0
#define RE_BOOT_FULL_PMIC 0x800

void ram_console_init(void)
{
	int i;
	struct reboot_reason_pl *rr_pl;
	ram_console = (struct ram_console_buffer *)RAM_CONSOLE_SRAM_ADDR;
	u32 def_type;

	def_type = ram_console_def_memory();
	if (def_type == RAM_CONSOLE_DEF_SRAM) {
		pal_log_info("%s using SRAM\n", MOD);
		ram_console = (struct ram_console_buffer *)RAM_CONSOLE_SRAM_ADDR;
		ram_console_size = RAM_CONSOLE_SRAM_SIZE;
	} else if (def_type == RAM_CONSOLE_DEF_DRAM) {
		/*
		 * Support dram only if we can get exp_type from non-reset register instand of
		 * DRAM or SRAM.
		 * Ram_console can't do init before the DRAM memory init finished, and, it is too
		 * late for the users of ram_console_is_abnormal_boot(), which have to get
		 * exp_type from the mapped memory.
		 */
		pal_log_info("%s using DRAM\n", MOD);
		ram_console = (struct ram_console_buffer *)RAM_CONSOLE_ADDR;
		ram_console_size = RAM_CONSOLE_SIZE;
	} else {
		pal_log_err("%s def type:%d\n", MOD, def_type);
		ram_console_fatal("[pl] unknown def type");
	}

	pal_log_info("%s start: 0x%x, size: 0x%x, sig: 0x%x\n", MOD, ram_console, ram_console_size, ram_console->sig);
	if (ram_console->sig == RAM_CONSOLE_SIG &&
		ram_console->sz_pl == sizeof(struct reboot_reason_pl) &&
		ram_console->off_pl + ALIGN(ram_console->sz_pl, 64) == ram_console->off_lpl
#if CFG_AEE_LK_MEMADDR
		&& !((g_rgu_status == RE_BOOT_FULL_PMIC) && (mtk_wdt_get_aee_rsv() == 0))
#endif
#if defined(CFG_APWDT) && (CFG_APWDT == V2)
		&& !mtk_is_rst_from_da()
#endif
	) {
		pal_log_info("%s preloader last status: ", MOD);
		rr_pl = (void*)ram_console + ram_console->off_pl;
		for (i = 0; i < RAM_CONSOLE_PL_SIZE; i++) {
			pal_log_info("0x%x ", rr_pl->last_func[i]);
		}
		pal_log_info("\n");
		memcpy((void *)ram_console + ram_console->off_lpl, (void *)ram_console + ram_console->off_pl, ram_console->sz_pl);
	} else {
		memset(ram_console, 0, ram_console_size);
		ram_console->sig = RAM_CONSOLE_SIG;
		ram_console->off_pl = sizeof(struct ram_console_buffer);
		ram_console->sz_pl = sizeof(struct reboot_reason_pl);
		ram_console->off_lpl = ram_console->off_pl + ALIGN(ram_console->sz_pl, 64);
		pal_log_info("%s init done\n", MOD);
	}
#if CFG_AEE_LK_MEMADDR
	/* sync temp data to ram_console memory */
	memcpy((void *)ram_console + ram_console->off_pl, (void *)&rr_pl_tmp,
		sizeof(struct reboot_reason_pl));
#endif
}

#define AEE_ENABLE_NO 0
#define AEE_ENABLE_MINI 1
#define AEE_ENABLE_FULL 2

int aee_check_enable(void)
{
#if MTK_AEE_LEVEL == 1
	int val = AEE_ENABLE_MINI;
#if MTK_DOE_CONFIG_ENV_SUPPORT
	const char *params = dconfig_getenv("aee_enable");

	if (params != NULL) {
		if (strcmp(params, "mini") == 0) {
			val = AEE_ENABLE_MINI;
		} else if (strcmp(params, "full") == 0) {
			val = AEE_ENABLE_FULL;
		} else if (strcmp(params, "no") == 0) {
			val = AEE_ENABLE_NO;
		}
	}
#endif
	print("aee enable %d\n", val);
	return val;
#elif MTK_AEE_LEVEL == 2
	return AEE_ENABLE_FULL;
#else
#error "Unknown MTK_AEE_LEVEL value"
#endif
}


void aee_exp_info_get(u32 *exp_type, u32 *wdt_status)
{
	if (exp_type != NULL)
		*exp_type = (u32)aee_exp_type;
	if (wdt_status != NULL)
		*wdt_status = aee_wdt_status;
}

#if CFG_AEE_LK_MEMADDR
void ram_console_reboot_reason_save(u32 rgu_status)
{
	struct reboot_reason_pl *rr_pl;

	rr_pl_tmp.wdt_status = rgu_status;
	pal_log_info("%s wdt status 0x%x\n", MOD, rgu_status);

	if (ram_console) {
		rr_pl = (void*)ram_console + ram_console->off_pl;
		rr_pl->wdt_status = rgu_status;
	}
}

void ram_console_pl_save(u32 val, int index)
{
	struct reboot_reason_pl *rr_pl;
	if (ram_console) {
		rr_pl = (void*)ram_console + ram_console->off_pl;
		if (index < RAM_CONSOLE_PL_SIZE)
			rr_pl->last_func[index] = val;
		pal_log_info("%s pl save [%d]=0x%x\n", MOD, index, val);
	} else {
		if (index < RAM_CONSOLE_PL_SIZE)
			rr_pl_tmp.last_func[index] = val;
		pal_log_info("%s pl save tmp [%d]=0x%x\n", MOD, index, val);
	}
}

bool ram_console_is_abnormal_boot(void)
{
	unsigned int wdt_status = 0;

	aee_exp_type = mtk_wdt_get_aee_rsv();
	aee_wdt_status = g_rgu_status;

	if (!aee_check_enable())
		return false;

	if (rr_pl_tmp.wdt_status == RC_UNINIT_RGU_VALUE)
		pal_log_info("%s WARN: calling %s before rgu status is ready\n", MOD, __func__);

	wdt_status = g_rgu_status;
	if (wdt_status != RE_BOOT_NORMAL_BOOT && wdt_status != RE_BOOT_BY_WDT_SW && wdt_status != RE_BOOT_FULL_PMIC) {
		pal_log_info("%s detect abnormal boot wdt_status:0x%x, aee_exp_type:%u\n", MOD, wdt_status, aee_exp_type);
		return true;
	}

	if (aee_exp_type) {
		pal_log_info("%s detect abnormal boot aee_exp_type:%u\n", MOD, aee_exp_type);
		return true;
	}

	return false;
}
#else
void ram_console_reboot_reason_save(u32 rgu_status)
{
	struct reboot_reason_pl *rr_pl;
	if (ram_console) {
		rr_pl = (void*)ram_console + ram_console->off_pl;
		rr_pl->wdt_status = rgu_status;
#if defined(CFG_APWDT) && (CFG_APWDT == V2)
		if (mtk_is_rst_from_da())
			rr_pl->wdt_status = 0;
#endif
		pal_log_info("%s wdt status (0x%x)=0x%x\n", MOD,
			rr_pl->wdt_status, rgu_status);
	}
}

void ram_console_pl_save(u32 val, int index)
{
	struct reboot_reason_pl *rr_pl;
	if (ram_console) {
		rr_pl = (void*)ram_console + ram_console->off_pl;
		if (index < RAM_CONSOLE_PL_SIZE)
			rr_pl->last_func[index] = val;
	}
}

bool ram_console_is_abnormal_boot(void)
{
	unsigned int fiq_step, wdt_status, exp_type;

	if (!aee_check_enable())
		return false;

	if (!ram_console) {
		ram_console_init();
	}
	wdt_status = ((struct reboot_reason_pl*)((void*)ram_console + ram_console->off_pl))->wdt_status;
	if (wdt_status != RE_BOOT_NORMAL_BOOT && wdt_status != RE_BOOT_BY_WDT_SW && wdt_status != RE_BOOT_FULL_PMIC) {
		print("%s detect abnormal boot wdt_status:0x%x\n", MOD, wdt_status);
		return true;
	}
	if (ram_console && ram_console->off_linux && ram_console->off_linux < ram_console_size && ram_console->off_pl < ram_console_size) {
		fiq_step = ((struct reboot_reason_kernel*)((void*)ram_console + ram_console->off_linux))->fiq_step;
		exp_type = ((struct reboot_reason_kernel*)((void*)ram_console + ram_console->off_linux))->exp_type;
		if (((exp_type ^ 0xaeedead0) > 0) && ((exp_type ^ 0xaeedead0) < 16)) {
			print("%s detect abnormal boot exp_type:0x%x\n", MOD, exp_type ^ 0xaeedead0);
			return true;
		}
		print("%s wdt_status 0x%x, fiq_step 0x%x, exp_type 0x%x\n", MOD, wdt_status, fiq_step, (exp_type ^ 0xaeedead0) < 16 ? exp_type ^ 0xaeedead0 : exp_type);
		if (fiq_step != 0 && (exp_type ^ 0xaeedead0) >= 16) {
			fiq_step = 0;
			((struct reboot_reason_kernel*)((void*)ram_console + ram_console->off_linux))->fiq_step = fiq_step;
		}
		if ((fiq_step == 0 && (wdt_status == RE_BOOT_BY_WDT_SW || wdt_status == RE_BOOT_FULL_PMIC)) || (wdt_status == RE_BOOT_NORMAL_BOOT))
			return false;
		else
			return true;
	}
	return false;
}
#endif

u32 ram_console_def_memory(void)
{
	if (RAM_CONSOLE_DEF_ADDR == RAM_CONSOLE_SRAM_ADDR)
		return RAM_CONSOLE_DEF_SRAM;
	else if (RAM_CONSOLE_DEF_ADDR == RAM_CONSOLE_DRAM_ADDR)
		return RAM_CONSOLE_DEF_DRAM;
	else
		return RAM_CONSOLE_DEF_UNKNOWN;
}

void ram_console_sram_addr_size(u32 *addr, u32 *size)
{
	if (addr != NULL)
		*addr = RAM_CONSOLE_SRAM_ADDR;
	if (size != NULL)
		*size = RAM_CONSOLE_SRAM_SIZE;
}

#define SHOW_ARGS(p, a, b, c, d) \
	print("RAM_CONSOLE " #a":0x%x, "#b":0x%x, "#c":0x%x, "#d":0x%x\n", (p)->a, (p)->b, (p)->c, (p)->d)
static void ram_console_memory_info_show(struct ram_console_memory_info *memory_info)
{
	if (memory_info == NULL) {
		print("%s. %s args null\n", MOD, __func__);
		return;
	}

	SHOW_ARGS(memory_info, sram_plat_dbg_info_addr, sram_plat_dbg_info_size, sram_log_store_addr, sram_log_store_size);
	SHOW_ARGS(memory_info, mrdump_addr, mrdump_size, dram_addr, dram_size);
	SHOW_ARGS(memory_info, pstore_addr, pstore_size, pstore_console_size, pstore_pmsg_size);
	SHOW_ARGS(memory_info, mrdump_mini_header_addr, mrdump_mini_header_size, magic1, magic2);
}

u32 ram_console_memory_info_offset(void)
{
#ifdef RAM_CONSOLE_SRAM_SIZE_OVERALL
	struct ram_console_memory_info *memory_info;

	u32 offset = RAM_CONSOLE_MEMORY_INFO_OFFSET;
	print("%s offset:0x%x\n", MOD, offset);

	if (offset <= 0 || offset >= RAM_CONSOLE_SRAM_SIZE_OVERALL) {
		print("%s. illegal offset\n", MOD);
		return 0;
	}

	memory_info = (struct ram_console_memory_info *)(RAM_CONSOLE_SRAM_ADDR + offset);
	memset(memory_info, 0, sizeof(struct ram_console_memory_info));
	memory_info->magic1 = MEM_MAGIC1;
	memory_info->sram_plat_dbg_info_addr = RAM_CONSOLE_PLAT_DBG_INFO_ADDR;
	memory_info->sram_plat_dbg_info_size = RAM_CONSOLE_PLAT_DBG_INFO_SIZE;
	memory_info->sram_log_store_addr = RAM_CONSOLE_LOG_STORE_ADDR;
	memory_info->sram_log_store_size = RAM_CONSOLE_LOG_STORE_SIZE;
	memory_info->mrdump_addr = RAM_CONSOLE_MRDUMP_ADDR;
	memory_info->mrdump_size = RAM_CONSOLE_MRDUMP_SIZE;
	memory_info->dram_addr = RAM_CONSOLE_DRAM_ADDR;
	memory_info->dram_size = RAM_CONSOLE_DRAM_SIZE;
	memory_info->pstore_addr = PSTORE_ADDR;
	memory_info->pstore_size = PSTORE_SIZE;
	memory_info->pstore_console_size = PSTORE_CONSOLE_SIZE;
	memory_info->pstore_pmsg_size = PSTORE_PMSG_SIZE;
	memory_info->mrdump_mini_header_addr = MRDUMP_MINI_HEADER_ADDR;
	memory_info->mrdump_mini_header_size = MRDUMP_MINI_HEADER_SIZE;
	memory_info->magic2 = MEM_MAGIC2;

	ram_console_memory_info_show(memory_info);

	return offset;
#else
	return 0;
#endif
}
