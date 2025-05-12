#include "boot/hal.h"
#include "boot/system_objects.h"
#include "lib/mxml/mxml-private.h"
#include <boot/error_message.h>
#include <string.h>
void split(char *src, char **dest, int *cnt, char sep) {
  if (!src || !dest || !cnt)
    return;
  int max = *cnt;
  if (max == 0)
    return;

  *cnt = 1;
  dest[0] = src;
  while (*src) {
    if (*src == sep) {
      *src = 0;
      if (*cnt >= max)
        return;
      dest[*cnt] = src + 1;
      ++(*cnt);
    }
    src++;
  }
}

static char map[] = {'0', '1', '2', '3', '4', '5', '6', '7',
                       '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

char *buftostring(const char *p, uint32_t len) {
  char *s = (char *)malloc(len * 2 + 1);
  if (s == 0)
  	return s;

  s[len * 2] = 0;

  int i = 0;
  int j = 0;
  for (; j < len; i += 2, j += 1) {
    s[i] = map[(p[j] >> 4) & 0xF];
    s[i + 1] = map[p[j] & 0xF];
  }

  return s;
}

char *lltohexstring(uint64_t v) {
  static char s[sizeof(uint64_t) * 2 + 1];
  s[sizeof(uint64_t) * 2] = 0;
  char *p = (char *)&v;

  int i = sizeof(uint64_t) * 2 - 1;
  int j = 0;
  for (; j < sizeof(uint64_t); i -= 2, j += 1) {
    s[i] = map[p[j] & 0xF];
    s[i - 1] = map[(p[j] >> 4) & 0xF];
  }

  return s;
}

static char *g_err_msg = 0;
#define MAX_ERROR_MSG_BUF_LEN 4096

void clear_error_msg() {
  if (g_err_msg == 0)
    g_err_msg = malloc(MAX_ERROR_MSG_BUF_LEN);

  if (g_err_msg == 0) return;
  g_err_msg[0] = 0;
}

void set_error_msg(const char *fmt, ...) {
  if (g_err_msg) {
    uint32_t offset = strnlen(g_err_msg, MAX_ERROR_MSG_BUF_LEN);
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(g_err_msg + offset, MAX_ERROR_MSG_BUF_LEN - offset, fmt, ap);
    va_end(ap);

    LOGE(g_err_msg + offset);
  }
}

const char *get_error_msg() { return g_err_msg; }