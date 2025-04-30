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

 /* drivers/hwmon/mt6516/amit/LS9801I_T.c - LS9801I_T ALS/PS driver
 *
 * Author: MingHsien Hsieh <minghsien.hsieh@mediatek.com>
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
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_irq.h>
#include <linux/gpio.h>
#include <linux/of_gpio.h>
#include <linux/interrupt.h>
#include <linux/i2c.h>
#include <linux/slab.h>
#include <linux/irq.h>
#include <linux/miscdevice.h>
#include <asm/uaccess.h>
#include <linux/delay.h>
#include <linux/input.h>
#include <linux/workqueue.h>
#include <linux/kobject.h>
#include <linux/platform_device.h>
#include <asm/atomic.h>
#include <linux/version.h>
#include <linux/fs.h>
#include <asm/io.h>
#include <linux/module.h>
#include "cust_alsps.h"
#include "alsps.h"
#include "ls981x.h"

#define LS9801I_T_TUNE0
#define CALI_EVERY_TIME
#define CTTRACKING
#define LS_PS_FACTORY_TEST

static int alsps_init_status = 0;
extern struct platform_device *get_alsps_platformdev(void);

/******************************************************************************
 * configuration
*******************************************************************************/

/*----------------------------------------------------------------------------*/
#define LS9801I_T_DEV_NAME       "ls9801i-T"
/*----------------------------------------------------------------------------*/
#define APS_TAG                  "[LS9801i_T_ALS/PS] "
#define APS_FUN(f)               printk(APS_TAG"%s\n", __func__)
#define APS_ERR(fmt, args...)    printk(APS_TAG"%s %d : "fmt, __func__, __LINE__, ##args)
// #define APS_LOG(fmt, args...)    printk(APS_TAG fmt, ##args)
#define APS_LOG(fmt, args...)
// #define APS_DBG(fmt, args...)    printk(APS_TAG fmt, ##args)
#define APS_DBG(fmt, args...)
#define DRIVER_ATTR(_name, _mode, _show, _store) \
	struct driver_attribute driver_attr_##_name = \
		__ATTR(_name, _mode, _show, _store)      

/******************************************************************************
 * extern functions
*******************************************************************************/
#ifdef LS9801I_T_TUNE0
#define LS9801I_T_MAX_MIN_DIFF          100 
#define LS9801I_T_LT_N_CT               200 
#define LS9801I_T_HT_N_CT               250 
#endif 

#define LS9801I_T_H_PS                  45000
#define LS9801I_T_H_HT                  800
#define LS9801I_T_H_LT                  600

/*----------------------------------------------------------------------------*/
static struct i2c_client *ls9801i_T_i2c_client = NULL;
/*----------------------------------------------------------------------------*/
static const struct i2c_device_id ls9801i_T_i2c_id[] = { {LS9801I_T_DEV_NAME, 0}, {} };

/*----------------------------------------------------------------------------*/
static int ls9801i_T_i2c_probe(struct i2c_client *client, const struct i2c_device_id *id);
static int ls9801i_T_i2c_remove(struct i2c_client *client);
static int ls9801i_T_i2c_detect(struct i2c_client *client, struct i2c_board_info *info);
/*----------------------------------------------------------------------------*/
static int ls9801i_T_i2c_suspend(struct device *dev);
static int ls9801i_T_i2c_resume(struct device *dev);

#ifndef C_I2C_FIFO_SIZE
#define C_I2C_FIFO_SIZE     10
#endif
static DEFINE_MUTEX(ls9801i_T_i2c_mutex);
static int	ls9801i_T_init_flag = -1;	// 0<==>OK -1 <==> fail

#ifdef LS9801I_T_TUNE0
static int	ls9801i_T_calibration_flag = 0;
#endif

static int  ls9801i_T_local_init(void);
static int  ls9801i_T_local_uninit(void);

static struct alsps_init_info ls9801i_T_init_info =
{
	.name = "ls9801i-T",
	.init = ls9801i_T_local_init,
	.uninit = ls9801i_T_local_uninit,
};

static struct alsps_hw alsps_cust;
static struct alsps_hw *hw = &alsps_cust;

/*----------------------------------------------------------------------------*/
typedef enum
{
	LS9801I_T_TRC_ALS_DATA =0x0001,
	LS9801I_T_TRC_PS_DATA = 0x0002,
	LS9801I_T_TRC_EINT =    0x0004,
	LS9801I_T_TRC_IOCTL =   0x0008,
	LS9801I_T_TRC_I2C =     0x0010,
	LS9801I_T_TRC_CVT_ALS = 0x0020,
	LS9801I_T_TRC_CVT_PS =  0x0040,
	LS9801I_T_TRC_DEBUG =   0x8000,
} LS9801I_T_TRC;
/*----------------------------------------------------------------------------*/
typedef enum
{
	LS9801I_T_BIT_ALS = 1,
	LS9801I_T_BIT_PS = 2,
} LS9801I_T_BIT;
/*----------------------------------------------------------------------------*/
struct ls9801i_T_i2c_addr
{
	/*define a series of i2c slave address*/
	u8  enable;      	
	u8  wait_en;     	
	u8  int1en;    	
	u8  int2en;   		
	u8  await;    	
	u8  aitl;
    u8  aith;
	u8  thdh1_ps;   	/* PS INT threshold high 1 */
	u8	thdh2_ps;		/* PS INT threshold high 2 */
	u8  thdl1_ps;   	/* PS INT threshold low 1 */
	u8  thdl2_ps;   	/* PS INT threshold low 2 */
	u8  lagain;		
	u8  ldgain;	
	u8  pwait;	
	u8  pit;	
	u8  pctrl1;
	u8  pctrl3;
	u8  pgain;
	u8  pdrv;
    u8  ppulse;
	u8  als_l;		/* als data1 */
	u8  als_h;		/* als data2 */
	u8  ir_l;		/* ir data1 */
	u8  ir_h;		/* ir data2 */
	u8  ps_l;		/* ps data1 */
    u8  int_flag;
    u8  reset;


};
/*----------------------------------------------------------------------------*/
struct ls9801i_T_priv
{
	struct alsps_hw  *hw;
	struct i2c_client *client;
	struct delayed_work  eint_work;

	/*i2c address group*/
	struct ls9801i_T_i2c_addr  addr;

	/*misc*/
	atomic_t    trace;
	atomic_t    i2c_retry;
	atomic_t    als_suspend;
	atomic_t    als_debounce;   /*debounce time after enabling als*/
	atomic_t    als_deb_on;     /*indicates if the debounce is on*/
	atomic_t    als_deb_end;    /*the jiffies representing the end of debounce*/
	atomic_t    ps_mask;        /*mask ps: always return far away*/
	atomic_t    ps_debounce;    /*debounce time after enabling ps*/
	atomic_t    ps_deb_on;      /*indicates if the debounce is on*/
	atomic_t    ps_deb_end;     /*the jiffies representing the end of debounce*/
	atomic_t    ps_suspend;
	atomic_t    init_done;
	struct      device_node *irq_node;
	int         irq;

	/*data*/
	u16         als;
	u16         ps;
    u8          pssar;
	u16         ir;
    bool        ps_enable;
    bool		als_enable;

	u16         als_level_num;
	u16         als_value_num;
	u32         als_level[C_CUST_ALS_LEVEL - 1];
	u32         als_value[C_CUST_ALS_LEVEL];

	atomic_t	state_val;
	atomic_t    ps_high_thd_val;     /*the cmd value can't be read, stored in ram*/
	atomic_t    ps_low_thd_val;     /*the cmd value can't be read, stored in ram*/
	ulong       enable;         /*enable mask*/
	ulong       pending_intr;   /*pending interrupt*/
	atomic_t	recv_reg;
	/*early suspend*/
#if defined(CONFIG_HAS_EARLYSUSPEND)
	struct early_suspend    early_drv;
#endif
	bool first_boot;
	struct hrtimer ps_tune0_timer;
	struct workqueue_struct *ls9801i_T_ps_tune0_wq;
	struct work_struct ls9801i_T_ps_tune0_work;
#ifdef LS9801I_T_TUNE0
	uint16_t psa;
	uint16_t psi;
	uint16_t psi_set;
	uint16_t ls9801i_T_max_min_diff;
	uint16_t ls9801i_T_lt_n_ct;
	uint16_t ls9801i_T_ht_n_ct;
	uint16_t crosstalk;
    uint16_t    last_ps_psi;
#ifdef CALI_EVERY_TIME
	uint16_t ps_high_thd_boot;
	uint16_t ps_low_thd_boot;
#endif
	uint16_t boot_cali;
#ifdef CTTRACKING
	bool ps_thd_update;
#endif
	ktime_t ps_tune0_delay;
	bool tune_zero_init_proc;
	uint32_t ps_stat_data[3];
	int data_count;
#endif
	u16 als_last;
	u16 ps_cali;
	uint8_t pid;
	uint8_t ps_distance_last;
	bool als_auto_gain;
	u16 als_again;
	u16 pre_again;
    uint32_t    als_count;
	uint16_t 	als_cali;
	uint32_t als_debug_count;
    uint8_t ps_temp;
};
/*----------------------------------------------------------------------------*/
#ifdef CONFIG_OF
static const struct of_device_id alsps_of_match[] =
{
	{.compatible = "mediatek,alsps"},
	{},
};
#endif

#ifdef CONFIG_PM_SLEEP
static const struct dev_pm_ops ls9801i_T_pm_ops = {
	SET_SYSTEM_SLEEP_PM_OPS(ls9801i_T_i2c_suspend, ls9801i_T_i2c_resume)
};
#endif

static struct i2c_driver ls9801i_T_i2c_driver =
{
	.probe = ls9801i_T_i2c_probe,
	.remove = ls9801i_T_i2c_remove,
	.detect = ls9801i_T_i2c_detect,
	//.suspend = ls9801i_T_i2c_suspend,
	//.resume = ls9801i_T_i2c_resume,
	.id_table = ls9801i_T_i2c_id,
	.driver = {
		.name = LS9801I_T_DEV_NAME,
#ifdef CONFIG_PM_SLEEP
		.pm   = &ls9801i_T_pm_ops,
#endif
#ifdef CONFIG_OF
		.of_match_table = alsps_of_match,
#endif
	},
};

static struct ls9801i_T_priv *ls9801i_T_obj = NULL;
static int ls9801i_T_get_ps_value(struct ls9801i_T_priv *obj, u16 ps);
static int ls9801i_T_get_ps_value_only(struct ls9801i_T_priv *obj, u16 ps);
static int ls9801i_T_get_als_value(struct ls9801i_T_priv *obj, u16 als);
static int ls9801i_T_read_als(struct i2c_client *client, u16 *data);
static int ls9801i_T_read_ps(struct i2c_client *client, u16 *data);

#ifdef LS9801I_T_TUNE0
static int ls9801i_T_ps_tune_zero_func_fae(struct ls9801i_T_priv *obj);
#endif
static int ls9801i_T_init_client(struct i2c_client *client);
//struct wake_lock mps_lock;
static DEFINE_MUTEX(run_cali_mutex);
#ifdef LS_PS_FACTORY_TEST
static uint8_t g_ps_last_status = 1;
#endif
/*----------------------------------------------------------------------------*/
int ls9801i_T_get_addr(struct ls9801i_T_i2c_addr *addr)
{
	if (!addr)
	{
		return -EFAULT;
	}

	addr->enable =  LS9801I_T_ENABLE_REG;
    addr->reset =   LS9801I_T_RESET_REG;
    addr->wait_en = LS9801I_T_WCRTL_REG;
	addr->int1en =  LS9801I_T_PROX_INT1EN_REG;
	addr->int2en =  LS9801I_T_PROX_INT2EN_REG;
	addr->await =   LS9801I_T_LIGHT_AWT_REG;
	addr->aitl =    LS9801I_T_LIGHT_AITL_REG;
	addr->aith =    LS9801I_T_LIGHT_AITH_REG;
	addr->lagain =  LS9801I_T_LIGHT_AGAIN_REG;
	addr->ldgain =  LS9801I_T_LIGHT_DGAIN_REG;
	addr->pwait =   LS9801I_T_PROX_PWT_REG;
	addr->pit =     LS9801I_T_PROX_PIT_REG;
	addr->pctrl1 =  LS9801I_T_PROX_PCTRL1_REG;
	addr->pctrl3 =  LS9801I_T_PROX_PCTRL3_REG;
	addr->pgain =   LS9801I_T_PROX_PGAIN_REG;
	addr->pdrv =    LS9801I_T_PROX_PDRV_REG;
    addr->ppulse =  LS9801I_T_PROX_PPULSE_REG;
	addr->als_l =   LS9801I_T_LIGHT_RAW_REG_L;
	addr->als_h =   LS9801I_T_LIGHT_RAW_REG_H;
	addr->ir_l =    LS9801I_T_IR_RAW_REG_L;
	addr->ir_h =    LS9801I_T_IR_RAW_REG_H;
	addr->ps_l =    LS9801I_T_PROX_RAW_REG_L;
	addr->thdl2_ps =LS9801I_T_PROX_PILTH_REG;
	addr->thdl1_ps =LS9801I_T_PROX_PILTL_REG;
	addr->thdh2_ps =LS9801I_T_PROX_PIHTH_REG;
	addr->thdh1_ps =LS9801I_T_PROX_PIHTL_REG;
    addr->int_flag =LS9801I_T_INT_STATUS_REG;

	return 0;
}
/*----------------------------------------------------------------------------*/
int ls9801i_T_hwmsen_read_block(struct i2c_client *client, u8 addr, u8 *data, u8 len)
{
	int err;
	u8 beg = addr;
	struct i2c_msg msgs[2] =
	{
		{
			.addr = client->addr,
			.flags = 0,
			.len = 1,
			.buf = &beg
		},
		{
			.addr = client->addr,
			.flags = I2C_M_RD,
			.len = len,
			.buf = data,
		}
	};
	mutex_lock(&ls9801i_T_i2c_mutex);

	if (!client)
	{
		mutex_unlock(&ls9801i_T_i2c_mutex);
		return -EINVAL;
	}
	else if (len > C_I2C_FIFO_SIZE)
	{
		mutex_unlock(&ls9801i_T_i2c_mutex);
		APS_LOG(" length %d exceeds %d\n", len, C_I2C_FIFO_SIZE);
		return -EINVAL;
	}

	err = i2c_transfer(client->adapter, msgs, sizeof(msgs) / sizeof(msgs[0]));
	mutex_unlock(&ls9801i_T_i2c_mutex);

	if (err != 2)
	{
		APS_LOG("i2c_transfer error: (%d %p %d) %d\n", addr, data, len, err);
		err = -EIO;
	}
	else
	{
		err = 0;/*no error*/
	}

	return err;
}

