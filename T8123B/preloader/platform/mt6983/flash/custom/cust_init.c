#include "boot/dev_interface/rid_interface.h"
#include "boot/security_export/anti_rollback.h"
#include "boot/security_export/remote_sec_policy.h"
#include "boot/security_export/sha1.h"
#include "boot/security_export/sha256.h"
#include "oemkey.h"
#include "type_define.h"
#include <debug.h>

#define RID_LEN (16)
#define SOCID_LEN (32)
#define RND_LEN (16)
#define DA_AUTH_MESSAGE_LEN (2048)
char hrid[RID_LEN];
uint32_t rnd[4] __attribute__((aligned(8)));

uint8_t key_n[] = {RSA_SLA_KEY_N};
uint8_t key_e[] = {RSA_SLA_KEY_E};

extern char *buftostring(const char *p, uint32_t len);

int cust_pubk_init(void) {
  int ret = 0;

  uint8_t oemkey[OEM_PUBK_SZ] = {OEM_PUBK};
  seclib_set_oemkey(oemkey, OEM_PUBK_SZ);
  uint8_t oemkey3072[OEM_3072_PUBK_SZ] = {OEM_3072_PUBK};
  seclib_set_oemkey(oemkey3072, OEM_3072_PUBK_SZ);

  return ret;
}

status_t cust_security_get_dev_fw_info(uint8_t **buf, uint32_t *buf_sz) {
  status_t r = STATUS_OK;

  if (buf == NULL || buf_sz == NULL)
    return STATUS_ERR;
#define CACHE_LINE 64
  *buf = memalign(CACHE_LINE, DA_AUTH_MESSAGE_LEN);
  if (*buf == NULL)
    return STATUS_ERR;

  r = get_hrid_internal(hrid, RID_LEN);
  if (FAIL(r)) {
    LOGE("[SEC]Get HRID failed.\n");
    return r;
  }

  r = plat_get_rnd(rnd);
  rnd[3] = rnd[2] = rnd[1] = rnd[0];
  if (FAIL(r)) {
    LOGE("[SEC]Get RND failed.\n");
    return r;
  }

  char *str_hrid = buftostring(hrid, RID_LEN);
  char *str_rnd = buftostring(rnd, RND_LEN);
  char *str_socid = buftostring(interface_get_random_id(), SOCID_LEN);

  char *xml_str = *buf;

  memset(xml_str, 0, DA_AUTH_MESSAGE_LEN);
  const char *sxml_s = "<?xml version=\"1.0\" "
                       "encoding=\"utf-8\"?>"
                       "<sla version=\"1.0\">";
  const char *sxml_e = "</sla>";

  size_t pos = strlcpy(xml_str, sxml_s, DA_AUTH_MESSAGE_LEN);
  int cur_w = 0;
  cur_w = snprintf(xml_str + pos, DA_AUTH_MESSAGE_LEN - pos, "<rnd>%s</rnd>",
                   str_rnd);
  pos += cur_w;
  cur_w = snprintf(xml_str + pos, DA_AUTH_MESSAGE_LEN - pos, "<hrid>%s</hrid>",
                   str_hrid);
  pos += cur_w;
  cur_w = snprintf(xml_str + pos, DA_AUTH_MESSAGE_LEN - pos,
                   "<socid>%s</socid>", str_socid);
  pos += cur_w;

  *buf_sz = strlcat(xml_str, sxml_e, DA_AUTH_MESSAGE_LEN - pos);

  LOGD("%s\n", xml_str);
  free(str_hrid);
  free(str_rnd);
  free(str_socid);
  return r;
}

status_t cust_security_verify_sec_policy(uint8_t *buf, uint32_t buf_sz) {
  status_t r = STATUS_OK;

  uint32_t idx = 0;
  int ret = 0;

  uint32_t key_n_sz = sizeof(key_n);
  uint32_t key_e_sz = sizeof(key_e);

  LOGI("SLA EMSG Received.\n");

  unsigned int buf_decrypt_len = 0;

  ret = rsa_oaep_decrypt(1, 0, 0, key_e, sizeof(key_e), key_n, sizeof(key_n),
                         buf, buf_sz, buf_sz, &buf_decrypt_len);

  if (ret) {
    LOGE("SLA signature invalid.");
    return STATUS_SEC_RSA_OAEP_FAIL;
  }

  if ((buf_decrypt_len == RND_LEN) && (memcmp(rnd, buf, RND_LEN) == 0)) {
    LOGI("[SEC]RND key match.");
    r = STATUS_OK;
  } else {
    LOGE("[SEC]RND key mismatch. Lock functions.\n");
    r = STATUS_SEC_CIPHER_KEY_INVALID;
  }

  return r;
}

unsigned int cust_init(void) { return 0; }
