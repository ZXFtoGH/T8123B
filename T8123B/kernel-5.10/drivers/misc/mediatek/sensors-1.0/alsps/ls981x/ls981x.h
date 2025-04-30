/*
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
#ifndef __LS980X_H__
#define __LS980X_H__

#include <linux/ioctl.h>

#define DRIVER_VERSION          "1.0_20240719"

/*ALSPS REGS*/
/*********************************************
* Register definitions *
*********************************************/
    
#define LS9801I_T_ENABLE_REG     		0x00
#define LS9801I_T_WCRTL_REG     		0x01
#define LS9801I_T_RESET_REG     		0x02
#define LS9801I_T_PROX_INT_CTRL_REG     0x03
#define LS9801I_T_PROX_INT1EN_REG     	0x04
#define LS9801I_T_PROX_INT2EN_REG     	0x05
#define LS9801I_T_PROX_PILTL_REG     	0x0C
#define LS9801I_T_PROX_PILTH_REG     	0x0D
#define LS9801I_T_PROX_PIHTL_REG     	0x0E
#define LS9801I_T_PROX_PIHTH_REG     	0x0F
#define LS9801I_T_LIGHT_AWT_REG     	0x11
#define LS9801I_T_LIGHT_AITL_REG     	0x12
#define LS9801I_T_LIGHT_AITH_REG     	0x13
#define LS9801I_T_LIGHT_AGAIN_REG     	0x16
#define LS9801I_T_LIGHT_DGAIN_REG       0x17
#define LS9801I_T_PROX_PWT_REG    		0x1D
#define LS9801I_T_PROX_PIT_REG     	    0x1E
#define LS9801I_T_PROX_PCTRL1_REG     	0x1F
#define LS9801I_T_PROX_PCTRL3_REG     	0x21
#define LS9801I_T_PROX_PGAIN_REG     	0x22
#define LS9801I_T_PROX_PDRV_REG     	0x23
#define LS9801I_T_PROX_PPULSE_REG     	0x24
#define LS9801I_T_PROX_PXTALKL_REG      0x25
#define LS9801I_T_PROX_PXTALKH_REG      0x26
#define LS9801I_T_DATA_STATUS_REG       0x2C
#define LS9801I_T_INT_STATUS_REG        0x2D
#define LS9801I_T_LIGHT_RAW_REG_L       0x2F
#define LS9801I_T_LIGHT_RAW_REG_H       0x30
#define LS9801I_T_IR_RAW_REG_L      	0x31
#define LS9801I_T_IR_RAW_REG_H      	0x32
#define LS9801I_T_PROX_RAW_REG_L        0x33
#define LS9801I_T_PROX_RAW_REG_H        0x34
#define LS9801I_T_PA_RAW_REG_L       	0x35
#define LS9801I_T_PA_RAW_REG_H       	0x36
#define LS9801I_T_CHECK_ID_REG     	    0x72
#define LS9801I_T_CHECK_ID_DATA    	    0x61

/*********************************************
* Register control                           *
*********************************************/
#define LS9801I_T_RESET_VALUE      	       0x80
#define LS9801I_T_WCRTL_VALUE			   0x05

#define LS9801I_T_LIGHT_ENABLE_VALUE       0x63
#define LS9801I_T_PROX_ENABLE_VALUE        0x64
#define LS9801I_T_LIGHT_DISABLE_VALUE      0x64
#define LS9801I_T_PROX_DISABLE_VALUE       0x63
#define LS9801I_T_PROX_INT_CTRL_VALUE      0X00	

#define LS9801I_T_LIGHT_AWT_VALUE     	   0x32
#define LS9801I_T_LIGHT_AITL_VALUE         0xFF
#define LS9801I_T_LIGHT_AITH_VALUE         0x1F
#define LS9801I_T_LIGHT_AGAIN_VALUE        0x66
#define LS9801I_T_LIGHT_DGAIN_VALUE        0x66

#define LS9801I_T_LIGHT_DATA_STATUS_VALUE     	0x02
#define LS9801I_T_PROX_DATA_STATUS_VALUE      	0x08
#define LS9801I_T_PROX_INT1EN_VALUE     		0x00
#define LS9801I_T_PROX_INT2EN_VALUE     		0xC0
#define LS9801I_T_PROX_PWT_VALUE     			0x50
#define LS9801I_T_PROX_PGAIN_VALUE     	    	0x67
#define LS9801I_T_PROX_PPULSE_VALUE     		0x03
#define LS9801I_T_PROX_PIT_VALUE     			0x32
#define LS9801I_T_PROX_PDRV_VALUE     			0x3b
#define LS9801I_T_PROX_PCTRL1_VALUE     		0x06
#define LS9801I_T_PROX_PCTRL3_VALUE     		0x07
#define LS9801I_T_PROX_PXTALKH_VALUE     		0x00
#define LS9801I_T_PROX_PXTALKL_VALUE     		0x00
#define LS9801I_T_IFLAG_P_H 					0x80
#define LS9801I_T_IFLAG_P_L 					0x40
    
#define LAGAIN_MASK                    	(0x07 << 0)
#define LAGAIN_1X                      	(0X02 << 0)
#define LAGAIN_2X                      	(0X03 << 0)
#define LAGAIN_4X                      	(0X04 << 0)
#define LAGAIN_8X                      	(0X05 << 0)
#define LAGAIN_16X                     	(0X06 << 0)
#define LAGAIN_32X                     	(0X07 << 0)
#define IRAGAIN_1X                      (0X02 << 4)
#define IRAGAIN_2X                      (0X03 << 4)
#define IRAGAIN_4X                      (0X04 << 4)
#define IRAGAIN_8X                      (0X05 << 4)
#define IRAGAIN_16X                     (0X06 << 4)
#define IRAGAIN_32X                     (0X07 << 4)
extern struct platform_device *get_alsps_platformdev(void);

#endif
