#include "checksum.h"
#include "debug.h"
#include "arch/thread.h"
#include "kernel/synchronic_api.h"

extern void arch_disable_cache(uint flags);
extern void arch_enable_cache(uint flags);
extern void arch_clean_invalidate_cache_range(addr_t start, size_t len);
extern uint32_t checksum_plain_impl(uint8_t* data, uint32_t length);

#ifdef DUMP_CHKSM_ERRER
void dump_buf(uint8_t* left_addr, uint8_t* right_addr, uint64_t emmc_addr, uint32_t data_length)
{	
	int index =0;
	int numj = 0;
	int begin = 0;
	int end = 0;
	int dump_length = 1024;
	LOGI("buf length 0x%x, emmc addr begin with 0x%llx, buffer dram addr begin with 0x%p, buf dram addr after storage read begin with 0x%p\n", data_length, emmc_addr, left_addr, right_addr);
	LOGI("dump buf left is before storage write, right is after storage read\n");
		for(index=0; index<data_length; index++)
			{
				if(*(left_addr+index)!= *(right_addr+index))
					{
						LOGE("left is 0x%x, right is 0x%x, error index is 0x%x\n", *(left_addr+index), *(right_addr+index), index);

						begin = 0;
						if(index > (dump_length))
							begin = index-dump_length;

						if((index +dump_length) < data_length)
							end = index+dump_length;
						else 
							end = data_length;
						LOGI("print the before and afterr 1024 bytes data\n ");
						for(; begin < end; begin++)
							{
							LOGI("left is 0x%x, right is 0x%x, index is 0x%x\n", *(left_addr+begin), *(right_addr+begin), begin);
							}
						}
			}
		return;
}
#endif

uint32_t checksum_plain(uint8_t* data, uint32_t length)
{ 
    return checksum_plain_impl(data, length);
}

uint32_t checksum_only(uint8_t* data, uint32_t length)
{
   return checksum_plain_impl(data, length);
}

