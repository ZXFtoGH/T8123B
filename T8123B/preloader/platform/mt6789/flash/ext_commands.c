#include "boot/commands_xml.h"
#include "boot/error_message.h"

#include "boot/dev_interface/accessible_register_address.h"
#include "boot/dev_interface/rid_interface.h"
#include "boot/partition/partition_struct.h"
#include "debug.h"
#include "error_code.h"
#include <driver/register_act_util.h>

#include "boot/dev_interface/storage_ctrl_interface.h"
#include "boot/dev_interface/ufs_interface.h"
#include "common_struct.h"
#include "string.h"

// COMMAND IMPLEMENT
extern int func_num;
extern const char *xml_err;
// get parameters


