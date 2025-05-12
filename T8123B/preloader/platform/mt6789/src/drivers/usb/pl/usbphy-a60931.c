/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */
/* MediaTek Inc. (C) 2010. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
 * AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 * NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 * SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 * SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
 * THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
 * THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
 * CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
 * SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
 * CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
 * MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 * The following software/firmware and/or related documentation ("MediaTek Software")
 * have been modified by MediaTek Inc. All revisions are subject to any receiver's
 * applicable license agreements with MediaTek Inc.
 */

#include "platform.h"
#include "usbphy.h"
#include "usbd.h"
#include "pll.h"

#include "mtk-phy-a60931.h"
#if CFG_FPGA_PLATFORM
#define U3_PHY_PAGE		0xff
#define I2C_CHIP		0xc0

static struct mt_i2c_t usb_i2c;

#define DEBUG 0
#if DEBUG
	#define PHY_LOG print
#else
	#define PHY_LOG
#endif

#define MYI2C 0

#ifdef CONFIG_U3_PHY_GPIO_SUPPORT

/* TEST CHIP PHY define, edit this in different platform */
#define U3_PHY_I2C_DEV				0x60
#define U3_PHYCLK_I2C_DEV			0x70
#define U3_PHY_PAGE				0xff
#define u3_sif_base				0x112A0000
#define GPIO_BASE 				u3_sif_base+0x700 //0xf0044700		//0x80080000
#define SSUSB_I2C_OUT			GPIO_BASE+0xd0
#define SSUSB_I2C_IN			GPIO_BASE+0xd4

/////////////////////////////////////////////////////////////////

#define OUTPUT		1
#define INPUT		0

#define SDA    		0        /// GPIO #0: I2C data pin
#define SCL    		1        /// GPIO #1: I2C clock pin

/////////////////////////////////////////////////////////////////

#define SDA_OUT		(1<<0)
#define SDA_OEN		(1<<1)
#define SCL_OUT		(1<<2)
#define SCL_OEN		(1<<3)

#define SDA_IN_OFFSET		0
#define SCL_IN_OFFSET		1

/* TYPE DEFINE */
typedef unsigned int	PHY_UINT32;
typedef int				PHY_INT32;
typedef	unsigned short	PHY_UINT16;
typedef short			PHY_INT16;
typedef unsigned char	PHY_UINT8;
typedef char			PHY_INT8;

/* CONSTANT DEFINE */
#define PHY_FALSE	0
#define PHY_TRUE	1


//#define 	GPIO_PULLEN1_SET    	(GPIO_BASE+0x0030+0x04)
//#define 	GPIO_DIR1_SET       	(GPIO_BASE+0x0000+0x04)
//#define 	GPIO_PULLEN1_CLR    	(GPIO_BASE+0x0030+0x08)
//#define 	GPIO_DIR1_CLR       	(GPIO_BASE+0x0000+0x08)
//#define 	GPIO_DOUT1_SET      	(GPIO_BASE+0x00C0+0x04)
//#define 	GPIO_DOUT1_CLR      	(GPIO_BASE+0x00C0+0x08)
//#define 	GPIO_DIN1       	(GPIO_BASE+0x00F0)

void gpio_dir_set(PHY_INT32 pin){
	PHY_INT32 temp;
    u64 addr;
	addr = SSUSB_I2C_OUT;
	temp = DRV_Reg32(addr);
	if(pin == SDA){
		temp |= SDA_OEN;
		DRV_WriteReg32(addr,temp);
	}
	else{
		temp |= SCL_OEN;
		DRV_WriteReg32(addr,temp);
	}
}

void gpio_dir_clr(PHY_INT32 pin){
	PHY_INT32 temp;
    u64 addr;
	addr = SSUSB_I2C_OUT;
	temp = DRV_Reg32(addr);
	if(pin == SDA){
		temp &= ~SDA_OEN;
		DRV_WriteReg32(addr,temp);
	}
	else{
		temp &= ~SCL_OEN;
		DRV_WriteReg32(addr,temp);
	}
}

void gpio_dout_set(PHY_INT32 pin){
	PHY_INT32 temp;
    u64 addr;
	addr = SSUSB_I2C_OUT;
	temp = DRV_Reg32(addr);
	if(pin == SDA){
		temp |= SDA_OUT;
		DRV_WriteReg32(addr,temp);
	}
	else{
		temp |= SCL_OUT;
		DRV_WriteReg32(addr,temp);
	}
}

void gpio_dout_clr(PHY_INT32 pin){
	PHY_INT32 temp;
    u64 addr;
	addr = SSUSB_I2C_OUT;
	temp = DRV_Reg32(addr);
	if(pin == SDA){
		temp &= ~SDA_OUT;
		DRV_WriteReg32(addr,temp);
	}
	else{
		temp &= ~SCL_OUT;
		DRV_WriteReg32(addr,temp);
	}
}

PHY_INT32 gpio_din(PHY_INT32 pin){
	PHY_INT32 temp;
    u64 addr;
	addr = SSUSB_I2C_IN;
	temp = DRV_Reg32(addr);
	if(pin == SDA){
		temp = (temp >> SDA_IN_OFFSET) & 1;
	}
	else{
		temp = (temp >> SCL_IN_OFFSET) & 1;
	}
	return temp;
}

//#define     GPIO_PULLEN_SET(_no)  (GPIO_PULLEN1_SET+(0x10*(_no)))
#define     GPIO_DIR_SET(pin)   gpio_dir_set(pin)
#define     GPIO_DOUT_SET(pin)  gpio_dout_set(pin);
//#define     GPIO_PULLEN_CLR(_no) (GPIO_PULLEN1_CLR+(0x10*(_no)))
#define     GPIO_DIR_CLR(pin)   gpio_dir_clr(pin)
#define     GPIO_DOUT_CLR(pin)  gpio_dout_clr(pin)
#define     GPIO_DIN(pin)       gpio_din(pin)


PHY_UINT32 i2c_dummy_cnt;