/*----------------------------------------------------------------------------*/
int ls9801i_T_master_recv(struct i2c_client *client, u16 addr, u8 *buf, int count)
{

	struct ls9801i_T_priv *obj = i2c_get_clientdata(client);
	int ret = 0, retry = 0;
	int trc = atomic_read(&obj->trace);
	int max_try = atomic_read(&obj->i2c_retry);

	while (retry++ < max_try)
	{
		ret = ls9801i_T_hwmsen_read_block(client, addr, buf, count);
		if (ret == 0)
			break;
		udelay(100);
	}

	if (unlikely(trc))
	{
		if ((retry != 1) && (trc & LS9801I_T_TRC_DEBUG))
		{
			APS_LOG("(recv) %d/%d\n", retry - 1, max_try);
		}
	}

	/* If everything went ok (i.e. 1 msg transmitted), return #bytes
	transmitted, else error code. */
	return (ret == 0) ? count : ret;

}
/*----------------------------------------------------------------------------*/
int ls9801i_T_master_send(struct i2c_client *client, u16 addr, u8 *buf, int count)
{
	int ret = 0, retry = 0;
	struct ls9801i_T_priv *obj = i2c_get_clientdata(client);
	int trc = atomic_read(&obj->trace);
	int max_try = atomic_read(&obj->i2c_retry);

	while (retry++ < max_try)
	{
		ret = hwmsen_write_block(client, addr, buf, count);
		if (ret == 0)
			break;
		udelay(100);
	}

	if (unlikely(trc))
	{
		if ((retry != 1) && (trc & LS9801I_T_TRC_DEBUG))
		{
			APS_LOG("(send) %d/%d\n", retry - 1, max_try);
		}
	}
	return (ret == 0) ? count : ret;

}
/*----------------------------------------------------------------------------*/
int ls9801i_T_read_als(struct i2c_client *client, u16 *data)
{
	struct ls9801i_T_priv *obj = i2c_get_clientdata(client);
	int ret = 0;
	u8 buf[2];//, value;
	u32 als_data;
	
	if (NULL == client)
	{
		return -EINVAL;
	}
	ret = ls9801i_T_master_recv(client, obj->addr.ir_l, buf, 0x2);
	if (ret < 0)
	{
		APS_DBG("error: %d\n", ret);
		return -EFAULT;
	}
	obj->ir = (buf[1] << 8) | (buf[0]);
	ret = ls9801i_T_master_recv(client, obj->addr.als_l, buf, 0x2);
	if (ret < 0)
	{
		APS_DBG("error: %d\n", ret);
		return -EFAULT;
	}
	als_data = (buf[1] << 8) | (buf[0]);
	*data = (u16)als_data;
    APS_DBG("ls9801i_T_read_als: %d,ir:%d\n", *data,obj->ir);
	if (atomic_read(&obj->trace) & LS9801I_T_TRC_ALS_DATA)
	{
		APS_DBG("ALS: 0x%04X\n", (u32)(*data));
	}

	return 0;
}
/*----------------------------------------------------------------------------*/
int ls9801i_T_write_als_again(struct i2c_client *client, u8 data)
{
	struct ls9801i_T_priv *obj = i2c_get_clientdata(client);
	int ret = 0;
	ret = ls9801i_T_master_send(client, obj->addr.lagain, &data, 1);
	if (ret < 0)
	{
		APS_ERR("write als = %d\n", ret);
		return -EFAULT;
	}
	return 0;
}
int ls9801i_T_write_als_dgain(struct i2c_client *client, u8 data)
{
	struct ls9801i_T_priv *obj = i2c_get_clientdata(client);
	int ret = 0;
	ret = ls9801i_T_master_send(client, obj->addr.ldgain, &data, 1);
	if (ret < 0)
	{
		APS_ERR("write als = %d\n", ret);
		return -EFAULT;
	}
	return 0;
}

/*----------------------------------------------------------------------------*/
int ls9801i_T_int_en(struct i2c_client *client, u8 *data)
{
	int ret = 0;
	u8 buf[1];

	ret = ls9801i_T_master_recv(client, LS9801I_T_INT_STATUS_REG, buf, 0x01);
	if (ret < 0)
	{
		APS_DBG("error: %d\n", ret);
		return -EFAULT;
	}
    *data = buf[0];

	return 0;
}
/*----------------------------------------------------------------------------*/
int ls9801i_T_read_id(struct i2c_client *client)
{
	struct ls9801i_T_priv *obj = i2c_get_clientdata(client);
	int ret = 0;
	u8 buf[2];

	if (NULL == client)
	{
		return -EINVAL;
	}

	ret = ls9801i_T_master_recv(client, LS9801I_T_CHECK_ID_REG, buf, 0x01);
	if (ret < 0)
	{
		APS_DBG("error: %d\n", ret);
		return -EFAULT;
	}

	obj->pid = buf[0];
	APS_LOG("%s: PID=0x%x\n", __func__, obj->pid);

	return 0;
}
/*----------------------------------------------------------------------------*/
int ls9801i_T_read_ps(struct i2c_client *client, u16 *data)
{
	struct ls9801i_T_priv *obj = i2c_get_clientdata(client);
	int ret = 0;
	u8 buf[2];

	if (NULL == client)
	{
		APS_ERR("i2c client is NULL\n");
		return -EINVAL;
	}

	ret = ls9801i_T_master_recv(client, obj->addr.ps_l, buf, 0x02);
	if (ret < 0)
	{
		APS_DBG("error: %d\n", ret);
		return -EFAULT;
	}
	else
	{
		*data = (buf[1] << 8) | (buf[0]);
	}
	APS_DBG("ls9801i_T_read_ps: %d\n", (u32)(*data));

	if (atomic_read(&obj->trace) & LS9801I_T_TRC_PS_DATA)
	{
		APS_DBG("PS: 0x%04X\n", (u32)(*data));
	}

	return 0;
}
/*----------------------------------------------------------------------------*/
int ls9801i_T_pwait(struct i2c_client *client, u8 data)
{
	struct ls9801i_T_priv *obj = i2c_get_clientdata(client);
	int ret = 0;
	ret = ls9801i_T_master_send(client, obj->addr.pwait, &data, 1);

	if (ret < 0)
	{
		APS_ERR("write ps = %d\n", ret);
		return -EFAULT;
	}

	return 0;
}
/*----------------------------------------------------------------------------*/
int ls9801i_T_pit(struct i2c_client *client, u8 data)
{
	struct ls9801i_T_priv *obj = i2c_get_clientdata(client);
	int ret = 0;
	ret = ls9801i_T_master_send(client, obj->addr.pit, &data, 1);
	if (ret < 0)
	{
		APS_ERR("write ps = %d\n", ret);
		return -EFAULT;
	}

	return 0;
}
/*----------------------------------------------------------------------------*/
int ls9801i_T_pctrl(struct i2c_client *client)
{
	struct ls9801i_T_priv *obj = i2c_get_clientdata(client);
	int ret = 0;
    u8 data;
    data = LS9801I_T_PROX_PCTRL1_VALUE;
	ret = ls9801i_T_master_send(client, obj->addr.pctrl1, &data, 1);

	if (ret < 0)
	{
		APS_ERR("write ps = %d\n", ret);
		return -EFAULT;
	}
    data = LS9801I_T_PROX_PCTRL3_VALUE;
	ret = ls9801i_T_master_send(client, obj->addr.pctrl3, &data, 1);

	if (ret < 0)
	{
		APS_ERR("write ps = %d\n", ret);
		return -EFAULT;
	}

	return 0;
}

/*----------------------------------------------------------------------------*/
int ls9801i_T_pgain(struct i2c_client *client, u8 data)
{
	struct ls9801i_T_priv *obj = i2c_get_clientdata(client);
	int ret = 0;
	ret = ls9801i_T_master_send(client, obj->addr.pgain, &data, 1);
	if (ret < 0)
	{
		APS_ERR("write ps = %d\n", ret);
		return -EFAULT;
	}

	return 0;
}
/*----------------------------------------------------------------------------*/
int ls9801i_T_pdrv(struct i2c_client *client, u8 data)
{
	struct ls9801i_T_priv *obj = i2c_get_clientdata(client);
	int ret = 0;
	ret = ls9801i_T_master_send(client, obj->addr.pdrv, &data, 1);
	if (ret < 0)
	{
		APS_ERR("write ps = %d\n", ret);
		return -EFAULT;
	}

	return 0;
}
int ls9801i_T_ppulse(struct i2c_client *client, u8 data)
{
	struct ls9801i_T_priv *obj = i2c_get_clientdata(client);
	int ret = 0;
	ret = ls9801i_T_master_send(client, obj->addr.ppulse, &data, 1);
	if (ret < 0)
	{
		APS_ERR("write ls9801i_T_ppulse = %d\n", ret);
		return -EFAULT;
	}

	return 0;
}

/*----------------------------------------------------------------------------*/
int ls9801i_T_enable(struct i2c_client *client, u8 data)
{
	struct ls9801i_T_priv *obj = i2c_get_clientdata(client);
	int ret = 0;
    u8 state = 0x00;
	APS_ERR("ls9801i_T_enable = %d\n", data);
    
    	ret = ls9801i_T_master_send(client, obj->addr.enable, &state, 1);
	if (ret < 0)
	{
		APS_ERR("write state err = %d\n", ret);
		return -EFAULT;
	}
	ret = ls9801i_T_master_send(client, obj->addr.enable, &data, 1);
	if (ret < 0)
	{
		APS_ERR("write state err = %d\n", ret);
		return -EFAULT;
	}

	return 0;
}

/*----------------------------------------------------------------------------*/
int ls9801i_T_write_sw_reset(struct i2c_client *client)
{
	struct ls9801i_T_priv *obj = i2c_get_clientdata(client);
	u8 buf = 0;
	int ret = 0;
	APS_LOG("%s: In\n", __FUNCTION__);
	buf = 0x80;
	ret = ls9801i_T_master_send(client, obj->addr.reset, (char*)&buf, sizeof(buf));

	if (ret < 0)
	{
		APS_ERR("i2c write test error = %d\n", ret);
		return -EFAULT;
	}

	msleep(50);
	return 0;
}
/*----------------------------------------------------------------------------*/
int ls9801i_T_write_ps_high_thd(struct i2c_client *client, u16 thd)
{
	struct ls9801i_T_priv *obj = i2c_get_clientdata(client);
	u8 buf[2];
	int ret = 0;
    buf[0] = (u8)(0x00FF & thd);
	buf[1] = (u8)((0xFF00 & thd) >> 8);

	ret = ls9801i_T_master_send(client, obj->addr.thdh1_ps, &buf[0], 1);
	if (ret < 0)
	{
		APS_ERR("WARNING: %d\n", ret);
		return -EFAULT;
	}

	ret = ls9801i_T_master_send(client, obj->addr.thdh2_ps, &buf[1], 1);
	if (ret < 0)
	{
		APS_ERR("WARNING: %d\n", ret);
		return -EFAULT;
	}

	return 0;
}
/*----------------------------------------------------------------------------*/
int ls9801i_T_write_ps_low_thd(struct i2c_client *client, u16 thd)
{
	struct ls9801i_T_priv *obj = i2c_get_clientdata(client);
	u8 buf[2];
	int ret = 0;
    buf[0] = (u8)(0x00FF & thd);
	buf[1] = (u8)((0xFF00 & thd) >> 8);

	ret = ls9801i_T_master_send(client, obj->addr.thdl1_ps, &buf[0], 1);
	if (ret < 0)
	{
		APS_ERR("WARNING: %s: %d\n", __func__, ret);
		return -EFAULT;
	}

	ret = ls9801i_T_master_send(client, obj->addr.thdl2_ps, &buf[1], 1);
	if (ret < 0)
	{
		APS_ERR("WARNING: %s: %d\n", __func__, ret);
		return -EFAULT;
	}

	return 0;
}

static int show_allreg(void)
{
	int ret = 0;
	u8 rbuf[0x22];
	int cnt;
	memset(rbuf, 0, sizeof(rbuf));

	if (!ls9801i_T_obj)
	{
		APS_ERR("ls9801i_T_obj is null!!\n");
		return 0;
	}

	ret = ls9801i_T_master_recv(ls9801i_T_obj->client, 0, &rbuf[0], 32);
	if (ret < 0)
	{
		APS_DBG("error: %d\n", ret);
		return -EFAULT;
	}

	ret = ls9801i_T_master_recv(ls9801i_T_obj->client, LS9801I_T_CHECK_ID_REG, &rbuf[32], 1);
	if (ret < 0)
	{
		APS_DBG("error: %d\n", ret);
		return -EFAULT;
	}

	for (cnt = 0; cnt < 0x20; cnt++)
	{
		APS_LOG("reg[0x%x]=0x%x\n", cnt, rbuf[cnt]);
	}

    APS_LOG("reg[0x72] ID=0x%x\n",  rbuf[cnt]);
	return 0;
}

