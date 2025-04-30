// SPDX-License-Identifier: GPL-2.0
/*
 *  Copyright (C) 2019 Fuzhou Rockchip Electronics Co., Ltd.
 *
 *  gm8773c driver  Single MIPI to dual MIPI
 *  timer :2023.05.16
 *  Author: http://www.corpro.cn/
 *  v0.1.0x10 : 1. create file.
 *  v0.2.0x10 : 2. add support for mipi parameters
 *  v0.3.0x10 : 3. add support for mipi PN Change
				   add support for select Lane number
				   optimize the driver architecture
 *  Version: GM8773C_DRIVER_V0.2.10
 */
#define DEBUG
#include <linux/clk.h>
#include <linux/delay.h>
#include <linux/err.h>
#include <linux/gpio/consumer.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/i2c.h>
#include <linux/kernel.h>
#include <linux/media.h>
#include <linux/cdev.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/of_graph.h>
#include <linux/of_gpio.h>

#include <linux/regulator/consumer.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/videodev2.h>
#include <linux/version.h>
//#include <linux/rk-camera-module.h>
#include <media/media-entity.h>
#include "gm8773c.h"
#if 1

#define DRIVER_VERSION			KERNEL_VERSION(0, 0x03, 0x00) /*driver version*/
#define DRIVER_NAME "gm8773c"
#define GM8773C_PASSWORD 0xAA
#define DRIVER_CNT 1
#define DUAL_PORT  2
#define REFCLK_MIPI				 0x00
#define REFCLK_OSC				 0x30
#define REFCLK_XTAL				 0x20

/*mipi 屏幕所用到的指令类型，根据所需修改，如有添加，请根据自己所需修改相关.h文件与.c文件驱动*/
#define DCS_LONG_WRITE		 	 0x39
#define DCS_SHORT_WRITE			 0x15
#define DCS_SHORT_Read 			 0x06

/**根据Mipi 屏幕的Timming 参数修改，注意我司IC有要求，HBP必须大于43，源端与GM8773C均要设置成一样**/
#define TX0_HACTIVE   800
#define TX0_VACTIVE   2176
#define TX1_HACTIVE   800
#define TX1_VACTIVE   2176
#define TX_HFP 		  18
#define TX_HSW		  12
#define TX_HBP		  48
#define TX_VFP 		  220
#define TX_VSW		  8
#define TX_VBP		  28
#define TX0_HTOTAL    (TX0_HACTIVE+TX_HFP+TX_HSW+TX_HBP)
#define TX0_VTOTAL    (TX0_VACTIVE+TX_VFP+TX_VSW+TX_VBP)
#define TX1_HTOTAL    (TX1_HACTIVE+TX_HFP+TX_HSW+TX_HBP)
#define TX1_VTOTAL    (TX1_VACTIVE+TX_VFP+TX_VSW+TX_VBP)
#define RXMIPI_FPS    60

#define DISPLAY_MODE  left_and_right_mode   		/*left_and_right_mode   左右异显模式*/
													/*tx0_equals_tx1		   左右复制模式*/
													/*Parity_mode		       奇偶模式*/

#define REFCLK_SECELT REFCLK_MIPI          		     /* 选择参考时钟： REFCLK_MIPI	 为通过前端mipi时钟做参考*
												     * 				  REFCLK_XTAL		为通过外部晶振时钟做参考*
												     *                REFCLK_OSC		为通过内部时钟做参考     */

#define MIPI_RX_LANE				4				/*输入只支持4或2 Lane */
#define MIPI_TX_LANE				4				/*输入只支持4,2,1 Lane */
#define DISPLAY_RXCLK_PN_CHANGE 	0			    /*RX MIPI CLK Lane 交换*/
#define DISPLAY_RXDATA_PN_CHANGE 	0			    /*RX MIPI DATA Lane 交换*/
#define DISPLAY_TX0CLK_PN_CHANGE 	0			    /*TX0 MIPI CLK Lane 交换*/
#define DISPLAY_TX0DATA_PN_CHANGE 	0			    /*TX0 MIPI DATA Lane 交换*/
#define DISPLAY_TX1CLK_PN_CHANGE 	0			    /*TX1 MIPI CLK Lane 交换*/
#define DISPLAY_TX1DATA_PN_CHANGE 	0			    /*TX1 MIPI DATA Lane 交换*/

#define EOTP_ENABLE   0								/*eotp enable*/
#define MIPI_MODE     SYNC_PULSE            		/*mipi mode  */ 
#define TX_CLOCK_MODE 0x01                  		/*0x01:输出mipi连续时钟     0x00:输出mipi 非连续时钟 */
#define MIPI_TX_LPX   0x05				    		/*tlpx参数，主要是对mipi初始化指令的下发，一般设置为0x05,0x18,0x28 */

/**							  使用GM8773C内部的测试模式 			 					    **/
#define GM8773C_BIST 0    				    //1 : open bist mode     0 : close bist mode
#define GREEN                               //bist color     GREEN   BLUE   RED     
#define gm8773c_ID(_msb, _lsb)		((_msb) << 8 | (_lsb))

extern struct gm8773c_dev gm8773c_dev;
extern struct gm8773c gm8773c_basic_cfg;
struct i2c_client *gm8773c_global_client;
EXPORT_SYMBOL_GPL(gm8773c_global_client);

