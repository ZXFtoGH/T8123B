#pragma once
#include <stdint.h>
#include <common_struct.h>

//----------------------------common-----------------------
// This may be changed according to NAND PMT evolution.
#define CURRENT_PMT_MAX_SIZE 48 * 1024
#define MAX_PARTITION_NAME_LEN 64

#define PART_ATTR_FTL_IMG 0x00000001
/*extension is used by NAND at current*/
struct extension {
  unsigned int type;    /*low page or full page*/
  unsigned char attr;   /*attr bit map*/
  unsigned char region; /*region*/
};

typedef struct {
  unsigned char name[MAX_PARTITION_NAME_LEN]; /* partition name */
  unsigned long long size;                    /* partition size */
  union {
    unsigned long long region;
        /* partition region */ // will be used as download type on L branch.
                               // xiaolei
    struct extension ext;
  };
  unsigned long long offset;     /* partition start */
  unsigned long long mask_flags; /* partition flags */
} pt_resident;

// partition table type
typedef enum partition_table_type {
  INVALID_PT = 0,
  PARTITION_MIRROR = 1,
  SGPT = PARTITION_MIRROR,
  MIRROR_PMT = PARTITION_MIRROR,

  PARTITION_MAJOR = 2,
  MAIN_PMT = PARTITION_MAJOR,
  PGPT = PARTITION_MAJOR,
} partition_table_type;

typedef enum partition_table_catagory {
  CATAGORY_GPT = 100,
  CATAGORY_PMT = 101,
} partition_table_catagory;

//----------------------------pmt----------------------------
#define MAIN_PMT_NAME "MAIN_PMT"
#define MIRROR_PMT_NAME "MIRROR_PMT"

//---xxNANDxx---
// signature
#define PT_SIG 0x50547633  //"PTv3"
#define MPT_SIG 0x4D505433 //"MPT3"
#define PT_SIG_SIZE 4
#define PT_TLCRATIO_SIZE 4
#define is_valid_mpt(buf) ((*(uint32_t *)(buf)) == MPT_SIG)
#define is_valid_pt(buf) ((*(uint32_t *)(buf)) == PT_SIG)
#define is_valid_pt_v1(buf) ((*(uint32_t *)(buf)) == PT_SIG_V1)

// sizes
#define LPAGE 16384
#define PMT_SPARE_SIZE 2048 // it is different with real spare size

// download type
#define REGION_LOW_PAGE 0x004C4F57
#define REGION_FULL_PAGE 0x46554C4C

#define MAX_OOBFREE_ENTRIES 8

struct nand_oobfree {
  unsigned int offset;
  unsigned int length;
};

struct nand_ecclayout {
  unsigned int eccbytes;
  unsigned int eccpos[64];
  struct nand_oobfree oobfree[MAX_OOBFREE_ENTRIES];
};

typedef struct {
  int sequencenumber : 8;
  int tool_or_sd_update : 4;
  int pt_next : 4;      // have next pt
  int mirror_pt_dl : 4; // mirror download OK
  int mirror_pt_has_space : 4;
  int pt_changed : 4;
  int pt_has_space : 4;
} pt_info;

//---xxNAND endxx---

// pmt end

//----------------------------gpt----------------------------
#define PGPT_NAME "PGPT"
#define SGPT_NAME "SGPT"

#define GPT_ENTRY_SIZE (sizeof(gpt_entry))
#define GPT_ENTRY_NR MAX_PARTITION_COUNT
#define GPT_ENTRY_TABLE_SIZE (GPT_ENTRY_SIZE * GPT_ENTRY_NR)
#define GPT_FIRST_USABLE_LBA 34 // 2 + ( GPT_ENTRY_TABLE_SIZE / 512 )
#define GPT_LBA_NUMBER 34 // mbr+header+entry--->PGPT 34  header+entry-->SGPT 33
#define GPT_LBA_SIZE 512

#define GPT_HEADER_SIGNATURE 0x5452415020494645ULL
#define GPT_HEADER_VERSION 0x00010000

#define EFI_GUID(a, b, c, d0, d1, d2, d3, d4, d5, d6, d7)                      \
   ((efi_guid_t)) \
{(a) & 0xff, ((a)>>8) & 0xff,((a)>>16) & 0xff,((a)>> 24) & 0xff,\
   (b) & 0xff,((b)>> 8) & 0xff,\
   (c) & 0xff, ((c)>> 8) & 0xff,\
   (d0),(d1),(d2),(d3),(d4),(d5),(d6),(d7)})

#define PARTITION_BASIC_DATA_GUID                                              \
  EFI_GUID(0xEBD0A0A2, 0xB9E5, 0x4433, 0x87, 0xC0, 0x68, 0xB6, 0xB7, 0x26,     \
           0x99, 0xC7)

typedef enum gpt_exist_type {
  PRIMARY_ONLY = 0,
  SECONDARY_ONLY,
  PRIMARY_SECONDARY,
  NO_VILIAD_GPT
} gpt_exist_type;

typedef struct {
  uint32_t a;
  uint16_t b;
  uint16_t c;
  uint8_t d0;
  uint8_t d1;
  uint8_t d2;
  uint8_t d3;
  uint8_t d4;
  uint8_t d5;
  uint8_t d6;
  uint8_t d7;
} efi_guid_raw_data;

/*GuidPT*/
typedef struct {
  uint8_t boot_ind;
  uint8_t head;
  uint8_t sector;
  uint8_t cyl;
  uint8_t sys_ind;
  uint8_t end_head;
  uint8_t end_sector;
  uint8_t end_cyl;
  uint32_t start_sector;
  uint32_t nr_sects;
} guidPT;

typedef struct {
  uint8_t b[16];
} efi_guid_t;

#define GPT_ENTRY_NAME_LEN (72 / sizeof(int16_t))

typedef struct {
  efi_guid_t partition_type_guid;
  efi_guid_t unique_partition_guid;
  uint64_t starting_lba;
  uint64_t ending_lba;
  uint64_t attributes;
  uint16_t partition_name[GPT_ENTRY_NAME_LEN];
} gpt_entry;

#pragma pack(push, 1)
typedef struct {
  uint8_t boot_code[440];
  uint32_t unique_mbr_signature;
  uint16_t unknown;
  guidPT partition_record[4];
  uint16_t signature;
} pmbr;

typedef struct {
  uint64_t signature;
  uint32_t revision;
  uint32_t header_size;
  uint32_t header_crc32;
  uint32_t reserved;
  uint64_t my_lba;
  uint64_t alternate_lba;
  uint64_t first_usable_lba;
  uint64_t last_usable_lba;
  efi_guid_t disk_guid;
  uint64_t partition_entry_lba;
  uint32_t num_partition_entries;
  uint32_t sizeof_partition_entry;
  uint32_t partition_entry_array_crc32;
} gpt_header;
#pragma pack(pop)
