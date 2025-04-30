// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2022 MediaTek Inc.
 */

#include <linux/backlight.h>
#include <drm/drm_mipi_dsi.h>
#include <drm/drm_panel.h>
#include <drm/drm_modes.h>
#include <linux/delay.h>
#include <drm/drm_connector.h>
#include <drm/drm_device.h>

#include <linux/gpio/consumer.h>
#include <linux/regulator/consumer.h>

#include <video/mipi_display.h>
#include <video/of_videomode.h>
#include <video/videomode.h>

#include <linux/module.h>
#include <linux/of_platform.h>
#include <linux/of_graph.h>
#include <linux/platform_device.h>

#include "./it6151_drv/it6151_edp.h"
#include "./it6151_drv/it6151_mipirx.h"

#define CONFIG_MTK_PANEL_EXT
#if defined(CONFIG_MTK_PANEL_EXT)
#include "../mediatek/mediatek_v2/mtk_panel_ext.h"
#include "../mediatek/mediatek_v2/mtk_drm_graphics_base.h"
#endif

#ifdef CONFIG_MTK_ROUND_CORNER_SUPPORT
#include "../mediatek/mediatek_v2/mtk_corner_pattern/mtk_data_hw_roundedpattern.h"
#endif

#define HSA (40)//(32)
#define HBP (75)//(65)
#define HFP (45)//(65)
#define VSA (4) //(6)
#define VBP (30) //(3)
#define VFP (26) //(3)
#define VAC (1200)	//vtotal:1260 //1212
#define HAC (1920)	//htotal:2080 //2082

struct lcm {
	struct device *dev;
	struct drm_panel panel;
	struct backlight_device *backlight;
	struct gpio_desc *vdd_en_gpio;
	struct gpio_desc *reset_gpio;
	struct gpio_desc *bl_en_gpio;
	struct gpio_desc *ivdd12_en_gpio;
	struct gpio_desc *ovdd18_en_gpio;

	bool prepared;
	bool enabled;

	int error;
};

struct lcm *g_ctx;
EXPORT_SYMBOL(g_ctx);

#if 1
/////////////////////////////////////////////////////////////////////
///       for it6151 defines start                   ///////////////////////////////////////
/////////////////////////////////////////////////////////////////////

//#define PANEL_RESOLUTION_1280x800_NOUFO
//#define PANEL_RESOLUTION_2048x1536_NOUFO_18B
//#define PANEL_RESOLUTION_2048x1536
// #define PANEL_RESOLUTION_2048x1536_NOUFO // FOR INTEL Platform
#define PANEL_RESOLUTION_1920x1200p60RB
//#define PANEL_RESOLUTION_1920x1080p60
//#define PANEL_RESULUTION_1536x2048
//#define PANEL_RESOLUTION_1368x768// -->> PANEL_RESOLUTION_1366x768

#define MIPI_4_LANE 	(3)
#define MIPI_3_LANE 	(2)
#define MIPI_2_LANE 	(1)
#define MIPI_1_LANE		(0)

// MIPI Packed Pixel Stream
#define RGB_24b         (0x3E)
#define RGB_30b         (0x0D)
#define RGB_36b         (0x1D)
#define RGB_18b_P       (0x1E)
#define RGB_18b_L       (0x2E)
#define YCbCr_16b       (0x2C)
#define YCbCr_20b       (0x0C)
#define YCbCr_24b       (0x1C)

// DPTX reg62[3:0]
#define B_DPTXIN_6Bpp   (0)
#define B_DPTXIN_8Bpp   (1)
#define B_DPTXIN_10Bpp  (2)
#define B_DPTXIN_12Bpp  (3)

#define B_LBR    		(1)
#define B_HBR    		(0)

#define B_4_LANE 		(3)
#define B_2_LANE 		(1)
#define B_1_LANE 		(0)

#define B_SSC_ENABLE   	(1)
#define B_SSC_DISABLE   (0)

///////////////////////////////////////////////////////////////////////////
//CONFIGURE
///////////////////////////////////////////////////////////////////////////
#define TRAINING_BITRATE	(B_HBR)
#define DPTX_SSC_SETTING	(B_SSC_DISABLE)//(B_SSC_ENABLE)//(B_SSC_DISABLE)
#define HIGH_PCLK			(1)
#define MP_MCLK_INV			(1)
#define MP_CONTINUOUS_CLK	(1)
#define MP_LANE_DESKEW		(1)
#define MP_PCLK_DIV			(2)
#define MP_LANE_SWAP		(0)
#define MP_PN_SWAP			(0)

#define DP_PN_SWAP			(0)
#define DP_AUX_PN_SWAP		(0)
#define DP_LANE_SWAP		(0)	//(0) our convert board need to LANE SWAP for data lane
#define FRAME_RESYNC		(0)
#define LVDS_LANE_SWAP		(0)
#define LVDS_PN_SWAP		(0)
#define LVDS_DC_BALANCE		(0)

#define LVDS_6BIT			(1) // '0' for 8 bit, '1' for 6 bit
#define VESA_MAP		    (1) // '0' for JEIDA , '1' for VESA MAP

#define INT_MASK			(3)
#define MIPI_INT_MASK		(0)
#define TIMER_CNT			(0x0A)
///////////////////////////////////////////////////////////////////////
// Global Setting
///////////////////////////////////////////////////////////////////////
#ifdef PANEL_RESOLUTION_1280x800_NOUFO
#define PANEL_WIDTH 1280
#define VIC 0
#define MP_HPOL 0
#define MP_VPOL 1
#define DPTX_LANE_COUNT  B_2_LANE
#define MIPI_LANE_COUNT  MIPI_4_LANE
#define EN_UFO 0
#define MIPI_PACKED_FMT		RGB_24b
#define MP_H_RESYNC			1
#define MP_V_RESYNC			0
#endif

#ifdef PANEL_RESOLUTION_1920x1080p60
#define PANEL_WIDTH 1920
#define VIC 0x10
#define MP_HPOL 1
#define MP_VPOL 1
#define DPTX_LANE_COUNT  B_2_LANE
#define MIPI_LANE_COUNT  MIPI_4_LANE
#define EN_UFO 0
#define MIPI_PACKED_FMT		RGB_18b_L
#define MP_H_RESYNC			1
#define MP_V_RESYNC			0
#endif

#ifdef PANEL_RESOLUTION_1920x1200p60RB
#define PANEL_WIDTH 1920
#define VIC 0 // non-Zero value for CEA setting, check the given input format.
#define MP_HPOL 1
#define MP_VPOL 0
#define DPTX_LANE_COUNT  B_2_LANE
#define MIPI_LANE_COUNT  MIPI_4_LANE
#define EN_UFO 0
#define MIPI_PACKED_FMT		RGB_24b//RGB_18b_P
#define MP_H_RESYNC			1
#define MP_V_RESYNC			0
#endif

#ifdef PANEL_RESOLUTION_2048x1536
#define PANEL_WIDTH 2048
#define VIC 0 // non-Zero value for CEA setting, check the given input format.
#define MP_HPOL 0
#define MP_VPOL 1
#define MIPI_LANE_COUNT  MIPI_4_LANE
#define DPTX_LANE_COUNT  B_4_LANE
#define EN_UFO 1
#define MIPI_PACKED_FMT		RGB_24b
#define MP_H_RESYNC			0
#define MP_V_RESYNC			0
#endif

#ifdef PANEL_RESOLUTION_2048x1536_NOUFO
#define PANEL_WIDTH 2048
#define VIC 0 // non-Zero value for CEA setting, check the given input format.
#define MP_HPOL 0
#define MP_VPOL 1
#define MIPI_LANE_COUNT  MIPI_4_LANE
#define DPTX_LANE_COUNT  B_4_LANE
#define EN_UFO 0
#define MIPI_PACKED_FMT		RGB_24b
#define MP_H_RESYNC			1
#define MP_V_RESYNC			0
#endif

#ifdef PANEL_RESOLUTION_2048x1536_NOUFO_18B
#define PANEL_WIDTH 2048
#define VIC 0 // non-Zero value for CEA setting, check the given input format.
#define MP_HPOL 0
#define MP_VPOL 1
#define MIPI_LANE_COUNT  MIPI_4_LANE
#define DPTX_LANE_COUNT  B_4_LANE
#define EN_UFO 0
#define MIPI_PACKED_FMT		RGB_18b_P
#define MP_H_RESYNC			1
#define MP_V_RESYNC			0
#endif