/*----------------------------------------------------------------------------*/
static void ls9801i_T_ps_report(struct ls9801i_T_priv *obj, int nf)
{
	/*
		hwm_sensor_data sensor_data;

		sensor_data.values[0] = nf;
		sensor_data.value_divide = 1;
		sensor_data.status = SENSOR_STATUS_ACCURACY_MEDIUM;
		if((nf = hwmsen_get_interrupt_data(ID_PROXIMITY, &sensor_data)))
		APS_ERR("call hwmsen_get_interrupt_data fail = %d\n", nf);
	*/
	if (ps_report_interrupt_data(nf))
		APS_ERR("call ps_report_interrupt_data fail\n");

	APS_LOG("%s:ps raw 0x%x -> value 0x%x \n", __FUNCTION__, obj->ps, nf);
	obj->ps_distance_last = nf;
}
/*----------------------------------------------------------------------------*/
static int ls9801i_T_enable_als(struct i2c_client *client, int enable)
{
	struct ls9801i_T_priv *obj = i2c_get_clientdata(client);
	int err, cur = 0;
	int trc = atomic_read(&obj->trace);
	APS_LOG("%s: enable=%d\n", __func__, enable);

	obj->als_debug_count = 0;
	obj->als_count = 0;

	if (enable)
	{
	    if(obj->ps_enable){
		    cur = LS9801I_T_LIGHT_ENABLE_VALUE | LS9801I_T_PROX_ENABLE_VALUE;
        }else{
            cur = LS9801I_T_LIGHT_ENABLE_VALUE;
        }
        obj->als_enable = true;
	}
	else
	{
		if(obj->ps_enable)
        {
            cur = LS9801I_T_PROX_ENABLE_VALUE;
        }
        obj->als_enable = false;
	}

	if (trc & LS9801I_T_TRC_DEBUG)
	{
		APS_LOG("%s: %08X, %d\n", __func__, cur, enable);
	}

	err = ls9801i_T_enable(client, cur);
	if (err < 0)
		return err;
	if (enable)
	{
		obj->als_last = 0;

		if (obj->hw->polling_mode_als)
		{
			atomic_set(&obj->als_deb_on, 1);
			atomic_set(&obj->als_deb_end, jiffies + atomic_read(&obj->als_debounce)*HZ / 1000);
		}
		else
		{
			//set_bit(LS9801I_T_BIT_ALS,  &obj->pending_intr);
			schedule_delayed_work(&obj->eint_work, 200 * HZ / 1000);
		}
	}

	if (trc & LS9801I_T_TRC_DEBUG)
	{
		APS_LOG("enable als (%d)\n", enable);
	}

#if 1
	APS_LOG("%s:show all reg\n", __FUNCTION__);
	show_allreg(); //for debug
#endif
	return err;
}
/*----------------------------------------------------------------------------*/
static int ls9801i_T_enable_ps_set_thd(struct ls9801i_T_priv *obj)
{
	int err;
	if ((err = ls9801i_T_write_ps_high_thd(obj->client, atomic_read(&obj->ps_high_thd_val))))
	{
		APS_ERR("write high thd error: %d\n", err);
		return err;
	}

	if ((err = ls9801i_T_write_ps_low_thd(obj->client, atomic_read(&obj->ps_low_thd_val))))
	{
		APS_ERR("write low thd error: %d\n", err);
		return err;
	}

	APS_LOG("%s:in driver, HT=%d, LT=%d\n", __func__, atomic_read(&obj->ps_high_thd_val), atomic_read(&obj->ps_low_thd_val));
	return 0;
}
/*----------------------------------------------------------------------------*/
static int ls9801i_T_enable_ps(struct i2c_client *client, int enable)
{
	struct ls9801i_T_priv *obj = i2c_get_clientdata(client);
	int err, cur = 0;
	int trc = atomic_read(&obj->trace);
    

	uint16_t    ps_threshold_high = 30000;
    uint16_t    ps_threshold_low =  29500;
#if 0
    if (obj->last_ps_psi != 0xFFFF) {
        obj->psi = obj->last_ps_psi;
        obj->psi_set = obj->psi;
    }
    APS_LOG("frist last_ps_psi=%d, obj->psi=%d, ht_n_ct=%d\n", obj->last_ps_psi, obj->psi, obj->ls9801i_T_ht_n_ct);
	ps_threshold_high = obj->psi + 2*obj->ls9801i_T_ht_n_ct;
	ps_threshold_low = obj->psi + obj->ls9801i_T_ht_n_ct + obj->ls9801i_T_lt_n_ct;
#endif
    atomic_set(&obj->ps_high_thd_val, ps_threshold_high);
    atomic_set(&obj->ps_low_thd_val, ps_threshold_low);

    APS_LOG("%s: before enable ps, HT=%d, LT=%d\n", __func__, atomic_read(&obj->ps_high_thd_val), atomic_read(&obj->ps_low_thd_val));

	if (enable)
	{
	    if(obj->als_enable){
		    cur = LS9801I_T_LIGHT_ENABLE_VALUE | LS9801I_T_PROX_ENABLE_VALUE;
        }else{
            cur = LS9801I_T_PROX_ENABLE_VALUE;
        }
        obj->ps_enable = true;
	}else
    {
	    if(obj->als_enable){
		    cur = LS9801I_T_LIGHT_ENABLE_VALUE;
        }
        obj->ps_enable = false;
    }   
    
	printk("%s: enable=%d ,cur=%d\n", __FUNCTION__, enable,cur);


    #ifdef LS9801I_T_TUNE0
	if (!(obj->psi_set) && !enable)
	{
		hrtimer_cancel(&obj->ps_tune0_timer);
		cancel_work_sync(&obj->ls9801i_T_ps_tune0_work);
	}
    #endif

	if (obj->first_boot == true)
	{
		obj->first_boot = false;
	}

	err = ls9801i_T_enable(client, cur);
	atomic_set(&obj->state_val, cur);
	if (enable)
	{
    	ls9801i_T_enable_ps_set_thd(obj);
        #ifdef LS9801I_T_TUNE0
    	obj->ps_thd_update = false;
    	obj->psi_set = 0;
    	obj->psa = 0;
    	obj->psi = 0xFFFF;
    	hrtimer_start(&obj->ps_tune0_timer, obj->ps_tune0_delay, HRTIMER_MODE_REL);
        #endif
    	APS_LOG("%s: polling mode =%d\n", __func__, obj->hw->polling_mode_ps);
    	if (obj->hw->polling_mode_ps)
    	{
    		atomic_set(&obj->ps_deb_on, 1);
    		atomic_set(&obj->ps_deb_end, jiffies + atomic_read(&obj->ps_debounce)*HZ / 1000);
    	}
    	else
    	{
    			msleep(50);
    			if ((err = ls9801i_T_read_ps(obj->client, &obj->ps)))
    			{
    				APS_ERR("ls9801i_T read ps data: %d\n", err);
    				return err;
    			}
    			err = ls9801i_T_get_ps_value_only(obj, obj->ps);
    			if (err < 0)
    			{
    				APS_ERR("ls9801i_T get ps value: %d\n", err);
    				return err;
    			}

    			ls9801i_T_ps_report(obj, err);
    			APS_LOG("%s: after ls9801i_T_ps_report, show all reg\n", __FUNCTION__);
    			show_allreg(); //for debug
    	
    	}
	}else{
    	obj->data_count  = 0;
        obj->psa = 0x00;
        if (obj->psi_set != 0xFFFF)
        {
            obj->last_ps_psi = obj->psi;
        }
        #ifdef CTTRACKING
		hrtimer_cancel(&obj->ps_tune0_timer);
        #endif
	}
	if (trc & LS9801I_T_TRC_DEBUG)
	{
		APS_LOG("enable ps  (%d)\n", enable);
	}

	return 0;
}
/*----------------------------------------------------------------------------*/
static int ls9801i_T_ps_calibration(struct i2c_client *client)
{
	struct ls9801i_T_priv *obj = i2c_get_clientdata(client);
	u8 reg;
	int err = 0;
	bool re_en_ps = false, re_en_als = false;
	int counter = 0;
	uint32_t ps_sum = 0;
	const int PS_CALI_COUNT = 5;

	if (obj->ps_enable)
	{
		APS_LOG("%s: force disable PS\n", __func__);
		ls9801i_T_enable_ps(obj->client, 0);
		re_en_ps = true;
	}

	if (obj->als_enable)
	{
		APS_LOG("%s: force disable ALS\n", __func__);
		ls9801i_T_enable_als(obj->client, 0);
		re_en_als = true;
	}

#if defined(CONFIG_OF)
	disable_irq(obj->irq);
#elif ((defined MT6573) || (defined MT6575) || (defined MT6577) || (defined MT6589) || (defined MT6572))
	mt65xx_eint_mask(CUST_EINT_ALS_NUM);
#else
	mt_eint_mask(CUST_EINT_ALS_NUM);
#endif
	reg = LS9801I_T_LIGHT_ENABLE_VALUE | LS9801I_T_PROX_ENABLE_VALUE;
	err = ls9801i_T_enable(client, reg);

	if (err < 0)
		goto err_out;

	while (counter < PS_CALI_COUNT)
	{
		msleep(60);

		if ((err = ls9801i_T_read_ps(obj->client, &obj->ps)))
			goto err_out;

		APS_LOG("%s: ps=%d\n", __func__, obj->ps);
		ps_sum += obj->ps;
		counter++;
	}

	obj->ps_cali = (ps_sum / PS_CALI_COUNT);
#ifdef LS9801I_T_TUNE0
	obj->crosstalk = (ps_sum / PS_CALI_COUNT);
	if (obj->crosstalk == 0)
	{
		obj->crosstalk = obj->hw->ps_threshold_low;
		ls9801i_T_calibration_flag = 0;
	}
	else
	{
		ls9801i_T_calibration_flag = 1;
	}

#endif
#ifdef CALI_EVERY_TIME
	obj->ps_high_thd_boot = obj->crosstalk + obj->ls9801i_T_ht_n_ct;
	obj->ps_low_thd_boot = obj->crosstalk + obj->ls9801i_T_lt_n_ct;
	printk("%s: crosstalk=%d, high thd=%d, low thd=%d\n", __func__, obj->crosstalk, obj->ps_high_thd_boot, obj->ps_low_thd_boot);
#endif
err_out:
#if defined(CONFIG_OF)
	disable_irq(obj->irq);
#elif ((defined MT6573) || (defined MT6575) || (defined MT6577) || (defined MT6589) || (defined MT6572))
	mt65xx_eint_mask(CUST_EINT_ALS_NUM);
#else
	mt_eint_mask(CUST_EINT_ALS_NUM);
#endif

	if (re_en_als)
	{
		APS_LOG("%s: re-enable ALS\n", __func__);
		ls9801i_T_enable_als(obj->client, 1);
	}

	if (re_en_ps)
	{
		APS_LOG("%s: re-enable PS\n", __func__);
		ls9801i_T_enable_ps(obj->client, 1);
	}

	return err;
}
/*----------------------------------------------------------------------------*/
static int ls9801i_T_check_intr(struct i2c_client *client, u8 *status)
{
	struct ls9801i_T_priv *obj = i2c_get_clientdata(client);
	int err;
    u8 buf;
    err = ls9801i_T_master_recv(client, obj->addr.int_flag, &buf, 0x1);
    if (err < 0)
    {
        APS_DBG("error: %d\n", err);
        return -EFAULT;
    }
    *status = buf;

	APS_LOG("%s: read status reg: 0x%x\n", __func__, buf);

	if (buf & (LS9801I_T_IFLAG_P_H|LS9801I_T_IFLAG_P_L))
	{
		set_bit(LS9801I_T_BIT_PS, &obj->pending_intr);
	}
	else
	{
		clear_bit(LS9801I_T_BIT_PS, &obj->pending_intr);
	}

	if (atomic_read(&obj->trace) & LS9801I_T_TRC_DEBUG)
	{
		APS_LOG("check intr: 0x%02X => 0x%08lX\n", *status, obj->pending_intr);
	}

	return 0;
}
/*----------------------------------------------------------------------------*/
#ifdef LS9801I_T_TUNE0
static int ls9801i_T_ps_tune_zero_final(struct ls9801i_T_priv *obj)
{
	int err;
	obj->tune_zero_init_proc = false;
    
	if (obj->data_count == -1)
	{
		APS_LOG("%s: exceed limit\n", __func__);
		hrtimer_cancel(&obj->ps_tune0_timer);
		return 0;
	}

	obj->psa = obj->ps_stat_data[0];
	obj->psi = obj->ps_stat_data[2];
#ifndef CALI_EVERY_TIME
	atomic_set(&obj->ps_high_thd_val, obj->ps_stat_data[1] + obj->ls9801i_T_ht_n_ct);
	atomic_set(&obj->ps_low_thd_val, obj->ps_stat_data[1] + obj->ls9801i_T_lt_n_ct);
#else
	obj->ps_high_thd_boot = obj->ps_stat_data[1] + obj->ls9801i_T_ht_n_ct;
	obj->ps_low_thd_boot = obj->ps_stat_data[1] + obj->ls9801i_T_lt_n_ct;
	atomic_set(&obj->ps_high_thd_val, obj->ps_high_thd_boot);
	atomic_set(&obj->ps_low_thd_val, obj->ps_low_thd_boot);
#endif
	if ((err = ls9801i_T_write_ps_high_thd(obj->client, atomic_read(&obj->ps_high_thd_val))))
	{
		APS_ERR("write high thd error: %d\n", err);
		return err;
	}
	if ((err = ls9801i_T_write_ps_low_thd(obj->client, atomic_read(&obj->ps_low_thd_val))))
	{
		APS_ERR("write low thd error: %d\n", err);
		return err;
	}

	obj->boot_cali = 1;
	APS_LOG("%s: set HT=%d,LT=%d\n", __func__, atomic_read(&obj->ps_high_thd_val), atomic_read(&obj->ps_low_thd_val));
	hrtimer_cancel(&obj->ps_tune0_timer);
	return 0;
}
/*----------------------------------------------------------------------------*/
static int32_t ls9801i_T_tune_zero_get_ps_data(struct ls9801i_T_priv *obj)
{
	int err;

	if ((err = ls9801i_T_read_ps(obj->client, &obj->ps)))
	{
		APS_ERR("ls9801i_T read ps data: %d\n", err);
		return err;
	}

	APS_LOG("%s: ps count  %d ,psdata =%d\n", __func__, obj->data_count, obj->ps);
	obj->ps_stat_data[1] += obj->ps;

	if (obj->ps > obj->ps_stat_data[0])
		obj->ps_stat_data[0] = obj->ps;

	if (obj->ps < obj->ps_stat_data[2])
		obj->ps_stat_data[2] = obj->ps;

	obj->data_count++;

	if (obj->data_count == 5)
	{
		obj->ps_stat_data[1] /= obj->data_count;
		ls9801i_T_ps_tune_zero_final(obj);
	}

	return 0;
}
/*----------------------------------------------------------------------------*/
static int ls9801i_T_ps_tune_zero_init(struct ls9801i_T_priv *obj)
{
    int ret;
    
	obj->psa = 0;
	obj->psi = 0xFFFF;
	obj->psi_set = 0;
	obj->tune_zero_init_proc = true;
	obj->ps_stat_data[0] = 0;
	obj->ps_stat_data[2] = 0xffff;
	obj->ps_stat_data[1] = 0;
	obj->data_count = 0;
	obj->boot_cali = 0;

    obj->last_ps_psi = 0xFFFF;
    ret = ls9801i_T_enable(obj->client, LS9801I_T_PROX_ENABLE_VALUE);
	if (ret < 0)
	{
		APS_ERR("ls9801i_T_ps_tune_zero_init enable err = %d\n", ret);
		return -EFAULT;
	}
	hrtimer_start(&obj->ps_tune0_timer, obj->ps_tune0_delay, HRTIMER_MODE_REL);
	return 0;
}
/*----------------------------------------------------------------------------*/
static int sun_num=0;
static bool sun_flag=false;
static int ls9801i_T_compare_sun_level(struct ls9801i_T_priv *obj)
{
    if (obj->ps < 10 || obj->pssar > 6)
	{
	    APS_LOG("%s: ls981x enter sun\n",__func__); 
        sun_flag=true;
		sun_num = 0;
	    if(obj->ps > obj->psi){
    		atomic_set(&obj->ps_high_thd_val, obj->ps + 2*ls9801i_T_obj->ls9801i_T_ht_n_ct);
    		atomic_set(&obj->ps_low_thd_val, obj->ps + 2*ls9801i_T_obj->ls9801i_T_lt_n_ct);
    		ls9801i_T_write_ps_high_thd(obj->client, atomic_read(&obj->ps_high_thd_val));
    		ls9801i_T_write_ps_low_thd(obj->client, atomic_read(&obj->ps_low_thd_val));
        }
		APS_LOG("%s: CTTRACKING sun state set HT=%d, LT=%d\n", __func__,atomic_read(&obj->ps_high_thd_val),atomic_read(&obj->ps_low_thd_val));
		return 1;
	}
	if(true == sun_flag)
	{
		sun_num++;
		if(sun_num > 30)
		{
			sun_flag = false;
		}
		return 1;
	}	
	return 0;
}

