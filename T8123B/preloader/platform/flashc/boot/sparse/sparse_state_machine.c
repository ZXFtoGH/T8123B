#include "boot/sparse/dynamic_partition.h"
#include "boot/sparse/sparse_format.h"
#include "boot/sparse/sparse_state_machine.h"

#include "boot/error_message.h"
#include "boot/hal.h"
#include "boot/system_objects.h"
#include "boot/transfer.h"
#include "error_code.h"

#include "boot/checksum/checksum.h"
#include "boot/checksum/checksum_info_manager.h"
#include "boot/checksum/storage_checksum.h"

// only engine data structure.
static unsparse_data_t m_unsparse_data;

static void write_sparse_data_internal(unsparse_status_t *status);
static void write_this_block(unsparse_status_t *status, uint64_t offset,
                             uint8_t *buf, uint64_t length);

static void set_unsparse_status(unsparse_status_t *status,
                                status_t handle_status,
                                unsparse_phase_t wait_phase, uint32_t size,
                                uint8_t *buf) {
  status->handle_status = handle_status;
  status->wait_phase = wait_phase;
  status->byte_to_process = size;
  status->buf = buf;
}

// tell NAND driver chunk cell usage: TLC or SLC
BOOL in_sparse_state = false;
uint16_t get_sparse_hdr_data_mode(void) {
  if (in_sparse_state)
    return m_unsparse_data.chunk_hdr.data_mode;
  return 0xFF;
}
/////////////////////////////////////////

void init_unsparse_status(unsparse_status_t *status, uint64_t image_base_addr,
                          uint32_t checksum_level, partition_info_struct_t *arg,
                          struct space_map_cell_t *dynamic_vm_map) {
  memset(status, 0, sizeof(unsparse_status_t));
  set_unsparse_status(status, STATUS_OK, UNSPARSE_WAIT_SPARSE_HEADER, 0, 0);
  status->checksum_of_flash = 0;
  status->image_base_addr = image_base_addr;
  status->checksum_level = checksum_level;
  status->arg = arg;
  if (dynamic_vm_map == NULL) {
    struct address_space_t vas;
    vas.start = 0;
    vas.length = arg->max_size;
    status->dynamic_vm_map = flash_address_map_va_pa_identical(&vas);
  } else {
    status->dynamic_vm_map = dynamic_vm_map;
  }
  in_sparse_state = false;
}

void end_write_sparse_data(unsparse_status_t *status) {
  // for the last package
  if (sysob_nand.type != STORAGE_NONE) {
    LOGI("NAND solution, begin to handle last package checksum.\n");
    if (status->checksum_level & CHKSUM_LEVEL_STORAGE) {
      LOGD("last package chksum.\n");
      set_chksum_ready();
      status->handle_status = storage_checksum_examine(NULL, 0);
      if (FAIL(status->handle_status)) {
        LOGE("Storage checksum failed!\n");
      }
    }
  }
}
static void memsetint(void *p, int i, int len) {
  int idx = 0;
  int count = len >> 2;
  int *buf = (int *)p;
  while (idx < count) {
    buf[idx++] = i;
  }
}

#define MEM_16K (16 * 1024)

#define FILLED_DATA_CACHE_SIZE (MEM_16K * sizeof(int32_t))

#define FILL_BUF_CNT (64)

struct fill_buff_cell_t {
  int32_t data[FILLED_DATA_CACHE_SIZE / sizeof(int32_t)];
  uint32_t hit;
};

struct fill_buff_t {
  struct fill_buff_cell_t f[FILL_BUF_CNT];
};

