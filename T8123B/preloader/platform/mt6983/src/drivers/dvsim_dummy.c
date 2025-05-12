#include "dramc_common.h"
#include "dramc_int_global.h"
#if (FOR_DV_SIMULATION_USED == 1) && !defined(VCS)

void broadcast_on(void) {};
void broadcast_off(void) {};


u32 register_read_c(u32 addr)
{
    return 0;
}

void register_write_c(u32 addr, u32 value)
{

}

void delay_us(u32 delay)
{

}

void delay_ns(u32 delay)
{

}

void mygetscope()
{

}

void mysetscope()
{

}

void build_api_initial()
{

}


void timestamp_show(void)
{

}


void conf_to_sram_sudo(int ch_id , u8 group_id, int conf_id)
{

}

void LP5_DRAM_init_sudo(int ch_id, int rank, int reg_addr, int reg_op)
{

}

void DvSetTopDebugVif_WckDelayEN(int ch_id, u8 group_id, int EN, int byte_id)
{

}

void main(void)
{
	DPI_md32_init();
	DPI_DramcSwImpedanceCal();
	DPI_DRAMC_init_entry();
	DPI_SA_Init1();
	DPI_POST_DCM_setting();
	DPI_SA_Init2();
	DPI_SA_CalibrationAllFreq();
}

void DPI_entry_DDR3_AC_Timing_setting(int frequency, u8 group_id, int dramc_dram_ratio)
{}
void DPI_entry_DDR4_AC_Timing_setting(int frequency, u8 group_id, int dramc_dram_ratio)
{}

/* Defined in sv_to_c_api.sv */
void fsdb_dump_start()
{}

void set_curr_freq_group(int freq_group)
{}

#endif
