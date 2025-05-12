#pragma once
#pragma once
#include <lib/mxml/mxml-private.h>
#include <boot/partition/partition_info_repertory.h>

int resolve_scatter_file(const char *image_path, const mxml_node_t *flist_node,
                 const char *scatter_xml, struct sys_scatter_struct *ss);
enum flash_update_changed_e compare_scatter_file(struct sys_partition_tbl *gpt,
                         struct sys_scatter_struct *ss);

BOOL is_preloader_partition(const char *part_name);
BOOL is_flashinfo_partition(const char *part_name);
BOOL is_partition_table(const char *part_name);
BOOL is_system_partition(const char *part_name);
BOOL if_native_contain_partition_table(const char *part_name, uint64_t address, uint64_t max_len);

BOOL get_protected_partitions(struct sys_partition_tbl *gpt,
                             struct sys_scatter_struct *ss,
                             struct protected_record_t *rcd);