static int ls9801i_T_ps_tune_zero_func_fae(struct ls9801i_T_priv *obj)
{
	uint16_t ct_value = 0;
	int32_t word_data;
	// bool ps_enabled = false;
	int ret, diff;
    u8 buf[1];

#ifdef CTTRACKING
    ls9801i_T_read_ps(obj->client,&obj->ps);
	ret = ls9801i_T_master_recv(obj->client, 0x51, buf, 0x01);
	if (ret < 0)
	{
		APS_DBG("error: %d\n", ret);
		return -EFAULT;
	}
	obj->pssar = buf[0];
    word_data = obj->ps;
    APS_LOG("%s: word_data=%d,obj->pssar:%d \n", __func__, word_data,obj->pssar);
	if(ls9801i_T_compare_sun_level(obj) != 0){
		 APS_LOG("%s: ls981x enter sun\n",__func__); 
		return 0xffff;
	}

	if ((obj->psi_set != 0))
	{
		if (obj->ps_distance_last == 1)
		{
			ct_value = atomic_read(&obj->ps_high_thd_val) - ls9801i_T_obj->ls9801i_T_ht_n_ct;
			if ((word_data < ct_value) && ((ct_value - word_data) > 30))
			{
				atomic_set(&obj->ps_high_thd_val, word_data + ls9801i_T_obj->ls9801i_T_ht_n_ct);
				atomic_set(&obj->ps_low_thd_val, word_data + ls9801i_T_obj->ls9801i_T_lt_n_ct);
				ls9801i_T_write_ps_high_thd(obj->client, atomic_read(&obj->ps_high_thd_val));
				ls9801i_T_write_ps_low_thd(obj->client, atomic_read(&obj->ps_low_thd_val));
				APS_LOG("%s: CTTRACKING set HT=%d, LT=%d\n", __func__,atomic_read(&obj->ps_high_thd_val),atomic_read(&obj->ps_low_thd_val));
				obj->psi = word_data;
                obj->ps_thd_update = true;
				APS_LOG("%s: CTTRACKING update psi=%d\n",__func__, obj->psi);
				if ((atomic_read(&obj->ps_high_thd_val) + 100) < (obj->ps_high_thd_boot))
				{
					obj->ps_high_thd_boot = word_data + obj->ls9801i_T_ht_n_ct + LS9801I_T_H_LT;
					obj->ps_low_thd_boot = word_data + obj->ls9801i_T_lt_n_ct + LS9801I_T_H_LT;
					obj->boot_cali = 1;
					APS_LOG("%s: update boot HT=%d, LT=%d\n",__func__,obj->ps_high_thd_boot, obj->ps_low_thd_boot);
				}
			}
		}
		return 0;
	}

#endif

	if (word_data > obj->psa)
	{
		obj->psa = word_data;
		APS_LOG("%s: update psa: psa=%d,psi=%d\n", __func__, obj->psa, obj->psi);
	}
	if (word_data < obj->psi)
	{
		obj->psi = word_data;
		APS_LOG("%s: update psi: psa=%d,psi=%d\n", __func__, obj->psa, obj->psi);
	}

	diff = obj->psa - obj->psi;
	if (diff > obj->ls9801i_T_max_min_diff)
	{
		obj->psi_set = obj->psi;
		atomic_set(&obj->ps_high_thd_val, obj->psi + obj->ls9801i_T_ht_n_ct);
		atomic_set(&obj->ps_low_thd_val, obj->psi + obj->ls9801i_T_lt_n_ct);
		if ((ret = ls9801i_T_write_ps_high_thd(obj->client, atomic_read(&obj->ps_high_thd_val))))
		{
			APS_ERR("write high thd error: %d\n", ret);
			return ret;
		}
		if ((ret = ls9801i_T_write_ps_low_thd(obj->client, atomic_read(&obj->ps_low_thd_val))))
		{
			APS_ERR("write low thd error: %d\n", ret);
			return ret;
		}
		APS_LOG("%s: set HT=%d, LT=%d\n", __func__,atomic_read(&obj->ps_high_thd_val),atomic_read(&obj->ps_low_thd_val));
        #ifndef CTTRACKING
		hrtimer_cancel(&obj->ps_tune0_timer);
        #endif
	}
	return 0;
}
#endif	/*#ifdef LS9801I_T_TUNE0	*/
static void ls9801i_T_ps_tune0_work_func(struct work_struct *work)
{
#ifdef LS9801I_T_TUNE0
	struct ls9801i_T_priv *obj = container_of(work, struct ls9801i_T_priv, ls9801i_T_ps_tune0_work);
	if (obj->tune_zero_init_proc)
		ls9801i_T_tune_zero_get_ps_data(obj);
	else
		ls9801i_T_ps_tune_zero_func_fae(obj);

#endif
	return;
}
/*----------------------------------------------------------------------------*/
#ifdef LS9801I_T_TUNE0
static enum hrtimer_restart ls9801i_T_ps_tune0_timer_func(struct hrtimer *timer)
{
	struct ls9801i_T_priv *obj = container_of(timer, struct ls9801i_T_priv, ps_tune0_timer);
	queue_work(obj->ls9801i_T_ps_tune0_wq, &obj->ls9801i_T_ps_tune0_work);
	hrtimer_forward_now(&obj->ps_tune0_timer, obj->ps_tune0_delay);
	return HRTIMER_RESTART;
}
#endif
/*----------------------------------------------------------------------------*/
static void ls9801i_T_ps_int_handle(struct ls9801i_T_priv *obj, int nf_state)
{
	struct hwm_sensor_data sensor_data;
	memset(&sensor_data, 0, sizeof(sensor_data));
	sensor_data.values[0] = nf_state;
	sensor_data.value_divide = 1;
	sensor_data.status = SENSOR_STATUS_ACCURACY_MEDIUM;
	APS_LOG("%s:ps raw %d -> value %d \n", __FUNCTION__, obj->ps, sensor_data.values[0]);

	if (ps_report_interrupt_data(sensor_data.values[0]))
		APS_ERR("call ps_report_interrupt_data fail\n");

	obj->ps_distance_last = sensor_data.values[0];
#ifdef LS_PS_FACTORY_TEST
    g_ps_last_status = sensor_data.values[0];
#endif
}
/*----------------------------------------------------------------------------*/
void ls9801i_T_eint_func(void)
{
	struct ls9801i_T_priv *obj = ls9801i_T_obj;
	APS_LOG(" interrupt fuc\n");

	if (!obj)
	{
		return;
	}

	//schedule_work(&obj->eint_work);
	if (obj->hw->polling_mode_ps == 0 || obj->hw->polling_mode_als == 0)
		schedule_delayed_work(&obj->eint_work, 0);

	if (atomic_read(&obj->trace) & LS9801I_T_TRC_EINT)
	{
		APS_LOG("eint: als/ps intrs\n");
	}
}
/*----------------------------------------------------------------------------*/
static void ls9801i_T_eint_work(struct work_struct *work)
{
	struct ls9801i_T_priv *obj = ls9801i_T_obj;
	int err;
	u8 flag_reg;
	u8 near_far_state = 0;
    
	APS_LOG(" eint work\n");

	if ((err = ls9801i_T_check_intr(obj->client, &flag_reg)))
	{
		APS_ERR("ls9801i_T_check_intr fail: %d\n", err);
		goto err_i2c_rw;
	}

	APS_LOG(" &obj->pending_intr =%lx\n", obj->pending_intr);

	if (((1 << LS9801I_T_BIT_ALS) & obj->pending_intr) && (obj->hw->polling_mode_als == 0))
	{
		//get raw data
		APS_LOG("LS9801I-T als change\n");
		if ((err = ls9801i_T_read_als(obj->client, &obj->als)))
		{
			APS_ERR("ls9801i_T_read_als failed %d\n", err);
			goto err_i2c_rw;
		}
	}

    if (((1 << LS9801I_T_BIT_PS) &  obj->pending_intr) && (obj->hw->polling_mode_ps == 0))
    {
    	APS_LOG("LS9801I-T ps change\n");
    	if ((err = ls9801i_T_read_ps(obj->client, &obj->ps)))
    	{
    		APS_ERR("ls9801i_T read ps data: %d\n", err);
    		goto err_i2c_rw;
    	}

        if(flag_reg & LS9801I_T_IFLAG_P_H)
        {
            near_far_state = 0;
        }else{
            near_far_state = 1;
        }

		ls9801i_T_ps_int_handle(obj, near_far_state);
        #if 0
		APS_LOG("%s: ps interrupt, show all reg\n", __FUNCTION__);
		show_allreg(); //for debug
        #endif
    }

#if defined(CONFIG_OF)
	enable_irq(obj->irq);
#endif
	return;
err_i2c_rw:
	msleep(30);
#if defined(CONFIG_OF)
	enable_irq(obj->irq);
#endif
	return;
}

#if defined(CONFIG_OF)
static irqreturn_t ls9801i_T_eint_handler(int irq, void *desc)
{
	printk("----------------ls9801i_T_eint_handler-----------------------\n");
	disable_irq_nosync(ls9801i_T_obj->irq);
	ls9801i_T_eint_func();
	return IRQ_HANDLED;
}
#endif

/*----------------------------------------------------------------------------*/
/*for interrup work mode support --*/
int ls9801i_T_setup_eint(struct i2c_client *client)
{
	int ret;
	struct pinctrl *pinctrl;
	//struct pinctrl_state *pins_default;
	struct pinctrl_state *pins_cfg;
	u32 ints[2] = { 0, 0 };

	pinctrl = devm_pinctrl_get(&client->dev);

	if (IS_ERR(pinctrl))
	{
		ret = PTR_ERR(pinctrl);
		APS_ERR("Cannot find alsps pinctrl!\n");
	}

	pins_cfg = pinctrl_lookup_state(pinctrl, "alps_pins_int_cfg");

	if (IS_ERR(pins_cfg))
	{
		ret = PTR_ERR(pins_cfg);
		APS_ERR("Cannot find alsps pinctrl alps_pins_int_cfg!\n");
	}

	/* eint request */
	if (ls9801i_T_obj->irq_node)
	{
	    APS_ERR("eint request \n");
		//node = of_find_matching_node(node, touch_of_match);
		pinctrl_select_state(pinctrl, pins_cfg);
#ifndef CONFIG_MTK_EIC
		/*upstream code*/
		ints[0] = of_get_named_gpio(ls9801i_T_obj->irq_node, "deb-gpios", 0);
		if (ints[0] < 0) {
			APS_ERR("debounce gpio not found\n");
		} else{
			ret = of_property_read_u32(ls9801i_T_obj->irq_node, "debounce", &ints[1]);
			if (ret < 0)
				APS_ERR("debounce time not found\n");
			else
				gpio_set_debounce(ints[0], ints[1]);
			APS_LOG("ints[0] = %d, ints[1] = %d!!\n", ints[0], ints[1]);
		}
#else
		ret = of_property_read_u32_array(ls9801i_T_obj->irq_node, "debounce", ints, ARRAY_SIZE(ints));
		if (ret) {
			APS_ERR("of_property_read_u32_array fail, ret = %d\n", ret);
			return ret;
		}
		gpio_set_debounce(ints[0], ints[1]);
		pinctrl_select_state(pinctrl, pins_cfg);
		APS_LOG("ints[0] = %d, ints[1] = %d!!\n", ints[0], ints[1]);
#endif
		ls9801i_T_obj->irq = irq_of_parse_and_map(ls9801i_T_obj->irq_node, 0);
		APS_LOG("ls9801i_T_obj->irq = %d\n", ls9801i_T_obj->irq);

		if (!ls9801i_T_obj->irq)
		{
			APS_ERR("irq_of_parse_and_map fail!!\n");
			return -EINVAL;
		}

        APS_ERR("ls9801i_T_obj->irq = %d \n", ls9801i_T_obj->irq);
		if (request_irq(ls9801i_T_obj->irq, ls9801i_T_eint_handler, IRQF_TRIGGER_LOW, "ALS-eint", NULL))
		{
			APS_ERR("IRQ LINE NOT AVAILABLE!!\n");
			return -EINVAL;
		}

		enable_irq_wake(ls9801i_T_obj->irq);
	}
	else
	{
		APS_ERR("null irq node!!\n");
		return -EINVAL;
	}
	return 0;
}

