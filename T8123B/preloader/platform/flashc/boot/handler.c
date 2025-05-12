#include "boot/commands_xml.h"
#include "boot/error_message.h"
#include "boot/hal.h"
#include "boot/handler.h"
#include "boot/protocol_key.h"
#include "boot/system_objects.h"
#include "command_define.h"
#include "error_code.h"
#include "lib/mxml/mxml-private.h"
#include "string.h"

const char *cstr_cmd_start = "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
                             "<host>"
                             "<version>1.0</version>"
                             "<command>CMD:START</command>"
                             "</host>";

const char *cstr_cmd_end = "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
                           "<host>"
                           "<version>1.0</version>"
                           "<command>CMD:END</command>"
                           "<arg><result>%s</result><message>%s</message></arg>"
                           "</host>";

static struct dagent_cmd end_guard = {NULL,  "1.0", CMD_UNKNOWN, false, unknown_command_guard};
static struct dagent_cmd *cmdlist = &end_guard;

int get_version_major(const char *version) { return atoi(version); }
int get_version_minor(const char *version) {
#define VER_STR_LEN 16
  if (strnlen(version, VER_STR_LEN*2) > VER_STR_LEN)
    return 0;

  int i = 0;
  for (; i < VER_STR_LEN; ++i) {
    if (version[i] == '.') {
      return atoi(&version[i+1]);
    }
  }
  return 0;
}

void register_major_command(const char *str_id, const char *version,
                            HHANDLE handle) {
  struct dagent_cmd *cmd;
  unsigned int security_enabled = false; // all disabled. LXO

  cmd = (struct dagent_cmd *)malloc(sizeof(*cmd));
  if (cmd) {
    cmd->str_id = str_id;
    cmd->version = version;
    cmd->sec_support = security_enabled;
    cmd->handle = handle;
    cmd->next = cmdlist;
    cmdlist = cmd;
  }
}

//sys-prop
struct sys_property *varlist = NULL;
void publish_sys_property(const char *key, const char *value) {
  struct sys_property *var;
  var = (struct sys_property *)malloc(sizeof(struct sys_property));
  if (var) {
    var->key = key;
    var->value = value;
    var->next = varlist;
    varlist = var;
  }
}

#define STATE_OFFLINE 0
#define STATE_COMMAND 1
#define STATE_COMPLETE 2
#define STATE_ERROR 3

unsigned int dagent_state = STATE_OFFLINE;

