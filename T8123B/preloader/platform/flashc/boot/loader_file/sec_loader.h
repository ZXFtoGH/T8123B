#pragma once
#include <stdint.h>
/********************
 *Exported API
 *********************/
typedef struct data_cache_t {
  char *data;
  uint32_t length;
}data_cache_t;

typedef enum nand_dev_type_e {
  NAND_DEV_SLC = 0,
  NAND_DEV_MLC,
  NAND_DEV_TLC,
  NAND_DEV_AMLC,
  NAND_DEV_SPI,
  NAND_DEV_3DMLC,
}nand_dev_type_e;

typedef enum flash_dev_type_e {
  FLASH_DEV_EMMC = 0,
  FLASH_DEV_UFS,
  FLASH_DEV_NAND,
  FLASH_DEV_COMBO,
  FLASH_DEV_NOR,
}flash_dev_type_e;


typedef struct nand_param_struct {
  uint32_t type;
  uint32_t page_size;
  uint32_t block_size;  
  uint32_t page_parity_size;
  uint64_t total_size;
  uint64_t base_addr;
}nand_param_struct;

uint32_t sec_skip_pl_bin_hb_size(const char *pl);
int sec_pl_bin_already_has_hb(const data_cache_t *pl);
status_t sec_get_hb_size(flash_dev_type_e fd, const data_cache_t *pl, uint32_t *hb_len);
status_t sec_create_loader_hb(flash_dev_type_e fd, const data_cache_t *pl, data_cache_t *hb);
status_t sec_get_updated_cert_hb(flash_dev_type_e fd, data_cache_t *hb, const data_cache_t *cert,
                                 uint32_t *cert_offset);

uint32_t sec_nand_get_hb_size(struct nand_param_struct *nand_param);
status_t sec_nand_create_loader_hb(const data_cache_t *pl, data_cache_t *hb,
                                   struct nand_param_struct *nand_param);

status_t sec_nand_get_updated_cert_hb(data_cache_t *hb, const data_cache_t *cert,
                                 uint32_t *cert_offset, struct nand_param_struct *nand_param);


/********************
 *Exported API END
 *********************/
 