/*GM8773C 的TX0 通道发送Mipi Command*/
static struct dcs_setting_entry dcs_custom_table_tx0[] = {
	{LONG_WRITE_CMD1,  0x39,	4, {0xB9, 0x83, 0x10, 0x2E},TX0_SEND},
    {LONG_WRITE_CMD2,  0x39,	5, {0xD1, 0x67, 0x0C, 0xFF, 0x05},TX0_SEND},
    {LONG_WRITE_CMD3,  0x39, 18, {0xB1, 0x10, 0xFA, 0xAF, 0xAF, 0x20, 0x20, 0xC1, 0x75, 0x39, 0x36, 0x36, 0x36, 0x36, 0x22, 0x21, 0x15, 0x00},TX0_SEND},
    {LONG_WRITE_CMD4,  0x39,	3, {0xD2, 0x20, 0x20},TX0_SEND},
    {LONG_WRITE_CMD5,  0x39, 17, {0xB2, 0x00, 0x40, 0x68, 0x80, 0x00, 0x22, 0xDC, 0x21, 0x00, 0x00, 0x00, 0x00, 0x15, 0x20, 0xD7, 0x00},TX0_SEND},
    {LONG_WRITE_CMD6,  0x15,	2, {0xBD, 0x03},TX0_SEND},
    {LONG_WRITE_CMD7,  0x15,	2, {0xB2, 0x80},TX0_SEND},
    {LONG_WRITE_CMD8,  0x15,	2, {0xBD, 0x00},TX0_SEND},
    {LONG_WRITE_CMD9,  0x39, 17, {0xB4, 0x88, 0x64, 0x88, 0x64, 0x88, 0x74, 0x68, 0x50, 0x01, 0xA4, 0x01, 0x58, 0x00, 0xFF, 0x00, 0xFF},TX0_SEND},
    {LONG_WRITE_CMD10,  0x39,	4, {0xB6, 0x38, 0x38, 0xE3},TX0_SEND},
    {LONG_WRITE_CMD11,  0x15,	2, {0xE9, 0xCD},TX0_SEND},
    {LONG_WRITE_CMD12,  0x15,	2, {0xBB, 0x01},TX0_SEND},
    {LONG_WRITE_CMD13,  0x15,	2, {0xE9, 0x00},TX0_SEND},
	{LONG_WRITE_CMD14,  0x39,	4, {0xBF, 0xFC, 0x85, 0x80},TX0_SEND},
	{LONG_WRITE_CMD15,  0x39, 9, {0xBA, 0x70, 0x03, 0xA8, 0x83, 0xF2, 0x00, 0x80, 0x0D},TX0_SEND},
	{LONG_WRITE_CMD16,  0x39,	44, {0xD3, 0x00, 0x00, 0x00, 0x00, 0x01, 0x04, 0x00, 0x08, 0x00, 0x37, 0x37, 0x64, 0x6F, 0x22, 0x22, 0x04, 0x04, 0x32, 0x10, 0x1F, 0x00, 0x1F, 0x32, 0x18, 0xA7, 0x08, 0xA7, 0x32, 0x10, 0x10, 0x00, 0x10, 0x00, 0x00, 0x1B, 0x37, 0x01, 0x7F, 0x1B, 0x37, 0x01, 0x7F, 0x0F},TX0_SEND},
	{LONG_WRITE_CMD17,  0x39, 47, {0xE0, 0x03, 0x06, 0x10, 0x17, 0x1F, 0x36, 0x4F, 0x56, 0x5D, 0x59, 0x72, 0x77, 0x7B, 0x88, 0x85, 0x8B, 0x93, 0xA5, 0xA5, 0x51, 0x59, 0x64, 0x72, 0x00, 0x06, 0x10, 0x17, 0x1F, 0x36, 0x4F, 0x56, 0x5D, 0x59, 0x72, 0x77, 0x7B, 0x88, 0x85, 0x8B, 0x93, 0xA5, 0xA5, 0x51, 0x59, 0x64, 0x74},TX0_SEND},
	{LONG_WRITE_CMD18,  0x15,	2, {0xBD, 0x01},TX0_SEND},
	{LONG_WRITE_CMD19,  0x39, 12, {0xD3, 0x01, 0x00, 0x7C, 0x00, 0x00, 0x11, 0x10, 0x00, 0x0A, 0x00, 0x01},TX0_SEND},
	{LONG_WRITE_CMD20,  0x15,	2, {0xBD, 0x02},TX0_SEND},
	{LONG_WRITE_CMD21,  0x39, 7, {0xB4, 0x4E, 0x00, 0x33, 0x11, 0x33, 0x88},TX0_SEND},
	{LONG_WRITE_CMD22,  0x39,	4, {0xBF, 0xF2, 0x00, 0x02},TX0_SEND},
	{LONG_WRITE_CMD23,  0x15,	2, {0xBD, 0x00},TX0_SEND},
	{LONG_WRITE_CMD24,  0x39,	15, {0xC0, 0x23, 0x23, 0x22, 0x11, 0xA2, 0x17, 0x00, 0x80, 0x00, 0x00, 0x08, 0x00, 0x63, 0x63},TX0_SEND},
    {LONG_WRITE_CMD25,  0x39, 9, {0xC8, 0x00, 0x04, 0x04, 0x00, 0x00, 0x82, 0x13, 0xFF},TX0_SEND},
    {LONG_WRITE_CMD26,  0x39, 4, {0xD0, 0x07, 0x04, 0x05},TX0_SEND},
    {LONG_WRITE_CMD27,  0x39,	45, {0xD5, 0x25, 0x24, 0x25, 0x24, 0x18, 0x18, 0x18, 0x18, 0x07, 0x06, 0x07, 0x06, 0x05, 0x04, 0x05, 0x04, 0x03, 0x02, 0x03, 0x02, 0x01, 0x00, 0x01, 0x00, 0x38, 0x38, 0x38, 0x38, 0x39, 0x39, 0x39, 0x39, 0x21, 0x20, 0x21, 0x20, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18},TX0_SEND},
    {LONG_WRITE_CMD28,  0x39,	45, {0xD6, 0x18, 0x18, 0x20, 0x21, 0x3A, 0x3A, 0x19, 0x19, 0x0E, 0x0F, 0x0E, 0x0F, 0x06, 0x07, 0x06, 0x07, 0x0C, 0x0D, 0x0C, 0x0D, 0x04, 0x05, 0x04, 0x05, 0x0A, 0x0B, 0x0A, 0x0B, 0x02, 0x03, 0x02, 0x03, 0x08, 0x09, 0x08, 0x09, 0x00, 0x01, 0x00, 0x01, 0x18, 0x18, 0x24, 0x25},TX0_SEND},
    {LONG_WRITE_CMD29,  0x39,	24, {0xE7, 0x1E, 0x15, 0x02, 0x02, 0x4F, 0x4F, 0x0E, 0x0E, 0x1A, 0x2A, 0x28, 0x6B, 0x2C, 0x6B, 0x01, 0x27, 0x00, 0x00, 0x00, 0x00, 0x17, 0x00, 0x68},TX0_SEND},
    {LONG_WRITE_CMD30,  0x15,	2, {0xBD, 0x01},TX0_SEND},
    {LONG_WRITE_CMD31,  0x39, 8, {0xE7, 0x01, 0x50, 0x01, 0xA0, 0x0D, 0xB0, 0x0E},TX0_SEND},
    {LONG_WRITE_CMD32,  0x15,	2, {0xBD, 0x02},TX0_SEND},
    {LONG_WRITE_CMD33,  0x39,	29, {0xE7, 0xFC, 0x01, 0xFD, 0x01, 0x00, 0x00, 0x25, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x81, 0x00, 0x02, 0x40},TX0_SEND},
    {LONG_WRITE_CMD34,  0x15, 2, {0xBD, 0x00},TX0_SEND},
    {LONG_WRITE_CMD35,  0x15,	2, {0xBD, 0x02},TX0_SEND},
    {LONG_WRITE_CMD36,  0x39, 13, {0xD8, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0},TX0_SEND},
    {LONG_WRITE_CMD37,  0x15,	2, {0xBD, 0x03},TX0_SEND},
    {LONG_WRITE_CMD38,  0x39, 25, {0xD8, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xA0, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xA0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0},TX0_SEND},
    {LONG_WRITE_CMD39,  0x15,	2, {0xBD, 0x00},TX0_SEND},
	{LONG_WRITE_CMD40,  0x15, 2, {0xCC, 0x02},TX0_SEND},
	{LONG_WRITE_CMD41,  0x15,	2, {0xBD, 0x00},TX0_SEND},
	{LONG_WRITE_CMD42,  0x39, 3, {0xBE, 0x11, 0x96},TX0_SEND},
	{LONG_WRITE_CMD43,  0x15,	2, {0xD9, 0x3F},TX0_SEND},
	{LONG_WRITE_CMD44,  0x39,	3, {0xE1, 0x01, 0x01},TX0_SEND},
	// {LONG_WRITE_CMD45,  0x15,	2, {0xCF, 0xFF},TX0_SEND},
};