static void write_this_block(unsparse_status_t *status, uint64_t offset,
                             uint8_t *buf, uint64_t length) {
  uint32_t pre_chksum = 0;
  uint32_t post_chksum = 0;
  uint32_t block_chksum = 0;

  struct address_space_t va_block = {0};
  struct address_space_t pa_block = {0};
  struct address_space_t va_block_next = {0};

  va_block_next.start = offset;
  va_block_next.length = length;

  while (va_block_next.length > 0) {
    va_block.start = va_block_next.start;
    va_block.length = va_block_next.length;
    status->handle_status = flash_address_va_to_pa(
        &va_block, &pa_block, &va_block_next, status->dynamic_vm_map);
    if (FAIL(status->handle_status)) {
      set_error_msg("Not mapped: VA:0x%llx Len:0x%llx", va_block.start,
           va_block.length);
      return;
    }
    // LOGI("VA[0x%llx]->PA[0x%llx] LenV[0x%llx]->LenP[0x%llx]\n",
    // va_block.start,
    //     pa_block.start, va_block.length, pa_block.length);
    status->handle_status =
        storage_interface.write(status->image_base_addr + pa_block.start, buf,
                                pa_block.length, status->arg);

    if (status->handle_status != STATUS_OK) {
      set_error_msg("S_STORAGE_WRITE_FAILED:%d, status=%d, size=%d\n",
           __LINE__, status->handle_status, status->byte_to_process);
      return;
    }

    if (status->checksum_level & CHKSUM_LEVEL_STORAGE) {
      pre_chksum = checksum_plain(buf, (uint32_t)pa_block.length);
      memset(buf, 0, pa_block.length);
      status->handle_status =
          storage_interface.read(status->image_base_addr + pa_block.start, buf,
                                 pa_block.length, status->arg);
      if (status->handle_status != STATUS_OK) {
        LOGE("S_STORAGE_READ_FAILED:%d, status=%d, size=%d\n",
             __LINE__, status->handle_status, status->byte_to_process);
        return;
      }

      post_chksum = checksum_plain(buf, (uint32_t)pa_block.length);

      status->checksum_of_flash += post_chksum;
      if (post_chksum != pre_chksum) {
        status->handle_status = STATUS_CHECKSUM_ERR;
        set_error_msg("S_STORAGE_CHECKSUM_ERR:%d, post_chksum=0x%x, "
             "pre_chksum=0x%x\n",
             __LINE__, post_chksum, pre_chksum);
        return;
      }
    }

    // in case this package was split to different PA.
    buf += pa_block.length;
  }

  return;
}