/*----------------------------------------------------------------------------*/
static int ls9801i_T_init_client(struct i2c_client *client)
{
	struct ls9801i_T_priv *obj = i2c_get_clientdata(client);
	int err,i;
	u8 regs_value;
    u8 regs[14] = {0x10,0x15,0x62,0x63,0x29,0x19,0x1b,LS9801I_T_WCRTL_REG,LS9801I_T_LIGHT_AWT_REG,LS9801I_T_LIGHT_AITL_REG,LS9801I_T_LIGHT_AITH_REG,
                   LS9801I_T_PROX_INT_CTRL_REG,LS9801I_T_PROX_INT1EN_REG,LS9801I_T_PROX_INT2EN_REG};
	u8 value[14] = {0x23,0x01,0x0d,0x01,0x51,0x4c,0x4a,LS9801I_T_WCRTL_VALUE,LS9801I_T_LIGHT_AWT_VALUE,LS9801I_T_LIGHT_AITL_VALUE,LS9801I_T_LIGHT_AITH_VALUE,
                   LS9801I_T_PROX_INT_CTRL_VALUE,LS9801I_T_PROX_INT1EN_VALUE,LS9801I_T_PROX_INT2EN_VALUE};

	APS_LOG("%s: In\n", __FUNCTION__);

	if ((err = ls9801i_T_read_id(client)))
	{
		APS_ERR("ls9801i_T_read_id error, err=%d", err);
		return err;
	}

	if ((err = ls9801i_T_write_sw_reset(client)))
	{
		APS_ERR("software reset error, err=%d", err);
		return err;
	}

	if (obj->first_boot == true)
	{
		if (obj->hw->polling_mode_ps == 0 || obj->hw->polling_mode_als == 0)
		{
			if ((err = ls9801i_T_setup_eint(client)))
			{
				APS_ERR("setup eint error: %d\n", err);
				return err;
			}
		}
	}
    for(i=0;i< sizeof(regs);i++)
    {
        regs_value = value[i];
        err = ls9801i_T_master_send(client, regs[i], &regs_value, 1);
    	if (err < 0)
    	{
    		APS_ERR("write init sys = %d\n", err);
    		return -EFAULT;
    	}
    }
    regs_value = LS9801I_T_LIGHT_AGAIN_VALUE;
    err = ls9801i_T_write_als_again(client, regs_value);
	if(err)
	{
		APS_ERR("write ls9801i_T_write_als_again error: %d\n", err);
		return err;
	}
	regs_value = LS9801I_T_LIGHT_DGAIN_VALUE;
    err = ls9801i_T_write_als_dgain(client,regs_value);
	if(err)
	{
		APS_ERR("write ls9801i_T_write_als_dgain error: %d\n", err);
		return err;
	}
	regs_value = LS9801I_T_PROX_PWT_VALUE;
	if ((err = ls9801i_T_pwait(client, regs_value)))
	{
		APS_ERR("write ps error: %d\n", err);
		return err;
	}
	regs_value = LS9801I_T_PROX_PIT_VALUE;
	if ((err = ls9801i_T_pit(client, regs_value)))
	{
		APS_ERR("write ps error: %d\n", err);
		return err;
	}
    regs_value = LS9801I_T_PROX_PPULSE_VALUE;
    if ((err = ls9801i_T_ppulse(client, regs_value)))
    {
        APS_ERR("write ls9801i_T_ppulse error: %d\n", err);
        return err;
    }
	if ((err = ls9801i_T_pctrl(client)))
	{
		APS_ERR("write ps error: %d\n", err);
		return err;
	}

	regs_value = LS9801I_T_PROX_PGAIN_VALUE;
	if ((err = ls9801i_T_pgain(client, regs_value)))
	{
		APS_ERR("write ps error: %d\n", err);
		return err;
	}
	regs_value = LS9801I_T_PROX_PDRV_VALUE;
	if ((err = ls9801i_T_pdrv(client, regs_value)))
	{
		APS_ERR("write ps error: %d\n", err);
		return err;
	}

			
#ifndef LS9801I_T_TUNE0
	if ((err = ls9801i_T_write_ps_high_thd(client, atomic_read(&obj->ps_high_thd_val))))
	{
		APS_ERR("write high thd error: %d\n", err);
		return err;
	}

	if ((err = ls9801i_T_write_ps_low_thd(client, atomic_read(&obj->ps_low_thd_val))))
	{
		APS_ERR("write low thd error: %d\n", err);
		return err;
}
#endif


#ifdef LS9801I_T_TUNE0
	if (obj->first_boot == true)
		ls9801i_T_ps_tune_zero_init(obj);
#endif

	obj->ps_distance_last = -1;
	return 0;
}

/******************************************************************************
 * Sysfs attributes
*******************************************************************************/
static ssize_t ls9801i_T_show_config(struct device_driver *ddri, char *buf)
{
	ssize_t res;

	if (!ls9801i_T_obj)
	{
		APS_ERR("ls9801i_T_obj is null!!\n");
		return 0;
	}

	res = scnprintf(buf, PAGE_SIZE, "(%d %d %d %d %d %d)\n",
		atomic_read(&ls9801i_T_obj->i2c_retry),
		atomic_read(&ls9801i_T_obj->als_debounce),
		atomic_read(&ls9801i_T_obj->ps_mask),
		atomic_read(&ls9801i_T_obj->ps_high_thd_val),
		atomic_read(&ls9801i_T_obj->ps_low_thd_val),
		atomic_read(&ls9801i_T_obj->ps_debounce));
	return res;
}
/*----------------------------------------------------------------------------*/
static ssize_t ls9801i_T_store_config(struct device_driver *ddri, const char *buf, size_t count)
{
	int retry, als_deb, ps_deb, mask, hthres, lthres, err;
	struct i2c_client *client;
	client = ls9801i_T_i2c_client;

	if (!ls9801i_T_obj)
	{
		APS_ERR("ls9801i_T_obj is null!!\n");
		return 0;
	}

	if (6 == sscanf(buf, "%d %d %d %d %d %d", &retry, &als_deb, &mask, &hthres, &lthres, &ps_deb))
	{
		atomic_set(&ls9801i_T_obj->i2c_retry, retry);
		atomic_set(&ls9801i_T_obj->als_debounce, als_deb);
		atomic_set(&ls9801i_T_obj->ps_mask, mask);
		atomic_set(&ls9801i_T_obj->ps_high_thd_val, hthres);
		atomic_set(&ls9801i_T_obj->ps_low_thd_val, lthres);
		atomic_set(&ls9801i_T_obj->ps_debounce, ps_deb);

		if ((err = ls9801i_T_write_ps_high_thd(client, atomic_read(&ls9801i_T_obj->ps_high_thd_val))))
		{
			APS_ERR("write high thd error: %d\n", err);
			return err;
		}

		if ((err = ls9801i_T_write_ps_low_thd(client, atomic_read(&ls9801i_T_obj->ps_low_thd_val))))
		{
			APS_ERR("write low thd error: %d\n", err);
			return err;
		}
	}
	else
	{
		APS_ERR("invalid content: '%s'\n", buf);
	}

	return count;
}
/*----------------------------------------------------------------------------*/
static ssize_t ls9801i_T_show_trace(struct device_driver *ddri, char *buf)
{
	ssize_t res;

	if (!ls9801i_T_obj)
	{
		APS_ERR("ls9801i_T_obj is null!!\n");
		return 0;
	}

	res = scnprintf(buf, PAGE_SIZE, "0x%04X\n", atomic_read(&ls9801i_T_obj->trace));
	return res;
}
/*----------------------------------------------------------------------------*/
static ssize_t ls9801i_T_store_trace(struct device_driver *ddri, const char *buf, size_t count)
{
	int trace;

	if (!ls9801i_T_obj)
	{
		APS_ERR("ls9801i_T_obj is null!!\n");
		return 0;
	}

	if (1 == sscanf(buf, "0x%x", &trace))
	{
		atomic_set(&ls9801i_T_obj->trace, trace);
	}
	else
	{
		APS_ERR("invalid content: '%s', length = %d\n", buf, (int)count);
	}

	return count;
}

/*----------------------------------------------------------------------------*/
static ssize_t ls9801i_T_show_als(struct device_driver *ddri, char *buf)
{
	int res;

	if (!ls9801i_T_obj)
	{
		APS_ERR("ls9801i_T_obj is null!!\n");
		return 0;
	}

	if ((res = ls9801i_T_read_als(ls9801i_T_obj->client, &ls9801i_T_obj->als)))
		return scnprintf(buf, PAGE_SIZE, "ERROR: %d\n", res);
	else
		return scnprintf(buf, PAGE_SIZE, "%d\n", ls9801i_T_obj->als);
}
/*----------------------------------------------------------------------------*/
static ssize_t ls9801i_T_show_ps(struct device_driver *ddri, char *buf)
{
	int res;

	if (!ls9801i_T_obj)
	{
		APS_ERR("ls9801i_T_obj is null!!\n");
		return 0;
	}

	if ((res = ls9801i_T_read_ps(ls9801i_T_obj->client, &ls9801i_T_obj->ps)))
	{
		return scnprintf(buf, PAGE_SIZE, "ERROR: %d\n", res);
	}
	else
	{
		return scnprintf(buf, PAGE_SIZE, "%d\n", ls9801i_T_obj->ps);
	}
}
/*----------------------------------------------------------------------------*/
static ssize_t ls9801i_T_show_reg(struct device_driver *ddri, char *buf)
{
	u8 int_status;

	if (!ls9801i_T_obj)
	{
		APS_ERR("ls9801i_T_obj is null!!\n");
		return 0;
	}

	/*read*/
	ls9801i_T_check_intr(ls9801i_T_obj->client, &int_status);
	ls9801i_T_read_ps(ls9801i_T_obj->client, &ls9801i_T_obj->ps);
	ls9801i_T_read_als(ls9801i_T_obj->client, &ls9801i_T_obj->als);
	/*write*/
	ls9801i_T_write_ps_high_thd(ls9801i_T_obj->client, atomic_read(&ls9801i_T_obj->ps_high_thd_val));
	ls9801i_T_write_ps_low_thd(ls9801i_T_obj->client, atomic_read(&ls9801i_T_obj->ps_low_thd_val));
	return 0;
}
/*----------------------------------------------------------------------------*/
static ssize_t ls9801i_T_show_send(struct device_driver *ddri, char *buf)
{
	return 0;
}
/*----------------------------------------------------------------------------*/
static ssize_t ls9801i_T_store_send(struct device_driver *ddri, const char *buf, size_t count)
{
	int addr, cmd;
	u8 data;

	if (!ls9801i_T_obj)
	{
		APS_ERR("ls9801i_T_obj is null!!\n");
		return 0;
	}
	else if (2 != sscanf(buf, "%x %x", &addr, &cmd))
	{
		APS_ERR("invalid format: '%s'\n", buf);
		return 0;
	}

	data = (u8)cmd;
	APS_LOG("send(%02X, %02X) = %d\n", addr, cmd,ls9801i_T_master_send(ls9801i_T_obj->client,(u16)addr, &data, sizeof(data)));
	return count;
}
/*----------------------------------------------------------------------------*/
static ssize_t ls9801i_T_show_recv(struct device_driver *ddri, char *buf)
{
	if (!ls9801i_T_obj)
	{
		APS_ERR("ls9801i_T_obj is null!!\n");
		return 0;
	}

	return scnprintf(buf, PAGE_SIZE, "0x%04X\n", atomic_read(&ls9801i_T_obj->recv_reg));
}
/*----------------------------------------------------------------------------*/
static ssize_t ls9801i_T_store_recv(struct device_driver *ddri, const char *buf, size_t count)
{
	int addr;
	u8 dat;

	if (!ls9801i_T_obj)
	{
		APS_ERR("ls9801i_T_obj is null!!\n");
		return 0;
	}
	else if (1 != sscanf(buf, "%x", &addr))
	{
		APS_ERR("invalid format: '%s'\n", buf);
		return 0;
	}

	APS_ERR("recv(%02X) = %d, 0x%02X\n", addr,
		ls9801i_T_master_recv(ls9801i_T_obj->client, (u16)addr, (char*)&dat, sizeof(dat)), dat);
	atomic_set(&ls9801i_T_obj->recv_reg, dat);
	return count;
}
/*----------------------------------------------------------------------------*/

static ssize_t ls9801i_T_show_allreg(struct device_driver *ddri, char *buf)
{
	int ret = 0;
	u8 rbuf[33];
	int cnt;
	int len = 0;
	memset(rbuf, 0, sizeof(rbuf));

	if (!ls9801i_T_obj)
	{
		APS_ERR("ls9801i_T_obj is null!!\n");
		return 0;
	}

	ret = ls9801i_T_master_recv(ls9801i_T_obj->client, 0, &rbuf[0], 32);

	if (ret < 0)
	{
		APS_DBG("error: %d\n", ret);
		return -EFAULT;
	}

	ret = ls9801i_T_master_recv(ls9801i_T_obj->client, LS9801I_T_CHECK_ID_REG, &rbuf[32], 1);
	if (ret < 0)
	{
		APS_DBG("error: %d\n", ret);
		return -EFAULT;
	}

	for (cnt = 0; cnt < 0x20; cnt++)
	{
		APS_LOG("reg[0x%x]=0x%x\n", cnt, rbuf[cnt]);
		len += scnprintf(buf + len, PAGE_SIZE - len, "[%2X]%2X,", cnt, rbuf[cnt]);
	}

	APS_LOG("reg[0x72]=0x%x\n", rbuf[cnt]);

	len += scnprintf(buf + len, PAGE_SIZE - len, "[0x72]%2X\n", rbuf[cnt]);
	return len;
}
/*----------------------------------------------------------------------------*/
static ssize_t ls9801i_T_show_status(struct device_driver *ddri, char *buf)
{
	ssize_t len = 0;
	u8 rbuf[25];
	int ret = 0;

	if (!ls9801i_T_obj)
	{
		APS_ERR("ls9801i_T_obj is null!!\n");
		return 0;
	}

	memset(rbuf, 0, sizeof(rbuf));
	ret = ls9801i_T_master_recv(ls9801i_T_obj->client, 0, &rbuf[0], 7);

	if (ret < 0)
	{
		APS_DBG("error: %d\n", ret);
		return -EFAULT;
	}

	ret = ls9801i_T_master_recv(ls9801i_T_obj->client, 7, &rbuf[7], 7);

	if (ret < 0)
	{
		APS_DBG("error: %d\n", ret);
		return -EFAULT;
	}

	ret = ls9801i_T_master_recv(ls9801i_T_obj->client, 14, &rbuf[14], 7);

	if (ret < 0)
	{
		APS_DBG("error: %d\n", ret);
		return -EFAULT;
	}

	len += scnprintf(buf + len, PAGE_SIZE - len, "[PS=%2X] [ALS=%2X] [WAIT=%4Xms] [EN_ASO=%2X] [EN_AK=%2X] [NEAR/FAR=%2X] [FLAG_OUI=%2X] [FLAG_PSINT=%2X] [FLAG_ALSINT=%2X]\n",
		rbuf[0] & 0x01, (rbuf[0] & 0x02) >> 1, ((rbuf[0] & 0x04) >> 2) * rbuf[5] * 6, (rbuf[0] & 0x20) >> 5,
		(rbuf[0] & 0x40) >> 6, rbuf[16] & 0x01, (rbuf[16] & 0x04) >> 2, (rbuf[16] & 0x10) >> 4, (rbuf[16] & 0x20) >> 5);
	return len;
}
/*----------------------------------------------------------------------------*/
#define IS_SPACE(CH) (((CH) == ' ') || ((CH) == '\n'))
/*----------------------------------------------------------------------------*/
static int read_int_from_buf(struct ls9801i_T_priv *obj, const char* buf, size_t count,
	u32 data[], int len)
{
	int idx = 0;
	char *cur = (char*)buf, *end = (char*)(buf + count);

	while (idx < len)
	{
		while ((cur < end) && IS_SPACE(*cur))
		{
			cur++;
		}

		if (1 != sscanf(cur, "%d", &data[idx]))
		{
			break;
		}

		idx++;

		while ((cur < end) && !IS_SPACE(*cur))
		{
			cur++;
		}
	}

	return idx;
}
/*----------------------------------------------------------------------------*/
static ssize_t ls9801i_T_show_alslv(struct device_driver *ddri, char *buf)
{
	ssize_t len = 0;
	int idx;

	if (!ls9801i_T_obj)
	{
		APS_ERR("ls9801i_T_obj is null!!\n");
		return 0;
	}

	for (idx = 0; idx < ls9801i_T_obj->als_level_num; idx++)
	{
		len += scnprintf(buf + len, PAGE_SIZE - len, "%d ", ls9801i_T_obj->hw->als_level[idx]);
	}

	len += scnprintf(buf + len, PAGE_SIZE - len, "\n");
	return len;
}
/*----------------------------------------------------------------------------*/
static ssize_t ls9801i_T_store_alslv(struct device_driver *ddri, const char *buf, size_t count)
{
	if (!ls9801i_T_obj)
	{
		APS_ERR("ls9801i_T_obj is null!!\n");
		return 0;
	}
	else if (!strcmp(buf, "def"))
	{
		memcpy(ls9801i_T_obj->als_level, ls9801i_T_obj->hw->als_level, sizeof(ls9801i_T_obj->als_level));
	}
	else if (ls9801i_T_obj->als_level_num != read_int_from_buf(ls9801i_T_obj, buf, count,
		ls9801i_T_obj->hw->als_level, ls9801i_T_obj->als_level_num))
	{
		APS_ERR("invalid format: '%s'\n", buf);
	}

	return count;
}
/*----------------------------------------------------------------------------*/
static ssize_t ls9801i_T_show_alsval(struct device_driver *ddri, char *buf)
{
	ssize_t len = 0;
	int idx;

	if (!ls9801i_T_obj)
	{
		APS_ERR("ls9801i_T_obj is null!!\n");
		return 0;
	}

	for (idx = 0; idx < ls9801i_T_obj->als_value_num; idx++)
	{
		len += scnprintf(buf + len, PAGE_SIZE - len, "%d ", ls9801i_T_obj->hw->als_value[idx]);
	}

	len += scnprintf(buf + len, PAGE_SIZE - len, "\n");
	return len;
}
/*----------------------------------------------------------------------------*/
static ssize_t ls9801i_T_store_alsval(struct device_driver *ddri, const char *buf, size_t count)
{
	if (!ls9801i_T_obj)
	{
		APS_ERR("ls9801i_T_obj is null!!\n");
		return 0;
	}
	else if (!strcmp(buf, "def"))
	{
		memcpy(ls9801i_T_obj->als_value, ls9801i_T_obj->hw->als_value, sizeof(ls9801i_T_obj->als_value));
	}
	else if (ls9801i_T_obj->als_value_num != read_int_from_buf(ls9801i_T_obj, buf, count,
		ls9801i_T_obj->hw->als_value, ls9801i_T_obj->als_value_num))
	{
		APS_ERR("invalid format: '%s'\n", buf);
	}

	return count;
}

