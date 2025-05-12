#ETT_Everest/inc/common.h	-> //printf (LOG_LEVEL>=1)
#dramc_pi_api.h 			-> define EVEREST_MEM_C_TEST_CODE
	#dramc_common.h 			-> //delay_a_while()
	#hal_io.c					-> //dsb()
	#dram_register.h			-> DRAMC_WBR,CLK_CFG_0, CLK_CFG_UPDATE
	#							-> register access
rm test
gcc -o test dramc_pi_basic_api.c dramc_pi_calibration_api.c dramc_pi_main.c Hal_io.c -Iinc -I../../../inc -I../../../inc/MT6755 -I../../../ett/ett/inc -I../../../arch/inc -I../../../arch/inc/MT6755 -D__ETT__ -DDDR_INIT_TIME_PROFILING