static void dagent_command_loop2(void) {
#define ALIGN_SZ (64)
#if STAGE_DA_LOOP
#define XML_CMD_BUFF_LEN (0x80000)
#else
#define XML_CMD_BUFF_LEN (2048)
#endif

#define CMD_RESULT_BUFF_LEN (64)

  struct dagent_cmd *cmd;

  uint8_t __attribute__((aligned(ALIGN_SZ))) result[CMD_RESULT_BUFF_LEN];
  static uint8_t *xml = 0;
  if (xml == 0)
    xml = memalign(ALIGN_SZ, XML_CMD_BUFF_LEN);

  if (xml == 0) {
    LOGE("Fatal: Alloc cmd buffer fail.");
    return;
  }
  // avoid calculate len every time.
  uint32_t cmd_start_len = strnlen(cstr_cmd_start, 512) + 1;
  uint32_t cmd_end_len;
  unsigned int len;
  int zresult;
  dagent_state = STATE_COMMAND;

  while (dagent_state == STATE_COMMAND) {
    // CMD:START
    LOGD("\n@Protocol: Tx START-CMD(%s)\n", cstr_cmd_start);
    com_channel.write((uint8_t *)cstr_cmd_start, cmd_start_len);

    len = CMD_RESULT_BUFF_LEN;
    if ((zresult = com_channel.read(result, (uint32_t *)&len)) != 0) {
      LOGE("\n@Protocol: REVEIVING Start CMD Result ERROR(0x%x). halt.\n",
           zresult);
      break;
    }

    //IMPORTANT: Do not put log before USB-read.
    //or cause USB data loss in later USB-read when LOG Channel is USB.
    //LOGD("\n@Protocol: Rx START-RESP(%s)\n", result);

    char *vec[3] = {0};
    int cnt = 3;
    split(result, vec, &cnt, '@');
    if (RSLT_FAILED(vec[0])) {
      LOGI("\n@Protocol: Host notice error. or cancel operation.\n");
      continue;
    }

    len = XML_CMD_BUFF_LEN;
    memset(xml, 0, XML_CMD_BUFF_LEN);
    if (com_channel.read((uint8_t *)xml, (uint32_t *)&len) != 0) {
      const char *res = RESULT_ERR;
      com_channel.write((uint8_t *)res, strlen(res) + 1);
      continue;
    }

    LOGD("\n@Protocol: Rx Host CMD(%s)\n", xml);

    mxml_node_t *tree, *node;
    const char *command_ref, *version_ref;
    char command[64];
    char version[16];
    memset(command, 0, 64);
    memset(version, 0, 16);
    if ((tree = mxmlLoadString(NULL, xml, MXML_OPAQUE_CALLBACK)) == NULL ||
        (node = mxmlFindPath(tree, "da/command")) == NULL ||
        (node->type != MXML_OPAQUE) ||
        (command_ref = mxmlGetOpaque(node)) == NULL ||
        (node = mxmlFindPath(tree, "da/version")) == NULL ||
        (node->type != MXML_OPAQUE) ||
        (version_ref = mxmlGetOpaque(node)) == NULL) {
      strlcpy(command, "#XML-FORMAT-ERR#", 64);
      strlcpy(version, "x.x", 16);
      LOGE("\n@Protocol: CMD XML(%s) invalid.\n", xml);
    } else {
      strlcpy(command, command_ref, 64);
      strlcpy(version, version_ref, 16);
    }

    if (tree != NULL)
      mxmlDelete(tree);

    char *res = RESULT_UNSUPPORTED;
    /*Pick up matched command and handle it*/
    for (cmd = cmdlist; cmd->next != NULL; cmd = cmd->next) {
      if ((0 == strnicmp(cmd->str_id, command, 64)) &&
          (get_version_major(cmd->version) == get_version_major(version))) {
        res = RESULT_OK;
        break;
      }
    }

    if (0 == strnicmp(cmd->str_id, CMD_UNKNOWN, XML_CMD_BUFF_LEN)) {
        res = RESULT_UNSUPPORTED;
    }

    com_channel.write((uint8_t *)res, strlen(res) + 1);
    // CMD:START  END

    LOGI("\n@Protocol: Execute CMD(%s)\n", command);
    if (sysob_runtime_params.hw_init_status == STATUS_OK) {
        zresult = cmd->handle(&com_channel, (const char*)xml);
    } else {
        zresult = sysob_runtime_params.hw_init_status;
    }

    // CMD:END
    xml_snprintf(xml, XML_CMD_BUFF_LEN, cstr_cmd_end,
                 zresult == 0
                     ? RESULT_OK
                     : zresult == STATUS_UNSUPPORT_CMD ? RESULT_UNSUPPORTED
                                                       : RESULT_ERR,
                 get_error_msg());
    LOGD("\n@Protocol: Tx END-CMD(%s)\n", xml);

    cmd_end_len = strnlen(xml, XML_CMD_BUFF_LEN) + 1;
    com_channel.write(xml, cmd_end_len);

    len = CMD_RESULT_BUFF_LEN;
    if (com_channel.read(result, (uint32_t *)&len) != 0) {
      LOGE("\n@Protocol: REVEIVING END CMD Result ERROR. halt.\n");
      break;
    }
    LOGD("\n@Protocol: Rx END-RESP(%s)\n", result);
    // CMD:END END
    // process dpc: reboot, boot_to, switch_usb_high_speed etc.
    if (get_cmd_dpc()->cb != 0) {
      LOGD("\n@Protocol: DPC CALL\n");
      get_cmd_dpc()->cb(get_cmd_dpc()->arg);
      if ((get_cmd_dpc()->key != 0) &&
          (strnicmp(get_cmd_dpc()->key, "reboot", 7) == 0)) {
        dagent_state = STATE_OFFLINE;
      }
      clear_cmd_dpc();
    }
    //If error occurred, stop the command loop
    if ((zresult != STATUS_OK) && (zresult != STATUS_UNSUPPORT_CMD)) {
      dagent_state = STATE_ERROR;
    }
  }
  dagent_state = STATE_OFFLINE;
}

void dagent_handler() { dagent_command_loop2(); }