#define I2C_DELAY 10
#define I2C_DUMMY_DELAY(_delay) for (i2c_dummy_cnt = ((_delay)) ; i2c_dummy_cnt!=0; i2c_dummy_cnt--)

void GPIO_InitIO(PHY_UINT32 dir, PHY_UINT32 pin)
{
    if (dir == OUTPUT)
    {
//        DRV_WriteReg16(GPIO_PULLEN_SET(no),(1 << remainder));
        GPIO_DIR_SET(pin);
    }
    else
    {
//        DRV_WriteReg16(GPIO_PULLEN_CLR(no),(1 << remainder));
        GPIO_DIR_CLR(pin);
    }
    I2C_DUMMY_DELAY(100);
}

void GPIO_WriteIO(PHY_UINT32 data, PHY_UINT32 pin)
{
    if (data == 1){
		GPIO_DOUT_SET(pin);
    }
    else{
        GPIO_DOUT_CLR(pin);
    }
}

PHY_UINT32 GPIO_ReadIO( PHY_UINT32 pin)
{
    PHY_UINT16 data;
    data=GPIO_DIN(pin);
    return (PHY_UINT32)data;
}


void SerialCommStop(void)
{
    GPIO_InitIO(OUTPUT,SDA);
    GPIO_WriteIO(0,SCL);
    I2C_DUMMY_DELAY(I2C_DELAY);
    GPIO_WriteIO(0,SDA);
    I2C_DUMMY_DELAY(I2C_DELAY);
    GPIO_WriteIO(1,SCL);
    I2C_DUMMY_DELAY(I2C_DELAY);
    GPIO_WriteIO(1,SDA);
    I2C_DUMMY_DELAY(I2C_DELAY);
	GPIO_InitIO(INPUT,SCL);
    GPIO_InitIO(INPUT,SDA);
}

void SerialCommStart(void) /* Prepare the SDA and SCL for sending/receiving */
{
	GPIO_InitIO(OUTPUT,SCL);
    GPIO_InitIO(OUTPUT,SDA);
    GPIO_WriteIO(1,SDA);
    I2C_DUMMY_DELAY(I2C_DELAY);
    GPIO_WriteIO(1,SCL);
    I2C_DUMMY_DELAY(I2C_DELAY);
    GPIO_WriteIO(0,SDA);
    I2C_DUMMY_DELAY(I2C_DELAY);
    GPIO_WriteIO(0,SCL);
    I2C_DUMMY_DELAY(I2C_DELAY);
}

PHY_UINT32 SerialCommTxByte(PHY_UINT8 data) /* return 0 --> ack */
{
    PHY_INT32 i, ack;

    GPIO_InitIO(OUTPUT,SDA);

    for(i=8; --i>0;){
        GPIO_WriteIO((data>>i)&0x01, SDA);
        I2C_DUMMY_DELAY(I2C_DELAY);
        GPIO_WriteIO( 1, SCL); /* high */
        I2C_DUMMY_DELAY(I2C_DELAY);
        GPIO_WriteIO( 0, SCL); /* low */
        I2C_DUMMY_DELAY(I2C_DELAY);
    }
    GPIO_WriteIO((data>>i)&0x01, SDA);
    I2C_DUMMY_DELAY(I2C_DELAY);
    GPIO_WriteIO( 1, SCL); /* high */
    I2C_DUMMY_DELAY(I2C_DELAY);
    GPIO_WriteIO( 0, SCL); /* low */
    I2C_DUMMY_DELAY(I2C_DELAY);

    GPIO_WriteIO(0, SDA);
    I2C_DUMMY_DELAY(I2C_DELAY);
    GPIO_InitIO(INPUT,SDA);
    I2C_DUMMY_DELAY(I2C_DELAY);
    GPIO_WriteIO(1, SCL);
    I2C_DUMMY_DELAY(I2C_DELAY);
    ack = GPIO_ReadIO(SDA); /// ack 1: error , 0:ok
    GPIO_WriteIO(0, SCL);
    I2C_DUMMY_DELAY(I2C_DELAY);

    if(ack==1)
        return PHY_FALSE;
    else
        return PHY_TRUE;
}

void SerialCommRxByte(PHY_UINT8 *data, PHY_UINT8 ack)
{
   PHY_INT32 i;
   PHY_UINT32 dataCache;

   dataCache = 0;
   GPIO_InitIO(INPUT,SDA);
   for(i=8; --i>=0;){
      dataCache <<= 1;
      I2C_DUMMY_DELAY(I2C_DELAY);
      GPIO_WriteIO(1, SCL);
      I2C_DUMMY_DELAY(I2C_DELAY);
      dataCache |= GPIO_ReadIO(SDA);
      GPIO_WriteIO(0, SCL);
      I2C_DUMMY_DELAY(I2C_DELAY);
   }
   GPIO_InitIO(OUTPUT,SDA);
   GPIO_WriteIO(ack, SDA);
   I2C_DUMMY_DELAY(I2C_DELAY);
   GPIO_WriteIO(1, SCL);
   I2C_DUMMY_DELAY(I2C_DELAY);
   GPIO_WriteIO(0, SCL);
   I2C_DUMMY_DELAY(I2C_DELAY);
   *data = (unsigned char)dataCache;
}

PHY_INT32 I2cWriteReg(PHY_UINT8 dev_id, PHY_UINT8 Addr, PHY_UINT8 Data)
{
    PHY_INT32 acknowledge=0;

    SerialCommStart();
    acknowledge=SerialCommTxByte((dev_id<<1) & 0xff);
    if(acknowledge)
        acknowledge=SerialCommTxByte(Addr);
    else
        return PHY_FALSE;
    acknowledge=SerialCommTxByte(Data);
    if(acknowledge)
    {
        SerialCommStop();
        return PHY_FALSE;
    }
    else
    {
        return PHY_TRUE;
    }
}