/*GM8773C 的TX1 通道发送Mipi Command*/
static struct dcs_setting_entry dcs_custom_table_tx1[] = {
	{LONG_WRITE_CMD1,  0x39,	4, {0xB9, 0x83, 0x10, 0x2E},TX1_SEND},
    {LONG_WRITE_CMD2,  0x39,	5, {0xD1, 0x67, 0x0C, 0xFF, 0x05},TX1_SEND},
    {LONG_WRITE_CMD3,  0x39, 18, {0xB1, 0x10, 0xFA, 0xAF, 0xAF, 0x20, 0x20, 0xC1, 0x75, 0x39, 0x36, 0x36, 0x36, 0x36, 0x22, 0x21, 0x15, 0x00},TX1_SEND},
    {LONG_WRITE_CMD4,  0x39,	3, {0xD2, 0x20, 0x20},TX1_SEND},
    {LONG_WRITE_CMD5,  0x39, 17, {0xB2, 0x00, 0x40, 0x68, 0x80, 0x00, 0x22, 0xDC, 0x21, 0x00, 0x00, 0x00, 0x00, 0x15, 0x20, 0xD7, 0x00},TX1_SEND},
    {LONG_WRITE_CMD6,  0x15,	2, {0xBD, 0x03},TX1_SEND},
    {LONG_WRITE_CMD7,  0x15,	2, {0xB2, 0x80},TX1_SEND},
    {LONG_WRITE_CMD8,  0x15,	2, {0xBD, 0x00},TX1_SEND},
    {LONG_WRITE_CMD9,  0x39, 17, {0xB4, 0x88, 0x64, 0x88, 0x64, 0x88, 0x74, 0x68, 0x50, 0x01, 0xA4, 0x01, 0x58, 0x00, 0xFF, 0x00, 0xFF},TX1_SEND},
    {LONG_WRITE_CMD10,  0x39,	4, {0xB6, 0x38, 0x38, 0xE3},TX1_SEND},
    {LONG_WRITE_CMD11,  0x15,	2, {0xE9, 0xCD},TX1_SEND},
    {LONG_WRITE_CMD12,  0x15,	2, {0xBB, 0x01},TX1_SEND},
    {LONG_WRITE_CMD13,  0x15,	2, {0xE9, 0x00},TX1_SEND},
	{LONG_WRITE_CMD14,  0x39,	4, {0xBF, 0xFC, 0x85, 0x80},TX1_SEND},
	{LONG_WRITE_CMD15,  0x39, 9, {0xBA, 0x70, 0x03, 0xA8, 0x83, 0xF2, 0x00, 0x80, 0x0D},TX1_SEND},
	{LONG_WRITE_CMD16,  0x39,	44, {0xD3, 0x00, 0x00, 0x00, 0x00, 0x01, 0x04, 0x00, 0x08, 0x00, 0x37, 0x37, 0x64, 0x6F, 0x22, 0x22, 0x04, 0x04, 0x32, 0x10, 0x1F, 0x00, 0x1F, 0x32, 0x18, 0xA7, 0x08, 0xA7, 0x32, 0x10, 0x10, 0x00, 0x10, 0x00, 0x00, 0x1B, 0x37, 0x01, 0x7F, 0x1B, 0x37, 0x01, 0x7F, 0x0F},TX1_SEND},
	{LONG_WRITE_CMD17,  0x39, 47, {0xE0, 0x03, 0x06, 0x10, 0x17, 0x1F, 0x36, 0x4F, 0x56, 0x5D, 0x59, 0x72, 0x77, 0x7B, 0x88, 0x85, 0x8B, 0x93, 0xA5, 0xA5, 0x51, 0x59, 0x64, 0x72, 0x00, 0x06, 0x10, 0x17, 0x1F, 0x36, 0x4F, 0x56, 0x5D, 0x59, 0x72, 0x77, 0x7B, 0x88, 0x85, 0x8B, 0x93, 0xA5, 0xA5, 0x51, 0x59, 0x64, 0x74},TX1_SEND},
	{LONG_WRITE_CMD18,  0x15,	2, {0xBD, 0x01},TX1_SEND},
	{LONG_WRITE_CMD19,  0x39, 12, {0xD3, 0x01, 0x00, 0x7C, 0x00, 0x00, 0x11, 0x10, 0x00, 0x0A, 0x00, 0x01},TX1_SEND},
	{LONG_WRITE_CMD20,  0x15,	2, {0xBD, 0x02},TX1_SEND},
	{LONG_WRITE_CMD21,  0x39, 7, {0xB4, 0x4E, 0x00, 0x33, 0x11, 0x33, 0x88},TX1_SEND},
	{LONG_WRITE_CMD22,  0x39,	4, {0xBF, 0xF2, 0x00, 0x02},TX1_SEND},
	{LONG_WRITE_CMD23,  0x15,	2, {0xBD, 0x00},TX1_SEND},
	{LONG_WRITE_CMD24,  0x39,	15, {0xC0, 0x23, 0x23, 0x22, 0x11, 0xA2, 0x17, 0x00, 0x80, 0x00, 0x00, 0x08, 0x00, 0x63, 0x63},TX1_SEND},
    {LONG_WRITE_CMD25,  0x39, 9, {0xC8, 0x00, 0x04, 0x04, 0x00, 0x00, 0x82, 0x13, 0xFF},TX1_SEND},
    {LONG_WRITE_CMD26,  0x39, 4, {0xD0, 0x07, 0x04, 0x05},TX1_SEND},
    {LONG_WRITE_CMD27,  0x39,	45, {0xD5, 0x25, 0x24, 0x25, 0x24, 0x18, 0x18, 0x18, 0x18, 0x07, 0x06, 0x07, 0x06, 0x05, 0x04, 0x05, 0x04, 0x03, 0x02, 0x03, 0x02, 0x01, 0x00, 0x01, 0x00, 0x38, 0x38, 0x38, 0x38, 0x39, 0x39, 0x39, 0x39, 0x21, 0x20, 0x21, 0x20, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18},TX1_SEND},
    {LONG_WRITE_CMD28,  0x39,	45, {0xD6, 0x18, 0x18, 0x20, 0x21, 0x3A, 0x3A, 0x19, 0x19, 0x0E, 0x0F, 0x0E, 0x0F, 0x06, 0x07, 0x06, 0x07, 0x0C, 0x0D, 0x0C, 0x0D, 0x04, 0x05, 0x04, 0x05, 0x0A, 0x0B, 0x0A, 0x0B, 0x02, 0x03, 0x02, 0x03, 0x08, 0x09, 0x08, 0x09, 0x00, 0x01, 0x00, 0x01, 0x18, 0x18, 0x24, 0x25},TX1_SEND},
    {LONG_WRITE_CMD29,  0x39,	24, {0xE7, 0x1E, 0x15, 0x02, 0x02, 0x4F, 0x4F, 0x0E, 0x0E, 0x1A, 0x2A, 0x28, 0x6B, 0x2C, 0x6B, 0x01, 0x27, 0x00, 0x00, 0x00, 0x00, 0x17, 0x00, 0x68},TX1_SEND},
    {LONG_WRITE_CMD30,  0x15,	2, {0xBD, 0x01},TX1_SEND},
    {LONG_WRITE_CMD31,  0x39, 8, {0xE7, 0x01, 0x50, 0x01, 0xA0, 0x0D, 0xB0, 0x0E},TX1_SEND},
    {LONG_WRITE_CMD32,  0x15,	2, {0xBD, 0x02},TX1_SEND},
    {LONG_WRITE_CMD33,  0x39,	29, {0xE7, 0xFC, 0x01, 0xFD, 0x01, 0x00, 0x00, 0x25, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x81, 0x00, 0x02, 0x40},TX1_SEND},
    {LONG_WRITE_CMD34,  0x15, 2, {0xBD, 0x00},TX1_SEND},
    {LONG_WRITE_CMD35,  0x15,	2, {0xBD, 0x02},TX1_SEND},
    {LONG_WRITE_CMD36,  0x39, 13, {0xD8, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0},TX1_SEND},
    {LONG_WRITE_CMD37,  0x15,	2, {0xBD, 0x03},TX1_SEND},
    {LONG_WRITE_CMD38,  0x39, 25, {0xD8, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xA0, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xA0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0},TX1_SEND},
    {LONG_WRITE_CMD39,  0x15,	2, {0xBD, 0x00},TX1_SEND},
	{LONG_WRITE_CMD40,  0x15, 2, {0xCC, 0x02},TX1_SEND},
	{LONG_WRITE_CMD41,  0x15,	2, {0xBD, 0x00},TX1_SEND},
	{LONG_WRITE_CMD42,  0x39, 3, {0xBE, 0x11, 0x96},TX1_SEND},
	{LONG_WRITE_CMD43,  0x15,	2, {0xD9, 0x3F},TX1_SEND},
	{LONG_WRITE_CMD44,  0x39,	3, {0xE1, 0x01, 0x01},TX1_SEND},
	// {LONG_WRITE_CMD45,  0x15,	2, {0xCF, 0xFF},TX1_SEND},
};