#ifdef PANEL_RESULUTION_1536x2048
#define PANEL_WIDTH 1536
#define VIC 0 // non-Zero value for CEA setting, check the given input format.
#define MP_HPOL 0
#define MP_VPOL 1
#define MIPI_LANE_COUNT  MIPI_4_LANE
#define DPTX_LANE_COUNT  B_4_LANE
#define EN_UFO 1
#define MIPI_PACKED_FMT		RGB_24b
#define MP_H_RESYNC			1
#define MP_V_RESYNC			0
#endif

#ifdef PANEL_RESOLUTION_1368x768 // -->> PANEL_RESOLUTION_1366x768
#define PANEL_WIDTH 1368
#define VIC 0
#define MP_HPOL 0
#define MP_VPOL 1
#define DPTX_LANE_COUNT  B_1_LANE
#define MIPI_LANE_COUNT  MIPI_4_LANE
#define EN_UFO 0
#define MIPI_PACKED_FMT		RGB_24b
#define MP_H_RESYNC			1
#define MP_V_RESYNC			0
#endif

#define MIPI_EVENT_MODE		(0)
#define	MIPI_HSYNC_W		(8)
#define MIPI_VSYNC_W		(2)

#define DP_I2C_ADDR 	(0x5C << 0)
#define MIPI_I2C_ADDR 	(0x6C << 0)

extern int it6151_i2c_read_byte(unsigned char dev_addr, unsigned char addr, unsigned char *returnData);
extern int it6151_i2c_write_byte(unsigned char dev_addr, unsigned char addr, unsigned char writeData);
extern int it6151_mipirx_i2c_read_byte(unsigned char dev_addr, unsigned char addr, unsigned char *returnData);
extern int it6151_mipirx_i2c_write_byte(unsigned char dev_addr, unsigned char addr, unsigned char writeData);


/////////////////////////////////////////////////////////////////////
///       for it6151 defines end                   /////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
// Function
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

static void it6151_dumpreg(void)
{
	unsigned char read_mipi_data[14] = {0};
	unsigned char read_dp_data[2] = {0};

	pr_info(">>>>>>>>>>>>dump it6151 start<<<<<<<<<<<\n");
	
	it6151_mipirx_i2c_read_byte(MIPI_I2C_ADDR, 0x0D, &read_mipi_data[0]);
	it6151_mipirx_i2c_read_byte(MIPI_I2C_ADDR, 0x31, &read_mipi_data[1]);
	it6151_mipirx_i2c_read_byte(MIPI_I2C_ADDR, 0x33, &read_mipi_data[2]);
	it6151_mipirx_i2c_read_byte(MIPI_I2C_ADDR, 0x35, &read_mipi_data[3]);
	it6151_mipirx_i2c_read_byte(MIPI_I2C_ADDR, 0x37, &read_mipi_data[4]);
	it6151_mipirx_i2c_read_byte(MIPI_I2C_ADDR, 0x39, &read_mipi_data[5]);
	it6151_mipirx_i2c_read_byte(MIPI_I2C_ADDR, 0x3B, &read_mipi_data[6]);
	it6151_mipirx_i2c_read_byte(MIPI_I2C_ADDR, 0x3F, &read_mipi_data[7]);
	it6151_mipirx_i2c_read_byte(MIPI_I2C_ADDR, 0x41, &read_mipi_data[8]);
	it6151_mipirx_i2c_read_byte(MIPI_I2C_ADDR, 0x43, &read_mipi_data[9]);
	it6151_mipirx_i2c_read_byte(MIPI_I2C_ADDR, 0x51, &read_mipi_data[10]);
	it6151_mipirx_i2c_read_byte(MIPI_I2C_ADDR, 0x53, &read_mipi_data[11]);
	it6151_mipirx_i2c_read_byte(MIPI_I2C_ADDR, 0x55, &read_mipi_data[12]);
	it6151_mipirx_i2c_read_byte(MIPI_I2C_ADDR, 0x57, &read_mipi_data[13]);

	it6151_i2c_read_byte(DP_I2C_ADDR, 0x0D, &read_dp_data[0]);
	it6151_i2c_read_byte(DP_I2C_ADDR, 0x0E, &read_dp_data[1]);

	usleep_range(10 * 1000, 11 * 1000);
	
	pr_info("read mipi reg 0x0D: 0x%x\n", read_mipi_data[0]);
	pr_info("read mipi reg 0x31: 0x%x\n", read_mipi_data[1]);
	pr_info("read mipi reg 0x33: 0x%x\n", read_mipi_data[2]);
	pr_info("read mipi reg 0x35: 0x%x\n", read_mipi_data[3]);
	pr_info("read mipi reg 0x37: 0x%x\n", read_mipi_data[4]);
	pr_info("read mipi reg 0x39: 0x%x\n", read_mipi_data[5]);
	pr_info("read mipi reg 0x3B: 0x%x\n", read_mipi_data[6]);
	pr_info("read mipi reg 0x3F: 0x%x\n", read_mipi_data[7]);
	pr_info("read mipi reg 0x41: 0x%x\n", read_mipi_data[8]);
	pr_info("read mipi reg 0x43: 0x%x\n", read_mipi_data[9]);
	pr_info("read mipi reg 0x51: 0x%x\n", read_mipi_data[10]);
	pr_info("read mipi reg 0x53: 0x%x\n", read_mipi_data[11]);
	pr_info("read mipi reg 0x55: 0x%x\n", read_mipi_data[12]);
	pr_info("read mipi reg 0x57: 0x%x\n", read_mipi_data[13]);
	
	pr_info("\n");
	
	pr_info("read dp reg 0x0D: 0x%x\n", read_dp_data[0]);
	pr_info("read dp reg 0x0E: 0x%x\n", read_dp_data[1]);
	
	pr_info(">>>>>>>>>>>>dump it6151 end<<<<<<<<<<<\n");
}

