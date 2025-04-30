#ifndef _PANEL_IT6151_MIPIRX_H
#define _PANEL_IT6151_MIPIRX_H

int it6151_mipirx_i2c_read_byte(unsigned char dev_addr, unsigned char addr, unsigned char *returnData);
int it6151_mipirx_i2c_write_byte(unsigned char dev_addr, unsigned char addr, unsigned char writeData);

#endif