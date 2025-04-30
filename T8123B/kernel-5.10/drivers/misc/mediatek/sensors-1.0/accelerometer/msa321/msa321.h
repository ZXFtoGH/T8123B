/*
 * Copyright (C) 2024 MEMSIC, Inc.
 *
 * Initial Code:
 *	Aaron Peng
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 *
 */

/*
 * Definitions for MSA321 accelorometer sensor chip.
 */
#ifndef __MSA321_H__
#define __MSA321_H__


#include	<linux/ioctl.h>	/* For IOCTL macros */



/************************************************/
/* 	Accelerometer defines section	 	*/
/************************************************/
#define MSA321_DEV_NAME		"msa321"

#define MSA321_I2C_ADDR		0x62

#define MSA321_REG_CHIP_ID          0x01

/* MSA321 register address */
#define MSA321_REG_X			0x02
#define MSA321_REG_Y			0x03
#define MSA321_REG_Z			0x06
//#define MSA321_REG_TEMP		0x09
#define MSA321_REG_RANG		0x0F
#define MSA321_REG_PWR		0x11

#define MSA321_ID			0x13



/*para setting*/
#define MSA321_AWAKE			0x1E	/* power on */
#define MSA321_SLEEP			0xDE	/* power donw */

#define MSA321_BW_50HZ			0x06
#define MSA321_RANGE_2G		0x00
#define MSA321_RANGE_4G		0x01
#define MSA321_RANGE_8G		0x02


/*ERR code*/
#define MSA321_SUCCESS			0
#define MSA321_ERR_I2C			-1
#define MSA321_ERR_STATUS		-3
#define MSA321_ERR_SETUP_FAILURE	-4
#define MSA321_ERR_GETGSENSORDATA	-5
#define MSA321_ERR_IDENTIFICATION	-6

#define MSA321_BUFSIZE			256
#define MSA321_STABLE_DELAY	10

#define TEMP_CALIBRATE_STATIC_THRESHOLD    120
#define TEMP_CALIBRATE_STATIC_COUNT        12

#define MSA_OFFSET_MAX				   250
#define MSA_OFFSET_CUS				   130
#define MSA_OFFSET_SEN				   1024

#endif /* __MSA321_H__ */

