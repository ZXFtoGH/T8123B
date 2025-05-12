#include "boot/partition/gpt_gen.h"
#include "error_code.h"
#include "boot/error_message.h"

#include "boot/hal.h"
#include "boot/partition/hw_depends.h"
#include "common_struct.h"
#include "pmt.h"

status_t pack_partition_data(partition_table_type type,
                             const struct sys_scatter_struct *scatter,
                             uint32_t signature, char *data, uint32_t length) {
  status_t status = STATUS_OK;

  // get image list
  uint32_t img_idx;

  uint32_t nr_images = scatter->tbl.count;
  // pt list
  pt_resident* pt_list = (pt_resident*)(data + PT_SIG_SIZE + PT_TLCRATIO_SIZE);

  memcpy(data, &signature, PT_SIG_SIZE);

  // slc percent
  uint32_t percent = 0; //  get_percent(); // to be implemented
  memcpy(data + PT_SIG_SIZE, &percent, PT_TLCRATIO_SIZE);

  // Partitions Infomation
  for (img_idx = 0; img_idx < nr_images; img_idx++) {
    memcpy(pt_list[img_idx].name, scatter->tbl.info[img_idx].name,
           MAX_PARTITION_NAME_LEN);
    pt_list[img_idx].offset = scatter->tbl.info[img_idx].base_addr;
    pt_list[img_idx].ext.region =
        (unsigned char)scatter->tbl.info[img_idx].section;
      pt_list[img_idx].size = scatter->tbl.info[img_idx].max_size;
    if (scatter->aux_tbl.info[img_idx].ncu == CELL_UNI) // for NAND only
    {
      pt_list[img_idx].ext.type = REGION_LOW_PAGE;
    } else if (scatter->aux_tbl.info[img_idx].ncu == CELL_MULTI) {
      pt_list[img_idx].ext.type = REGION_FULL_PAGE;
    }
  }

  // pt info
  pt_info pi;
  memset(&pi, 0, sizeof(pt_info));
  if (type == PARTITION_MIRROR) {
    pi.pt_changed = 1;
    pi.sequencenumber += 1;
    /* Indicate this pt is tool update */
    pi.tool_or_sd_update = 3;
  }

  memcpy(data + PT_SIG_SIZE + PT_TLCRATIO_SIZE +
             nr_images * sizeof(pt_resident),
         (char *)&pi, PT_SIG_SIZE);
  return status;
}

status_t generate_pmt(enum partition_table_type type,
                      /*IN*/ const struct sys_scatter_struct *scatter,
                      char **data, int *length) {
  status_t status = STATUS_OK;
  if (data == 0 || length == 0)
    return STATUS_ERR;

  uint32_t page_size = sysob_nand.page_size;
  uint32_t signature = 0;

  // init
  if (type == PARTITION_MAJOR) {
    *length = LPAGE;
    *data = malloc(*length);
    if (*data == 0) return STATUS_MEM_ALLOC_FAIL;
    signature = PT_SIG;
  } else if (type == PARTITION_MIRROR) {
    *length = page_size;
    *data = malloc(*length);
    if (*data == 0) return STATUS_MEM_ALLOC_FAIL;
    signature = MPT_SIG; // mirror PT
  }

  status = pack_partition_data(type, scatter, signature, *data, *length);

  return status;
}