static void IT6151_DPTX_init(void)
{   
	it6151_i2c_write_byte(DP_I2C_ADDR,0x05,0x29);
	it6151_i2c_write_byte(DP_I2C_ADDR,0x05,0x00);
	
	it6151_i2c_write_byte(DP_I2C_ADDR,0x09,INT_MASK);// Enable HPD_IRQ,HPD_CHG,VIDSTABLE
	it6151_i2c_write_byte(DP_I2C_ADDR,0x0A,0x00);
	it6151_i2c_write_byte(DP_I2C_ADDR,0x0B,0x00);
	it6151_i2c_write_byte(DP_I2C_ADDR,0xC5,0xC1);
	it6151_i2c_write_byte(DP_I2C_ADDR,0xB5,0x00);
	it6151_i2c_write_byte(DP_I2C_ADDR,0xB7,0x80);
	it6151_i2c_write_byte(DP_I2C_ADDR,0xC4,0xF0);
	it6151_i2c_write_byte(DP_I2C_ADDR,0x06,0xFF);// Clear all interrupt
	it6151_i2c_write_byte(DP_I2C_ADDR,0x07,0xFF);// Clear all interrupt
	it6151_i2c_write_byte(DP_I2C_ADDR,0x08,0xFF);// Clear all interrupt
	
	it6151_i2c_write_byte(DP_I2C_ADDR,0x05,0x00);
	it6151_i2c_write_byte(DP_I2C_ADDR,0x0c,0x08);
	it6151_i2c_write_byte(DP_I2C_ADDR,0x21,0x05);
	it6151_i2c_write_byte(DP_I2C_ADDR,0x3a,0x04);
	it6151_i2c_write_byte(DP_I2C_ADDR,0x5f,0x06);
	it6151_i2c_write_byte(DP_I2C_ADDR,0xc9,0xf5);
	it6151_i2c_write_byte(DP_I2C_ADDR,0xca,0x4c);
	it6151_i2c_write_byte(DP_I2C_ADDR,0xcb,0x37);
	it6151_i2c_write_byte(DP_I2C_ADDR,0xce,0x80);
	it6151_i2c_write_byte(DP_I2C_ADDR,0xd3,0x03);
	it6151_i2c_write_byte(DP_I2C_ADDR,0xd4,0x60);
	it6151_i2c_write_byte(DP_I2C_ADDR,0xe8,0x11);
	it6151_i2c_write_byte(DP_I2C_ADDR,0xec,VIC);
	usleep_range(5 * 1000, 6 * 1000);

	it6151_i2c_write_byte(DP_I2C_ADDR,0x23,0x42);
	it6151_i2c_write_byte(DP_I2C_ADDR,0x24,0x07);
	it6151_i2c_write_byte(DP_I2C_ADDR,0x25,0x01);
	it6151_i2c_write_byte(DP_I2C_ADDR,0x26,0x00);
	it6151_i2c_write_byte(DP_I2C_ADDR,0x27,0x10);
	it6151_i2c_write_byte(DP_I2C_ADDR,0x2B,0x05);
	it6151_i2c_write_byte(DP_I2C_ADDR,0x23,0x40);
	it6151_i2c_write_byte(DP_I2C_ADDR,0x22,(DP_AUX_PN_SWAP<<3)|(DP_PN_SWAP<<2)|0x03);
	it6151_i2c_write_byte(DP_I2C_ADDR,0x16,(DPTX_SSC_SETTING<<4)|(DP_LANE_SWAP<<3)|(DPTX_LANE_COUNT<<1)|TRAINING_BITRATE);
	it6151_i2c_write_byte(DP_I2C_ADDR,0x62,B_DPTXIN_8Bpp);
	it6151_i2c_write_byte(DP_I2C_ADDR,0x0f,0x01);
	it6151_i2c_write_byte(DP_I2C_ADDR,0x76,0xa7);
	it6151_i2c_write_byte(DP_I2C_ADDR,0x77,0xaf);
	it6151_i2c_write_byte(DP_I2C_ADDR,0x7e,0x8f);
	it6151_i2c_write_byte(DP_I2C_ADDR,0x7f,0x07);
	it6151_i2c_write_byte(DP_I2C_ADDR,0x80,0xef);
	it6151_i2c_write_byte(DP_I2C_ADDR,0x81,0x5f);
	it6151_i2c_write_byte(DP_I2C_ADDR,0x82,0xef);
	it6151_i2c_write_byte(DP_I2C_ADDR,0x83,0x07);
	it6151_i2c_write_byte(DP_I2C_ADDR,0x88,0x38);
	it6151_i2c_write_byte(DP_I2C_ADDR,0x89,0x1f);
	it6151_i2c_write_byte(DP_I2C_ADDR,0x8a,0x48);
	it6151_i2c_write_byte(DP_I2C_ADDR,0x0f,0x00);
	it6151_i2c_write_byte(DP_I2C_ADDR,0x5c,0xf3);
	it6151_i2c_write_byte(DP_I2C_ADDR,0x17,0x04);
	it6151_i2c_write_byte(DP_I2C_ADDR,0x17,0x01);
	it6151_i2c_write_byte(DP_I2C_ADDR,0x19,0xff);
	it6151_i2c_write_byte(DP_I2C_ADDR,0x1a,0xff);
	usleep_range(5 * 1000, 6 * 1000);

	pr_debug(">>>>11 %s\n", __func__);
}

