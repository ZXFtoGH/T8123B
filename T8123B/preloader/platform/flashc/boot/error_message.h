#pragma once
#include <compiler.h>
#include <stdarg.h>
#include <error_code.h>
#include <debug.h>

void split(char *src, char **dest, int *cnt, char sep);
char *lltohexstring(uint64_t v);

void clear_error_msg();
void set_error_msg(const char *fmt, ...) __PRINTFLIKE(1, 2);;
const char *get_error_msg();