/*GM8773C 使用内部寄存器来下发mipi command，注意使用这个方式对长包有个数限制
  ，最多8个byte，一般不使用这个方式，仅限有特殊时序屏幕，通常使用可以注释掉*/
#if 0
static struct dcs_setting_entry dcs_custom_table_regist[]={
	{LONG_WRITE_CMD1, 0x23,2,{0xb0,0x00},TX0_AND_TX1_SEND},
	{DELAY,  0,	0, {50},0},
	{LONG_WRITE_CMD2, 0x23,2,{0xb3,0x14},TX0_AND_TX1_SEND},
	{DELAY,  0,	0, {50},0},
};
#endif

/*GM8773C TX0 TX1 发送上电指令*/
static struct dcs_setting_entry dcs_display_on[]={
	{LONG_WRITE_CMD1, 0x05,2,{0x11,0x00},TX0_AND_TX1_SEND},
	{DELAY,  0,	0, {120},0},
	{LONG_WRITE_CMD2, 0x05,2,{0x29,0x00},TX0_AND_TX1_SEND},
	{DELAY,  0,	0, {50},0},
};

/*GM8773C读取屏幕寄存器，0xDA为屏幕ID寄存器，请根据情况修改，
  注意使用这个方式对长包有个数限制，最多8个byte*/
static struct dcs_setting_entry dcs_read_reg[]={
	{LONG_WRITE_CMD1, 0x06,2,{0xDA,0x00},TX0_AND_TX1_SEND},
	//{LONG_WRITE_CMD2, 0x06,2,{0x1A,0x00},TX0_AND_TX1_SEND},
};

static int gm8773c_write(struct i2c_client *client, u16 reg, u16 val)
{
	struct i2c_msg msg;
	u8 buf[2];
	int ret;

	// pr_info("write reg(0x%x val:0x%x)!\n", reg, val);

	buf[0] = reg;
	buf[1] = val;

	msg.addr = client->addr;
	msg.flags = client->flags;
	msg.buf = buf;
	msg.len = sizeof(buf);

	ret = i2c_transfer(client->adapter, &msg, 1);
	if (ret >= 0)
		return 0;

	dev_err(&client->dev,
		"gm8773c write reg(0x%x val:0x%x) failed !\n", reg, val);

	return ret;
}


static int gm8773c_write_bytes(struct i2c_client *client, const u8 *data, u16 data_len)
{
	int ret;
	struct i2c_msg msg;

	msg.addr = client->addr;
	msg.flags = 0;
	msg.len = data_len;
	msg.buf = (u8 *)data;

	ret = i2c_transfer(client->adapter, &msg, 1);
	if (ret >= 0)
		return 0;

	return ret;
}

int gm8773c_i2c_write_byte(struct i2c_client *client,u8 reg,u8 dcs_type, u8 *data,u16 data_len)
{

	if(dcs_type==DCS_LONG_WRITE){
		u8 *buf = kmalloc(data_len + 4, GFP_KERNEL);
        int i, ret;

        if (!buf)
            return -ENOMEM;

		buf[0]=reg;
		buf[1]=dcs_type;
		buf[2]=data_len & 0xff;
		buf[3]=0x00;
		for(i = 0; i < data_len; i++) {
            buf[i+4] = data[i];
        }
        ret = gm8773c_write_bytes(client, buf, data_len+4);
        kfree(buf);
        return ret;
	}
	else{
		u8 *buf = kmalloc(data_len + 2, GFP_KERNEL);
        int i, ret;

        if (!buf)
            return -ENOMEM;

        buf[0] = reg;
        buf[1] = dcs_type;
        for(i = 0; i < data_len; i++) {
            buf[i+2] = data[i];
        }
        ret = gm8773c_write_bytes(client, buf, data_len+2);
        kfree(buf);
        return ret;
	}
	return 1;
}

static int gm8773c_read(struct i2c_client *client, u16 reg, u8 *val)
{
	struct i2c_msg msg[2];
	u8 buf[2];
	int ret;

	buf[0] = reg;

	msg[0].addr = client->addr;
	msg[0].flags = client->flags;
	msg[0].buf = buf;
	msg[0].len = 1;

	msg[1].addr = client->addr;
	msg[1].flags = client->flags | I2C_M_RD;
	msg[1].buf = buf;
	msg[1].len = 2;

	ret = i2c_transfer(client->adapter, msg, 2);
	if (ret >= 0) {
		*val = buf[0];
		return 0;
	}

	dev_err(&client->dev,
		"gm8773cread reg:0x%x failed !\n", reg);

	return ret;
}

static int gm8773c_password(struct i2c_client *client)
{
	int ret;
	u8 val;
	gm8773c_write(client,PASSWORD_REG,GM8773C_PASSWORD);
	ret=gm8773c_read(client,PASSWORD_REG,&val);
	if(ret>0)
	{
		dev_dbg(&client->dev, "gm8773c driver read password error\n");
	}
	else
	{
		if(val == GM8773C_PASSWORD)
			dev_dbg(&client->dev, "gm8773c driver read ID success\n");
		else
			dev_dbg(&client->dev, "gm8773c driver read ID error\n");
			return -1;
	}
	return ret;
}