static int IT6151_init(void)
{
	unsigned char VenID[2] = {0};
	unsigned char DevID[2] = {0}; 
	unsigned char RevID = {0};
	//unsigned char cmdBuffer;
	
	int ret = -1;
	ret = it6151_i2c_read_byte(DP_I2C_ADDR, 0x00, &VenID[0]);

	if(ret != 0)
		pr_err("[LCM]it6151_i2c_read_byte,failed ret:%d\n", ret);

	it6151_i2c_read_byte(DP_I2C_ADDR, 0x01, &VenID[1]);
	it6151_i2c_read_byte(DP_I2C_ADDR, 0x02, &DevID[0]);
	it6151_i2c_read_byte(DP_I2C_ADDR, 0x03, &DevID[1]);
	it6151_i2c_read_byte(DP_I2C_ADDR, 0x04, &RevID);	
		
	if( VenID[0]==0x54 && VenID[1]==0x49 && DevID[0]==0x51 && DevID[1]==0x61 )
	{
		it6151_i2c_write_byte(DP_I2C_ADDR,0x05,0x04);// DP SW Reset
		it6151_i2c_write_byte(DP_I2C_ADDR,0xfd,(MIPI_I2C_ADDR<<1)|1);
		it6151_mipirx_i2c_write_byte(MIPI_I2C_ADDR,0x05,0x00);
		it6151_mipirx_i2c_write_byte(MIPI_I2C_ADDR,0x0c,(MP_LANE_SWAP<<7)|(MP_PN_SWAP<<6)|(MIPI_LANE_COUNT<<4)|EN_UFO);
		it6151_mipirx_i2c_write_byte(MIPI_I2C_ADDR,0x11,MP_MCLK_INV);

        	if(RevID == 0xA1)
		{			
		    it6151_mipirx_i2c_write_byte(MIPI_I2C_ADDR,0x19, MP_LANE_DESKEW); 
		}
		else
		{
		    it6151_mipirx_i2c_write_byte(MIPI_I2C_ADDR,0x19,(MP_CONTINUOUS_CLK<<1) | MP_LANE_DESKEW); 
   		}
				
		it6151_mipirx_i2c_write_byte(MIPI_I2C_ADDR,0x27, MIPI_PACKED_FMT);
		it6151_mipirx_i2c_write_byte(MIPI_I2C_ADDR,0x28,((PANEL_WIDTH/4-1)>>2)&0xC0);
		it6151_mipirx_i2c_write_byte(MIPI_I2C_ADDR,0x29,(PANEL_WIDTH/4-1)&0xFF);
		
		it6151_mipirx_i2c_write_byte(MIPI_I2C_ADDR,0x2e,0x34);
		it6151_mipirx_i2c_write_byte(MIPI_I2C_ADDR,0x2f,0x01);
		
		
		it6151_mipirx_i2c_write_byte(MIPI_I2C_ADDR,0x4e,(MP_V_RESYNC<<3)|(MP_H_RESYNC<<2)|(MP_VPOL<<1)|(MP_HPOL));
		it6151_mipirx_i2c_write_byte(MIPI_I2C_ADDR,0x80,(EN_UFO<<5)|MP_PCLK_DIV);
		it6151_mipirx_i2c_write_byte(MIPI_I2C_ADDR,0x84,0x8f);
		it6151_mipirx_i2c_write_byte(MIPI_I2C_ADDR,0x09,MIPI_INT_MASK);
		it6151_mipirx_i2c_write_byte(MIPI_I2C_ADDR,0x92,TIMER_CNT);	

		#if (MIPI_EVENT_MODE == 1)
		it6151_mipirx_i2c_write_byte(MIPI_I2C_ADDR,0x33,0x80 | MIPI_HSYNC_W >> 8);
		it6151_mipirx_i2c_write_byte(MIPI_I2C_ADDR,0x32,MIPI_HSYNC_W & 0xFF);
		it6151_mipirx_i2c_write_byte(MIPI_I2C_ADDR,0x3D,0x80 | MIPI_VSYNC_W >> 8);
		it6151_mipirx_i2c_write_byte(MIPI_I2C_ADDR,0x3C,MIPI_VSYNC_W & 0xFF);
		#endif	

		IT6151_DPTX_init();

		return 0;
	}

	it6151_mipirx_i2c_read_byte(MIPI_I2C_ADDR, 0x00, &VenID[0]);
	it6151_mipirx_i2c_read_byte(MIPI_I2C_ADDR, 0x01, &VenID[1]);
	it6151_mipirx_i2c_read_byte(MIPI_I2C_ADDR, 0x02, &DevID[0]);
	it6151_mipirx_i2c_read_byte(MIPI_I2C_ADDR, 0x03, &DevID[1]);
	it6151_mipirx_i2c_read_byte(MIPI_I2C_ADDR, 0x04, &RevID);

	if( VenID[0]==0x54 && VenID[1]==0x49 && DevID[0]==0x51 && DevID[1]==0x61 )
	{
		it6151_i2c_write_byte(DP_I2C_ADDR,0x05,0x04);// DP SW Reset
		it6151_i2c_write_byte(DP_I2C_ADDR,0xfd,(MIPI_I2C_ADDR<<1)|1);
		it6151_mipirx_i2c_write_byte(MIPI_I2C_ADDR,0x05,0x00);
		it6151_mipirx_i2c_write_byte(MIPI_I2C_ADDR,0x0c,(MP_LANE_SWAP<<7)|(MP_PN_SWAP<<6)|(MIPI_LANE_COUNT<<4)|EN_UFO);
		it6151_mipirx_i2c_write_byte(MIPI_I2C_ADDR,0x11,MP_MCLK_INV);

		if(RevID == 0xA1)
		{			
			it6151_mipirx_i2c_write_byte(MIPI_I2C_ADDR,0x19, MP_LANE_DESKEW); 
		}
		else
		{
			it6151_mipirx_i2c_write_byte(MIPI_I2C_ADDR,0x19,(MP_CONTINUOUS_CLK<<1) | MP_LANE_DESKEW); 
		}
					
		it6151_mipirx_i2c_write_byte(MIPI_I2C_ADDR,0x27, MIPI_PACKED_FMT);
		it6151_mipirx_i2c_write_byte(MIPI_I2C_ADDR,0x28,((PANEL_WIDTH/4-1)>>2)&0xC0);
		it6151_mipirx_i2c_write_byte(MIPI_I2C_ADDR,0x29,(PANEL_WIDTH/4-1)&0xFF);
		
		it6151_mipirx_i2c_write_byte(MIPI_I2C_ADDR,0x2e,0x34);
		it6151_mipirx_i2c_write_byte(MIPI_I2C_ADDR,0x2f,0x01);
		
		it6151_mipirx_i2c_write_byte(MIPI_I2C_ADDR,0x4e,(MP_V_RESYNC<<3)|(MP_H_RESYNC<<2)|(MP_VPOL<<1)|(MP_HPOL));
		it6151_mipirx_i2c_write_byte(MIPI_I2C_ADDR,0x80,(EN_UFO<<5)|MP_PCLK_DIV);
		it6151_mipirx_i2c_write_byte(MIPI_I2C_ADDR,0x84,0x8f);
		it6151_mipirx_i2c_write_byte(MIPI_I2C_ADDR,0x09,MIPI_INT_MASK);
		it6151_mipirx_i2c_write_byte(MIPI_I2C_ADDR,0x92,TIMER_CNT);

		#if (MIPI_EVENT_MODE == 1)
		it6151_mipirx_i2c_write_byte(MIPI_I2C_ADDR,0x33,0x80 | MIPI_HSYNC_W >> 8);
		it6151_mipirx_i2c_write_byte(MIPI_I2C_ADDR,0x32,MIPI_HSYNC_W & 0xFF);
		it6151_mipirx_i2c_write_byte(MIPI_I2C_ADDR,0x3D,0x80 | MIPI_VSYNC_W >> 8);
		it6151_mipirx_i2c_write_byte(MIPI_I2C_ADDR,0x3C,MIPI_VSYNC_W & 0xFF);
		#endif	
		
		IT6151_DPTX_init();
	
		return 0;
	}

	if(VenID[0]==0x54 && VenID[1]==0x49 && DevID[0]==0x21 && DevID[1]==0x61 )
	{
		it6151_mipirx_i2c_write_byte(MIPI_I2C_ADDR,0x05,0x33);
		it6151_mipirx_i2c_write_byte(MIPI_I2C_ADDR,0x05,0x40);
		it6151_mipirx_i2c_write_byte(MIPI_I2C_ADDR,0x05,0x00);
		it6151_mipirx_i2c_write_byte(MIPI_I2C_ADDR,0x0c,(MP_LANE_SWAP<<7)|(MP_PN_SWAP<<6)|(MIPI_LANE_COUNT<<4));
		it6151_mipirx_i2c_write_byte(MIPI_I2C_ADDR,0x11, MP_MCLK_INV); 
		it6151_mipirx_i2c_write_byte(MIPI_I2C_ADDR,0x19,(MP_CONTINUOUS_CLK<<1) | MP_LANE_DESKEW);  
		it6151_mipirx_i2c_write_byte(MIPI_I2C_ADDR,0x4B,(FRAME_RESYNC<<4));
		it6151_mipirx_i2c_write_byte(MIPI_I2C_ADDR,0x4E,(MP_V_RESYNC<<3)|(MP_H_RESYNC<<2)|(MP_VPOL<<1)|(MP_HPOL));      
		it6151_mipirx_i2c_write_byte(MIPI_I2C_ADDR,0x72,0x01); 
		it6151_mipirx_i2c_write_byte(MIPI_I2C_ADDR,0x73,0x03); 
		it6151_mipirx_i2c_write_byte(MIPI_I2C_ADDR,0x80,MP_PCLK_DIV); 
		it6151_mipirx_i2c_write_byte(MIPI_I2C_ADDR,0xC0,(HIGH_PCLK<< 4) | 0x0F);   
		it6151_mipirx_i2c_write_byte(MIPI_I2C_ADDR,0xC1,0x01);  
		it6151_mipirx_i2c_write_byte(MIPI_I2C_ADDR,0xC2,0x47);  
		it6151_mipirx_i2c_write_byte(MIPI_I2C_ADDR,0xC3,0x67);  
		it6151_mipirx_i2c_write_byte(MIPI_I2C_ADDR,0xC4,0x04);  
		it6151_mipirx_i2c_write_byte(MIPI_I2C_ADDR,0xCB,(LVDS_PN_SWAP<<5)|(LVDS_LANE_SWAP<<4)|(LVDS_6BIT<<2)|(LVDS_DC_BALANCE<<1)| VESA_MAP);  

		#if (MIPI_EVENT_MODE == 1)
		it6151_mipirx_i2c_write_byte(MIPI_I2C_ADDR,0x33,0x80 | MIPI_HSYNC_W >> 8);
		it6151_mipirx_i2c_write_byte(MIPI_I2C_ADDR,0x32,MIPI_HSYNC_W & 0xFF);
		it6151_mipirx_i2c_write_byte(MIPI_I2C_ADDR,0x3D,0x80 | MIPI_VSYNC_W >> 8);
		it6151_mipirx_i2c_write_byte(MIPI_I2C_ADDR,0x3C,MIPI_VSYNC_W & 0xFF);
		#endif	
		return 0;
  	}
	
	return -1;
}