#ifdef LS9801I_T_TUNE0
static ssize_t ls9801i_T_show_cali(struct device_driver *ddri, char *buf)
{
	int32_t word_data;
	u8 r_buf[2];
	int ret;

	if (!ls9801i_T_obj)
	{
		APS_ERR("ls9801i_T_obj is null!!\n");
		return 0;
	}

	ret = ls9801i_T_master_recv(ls9801i_T_obj->client, LS9801I_T_LIGHT_RAW_REG_L, r_buf, 2);

	if (ret < 0)
	{
		APS_ERR("%s fail, err=0x%x", __FUNCTION__, ret);
		return ret;
	}

	word_data = (r_buf[0] << 8) | r_buf[1];
	ret = ls9801i_T_master_recv(ls9801i_T_obj->client, LS9801I_T_IR_RAW_REG_L, r_buf, 2);

	if (ret < 0)
	{
		APS_ERR("%s fail, err=0x%x", __FUNCTION__, ret);
		return ret;
	}

	word_data += (r_buf[0] << 8) | r_buf[1];
	APS_LOG("%s: psi_set=%d, psa=%d,psi=%d, word_data=%d\n", __FUNCTION__,
		ls9801i_T_obj->psi_set, ls9801i_T_obj->psa, ls9801i_T_obj->psi, word_data);
#ifdef CALI_EVERY_TIME
	APS_LOG("%s: boot HT=%d, LT=%d\n", __func__, ls9801i_T_obj->ps_high_thd_boot, ls9801i_T_obj->ps_low_thd_boot);
#endif
	return scnprintf(buf, PAGE_SIZE, "%5d\n", ls9801i_T_obj->psi_set);
	//return 0;
}

static ssize_t ls9801i_T_ps_maxdiff_store(struct device_driver *ddri, const char *buf, size_t count)
{
	unsigned long value = 0;
	int ret;

	if ((ret = kstrtoul(buf, 10, &value)) < 0)
	{
		printk(KERN_ERR "%s:kstrtoul failed, ret=0x%x\n", __func__, ret);
		return ret;
	}

	ls9801i_T_obj->ls9801i_T_max_min_diff = (int)value;
	return count;
}


static ssize_t ls9801i_T_ps_maxdiff_show(struct device_driver *ddri, char *buf)
{
	if (!ls9801i_T_obj)
	{
		APS_ERR("ls9801i_T_obj is null!!\n");
		return 0;
	}

	return scnprintf(buf, PAGE_SIZE, "%d\n", ls9801i_T_obj->ls9801i_T_max_min_diff);
}

static ssize_t ls9801i_T_ps_ltnct_store(struct device_driver *ddri, const char *buf, size_t count)
{
	unsigned long value = 0;
	int ret;

	if ((ret = kstrtoul(buf, 10, &value)) < 0)
	{
		printk(KERN_ERR "%s:kstrtoul failed, ret=0x%x\n", __func__, ret);
		return ret;
	}

	ls9801i_T_obj->ls9801i_T_lt_n_ct = (int)value;
	return count;
}

static ssize_t ls9801i_T_ps_ltnct_show(struct device_driver *ddri, char *buf)
{
	if (!ls9801i_T_obj)
	{
		APS_ERR("ls9801i_T_obj is null!!\n");
		return 0;
	}

	return scnprintf(buf, PAGE_SIZE, "%d\n", ls9801i_T_obj->ls9801i_T_lt_n_ct);
}

static ssize_t ls9801i_T_ps_htnct_store(struct device_driver *ddri, const char *buf, size_t count)
{
	unsigned long value = 0;
	int ret;

	if ((ret = kstrtoul(buf, 10, &value)) < 0)
	{
		printk(KERN_ERR "%s:kstrtoul failed, ret=0x%x\n", __func__, ret);
		return ret;
	}

	ls9801i_T_obj->ls9801i_T_ht_n_ct = (int)value;
	return count;
}

static ssize_t ls9801i_T_ps_htnct_show(struct device_driver *ddri, char *buf)
{
	if (!ls9801i_T_obj)
	{
		APS_ERR("ls9801i_T_obj is null!!\n");
		return 0;
	}

	return scnprintf(buf, PAGE_SIZE, "%d\n", ls9801i_T_obj->ls9801i_T_ht_n_ct);
}

#endif	// #ifdef LS9801I_T_TUNE0

/*---Offset At-------------------------------------------------------------------------*/
static ssize_t ls9801i_T_show_ps_offset(struct device_driver *ddri, char *buf)
{
	if (!ls9801i_T_obj)
	{
		APS_ERR("ls9801i_T_obj is null!!\n");
		return 0;
	}

#ifdef LS9801I_T_TUNE0
	return snprintf(buf, PAGE_SIZE, "%d\n", ls9801i_T_obj->crosstalk);
#else
	return 0;
#endif
}
/*----------------------------------------------------------------------------*/
static ssize_t ls9801i_T_set_ps_offset(struct device_driver *ddri, const char *buf, size_t count)
{
	int ret;
	ret = ls9801i_T_ps_calibration(ls9801i_T_obj->client);
	return count;
}

/*----------------------------------------------------------------------------*/
static ssize_t ls9801i_T_show_pthreshold_calibration(struct device_driver *ddri, char *buf)
{
	// struct ls9801i_T_priv *data = i2c_get_clientdata(ls9801i_T_obj->client);
#ifdef CALI_EVERY_TIME
	return snprintf(buf, PAGE_SIZE, "Low threshold = %d , High threshold = %d\n",
		ls9801i_T_obj->ps_low_thd_boot, ls9801i_T_obj->ps_high_thd_boot);
#else
	return 0;
#endif
}

static ssize_t ls9801i_T_show_raw_data(struct device_driver *ddri, char *buf)
{
	int res;

	if (!ls9801i_T_obj)
	{
		APS_ERR("ls9801i_T_obj is null!!\n");
		return 0;
	}

	if ((res = ls9801i_T_read_ps(ls9801i_T_obj->client, &ls9801i_T_obj->ps)))
	{
		return scnprintf(buf, PAGE_SIZE, "ERROR: %d\n", res);
	}
	else
	{
		return scnprintf(buf, PAGE_SIZE, "%d\n", ls9801i_T_obj->ps);
	}
}

/*----------------------------------------------------------------------------*/
#ifdef LS_PS_FACTORY_TEST
/*
*ps_fty_test node use for ps factory test
*return: 1 = FAR ; 0 = NEAR
*/
static ssize_t ps_fty_test_show(struct device_driver *ddri, char *buf)
{
    int ret = 0;

    APS_LOG(" ps_fty_test_show --->>>\n");
    if (!ls9801i_T_obj)
    {
        APS_ERR("ls9801i_T_obj is null!!\n");
        return 0;
    }

    ret = ls9801i_T_enable_ps(ls9801i_T_obj->client, 1);
    if (ret)
    {
        APS_ERR("ps enable is failed!!\n");
        return -EFAULT;
    }

    udelay(5000);

    ret = ls9801i_T_enable_ps(ls9801i_T_obj->client, 0);
    if (ret)
    {
        APS_ERR("ps disable is failed!!\n");
        return -EFAULT;
    }

    APS_LOG(" ps_fty_test_show <<<---\n");
    return scnprintf(buf, PAGE_SIZE, "%d\n", g_ps_last_status);
}
#endif

/*----------------------------------------------------------------------------*/
static DRIVER_ATTR(als, S_IWUSR | S_IRUGO, ls9801i_T_show_als, NULL);
static DRIVER_ATTR(ps, S_IWUSR | S_IRUGO, ls9801i_T_show_ps, NULL);
static DRIVER_ATTR(config, S_IWUSR | S_IRUGO, ls9801i_T_show_config, ls9801i_T_store_config);
static DRIVER_ATTR(alslv, S_IWUSR | S_IRUGO, ls9801i_T_show_alslv, ls9801i_T_store_alslv);
static DRIVER_ATTR(alsval, S_IWUSR | S_IRUGO, ls9801i_T_show_alsval, ls9801i_T_store_alsval);
static DRIVER_ATTR(trace, S_IWUSR | S_IRUGO, ls9801i_T_show_trace, ls9801i_T_store_trace);
static DRIVER_ATTR(send, S_IWUSR | S_IRUGO, ls9801i_T_show_send, ls9801i_T_store_send);
static DRIVER_ATTR(recv, S_IWUSR | S_IRUGO, ls9801i_T_show_recv, ls9801i_T_store_recv);
static DRIVER_ATTR(reg, S_IWUSR | S_IRUGO, ls9801i_T_show_reg, NULL);
static DRIVER_ATTR(allreg, S_IWUSR | S_IRUGO, ls9801i_T_show_allreg, NULL);
static DRIVER_ATTR(status, S_IWUSR | S_IRUGO, ls9801i_T_show_status, NULL);
static DRIVER_ATTR(pscalibration, S_IWUSR | S_IRUGO, ls9801i_T_show_ps_offset, ls9801i_T_set_ps_offset);
static DRIVER_ATTR(pthredcalibration, S_IWUSR | S_IRUGO, ls9801i_T_show_pthreshold_calibration, NULL);
static DRIVER_ATTR(raw_data, S_IWUSR | S_IRUGO, ls9801i_T_show_raw_data, NULL);
#ifdef LS9801I_T_TUNE0
static DRIVER_ATTR(cali, S_IWUSR | S_IRUGO, ls9801i_T_show_cali, NULL);
static DRIVER_ATTR(maxdiff, S_IWUSR | S_IRUGO, ls9801i_T_ps_maxdiff_show, ls9801i_T_ps_maxdiff_store);
static DRIVER_ATTR(ltnct, S_IWUSR | S_IRUGO, ls9801i_T_ps_ltnct_show, ls9801i_T_ps_ltnct_store);
static DRIVER_ATTR(htnct, S_IWUSR | S_IRUGO, ls9801i_T_ps_htnct_show, ls9801i_T_ps_htnct_store);
#endif
#ifdef LS_PS_FACTORY_TEST
static DRIVER_ATTR_RO(ps_fty_test);
#endif

/*----------------------------------------------------------------------------*/
static struct driver_attribute *ls9801i_T_attr_list[] =
{
	&driver_attr_als,
	&driver_attr_ps,
	&driver_attr_trace,      /*trace log*/
	&driver_attr_config,
	&driver_attr_alslv,
	&driver_attr_alsval,
	&driver_attr_status,
	&driver_attr_send,
	&driver_attr_recv,
	&driver_attr_allreg,
	&driver_attr_reg,
	&driver_attr_pscalibration,
	&driver_attr_pthredcalibration,
	&driver_attr_raw_data,
#ifdef LS9801I_T_TUNE0
	&driver_attr_cali,
	&driver_attr_maxdiff,
	&driver_attr_ltnct,
	&driver_attr_htnct,
#endif
#ifdef LS_PS_FACTORY_TEST
	&driver_attr_ps_fty_test,
#endif

};

/*----------------------------------------------------------------------------*/
static int ls9801i_T_create_attr(struct device_driver *driver)
{
	int idx, err = 0;
	int num = (int)(sizeof(ls9801i_T_attr_list) / sizeof(ls9801i_T_attr_list[0]));

	if (driver == NULL)
	{
		return -EINVAL;
	}

	for (idx = 0; idx < num; idx++)
	{
		err = driver_create_file(driver, ls9801i_T_attr_list[idx]);
		if (err)
		{
			APS_ERR("driver_create_file (%s) = %d\n", ls9801i_T_attr_list[idx]->attr.name, err);
			break;
		}
	}
	return err;
}
/*----------------------------------------------------------------------------*/
static int ls9801i_T_delete_attr(struct device_driver *driver)
{
	int idx, err = 0;
	int num = (int)(sizeof(ls9801i_T_attr_list) / sizeof(ls9801i_T_attr_list[0]));

	if (!driver)
		return -EINVAL;

	for (idx = 0; idx < num; idx++)
	{
		driver_remove_file(driver, ls9801i_T_attr_list[idx]);
	}

	return err;
}
/******************************************************************************
 * Function Configuration
******************************************************************************/

