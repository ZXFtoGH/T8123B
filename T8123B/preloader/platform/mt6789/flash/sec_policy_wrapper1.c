/*******************************************************************************
* Filename:
* ---------
*  sec_policy_wrapper1.c
*
* Project:
* --------
*  Download Agent
*
* Description:
* ------------
*  security policy wrapper for SRAM DA, which discards security functions.
*
* Author:
* -------
*  Pangyen Chen (mtk02464)

*******************************************************************************/
#include <boot/hal.h>
#include "boot/transfer.h"
#include <debug.h>
#include "error_code.h"

unsigned int is_security_enabled(unsigned int *enable)
{
    unsigned int ret = 0;
#if (DA_ENABLE_SECURITY == 1)
    *enable = 1;
#else
    *enable = 0;
#endif
    return ret;
}

unsigned int is_write_all_allowed(unsigned int *perm)
{
   /* use the most strict policy */
   unsigned int ret = 0;
   *perm = 0;
   return ret;
}

/* although security is not related to transfer control */
/* DA owner chose to put security control into transfer control, so be it */
void get_part_sec_policy(uint8_t *partition_name, struct transfer_control *op_ctrl)
{
   /* ignore partition name */
   /* use the most strict policy */
   op_ctrl->hash_binding = 0;
   op_ctrl->img_auth_required = 0;
   op_ctrl->dl_forbidden = 1;
   op_ctrl->hash = NULL;
   op_ctrl->subimg_auth_bypass_list = NULL;

   return;
}

char *sec_get_verified_image_name(char *name){
   return name;
}
char *sec_get_sig_image_name(char *name){
  return NULL;
}