static void it6151_showInfo(void)
{
	u8 Reg3x[0x10] = {0};
	u8 Reg4x[0x4] = {0};
	u8 Reg5x[0x0A] = {0};
	u8 MipiReg0D = 0;
	u8 MipiReg = 0;	
	u8 DpReg0D = 0;
	u8 DpReg = 0;
	u8 i;

	u16 PHSS, PHSE, PHDES, PHDEE, PHTotal;
	u16 PVSS, PVSE, PVDES, PVDEE, PVTotal;
	u16 MHSS, MHSE, MHDES, MHDEE, MHTotal;
	
	pr_info("\r\n=====================ShowInfo()====================\r\n");
	//Mipi Reg 0x30~0x3F
	for (i = 0; i < 0x10; i++)
	{
		it6151_mipirx_i2c_read_byte(MIPI_I2C_ADDR, (0x30 + i), &(Reg3x[i]));
		//pr_info("Reg[%02x]=%02x\r\n", (0x30 + i), Reg3x[i]);
	}
	//Mipi Reg 0x40 ~ 0x43
	for (i = 0; i < 0x4; i++)
	{
		it6151_mipirx_i2c_read_byte(MIPI_I2C_ADDR, (0x40 + i), &(Reg4x[i]));
		//pr_info("Reg[%02x]=%02x\r\n", (0x40 + i), Reg4x[i]);
	}
	//Mipi 0x50~0x59
	for (i = 0; i < 0xA; i++)
	{
		it6151_mipirx_i2c_read_byte(MIPI_I2C_ADDR, (0x50 + i), &(Reg5x[i]));
		//pr_info("Reg[%02x]=%02x\r\n", (0x50 + i), Reg5x[i]);
	}


	pr_info("\r\nDPInfo ...\r\n");
	//DP Reg 0x0D
	it6151_i2c_read_byte(DP_I2C_ADDR, 0x0D, &DpReg0D);
	pr_info("DP Reg[0x0D]=%02x\r\n",DpReg0D);
	pr_info("   HPD Status is %s\r\n", ((DpReg0D&0x01) ? "Plug":"UnPlug"));	
	pr_info("   Video input Status is %s\r\n", ((DpReg0D&0x02) ? "stable":"Unstable"));

	it6151_i2c_read_byte(DP_I2C_ADDR, 0x16, &DpReg);
	pr_info("DP Reg[0x16]=%02x\r\n",DpReg);	
	pr_info("   Lane Number = %02x\r\n", ((DpReg&0x06) ));
	pr_info("   Link Rate = %s\r\n", ((DpReg&0x80) ? "2.7Gbps":"1.6Gbps"));		

	it6151_i2c_read_byte(DP_I2C_ADDR, 0x0E, &DpReg);
	pr_info("DP Reg[0x0E]=%02x\r\n",DpReg);		
	pr_info("   Link Status = %02x\r\n", ((DpReg&0x1F) ));	
	pr_info("   Lane Swap = %s\r\n", ((DpReg&0x04) ? "Swap":"No Swap"));	
	
	pr_info("\r\nMipiInfo ...\r\n");	
	it6151_mipirx_i2c_read_byte(MIPI_I2C_ADDR, 0x4E, &MipiReg);
	pr_info("MMIPI Reg[0x4E]=%02x\r\n",MipiReg);	
	pr_info("  RegHReSyncEn is %s\r\n", ((MipiReg&0x04) ? "Enable":"DisEnable"));		
	pr_info("  RegVReSyncEn is %s\r\n", ((MipiReg&0x08) ? "Enable":"DisEnable"));			

	it6151_mipirx_i2c_read_byte(MIPI_I2C_ADDR, 0x2F, &MipiReg);
	pr_info("MMIPI Reg[0x2F]=%02x\r\n",MipiReg);	
	pr_info("  UFO re-sync is %s\r\n", ((MipiReg&0x01) ? "Enable":"DisEnable"));		
	
	
	//Mipi Reg 0x0D
	it6151_mipirx_i2c_read_byte(MIPI_I2C_ADDR, 0x0D, &MipiReg0D);
	pr_info("MMIPI Reg[0x0D]=%02x\r\n",MipiReg0D);
	pr_info("   MVidstb is %s\r\n", ((MipiReg0D&0x10) ? "stable":"Unstable"));	
	pr_info("   PVidstb is %s\r\n", ((MipiReg0D&0x20) ? "stable":"Unstable"));	
	
	pr_info("   MCLK %s\r\n", ((MipiReg0D&0x08) ? "off":"on"));
	pr_info("   MVidStb %s\r\n", ((MipiReg0D&0x10) ? "stable":"unstable!!"));
	pr_info("   PVidStb %s\r\n", ((MipiReg0D&0x20) ? "stable":"unstable!!"));
	pr_info("   DP Video input %s\r\n", ((DpReg0D&0x04) ? "stable":"unstable!!"));

	PHSS = Reg3x[0] | ((Reg3x[1] & 0x0F) << 8);
	PHSE = Reg3x[2] | ((Reg3x[3] & 0x0F) << 8);
	PHDES = Reg3x[4] | ((Reg3x[5] & 0x0F) << 8);
	PHDEE = Reg3x[6] | ((Reg3x[7] & 0x0F) << 8);
	PHTotal = Reg3x[8] | ((Reg3x[9] & 0x0F) << 8);
	PVSS = Reg3x[0xA] | ((Reg3x[0xB] & 0x0F) << 8);
	PVSE = Reg3x[0xC] | ((Reg3x[0xD] & 0x0F) << 8);
	PVDES = Reg3x[0xE] | ((Reg3x[0xF] & 0x0F) << 8);
	PVDEE = Reg4x[0x0] | ((Reg4x[0x1] & 0x0F) << 8);
	PVTotal = Reg4x[0x2] | ((Reg4x[0x3] & 0x0F) << 8);
	MHSS = Reg5x[0] | ((Reg5x[1] & 0x0F) << 8);
	MHSE = Reg5x[2] | ((Reg5x[3] & 0x0F) << 8);
	MHDES = Reg5x[4] | ((Reg5x[5] & 0x0F) << 8);
	MHDEE = Reg5x[6] | ((Reg5x[7] & 0x0F) << 8);
	MHTotal = Reg5x[8] | ((Reg5x[9] & 0x0F) << 8);
	pr_info("PHSS    (%4d -1)/4 = %4d ...%d %s\r\n", PHSS, ((PHSS-1)/4),((PHSS - 1)%4), (((PHSS - 1)%4)?"!!!!":""));
	pr_info("PHSE     %4d    /4 = %4d ...%d %s\r\n", PHSE, (PHSE/4),(PHSE%4),(((PHSE)%4)?"!!!!":""));
	pr_info("PHDES   (%4d -1)/4 = %4d ...%d %s\r\n", PHDES, ((PHDES-1)/4),((PHDES - 1)%4),(((PHDES - 1)%4)?"!!!!":""));
	pr_info("PHDEE    %4d    /4 = %4d ...%d %s\r\n", PHDEE, (PHDEE/4),(PHDEE%4),(((PHDEE)%4)?"!!!!":""));
	pr_info("PHActive  %4d\r\n", PHDEE-PHDES+1);	
	pr_info("PHTotal  %4d    /4 = %4d ...%d %s\r\n", PHTotal, (PHTotal/4),(PHTotal%4),(((PHTotal)%4)?"!!!!":""));
	pr_info("PVSS     %4d\r\n", PVSS);
	pr_info("PVSE     %4d\r\n", PVSE);
	pr_info("PVDES    %4d\r\n", PVDES);
	pr_info("PVDEE    %4d\r\n", PVDEE);
	pr_info("PVActive  %4d\r\n", PVDEE-PVDES+1);	
	pr_info("PVTotal  %4d\r\n", PVTotal);
	pr_info("MHSS    (%4d -1)/3 = %4d ...%d %s\r\n", MHSS, ((MHSS-1)/3),((MHSS - 1)%3),(((MHSS-1)%3)?"!!!!":""));
	pr_info("MHSE     %4d    /3 = %4d ...%d %s\r\n", MHSE, (MHSE/3),(MHSE%3),(((MHSE)%3)?"!!!!":""));
	pr_info("MHDES   (%4d -1)/3 = %4d ...%d %s\r\n", MHDES, ((MHDES-1)/3),((MHDES - 1)%3),(((MHDES-1)%3)?"!!!!":""));
	pr_info("MHDEE    %4d    /3 = %4d ...%d %s\r\n", MHDEE, (MHDEE/3),(MHDEE%3),(((MHDEE)%3)?"!!!!":""));
	pr_info("MHTotal  %4d    /3 = %4d ...%d %s\r\n", MHTotal, (MHTotal/3),(MHTotal%3),(((MHTotal)%3)?"!!!!":""));
	pr_info("\r\n==============================================\r\n");
}

static void it6151_mipi_edp_init(void)
{
	if (IT6151_init()){
		usleep_range(100 * 1000, 101 * 1000);
		it6151_dumpreg();
		it6151_showInfo();
	}
}
#endif

static inline struct lcm *panel_to_lcm(struct drm_panel *panel)
{
	return container_of(panel, struct lcm, panel);
}

static int lcm_disable(struct drm_panel *panel)
{
	struct lcm *ctx = panel_to_lcm(panel);

	dev_err(ctx->dev, "zkc %s", __func__);
	if (!ctx->enabled)
		return 0;

	if (ctx->backlight) {
		ctx->backlight->props.power = FB_BLANK_POWERDOWN;
		backlight_update_status(ctx->backlight);
	}

	ctx->enabled = false;

	return 0;
}