PHY_INT32 I2cReadReg(PHY_UINT8 dev_id, PHY_UINT8 Addr, PHY_UINT8 *Data)
{
    PHY_INT32 acknowledge=0;
    SerialCommStart();
    acknowledge=SerialCommTxByte((dev_id<<1) & 0xff);
    if(acknowledge)
        acknowledge=SerialCommTxByte(Addr);
    else
        return PHY_FALSE;
    SerialCommStart();
    acknowledge=SerialCommTxByte(((dev_id<<1) & 0xff) | 0x01);
    if(acknowledge)
        SerialCommRxByte(Data, 1);  // ack 0: ok , 1 error
    else
        return PHY_FALSE;
    SerialCommStop();
    return acknowledge;
}

#endif

#if MYI2C
#define REG_I2C_START_BIT	0x1
#define I2C_READ_BIT	 	0x1

#define PHY_I2C_BASE		(0x11008000)
#define REG_I2C_DATA_PORT	(*((volatile unsigned short int *) (PHY_I2C_BASE + 0x00)))
#define REG_I2C_SLAVE_ADDR	(*((volatile unsigned short int *) (PHY_I2C_BASE + 0x04)))
#define REG_I2C_TRANSFER_LEN	(*((volatile unsigned short int *) (PHY_I2C_BASE + 0x14)))
#define REG_I2C_START		(*((volatile unsigned short int *) (PHY_I2C_BASE + 0x24)))
#define REG_I2C_SOFT_RESET	(*((volatile unsigned short int *) (PHY_I2C_BASE + 0x50)))
#define REG_I2C_CONTROL		(*((volatile unsigned short int *) (PHY_I2C_BASE + 0x10)))

u32 i2c_write_reg(u8 dev_id, u8 addr, u8 val)
{
	REG_I2C_SLAVE_ADDR = dev_id<<1;
	REG_I2C_TRANSFER_LEN = 2;

	REG_I2C_DATA_PORT = addr;
	REG_I2C_DATA_PORT = val;

	REG_I2C_START = REG_I2C_START_BIT;

	while ((REG_I2C_START & REG_I2C_START_BIT));
	return TRUE;
}

u32 i2c_read_reg(u8 dev_id, u8 addr, u8 *data)
{
	REG_I2C_SLAVE_ADDR = dev_id<<1;
	REG_I2C_TRANSFER_LEN = 0x01;
	REG_I2C_DATA_PORT = addr;
	REG_I2C_START = REG_I2C_START_BIT;

	while ((REG_I2C_START & REG_I2C_START_BIT));

	REG_I2C_SLAVE_ADDR = (dev_id << 1) | I2C_READ_BIT;
	REG_I2C_TRANSFER_LEN = 0x01;
	REG_I2C_START = REG_I2C_START_BIT;

	while ((REG_I2C_START & REG_I2C_START_BIT));

	*data = REG_I2C_DATA_PORT;

	return TRUE; /* !!(int)*data; */
}
#endif

#define U3_PHY_I2C_DEV		0x60
#define U3_PHYCLK_I2C_DEV	0x70
u32 usb_i2c_read8(u8 addr, u8 *databuffer)
{
	#ifdef CONFIG_U3_PHY_GPIO_SUPPORT

	PHY_INT32 ret;
	ret = I2cReadReg(U3_PHY_I2C_DEV, addr, databuffer);
	return ret;

	#else
	#if MYI2C
	u32 ret = I2C_OK;

	ret = i2c_read_reg(U3_PHY_I2C_DEV, addr, databuffer);

	return ret;
	#else
	u32 ret_code = I2C_OK;
	u16 len;
	*databuffer = addr;

	usb_i2c.id = USB_I2C_ID;
	/* Since i2c will left shift 1 bit, we need to set USB I2C address to 0x60 (0xC0>>1) */
	usb_i2c.addr = 0x60;
	usb_i2c.mode = ST_MODE;
	usb_i2c.speed = 100;
	len = 1;

	ret_code = i2c_write_read(&usb_i2c, databuffer, len, len);

	return ret_code;
	#endif
	#endif
}

u32 usb_i2c_write8(u8 addr, u8 value)
{
	#ifdef CONFIG_U3_PHY_GPIO_SUPPORT

	PHY_INT32 ret;
	ret = I2cWriteReg(U3_PHY_I2C_DEV, addr, value);
	#else
	#if MYI2C
	u32 ret = I2C_OK;

	ret = i2c_write_reg(U3_PHY_I2C_DEV, addr, value);

	return ret;
	#else
	u32 ret_code = I2C_OK;
	u8 write_data[2];
	u16 len;

	write_data[0]= addr;
	write_data[1] = value;

	usb_i2c.id = USB_I2C_ID;
	/* Since i2c will left shift 1 bit, we need to set USB I2C address to 0x60 (0xC0>>1) */
	usb_i2c.addr = 0x60;
	usb_i2c.mode = ST_MODE;
	usb_i2c.speed = 100;
	len = 2;

	ret_code = i2c_write(&usb_i2c, write_data, len);

	return ret_code;
	#endif
	#endif
}

void _u3_write_bank(u32 value)
{
	usb_i2c_write8((u8)U3_PHY_PAGE, (u8)value);
}

u32 _u3_read_reg(u32 address)
{
	u8 databuffer = 0;
	usb_i2c_read8((u8)address, &databuffer);
	return databuffer;
}

void _u3_write_reg(u32 address, u32 value)
{
	usb_i2c_write8((u8 )address, (u8 )value);
}

u32 u3_phy_read_reg32(u32 addr)
{
	u32 bank;
	u32 addr8;
	u32 data;

	bank = (addr >> 16) & 0xff;
	addr8 = addr & 0xff;

	_u3_write_bank(bank);
	data = _u3_read_reg(addr8);
	data |= (_u3_read_reg(addr8 + 1) << 8);
	data |= (_u3_read_reg(addr8 + 2) << 16);
	data |= (_u3_read_reg(addr8 + 3) << 24);
	return data;
}

