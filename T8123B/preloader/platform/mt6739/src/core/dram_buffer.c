#include "dram_buffer.h"
#include "typedefs.h"
#include "emi.h"

#define MOD "[Dram_Buffer]"

dram_buf_t* g_dram_buf = 0;

void init_dram_buffer(){
	u32 structure_size = sizeof(dram_buf_t);

	pal_log_debug("%s dram_buf_t size: 0x%x \n" ,MOD, structure_size);
	pal_log_debug("%s part_hdr_t size: 0x%x \n" ,MOD, sizeof(part_hdr_t));
	pal_log_debug("%s sizeof(boot_arg_t): 0x%x \n" ,MOD, sizeof(boot_arg_t));
	/*allocate dram_buf*/
#if defined(SLT)
    g_dram_buf = CFG_DRAM_ADDR + platform_memory_size() - (5*1024*1024); // Max DRAM size - 5MB
#elif defined(DUMMY_AP)
    g_dram_buf = CFG_DRAM_ADDR;
#else
    #if CFG_BYPASS_EMI
    g_dram_buf = CFG_DRAM_ADDR;
    #else
    g_dram_buf = (dram_buf_t *)(CFG_DRAM_ADDR + 0x4800000); //0x4480_0000 ~ 0x44A0_0000
    #endif
#endif
    // init boot argument
    memset((void *)&(g_dram_buf->bootarg), 0, sizeof(boot_arg_t));

	//make sure dram_buffer if move to the head of memory
	pal_log_info("%s g_dram_buf start addr: 0x%x \n" ,MOD, g_dram_buf);
	//make sure msdc_gpd_pool and msdc_bd_pool is 64 bytes alignment
	pal_log_info("%s g_dram_buf->msdc_gpd_pool start addr: 0x%x \n" ,MOD, g_dram_buf->msdc_gpd_pool);
	pal_log_info("%s g_dram_buf->msdc_bd_pool start addr: 0x%x \n" ,MOD, g_dram_buf->msdc_bd_pool);
}