int lcm_unprepare_power(void)
{
	struct lcm *ctx;
	unsigned char temp;
	
	ctx = g_ctx;

	pr_err("%s enter\n", __func__);

	it6151_i2c_read_byte(DP_I2C_ADDR, 0x22, &temp);
	temp = temp | 0x10;
	it6151_i2c_write_byte(DP_I2C_ADDR, 0x22, temp);
	usleep_range(20 * 1000, 21 * 1000);
	
	//bl_en off
	ctx->bl_en_gpio = devm_gpiod_get(ctx->dev, "bl_en", GPIOD_OUT_HIGH);
	if (IS_ERR(ctx->bl_en_gpio)) {
		dev_err(ctx->dev, "%s: cannot get bl_en-gpios %ld\n",
			__func__, PTR_ERR(ctx->bl_en_gpio));
		return PTR_ERR(ctx->bl_en_gpio);
	}
	gpiod_set_value(ctx->bl_en_gpio, 0);
	devm_gpiod_put(ctx->dev, ctx->bl_en_gpio);

	//reset off
	ctx->reset_gpio = devm_gpiod_get(ctx->dev, "reset", GPIOD_OUT_HIGH);
	if (IS_ERR(ctx->reset_gpio)) {
		dev_err(ctx->dev, "%s: cannot get reset_gpio %ld\n",
			__func__, PTR_ERR(ctx->reset_gpio));
		return PTR_ERR(ctx->reset_gpio);
	}
	gpiod_set_value(ctx->reset_gpio, 0);
	devm_gpiod_put(ctx->dev, ctx->reset_gpio);

	//vdd33 off
	ctx->vdd_en_gpio = devm_gpiod_get(ctx->dev, "vdd33_en", GPIOD_OUT_HIGH);
	if (IS_ERR(ctx->vdd_en_gpio)) {
		dev_err(ctx->dev, "%s: cannot get vdd_en_gpio %ld\n",
			__func__, PTR_ERR(ctx->vdd_en_gpio));
		return PTR_ERR(ctx->vdd_en_gpio);
	}
	gpiod_set_value(ctx->vdd_en_gpio, 0);
	devm_gpiod_put(ctx->dev, ctx->vdd_en_gpio);

	//ovdd18 off
	ctx->ovdd18_en_gpio = devm_gpiod_get(ctx->dev, "ovdd18_en", GPIOD_OUT_HIGH);
	if (IS_ERR(ctx->ovdd18_en_gpio)) {
		dev_err(ctx->dev, "%s: cannot get ovdd18_en-gpios %ld\n",
			__func__, PTR_ERR(ctx->ovdd18_en_gpio));
		return PTR_ERR(ctx->ovdd18_en_gpio);
	}
	gpiod_set_value(ctx->ovdd18_en_gpio, 0);
	devm_gpiod_put(ctx->dev, ctx->ovdd18_en_gpio);

	//ivdd12 off
	ctx->ivdd12_en_gpio = devm_gpiod_get(ctx->dev, "ivdd12_en", GPIOD_OUT_HIGH);
	if (IS_ERR(ctx->ivdd12_en_gpio)) {
		dev_err(ctx->dev, "%s: cannot get ivdd12_en-gpios %ld\n",
			__func__, PTR_ERR(ctx->ivdd12_en_gpio));
		return PTR_ERR(ctx->ivdd12_en_gpio);
	}
	gpiod_set_value(ctx->ivdd12_en_gpio, 0);
	devm_gpiod_put(ctx->dev, ctx->ivdd12_en_gpio);	

	pr_err("%s out\n", __func__);

	return 0;
}
EXPORT_SYMBOL(lcm_unprepare_power);

static int lcm_unprepare(struct drm_panel *panel)
{
	struct lcm *ctx = panel_to_lcm(panel);
	//unsigned char temp;

	dev_err(ctx->dev, "zkc %s", __func__);
	if (!ctx->prepared)
		return 0;

	ctx->error = 0;
	ctx->prepared = false;

#if 0
	it6151_i2c_read_byte(DP_I2C_ADDR, 0x22, &temp);
	temp = temp | 0x10;
	it6151_i2c_write_byte(DP_I2C_ADDR, 0x22, temp);
	usleep_range(20 * 1000, 21 * 1000);	

	//bl_en off
	ctx->bl_en_gpio = devm_gpiod_get(ctx->dev, "bl_en", GPIOD_OUT_HIGH);
	if (IS_ERR(ctx->bl_en_gpio)) {
		dev_err(ctx->dev, "%s: cannot get bl_en-gpios %ld\n",
			__func__, PTR_ERR(ctx->bl_en_gpio));
		return PTR_ERR(ctx->bl_en_gpio);
	}
	gpiod_set_value(ctx->bl_en_gpio, 0);
	devm_gpiod_put(ctx->dev, ctx->bl_en_gpio);

	//reset off
	ctx->reset_gpio = devm_gpiod_get(ctx->dev, "reset", GPIOD_OUT_HIGH);
	if (IS_ERR(ctx->reset_gpio)) {
		dev_err(ctx->dev, "%s: cannot get reset_gpio %ld\n",
			__func__, PTR_ERR(ctx->reset_gpio));
		return PTR_ERR(ctx->reset_gpio);
	}
	gpiod_set_value(ctx->reset_gpio, 0);
	devm_gpiod_put(ctx->dev, ctx->reset_gpio);

	//vdd33 off
	ctx->vdd_en_gpio = devm_gpiod_get(ctx->dev, "vdd33_en", GPIOD_OUT_HIGH);
	if (IS_ERR(ctx->vdd_en_gpio)) {
		dev_err(ctx->dev, "%s: cannot get vdd_en_gpio %ld\n",
			__func__, PTR_ERR(ctx->vdd_en_gpio));
		return PTR_ERR(ctx->vdd_en_gpio);
	}
	gpiod_set_value(ctx->vdd_en_gpio, 0);
	devm_gpiod_put(ctx->dev, ctx->vdd_en_gpio);

	//ovdd18 off
	ctx->ovdd18_en_gpio = devm_gpiod_get(ctx->dev, "ovdd18_en", GPIOD_OUT_HIGH);
	if (IS_ERR(ctx->ovdd18_en_gpio)) {
		dev_err(ctx->dev, "%s: cannot get ovdd18_en-gpios %ld\n",
			__func__, PTR_ERR(ctx->ovdd18_en_gpio));
		return PTR_ERR(ctx->ovdd18_en_gpio);
	}
	gpiod_set_value(ctx->ovdd18_en_gpio, 0);
	devm_gpiod_put(ctx->dev, ctx->ovdd18_en_gpio);

	//ivdd12 off
	ctx->ivdd12_en_gpio = devm_gpiod_get(ctx->dev, "ivdd12_en", GPIOD_OUT_HIGH);
	if (IS_ERR(ctx->ivdd12_en_gpio)) {
		dev_err(ctx->dev, "%s: cannot get ivdd12_en-gpios %ld\n",
			__func__, PTR_ERR(ctx->ivdd12_en_gpio));
		return PTR_ERR(ctx->ivdd12_en_gpio);
	}
	gpiod_set_value(ctx->ivdd12_en_gpio, 0);
	devm_gpiod_put(ctx->dev, ctx->ivdd12_en_gpio);
#endif

	return 0;
}