u32 u3_phy_write_reg32(u32 addr, u32 data) {
	u32 bank;
	u32 addr8;
	u32 data_0, data_1, data_2, data_3;

	bank = (addr >> 16) & 0xff;
	addr8 = addr & 0xff;
	data_0 = data & 0xff;
	data_1 = (data >> 8) & 0xff;
	data_2 = (data >> 16) & 0xff;
	data_3 = (data >> 24) & 0xff;

	_u3_write_bank(bank);
	_u3_write_reg(addr8, data_0);
	_u3_write_reg(addr8 + 1, data_1);
	_u3_write_reg(addr8 + 2, data_2);
	_u3_write_reg(addr8 + 3, data_3);

	return 0;
}

void u3_phy_write_field32(int addr, int offset, int mask, int value)
{
	u32 cur_value;
	u32 new_value;

	cur_value = u3_phy_read_reg32(addr);
	new_value = (cur_value & (~mask)) | ((value << offset) & mask);
	//udelay(i2cdelayus);
	u3_phy_write_reg32(addr, new_value);
}

u32 u3_phy_write_reg8(u32 addr, u8 data)
{
	u32 bank;
	u32 addr8;

	bank = (addr >> 16) & 0xff;
	addr8 = addr & 0xff;
	_u3_write_bank(bank);
	_u3_write_reg(addr8, data);

	return 0;
}

#define USBPHY_I2C_READ8(addr, buffer)	 usb_i2c_read8(addr, buffer)
#define USBPHY_I2C_WRITE8(addr, value)	 usb_i2c_write8(addr, value)
#endif

#if CFG_FPGA_PLATFORM
int u2_slew_rate_calibration_a60931(struct u3phy_info *info)
{
	u32 i = 0;
	u32 ret = 0;
	u32 u4fmout = 0;
	u32 u4tmp = 0;
	volatile u32 chkvalue;

	PHY_LOG("[UPHY] %s starts\n", __func__);

	/* => RG_USB20_HSTX_SRCAL_EN = 1 */
	/* enable HS TX SR calibration */
	u3_phy_write_field32(((u32)&info->u2phy_regs_a->usbphyacr5),
		A60931_RG_USB20_HSTX_SRCAL_EN_OFST, A60931_RG_USB20_HSTX_SRCAL_EN, 1);
	mdelay(1);
	chkvalue= u3_phy_read_reg32(((u32)&info->u2phy_regs_a->usbphyacr5));
	PHY_LOG("chkvalue: %d\n", chkvalue);

	/* => RG_FRCK_EN = 1 */
	/* Enable free run clock */
	u3_phy_write_field32(((u32)&info->sifslv_fm_regs_a->fmmonr1),
		A60931_RG_FRCK_EN_OFST, A60931_RG_FRCK_EN, 0x1);

	/* => RG_CYCLECNT = 4 */
	/* Setting cyclecnt = 4 */
	u3_phy_write_field32(((u32)&info->sifslv_fm_regs_a->fmcr0),
		A60931_RG_CYCLECNT_OFST, A60931_RG_CYCLECNT, 0x4);

	/* => RG_FREQDET_EN = 1 */
	/* Enable frequency meter */
	u3_phy_write_field32(((u32)&info->sifslv_fm_regs_a->fmcr0),
		A60931_RG_FREQDET_EN_OFST, A60931_RG_FREQDET_EN, 0x1);

	/* wait for FM detection done, set 10ms timeout */
	for (i = 0; i < 10; i++) {
		/* => u4fmout = USB_FM_OUT */
		/* read FM_OUT */
		u4fmout = u3_phy_readr_reg32(((u32)&info->sifslv_fm_regs_a->fmmonr0));
		PHY_LOG("FM_OUT value: u4fmout = %d(0x%08X)\n", u4fmout, u4fmout);

		/* check if FM detection done */
		if (u4fmout != 0) {
			ret = 0;
			PHY_LOG("FM detection done! loop = %d\n", i);
			break;
		}

		ret = 1;
		mdelay(1);
	}

	/* => RG_FREQDET_EN = 0 */
	/* disable frequency meter */
	u3_phy_write_field32(((u32)&info->sifslv_fm_regs_a->fmcr0),
		A60931_RG_FREQDET_EN_OFST, A60931_RG_FREQDET_EN, 0);

	/* => RG_FRCK_EN = 0 */
	/* disable free run clock */
	u3_phy_write_field32(((u32)&info->sifslv_fm_regs_a->fmmonr1),
		A60931_RG_FRCK_EN_OFST, A60931_RG_FRCK_EN, 0);

	/* => RG_USB20_HSTX_SRCAL_EN = 0 */
	/* disable HS TX SR calibration */
	u3_phy_write_field32(((u32)&info->u2phy_regs_a->usbphyacr0),
		A60931_RG_USB20_HSTX_SRCAL_EN_OFST, A60931_RG_USB20_HSTX_SRCAL_EN, 0);
	mdelay(1);

	if (u4fmout == 0) {
		u3_phy_write_field32(((u32)&info->u2phy_regs_a->usbphyacr5),
			A60931_RG_USB20_HSTX_SRCTRL_OFST, A60931_RG_USB20_HSTX_SRCTRL, 0x4);
		ret = 1;
	} else {
		/* set reg = (1024/FM_OUT) * 25 * 0.028 (round to the nearest digits) */
		u4tmp = (((1024 * 25 * U2_SR_COEF_A60931) / u4fmout) + 500) / 1000;
		PHY_LOG("SR calibration value u1SrCalVal = %d\n", (u8)u4tmp);
		u3_phy_write_field32(((u32)&info->u2phy_regs_a->usbphyacr5),
		A60931_RG_USB20_HSTX_SRCTRL_OFST, A60931_RG_USB20_HSTX_SRCTRL, u4tmp);
	}

	PHY_LOG("[UPHY] %s ends\n", __func__);

	return ret;
}

#define PHY_DRV_SHIFT	3
#define PHY_PHASE_SHIFT	3
#define PHY_PHASE_DRV_SHIFT	1

char u3_phy_read_reg8(u32 addr) {
	int bank;
	int addr8;
	int data;

	bank = (addr >> 16) & 0xff;
	addr8 = addr & 0xff;
	_u3_write_bank(bank);
	data = _u3_read_reg(addr8);

	return data;
}


