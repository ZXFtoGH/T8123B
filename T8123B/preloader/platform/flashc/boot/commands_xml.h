#pragma once
#include <lib/mxml/mxml-private.h>
#include "hal.h"

typedef void (*cmd_dpc_cb)(void *arg);
struct cmd_dpc_t {
  const char *key;
  cmd_dpc_cb cb;
  void *arg;
};
struct cmd_dpc_t *get_cmd_dpc();
void clear_cmd_dpc();


const char *mxmlGetNodeText(mxml_node_t *tree, const char *path);

    // COMMAND IMPLEMENT
int unknown_command_guard(struct com_channel_struct *channel, const char *xml);
int cmd_write_partition(struct com_channel_struct *channel, const char *xml);
int cmd_write_partitions(struct com_channel_struct *channel, const char *xml);
int cmd_write_flash(struct com_channel_struct *channel, const char *xml);
int cmd_readback_partition(struct com_channel_struct *channel, const char *xml);
int cmd_readback_flash(struct com_channel_struct *channel, const char *xml);
int cmd_readback_partition_table(struct com_channel_struct *channel, const char *xml);

int cmd_set_runtime_parameter(struct com_channel_struct *channel,
                              const char *xml);
int cmd_boot_to(struct com_channel_struct *channel, const char *xml);
int cmd_init_dram(struct com_channel_struct *channel, const char *xml);
int cmd_erase_partition(struct com_channel_struct *channel, const char *xml);
int cmd_erase_flash(struct com_channel_struct *channel, const char *xml);

int cmd_flash_all(struct com_channel_struct *channel, const char *xml);
int cmd_flash_update(struct com_channel_struct *channel, const char *xml);


int cmd_notify_init_hw(struct com_channel_struct *channel, const char *xml);
int cmd_set_host_info(struct com_channel_struct *channel, const char *xml);

int cmd_write_efuse(struct com_channel_struct *channel, const char *xml);
int cmd_read_efuse(struct com_channel_struct *channel, const char *xml);

struct reboot_dpc_t *get_reboot_dpc();
int cmd_reboot(struct com_channel_struct *channel, const char *xml);
int cmd_ram_test(struct com_channel_struct *channel, const char *xml);

int cmd_set_boot_mode(struct com_channel_struct *channel, const char *xml);
int cmd_set_rsc_info(struct com_channel_struct *channel, const char *xml);
int cmd_set_cert_file(struct com_channel_struct *channel, const char *xml);
int cmd_get_da_info(struct com_channel_struct *channel, const char *xml);
int cmd_get_hw_info(struct com_channel_struct *channel, const char *xml);

int cmd_emmc_control(struct com_channel_struct *channel,
                              const char *xml);
int cmd_security_set_external_signature(struct com_channel_struct *channel,
                               const char *xml);
int cmd_security_get_dev_fw_info(struct com_channel_struct *channel,
                                 const char *xml);
int cmd_security_set_all_in_one_signature(struct com_channel_struct *channel,
                                 const char *xml);
int cmd_security_set_remote_sec_policy(struct com_channel_struct *channel,
                              const char *xml);
int cmd_dram_repair(struct com_channel_struct *channel,
                                    const char *xml);
int cmd_ufs_setting(struct com_channel_struct *channel, const char *xml);
int cmd_get_downloaded_image_feedback(struct com_channel_struct *channel,
                                const char *xml);
int cmd_get_system_property(struct com_channel_struct *channel,
                                const char *xml);
int cmd_host_supported_cmds(struct com_channel_struct *channel,
                            const char *xml);


//platform
int cmd_read_register(struct com_channel_struct *channel, const char *xml);
int cmd_write_register(struct com_channel_struct *channel, const char *xml);