int lcm_prepare_power(void)
{
	struct lcm *ctx;
	
	ctx = g_ctx;

	pr_err("%s enter\n", __func__);

	//vdd33
	ctx->vdd_en_gpio = devm_gpiod_get(ctx->dev, "vdd33_en", GPIOD_OUT_HIGH);
	if (IS_ERR(ctx->vdd_en_gpio)) {
		dev_err(ctx->dev, "%s: cannot get vdd_en_gpio %ld\n",
			__func__, PTR_ERR(ctx->vdd_en_gpio));
		return PTR_ERR(ctx->vdd_en_gpio);
	}
	gpiod_set_value(ctx->vdd_en_gpio, 1);
	devm_gpiod_put(ctx->dev, ctx->vdd_en_gpio);

	usleep_range(10 * 1000, 11 * 1000);

	//ivdd12
	ctx->ivdd12_en_gpio = devm_gpiod_get(ctx->dev, "ivdd12_en", GPIOD_OUT_HIGH);
	if (IS_ERR(ctx->ivdd12_en_gpio)) {
		dev_err(ctx->dev, "%s: cannot get ivdd12_en-gpios %ld\n",
			__func__, PTR_ERR(ctx->ivdd12_en_gpio));
		return PTR_ERR(ctx->ivdd12_en_gpio);
	}
	gpiod_set_value(ctx->ivdd12_en_gpio, 1);
	devm_gpiod_put(ctx->dev, ctx->ivdd12_en_gpio);

	usleep_range(5 * 1000, 6 * 1000);

	//ovdd18
	ctx->ovdd18_en_gpio = devm_gpiod_get(ctx->dev, "ovdd18_en", GPIOD_OUT_HIGH);
	if (IS_ERR(ctx->ovdd18_en_gpio)) {
		dev_err(ctx->dev, "%s: cannot get ovdd18_en-gpios %ld\n",
			__func__, PTR_ERR(ctx->ovdd18_en_gpio));
		return PTR_ERR(ctx->ovdd18_en_gpio);
	}
	gpiod_set_value(ctx->ovdd18_en_gpio, 1);
	devm_gpiod_put(ctx->dev, ctx->ovdd18_en_gpio);

	usleep_range(10 * 1000, 11 * 1000);

	ctx->reset_gpio = devm_gpiod_get(ctx->dev, "reset", GPIOD_OUT_HIGH);
	if (IS_ERR(ctx->reset_gpio)) {
		dev_err(ctx->dev, "%s: cannot get reset_gpio %ld\n",
			__func__, PTR_ERR(ctx->reset_gpio));
		return PTR_ERR(ctx->reset_gpio);
	}
	gpiod_set_value(ctx->reset_gpio, 0);
	usleep_range(10 * 1000, 11 * 1000);
	gpiod_set_value(ctx->reset_gpio, 1);
	usleep_range(50 * 1000, 51 * 1000);
	devm_gpiod_put(ctx->dev, ctx->reset_gpio);

	mdelay(50);
	pr_err("%s out\n", __func__);

	return 0;
}
EXPORT_SYMBOL(lcm_prepare_power);

static int lcm_prepare(struct drm_panel *panel)
{
	struct lcm *ctx = panel_to_lcm(panel);
	int ret;
	
	dev_err(ctx->dev, "zkc %s", __func__);
	
	if (ctx->prepared)
		return 0;

#if 0
	//vdd33
	ctx->vdd_en_gpio = devm_gpiod_get(ctx->dev, "vdd33_en", GPIOD_OUT_HIGH);
	if (IS_ERR(ctx->vdd_en_gpio)) {
		dev_err(ctx->dev, "%s: cannot get vdd_en_gpio %ld\n",
			__func__, PTR_ERR(ctx->vdd_en_gpio));
		return PTR_ERR(ctx->vdd_en_gpio);
	}
	gpiod_set_value(ctx->vdd_en_gpio, 1);
	devm_gpiod_put(ctx->dev, ctx->vdd_en_gpio);

	usleep_range(10 * 1000, 11 * 1000);

	//ivdd12
	ctx->ivdd12_en_gpio = devm_gpiod_get(ctx->dev, "ivdd12_en", GPIOD_OUT_HIGH);
	if (IS_ERR(ctx->ivdd12_en_gpio)) {
		dev_err(ctx->dev, "%s: cannot get ivdd12_en-gpios %ld\n",
			__func__, PTR_ERR(ctx->ivdd12_en_gpio));
		return PTR_ERR(ctx->ivdd12_en_gpio);
	}
	gpiod_set_value(ctx->ivdd12_en_gpio, 1);
	devm_gpiod_put(ctx->dev, ctx->ivdd12_en_gpio);

	usleep_range(5 * 1000, 6 * 1000);

	//ovdd18
	ctx->ovdd18_en_gpio = devm_gpiod_get(ctx->dev, "ovdd18_en", GPIOD_OUT_HIGH);
	if (IS_ERR(ctx->ovdd18_en_gpio)) {
		dev_err(ctx->dev, "%s: cannot get ovdd18_en-gpios %ld\n",
			__func__, PTR_ERR(ctx->ovdd18_en_gpio));
		return PTR_ERR(ctx->ovdd18_en_gpio);
	}
	gpiod_set_value(ctx->ovdd18_en_gpio, 1);
	devm_gpiod_put(ctx->dev, ctx->ovdd18_en_gpio);

	usleep_range(10 * 1000, 11 * 1000);

	ctx->reset_gpio = devm_gpiod_get(ctx->dev, "reset", GPIOD_OUT_HIGH);
	if (IS_ERR(ctx->reset_gpio)) {
		dev_err(ctx->dev, "%s: cannot get reset_gpio %ld\n",
			__func__, PTR_ERR(ctx->reset_gpio));
		return PTR_ERR(ctx->reset_gpio);
	}
	gpiod_set_value(ctx->reset_gpio, 0);
	usleep_range(10 * 1000, 11 * 1000);
	gpiod_set_value(ctx->reset_gpio, 1);
	usleep_range(50 * 1000, 51 * 1000);
	devm_gpiod_put(ctx->dev, ctx->reset_gpio);
#endif

	it6151_mipi_edp_init();
	mdelay(5);

	ret = ctx->error;
	if (ret < 0)
		lcm_unprepare(panel);

	ctx->prepared = true;

#if defined(CONFIG_MTK_PANEL_EXT)
	mtk_panel_tch_rst(panel);
#endif

	return ret;
}

static int lcm_enable(struct drm_panel *panel)
{
	struct lcm *ctx = panel_to_lcm(panel);

	dev_err(ctx->dev, "zkc %s", __func__);
	
	if (ctx->enabled)
		return 0;

	if (ctx->backlight) {
		ctx->backlight->props.power = FB_BLANK_UNBLANK;
		backlight_update_status(ctx->backlight);
	}

	usleep_range(100 * 1000, 101 * 1000);

	ctx->bl_en_gpio = devm_gpiod_get(ctx->dev, "bl_en", GPIOD_OUT_HIGH);	//bl_en
	if (IS_ERR(ctx->bl_en_gpio)) {
		dev_err(ctx->dev, "%s: cannot get bl_en-gpios %ld\n",
			__func__, PTR_ERR(ctx->bl_en_gpio));
		return PTR_ERR(ctx->bl_en_gpio);
	}
	gpiod_set_value(ctx->bl_en_gpio, 1);
	devm_gpiod_put(ctx->dev, ctx->bl_en_gpio);
	ctx->enabled = true;

	return 0;
}

static struct drm_display_mode default_mode = {
	.clock = 157248,//155472,//151403,	//vtotal * htotal *vrefresh / 1000
	.hdisplay = HAC,
	.hsync_start = HAC + HFP,
	.hsync_end = HAC + HFP + HSA,
	.htotal = HAC + HFP + HSA + HBP,
	.vdisplay = VAC,
	.vsync_start = VAC + VFP,
	.vsync_end = VAC + VFP + VSA,
	.vtotal = VAC + VFP + VSA + VBP,
	//.vrefresh = 60,
};

#if defined(CONFIG_MTK_PANEL_EXT)
static int panel_ext_reset(struct drm_panel *panel, int on)
{
	struct lcm *ctx = panel_to_lcm(panel);

	ctx->reset_gpio =
		devm_gpiod_get(ctx->dev, "reset", GPIOD_OUT_HIGH);
	if (IS_ERR(ctx->reset_gpio)) {
		dev_err(ctx->dev, "%s: cannot get reset_gpio %ld\n",
			__func__, PTR_ERR(ctx->reset_gpio));
		return PTR_ERR(ctx->reset_gpio);
	}
	gpiod_set_value(ctx->reset_gpio, on);
	devm_gpiod_put(ctx->dev, ctx->reset_gpio);

	return 0;
}

static int panel_ata_check(struct drm_panel *panel)
{
	unsigned char DevID[2] = {0}; 

	it6151_i2c_read_byte(DP_I2C_ADDR, 0x02, &DevID[0]);
	it6151_i2c_read_byte(DP_I2C_ADDR, 0x03, &DevID[1]);

	if(DevID[0]==0x51 && DevID[1]==0x61){
		return 1;
	}
	return 0;
}

static int lcm_setbacklight_cmdq(void *dsi, dcs_write_gce cb,
	void *handle, unsigned int level)
{
	char bl_tb0[] = {0x51, 0xFF};

	bl_tb0[1] = level;

	if (!cb)
		return -1;

	cb(dsi, handle, bl_tb0, ARRAY_SIZE(bl_tb0));

	return 0;
}