int phy_change_pipe_phase_a60931(struct u3phy_info *info, int phy_drv, int pipe_phase){
	int drv_reg_value;
	int phase_reg_value;
	int temp;

	printf("phy_change_pipe_phase_a60931: %d", pipe_phase);

	drv_reg_value = phy_drv << PHY_DRV_SHIFT;
	phase_reg_value = (pipe_phase << PHY_PHASE_SHIFT) | (phy_drv << PHY_PHASE_DRV_SHIFT);
	temp = U3PhyReadReg8(((u32)&info->sifslv_chip_regs_a->gpio_ctla)+2);
	temp &= ~(0x3 << PHY_DRV_SHIFT);
	temp |= drv_reg_value;
	u3_phy_write_reg8(((u32)&info->sifslv_chip_regs_a->gpio_ctla)+2, temp);

	printf("gpio_clta+2=0x%x\n", U3PhyReadReg8(((u32)&info->sifslv_chip_regs_a->gpio_ctla)+2));

	temp = U3PhyReadReg8(((u32)&info->sifslv_chip_regs_a->gpio_ctla)+3);
	temp &= ~((0x3 << PHY_PHASE_DRV_SHIFT) | (0x1f << PHY_PHASE_SHIFT));
	temp |= phase_reg_value;
	u3_phy_write_reg8(((u32)&info->sifslv_chip_regs_a->gpio_ctla)+3, temp);

	printf("gpio_clta+3=0x%x\n", U3PhyReadReg8(((u32)&info->sifslv_chip_regs_a->gpio_ctla)+3));

	return TRUE;
}

#define os_writel(addr, value) __raw_writel(value, addr)

#define ENTER_U0_TH			5
#define MAX_PHASE_RANGE 		31
#define MAX_TIMEOUT_COUNT 		100
#define DATA_DRIVING_MASK 		0x06
#define MAX_DRIVING_RANGE 		0x04
#define MAX_LATCH_SELECT 		0x02

#define U3_PHY_I2C_PCLK_DRV_REG		0x0A
#define U3_PHY_I2C_PCLK_PHASE_REG	0x0B
#define	STATE_U0_STATE			(13)
#define CLR_RECOV_CNT			(0x1 << 16) /* 16:16 */
#define CLR_LINK_ERR_CNT		(0x1 << 16) /* 16:16 */
#define	STATE_DISABLE			(1)


