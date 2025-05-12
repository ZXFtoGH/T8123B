#include "dram_buffer.h"
#include "typedefs.h"
#include "emi.h"
#include "memory_layout.h"

#define MOD "[Dram_Buffer]"

#if defined(SLT)
#define DRAM_4GB_SIZE    0x100000000
#endif

dram_buf_t* g_dram_buf = 0;
boot_tag *boottag = 0; /*boot args pass to LK by ATAG*/

void init_dram_buffer(void){
	u32 dram_buf_size = sizeof(dram_buf_t);

	pal_log_debug("%s dram_buf_t size: 0x%x \n" ,MOD, dram_buf_size);
	if (dram_buf_size > PL_DRAMBUF_MAX_SIZE) {
		pal_log_debug("\n%s dram_buffer oversize!\n", MOD);
		ASSERT(0);
	}

	pal_log_debug("%s part_hdr_t size: 0x%x \n" ,MOD, sizeof(part_hdr_t));
	pal_log_debug("%s sizeof(boot_arg_t): 0x%x \n" ,MOD, sizeof(boot_arg_t));
	/*allocate dram_buf*/
#if defined(SLT)
    if ((u64)CFG_DRAM_ADDR + platform_memory_size() > (u64)DRAM_4GB_SIZE)  // DRAM size > 4GB
        g_dram_buf = DRAM_4GB_SIZE - (5*1024*1024);
    else
        g_dram_buf = CFG_DRAM_ADDR + platform_memory_size() - (5*1024*1024); // Max DRAM size - 5MB
    boottag = g_dram_buf + dram_buf_size;
#elif defined(DUMMY_AP)
    g_dram_buf = CFG_DRAM_ADDR;
    boottag = g_dram_buf + dram_buf_size;
#else
    #if CFG_BYPASS_EMI
    g_dram_buf = CFG_DRAM_ADDR;
    boottag = g_dram_buf + dram_buf_size;
    #else
    g_dram_buf = PL_DRAMBUF_BASE;
    boottag = PL_BOOTTAGS_BASE;
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