static int lcm_get_virtual_heigh(void)
{
	return VAC;
}

static int lcm_get_virtual_width(void)
{
	return HAC;
}

static struct mtk_panel_params ext_params = {
	.pll_clk = 471,//454,
	.data_rate = 942,//908,
	//.vfp_low_power = 750,
	.cust_esd_check = 0,
	.esd_check_enable = 0,
	//.lcm_esd_check_table[0] = {
	//	.cmd = 0x0a,
	//	.count = 1,
	//	.para_list[0] = 0x9d,
	//},
};

static struct mtk_panel_funcs ext_funcs = {
	.reset = panel_ext_reset,
	.set_backlight_cmdq = lcm_setbacklight_cmdq,
	.ata_check = panel_ata_check,
	.get_virtual_heigh = lcm_get_virtual_heigh,
	.get_virtual_width = lcm_get_virtual_width,
};
#endif

static int lcm_get_modes(struct drm_panel *panel, struct drm_connector *connector)
{
	struct drm_display_mode *mode;

	mode = drm_mode_duplicate(connector->dev, &default_mode);
	if (!mode) {
		dev_info(connector->dev->dev, "failed to add mode %ux%ux@%u\n",
			default_mode.hdisplay, default_mode.vdisplay,
			drm_mode_vrefresh(&default_mode));
		return -ENOMEM;
	}

	drm_mode_set_name(mode);
	mode->type = DRM_MODE_TYPE_DRIVER | DRM_MODE_TYPE_PREFERRED;
	drm_mode_probed_add(connector, mode);

	connector->display_info.width_mm = 302;
	connector->display_info.height_mm = 189;

	return 1;
}

static const struct drm_panel_funcs lcm_drm_funcs = {
	.disable = lcm_disable,
	.unprepare = lcm_unprepare,
	.prepare = lcm_prepare,
	.enable = lcm_enable,
	.get_modes = lcm_get_modes,
};

static int lcm_probe(struct mipi_dsi_device *dsi)
{
	struct device *dev = &dsi->dev;
	struct device_node *dsi_node, *remote_node = NULL, *endpoint = NULL;
	struct lcm *ctx;
	struct device_node *backlight;
	int ret;

	pr_info("%s+\n", __func__);
	dsi_node = of_get_parent(dev->of_node);
	if (dsi_node) {
		endpoint = of_graph_get_next_endpoint(dsi_node, NULL);
		if (endpoint) {
			remote_node = of_graph_get_remote_port_parent(endpoint);
			if (!remote_node) {
				pr_info("No panel connected,skip probe lcm\n");
				return -ENODEV;
			}
			pr_info("device node name:%s\n", remote_node->name);
		}
	}
	if (remote_node != dev->of_node) {
		pr_info("%s+ skip probe due to not current lcm\n", __func__);
		return -ENODEV;
	}

	ctx = devm_kzalloc(dev, sizeof(struct lcm), GFP_KERNEL);
	if (!ctx)
		return -ENOMEM;

	mipi_dsi_set_drvdata(dsi, ctx);

	ctx->dev = dev;
	dsi->lanes = 4;
	dsi->format = MIPI_DSI_FMT_RGB888;
	dsi->mode_flags = MIPI_DSI_MODE_VIDEO | MIPI_DSI_MODE_VIDEO_SYNC_PULSE | MIPI_DSI_MODE_EOT_PACKET;

	backlight = of_parse_phandle(dev->of_node, "backlight", 0);
	if (backlight) {
		ctx->backlight = of_find_backlight_by_node(backlight);
		of_node_put(backlight);

		if (!ctx->backlight)
			return -EPROBE_DEFER;
	}

	ctx->vdd_en_gpio = devm_gpiod_get(dev, "vdd33_en", GPIOD_OUT_HIGH);
	if (IS_ERR(ctx->vdd_en_gpio)) {
		dev_err(dev, "%s: cannot get vdd33_en-gpios %ld\n",
			__func__, PTR_ERR(ctx->vdd_en_gpio));
		return PTR_ERR(ctx->vdd_en_gpio);
	}
	devm_gpiod_put(dev, ctx->vdd_en_gpio);

	ctx->reset_gpio = devm_gpiod_get(dev, "reset", GPIOD_OUT_HIGH);
	if (IS_ERR(ctx->reset_gpio)) {
		dev_err(dev, "%s: cannot get reset-gpios %ld\n",
			__func__, PTR_ERR(ctx->reset_gpio));
		return PTR_ERR(ctx->reset_gpio);
	}
	devm_gpiod_put(dev, ctx->reset_gpio);

	ctx->bl_en_gpio = devm_gpiod_get(dev, "bl_en", GPIOD_OUT_HIGH);
	if (IS_ERR(ctx->bl_en_gpio)) {
		dev_err(dev, "%s: cannot get bl_en-gpios %ld\n",
			__func__, PTR_ERR(ctx->bl_en_gpio));
		return PTR_ERR(ctx->bl_en_gpio);
	}
	devm_gpiod_put(dev, ctx->bl_en_gpio);

	ctx->ivdd12_en_gpio = devm_gpiod_get(dev, "ivdd12_en", GPIOD_OUT_HIGH);
	if (IS_ERR(ctx->ivdd12_en_gpio)) {
		dev_err(dev, "%s: cannot get ivdd12_en-gpios %ld\n",
			__func__, PTR_ERR(ctx->ivdd12_en_gpio));
		return PTR_ERR(ctx->ivdd12_en_gpio);
	}
	devm_gpiod_put(dev, ctx->ivdd12_en_gpio);

	ctx->ovdd18_en_gpio = devm_gpiod_get(dev, "ovdd18_en", GPIOD_OUT_HIGH);
	if (IS_ERR(ctx->ovdd18_en_gpio)) {
		dev_err(dev, "%s: cannot get ovdd18_en-gpios %ld\n",
			__func__, PTR_ERR(ctx->ovdd18_en_gpio));
		return PTR_ERR(ctx->ovdd18_en_gpio);
	}
	devm_gpiod_put(dev, ctx->ovdd18_en_gpio);

	ctx->prepared = true;
	ctx->enabled = true;

	drm_panel_init(&ctx->panel, dev, &lcm_drm_funcs, DRM_MODE_CONNECTOR_DSI);

	drm_panel_add(&ctx->panel);

	ret = mipi_dsi_attach(dsi);
	if (ret < 0)
		drm_panel_remove(&ctx->panel);

#if defined(CONFIG_MTK_PANEL_EXT)
	mtk_panel_tch_handle_reg(&ctx->panel);
	ret = mtk_panel_ext_create(dev, &ext_params, &ext_funcs, &ctx->panel);
	if (ret < 0)
		return ret;
#endif
	g_ctx = ctx;
	pr_info("%s-\n", __func__);

	return ret;
}


static int lcm_remove(struct mipi_dsi_device *dsi)
{
	struct lcm *ctx = mipi_dsi_get_drvdata(dsi);
#if defined(CONFIG_MTK_PANEL_EXT)
	struct mtk_panel_ctx *ext_ctx = find_panel_ctx(&ctx->panel);
#endif

	mipi_dsi_detach(dsi);
	drm_panel_remove(&ctx->panel);

#if defined(CONFIG_MTK_PANEL_EXT)
	mtk_panel_detach(ext_ctx);
	mtk_panel_remove(ext_ctx);
#endif

	return 0;
}

static const struct of_device_id lcm_of_match[] = {
	{ .compatible = "zc,sl140pn50d3381,edp", },
	{ }
};

MODULE_DEVICE_TABLE(of, lcm_of_match);

static struct mipi_dsi_driver lcm_driver = {
	.probe = lcm_probe,
	.remove = lcm_remove,
	.driver = {
		.name = "panel-ZC_SL140PN50D3381_A00_EDP",
		.owner = THIS_MODULE,
		.of_match_table = lcm_of_match,
	},
};

module_mipi_dsi_driver(lcm_driver);

MODULE_AUTHOR("Xiuhai Deng <xiuhai.deng@mediatek.com>");
MODULE_DESCRIPTION("ZC_SL140PN50D3381_A00 EDP LCD Panel Driver");
MODULE_LICENSE("GPL v2");
