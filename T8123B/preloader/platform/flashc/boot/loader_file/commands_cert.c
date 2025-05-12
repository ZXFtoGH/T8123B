#include "assert.h"
#include "boot/error_message.h"
#include "boot/hal.h"
#include "boot/loader_file/commands_preloader.h"
#include "boot/storage_access.h"
#include "error_code.h"

#include "boot/loader_file/sec_loader.h"

status_t write_cert(char *data, uint32_t length,
                    struct partition_info_struct *info) {
  status_t status = STATUS_OK;
  struct nand_param_struct nand;
  flash_dev_type_e fd;

  data_cache_t cert;
  cert.data = data;
  cert.length = length;

  data_cache_t hb;
  memset(&hb, 0, sizeof(data_cache_t));

  if ((sysob_mmc.type != STORAGE_NONE) || (sysob_ufs.type != STORAGE_NONE)) {
    fd = (sysob_mmc.type != STORAGE_NONE) ? FLASH_DEV_EMMC : FLASH_DEV_UFS;
    status = sec_get_hb_size(fd, &cert, &hb.length);
  } else if (sysob_nand.type != STORAGE_NONE) {
    nand.base_addr = 0;
    nand.block_size = sysob_nand.block_size;
    nand.page_size = sysob_nand.page_size;
    nand.page_parity_size = sysob_nand.page_parity_size;
    nand.total_size = sysob_nand.total_size;
    // make sure sysob_nand.type define sequence is the same as nand.type
    nand.type = ((int)sysob_nand.type - (int)STORAGE_NAND_SLC);

    hb.length = sec_nand_get_hb_size(&nand);
  } else {
    set_error_msg("No storage is found.");
    return STATUS_ERR;
  }

  if (FAIL(status)) {
    set_error_msg("cert file invalid.");
    return status;
  }

  hb.data = (char *)malloc(hb.length);
  if (hb.data == NULL) {
    set_error_msg("Cannot alloc 0x%x.", hb.length);
    return STATUS_ERR;
  }

  status = storage_interface.read(0, hb.data, hb.length, info);

  if (FAIL(status)) {
    set_error_msg("cert file invalid.");
    free(hb.data);
    return status;
  }

  uint32_t cert_offset = 0;
  if ((sysob_mmc.type != STORAGE_NONE) || (sysob_ufs.type != STORAGE_NONE)) {
    status = sec_get_updated_cert_hb(fd, &hb, &cert, &cert_offset);
  } else if (sysob_nand.type != STORAGE_NONE) {
    status = sec_nand_get_updated_cert_hb(&hb, &cert, &cert_offset, &nand);
  }

  if (FAIL(status)) {
    set_error_msg("cert file is invalid.");
    free(hb.data);
    return status;
  }

  status = storage_interface.write(0, hb.data, hb.length, info);
  if (FAIL(status)) {
    set_error_msg("write cert file header fail.");
    free(hb.data);
    return status;
  }
  status = storage_interface.write(cert_offset, cert.data, cert.length, info);
  if (FAIL(status)) {
    set_error_msg("write cert file fail.");
  }
  free(hb.data);
  return status;
}