static int gm8773c_debug(struct i2c_client *client)
{
	pr_info("Dev addr  : 0x%x",client->addr);
	#if (REFCLK_SECELT==REFCLK_MIPI)
	pr_info("Ref clock : MIPI ");
	#elif(REFCLK_SECELT==REFCLK_OSC)
	pr_info("Ref clock : OSC ");
	#elif( REFCLK_SECELT==REFCLK_XTAL)
	pr_info("Ref clock : XTAL ");
	#endif
	#if (GM8773C_BIST==1)
	pr_info("Mode out  : Bist ");
	#else
	pr_info("Mode out  : Normal ");
	#endif
	if(gm8773c_basic_cfg.gm8773c_tx_mode==left_and_right_mode){
	pr_info("Display mode  : left_and_right ");
	}
	else if(gm8773c_basic_cfg.gm8773c_tx_mode==tx0_equals_tx1){
	pr_info("Display mode  : tx0_equals_tx1 ");
	}else{
	pr_info("Display mode  : Parity_mode ");
	}
	pr_info("tx0_hactive               = %d    tx0_vactive =%d", gm8773c_basic_cfg.tx0_hactive,gm8773c_basic_cfg.tx0_vactive);
	pr_info("hsync_width               = %d", gm8773c_basic_cfg.hsync_width);
	pr_info("hback_porch               = %d",gm8773c_basic_cfg.hback_porch);
	pr_info("tx0_htotal                = %d    tx0_vtotal  =%d", gm8773c_basic_cfg.tx0_htotal, gm8773c_basic_cfg.tx0_vtotal);
	pr_info("tx1_hactive               = %d    tx1_vactive =%d", gm8773c_basic_cfg.tx1_hactive ,gm8773c_basic_cfg.tx1_vactive);
	pr_info("mipi_rx_clk               = %d MHz", gm8773c_basic_cfg.mipi_rx_clk/1000000);
	pr_info("mipi_rx_lane_num          = %d  Lane",gm8773c_basic_cfg.Rx_Lane_num);
	pr_info("mipi_tx_lane_num          = %d  Lane",gm8773c_basic_cfg.Tx_Lane_num);
	pr_info("refclk_div                = %d", gm8773c_basic_cfg.refclk_div);
	pr_info("refclk_int                = %d", gm8773c_basic_cfg.refclk_int);
	pr_info("refclk_postdiv            = %d", gm8773c_basic_cfg.refclk_postdiv);
	pr_info("mipi out clk              = %d MHz", gm8773c_basic_cfg.mipi_tx_clk/1000000);
	pr_info("rx date lane change enable= %d ",(gm8773c_basic_cfg.Rx_DateLane_PN_Change ? 1 : 0));
	pr_info("rx clk lane change enable= %d ",(gm8773c_basic_cfg.Rx_ClkLane_PN_Change ? 1 : 0));
	pr_info("tx0 date lane change enable= %d ",(gm8773c_basic_cfg.Tx0_DataLane_PN_Change ? 1 : 0));
	pr_info("tx0 clk lane change enable= %d ",(gm8773c_basic_cfg.Tx0_ClkLane_PN_Change ? 1 : 0));
	pr_info("tx1 date lane change enable= %d ",(gm8773c_basic_cfg.Tx1_DataLane_PN_Change ? 1 : 0));
	pr_info("tx1 clk lane change enable= %d ",(gm8773c_basic_cfg.Tx1_ClkLane_PN_Change ? 1 : 0));
	return 0;
}

static int gm8773c_pllsetting(struct i2c_client *client)
{
	/*refclk_div*/
	if(gm8773c_basic_cfg.refclk_div<=15)
		gm8773c_write(client,0x01,gm8773c_basic_cfg.refclk_div & 0xFF);
	else if(gm8773c_basic_cfg.refclk_div==16)
		gm8773c_write(client,0x01,0x00);
	else
		gm8773c_write(client,0x01,(((gm8773c_basic_cfg.refclk_div)/2 & 0x0F) | 0x10));   
	/*refclk_int*/
	gm8773c_write(client,0x02,gm8773c_basic_cfg.refclk_int & 0xFF); 
	/*refcl_postdiv*/
	if(gm8773c_basic_cfg.refclk_postdiv == 1)
		gm8773c_write(client,0x2C,0x40);
	else if(gm8773c_basic_cfg.refclk_postdiv == 2)
		gm8773c_write(client,0x2C,0x48);
	else if(gm8773c_basic_cfg.refclk_postdiv == 4)
		gm8773c_write(client,0x2C,0x50);
	else if(gm8773c_basic_cfg.refclk_postdiv == 8)
		gm8773c_write(client,0x2C,0x58);
	else
		dev_err(&client->dev,
		"posetdiv setting error ; gm8773c refclk_postdiv must setting 1 or 2 or 4 or 8 ");
	gm8773c_write(client,0x7f,0x70);  
	if(gm8773c_basic_cfg.Rx_Lane_num==4){
		gm8773c_write(client,0x04,(0x05 |((gm8773c_basic_cfg.Rx_DateLane_PN_Change<<7) | \
										  (gm8773c_basic_cfg.Rx_DateLane_PN_Change<<6) | \
										  (gm8773c_basic_cfg.Rx_DateLane_PN_Change<<5) | \
										  (gm8773c_basic_cfg.Rx_DateLane_PN_Change<<4) |\
										  (gm8773c_basic_cfg.Rx_ClkLane_PN_Change<<3)  )));
	}
	else if(gm8773c_basic_cfg.Rx_Lane_num==2){
		gm8773c_write(client,0x04,(0x04 |((gm8773c_basic_cfg.Rx_DateLane_PN_Change<<7) | \
										  (gm8773c_basic_cfg.Rx_DateLane_PN_Change<<6) | \
										  (gm8773c_basic_cfg.Rx_DateLane_PN_Change<<5) | \
										  (gm8773c_basic_cfg.Rx_DateLane_PN_Change<<4) |\
										  (gm8773c_basic_cfg.Rx_ClkLane_PN_Change<<3)  )));
	}

	if(gm8773c_basic_cfg.Tx_Lane_num==4)
		gm8773c_write(client,0xa4,0x97);
	else if(gm8773c_basic_cfg.Tx_Lane_num==2)
		gm8773c_write(client,0xa4,0x91);
	else if(gm8773c_basic_cfg.Tx_Lane_num==1)
		gm8773c_write(client,0xa4,0x90);


	gm8773c_write(client,0xa2,(0x00 | (gm8773c_basic_cfg.Tx1_DataLane_PN_Change<<7) | \
										(gm8773c_basic_cfg.Tx1_DataLane_PN_Change<<6) | \
										(gm8773c_basic_cfg.Tx1_DataLane_PN_Change<<5) | \
										(gm8773c_basic_cfg.Tx0_ClkLane_PN_Change<<4) | \
										(gm8773c_basic_cfg.Tx0_DataLane_PN_Change<<3) | \
										(gm8773c_basic_cfg.Tx0_DataLane_PN_Change<<2) | \
										(gm8773c_basic_cfg.Tx0_DataLane_PN_Change<<1) | \
										(gm8773c_basic_cfg.Tx0_DataLane_PN_Change<<0)   ));
	gm8773c_write(client,0xa3,0x14 | (gm8773c_basic_cfg.Tx1_ClkLane_PN_Change << 7) |\
									 (gm8773c_basic_cfg.Tx1_DataLane_PN_Change << 6)	);

	return 0;
}


static int gm8773c_sync_dly(struct i2c_client *client)
{
	int sync_dly;
	/*sync_dly*/
	sync_dly=(((gm8773c_basic_cfg.tx0_htotal)/2)+200) & 0x0FFF;
	gm8773c_write(client,0xBD,((gm8773c_basic_cfg.tx1_vactive & 0x0F00)>>8) | (sync_dly & 0x0F00)>>4);
	gm8773c_write(client,0xBE,sync_dly & 0x00FF);
	return 0;
}

#if GM8773C_BIST
static int gm8773c_bist(struct i2c_client *client)
{
	unsigned int  bist_line_length;
#ifdef GREEN
	gm8773c_write(client,0x3F,0xFF);
	gm8773c_write(client,0x40,0x00);
	gm8773c_write(client,0x41,0x00);
#endif
#ifdef BLUE
	gm8773c_write(client,0x3F,0x00);
	gm8773c_write(client,0x40,0xFF);
	gm8773c_write(client,0x41,0x00);
#endif
#ifdef RED
	gm8773c_write(client,0x3F,0x00);
	gm8773c_write(client,0x40,0x00);
	gm8773c_write(client,0x41,0xFF);
#endif
	bist_line_length = (16666*(gm8773c_basic_cfg.mipi_tx_clk/1000000)/2/3)/(gm8773c_basic_cfg.tx0_vtotal);
	gm8773c_write(client,0x42,bist_line_length & 0x00FF);
	gm8773c_write(client,0x43,((bist_line_length & 0xFF00) >> 8));
	return 0;
}
#endif