static int ls9801i_T_get_als_value(struct ls9801i_T_priv *obj, u16 als)
{
	//int i;
	uint16_t lux = 0;

	APS_DBG("ls9801i_T getLuxFromAlsData: mTask.als_raw_data =%d,mTask.r_data=%d\n",als,obj->ir);
	lux = als;
	return lux;
}
/*----------------------------------------------------------------------------*/
static int ls9801i_T_get_ps_value_only(struct ls9801i_T_priv *obj, u16 ps)
{
	int mask = atomic_read(&obj->ps_mask);
	int invalid = 0, val;
	int err;
	u8 flag;
	err = ls9801i_T_int_en(obj->client, &flag);
	if (err)
		return err;

	if ((flag & LS9801I_T_IFLAG_P_H) == 0x80)
	{
		val = 0;
	}else if((flag & LS9801I_T_IFLAG_P_L)==0x40)
	{
		val = 1;
	}
	if (atomic_read(&obj->ps_suspend))
	{
		invalid = 1;
	}
	else if (1 == atomic_read(&obj->ps_deb_on))
	{
		unsigned long endt = atomic_read(&obj->ps_deb_end);

		if (time_after(jiffies, endt))
		{
			atomic_set(&obj->ps_deb_on, 0);
		}

		if (1 == atomic_read(&obj->ps_deb_on))
		{
			invalid = 1;
		}
	}
	if (!invalid)
	{
		if (unlikely(atomic_read(&obj->trace) & LS9801I_T_TRC_CVT_PS))
		{
			if (mask)
			{
				APS_DBG("PS:  %05d => %05d [M] \n", ps, val);
			}
			else
			{
				APS_DBG("PS:  %05d => %05d\n", ps, val);
			}
		}

		return val;
	}
	else
	{
		APS_ERR(" ps value is invalid, PS:  %05d => %05d\n", ps, val);

		if (unlikely(atomic_read(&obj->trace) & LS9801I_T_TRC_CVT_PS))
		{
			APS_DBG("PS:  %05d => %05d (-1)\n", ps, val);
		}

		return -1;
	}
}
/*----------------------------------------------------------------------------*/
static int ls9801i_T_get_ps_value(struct ls9801i_T_priv *obj, u16 ps)
{
	int mask = atomic_read(&obj->ps_mask);
	int invalid = 0, val=1;
	//u8 flag;
	int tmp_ps_high_thd_val;
	int tmp_ps_low_thd_val;

	tmp_ps_high_thd_val = atomic_read(&obj->ps_high_thd_val);
	tmp_ps_low_thd_val = atomic_read(&obj->ps_low_thd_val);
	if(ps >tmp_ps_high_thd_val)
	{
		val = 0;
        obj->ps_temp =0;
	}else if(ps < tmp_ps_low_thd_val)
	{
		val = 1;
        obj->ps_temp =1;
	}else{
        val = obj->ps_temp;
    }
	obj->ps_distance_last = val; 

	APS_LOG("%s: read_flag val = %d\n", __FUNCTION__, val);

	if (atomic_read(&obj->ps_suspend))
	{
		invalid = 1;
	}
	else if (1 == atomic_read(&obj->ps_deb_on))
	{
		unsigned long endt = atomic_read(&obj->ps_deb_end);

		if (time_after(jiffies, endt))
		{
			atomic_set(&obj->ps_deb_on, 0);
		}

		if (1 == atomic_read(&obj->ps_deb_on))
		{
			invalid = 1;
		}
	}

	if (!invalid)
	{
		if (unlikely(atomic_read(&obj->trace) & LS9801I_T_TRC_CVT_PS))
		{
			if (mask)
			{
				APS_DBG("PS:  %05d => %05d [M] \n", ps, val);
			}
			else
			{
				APS_DBG("PS:  %05d => %05d\n", ps, val);
			}
		}

		return val;
	}
	else
	{
		APS_ERR(" ps value is invalid, PS:  %05d => %05d\n", ps, val);

		if (unlikely(atomic_read(&obj->trace) & LS9801I_T_TRC_CVT_PS))
		{
			APS_DBG("PS:  %05d => %05d (-1)\n", ps, val);
		}

		return -1;
	}
}


/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
static int ls9801i_T_i2c_suspend(struct device *dev) 
{
	int err;
	struct i2c_client *client = to_i2c_client(dev);
	struct ls9801i_T_priv *obj = i2c_get_clientdata(client);
	

	APS_LOG("ls9801i_T_i2c_suspend\n");

	if (!obj) {
		APS_ERR("null pointer!!\n");
		return 0;
	}

	atomic_set(&obj->als_suspend, 1);
	err = ls9801i_T_enable_als(obj->client, 0);
	if (err)
		APS_ERR("disable als fail: %d\n", err);

	return 0;
}
/*----------------------------------------------------------------------------*/
static int ls9801i_T_i2c_resume(struct device *dev)
{
	int err;
	struct i2c_client *client = to_i2c_client(dev);
   	struct ls9801i_T_priv *obj = i2c_get_clientdata(client);
	
	APS_LOG("ls9801i_T_i2c_resume\n");
	if (!obj) {
		APS_ERR("null pointer!!\n");
		return 0;
	}

	atomic_set(&obj->als_suspend, 0);
	if (test_bit(LS9801I_T_BIT_ALS, &obj->enable)) {
		err = ls9801i_T_enable_als(obj->client, 1);
		if (err)
			APS_ERR("enable als fail: %d\n", err);
	}

	return 0;
}
/*----------------------------------------------------------------------------*/
static int ls9801i_T_i2c_detect(struct i2c_client *client, struct i2c_board_info *info)
{
	strcpy(info->type, LS9801I_T_DEV_NAME);
	return 0;
}

static int als_open_report_data(int open)
{
	//should queuq work to report event if  is_report_input_direct=true
	return 0;
}

// if use  this typ of enable , Gsensor only enabled but not report inputEvent to HAL

static int als_enable_nodata(int en)
{
	int res = 0;
#ifdef CUSTOM_KERNEL_SENSORHUB
	SCP_SENSOR_HUB_DATA req;
	int len;
#endif //#ifdef CUSTOM_KERNEL_SENSORHUB
	APS_LOG("ls9801i_T_obj als enable value = %d\n", en);
#ifdef CUSTOM_KERNEL_SENSORHUB
	req.activate_req.sensorType = ID_LIGHT;
	req.activate_req.action = SENSOR_HUB_ACTIVATE;
	req.activate_req.enable = en;
	len = sizeof(req.activate_req);
	res = SCP_sensorHub_req_send(&req, &len, 1);
#else //#ifdef CUSTOM_KERNEL_SENSORHUB

	if (!ls9801i_T_obj)
	{
		APS_ERR("ls9801i_T_obj is null!!\n");
		return -1;
	}

	res = ls9801i_T_enable_als(ls9801i_T_obj->client, en);
#endif //#ifdef CUSTOM_KERNEL_SENSORHUB

	if (res)
	{
		APS_ERR("als_enable_nodata is failed!!\n");
		return -1;
	}

	return 0;
}

static int als_set_delay(u64 ns)
{
	return 0;
}

static int als_batch(int flag, int64_t samplingPeriodNs, int64_t maxBatchReportLatencyNs)
{
	return als_set_delay(samplingPeriodNs);
}

static int als_flush(void)
{
	return als_flush_report();
}

static int als_get_data(int* value, int* status)
{
	int err = 0;
#ifdef CUSTOM_KERNEL_SENSORHUB
	SCP_SENSOR_HUB_DATA req;
	int len;
#else
	struct ls9801i_T_priv *obj = NULL;
#endif //#ifdef CUSTOM_KERNEL_SENSORHUB
#ifdef CUSTOM_KERNEL_SENSORHUB
	req.get_data_req.sensorType = ID_LIGHT;
	req.get_data_req.action = SENSOR_HUB_GET_DATA;
	len = sizeof(req.get_data_req);
	err = SCP_sensorHub_req_send(&req, &len, 1);

	if (err)
	{
		APS_ERR("SCP_sensorHub_req_send fail!\n");
	}
	else
	{
		*value = req.get_data_rsp.int16_Data[0];
		*status = SENSOR_STATUS_ACCURACY_MEDIUM;
	}

	if (atomic_read(&ls9801i_T_obj->trace) & CMC_TRC_PS_DATA)
	{
		APS_LOG("value = %d\n", *value);
		//show data
	}

#else //#ifdef CUSTOM_KERNEL_SENSORHUB

	if (!ls9801i_T_obj)
	{
		APS_ERR("ls9801i_T_obj is null!!\n");
		return -1;
	}

	obj = ls9801i_T_obj;
	if ((err = ls9801i_T_read_als(obj->client, &obj->als)))
	{
		err = -1;
	}
	else
	{
		if (obj->als < 3)
		{
			obj->als_last = obj->als;
			*value = ls9801i_T_get_als_value(obj, 0);
		}
		else 
		{
			obj->als_last = obj->als;
			*value = ls9801i_T_get_als_value(obj, obj->als);
		}

		*status = SENSOR_STATUS_ACCURACY_MEDIUM;
	}

	obj->als_debug_count ++;
	if(obj->als_debug_count > 50000){
		obj->als_debug_count = 0;
	}else if((obj->als_debug_count%10)==5){
		APS_ERR("ALS: als=%d, lux=%d\n", obj->als, (*value));
	}
#endif //#ifdef CUSTOM_KERNEL_SENSORHUB
	return err;
}

static int ps_open_report_data(int open)
{
	//should queuq work to report event if  is_report_input_direct=true
	return 0;
}
static int ps_enable_nodata(int en)
{
	int res = 0;
	//struct ls9801i_T_priv *obj = i2c_get_clientdata(client);
#ifdef CUSTOM_KERNEL_SENSORHUB
	SCP_SENSOR_HUB_DATA req;
	int len;
#endif //#ifdef CUSTOM_KERNEL_SENSORHUB
	APS_LOG("ls9801i_T_obj als enable value = %d\n", en);
#ifdef CUSTOM_KERNEL_SENSORHUB
	req.activate_req.sensorType = ID_PROXIMITY;
	req.activate_req.action = SENSOR_HUB_ACTIVATE;
	req.activate_req.enable = en;
	len = sizeof(req.activate_req);
	res = SCP_sensorHub_req_send(&req, &len, 1);
#else //#ifdef CUSTOM_KERNEL_SENSORHUB

	if (!ls9801i_T_obj)
	{
		APS_ERR("ls9801i_T_obj is null!!\n");
		return -1;
	}

	res = ls9801i_T_enable_ps(ls9801i_T_obj->client, en);

	if (res)
	{
		APS_ERR("als_enable_nodata is failed!!\n");
		return -1;
	}

#endif //#ifdef CUSTOM_KERNEL_SENSORHUB
	return 0;
}

static int ps_set_delay(u64 ns)
{
	return 0;
}

static int ps_batch(int flag, int64_t samplingPeriodNs, int64_t maxBatchReportLatencyNs)
{
	return 0;
}

static int ps_flush(void)
{
	return ps_flush_report();
}

static int ps_get_data(int* value, int* status)
{
	int err = 0;
#ifdef CUSTOM_KERNEL_SENSORHUB
	SCP_SENSOR_HUB_DATA req;
	int len;
#endif //#ifdef CUSTOM_KERNEL_SENSORHUB
#ifdef CUSTOM_KERNEL_SENSORHUB
	req.get_data_req.sensorType = ID_PROXIMITY;
	req.get_data_req.action = SENSOR_HUB_GET_DATA;
	len = sizeof(req.get_data_req);
	err = SCP_sensorHub_req_send(&req, &len, 1);

	if (err)
	{
		APS_ERR("SCP_sensorHub_req_send fail!\n");
	}
	else
	{
		*value = req.get_data_rsp.int16_Data[0];
		*status = SENSOR_STATUS_ACCURACY_MEDIUM;
	}

	if (atomic_read(&ls9801i_T_obj->trace) & CMC_TRC_PS_DATA)
	{
		APS_LOG("value = %d\n", *value);
		//show data
	}

#else //#ifdef CUSTOM_KERNEL_SENSORHUB

	if (!ls9801i_T_obj)
	{
		APS_ERR("ls9801i_T_obj is null!!\n");
		return -1;
	}
    APS_LOG("ls9801i_T_ps_get_data %d\n");

	if ((err = ls9801i_T_read_ps(ls9801i_T_obj->client, &ls9801i_T_obj->ps)))
	{
		err = -1;;
	}
	else
	{
		*value = ls9801i_T_get_ps_value(ls9801i_T_obj, ls9801i_T_obj->ps);
		*status = SENSOR_STATUS_ACCURACY_MEDIUM;
	}

#endif //#ifdef CUSTOM_KERNEL_SENSORHUB
	return 0;
}


static int ls9801i_T_als_factory_enable_sensor(bool enable_disable, int64_t sample_periods_ms)
{
	int err = 0;
	APS_FUN();
	
	err = als_enable_nodata(enable_disable ? 1 : 0);
	if (err) {
		APS_ERR("%s:%s failed\n", __func__, enable_disable ? "enable" : "disable");
		return -1;
	}
	err = als_batch(0, sample_periods_ms * 1000000, 0);
	if (err) {
		APS_ERR("%s set_batch failed\n", __func__);
		return -1;
	}
	return 0;
}

static int ls9801i_T_als_factory_get_data(int32_t *data)
{
	int status;
	
	APS_FUN();
	return als_get_data(data, &status);
}