int phy_init_a60931(struct u3phy_info *info)
{
	/* 0xFC[31:24], Change bank address to 0 */
	//phy_writeb(i2c, 0x60, 0xff, 0x0);
	/* 0x14[14:12],  RG_USB20_HSTX_SRCTRL, set U2 slew rate as 4 */
	u3_phy_write_field32(((u32)&info->u2phy_regs_a->usbphyacr5),
		A60931_RG_USB20_HSTX_SRCTRL_OFST, A60931_RG_USB20_HSTX_SRCTRL, 0x4);

	/* 0x18[23:23],  RG_USB20_BC11_SW_EN, Disable BC 1.1 */
	//phy_writelmsk(i2c, 0x60, 0x18, 23, BIT(23), 0x0);
	u3_phy_write_field32(((u32)&info->u2phy_regs_a->usbphyacr6),
		A60931_RG_USB20_BC11_SW_EN_OFST, A60931_RG_USB20_BC11_SW_EN, 0x0);

	/* 0x68[18:18],  force_suspendm = 0 */
	//phy_writelmsk(i2c, 0x60, 0x68, 18, BIT(18), 0x0);
	u3_phy_write_field32(((u32)&info->u2phy_regs_a->u2phydtm0),
		A60931_FORCE_SUSPENDM_OFST, A60931_FORCE_SUSPENDM, 0x0);

	/* 0xFC[31:24], Change bank address to 0x30 */
	//phy_writeb(i2c, 0x60, 0xff, 0x30);
	/* 0x04[29:29],  RG_VUSB10_ON, SSUSB 1.0V power ON */
	//phy_writelmsk(i2c, 0x60, 0x04, 29, BIT(29), 0x1);
	u3_phy_write_field32(((u32)&info->u3phya_regs_a->reg1),
		A60931_RG_VUSB10_ON_OFST, A60931_RG_VUSB10_ON, 0x1);

	/* 0x04[25:21], RG_SSUSB_XTAL_TOP_RESERVE */
	//phy_writelmsk(i2c, 0x60, 0x04, 21, GENMASK(25, 21), 0x11);
	u3_phy_write_field32(((u32)&info->u3phya_regs_a->reg1),
		A60931_RG_SSUSB_XTAL_TOP_RESERVE_OFST,
		A60931_RG_SSUSB_XTAL_TOP_RESERVE, 0x11);

	/* 0xFC[31:24], Change bank address to 0x40 */
	//phy_writeb(i2c, 0x60, 0xff, 0x40);

	/* 0x38[15:0], DA_SSUSB_PLL_SSC_DELTA1 */
	/* fine tune SSC delta1 to let SSC min average ~0ppm */
	//phy_writelmsk(i2c, 0x60, 0x38, 0, GENMASK(15, 0)<<0, 0x47);
	u3_phy_write_field32(((u32)&info->u3phya_da_regs_a->reg19),
		A60931_RG_SSUSB_PLL_SSC_DELTA1_U3_OFST,
		A60931_RG_SSUSB_PLL_SSC_DELTA1_U3, 0x47);

	/* 0x40[31:16], DA_SSUSB_PLL_SSC_DELTA */
	/* fine tune SSC delta to let SSC min average ~0ppm */
	//phy_writelmsk(i2c, 0x60, 0x40, 16, GENMASK(31, 16), 0x44);
	u3_phy_write_field32(((u32)&info->u3phya_da_regs_a->reg21),
		A60931_RG_SSUSB_PLL_SSC_DELTA_U3_OFST,
		A60931_RG_SSUSB_PLL_SSC_DELTA_U3, 0x44);

	/* 0xFC[31:24], Change bank address to 0x30 */
	//phy_writeb(i2c, 0x60, 0xff, 0x30);
	/* 0x14[15:0],  RG_SSUSB_PLL_SSC_PRD */
	/* fine tune SSC PRD to let SSC freq average 31.5KHz */
	//phy_writelmsk(i2c, 0x60, 0x14, 0, GENMASK(15, 0), 0x190);
	u3_phy_write_field32(((u32)&info->u3phya_regs_a->reg7),
		A60931_RG_SSUSB_PLL_SSC_PRD_OFST, A60931_RG_SSUSB_PLL_SSC_PRD, 0x190);

	/* ToDo: PCIE CODA A60931A_PCIE_GLB_CSR_Description */
#ifdef CONFIG_A60931_PCIE
	/* 0xFC[31:24], Change bank address to 0x70 */
	//phy_writeb(i2c, 0x70, 0xff, 0x70);
	/* 0x88[3:2], Pipe reset, clk driving current */
	phy_writelmsk(i2c, 0x70, 0x88, 2, GENMASK(3, 2), 0x1);
	/* 0x88[5:4], Data lane 0 driving current */
	phy_writelmsk(i2c, 0x70, 0x88, 4, GENMASK(5, 4), 0x1);
	/* 0x88[7:6], Data lane 1 driving current */
	phy_writelmsk(i2c, 0x70, 0x88, 6, GENMASK(7, 6), 0x1);
	/* 0x88[9:8], Data lane 2 driving current */
	phy_writelmsk(i2c, 0x70, 0x88, 8, GENMASK(9, 8), 0x1);
	/* 0x88[11:10], Data lane 3 driving current */
	phy_writelmsk(i2c, 0x70, 0x88, 10, GENMASK(11, 10), 0x1);
	/* 0x9C[4:0],  rg_ssusb_ckphase, PCLK phase 0x00~0x1F */
	phy_writelmsk(i2c, 0x70, 0x9c, 0, GENMASK(4, 0), 0x19);
#endif
	/* Set INTR & TX/RX Impedance */

	/* 0xFC[31:24], Change bank address to 0x30 */
	//phy_writeb(i2c, 0x60, 0xff, 0x30);

	/* 0x00[26:26],  RG_SSUSB_INTR_EN */
	//phy_writelmsk(i2c, 0x60, 0x00, 26, BIT(26), 0x1);
	u3_phy_write_field32(((u32)&info->u3phya_regs_a->reg0),
		A60931_RG_SSUSB_INTR_EN_OFST, A60931_RG_SSUSB_INTR_EN, 0x1);

	/* 0x00[15:10],  RG_SSUSB_IEXT_INTR_CTRL, Set Iext R selection */
	//phy_writelmsk(i2c, 0x60, 0x00, 10, GENMASK(15, 10), 0x26);
	u3_phy_write_field32(((u32)&info->u3phya_regs_a->reg0),
		A60931_RG_SSUSB_IEXT_INTR_CTRL_OFST, A60931_RG_SSUSB_IEXT_INTR_CTRL, 0x26);

	/* 0xFC[31:24], Change bank address to 0x10 */
	//phy_writeb(i2c, 0x60, 0xff, 0x10);

	/* 0x10[31:31],  rg_ssusb_force_tx_impsel,  enable */
	//phy_writelmsk(i2c, 0x60, 0x10, 31, BIT(31), 0x1);
	u3_phy_write_field32(((u32)&info->u3phyd_regs_a->phyd_impcal0),
		A60931_RG_SSUSB_FORCE_TX_IMPSEL_OFST, A60931_RG_SSUSB_FORCE_TX_IMPSEL, 0x1);

	/* 0x10[28:24],  rg_ssusb_tx_impsel, Set TX Impedance */
	//phy_writelmsk(i2c, 0x60, 0x10, 24, GENMASK(28, 24), 0x10);
	u3_phy_write_field32(((u32)&info->u3phyd_regs_a->phyd_impcal0),
		A60931_RG_SSUSB_TX_IMPSEL_OFST, A60931_RG_SSUSB_RX_IMPSEL, 0x10);

	/* 0x14[31:31],  rg_ssusb_force_rx_impsel, enable */
	//phy_writelmsk(i2c, 0x60, 0x14, 31, BIT(31), 0x1);
	u3_phy_write_field32(((u32)&info->u3phyd_regs_a->phyd_impcal1),
		A60931_RG_SSUSB_FORCE_RX_IMPSEL_OFST, A60931_RG_SSUSB_FORCE_RX_IMPSEL, 0x1);

	/* 0x14[28:24],  rg_ssusb_rx_impsel, Set RX Impedance */
	//phy_writelmsk(i2c, 0x60, 0x14, 24, GENMASK(28, 24), 0x10);
	u3_phy_write_field32(((u32)&info->u3phyd_regs_a->phyd_impcal1),
		A60931_RG_SSUSB_RX_IMPSEL_OFST, A60931_RG_SSUSB_RX_IMPSEL, 0x10);

	/* 0xFC[31:24], Change bank address to 0x00 */
	//phy_writeb(i2c, 0x60, 0xff, 0x00);
	/* 0x00[05:05],  RG_USB20_INTR_EN, U2 INTR_EN */
	//phy_writelmsk(i2c, 0x60, 0x00, 5, BIT(5), 0x1);
	u3_phy_write_field32(((u32)&info->u2phy_regs_a->usbphyacr0),
		A60931_RG_USB20_INTR_EN_OFST, A60931_RG_USB20_INTR_EN, 0x1);

	/* 0x04[23:19],  RG_USB20_INTR_CAL, Set Iext R selection */
	//phy_writelmsk(i2c, 0x60, 0x04, 19, GENMASK(23, 19), 0x14);
	u3_phy_write_field32(((u32)&info->u2phy_regs_a->usbphyacr1),
		A60931_RG_USB20_INTR_CAL_OFST, A60931_RG_USB20_INTR_CAL, 0x14);

	return TRUE;
}