static int gm8773c_display_timming(struct i2c_client *client)
{
	
	gm8773c_write(client,0xB6,gm8773c_basic_cfg.vsync_width & 0x0FF); //VSA [7:0]
	gm8773c_write(client,0xB7,gm8773c_basic_cfg.vback_porch & 0x0FF); //VBP [7:0]
	//gm8773c_write(client,0xB8,0x00);//VSA[11:8]  VBP[11:8]
	gm8773c_write(client,0xB8,((gm8773c_basic_cfg.vback_porch &0xF00)>>8) | ((gm8773c_basic_cfg.vsync_width &0xF00)>>4) );//VSA[11:8]  VBP[11:8]
	gm8773c_write(client,0xB9,gm8773c_basic_cfg.tx0_vactive & 0x00FF); //VAC_TX0[7:0]
	gm8773c_write(client,0xBA,gm8773c_basic_cfg.vfront_porch & 0xFF); //VFP[7:0]
	gm8773c_write(client,0xBB,(gm8773c_basic_cfg.tx0_vactive & 0x0F00)>>4);
	gm8773c_write(client,0xBC,(gm8773c_basic_cfg.tx1_vactive & 0x00FF));

	gm8773c_sync_dly(client);
	gm8773c_write(client,0xB3,gm8773c_basic_cfg.tx0_hactive & 0x00FF);
	gm8773c_write(client,0xB4,gm8773c_basic_cfg.tx1_hactive & 0x00FF);
	gm8773c_write(client,0xB5,(gm8773c_basic_cfg.tx1_hactive & 0x0F00)>>4 | (gm8773c_basic_cfg.tx0_hactive & 0x0F00)>>8);
	gm8773c_write(client,0xBF,gm8773c_basic_cfg.hsync_width & 0xFF);
	gm8773c_write(client,0xC0,(gm8773c_basic_cfg.hsync_width & 0xFF00)>>8);
	gm8773c_write(client,0xC1,gm8773c_basic_cfg.hback_porch & 0xFF);
	gm8773c_write(client,0xC2,(gm8773c_basic_cfg.hback_porch & 0xFF00)>>8);
	gm8773c_write(client,0xC3,gm8773c_basic_cfg.hfront_porch & 0xFF);
	gm8773c_write(client,0xC4,(gm8773c_basic_cfg.hfront_porch & 0xFF00)>>8);
#if GM8773C_BIST
	gm8773c_bist(client);
#endif
	return 0;
}

static int gm8773c_display_mode(struct i2c_client *client)
{
	//gm8773c_write(client,0x39,0x4E);
	if(gm8773c_basic_cfg.eotp==1){   // eotp enable
		gm8773c_write(client,0xAA,gm8773c_basic_cfg.display_mode | 0x10);
	}else if(gm8773c_basic_cfg.eotp==0){ //eotp disenable
		gm8773c_write(client,0xAA,gm8773c_basic_cfg.display_mode | 0x00);
	}
	gm8773c_write(client,0xA2,DISPLAY_TX0CLK_PN_CHANGE ? 0x10 :0x00);
	gm8773c_write(client,0xA3,DISPLAY_TX1CLK_PN_CHANGE ? 0x94 :0x14);
	gm8773c_write(client,0xAB,gm8773c_basic_cfg.mipi_tx_lpx_num);
	gm8773c_write(client,0xA1,MIPI_MODE | TX_CLOCK_MODE);

	gm8773c_write(client,0x06,0x11);    //after config all register, this bit should be set to "1"
	return 0;
}
#if 1

static void gm8773c_dcs_regist(struct i2c_client *client,struct dcs_setting_entry entry[],int num)
{
	u8 index;
	u8 i,j;
	int start=0;
	for(index = start; index < start + num; index++){
		if(entry[index].cmd_name == DELAY){
			mdelay(entry[index].para_list[0]);
		}else{
			if(entry[index].channel_lp == TX0_AND_TX1_SEND)
			{
				gm8773c_write(client,0xed,0x01);// tx0 sends an instruction
				gm8773c_write(client,0xee,0x01);// tx1 sends an instruction
				for(i=0;i<DUAL_PORT;i++){
					gm8773c_write(client,0xef,entry[index].dcs_type);
					if(entry[index].dcs_type == DCS_LONG_WRITE)	{
						gm8773c_write(client,0xef,(entry[index].count) & 0xFF );
						gm8773c_write(client,0xef,0x00);
							for(j=0;j<entry[index].count;j++)
							{
								gm8773c_write(client,0xef,entry[index].para_list[j]);
							}
					}
					else{
							for(j=0;j<entry[index].count;j++)
							{
								gm8773c_write(client,0xef,entry[index].para_list[j]);
							}
					}
				}
			}
			else if(entry[index].channel_lp == TX0_SEND)
			{
				gm8773c_write(client,0xed,0x01);
				gm8773c_write(client,0xee,0x00);
				gm8773c_write(client,0xef,entry[index].dcs_type);
				if(entry[index].dcs_type == DCS_LONG_WRITE)	{
						gm8773c_write(client,0xef,(entry[index].count) & 0xFF );
						gm8773c_write(client,0xef,0x00);
							for(j=0;j<entry[index].count;j++)
							{
								gm8773c_write(client,0xef,entry[index].para_list[j]);
							}
					}
					else{
							for(j=0;j<entry[index].count;j++)
							{
								gm8773c_write(client,0xef,entry[index].para_list[j]);
							}
					}
			}
			else if(entry[index].channel_lp == TX1_SEND)
			{
				gm8773c_write(client,0xed,0x00);
				gm8773c_write(client,0xee,0x01);
				gm8773c_write(client,0xef,entry[index].dcs_type);
				if(entry[index].dcs_type == DCS_LONG_WRITE)	{
						gm8773c_write(client,0xef,(entry[index].count) & 0xFF );
						gm8773c_write(client,0xef,0x00);
							for(j=0;j<entry[index].count;j++)
							{
								gm8773c_write(client,0xef,entry[index].para_list[j]);
							}
					}
					else{
							for(j=0;j<entry[index].count;j++)
							{
								gm8773c_write(client,0xef,entry[index].para_list[j]);
							}
					}
			}
			gm8773c_write(client,0xfb,entry[index].channel_lp);
		}
	}

	pr_info("gm8773 LP Write Finish\n");
}
#endif

