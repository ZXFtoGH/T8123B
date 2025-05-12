/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein is
 * confidential and proprietary to MediaTek Inc. and/or its licensors. Without
 * the prior written permission of MediaTek inc. and/or its licensors, any
 * reproduction, modification, use or disclosure of MediaTek Software, and
 * information contained herein, in whole or in part, shall be strictly
 * prohibited.
 *
 * MediaTek Inc. (C) 2010. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER
 * ON AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL
 * WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR
 * NONINFRINGEMENT. NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH
 * RESPECT TO THE SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY,
 * INCORPORATED IN, OR SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES
 * TO LOOK ONLY TO SUCH THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO.
 * RECEIVER EXPRESSLY ACKNOWLEDGES THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO
 * OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES CONTAINED IN MEDIATEK
 * SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE
 * RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S
 * ENTIRE AND CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE
 * RELEASED HEREUNDER WILL BE, AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE
 * MEDIATEK SOFTWARE AT ISSUE, OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE
 * CHARGE PAID BY RECEIVER TO MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 * The following software/firmware and/or related documentation ("MediaTek
 * Software") have been modified by MediaTek Inc. All revisions are subject to
 * any receiver's applicable license agreements with MediaTek Inc.
 */
#include <typedefs.h>
#include <mt6685_rtc.h>
#include <timer.h>
#include <cust_rtc.h>
#include <mt6685_hw.h>
#include <mt6685_rtc_hw.h>
#include <pal_log.h>
#include <spmi.h>
#include <spmi_sw.h>

#define RTC_GPIO_USER_MASK	  (((1U << 13) - 1) & 0xff00)
#define RTCTAG	"[RTC]"
#define RTC_ERR(fmt, arg...) pal_log_err(RTCTAG fmt, ##arg)
#define RTC_WARN(fmt, arg...) pal_log_warn(RTCTAG fmt, ##arg)
#define RTC_INFO(fmt, arg...) pal_log_info(RTCTAG fmt, ##arg)
#define RTC_DBG(fmt, arg...) pal_log_debug(RTCTAG fmt, ##arg)

static bool recovery_flag = false;
static bool rtc_busy_wait(void);
static bool Write_trigger(void);
static U16 eosc_cali(void);
static bool rtc_first_boot_init(U16 *result);
static U16 get_frequency_meter(U16 val, U16 measureSrc, U16 window_size);
static bool rtc_frequency_meter_check(void);
static void rtc_recovery_flow(void);
static bool rtc_recovery_mode_check(void);
static bool rtc_init_after_recovery(void);
static bool rtc_get_recovery_mode_stat(void);
static bool rtc_gpio_init(void);
static bool rtc_hw_init(void);
static bool rtc_android_init(void);
static bool rtc_lpd_init(void);
static bool Writeif_unlock(void);
static bool rtc_2sec_stat_clear(void);
void rtc_disable_2sec_reboot(void);
void rtc_enable_2sec_reboot(void);
void rtc_save_2sec_stat(void);
void rtc_set_xomode(bool bIs32kLess);
static bool rtc_lpd_enable_check(void);

#if CFG_GZ_SUPPORT
static bool secure_rtc_flag ;
void secure_rtc_init(void);
void secure_rtc_set_ck(void);
#endif

struct spmi_device *mt6685_sdev = NULL;

static void rtc_bus_init()
{
	mt6685_sdev = get_spmi_device(SPMI_MASTER_1, SPMI_SLAVE_9);
	if (!mt6685_sdev) {
		RTC_ERR("%s: get spmi device fail\n", __func__);
	}
}

static U16 rtc_config_interface(U16 RegNum, U16 val, U16 MASK, U16 SHIFT)
{
	int ret = 0;
	u16 org = 0;

	if (mt6685_sdev == NULL)
		rtc_bus_init();

	ret = spmi_ext_register_readl(mt6685_sdev, RegNum, &org, 1);
	if (ret < 0) {
		dbg_print("%s: fail, addr = 0x%x, ret = %d\n",
			  __func__, RegNum, ret);
		return ret;
	}
	org &= ~(MASK << SHIFT);
	org |= (val << SHIFT);

	ret = spmi_ext_register_writel(mt6685_sdev, RegNum, &org, 1);
	if (ret < 0) {
		dbg_print("%s: fail, addr = 0x%x, ret = %d\n",
			  __func__, RegNum, ret);
		return ret;
	}
	return 0;
}

static U16 rtc_config_interface_read(U16 addr)
{
	int ret = 0;
	U8 rdata=0;

	if (mt6685_sdev == NULL)
		rtc_bus_init();

	ret = spmi_ext_register_readl(mt6685_sdev, addr, &rdata, 1);
	if (ret < 0) {
		RTC_ERR("%s: fail, addr = 0x%x, ret = %d\n",
			   __func__, addr, ret);
		return ret;
	}

	return rdata;
}

static U16 RTC_Read(U16 addr)
{
	int ret = 0;
	U16 rdata=0;

	if (mt6685_sdev == NULL)
		rtc_bus_init();

	ret = spmi_ext_register_readl(mt6685_sdev, addr, &rdata, 2);
	if (ret < 0) {
		RTC_ERR("%s: fail, addr = 0x%x, ret = %d\n",
			   __func__, addr, ret);
		return ret;
	}

	return rdata;
}

static RTC_Write(U16 addr, U16 data)
{
	int ret = 0;
	U16 rdata=0;

	if (mt6685_sdev == NULL)
		rtc_bus_init();

	ret = spmi_ext_register_writel(mt6685_sdev, addr, &data, 2);
	if (ret < 0) {
		RTC_ERR("%s: fail, addr = 0x%x, ret = %d\n",
			  __func__, addr, ret);
		return ret;
	}

	return 0;
}

static bool rtc_busy_wait(void)
{
	unsigned long begin = get_timer(0);

	do {
		while (RTC_Read(RTC_BBPU) & RTC_BBPU_CBUSY) {
			/* Time > 1sec,  time out and set recovery mode enable. */
			if (get_timer(begin) > 1000) {
				RTC_ERR("rtc cbusy time out!!!!!\n");
				return false;
			}
		}
	} while (0);
	return true;
}

static bool rtc_spar_alarm_clear_wait(void)
{
	unsigned long begin = get_timer(0);

	while ((RTC_Read(RTC_BBPU) & RTC_BBPU_RESET_ALARM) || (RTC_Read(RTC_BBPU) & RTC_BBPU_RESET_SPAR))
	{
		if (get_timer(begin) > 500)
		{
			RTC_ERR("rtc spar/alarm clear time out!!!!!\n");
			return false;
		}
	}
	return true;
}

static void rtc_call_exception(void)
{
	//ASSERT(0);
}

static bool rtc_eosc_check_clock(U16 *result)
{
	if ((result[0] >= 3  &&result[0] <= 7 )&&
			(result[2] > 2 && result[2] < 9) &&
			(result[3] > 300 && result[3] < 10400))
		return true;
	else
		return false;
}

static void rtc_xosc_write(U16 val)
{
	RTC_Write(RTC_OSC32CON, RTC_OSC32CON_UNLOCK1);
	mdelay(1);

	RTC_Write(RTC_OSC32CON, RTC_OSC32CON_UNLOCK2);
	mdelay(1);

	RTC_Write(RTC_OSC32CON, val);
	mdelay(1);
}

static U16 get_frequency_meter(U16 val, U16 measureSrc, U16 window_size)
{
	U16 ret, data;
	unsigned long begin = 0;

	if (val != 0) {
		U16 osc32con;
		RTC_Write(RTC_BBPU, RTC_Read(RTC_BBPU) | RTC_BBPU_KEY | RTC_BBPU_RELOAD);
		Write_trigger();
		osc32con = RTC_Read(RTC_OSC32CON) & 0xFFE0;
		rtc_xosc_write(osc32con | (val & RTC_XOSCCALI_MASK));
	}

	// RG_BANK_FQMTR_RST = 1 reset FQMTR
	rtc_config_interface(RG_BANK_FQMTR_RST, 1, RG_BANK_FQMTR_RST_MASK, RG_BANK_FQMTR_RST_SHIFT);
	udelay(20);
	// RG_BANK_FQMTR_RST = 0 release FQMTR
	rtc_config_interface(RG_BANK_FQMTR_RST, 0, RG_BANK_FQMTR_RST_MASK, RG_BANK_FQMTR_RST_SHIFT);

	//Write Protection Key to unlock TOP_CKPDN_CON0
	rtc_config_interface(TOP_DIG_WPK, 0x15, DIG_WPK_KEY_MASK , DIG_WPK_KEY_SHIFT);
	rtc_config_interface(TOP_DIG_WPK_H, 0x63, DIG_WPK_KEY_H_MASK, DIG_WPK_KEY_H_SHIFT);

	rtc_config_interface(RG_FQMTR_CLK_CK_PDN_CLR, 1, RG_FQMTR_CLK_CK_PDN_MASK, RG_FQMTR_CLK_CK_PDN_SHIFT);
	rtc_config_interface(RG_FQMTR_32K_CK_PDN_CLR, 1, RG_FQMTR_32K_CK_PDN_MASK, RG_FQMTR_32K_CK_PDN_SHIFT);

	rtc_config_interface(RG_FQMTR_DCXO26M_EN, 1, RG_FQMTR_DCXO26M_MASK, RG_FQMTR_DCXO26M_SHIFT);
	RTC_Write(RG_FQMTR_WINSET, window_size); //set freq. meter window value (0=1X32K(fix clock))
	rtc_config_interface(RG_FQMTR_TCKSEL, FQMTR_DCXO26M_EN | measureSrc, RG_FQMTR_TCKSEL_MASK, RG_FQMTR_TCKSEL_SHIFT);
	mdelay(1); //designer suggest : enable 26M -> delay 100us -> enable FQMTR
	rtc_config_interface(RG_FQMTR_EN, 1, RG_FQMTR_EN_MASK, RG_FQMTR_EN_SHIFT);
	mdelay(1);

	while((FQMTR_BUSY & RTC_Read(RG_FQMTR_BUSY)) == FQMTR_BUSY)
	{
		if (begin > 1000)
		{
			RTC_ERR("get frequency time out\n");
			break;
		}
		begin++;
		mdelay(1);
	};		// FQMTR read until ready
	/* read data should be closed to 26M/32k = 794 */
	ret = RTC_Read(RG_FQMTR_DATA);

	rtc_config_interface(RG_FQMTR_DCXO26M_EN, 0, RG_FQMTR_DCXO26M_MASK, RG_FQMTR_DCXO26M_SHIFT);
	rtc_config_interface(RG_FQMTR_TCKSEL, 0, RG_FQMTR_DCXO26M_MASK, RG_FQMTR_DCXO26M_SHIFT);
	mdelay(1); //designer suggest : disable FQMTR -> delay 100us -> disable 26M
	rtc_config_interface(RG_FQMTR_EN, 0, RG_FQMTR_EN_MASK, RG_FQMTR_EN_SHIFT);

	RTC_INFO("%s: input=0x%x, ouput=%d\n", __func__, val, ret);

	rtc_config_interface(RG_FQMTR_CLK_CK_PDN_SET, 1, RG_FQMTR_CLK_CK_PDN_MASK, RG_FQMTR_CLK_CK_PDN_SHIFT);
	rtc_config_interface(RG_FQMTR_32K_CK_PDN_SET, 1, RG_FQMTR_32K_CK_PDN_MASK, RG_FQMTR_32K_CK_PDN_SHIFT);

	return ret;
}

static void rtc_measure_four_clock(U16 *result)
{
	U16 window_size, data;
	rtc_config_interface(RG_FQMTR_CKSEL, FQMTR_FIX_CLK_26M, RG_FQMTR_CKSEL_MASK, RG_FQMTR_CKSEL_SHIFT);
	window_size = 4;
	mdelay(1);
	result[0] = get_frequency_meter(0, FQMTR_FQM26M_CK, window_size); 		//select 26M as target clock

	rtc_config_interface(RG_FQMTR_CKSEL, FQMTR_FIX_CLK_26M, RG_FQMTR_CKSEL_MASK, RG_FQMTR_CKSEL_SHIFT);
	window_size = 3970;  // (26M / 32K) * 5
	mdelay(1);
	result[2] = get_frequency_meter(0, FQMTR_DCXO_F32K_CK, window_size); 		//select DCXO_32 as target clock

	rtc_config_interface(RG_FQMTR_CKSEL, FQMTR_FIX_CLK_EOSC_32K, RG_FQMTR_CKSEL_MASK, RG_FQMTR_CKSEL_SHIFT);
	window_size = 4;
	mdelay(1);
	result[3] = get_frequency_meter(0, FQMTR_FQM26M_CK, window_size);		//select 26M as target clock
}

static void rtc_switch_mode(bool XOSC, bool recovery)
{
	U16 osc32con;

	if (recovery){
		/* 1: without the external xtal */
		rtc_config_interface(SCK_TOP_XTAL_SEL_ADDR,1,SCK_TOP_XTAL_SEL_MASK,SCK_TOP_XTAL_SEL_SHIFT);
		mdelay(100);
	}
	osc32con = OSC32CON_ANALOG_SETTING | RTC_REG_XOSC32_ENB;
	rtc_xosc_write(osc32con); /*crystal not exist + eosc cali = 0xF*/
	mdelay(10);
}

static void rtc_switch_to_dcxo_mode(void)
{
	rtc_switch_mode(false, false);
}

static void rtc_switch_to_dcxo_recv_mode(void)
{
	rtc_switch_mode(false, true);
}

static bool rtc_frequency_meter_check(void)
{
	U16  result[4], osc32con, val = 0;
	unsigned long begin;

	if (!rtc_get_recovery_mode_stat())
		rtc_switch_to_dcxo_mode();
	else
		rtc_switch_to_dcxo_recv_mode();

	begin = get_timer(0);
	do {

		if (get_timer(begin) > 1000)
		{
			RTC_ERR("without eosc clock!\n");
			return false;
		}
		rtc_measure_four_clock(result);

	} while(!rtc_eosc_check_clock(result));

	val = eosc_cali();
	RTC_INFO("EOSC cali val = 0x%x\n", val);
	//EMB_HW_Mode
	osc32con = OSC32CON_ANALOG_SETTING | RTC_REG_XOSC32_ENB;
	val = (val & RTC_XOSCCALI_MASK) | osc32con;
	RTC_INFO("EOSC cali val = 0x%x\n", val);
	rtc_xosc_write(val);

	return true;
}

static void rtc_set_recovery_mode_stat(bool enable)
{
	recovery_flag = enable;
}

static bool rtc_get_recovery_mode_stat(void)
{
	return recovery_flag;
}

static bool rtc_init_after_recovery(void)
{
	RTC_INFO("%s write powerkey\n", __func__);

	/* write powerkeys */
	RTC_Write(RTC_POWERKEY1, RTC_POWERKEY1_KEY);
	RTC_Write(RTC_POWERKEY2, RTC_POWERKEY2_KEY);
	if (!Write_trigger())
		return false;
	if (!Writeif_unlock())
		return false;
	if (!rtc_gpio_init())
		return false;
	if (!rtc_hw_init())
		return false;
	if (!rtc_android_init())
		return false;
	if (!rtc_lpd_init())
		return false;

	RTC_Write(RTC_POWERKEY1, RTC_POWERKEY1_KEY);
	RTC_Write(RTC_POWERKEY2, RTC_POWERKEY2_KEY);
	if (!Write_trigger())
		return false;
	if (!Writeif_unlock())
		return false;

#if CFG_GZ_SUPPORT
	secure_rtc_init();
#endif

	RTC_INFO("%s done\n", __func__);
	return true;
}

static bool rtc_recovery_mode_check(void)
{
	// fix me add return ret for recovery mode check fail
	if (!rtc_frequency_meter_check())
	{
		rtc_call_exception();
		return false;
	}
	return true;
}

static void rtc_recovery_flow(void)
{
	U8 count = 0;

	RTC_WARN("%s\n", __func__);

	rtc_set_recovery_mode_stat(true);

	while (count < 3)
	{
		if(rtc_recovery_mode_check())
		{
			if (rtc_init_after_recovery())
				break;
		}
		count++;
	}
	rtc_set_recovery_mode_stat(false);
	if (count == 3)
		rtc_call_exception();
}

static unsigned long rtc_mktime(int yea, int mth, int dom, int hou, int min, int sec)
{
	unsigned long d1, d2, d3;
	mth -= 2;
	if (mth <= 0) {
		mth += 12;
		yea -= 1;
	}

	d1 = (yea - 1) * 365 + (yea / 4 - yea / 100 + yea / 400);
	d2 = (367 * mth / 12 - 30) + 59;
	d3 = d1 + d2 + (dom - 1) - 719162;

	return ((d3 * 24 + hou) * 60 + min) * 60 + sec;
}

static bool Write_trigger(void)
{
	RTC_Write(RTC_WRTGR, 1);

	if (rtc_busy_wait())
		return true;

	else
		return false;
}

static bool Writeif_unlock(void)
{
	RTC_Write(RTC_PROT, RTC_PROT_UNLOCK1);
	if (!Write_trigger())
		return false;
	RTC_Write(RTC_PROT, RTC_PROT_UNLOCK2);
	if (!Write_trigger())
		return false;

	return true;
}

static bool rtc_hw_init(void)
{
	unsigned long begin = get_timer(0);

	RTC_Write(RTC_BBPU, RTC_Read(RTC_BBPU) | RTC_BBPU_KEY | RTC_BBPU_RESET_ALARM | RTC_BBPU_RESET_SPAR & (~RTC_BBPU_SPAR_SW));
	Write_trigger();

	do {

		if (get_timer(begin) > 500) {
			RTC_ERR("%s time out!\n", __func__);
			return false;
		}
		RTC_Write(RTC_BBPU, RTC_Read(RTC_BBPU) | RTC_BBPU_KEY | RTC_BBPU_RELOAD);
		Write_trigger();

	} while ((RTC_Read(RTC_BBPU) & RTC_BBPU_RESET_ALARM) || (RTC_Read(RTC_BBPU) & RTC_BBPU_RESET_SPAR));

	return true;
}

static bool rtc_android_init(void)
{
	U16 irqsta;

	RTC_Write(RTC_IRQ_EN, 0);
	RTC_Write(RTC_CII_EN, 0);
	RTC_Write(RTC_AL_MASK, 0);

	RTC_Write(RTC_AL_YEA, (RTC_Read(RTC_AL_YEA) & ~(RTC_AL_YEA_MASK) | ((1970 - RTC_MIN_YEAR) & RTC_AL_YEA_MASK)));
	RTC_Write(RTC_AL_MTH, 1);
	RTC_Write(RTC_AL_DOM, 1);	/* NEW_SPARE1[0] = 0 */
	RTC_Write(RTC_AL_DOW, 1);
	RTC_Write(RTC_AL_DOW, (RTC_RG_EOSC_CALI_TD_8SEC | 1));	/* set RTC EOSC calibration period = 8sec */
	RTC_Write(RTC_AL_HOU, RTC_Read(RTC_AL_HOU) & RTC_NEW_SPARE0);
	RTC_Write(RTC_AL_MIN, 0);
	RTC_Write(RTC_AL_SEC, RTC_Read(RTC_AL_SEC) & (~RTC_AL_SEC_MASK));

	RTC_Write(RTC_PDN1, RTC_PDN1_DEBUG);   /* set Debug bit */
	RTC_Write(RTC_PDN2, ((1970 - RTC_MIN_YEAR) << RTC_PDN2_PWRON_YEA_SHIFT) | 1);
	RTC_Write(RTC_SPAR0, 0);
	RTC_Write(RTC_SPAR1, (1 << RTC_SPAR1_PWRON_DOM_SHIFT));

	RTC_Write(RTC_DIFF, 0);
	RTC_Write(RTC_CALI, 0);
	if (!Write_trigger())
		return false;

	rtc_disable_2sec_reboot();

	if (!rtc_2sec_stat_clear())
		return false;
	if (!Write_trigger())
		return false;

	irqsta = RTC_Read(RTC_IRQ_STA);	/* read clear */

	/* init time counters after resetting RTC_DIFF and RTC_CALI */
	RTC_Write(RTC_TC_YEA, RTC_DEFAULT_YEA - RTC_MIN_YEAR);
	RTC_Write(RTC_TC_MTH, RTC_DEFAULT_MTH);
	RTC_Write(RTC_TC_DOM, RTC_DEFAULT_DOM);
	RTC_Write(RTC_TC_DOW, 1);
	RTC_Write(RTC_TC_HOU, 0);
	RTC_Write(RTC_TC_MIN, 0);
	RTC_Write(RTC_TC_SEC, 0);
	if(!Write_trigger())
		return false;

	return true;
}

static bool rtc_gpio_init(void)
{
	U16 con;

	/* GPI mode and pull enable + pull down */
	con = RTC_Read(RTC_CON) & (RTC_CON_LPSTA_RAW | RTC_CON_LPRST | RTC_XOSC32_LPEN | RTC_EOSC32_LPEN);
	con &= ~RTC_CON_GPU;
	con &= ~RTC_CON_F32KOB; //for avoid leak current
	con |= RTC_CON_GPEN | RTC_CON_GOE;
	RTC_Write(RTC_CON, con);
	if (Write_trigger())
		return true;
	else
		return false;
}

static U16 eosc_cali(void)
{
	U16 val, diff1, diff2, regval;
	int middle;
	int left = RTC_XOSCCALI_START, right = RTC_XOSCCALI_END;

	regval = RTC_Read(RG_FQMTR_CKSEL) & (~(RG_FQMTR_CKSEL_MASK << RG_FQMTR_CKSEL_SHIFT));
	RTC_Write(RG_FQMTR_CKSEL, regval | FQMTR_FIX_CLK_EOSC_32K);//select EOSC_32 as fixed clock
	RTC_INFO("%s: RG_FQMTR_CKSEL=0x%x\n", __func__, RTC_Read(RG_FQMTR_CKSEL));

	while (left <= right)
	{
		middle = (right + left) / 2;
		if (middle == left)
			break;

		val = get_frequency_meter(middle, FQMTR_FQM26M_CK, 0);//select 26M as target clock
		RTC_INFO("%s: val=0x%x\n", __func__, val);
		if ((val >= RTC_FQMTR_LOW_BASE) && (val <= RTC_FQMTR_HIGH_BASE))
			break;
		if (val > RTC_FQMTR_HIGH_BASE)
			right = middle;
		else
			left = middle;
	}

	if ((val >= RTC_FQMTR_LOW_BASE) && (val <= RTC_FQMTR_HIGH_BASE))
		return middle;

	val = get_frequency_meter(left, FQMTR_FQM26M_CK, 0);
	if (val > RTC_FQMTR_LOW_BASE)
		diff1 = val - RTC_FQMTR_LOW_BASE;
	else
		diff1 = RTC_FQMTR_LOW_BASE - val;

	val = get_frequency_meter(right, FQMTR_FQM26M_CK, 0);
	if (val > RTC_FQMTR_LOW_BASE)
		diff2 = val - RTC_FQMTR_LOW_BASE;
	else
		diff2 = RTC_FQMTR_LOW_BASE - val;

	if (diff1 < diff2)
		return left;
	else
		return right;
}

static void rtc_lpd_state_clr(void)
{
	U16 spar0;

	spar0 = RTC_Read(RTC_SPAR0);
	RTC_Write(RTC_SPAR0, spar0 & (~RTC_PDN1_PWRON_TIME));	/* bit 7 for low power detected in preloader */
	Write_trigger();

	spar0 = RTC_Read(RTC_SPAR0);
	RTC_INFO("RTC_SPAR0=0x%x \n", spar0);
}

static void rtc_osc_init(void)
{
	U16 osc32con, val;

	/* disable 32K export if there are no RTC_GPIO users */
	if (!(RTC_Read(RTC_PDN1) & RTC_GPIO_USER_MASK))
		rtc_gpio_init();

	val = eosc_cali();
	osc32con = OSC32CON_ANALOG_SETTING | RTC_REG_XOSC32_ENB;
	val = (val & RTC_XOSCCALI_MASK) | osc32con;
	RTC_INFO("EOSC cali val = 0x%x\n", val);
	//SW_EOSC_Mode
	rtc_xosc_write(val);

	rtc_lpd_state_clr();
}

static bool rtc_lpd_init(void)
{
	U16 con;

	// Enable EOSC LPD only
	RTC_INFO("Enable EOSC LPD only\n");
	RTC_Write(RTC_AL_SEC, (RTC_Read(RTC_AL_SEC) & (~RTC_LPD_OPT_MASK)) | RTC_LPD_OPT_EOSC_LPD);
	Write_trigger();

	con = RTC_Read(RTC_CON) | RTC_XOSC32_LPEN;
	con &= ~RTC_CON_LPRST;
	RTC_Write(RTC_CON, con);
	if (!Write_trigger())
		return false;

	con |= RTC_CON_LPRST;
	RTC_Write(RTC_CON, con);
	if (!Write_trigger())
		return false;

	con &= ~RTC_CON_LPRST;
	RTC_Write(RTC_CON, con);
	if (!Write_trigger())
		return false;

	con = RTC_Read(RTC_CON) | RTC_EOSC32_LPEN;
	con &= ~RTC_CON_LPRST;
	RTC_Write(RTC_CON, con);
	if (!Write_trigger())
		return false;

	con |= RTC_CON_LPRST;
	RTC_Write(RTC_CON, con);
	if (!Write_trigger())
		return false;

	con &= ~RTC_CON_LPRST;
	RTC_Write(RTC_CON, con);
	if (!Write_trigger())
		return false;

	RTC_INFO("%s RTC_CON=0x%x\n", __func__, RTC_Read(RTC_CON));

	RTC_Write(RTC_SPAR0, RTC_Read(RTC_SPAR0) | RTC_PDN1_PWRON_TIME);	/* bit 7 for low power detected in preloader */
	if (!Write_trigger())
		return false;

	return true;
}

static bool rtc_first_boot_init(U16 *result)
{
	RTC_INFO("%s\n", __func__);

	/* turn on DCXO */
	rtc_set_xomode(true);

	rtc_switch_to_dcxo_recv_mode();

	RTC_Write(RTC_BBPU, RTC_Read(RTC_BBPU) | RTC_BBPU_KEY | RTC_BBPU_RESET_SPAR);
	Write_trigger();

	if (!Writeif_unlock())
	{
		RTC_ERR("Writeif_unlock fail1\n");
		return false;
	}

	if (!rtc_gpio_init())
		return false;

	/* write powerkeys */
	RTC_Write(RTC_AL_SEC, RTC_Read(RTC_AL_SEC) & (~RTC_K_EOSC32_VTCXO_ON_SEL));
	RTC_Write(RTC_AL_YEA, (RTC_Read(RTC_AL_YEA) & RTC_AL_YEA_MASK) | RTC_K_EOSC_RSV_7 | RTC_K_EOSC_RSV_6);
	RTC_Write(RTC_POWERKEY1, RTC_POWERKEY1_KEY);
	RTC_Write(RTC_POWERKEY2, RTC_POWERKEY2_KEY);

	if (!Writeif_unlock())
	{
		RTC_ERR("Writeif_unlock fail\n");
	}
	RTC_Write(RTC_BBPU, RTC_Read(RTC_BBPU) | RTC_BBPU_KEY | RTC_BBPU_RESET_SPAR);
	Write_trigger();

	if (!Write_trigger())
	{
		RTC_ERR("%s Write_trigger fail1\n", __func__);
		return false;
	}
	if (!Writeif_unlock())
	{
		RTC_ERR("%s Writeif_unlock fail2\n", __func__);
		return false;
	}
	if (!rtc_lpd_init())
		return false;

	//MT6351 need write POWERKEY again to unlock RTC
	RTC_Write(RTC_POWERKEY1, RTC_POWERKEY1_KEY);
	RTC_Write(RTC_POWERKEY2, RTC_POWERKEY2_KEY);

	if (!Write_trigger())
	{
		RTC_ERR("%s Write_trigger fail2\n", __func__);
		return false;
	}
	if (!Writeif_unlock())
	{
		RTC_ERR("%s Writeif_unlock fail3\n", __func__);
		return false;
	}

	if (!rtc_frequency_meter_check())
		return false;

	if (!rtc_hw_init())
		return false;

	if (!rtc_android_init())
		return false;

#if CFG_GZ_SUPPORT
	secure_rtc_init();
#endif
	return true;
}

void rtc_enable_k_eosc(void)
{
	rtc_config_interface(RG_RTC_EOSC32_CK_PDN, 0, RG_RTC_EOSC32_CK_PDN_MASK, RG_RTC_EOSC32_CK_PDN_SHIFT);
	/*Switch the DCXO from 32k-less mode to RTC mode, otherwise, EOSC cali will fail*/

	/* If cali eosc every second, needing to add the following configuration, default period is 8 sec */
	//rtc_config_interface(PMIC_EOSC_CALI_TD_ADDR, 0x3, PMIC_EOSC_CALI_TD_MASK, PMIC_EOSC_CALI_TD_SHIFT);

	RTC_Write(RTC_BBPU, RTC_Read(RTC_BBPU) | RTC_BBPU_KEY | RTC_BBPU_RELOAD);
	Write_trigger();

	/*Enable K EOSC mode for normal power off and then plug out battery */
	RTC_Write(RTC_AL_YEA, ((RTC_Read(RTC_AL_YEA) | RTC_K_EOSC_RSV_0) & (~RTC_K_EOSC_RSV_1)) | RTC_K_EOSC_RSV_2);
	Write_trigger();

	rtc_xosc_write(RTC_Read(RTC_OSC32CON) | RTC_EMBCK_SRC_SEL);

	RTC_Write(RTC_BBPU, RTC_Read(RTC_BBPU) | RTC_BBPU_KEY | RTC_BBPU_RELOAD);
	Write_trigger();

	RTC_INFO("%s bbpu = 0x%x, con = 0x%x, osc32con = 0x%x, sec = 0x%x, yea = 0x%x\n", __func__, RTC_Read(RTC_BBPU), RTC_Read(RTC_CON), RTC_Read(RTC_OSC32CON), RTC_Read(RTC_AL_SEC), RTC_Read(RTC_AL_YEA));
}

void rtc_enable_dcxo(void)
{
	if (!Writeif_unlock()) /* Unlock for reload */
		RTC_ERR("Writeif_unlock() fail\n");

	RTC_Write(RTC_BBPU, RTC_Read(RTC_BBPU) | RTC_BBPU_KEY | RTC_BBPU_RELOAD);
	Write_trigger();
	rtc_xosc_write(RTC_Read(RTC_OSC32CON) & ~RTC_EMBCK_SRC_SEL); //0: f32k_ck src = dcxo_ck

	RTC_Write(RTC_BBPU, RTC_Read(RTC_BBPU) | RTC_BBPU_KEY | RTC_BBPU_RELOAD);
	Write_trigger();

	RTC_INFO("%s con = 0x%x, osc32con = 0x%x, sec = 0x%x\n", __func__, RTC_Read(RTC_CON), RTC_Read(RTC_OSC32CON), RTC_Read(RTC_AL_SEC));
}

void rtc_disable_2sec_reboot(void)
{
	U16 reboot;

	reboot = (RTC_Read(RTC_AL_SEC) & ~RTC_BBPU_2SEC_EN) & ~RTC_BBPU_AUTO_PDN_SEL;
	RTC_Write(RTC_AL_SEC, reboot);
	Write_trigger();
}

void rtc_bbpu_power_down(void)
{
	U16 bbpu;

#if RTC_2SEC_REBOOT_ENABLE
	rtc_disable_2sec_reboot();
#endif
	rtc_enable_k_eosc();
	if (!Writeif_unlock())
		RTC_ERR("%s Writeif_unlock() fail\n", __func__);

	/* pull PWRBB low */
	RTC_Write(RTC_BBPU, RTC_Read(RTC_BBPU) | RTC_BBPU_KEY | RTC_BBPU_PWREN | RTC_BBPU_RESET_ALARM | RTC_BBPU_RESET_SPAR);
	Write_trigger();

	rtc_spar_alarm_clear_wait();

}

void rtc_bbpu_power_on(void)
{
	U16 bbpu;

	/* pull pwrhold high, control by pmic*/
	RTC_Write(RTC_BBPU, RTC_Read(RTC_BBPU) | RTC_BBPU_KEY | RTC_BBPU_PWREN | RTC_BBPU_RESET_ALARM | RTC_BBPU_RESET_SPAR);
	Write_trigger();

	rtc_spar_alarm_clear_wait();

	RTC_INFO("%s done\n", __func__);

#if RTC_2SEC_REBOOT_ENABLE
	rtc_enable_2sec_reboot();
#else
	RTC_Write(RTC_AL_SEC, RTC_Read(RTC_AL_SEC) & ~RTC_BBPU_2SEC_EN);
	Write_trigger();
#endif
}

void rtc_mark_bypass_pwrkey(void)
{
	RTC_INFO("%s: not support\n", __func__);
}

static void rtc_clean_mark(void)
{
	U16 pdn1, pdn2;

	pdn1 = RTC_Read(RTC_PDN1) & ~(RTC_PDN1_DEBUG | RTC_PDN1_BYPASS_PWR);   /* also clear Debug bit */
	pdn2 = RTC_Read(RTC_PDN2) & ~RTC_PDN1_FAC_RESET;
	RTC_Write(RTC_PDN1, pdn1);
	RTC_Write(RTC_PDN2, pdn2);
	Write_trigger();
}

void rtc_bbpu_power_clear(void)
{
	RTC_Write(RTC_BBPU, RTC_Read(RTC_BBPU) | RTC_BBPU_KEY | RTC_BBPU_CLRPKY);
	Write_trigger();
}

U16 rtc_get_spare_fg_value(void)
{
	U16 temp;

	temp = RTC_Read(RTC_AL_MTH) & RTC_AL_MTH_FG_MASK;
	temp = temp >> RTC_AL_MTH_FG_SHIFT;

	return temp;
}

void rtc_init(void)
{
	U16 spar0, result[4];
	bool check_mode_flag = false;
	struct rtc_time tm;

	// If EOSC cali is enabled in last power off. Needing to switch to DCXO clock source,
	// Or the FQMTR can't measure DCXO clock source.
	if((RTC_Read(RTC_DIFF) & RTC_POWER_DETECTED) && !(RTC_Read(RTC_CON) & RTC_CON_LPSTA_RAW) &&
		(RTC_Read(RTC_POWERKEY1) == RTC_POWERKEY1_KEY && RTC_Read(RTC_POWERKEY2) == RTC_POWERKEY2_KEY))
		rtc_enable_dcxo();

	/* check clock source are match with 32K exist */
	rtc_measure_four_clock(result);

	if (!rtc_eosc_check_clock(result)) {
		RTC_ERR("Enter first boot init. \n");
		check_mode_flag = true;
	}

	RTC_INFO("%s#1 powerkey1 = 0x%x, powerkey2 = 0x%x, %s LPD\n", __func__, RTC_Read(RTC_POWERKEY1), RTC_Read(RTC_POWERKEY2)
																, (RTC_Read(RTC_CON) & RTC_CON_LPSTA_RAW) ? "with" : "without" );

	RTC_INFO("bbpu = 0x%x, con = 0x%x, osc32con = 0x%x, sec = 0x%x, yea = 0x%x\n", RTC_Read(RTC_BBPU), RTC_Read(RTC_CON), RTC_Read(RTC_OSC32CON)
																				 , RTC_Read(RTC_AL_SEC), RTC_Read(RTC_AL_YEA));

	if ((RTC_Read(RTC_AL_SEC) & RTC_BBPU_2SEC_STAT_STA) || (RTC_Read(RTC_CON) & RTC_CON_LPSTA_RAW) || check_mode_flag || !(RTC_Read(RTC_DIFF) & RTC_POWER_DETECTED) || !rtc_lpd_enable_check())
	{
		rtc_save_2sec_stat();

		if (!rtc_first_boot_init(result)) {
			rtc_recovery_flow();
		}
	}
	else
	{
	/* normally HW reload is done in BROM but check again here */
		RTC_INFO("%s#2 powerkey1 = 0x%x, powerkey2 = 0x%x\n", __func__,	RTC_Read(RTC_POWERKEY1), RTC_Read(RTC_POWERKEY2));
		RTC_Write(RTC_BBPU, RTC_Read(RTC_BBPU) | RTC_BBPU_KEY | RTC_BBPU_RELOAD);
		if (!Write_trigger())
		{
			rtc_recovery_flow();
		}else
		{
			if (!Writeif_unlock())
			{
				rtc_recovery_flow();
			}else
			{
				RTC_INFO("%s Writeif_unlock\n", __func__);
				if (RTC_Read(RTC_POWERKEY1) != RTC_POWERKEY1_KEY || RTC_Read(RTC_POWERKEY2) != RTC_POWERKEY2_KEY)
				{
					RTC_INFO("%s#3 powerkey1 = 0x%x, powerkey2 = 0x%x\n", __func__, RTC_Read(RTC_POWERKEY1), RTC_Read(RTC_POWERKEY2));
					if (!rtc_first_boot_init(result)) {
						rtc_recovery_flow();
					}
				} else
				{
					rtc_osc_init();
				}
			}
		}
	}
	/* make sure RTC get the latest register info. */
	RTC_Write(RTC_BBPU, RTC_Read(RTC_BBPU) | RTC_BBPU_KEY | RTC_BBPU_RELOAD);
	Write_trigger();

	/* HW K EOSC mode whatever power off (including plug out battery) */
	RTC_Write(RTC_AL_YEA, RTC_Read(RTC_AL_YEA) | RTC_K_EOSC_RSV_0 & (~RTC_K_EOSC_RSV_1) & (~RTC_K_EOSC_RSV_2));

	/*Write Protection Key to unlock TOP_CKPDN_CON0*/
	rtc_config_interface(TOP_DIG_WPK, 0x15, DIG_WPK_KEY_MASK, DIG_WPK_KEY_SHIFT);
	rtc_config_interface(TOP_DIG_WPK_H, 0x63, DIG_WPK_KEY_H_MASK, DIG_WPK_KEY_H_SHIFT);

	//Selects RTC_32K1V8_0_CK clock
	rtc_config_interface(SCK_TOP_CKSEL_CON, 1, R_SCK32K_CK_MASK, R_SCK32K_CK_SHIFT);
	RTC_INFO("SCK_TOP_CKSEL_CON = 0x%x\n", RTC_Read(SCK_TOP_CKSEL_CON));

	/* Truning off eosc cali mode clock */
	rtc_config_interface(SCK_TOP_CKPDN_CON0_L_SET, 1, RG_RTC_EOSC32_CK_PDN_MASK, RG_RTC_EOSC32_CK_PDN_SHIFT);

	rtc_clean_mark();

	/* set register to let MD know 32k status */
	spar0 = RTC_Read(RTC_SPAR0);
	RTC_Write(RTC_SPAR0, (spar0 & ~RTC_SPAR0_32K_LESS) );
	rtc_set_xomode(true);
	RTC_INFO("32k-less mode\n");
	Write_trigger();

	rtc_2sec_stat_clear();
	RTC_Write(RTC_BBPU, RTC_Read(RTC_BBPU) | RTC_BBPU_KEY | RTC_BBPU_RELOAD);
	Write_trigger();

#if RTC_2SEC_REBOOT_ENABLE
	rtc_enable_2sec_reboot();
#endif

#if CFG_GZ_SUPPORT
	secure_rtc_set_ck();
#endif
}

bool rtc_boot_check(void)
{
	U16 irqsta, pdn1, pdn2, spar0, spar1;
	bool kpoc_flag = false;

	irqsta = RTC_Read(RTC_IRQ_STA);	/* Read clear */
	pdn1 = RTC_Read(RTC_PDN1);
	pdn2 = RTC_Read(RTC_PDN2);
	spar0 = RTC_Read(RTC_SPAR0);
	spar1 = RTC_Read(RTC_SPAR1);

	RTC_INFO("irqsta = 0x%x, pdn1 = 0x%x, pdn2 = 0x%x, spar0 = 0x%x, spar1 = 0x%x\n", irqsta, pdn1, pdn2, spar0, spar1);
	RTC_INFO("new_spare0 = 0x%x, new_spare1 = 0x%x, new_spare2 = 0x%x, new_spare3 = 0x%x\n", RTC_Read(RTC_AL_HOU), RTC_Read(RTC_AL_DOM), RTC_Read(RTC_AL_DOW), RTC_Read(RTC_AL_MTH));
	RTC_INFO("bbpu = 0x%x, con = 0x%x, cali = 0x%x, osc32con = 0x%x\n", RTC_Read(RTC_BBPU), RTC_Read(RTC_CON), RTC_Read(RTC_AL_SEC), RTC_Read(RTC_OSC32CON));

#if defined (MTK_KERNEL_POWER_OFF_CHARGING)
	if ((pdn1 & RTC_PDN1_KPOC) == RTC_PDN1_KPOC)
		kpoc_flag = true;
#endif

	if (irqsta & RTC_IRQ_STA_AL || kpoc_flag) {
		if (pdn1 & RTC_PDN1_PWRON_TIME) {	/* power-on time is available */
			U16 now_sec, now_min, now_hou, now_dom, now_mth, now_yea;
			U16 irqen, sec, min, hou, dom, mth, yea;
			unsigned long now_time, time;
			unsigned long time_upper, time_lower;

			/*get current RTC time*/
			now_sec = RTC_Read(RTC_TC_SEC);
			now_min = RTC_Read(RTC_TC_MIN);
			now_hou = RTC_Read(RTC_TC_HOU);
			now_dom = RTC_Read(RTC_TC_DOM);
			now_mth = RTC_Read(RTC_TC_MTH) & RTC_TC_MTH_MASK;
			now_yea = RTC_Read(RTC_TC_YEA) + RTC_MIN_YEAR;
			if (RTC_Read(RTC_TC_SEC) < now_sec) {  /* SEC has carried */
				now_sec = RTC_Read(RTC_TC_SEC);
				now_min = RTC_Read(RTC_TC_MIN);
				now_hou = RTC_Read(RTC_TC_HOU);
				now_dom = RTC_Read(RTC_TC_DOM);
				now_mth = RTC_Read(RTC_TC_MTH) & RTC_TC_MTH_MASK;
				now_yea = RTC_Read(RTC_TC_YEA) + RTC_MIN_YEAR;
			}

			/* get power-on time from SPARE regiters */
			sec = ((spar0 & RTC_SPAR0_PWRON_SEC_MASK) >> RTC_SPAR0_PWRON_SEC_SHIFT);
			min = ((spar1 & RTC_SPAR1_PWRON_MIN_MASK) >> RTC_SPAR1_PWRON_MIN_SHIFT);
			hou = ((spar1 & RTC_SPAR1_PWRON_HOU_MASK) >> RTC_SPAR1_PWRON_HOU_SHIFT);
			dom = ((spar1 & RTC_SPAR1_PWRON_DOM_MASK) >> RTC_SPAR1_PWRON_DOM_SHIFT);
			mth = ((pdn2  & RTC_PDN2_PWRON_MTH_MASK) >> RTC_PDN2_PWRON_MTH_SHIFT);
			yea = ((pdn2  & RTC_PDN2_PWRON_YEA_MASK) >> RTC_PDN2_PWRON_YEA_SHIFT) + RTC_MIN_YEAR;

			now_time = rtc_mktime(now_yea, now_mth, now_dom, now_hou, now_min, now_sec);
			time = rtc_mktime(yea, mth, dom, hou, min, sec);

			RTC_INFO("now = %d/%d/%d %d:%d:%d (%u)\n", now_yea, now_mth, now_dom, now_hou, now_min, now_sec, (unsigned int)now_time);
			RTC_INFO("power-on = %d/%d/%d %d:%d:%d (%u)\n", yea, mth, dom, hou, min, sec, (unsigned int)time);

#if defined (MTK_KERNEL_POWER_OFF_CHARGING)
			if (kpoc_flag == true) {
				time_upper = time + 90;
				time_lower = time - 2;
			} else
#endif
			{
				time_upper = time + 4;
				time_lower = time - 1;
			}
			if (now_time >= time_lower && now_time <= time_upper) {	 /* trigger power on */
				pdn1 = (pdn1 & ~RTC_PDN1_PWRON_TIME);
				RTC_Write(RTC_PDN1, pdn1);
				RTC_Write(RTC_PDN2, pdn2 | RTC_PDN2_PWRON_ALARM);
				Write_trigger();
				if (!(pdn2 & RTC_PDN2_PWRON_LOGO))   /* no logo means ALARM_BOOT */
					g_boot_mode = ALARM_BOOT;
#if defined (MTK_KERNEL_POWER_OFF_CHARGING)
			if (kpoc_flag == true) {
				RTC_Write(RTC_PDN1, pdn1 & ~RTC_PDN1_KPOC);
				Write_trigger();
			}
#endif
				return true;
			} else if (now_time < time) {   /* set power-on alarm */
				RTC_Write(RTC_AL_YEA,  (RTC_Read(RTC_AL_YEA) & (~RTC_AL_YEA_MASK)) | ((yea - RTC_MIN_YEAR) & RTC_AL_YEA_MASK));
				RTC_Write(RTC_AL_MTH, (RTC_Read(RTC_AL_MTH)&RTC_NEW_SPARE3)|mth);
				RTC_Write(RTC_AL_DOM, (RTC_Read(RTC_AL_DOM)&RTC_NEW_SPARE1)|dom);
				RTC_Write(RTC_AL_HOU, (RTC_Read(RTC_AL_HOU)&RTC_NEW_SPARE0)|hou);
				RTC_Write(RTC_AL_MIN, min);
				RTC_Write(RTC_AL_SEC, (RTC_Read(RTC_AL_SEC) & (~RTC_AL_SEC_MASK)) | (sec & RTC_AL_SEC_MASK));
				RTC_Write(RTC_AL_MASK, RTC_AL_MASK_DOW);	/* mask DOW */
				Write_trigger();
				irqen = RTC_Read(RTC_IRQ_EN) | RTC_IRQ_EN_ONESHOT_AL;
				RTC_Write(RTC_IRQ_EN, irqen);
				Write_trigger();
			}
			else{
				pdn1 = (pdn1 & ~RTC_PDN1_PWRON_TIME);
#if defined (MTK_KERNEL_POWER_OFF_CHARGING)
				if (kpoc_flag == true)
					pdn1 = (pdn1 & ~RTC_PDN1_KPOC);
#endif
				RTC_Write(RTC_PDN1, pdn1);
				Write_trigger();
				RTC_INFO("Expired alarm\n");
			}
		}
	}

	if ((pdn1 & RTC_PDN1_RECOVERY_MASK) == RTC_PDN1_FAC_RESET) {	/* factory data reset */
		/* keep bit 4 set until rtc_boot_check() in U-Boot */
		return true;
	}
	return false;
}

static bool g_rtc_2sec_stat;
void __attribute__ ((weak))
	fg_set_2sec_reboot(void)
{
	RTC_INFO("do not support fg_set_2sec_reboot()\n");
}

static bool rtc_2sec_stat_clear(void)
{
	RTC_INFO("%s\n", __func__);

	RTC_Write(RTC_AL_SEC, RTC_Read(RTC_AL_SEC) & ~RTC_BBPU_2SEC_STAT_CLEAR);
	if (!Write_trigger())
		return false;
	RTC_Write(RTC_AL_SEC, RTC_Read(RTC_AL_SEC) | RTC_BBPU_2SEC_STAT_CLEAR);
	if(!Write_trigger())
		return false;
	RTC_Write(RTC_AL_SEC, RTC_Read(RTC_AL_SEC) & ~RTC_BBPU_2SEC_STAT_CLEAR);
	if(!Write_trigger())
		return false;

	return true;
}

void rtc_save_2sec_stat(void)
{
	U16 reboot, pdn2;
	static bool save_stat=false;

	if(save_stat==true)
		return;
	else
		save_stat = true;

	reboot = RTC_Read(RTC_AL_SEC);
	RTC_INFO("rtc_2sec_reboot_check 0x%x, %s 2sec reboot, type 0x%x\n", reboot, (reboot & RTC_BBPU_2SEC_STAT_STA) ? "with" : "without"
																	  ,	(reboot & RTC_BBPU_2SEC_MODE_MSK) >> RTC_BBPU_2SEC_MODE_SHIFT);

	if (reboot & RTC_BBPU_2SEC_EN) {

		switch((reboot & RTC_BBPU_2SEC_MODE_MSK) >> RTC_BBPU_2SEC_MODE_SHIFT) {
			case 0:
			case 1:
			case 2:
				if(reboot & RTC_BBPU_2SEC_STAT_STA) {
					printf("1.RTC_AL_SEC = 0x%x\n", RTC_Read(RTC_AL_SEC));
					printf("1.PONSTS = 0x%x\n\n", rtc_config_interface_read(MT6685_PONSTS));
					g_rtc_2sec_stat = true;
				} else {
					printf("2.RTC_AL_SEC = 0x%x\n", RTC_Read(RTC_AL_SEC));
					printf("2.PONSTS = 0x%x\n\n", rtc_config_interface_read(MT6685_PONSTS));
					g_rtc_2sec_stat = false;
				}
				break;
			case 3:
				printf("3.RTC_AL_SEC = 0x%x\n", RTC_Read(RTC_AL_SEC));
				printf("3.PONSTS = 0x%x\n\n", rtc_config_interface_read(MT6685_PONSTS));
				g_rtc_2sec_stat = true;
			default:
				break;
		}
	} else {
		RTC_WARN("rtc 2sec reboot is not enabled\n");
	}

	if (g_rtc_2sec_stat == true)
		fg_set_2sec_reboot();
}

bool rtc_2sec_reboot_check(void)
{
#if (RTC_2SEC_REBOOT_ENABLE)
	RTC_INFO("(O)RTC_2SEC_REBOOT_ENABLE\n");
	return g_rtc_2sec_stat;
#else
	RTC_INFO("(X)RTC_2SEC_REBOOT_ENABLE\n");
	return false;
#endif
}

void rtc_enable_2sec_reboot(void)
{
	U16 reboot;

	RTC_Write(RTC_SPAR_MACRO, RTC_Read(RTC_SPAR_MACRO) & ~ (SPAR_THRE_SEL));
	RTC_Write(RTC_SPAR_MACRO, RTC_Read(RTC_SPAR_MACRO) | (SPAR_UVLO_WAIT_4ms << SPAR_UVLO_WAIT_SHIFT));
	Write_trigger();

	reboot = RTC_Read(RTC_AL_SEC) | RTC_BBPU_2SEC_EN | RTC_BBPU_AUTO_PDN_SEL;
	reboot = (reboot & ~(RTC_BBPU_2SEC_MODE_MSK)) | (RTC_2SEC_MODE << RTC_BBPU_2SEC_MODE_SHIFT);
	RTC_Write(RTC_AL_SEC, reboot);
	Write_trigger();
}

void rtc_set_xomode(bool bIs32kLess)
{
}

static bool rtc_lpd_enable_check(void)
{
	RTC_Write(RTC_BBPU, RTC_Read(RTC_BBPU) | RTC_BBPU_KEY | RTC_BBPU_RELOAD);
	Write_trigger();

	return ((RTC_Read(RTC_AL_SEC) & RTC_LPD_OPT_F32K_CK_ALIVE) == RTC_LPD_OPT_F32K_CK_ALIVE) ? false : true;
}

static void rtc_get_tick(struct rtc_time *tm)
{
	RTC_Write(RTC_BBPU, RTC_Read(RTC_BBPU) | RTC_BBPU_KEY | RTC_BBPU_RELOAD);
	Write_trigger();

	tm->tm_sec = RTC_Read(RTC_TC_SEC);
	tm->tm_min = RTC_Read(RTC_TC_MIN);
	tm->tm_hour = RTC_Read(RTC_TC_HOU);
	tm->tm_mday = RTC_Read(RTC_TC_DOM);
	tm->tm_mon = RTC_Read(RTC_TC_MTH) & RTC_TC_MTH_MASK;
	tm->tm_year = RTC_Read(RTC_TC_YEA);
}
void rtc_get_time(struct rtc_time *tm) {
	rtc_get_tick(tm);
	if (RTC_Read(RTC_TC_SEC) < tm->tm_sec) {	/* SEC has carried */
		rtc_get_tick(tm);
	}
	tm->tm_year += RTC_MIN_YEAR;
}

#if CFG_MICROTRUST_TEE_SUPPORT
unsigned long rtcgettime_teei(void)
{
	U16 now_sec, now_min, now_hou, now_dom, now_mth, now_yea;
	unsigned long now_time;

	now_sec = RTC_Read(RTC_TC_SEC);
	now_min = RTC_Read(RTC_TC_MIN);
	now_hou = RTC_Read(RTC_TC_HOU);
	now_dom = RTC_Read(RTC_TC_DOM);
	now_mth = RTC_Read(RTC_TC_MTH) & RTC_TC_MTH_MASK;
	now_yea = RTC_Read(RTC_TC_YEA) + RTC_MIN_YEAR;
	now_time = rtc_mktime(now_yea, now_mth, now_dom, now_hou, now_min, now_sec);

	return now_time;
}
#endif

#if CFG_GZ_SUPPORT
void Secure_Write_trigger(void)
{
	RTC_Write(RTC_WRTGR_SEC, 1);
}

void secure_rtc_set_ck(void)
{
	uint32_t val = 0;

	RTC_INFO("%s \n", __func__);

	if(secure_rtc_flag == true)
		return;

	val = (RG_RTC_SEC_MCLK_PDN_MASK | (RG_RTC_SEC_32K_CK_PDN_MASK << RG_RTC_SEC_32K_CK_PDN_SHIFT));
	rtc_config_interface(SCK_TOP_CKPDN_CON0_L_CLR, val, val, SCK_TOP_CKPDN_CON0_L_CLR_SHIFT);

	udelay(50);
	RTC_Write(RTC_SEC_CK_PDN, 1);
	Secure_Write_trigger();

	val = rtc_config_interface_read(MT6685_SCK_TOP_CKPDN_CON0_L);
	RTC_INFO("RTC_SEC_DSN_ID[0x%x]=0x%x, RTC_SEC_DSN_REV0[0x%x]=0x%x\n", RTC_SEC_DSN_ID, RTC_Read(RTC_SEC_DSN_ID), RTC_SEC_DSN_REV0, RTC_Read(RTC_SEC_DSN_REV0));
	RTC_INFO("SCK_TOP_CKPDN_CON0[0x%x]=0x%x, RTC_SEC_CK_PDN[0x%x]=0x%x\n", SCK_TOP_CKPDN_CON0_L, val, RTC_SEC_CK_PDN, RTC_Read(RTC_SEC_CK_PDN));
}

void secure_rtc_init(void)
{
	uint32_t val = 0;
	RTC_INFO("%s \n", __func__);
	secure_rtc_set_ck();
	udelay(100);
	//RTC_INFO("%s before= %d/%d/%d %d:%d:%d\n", __func__, RTC_Read(RTC_TC_YEA_SEC), RTC_Read(RTC_TC_MTH_SEC), RTC_Read(RTC_TC_DOM_SEC), RTC_Read(RTC_TC_HOU_SEC), RTC_Read(RTC_TC_MIN_SEC), RTC_Read(RTC_TC_SEC_SEC));
	RTC_Write(RTC_TC_YEA_SEC, (RTC_DEFAULT_YEA - RTC_MIN_YEAR));
	RTC_Write(RTC_TC_MTH_SEC, RTC_DEFAULT_MTH);
	RTC_Write(RTC_TC_DOM_SEC, RTC_DEFAULT_DOM);
	RTC_Write(RTC_TC_DOW_SEC, 1);
	RTC_Write(RTC_TC_HOU_SEC, 0);
	RTC_Write(RTC_TC_MIN_SEC, 0);
	RTC_Write(RTC_TC_SEC_SEC, 0);
	Secure_Write_trigger();

	//RTC_INFO("%s after= %d/%d/%d %d:%d:%d\n", __func__, RTC_Read(RTC_TC_YEA_SEC), RTC_Read(RTC_TC_MTH_SEC), RTC_Read(RTC_TC_DOM_SEC), RTC_Read(RTC_TC_HOU_SEC), RTC_Read(RTC_TC_MIN_SEC), RTC_Read(RTC_TC_SEC_SEC));

	secure_rtc_flag = true;
}

#endif