void mt_usb_phy_poweron (void)
{
	static struct u3phy_info info;
	volatile u32 u3phy_version;

	info.phyd_version_addr = 0x2000e4;
	u3phy_version = u3_phy_read_reg32(info.phyd_version_addr);
	PHY_LOG("[UPHY] Phy version is %x\n", u3phy_version);
	//info.u2phy_regs_a = (struct u2phy_reg_d *)0x0;
	//info.u3phyd_regs_a = (struct u3phyd_reg_d *)0x100000;
	//info.u3phyd_bank2_regs_a = (struct u3phyd_bank2_reg_d *)0x200000;
	//info.u3phya_regs_a = (struct u3phya_reg_d *)0x300000;
	//info.u3phya_da_regs_a = (struct u3phya_da_reg_d *)0x400000;
	//info.sifslv_chip_regs_a = (struct sifslv_chip_reg_d *)0x500000;
	//info.sifslv_fm_regs_a = (struct sifslv_fm_feg_d *)0xf00000;

	info.u2phy_regs_a = (struct u2phy_reg_a *)0x0;
	info.u3phyd_regs_a = (struct u3phyd_reg_a *)0x100000;
	info.u3phyd_bank2_regs_a = (struct u3phyd_bank2_reg_a *)0x200000;
	info.u3phya_regs_a = (struct u3phya_reg_e *)0x300000;
	info.u3phya_da_regs_a = (struct u3phya_da_reg_a *)0x400000;
	info.sifslv_chip_regs_a = (struct sifslv_chip_reg_a *)0x500000;
	info.spllc_regs_a = (struct spllc_reg_a *)0x600000;
	info.sifslv_fm_regs_a = (struct sifslv_fm_feg_a *)0xf00000;

	phy_init_a60931(&info);

	/* for RF desense */
	//u2_slew_rate_calibration_a60931(&info);

}
void mt_usb_phy_savecurrent(void)
{
}
void mt_usb_phy_recover(void)
{
}
void mt_usb11_phy_savecurrent(void)
{
}
#else

void switch_2_usb()
{
	/*switch to USB function. (system register, force ip into usb mode) */
/*
	USB_CLRMASK(U3D_U2PHYDTM0, A60931_FORCE_UART_EN);
	USB_CLRMASK(U3D_U2PHYDTM1, A60931_RG_UART_EN);
	USB_CLRMASK(U3D_U2PHYACR4, A60931_RG_USB20_GPIO_CTL);
	USB_CLRMASK(U3D_U2PHYACR4, A60931_USB20_GPIO_MODE);
*/
}

void mt_usb_phy_poweron(void)
{
    /*
	 * force_uart_en	1'b0		0x68 26
	 * RG_UART_EN		1'b0		0x6c 16
	 * rg_usb20_gpio_ctl	1'b0		0x20 09
	 * usb20_gpio_mode	1'b0		0x20 08
	 * RG_USB20_BC11_SW_EN	1'b0		0x18 23
	 * rg_usb20_dp_100k_mode 1'b1		0x20 18
	 * USB20_DP_100K_EN	1'b0		0x20 16
	 * RG_USB20_DM_100K_EN	1'b0		0x20 17
	 * RG_USB20_OTG_VBUSCMP_EN	1'b1	0x18 20
	 * force_suspendm		1'b0	0x68 18
	*/

	/* force_uart_en, 1'b0 */
	USBPHY_CLR32(0x68, (0x1 << 26));
	/* RG_UART_EN, 1'b0 */
	USBPHY_CLR32(0x6c, (0x1 << 16));
	/* rg_usb20_gpio_ctl, 1'b0, usb20_gpio_mode, 1'b0 */
	USBPHY_CLR32(0x20, ((0x1 << 9) | (0x1 << 8)));

	/* RG_USB20_BC11_SW_EN, 1'b0 */
	USBPHY_CLR32(0x18, (0x1 << 23));

	/* rg_usb20_dp_100k_mode, 1'b1 */
	USBPHY_SET32(0x20, (0x1 << 18));
	/* USB20_DP_100K_EN 1'b0, RG_USB20_DM_100K_EN, 1'b0 */
	USBPHY_CLR32(0x20, ((0x1 << 16) | (0x1 << 17)));

	/* RG_USB20_OTG_VBUSCMP_EN, 1'b1 */
	USBPHY_SET32(0x18, (0x1 << 20));

	/* force_suspendm, 1'b0 */
	USBPHY_CLR32(0x68, (0x1 << 18));

	/* wait for 800 usec. */
	udelay(800);
}

