#include "error_code.h"
#include <debug.h>
#include <sys/types.h>

#if INTERFACE_I2C_IMPL

extern int i2c_hw_init(void);
void dev_i2c_init() { i2c_hw_init(); }

#else

void dev_i2c_init() {}

#endif