static void gm8773c_dcs_regist_f0(struct i2c_client *client,struct dcs_setting_entry entry[],int num)
{
	u8 index;
	int start=0;
	u8 val;
	int ret,i;
	u8 return_reg[]={0xD0,0xD1,0xD2,0xD3,0xD4,0xD5,0xD6,0xD7	\
					,0xD8,0xD9,0xDA,0xDB,0xDC,0xDD,0xDE,0xDF};
	int return_reg_len=sizeof(return_reg)/sizeof(return_reg[0]);
	dev_dbg(&client->dev, "------gm8773c driver read dcs operate start------\n");
	for(index = start; index < start + num; index++){
		if(entry[index].cmd_name == DELAY){
			mdelay(entry[index].para_list[0]);
		}
		else{
			if(entry[index].channel_lp == TX0_AND_TX1_SEND)
			{
 				gm8773c_i2c_write_byte(client,0xf0, entry[index].dcs_type, entry[index].para_list,entry[index].count);
				gm8773c_write(client,0xfb,0xaa);
				gm8773c_write(client,0xfb,0xbb);
				if(entry[index].dcs_type==DCS_SHORT_Read){
					for(i=0 ; i<return_reg_len ; i++){
						ret=gm8773c_read(client,return_reg[i],&val);
						if(ret<0){
							dev_dbg(&client->dev, "gm8773c driver read dcs error\n");
						}else{
							dev_dbg(&client->dev,"gm8773 return reg = %x    value = %x  \n",return_reg[i],val);
						}
					}
				}
			}
			else if(entry[index].channel_lp == TX0_SEND)
			{
				gm8773c_i2c_write_byte(client,0xf0,entry[index].dcs_type, entry[index].para_list,entry[index].count);
				gm8773c_write(client,0xfb,0xaa);
				if(entry[index].dcs_type==DCS_SHORT_Read){
					for(i=0 ; i<return_reg_len/2; i++){
						ret=gm8773c_read(client,return_reg[i],&val);
						if(ret<0){
							dev_dbg(&client->dev, "gm8773c driver read dcs error\n");
						}else{
							dev_dbg(&client->dev,"gm8773 return reg = %x    value = %x  \n",return_reg[i],val);
						}
					}
				}
			}
			else if(entry[index].channel_lp == TX1_SEND)
			{
				gm8773c_i2c_write_byte(client,0xf0,entry[index].dcs_type,entry[index].para_list,entry[index].count);
				gm8773c_write(client,0xfb,0xbb);
				if(entry[index].dcs_type==DCS_SHORT_Read){
					for(i=0 ; i<return_reg_len ; i++){
						ret=gm8773c_read(client,return_reg[i],&val);
						if(ret<0){
							dev_dbg(&client->dev, "gm8773c driver read dcs error\n");
						}else{
							dev_dbg(&client->dev,"gm8773 return reg = %x    value = %x  \n",return_reg[i+(return_reg_len/2)],val);
						}
					}
				}
			}
		}
	}

	dev_dbg(&client->dev, "------gm8773c driver read dcs operate end------\n");
}

static void gm8773c_dcs_init(struct i2c_client *client)
{
	int dcs_custom_table_tx0_len=sizeof(dcs_custom_table_tx0)/sizeof(dcs_custom_table_tx0[0]);
	int dcs_custom_table_tx1_len=sizeof(dcs_custom_table_tx1)/sizeof(dcs_custom_table_tx1[0]);
	int dcs_display_on_len=sizeof(dcs_display_on)/sizeof(dcs_display_on[0]);
	#if 0
	int dcs_custom_table_regist_len=sizeof(dcs_custom_table_regist)/sizeof(dcs_custom_table_regist[0]);
	#endif
	int dcs_read_reg_len=sizeof(dcs_read_reg)/sizeof(dcs_read_reg[0]);

	/*屏幕初始化指令 */
	gm8773c_dcs_regist(client,dcs_custom_table_tx0,dcs_custom_table_tx0_len);//Tx0 Send
	gm8773c_dcs_regist(client,dcs_custom_table_tx1,dcs_custom_table_tx1_len);//Tx1 Send
	/*屏幕display on */
	gm8773c_dcs_regist(client,dcs_display_on,dcs_display_on_len);
	mdelay(50);
	#if 0
	gm8773c_dcs_regist_f0(client,dcs_custom_table_regist,dcs_custom_table_regist_len);
	#endif
	/*读取屏幕寄存器 */
	gm8773c_dcs_regist_f0(client,dcs_read_reg,dcs_read_reg_len);
}

void gm8773c_set_output(struct i2c_client *client)
{
	gm8773c_write(client,0xFF,0x01);//config finish
	dev_dbg(&client->dev,"chip have cfg done\n");
#if GM8773C_BIST
	gm8773c_write(client,0xC9,0x05);
	dev_dbg(&client->dev,"chip in test mode\n");
#endif
}

void gm8773c_init(struct i2c_client *client)
{
	gm8773c_debug(client);
	gm8773c_password(client);
	gm8773c_write(client,0xff,0x00);
	gm8773c_write(client,0x09,0x00);
	gm8773c_write(client,0x14,REFCLK_SECELT);
	gm8773c_pllsetting(client);
	gm8773c_display_timming(client);
	gm8773c_display_mode(client);
	gm8773c_dcs_init(client);
	gm8773c_set_output(client);
	mdelay(50);
}