void mt_usb_phy_savecurrent(void)
{
	/*
	 * force_uart_en	1'b0		0x68 26
	 * RG_UART_EN		1'b0		0x6c 16
	 * rg_usb20_gpio_ctl	1'b0		0x20 09
	 * usb20_gpio_mode	1'b0		0x20 08

	 * RG_USB20_BC11_SW_EN	1'b0		0x18 23
	 * RG_USB20_OTG_VBUSCMP_EN	1'b0	0x18 20
	 * RG_SUSPENDM		1'b1		0x68 03
	 * force_suspendm	1'b1		0x68 18

	 * RG_DPPULLDOWN	1'b1		0x68 06
	 * RG_DMPULLDOWN	1'b1		0x68 07
	 * RG_XCVRSEL[1:0]	2'b01		0x68 [04-05]
	 * RG_TERMSEL		1'b1		0x68 02
	 * RG_DATAIN[3:0]	4'b0000		0x68 [10-13]
	 * force_dp_pulldown	1'b1		0x68 20
	 * force_dm_pulldown	1'b1		0x68 21
	 * force_xcversel	1'b1		0x68 19
	 * force_termsel	1'b1		0x68 17
	 * force_datain		1'b1		0x68 23

	 * RG_SUSPENDM		1'b0		0x68 03
	*/

	/* force_uart_en, 1'b0 */
	USBPHY_CLR32(0x68, (0x1 << 26));
	/* RG_UART_EN, 1'b0 */
	USBPHY_CLR32(0x6c, (0x1 << 16));
	/* rg_usb20_gpio_ctl, 1'b0, usb20_gpio_mode, 1'b0 */
	USBPHY_CLR32(0x20, (0x1 << 9));
	USBPHY_CLR32(0x20, (0x1 << 8));

	/* RG_USB20_BC11_SW_EN, 1'b0 */
	USBPHY_CLR32(0x18, (0x1 << 23));
	/* RG_USB20_OTG_VBUSCMP_EN, 1'b0 */
	USBPHY_CLR32(0x18, (0x1 << 20));

	/* RG_SUSPENDM, 1'b1 */
	USBPHY_SET32(0x68, (0x1 << 3));
	/* force_suspendm, 1'b1 */
	USBPHY_SET32(0x68, (0x1 << 18));

	/* RG_DPPULLDOWN, 1'b1, RG_DMPULLDOWN, 1'b1 */
	USBPHY_SET32(0x68, ((0x1 << 6) | (0x1 << 7)));

	/* RG_XCVRSEL[1:0], 2'b01. */
	USBPHY_CLR32(0x68, (0x3 << 4));
	USBPHY_SET32(0x68, (0x1 << 4));
	/* RG_TERMSEL, 1'b1 */
	USBPHY_SET32(0x68, (0x1 << 2));
	/* RG_DATAIN[3:0], 4'b0000 */
	USBPHY_CLR32(0x68, (0xF << 10));

	/* force_dp_pulldown, 1'b1, force_dm_pulldown, 1'b1,
	 * force_xcversel, 1'b1, force_termsel, 1'b1, force_datain, 1'b1
	 */
	USBPHY_SET32(0x68, ((0x1 << 20) | (0x1 << 21) | (0x1 << 19) | (0x1 << 17) | (0x1 << 23)));

	udelay(800);

	/* RG_SUSPENDM, 1'b0 */
	USBPHY_CLR32(0x68, (0x1 << 3));

	udelay(1);

}

void mt_usb_phy_recover(void)
{
    /*
	 * 04.force_uart_en	1'b0 0x68 26
	 * 04.RG_UART_EN		1'b0 0x6C 16
	 * 04.rg_usb20_gpio_ctl	1'b0 0x20 09
	 * 04.usb20_gpio_mode	1'b0 0x20 08

	 * 05.force_suspendm	1'b0 0x68 18

	 * 06.RG_DPPULLDOWN	1'b0 0x68 06
	 * 07.RG_DMPULLDOWN	1'b0 0x68 07
	 * 08.RG_XCVRSEL[1:0]	2'b00 0x68 [04:05]
	 * 09.RG_TERMSEL		1'b0 0x68 02
	 * 10.RG_DATAIN[3:0]	4'b0000 0x68 [10:13]
	 * 11.force_dp_pulldown	1'b0 0x68 20
	 * 12.force_dm_pulldown	1'b0 0x68 21
	 * 13.force_xcversel	1'b0 0x68 19
	 * 14.force_termsel	1'b0 0x68 17
	 * 15.force_datain	1'b0 0x68 23
	 * 16.RG_USB20_BC11_SW_EN	1'b0 0x18 23
	 * 17.RG_USB20_OTG_VBUSCMP_EN	1'b1 0x18 20
	*/

	/* clean PUPD_BIST_EN */
	/* PUPD_BIST_EN = 1'b0 */
	/* PMIC will use it to detect charger type */
	/* NEED?? USBPHY_CLR8(0x1d, 0x10);*/
	USBPHY_CLR32(0x1c, (0x1 << 12));

	/* force_uart_en, 1'b0 */
	USBPHY_CLR32(0x68, (0x1 << 26));
	/* RG_UART_EN, 1'b0 */
	USBPHY_CLR32(0x6C, (0x1 << 16));
	/* rg_usb20_gpio_ctl, 1'b0, usb20_gpio_mode, 1'b0 */
	USBPHY_CLR32(0x20, (0x1 << 9));
	USBPHY_CLR32(0x20, (0x1 << 8));

	/* force_suspendm, 1'b0 */
	USBPHY_CLR32(0x68, (0x1 << 18));

	/* RG_DPPULLDOWN, 1'b0, RG_DMPULLDOWN, 1'b0 */
	USBPHY_CLR32(0x68, ((0x1 << 6) | (0x1 << 7)));

	/* RG_XCVRSEL[1:0], 2'b00. */
	USBPHY_CLR32(0x68, (0x3 << 4));

	/* RG_TERMSEL, 1'b0 */
	USBPHY_CLR32(0x68, (0x1 << 2));
	/* RG_DATAIN[3:0], 4'b0000 */
	USBPHY_CLR32(0x68, (0xF << 10));

	/* force_dp_pulldown, 1'b0, force_dm_pulldown, 1'b0,
	 * force_xcversel, 1'b0, force_termsel, 1'b0, force_datain, 1'b0
	 */
	USBPHY_CLR32(0x68, ((0x1 << 20) | (0x1 << 21) | (0x1 << 19) | (0x1 << 17) | (0x1 << 23)));

	/* RG_USB20_BC11_SW_EN, 1'b0 */
	USBPHY_CLR32(0x18, (0x1 << 23));
	/* RG_USB20_OTG_VBUSCMP_EN, 1'b1 */
	USBPHY_SET32(0x18, (0x1 << 20));

	/* wait 800 usec. */
	udelay(800);

	/* force enter device mode */
	USBPHY_CLR32(0x6C, (0x10<<0));
	USBPHY_SET32(0x6C, (0x2F<<0));
	USBPHY_SET32(0x6C, (0x3F<<8));
}

void mt_usb11_phy_savecurrent(void)
{
}

#endif

void Charger_Detect_Init(void)
{
    /* RG_USB20_BC11_SW_EN = 1'b1 */
    USBPHY_CLR32(0x18, (0x1 << 23));
}

u32 Charger_Detect_Release(void)
{
    /* check RG_SUSPENDM */
    return (USBPHY_READ32(0x68) & (0x1 << 3));
}
