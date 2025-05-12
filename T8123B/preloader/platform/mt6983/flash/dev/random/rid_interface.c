#include "boot/dev_interface/rid_interface.h"

extern const char *sec_get_chip_random_id();
//extern int sec_read_product_usbid(char *serialno);
int __attribute__((weak)) sec_read_product_usbid(char *serialno) {return 0;}

const char *interface_get_random_id() {
#if (DA_MODE_FPGA)
  return "XX";
#else
#if (DA_ENABLE_SECURITY)
  return sec_get_chip_random_id();
#else
  return "XX";
#endif
#endif
}

const char *interface_read_product_id() {
#define MAX_PRODUCT_ID 64  //current is 16B
  static char proid[MAX_PRODUCT_ID] = {0};
  sec_read_product_usbid(proid);
  return proid;
}
