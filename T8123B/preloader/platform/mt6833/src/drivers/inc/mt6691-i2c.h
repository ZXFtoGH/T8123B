#ifndef __LINUX_MT6691_I2C_H
#define __LINUX_MT6691_I2C_H

#include <typedefs.h>
#include "pal_log.h"

#define MT6691_DEBUG 1

#if MT6691_DEBUG
#define MT6691TAG		"[mt6691] "
#define MT6691LOG(fmt, arg...)	pal_log_info(MT6691TAG fmt, ##arg)
#else
#define MT6691LOG(...) do { }	while(0)
#endif /* MT6691_DEBUG */

#define MT6691_CID_CODE		(0x01)

#define MT6691_VDD2_ID		0x0000
#define MT6691_VDDQ_ID		0x0000

/* Voltage setting */
#define MT6691_VSEL0		0x00
#define MT6691_VSEL1		0x01
/* Control register */
#define MT6691_CONTROL		0x02
/* IC Type */
#define MT6691_ID1		0x03
/* IC mask version */
#define MT6691_ID2		0x04
/* Monitor register */
#define MT6691_MONITOR		0x05
#define MT6691_PGOOD_SHIFT	7
#define MT6691_CTRL2		0x06
#define MT6691_DN_SR_MASK	0x7
#define MT6691_DN_SR_SHIFT	5
#define MT6691_CTRL3		0x07
#define MT6691_CTRL4		0x08
/* Hidden mode */
#define	MT6691_CTRL5		0x09

struct mt6691_chip {
	kal_uint8 id;
	kal_uint16 i2c_channel;
	kal_uint8 slave_addr;
	kal_uint8 buck_ctrl;
	kal_uint8 mode_shift;
	kal_uint8 en_shift;
	bool is_hw_exist;
};

extern kal_uint32 mt6691_read_interface(struct mt6691_chip *chip,
					kal_uint8 RegNum, kal_uint8 *val,
					kal_uint8 MASK, kal_uint8 SHIFT);
extern kal_uint32 mt6691_config_interface(struct mt6691_chip *chip,
					  kal_uint8 RegNum, kal_uint8 val,
					  kal_uint8 MASK, kal_uint8 SHIFT);
extern struct mt6691_chip *mt6691_find_chip(int id);
extern int mt6691_i2c_probe(void);
#endif /* __LINUX_MT6691_I2C_H */
