#include <sys/types.h>
#include <debug.h>
#include "low_power.h"


void Close_Useless_GPU(void)
{	
  // GPU_Core1_power_down();	//define at low_power.c
}

void Close_Useless_CPU(void)
{
  // close_cpus();			//define at low_power.c
}

void Close_Modem_1(void)
{

}
void Close_Modem_2(void)
{
//   internal_md2_power_down();
}

void Change_Settings(void)
{
    //enable_dcm();
}