static void write_sparse_data_internal(unsparse_status_t *status) {
  uint32_t size = status->byte_to_process;
  uint8_t *buf = status->buf;
  uint32_t sizeOfX = 0;
  volatile uint8_t *pdump = 0;
  int filled_seed = 0;
  uint32_t excepted_checksum = 0;
  uint8_t *filled_data_cache = 0;
  static uint8_t *s_filled_data_cache = 0;
  int slot = 0;
  int less_use_idx = 0;
  bool found = false;
  uint32_t kk = 0;
  struct address_space_t va_block = {0};
  struct address_space_t pa_block = {0};
  struct address_space_t va_block_next = {0};
  if (size == 0) {
    return;
  }

  switch (status->wait_phase) {
  case UNSPARSE_WAIT_SPARSE_HEADER: {
    if (size >= sizeof(sparse_header_t)) {
      memset((void *)&m_unsparse_data, 0x00, sizeof(unsparse_data_t));
      memcpy((void *)&m_unsparse_data.sparse_hdr, buf, sizeof(sparse_header_t));

      // for real sparse header larger than sparse_header_t struct
      // for titan ext4_sparse_header_8byte_align:  sparse_header_t 8byte align,
      // chunk_header_t 8byte align

      size -= m_unsparse_data.sparse_hdr.file_hdr_sz;
      buf += m_unsparse_data.sparse_hdr.file_hdr_sz;

      m_unsparse_data.unhandle_buf_size = 0;
      m_unsparse_data.chunks_processed = 0;
      set_unsparse_status(status, STATUS_OK, UNSPARSE_WAIT_CHUNK_HEADER, size,
                          buf);
    }
    break;
  }

  case UNSPARSE_WAIT_CHUNK_HEADER: {
    if (m_unsparse_data.chunks_processed >=
        m_unsparse_data.sparse_hdr.total_chunks) {
      LOGI("#Reach the sparse image end. processed chunks[0x%x] Skip remain "
           "data.\n",
           m_unsparse_data.chunks_processed);
      set_unsparse_status(status, STATUS_OK, UNSPARSE_WAIT_CHUNK_HEADER, 0,
                          buf);
      return;
    }
    if (m_unsparse_data.unhandle_buf_size + size >=
        (m_unsparse_data.sparse_hdr.chunk_hdr_sz)) {
      if (m_unsparse_data.unhandle_buf_size > 0) {
        uint32_t sizeOfUsedBuf = m_unsparse_data.sparse_hdr.chunk_hdr_sz -
                                 m_unsparse_data.unhandle_buf_size;
        memcpy(&m_unsparse_data.chunk_hdr, m_unsparse_data.unhandle_buf,
               m_unsparse_data.unhandle_buf_size);
        // for real sparse header larger than sparse_header_t struct
        if ((m_unsparse_data.sparse_hdr.chunk_hdr_sz >
             sizeof(chunk_header_t))) {
          memcpy(((uint8_t *)&m_unsparse_data.chunk_hdr) +
                     m_unsparse_data.unhandle_buf_size,
                 buf,
                 sizeOfUsedBuf - (m_unsparse_data.sparse_hdr.chunk_hdr_sz -
                                  sizeof(chunk_header_t)));
        } else {
          memcpy(((uint8_t *)&m_unsparse_data.chunk_hdr) +
                     m_unsparse_data.unhandle_buf_size,
                 buf, sizeOfUsedBuf);
        }

        size -= sizeOfUsedBuf;
        buf += sizeOfUsedBuf;
        m_unsparse_data.unhandle_buf_size = 0;
      } else {
        // for real sparse header larger than sparse_header_t struct
        memcpy(&m_unsparse_data.chunk_hdr, buf, sizeof(chunk_header_t));
        size -= m_unsparse_data.sparse_hdr.chunk_hdr_sz;
        buf += m_unsparse_data.sparse_hdr.chunk_hdr_sz;
      }

      m_unsparse_data.chunk_remain_data_size =
          (uint64_t)m_unsparse_data.chunk_hdr.chunk_sz *
          m_unsparse_data.sparse_hdr.blk_sz;
      switch (m_unsparse_data.chunk_hdr.chunk_type) {
      case CHUNK_TYPE_RAW: {
        set_unsparse_status(status, STATUS_OK, UNSPARSE_WAIT_CHUNK_DATA, size,
                            buf);
        break;
      }
      case CHUNK_TYPE_DONT_CARE: {
        m_unsparse_data.image_address_offset +=
            m_unsparse_data.chunk_remain_data_size;
        m_unsparse_data.chunk_remain_data_size = 0;
        set_unsparse_status(status, STATUS_OK, UNSPARSE_WAIT_CHUNK_HEADER, size,
                            buf);
        break;
      }
      case CHUNK_TYPE_FILL: {
        set_unsparse_status(status, STATUS_OK, UNSPARSE_WAIT_CHUNK_FILL, size,
                            buf);
        break;
      }
      case CHUNK_TYPE_CRC: {
        set_unsparse_status(status, STATUS_OK, UNSPARSE_WAIT_CHUNK_CRC, size,
                            buf);
        break;
      }
      default:
        LOGI("@: CHUNK_TYPE_UNKNOWN: 0x%x\n",
             m_unsparse_data.chunk_hdr.chunk_type);
        status->handle_status = STATUS_UNKNOWN_SPARSE_CHUNK_TYPE;
        return;
      }
      ++m_unsparse_data.chunks_processed;
    } else {
      m_unsparse_data.unhandle_buf_size = size;
      memcpy(m_unsparse_data.unhandle_buf, buf, size);
      size = 0; // force to jump out while loop
      set_unsparse_status(status, STATUS_OK, UNSPARSE_WAIT_CHUNK_HEADER, size,
                          buf);
    }
    break;
  }
  case UNSPARSE_WAIT_CHUNK_FILL: {
    filled_seed = *(int *)buf;
    buf += sizeof(uint32_t);
    size -= sizeof(uint32_t);

    if (s_filled_data_cache == 0) {
#if DA_ENABLE_DCACHE
      s_filled_data_cache =
        (uint8_t *)reserve_cached_memory_block(sizeof(struct fill_buff_t));
#else
      s_filled_data_cache = (uint8_t *)malloc(sizeof(struct fill_buff_t));
#endif
      if (s_filled_data_cache == 0) {
        status->handle_status = STATUS_MEM_ALLOC_FAIL;
        return;
      }
      memset(s_filled_data_cache, 0, sizeof(struct fill_buff_t));
      memset(((struct fill_buff_t *)s_filled_data_cache)->f[1].data, 0xFF,
             FILLED_DATA_CACHE_SIZE);
    }
    struct fill_buff_t *p = (struct fill_buff_t *)s_filled_data_cache;

    slot = 0;
    less_use_idx = 0;
    found = false;
    for (; slot < FILL_BUF_CNT; ++slot) {
      // to reduce search time, use 0 as guard.
      if (*(int *)(p->f[slot].data) == 0 && slot > 1)
        break;

      if (*(int *)(p->f[slot].data) == filled_seed) {
        found = true;
        p->f[slot].hit++;
        break;
      }
    }

    if (!found) {
      if (slot < FILL_BUF_CNT) {
        memsetint(p->f[slot].data, filled_seed, FILLED_DATA_CACHE_SIZE);
      } else {
        // not enough
        less_use_idx = 2; // 0, 1 used for special value.
        int i = 2;
        for (; i < FILL_BUF_CNT; ++i) {
          if (p->f[less_use_idx].hit > p->f[i].hit)
            less_use_idx = i;
        }
        slot = less_use_idx;
        memsetint(p->f[slot].data, filled_seed, FILLED_DATA_CACHE_SIZE);
      }
    }
    filled_data_cache = (uint8_t *)(p->f[slot].data);

    if (status->checksum_level & CHKSUM_LEVEL_STORAGE) {
      excepted_checksum =
          status->checksum_of_flash +
          filled_seed * (m_unsparse_data.chunk_remain_data_size / sizeof(int));
    }

    while (true) {
      kk = m_unsparse_data.chunk_remain_data_size > FILLED_DATA_CACHE_SIZE
               ? FILLED_DATA_CACHE_SIZE
               : m_unsparse_data.chunk_remain_data_size;
      if (kk == 0) {
        break;
      }
      write_this_block(status, m_unsparse_data.image_address_offset,
                       filled_data_cache, kk);
      if (FAIL(status->handle_status)) {
        return;
      }

      m_unsparse_data.image_address_offset += kk;
      m_unsparse_data.chunk_remain_data_size -= kk;
#if (DA_MODE_FPGA)
      // send_da_alive();
#endif
    }
    if (status->checksum_level & CHKSUM_LEVEL_STORAGE) {
      if (excepted_checksum != status->checksum_of_flash) {
        status->handle_status = STATUS_CHECKSUM_ERR;
        set_error_msg("Sparse image FILL part checksum mismatch.");
        return;
      }
    }

    set_unsparse_status(status, STATUS_OK, UNSPARSE_WAIT_CHUNK_HEADER, size,
                        buf);
    break;
  }
  case UNSPARSE_WAIT_CHUNK_CRC: {
    filled_seed = *(uint32_t *)buf;
    buf += sizeof(uint32_t);
    size -= sizeof(uint32_t);
    m_unsparse_data.chunk_remain_data_size = 0;
    set_unsparse_status(status, STATUS_OK, UNSPARSE_WAIT_CHUNK_HEADER, size,
                        buf);
    break;
  }
  case UNSPARSE_WAIT_CHUNK_DATA: {
    if (size >= m_unsparse_data.chunk_remain_data_size) {

      write_this_block(status, m_unsparse_data.image_address_offset, buf,
                       m_unsparse_data.chunk_remain_data_size);
      if (FAIL(status->handle_status)) {
        return;
      }

      buf += m_unsparse_data.chunk_remain_data_size;
      size -= m_unsparse_data.chunk_remain_data_size;
      m_unsparse_data.image_address_offset +=
          m_unsparse_data.chunk_remain_data_size;
      m_unsparse_data.chunk_remain_data_size = 0;
      set_unsparse_status(status, STATUS_OK, UNSPARSE_WAIT_CHUNK_HEADER, size,
                          buf);
    } else { // big trunk, so it need more than 1 package.
      sizeOfX = size;
      if (size & (STORAGE_BLOCK_SIZE - 1)) // not 512 aligned.
      {
        if (size < STORAGE_BLOCK_SIZE) // last fragment in a package. this
                                       // package must have successive package
                                       // that contain the identical trunk.
        {
          set_unsparse_status(status, STATUS_SPARSE_INCOMPLETE,
                              UNSPARSE_WAIT_CHUNK_DATA, size, buf);
          break;
        } else {
          size &= ~(STORAGE_BLOCK_SIZE - 1);
        }
      }

      write_this_block(status, m_unsparse_data.image_address_offset, buf, size);
      if (FAIL(status->handle_status)) {
        return;
      }

      m_unsparse_data.image_address_offset += size;
      m_unsparse_data.chunk_remain_data_size -= size;
      buf += size;
      size = sizeOfX & (STORAGE_BLOCK_SIZE - 1); // size = 0 org
      set_unsparse_status(status, STATUS_OK, UNSPARSE_WAIT_CHUNK_DATA, size,
                          buf);
    }
    break;
  }
  default:
    LOGI("sparse do nothing.\n");
    break;
  }

  return;
}

void write_sparse_data(unsparse_status_t *status, uint8_t *data,
                       uint32_t length) {
  status->buf = data;
  status->byte_to_process = length;
  in_sparse_state = true;

  do {
    write_sparse_data_internal(status);

    if (status->handle_status != STATUS_OK) {
      in_sparse_state = false;
      return;
    }

  } while (status->byte_to_process > 0);

  in_sparse_state = false;

  return;
}

BOOL is_sparse_image(uint8_t *data, uint32_t length) {
  sparse_header_t *sparse_header = (sparse_header_t *)data;
  return (sparse_header->magic == SPARSE_HEADER_MAGIC);
}

uint64_t unspared_size(uint8_t *data) {
  uint64_t size = 0;
  sparse_header_t *sparse_header = (sparse_header_t *)data;
  size = (uint64_t)sparse_header->blk_sz * sparse_header->total_blks;

  return size;
}

BOOL support_sparse_image(void) { return true; }