void gm8773c_config_init(struct i2c_client *client)
{
	// gm8773c_basic_cfg.device_addr=client->addr;
	pr_info("gm8773c_config_init\n");
	gm8773c_basic_cfg.tx0_hactive=TX0_HACTIVE;
	gm8773c_basic_cfg.tx1_hactive=TX1_HACTIVE;
	gm8773c_basic_cfg.hfront_porch=TX_HFP;
	gm8773c_basic_cfg.hsync_width=TX_HSW;
	gm8773c_basic_cfg.hback_porch=TX_HBP;
	gm8773c_basic_cfg.tx0_htotal=TX0_HTOTAL;
	gm8773c_basic_cfg.tx0_vactive=TX0_VACTIVE;
	gm8773c_basic_cfg.tx1_vactive=TX1_VACTIVE;
	gm8773c_basic_cfg.vfront_porch=TX_VFP;
	gm8773c_basic_cfg.vsync_width=TX_VSW;
	gm8773c_basic_cfg.vback_porch=TX_VBP;
	gm8773c_basic_cfg.tx0_vtotal=TX0_VTOTAL;
	gm8773c_basic_cfg.Rx_Lane_num=MIPI_RX_LANE;
	gm8773c_basic_cfg.Tx_Lane_num=MIPI_TX_LANE;
	if(REFCLK_SECELT==REFCLK_MIPI){
		gm8773c_basic_cfg.select_refclk=REFCLK_SECELT;
		if(DISPLAY_MODE==left_and_right_mode || DISPLAY_MODE==Parity_mode) {
			gm8773c_basic_cfg.mipi_rx_clk=((TX0_HTOTAL+TX1_HTOTAL)*TX0_VTOTAL*RXMIPI_FPS*(24/gm8773c_basic_cfg.Rx_Lane_num/2));
			gm8773c_basic_cfg.mipi_tx_clk=gm8773c_basic_cfg.mipi_rx_clk*1.4;
			gm8773c_basic_cfg.refclk_div=20;
			gm8773c_basic_cfg.refclk_int=(((gm8773c_basic_cfg.mipi_tx_clk/1000000)*2*gm8773c_basic_cfg.refclk_div)/(gm8773c_basic_cfg.mipi_rx_clk/1000000));
		}
		else{
			gm8773c_basic_cfg.mipi_rx_clk=((TX0_HTOTAL*TX1_VTOTAL)*RXMIPI_FPS)*(24/gm8773c_basic_cfg.Rx_Lane_num/2);
			gm8773c_basic_cfg.mipi_tx_clk=((TX0_HTOTAL+TX1_HTOTAL)*TX0_VTOTAL*RXMIPI_FPS)*(24/gm8773c_basic_cfg.Rx_Lane_num/2)*1.2;
			gm8773c_basic_cfg.refclk_div=20;
			gm8773c_basic_cfg.refclk_int=(((gm8773c_basic_cfg.mipi_tx_clk/1000000)*2*gm8773c_basic_cfg.refclk_div)/(gm8773c_basic_cfg.mipi_rx_clk/1000000));
		} 
	}
	else if(REFCLK_SECELT==REFCLK_XTAL){
		gm8773c_basic_cfg.select_refclk=REFCLK_SECELT;
		if(DISPLAY_MODE==left_and_right_mode || DISPLAY_MODE==Parity_mode) {
			gm8773c_basic_cfg.mipi_rx_clk=((TX0_HTOTAL+TX1_HTOTAL)*TX0_VTOTAL*RXMIPI_FPS*(24/gm8773c_basic_cfg.Rx_Lane_num/2));
			gm8773c_basic_cfg.mipi_tx_clk=gm8773c_basic_cfg.mipi_rx_clk*1.5;
			gm8773c_basic_cfg.refclk_div=2;
			gm8773c_basic_cfg.refclk_int=((gm8773c_basic_cfg.mipi_tx_clk/1000000)/gm8773c_basic_cfg.refclk_div/25*2);
		}
		else{
			gm8773c_basic_cfg.mipi_rx_clk=(TX0_HTOTAL+TX1_HTOTAL)*TX0_VTOTAL*RXMIPI_FPS*(24/gm8773c_basic_cfg.Rx_Lane_num/2);
			gm8773c_basic_cfg.mipi_tx_clk=((TX0_HTOTAL+TX1_HTOTAL)*TX0_VTOTAL*RXMIPI_FPS*(24/gm8773c_basic_cfg.Rx_Lane_num/2))*1.5;
			gm8773c_basic_cfg.refclk_div=2;
			gm8773c_basic_cfg.refclk_int=((gm8773c_basic_cfg.mipi_tx_clk/1000000)/gm8773c_basic_cfg.refclk_div/25*2);
		} 
	}
	else if(REFCLK_SECELT==REFCLK_OSC){
		gm8773c_basic_cfg.select_refclk=REFCLK_SECELT;
		if(DISPLAY_MODE==left_and_right_mode || DISPLAY_MODE==Parity_mode) {
			gm8773c_basic_cfg.mipi_rx_clk=((TX0_HTOTAL+TX1_HTOTAL)*TX0_VTOTAL*RXMIPI_FPS*(24/gm8773c_basic_cfg.Rx_Lane_num/2));
			gm8773c_basic_cfg.mipi_tx_clk=gm8773c_basic_cfg.mipi_rx_clk*1.5;
			gm8773c_basic_cfg.refclk_div=2;
			gm8773c_basic_cfg.refclk_int=(gm8773c_basic_cfg.mipi_tx_clk/1000000/gm8773c_basic_cfg.refclk_div/25*2);
		}
		else{
			gm8773c_basic_cfg.mipi_rx_clk=((TX0_HTOTAL+TX1_HTOTAL)*TX0_VTOTAL*RXMIPI_FPS*(24/gm8773c_basic_cfg.Rx_Lane_num/2));
			gm8773c_basic_cfg.mipi_tx_clk=(TX0_HTOTAL+TX1_HTOTAL)*TX1_VTOTAL*RXMIPI_FPS*(24/gm8773c_basic_cfg.Rx_Lane_num/2)*1.5;
			gm8773c_basic_cfg.refclk_div=2;
			gm8773c_basic_cfg.refclk_int=(gm8773c_basic_cfg.mipi_tx_clk/1000000/gm8773c_basic_cfg.refclk_div/25*2);
		} 
	}
	gm8773c_basic_cfg.refclk_postdiv=1;
	gm8773c_basic_cfg.mipi_tx_lpx_num=MIPI_TX_LPX;
	gm8773c_basic_cfg.display_mode=DISPLAY_MODE;
	gm8773c_basic_cfg.eotp=EOTP_ENABLE;
	gm8773c_basic_cfg.Rx_DateLane_PN_Change=DISPLAY_RXDATA_PN_CHANGE;
	gm8773c_basic_cfg.Rx_ClkLane_PN_Change=DISPLAY_RXCLK_PN_CHANGE;
	gm8773c_basic_cfg.Tx0_ClkLane_PN_Change=DISPLAY_TX0CLK_PN_CHANGE;
	gm8773c_basic_cfg.Tx1_ClkLane_PN_Change=DISPLAY_TX1CLK_PN_CHANGE;
	gm8773c_basic_cfg.Tx0_DataLane_PN_Change=DISPLAY_TX0DATA_PN_CHANGE;
	gm8773c_basic_cfg.Tx1_DataLane_PN_Change=DISPLAY_TX1DATA_PN_CHANGE;
	gm8773c_basic_cfg.gm8773c_tx_mode=DISPLAY_MODE;
	gm8773c_init(client);

}
EXPORT_SYMBOL_GPL(gm8773c_config_init);

static const struct file_operations gm8773c_ops = {
	 .owner = THIS_MODULE,
};

static int gm8773c_probe(struct i2c_client *client,
			const struct i2c_device_id *id)
{
	printk("zrk test gm8773c_probe start\n");
	dev_info(&client->dev, "driver version: %02x.%02x.%02x",
		 DRIVER_VERSION >> 16,
		 (DRIVER_VERSION & 0xFF00) >> 8,
		 DRIVER_VERSION & 0x00FF);
	if (gm8773c_dev.major) {
			gm8773c_dev.devid = MKDEV(gm8773c_dev.major, 0);
			register_chrdev_region(gm8773c_dev.devid, DRIVER_CNT, DRIVER_NAME);
		 }
	else
		 {
			alloc_chrdev_region(&gm8773c_dev.devid, 0, DRIVER_CNT, DRIVER_NAME);
			gm8773c_dev.major = MAJOR(gm8773c_dev.devid);
		 }

	cdev_init(&gm8773c_dev.cdev, &gm8773c_ops);
	cdev_add(&gm8773c_dev.cdev, gm8773c_dev.devid, DRIVER_CNT);
	gm8773c_dev.class = class_create(THIS_MODULE, DRIVER_NAME);
	if (IS_ERR(gm8773c_dev.class))
	{
		return PTR_ERR(gm8773c_dev.class);
	}

	gm8773c_dev.device = device_create(gm8773c_dev.class, NULL, gm8773c_dev.devid, NULL, DRIVER_NAME);

	if (IS_ERR(gm8773c_dev.device)) {
		return PTR_ERR(gm8773c_dev.device);
	}
	gm8773c_dev.private_data = client;

    gm8773c_global_client = client;

	//gm8773c_config_init(client);

	printk("zrk test gm8773c_probe end\n");
	return 0;
}

static int gm8773c_remove(struct i2c_client *client)
{
	cdev_del(&gm8773c_dev.cdev); 
	 unregister_chrdev_region(gm8773c_dev.devid, DRIVER_CNT);
	  /* unregister classes and device */ 
	 device_destroy(gm8773c_dev.class, gm8773c_dev.devid); 
	 class_destroy(gm8773c_dev.class);
	return 0;
}

static const struct i2c_device_id gm8773c_id[] = {
	{ "corpro,gm8773c", 0 },
	{ /* sentinel */ },
};
MODULE_DEVICE_TABLE(i2c, gm8773c_id);

#if IS_ENABLED(CONFIG_OF)
static const struct of_device_id gm8773c_of_match[] = {
	{ .compatible = "corpro,gm8773c" },
	{ /* sentinel */ },
};
MODULE_DEVICE_TABLE(of, gm8773c_of_match);
#endif

static struct i2c_driver gm8773c_i2c_driver = {
	.driver = {
		.name	= DRIVER_NAME,
		.of_match_table = of_match_ptr(gm8773c_of_match),
	},
	.probe		= gm8773c_probe,
	.remove		= gm8773c_remove,
	.id_table	= gm8773c_id,
};

static int __init gm8773c_mod_init(void)
{
	printk("zrk test gm8773c_mod_init\n");
	return i2c_add_driver(&gm8773c_i2c_driver);
}

static void __exit gm8773c_mod_exit(void)
{
	i2c_del_driver(&gm8773c_i2c_driver);
}

device_initcall_sync(gm8773c_mod_init);
module_exit(gm8773c_mod_exit);
MODULE_DESCRIPTION("GM8773C driver");
MODULE_LICENSE("GPL v2");
#endif