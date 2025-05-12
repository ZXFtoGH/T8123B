#include "boot/handler.h"
#include "boot/hal.h"
#include "error_code.h"
#include "common_struct.h"
#include "boot/arguments.h"
#include "boot/system_objects.h"
#include "debug.h"


int bootstrap2(void *arg)
{

   LOGI("\n***8.Enter bootstrap2***\n\n");
 
   //platform init can code here. storage init
   sysob_runtime_params.hw_init_status = platform_init_da(); 
   LOGI("\n***9.platform_init pass***\n");

   platform_establish_env();
   LOGI("\n***10.dagent_register_commands.\n");
   //register function commands.
   dagent_register_commands();

   //enter main command loop. never returned.
   dagent_handler();

   return 0;
}

