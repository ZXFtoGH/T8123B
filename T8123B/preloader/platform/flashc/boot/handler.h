#pragma once
#include <platform.h>
#include <boot/hal.h>

typedef int (*HHANDLE)(struct com_channel_struct*c, const char* xml);

struct dagent_cmd {
  struct dagent_cmd *next;
  const char *version;
  const char *str_id;
  unsigned int sec_support;
  HHANDLE handle;
};

struct sys_property {
  struct sys_property *next;
  const char *key;
  const char *value;
};
extern struct sys_property *varlist;
#define FOREACH_SYS_PROP(var) for(var=varlist;var;var=var->next)


void register_major_command(const char *str_id,
                            const char *version, HHANDLE handle);
void dagent_handler();

int get_version_major(const char *version);
int get_version_minor(const char *version);

void publish_sys_property(const char *key, const char *value);
