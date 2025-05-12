#pragma once
#include "boot/partition/partition_struct.h"
#include "boot/dev_interface/hw_interface.h"
uint32_t
get_main_partition_table_size(const enum partition_table_catagory cata);

uint32_t
get_mirror_partition_table_size(const enum partition_table_catagory cata);

enum partition_table_catagory get_partition_type();

enum storage_type get_partition_resident();
enum storage_section get_partition_table_section();

uint64_t get_user_size();

uint32_t get_block_size();

uint64_t get_pgpt_max_size();

uint64_t get_pgpt_addr();

enum storage_section get_preloader_resident(uint8_t *partition);

uint64_t get_boot_size(uint8_t *partition);


#define _countof(array) (sizeof(array) / sizeof(array[0]))

enum storage_section get_map_section_fields(const char *str);
enum storage_type get_storage_type();

char **get_all_native_partition_list() ;

struct storage_parameter_t {
  enum storage_type storage;
  enum storage_section section;
  uint64_t max_size;
};

BOOL get_storage_section_parameters(
    const char *section_str,
    /*IN OUT*/ struct storage_parameter_t *param);

BOOL is_disable_this_operation(const char *op, void *arg);