static int ls9801i_T_als_factory_get_raw_data(int32_t *data)
{
	int err = 0;
	struct ls9801i_T_priv *obj = ls9801i_T_obj;

	APS_FUN();

	if (!obj) {
		APS_ERR("obj is null!!\n");
		return -1;
	}

	err = ls9801i_T_read_als(obj->client, &obj->als);
	if (err) {
		APS_ERR("%s failed\n", __func__);
		return -1;
	}
	*data = ls9801i_T_obj->als;

	return 0;
}
static int ls9801i_T_als_factory_enable_calibration(void)
{
	APS_FUN();
	return 0;
}
static int ls9801i_T_als_factory_clear_cali(void)
{
	APS_FUN();
	return 0;
}
static int ls9801i_T_als_factory_set_cali(int32_t offset)
{
	APS_FUN();
	return 0;
}
static int ls9801i_T_als_factory_get_cali(int32_t *offset)
{
	APS_FUN();
	return 0;
}
static int ls9801i_T_ps_factory_enable_sensor(bool enable_disable, int64_t sample_periods_ms)
{
	int err = 0;
	
	APS_FUN();
	err = ps_enable_nodata(enable_disable ? 1 : 0);
	if (err) {
		APS_ERR("%s:%s failed\n", __func__, enable_disable ? "enable" : "disable");
		return -1;
	}
	err = ps_batch(0, sample_periods_ms * 1000000, 0);
	if (err) {
		APS_ERR("%s set_batch failed\n", __func__);
		return -1;
	}
	return err;
}
static int ls9801i_T_ps_factory_get_data(int32_t *data)
{
	int err = 0, status = 0;

	APS_FUN();
	err = ps_get_data(data, &status);
	if (err < 0)
		return -1;
	return 0;
}
static int ls9801i_T_ps_factory_get_raw_data(int32_t *data)
{
	int err = 0;
	struct ls9801i_T_priv *obj = ls9801i_T_obj;

	APS_FUN();
	err = ls9801i_T_read_ps(obj->client, &obj->ps);
	if (err) {
		APS_ERR("%s failed\n", __func__);
		return -1;
	}
	*data = ls9801i_T_obj->ps;
	return 0;
}
static int ls9801i_T_ps_factory_enable_calibration(void)
{
	APS_FUN();
	return 0;
}
static int ls9801i_T_ps_factory_clear_cali(void)
{
	struct ls9801i_T_priv *obj = ls9801i_T_obj;

	APS_FUN();
	obj->ps_cali = 0;
	return 0;
}
static int ls9801i_T_ps_factory_set_cali(int32_t offset)
{
	struct ls9801i_T_priv *obj = ls9801i_T_obj;

	APS_FUN();
	obj->ps_cali = offset;
	return 0;
}
static int ls9801i_T_ps_factory_get_cali(int32_t *offset)
{
	struct ls9801i_T_priv *obj = ls9801i_T_obj;

	APS_FUN();
	*offset = obj->ps_cali;
	return 0;
}

static struct alsps_factory_fops ls9801i_T_factory_fops = {
	.als_enable_sensor = ls9801i_T_als_factory_enable_sensor,
	.als_get_data = ls9801i_T_als_factory_get_data,
	.als_get_raw_data = ls9801i_T_als_factory_get_raw_data,
	.als_enable_calibration = ls9801i_T_als_factory_enable_calibration,
	.als_clear_cali = ls9801i_T_als_factory_clear_cali,
	.als_set_cali = ls9801i_T_als_factory_set_cali,
	.als_get_cali = ls9801i_T_als_factory_get_cali,

	.ps_enable_sensor = ls9801i_T_ps_factory_enable_sensor,
	.ps_get_data = ls9801i_T_ps_factory_get_data,
	.ps_get_raw_data = ls9801i_T_ps_factory_get_raw_data,
	.ps_enable_calibration = ls9801i_T_ps_factory_enable_calibration,
	.ps_clear_cali = ls9801i_T_ps_factory_clear_cali,
	.ps_set_cali = ls9801i_T_ps_factory_set_cali,
	.ps_get_cali = ls9801i_T_ps_factory_get_cali,
};

static struct alsps_factory_public ls9801i_T_factory_device = {
	.gain = 1,
	.sensitivity = 1,
	.fops = &ls9801i_T_factory_fops,
};

static int ls9801i_T_init_reg(struct i2c_client *client)
{
	struct ls9801i_T_priv *obj;

	obj = ls9801i_T_obj;
	client->addr = 0x54;
	printk("ls9801i_T_init_reg client->addr======%x\n", client->addr);
	obj->client = client;
	i2c_set_clientdata(client, obj);
	atomic_set(&obj->als_debounce, 200);
	atomic_set(&obj->als_deb_on, 0);
	atomic_set(&obj->als_deb_end, 0);
	atomic_set(&obj->ps_debounce, 10);
	atomic_set(&obj->ps_deb_on, 0);
	atomic_set(&obj->ps_deb_end, 0);
	atomic_set(&obj->ps_mask, 0);
	atomic_set(&obj->trace, 0x00);
	atomic_set(&obj->als_suspend, 0);
	atomic_set(&obj->init_done, 0);
	obj->irq_node = client->dev.of_node;
	atomic_set(&obj->state_val, 0);

	obj->first_boot = true;
	atomic_set(&obj->ps_high_thd_val, obj->hw->ps_threshold_high);
	atomic_set(&obj->ps_low_thd_val, obj->hw->ps_threshold_low);
	atomic_set(&obj->recv_reg, 0);

	return 0;
}
/*----------------------------------------------------------------------------*/
static int ls9801i_T_i2c_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	struct ls9801i_T_priv *obj;
	int err = 0;
	struct als_control_path als_ctl = { 0 };
	struct als_data_path als_data = { 0 };
	struct ps_control_path ps_ctl = { 0 };
	struct ps_data_path ps_data = { 0 };

	if (!(obj = kzalloc(sizeof(*obj), GFP_KERNEL)))
	{
		err = -ENOMEM;
		goto exit;
	}

	memset(obj, 0, sizeof(*obj));
	err = get_alsps_dts_func(client->dev.of_node, hw);
	if (err < 0)
	{
		APS_ERR("get customization info from dts failed\n");
		goto exit_init_failed;
	}
	ls9801i_T_obj = obj;
	obj->hw = hw;
	ls9801i_T_get_addr(&obj->addr);
	INIT_DELAYED_WORK(&obj->eint_work, ls9801i_T_eint_work);
	printk("ls9801i_T client->addr======%x\n",client->addr);
	ls9801i_T_init_reg(client);

#ifdef CTTRACKING
	obj->ps_thd_update = false;
#endif
#ifdef LS9801I_T_TUNE0
	obj->crosstalk = 0;
	obj->ls9801i_T_max_min_diff = LS9801I_T_MAX_MIN_DIFF;
	obj->ls9801i_T_lt_n_ct = LS9801I_T_LT_N_CT;
	obj->ls9801i_T_ht_n_ct = LS9801I_T_HT_N_CT;
#ifdef CALI_EVERY_TIME
	obj->ps_high_thd_boot = obj->hw->ps_threshold_high;
	obj->ps_low_thd_boot = obj->hw->ps_threshold_low;

	if (obj->ps_high_thd_boot <= 0)
	{
		obj->ps_high_thd_boot = obj->ls9801i_T_ht_n_ct * 3;
		obj->ps_low_thd_boot = obj->ls9801i_T_lt_n_ct * 3;
	}

#endif
#endif

	if (obj->hw->polling_mode_ps == 0)
	{
		APS_LOG("%s: enable PS interrupt\n", __FUNCTION__);
	}else{
		APS_LOG("%s: enable PS polling\n", __FUNCTION__);
	}

	obj->irq_node = client->dev.of_node;//of_find_compatible_node(NULL, NULL, "mediatek, ALS-eint");
	
	obj->als_cali = 1;
	obj->enable = 0;
	obj->pending_intr = 0;
	obj->als_level_num = sizeof(obj->hw->als_level) / sizeof(obj->hw->als_level[0]);
	obj->als_value_num = sizeof(obj->hw->als_value) / sizeof(obj->hw->als_value[0]);
	BUG_ON(sizeof(obj->als_level) != sizeof(obj->hw->als_level));
	memcpy(obj->als_level, obj->hw->als_level, sizeof(obj->als_level));
	BUG_ON(sizeof(obj->als_value) != sizeof(obj->hw->als_value));
	memcpy(obj->als_value, obj->hw->als_value, sizeof(obj->als_value));
	atomic_set(&obj->i2c_retry, 5);

	ls9801i_T_i2c_client = client;
	obj->ls9801i_T_ps_tune0_wq = create_singlethread_workqueue("ls9801i_T_ps_tune0_wq");
	INIT_WORK(&obj->ls9801i_T_ps_tune0_work, ls9801i_T_ps_tune0_work_func);
	hrtimer_init(&obj->ps_tune0_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
#ifdef LS9801I_T_TUNE0
	obj->ps_tune0_delay = ns_to_ktime(60 * NSEC_PER_MSEC);
	obj->ps_tune0_timer.function = ls9801i_T_ps_tune0_timer_func;
#endif

	if ((err = ls9801i_T_init_client(client)))
	{
		goto exit_init_failed;
	}

	APS_LOG("ls9801i_T_init_client() OK!\n");
	err = alsps_factory_device_register(&ls9801i_T_factory_device);
	if(err)
	{
		APS_ERR("ls9801i_T_device register failed\n");
		goto exit_misc_device_register_failed;
	}

	err = ls9801i_T_create_attr(&(ls9801i_T_init_info.platform_diver_addr->driver));
		//if((err = ls9801i_T_create_attr(&ls9801i_T_alsps_driver.driver)))
	if (err)
	{
		APS_ERR("create attribute err = %d\n", err);
		goto exit_create_attr_failed;
	}

	als_ctl.open_report_data = als_open_report_data;
	als_ctl.enable_nodata = als_enable_nodata;
	als_ctl.set_delay = als_set_delay;
	als_ctl.batch = als_batch;
	als_ctl.flush = als_flush;
	als_ctl.is_report_input_direct = false;
	als_ctl.is_use_common_factory = false;

	if (1 == obj->hw->polling_mode_als)
	{
		als_ctl.is_polling_mode = true;
	}
	else
	{
		als_ctl.is_polling_mode = false;
	}

#ifdef CUSTOM_KERNEL_SENSORHUB
	als_ctl.is_support_batch = obj->hw.is_batch_supported_als;
#else
	als_ctl.is_support_batch = false;
#endif
	err = als_register_control_path(&als_ctl);

	if (err)
	{
		APS_ERR("als_control register fail = %d\n", err);
		goto exit_sensor_obj_attach_fail;
	}

	als_data.get_data = als_get_data;
	als_data.vender_div = 100;
	err = als_register_data_path(&als_data);

	if (err)
	{
		APS_ERR("als_data register fail = %d\n", err);
		goto exit_sensor_obj_attach_fail;
	}

	ps_ctl.open_report_data = ps_open_report_data;
	ps_ctl.enable_nodata = ps_enable_nodata;
	ps_ctl.set_delay = ps_set_delay;
	ps_ctl.batch = ps_batch;
	ps_ctl.flush = ps_flush;

	ps_ctl.is_use_common_factory = false;
	ps_ctl.is_support_batch = false;

	if (1 == obj->hw->polling_mode_ps)
	{
		ps_ctl.is_polling_mode = true;
		ps_ctl.is_report_input_direct = false;
	}
	else
	{
		ps_ctl.is_polling_mode = false;
		ps_ctl.is_report_input_direct = true;
	}

#ifdef CUSTOM_KERNEL_SENSORHUB
	ps_ctl.is_support_batch = obj->hw.is_batch_supported_ps;
#else
	ps_ctl.is_support_batch = false;
#endif
	err = ps_register_control_path(&ps_ctl);
	if (err)
	{
		APS_ERR("ps_control register fail = %d\n", err);
		goto exit_sensor_obj_attach_fail;
	}

	ps_data.get_data = ps_get_data;
	ps_data.vender_div = 100;
	err = ps_register_data_path(&ps_data);
	if (err)
	{
		APS_ERR("ps_data register fail = %d\n", err);
		goto exit_sensor_obj_attach_fail;
	}

	ls9801i_T_init_flag = 0;
	alsps_init_status = 1;
	APS_LOG("ls9801i_T_i2c_probe() OK!\n");
	APS_LOG("%s: OK\n", __FUNCTION__);
	return 0;
exit_sensor_obj_attach_fail:
exit_create_attr_failed:
	//misc_deregister(&ls9801i_T_device);
exit_misc_device_register_failed:
exit_init_failed:
	//i2c_detach_client(client);
	//	exit_kfree:
	kfree(obj);
exit:
	ls9801i_T_i2c_client = NULL;
	ls9801i_T_init_flag = -1;
	APS_ERR("%s: err = %d\n", __FUNCTION__, err);
	return err;
	}
/*----------------------------------------------------------------------------*/
static int ls9801i_T_i2c_remove(struct i2c_client *client)
{
	int err;
#ifdef LS9801I_T_TUNE0
	struct ls9801i_T_priv *obj = i2c_get_clientdata(client);
	destroy_workqueue(obj->ls9801i_T_ps_tune0_wq);
#endif
	err = ls9801i_T_delete_attr(&(ls9801i_T_init_info.platform_diver_addr->driver));
	if (err)
	{
		APS_ERR("ls9801i_T_delete_attr fail: %d\n", err);
	}

	alsps_factory_device_deregister(&ls9801i_T_factory_device);


	ls9801i_T_i2c_client = NULL;
	i2c_unregister_device(client);
	kfree(i2c_get_clientdata(client));
	return 0;
}
/*----------------------------------------------------------------------------*/
static int  ls9801i_T_local_uninit(void)
{
	APS_FUN();
	//ls9801i_T_power(hw, 0);
	i2c_del_driver(&ls9801i_T_i2c_driver);
	ls9801i_T_i2c_client = NULL;
	return 0;
}
/*----------------------------------------------------------------------------*/
static int ls9801i_T_local_init(void)
{
	APS_FUN();
	mdelay(10);
	if (i2c_add_driver(&ls9801i_T_i2c_driver))
	{
		APS_ERR("add driver error\n");
		return -1;
	}

	if (-1 == ls9801i_T_init_flag)
	{
		APS_ERR("ls9801i_T_local_init fail with ls9801i_T_init_flag=%d\n", ls9801i_T_init_flag);
		return -1;
	}

	return 0;
}
/*----------------------------------------------------------------------------*/
static int __init ls9801i_T_init(void)
{
	APS_FUN();
	printk("ls9801i_T_init In \n");
	alsps_driver_add(&ls9801i_T_init_info);
	printk("ls9801i_T_init Out \n");
	return 0;
}
/*----------------------------------------------------------------------------*/
static void __exit ls9801i_T_exit(void)
{
	APS_FUN();
}
/*----------------------------------------------------------------------------*/
module_init(ls9801i_T_init);
module_exit(ls9801i_T_exit);
/*----------------------------------------------------------------------------*/
MODULE_AUTHOR("LEVELEK");
MODULE_DESCRIPTION("LEVELEK ls9801i_T proximity and light sensor driver");
MODULE_LICENSE("GPL");
