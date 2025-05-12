#include <error_code.h>
#define __WEAK __attribute__((weak))

__WEAK unsigned int get_dbg_info_base(unsigned int key)
{
    return 0;
}

__WEAK void pl_log_store(char c){ return;}

__WEAK unsigned int seclib_get_devinfo_with_index(unsigned int index)
{
    return 0;
}
__WEAK unsigned int internal_seclib_get_devinfo_with_index(unsigned int index)
{
    return 0;
}

__WEAK int sec_util_brom_download_recovery_check(void)
{
    return 0;
}

__WEAK void print(char *fmt, ...)
{
}

__WEAK void log_buf_ctrl(int drambuf)
{
    return;
}
__WEAK void log_ctrl(int enable)
{
    return;
}
__WEAK int log_status(void)
{
    return 0;
}
__WEAK void dbg_print(char *fmt, ...)
{
    return;
}


__WEAK void dconfig_initenv(void) {
}

__WEAK void dconfig_printenv(void) {
}

__WEAK char *dconfig_getenv(char *name) {
	return 0;
}

__WEAK int sec_pl_header_info_read(void *data, unsigned int data_sz, unsigned int index)
{
    //dummy function, if sec_pl_header_info_read is not defined in DA.
    return STATUS_SEC_NOT_VALID_IMGHDR;
}

__WEAK int sec_hash_cmp(unsigned char *data, unsigned int data_len, unsigned char *hash, unsigned int hash_len)
{
    //dummy function, if sec_hash_cmp is not defined in DA.
    return STATUS_SEC_HASH_OP_FAIL;
}
