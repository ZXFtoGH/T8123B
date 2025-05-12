#include "boot/sparse/sparse_format.h"
#include "boot/sparse/sparse_state_machine.h"
#include "debug.h"
#include "error_code.h"

void init_unsparse_status(unsparse_status_t *status, uint64_t image_base_addr,
                          uint32_t checksum_level, partition_info_struct_t *arg,
                          struct space_map_cell_t *dynamic_vm_map) {}

void end_write_sparse_data(unsparse_status_t *status) {}

void write_sparse_data(unsparse_status_t *status, uint8_t *data,
                       uint32_t length) {
  status->handle_status = STATUS_OK;
  return;
}

inline BOOL is_sparse_image(uint8_t *data, uint32_t length) {
  sparse_header_t *sparse_header = (sparse_header_t *)data;
  return (sparse_header->magic == SPARSE_HEADER_MAGIC);
}

inline uint64_t unspared_size(uint8_t *data) { return 0; }

/*Sparse Image is not supported in first DA*/
inline BOOL support_sparse_image(void) { return false; }
