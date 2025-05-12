#include "boot/dev_interface/efuse_interface.h"
#include "boot/hal.h"
#include "debug.h"
#include "error_code.h"
#include "sec_efuse.h"

#if INTERFACE_EFUSE_IMPL


extern int da_efuse_blow(const char *efuse_img, unsigned int magic_key1,
                         unsigned int magic_key2);

int dev_efuse_write(const char *blow_data, int length) {
  int ret = 0;
  LOGI("\n Enter dev_efuse_write.\n");
  ret = da_efuse_blow(blow_data, EFUSE_BLOW_KEY1, EFUSE_BLOW_KEY2);
  LOGI("Exit dev_efuse_write.\n");
  return ret;
}

int dev_efuse_read(char *read_data, int length) {
  LOGI("\n Enter dev_efuse_read.==%d\n", length);
  da_efuse_read(read_data, length);
  LOGI("Exit dev_efuse_read.\n");
  return STATUS_OK;
}

#else

int dev_efuse_write(const char *blow_data, int length) { return STATUS_ERR; }
int dev_efuse_read(char *read_data, int length) { return STATUS_ERR; }

#endif
