/*******************************************************************************
* Filename:
* ---------
*  sec_policy_wrapper2.c
*
* Project:
* --------
*  Download Agent
*
* Description:
* ------------
*  security policy wrapper for DRAM DA.
*
* Author:
* -------
*  Pangyen Chen (mtk02464)

*******************************************************************************/
#include "stdint.h"
#include "boot/transfer.h"
#include "error_code.h"
#include "assert.h"
#include <boot/error_message.h>
#include <boot/hal.h>
/////////////////////////EXTERNAL FUNCTION START////////////////////////////////
extern unsigned int is_all_partition_writable(unsigned int *write_all_perm);
extern unsigned int get_policy_entry_idx(unsigned char *part_name);
extern unsigned int get_hash_binding_policy(unsigned int policy_entry_idx);
extern unsigned int get_vfy_policy(unsigned int policy_entry_idx);
extern unsigned int get_dl_policy(unsigned int policy_entry_idx);
extern unsigned char *get_binding_hash(unsigned int policy_entry_idx);
extern void get_image_policy(unsigned char *partition_name,
                             struct transfer_control *op_ctrl);

extern int cust_pubk_init(void);
extern unsigned int is_security_enabled(unsigned int *enable);
extern unsigned int cust_init(void) __attribute__((weak));


/////////////////////////EXTERNAL FUNCTION END////////////////////////////////

unsigned int is_security_enabled(unsigned int *enable) {
  unsigned int ret = 0;
#if (DA_ENABLE_SECURITY)
  *enable = 1;
#else
  *enable = 0;
#endif
  return ret;
}

unsigned int is_write_all_allowed(unsigned int *perm) {
  unsigned int ret = 0;
  ret = is_all_partition_writable(perm);
  return ret;
}

/* although security is not related to transfer control */
/* DA owner chose to put security control into transfer control, so be it */
void get_part_sec_policy(uint8_t *partition_name,
                         struct transfer_control *op_ctrl) {
  unsigned int sec_policy_entry_idx = 0;

  if (op_ctrl == 0) return;
  sec_policy_entry_idx = get_policy_entry_idx(partition_name);

  op_ctrl->hash_binding = get_hash_binding_policy(sec_policy_entry_idx);
  op_ctrl->img_auth_required = get_vfy_policy(sec_policy_entry_idx);
  op_ctrl->dl_forbidden = get_dl_policy(sec_policy_entry_idx);
  op_ctrl->hash = get_binding_hash(sec_policy_entry_idx);
  op_ctrl->subimg_auth_bypass_list =
      get_image_auth_bypass_list(sec_policy_entry_idx);
  LOGD("==========security policy==========\n");
  LOGD("policy of partition: %s\n", partition_name);
  LOGD("hash_binding = %d\n", op_ctrl->hash_binding);
  if (op_ctrl->hash_binding) {
    unsigned int i;
    /* hash is sha256, so its size is 32 bytes */
    LOGD("hash = ");
    for (i = 0; i < 32; i++) {
      //LOGD("0x%x ", *(op_ctrl->hash + i));
      if (0 == ((i + 1) % 16))
        LOGD("\n");
    }
  }
  LOGD("img_auth_required = %d\n", op_ctrl->img_auth_required);
  LOGD("dl_forbidden = %d\n", op_ctrl->dl_forbidden);
  LOGD("===================================\n");

  return;
}

status_t security_check()
{
#if (DA_VER_CHECK_EN)
  status_t status = da_version_check();
  if (status) {
      LOGE("DA version is invalid.\n");
	  ASSERT(0);
  }
  return status;
#else
  LOGE("[SEC] Warrning! Bypass DA version check.\n");
  return 0;
#endif
}

status_t setup_security_env()
{
  unsigned int security_enable = 0;

  if (is_security_enabled(&security_enable)) {
    /* use safe setting is error occurs */
    security_enable = 1;
  }

  status_t status = STATUS_OK;
  if (security_enable) {    
    status = update_rom_info();
    if (status != STATUS_OK)
      LOGE("ROM INFO NOT FOUND...\n");
    else {
      status = sec_sej_key_init();
      if (status != STATUS_OK)
        LOGE("HACC init...fail\n");
    }

    if (cust_init) {
      status = cust_init();
      if (status != STATUS_OK)
        LOGE("custom init fail. 0x%x\n", status);
    }

    status = cust_pubk_init();
    if (status != STATUS_OK)
      LOGE("custom public key init fail.\n");
	status = security_check();
    LOGI("\n security pass.\n");
  } else {
    LOGI("\n skip security.\n");
  }

  return status;
}

#define FILE_EXT_MAX_SZ   (8)
#define VFY_FILE_EXT      "-verified"
#define SIG_FILE_EXT      ".sig"

char *sec_get_verified_image_name(char *name) {
  if (name == NULL || strnlen(name, MAX_FILE_NAME_LEN) >= MAX_FILE_NAME_LEN)
    return NULL;

  static char verified_name[MAX_FILE_NAME_LEN];
  memset(verified_name, 0, MAX_FILE_NAME_LEN);

  // find the last '.' in the name
  char *dot_pos = strrchr(name, '.');
  if (dot_pos == NULL ||
      strlen(name) - (dot_pos - name) > FILE_EXT_MAX_SZ ||       // file ext length must less than FILE_EXT_MAX_SZ
      dot_pos - name + sizeof(VFY_FILE_EXT) > MAX_FILE_NAME_LEN) // verified_name length must less than MAX_FILE_NAME_LEN
    return NULL;

  // copy the name before the last '.' to the verified_name
  memcpy(verified_name, name, dot_pos - name);
  // append the VFY_FILE_EXT to the verified_name
  strncat(verified_name, VFY_FILE_EXT, strlen(VFY_FILE_EXT));
  // copy the rest of the name to the verified_name
  strncat(verified_name, dot_pos, strlen(name) - (dot_pos - name));
  return verified_name;
}

char *sec_get_sig_image_name(char *name) {
  if (name == NULL || strnlen(name, MAX_FILE_NAME_LEN) >= MAX_FILE_NAME_LEN)
    return NULL;

  static char sig_name[MAX_FILE_NAME_LEN];
  memset(sig_name, 0, MAX_FILE_NAME_LEN);

  // find the last '.' in the name
  char *dot_pos = strrchr(name, '.');
  if (dot_pos == NULL ||
      strlen(name) - (dot_pos - name) > FILE_EXT_MAX_SZ ||       // file ext length must less than FILE_EXT_MAX_SZ
      dot_pos - name > MAX_FILE_NAME_LEN - sizeof(SIG_FILE_EXT)) // file name length must less than MAX_FILE_NAME_LEN - sizeof(SIG_FILE_EXT)
    return NULL;

  // copy the name before the last '.' to the sig_name
  memcpy(sig_name, name, dot_pos - name);
  // append the sig file extension to the sig_name
  strncat(sig_name, SIG_FILE_EXT, strlen(SIG_FILE_EXT));
  return sig_name;
}

