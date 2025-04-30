/******************** (C) COPYRIGHT 2024 MEMS ********************
 *
 * File Name          : msa321.c
 * Description        : MSA321 accelerometer sensor API
 *
 *******************************************************************************
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * THE PRESENT SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES
 * OR CONDITIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED, FOR THE SOLE
 * PURPOSE TO SUPPORT YOUR APPLICATION DEVELOPMENT.
 * AS A RESULT, MEMSIC SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
 * INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
 * CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
 * INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 *
 * THIS SOFTWARE IS SPECIFICALLY DESIGNED FOR EXCLUSIVE USE WITH MEMSIC PARTS.
 *

 ******************************************************************************/

#include <linux/uaccess.h>
#include <sensors_io.h>

#include "accel.h"
#include "cust_acc.h"
#include "msa321.h"
#ifdef XUNHU_LPS_TEKHW_SUPPORT
#include <teksunhw.h>
#endif
//#include <math.h>

#define I2C_DRIVERID_MSA321		120
#define SW_CALIBRATION
#define DRIVER_VERSION				"1.00.20132"

#define GSE_DEBUG_ON          		1
#define GSE_DEBUG_FUNC_ON     		1
/* Log define */
#define GSE_INFO(fmt, arg...)      	pr_warn("<<-GSE INFO->> "fmt"\n", ##arg)
#define GSE_ERR(fmt, arg...)          	pr_err("<<-GSE ERROR->> "fmt"\n", ##arg)
#define GSE_DEBUG(fmt, arg...)		do {\
						if (GSE_DEBUG_ON)\
							pr_warn("<<-GSE DEBUG->> [%d]"fmt"\n", __LINE__, ##arg);\
					} while (0)
#define GSE_DEBUG_FUNC()		do {\
						if (GSE_DEBUG_FUNC_ON)\
							pr_debug("<<-GSE FUNC->> Func:%s@Line:%d\n", __func__, __LINE__);\
					} while (0)

#define DRIVER_ATTR(_name, _mode, _show, _store) \
  	struct driver_attribute driver_attr_##_name = __ATTR(_name, _mode, \
  			_show, _store)
						
#define MSA321_AXIS_X          	0
#define MSA321_AXIS_Y          	1
#define MSA321_AXIS_Z          	2
#define MSA321_AXES_NUM        	3
#define MSA321_DATA_LEN        	6
#define C_MAX_FIR_LENGTH 		(32)
#define  USE_DELAY
#define MSA321_BUF_SIZE                256
static s16 cali_sensor_data;
static struct acc_hw accel_cust;
static struct acc_hw *hw = &accel_cust;
#ifdef XUNHU_LPS_TEKHW_SUPPORT
extern int accel_compatible_flag;
#endif
#ifdef USE_DELAY
static int delay_state = 0;
#endif

static int z_offset,x_offset,y_offset;

static const struct i2c_device_id msa321_i2c_id[] = { { MSA321_DEV_NAME, 0 }, { }, };
//static int msa321_i2c_probe(struct i2c_client *client);
//static void msa321_i2c_remove(struct i2c_client *client);

static int msa321_i2c_remove(struct i2c_client *client);
static int msa321_i2c_probe(struct i2c_client *client, const struct i2c_device_id *id);

static int  msa321_local_init(void);
static int msa321_remove(void);
static int msa321_factory_set_cali(int32_t data[3]);

typedef enum {
	ADX_TRC_FILTER   = 0x01,
	ADX_TRC_RAWDATA  = 0x02,
	ADX_TRC_IOCTL	 = 0x04,
	ADX_TRC_CALI	 = 0X08,
	ADX_TRC_INFO	 = 0X10,
} ADX_TRC;

struct scale_factor{
	u8  whole;
	u8  fraction;
};

struct data_resolution {
	struct scale_factor scalefactor;
	int    sensitivity;
};


struct data_filter {
	s16 raw[C_MAX_FIR_LENGTH][MSA321_AXES_NUM];
	int sum[MSA321_AXES_NUM];
	int num;
	int idx;
};
static int msa321_init_flag = -1;
/*----------------------------------------------------------------------------*/
static struct acc_init_info msa321_init_info = {
		.name = "msa321",
		.init = msa321_local_init,
		.uninit = msa321_remove,
};
struct msa321_i2c_data {
		 struct i2c_client *client;
		 struct acc_hw *hw;
		 struct hwmsen_convert	 cvt;
		 atomic_t layout;
		 /*misc*/
		 struct data_resolution *reso;
		 atomic_t				 trace;
		 atomic_t				 suspend;
		 atomic_t				 selftest;
		 atomic_t				 filter;
		 s16					 cali_sw[MSA321_AXES_NUM+1];

		 /*data*/
		 s8 					 offset[MSA321_AXES_NUM+1];  /*+1: for 4-byte alignment*/
		 s16					 data[MSA321_AXES_NUM+1];
		 bool                    first_boot;
};

#ifdef CONFIG_OF
static const struct of_device_id accel_of_match[] = {
	{.compatible = "mediatek,gsensor_msa321"},
	{.compatible = "mediatek,gsensor"},
	{},
};
#endif

static struct i2c_driver msa321_i2c_driver = {
	 .driver = {
		 .name			 = MSA321_DEV_NAME,
	   #ifdef CONFIG_OF
		.of_match_table = accel_of_match,
	   #endif
	 },
	 .probe 			 = msa321_i2c_probe,
	 .remove			 = msa321_i2c_remove,
	 .id_table = msa321_i2c_id,
};

struct i2c_client      			*msa321_i2c_client = NULL;
static struct msa321_i2c_data 		*obj_i2c_data = NULL;
static bool sensor_power = false;
static struct GSENSOR_VECTOR3D	gsensor_gain;
static struct mutex msa321_mutex;
static bool enable_status = false;

static struct data_resolution msa321_data_resolution[] = {
    {{ 0, 9}, 1024},   /*+/-2g  in 12-bit resolution:  0.9 mg/LSB*/
    {{ 1, 9}, 512},   /*+/-4g  in 12-bit resolution:  1.9 mg/LSB*/
    {{ 3, 9},  256},   /*+/-8g  in 12-bit resolution: 3.9 mg/LSB*/
};
static struct data_resolution msa321_offset_resolution = {{3, 9}, 256};

static int msa321_i2c_read_block(struct i2c_client *client, u8 addr, u8 *data, u8 len)
{
	u8 beg = addr;
	struct i2c_msg msgs[2] = {
		{
			.addr = client->addr,	.flags = 0,
			.len = 1,	.buf = &beg
		},
		{
			.addr = client->addr,	.flags = I2C_M_RD,
			.len = len,	.buf = data,
		}
	};
	int err;

	if (!client)
		return -EINVAL;
	else if (len > C_I2C_FIFO_SIZE) {
		GSE_ERR(" length %d exceeds %d\n", len, C_I2C_FIFO_SIZE);
		return -EINVAL;
	}

	err = i2c_transfer(client->adapter, msgs, sizeof(msgs)/sizeof(msgs[0]));
	if (err != 2) {
		GSE_ERR("i2c_transfer error: (%d %p %d) %d\n",
				addr, data, len, err);
		err = -EIO;
	} else {
		err = 0;
	}
	return err;

}
static void msa321_power(struct acc_hw *hw, unsigned int on)
{
	 //static unsigned int power_on = 0;

	 //power_on = on;
}

static int MSA321_SaveData(int buf[MSA321_AXES_NUM])
{

	mutex_lock(&msa321_mutex);
	cali_sensor_data = buf[2];

	mutex_unlock(&msa321_mutex);

    return 0;
}
/*
static int MSA321_ReadTemp(struct i2c_client *client, s8 *data)
{
	u8 addr = MSA321_REG_TEMP;
	int err = 0;
	s8 temp = 0;

	if(NULL == client)
	{
		GSE_ERR("client is null\n");
		err = -EINVAL;
	}
	if((err = msa321_i2c_read_block(client, addr, &temp, 1)))
	{
		GSE_ERR("error: %d\n", err);
	}
	*data = temp;

	return err;
}
*/

static int MSA321_ReadDant(struct i2c_client *client, int dant[MSA321_AXES_NUM+1])
{
	u8 addr = MSA321_REG_X;
	u8 buf[MSA321_DATA_LEN] = {0};
	int err = 0;

	if(NULL == client)
	{
        	GSE_ERR("client is null\n");
		err = -EINVAL;
	}
	if((err = msa321_i2c_read_block(client, addr, buf, MSA321_DATA_LEN)))
	{
		GSE_ERR("error: %d\n", err);
	}
	else
    {
		dant[MSA321_AXIS_X] = (s16)((s16)buf[1] << 8 | (s16)buf[0]) >> 4;
		dant[MSA321_AXIS_Y] = (s16)((s16)buf[3] << 8 | (s16)buf[2]) >> 4;
		dant[MSA321_AXIS_Z] = (s16)((s16)buf[5] << 8 | (s16)buf[4]) >> 4;
		//dant[MSA321_AXIS_Z+1] = (s16)buf[6];
	}

	return err;
}
static int MSA321_SetDataResolution(struct msa321_i2c_data *obj)
{
 	obj->reso = &msa321_data_resolution[2];
	return MSA321_SUCCESS;
}

int msa_temp_calibrate_detect_static(short x, short y, short z)
{
	static int count_static=0;
	static short temp_x[TEMP_CALIBRATE_STATIC_COUNT];
	static short temp_y[TEMP_CALIBRATE_STATIC_COUNT];
	static short temp_z[TEMP_CALIBRATE_STATIC_COUNT];
	static short max_x=0,max_y=0,max_z=0;
	static short min_x=0,min_y=0,min_z=0;
	static char is_first=1;
	int i,delta_sum=0;
	
	count_static++;
	
	if(is_first)    
	{
		temp_x[0] = x;
		temp_y[0] = y;
		temp_z[0] = z;
		for (i=1;i<TEMP_CALIBRATE_STATIC_COUNT;i++)
		{
		temp_x[i] = temp_x[0];
		temp_y[i] = temp_y[0];
		temp_z[i] = temp_z[0];
		}
		is_first = 0;
	}
	else
	{       
		max_x = min_x = temp_x[0];
		max_y = min_y = temp_y[0];
		max_z = min_z = temp_z[0];
		
		for (i=0;i<TEMP_CALIBRATE_STATIC_COUNT-1;i++)
		{
			temp_x[i] = temp_x[i+1];
			temp_y[i] = temp_y[i+1];
			temp_z[i] = temp_z[i+1];
			
			max_x = (max_x > temp_x[i]) ? max_x:temp_x[i];
			max_y = (max_y > temp_y[i]) ? max_y:temp_y[i];
			max_z = (max_z > temp_z[i]) ? max_z:temp_z[i];
			
			min_x = (min_x < temp_x[i]) ? min_x:temp_x[i];
			min_y = (min_y < temp_y[i]) ? min_y:temp_y[i];
			min_z = (min_z < temp_z[i]) ? min_z:temp_z[i];
		}
		temp_x[TEMP_CALIBRATE_STATIC_COUNT-1] = x;
		temp_y[TEMP_CALIBRATE_STATIC_COUNT-1] = y;
		temp_z[TEMP_CALIBRATE_STATIC_COUNT-1] = z;
	}

  if(count_static>TEMP_CALIBRATE_STATIC_COUNT)
  {
    count_static = TEMP_CALIBRATE_STATIC_COUNT;
    delta_sum = abs(max_x - min_x) + abs(max_y - min_y) + abs(max_z - min_z);

    if (delta_sum < TEMP_CALIBRATE_STATIC_THRESHOLD)
      return 1;
  }

  return 0;
}

int squareRoot(int val)
{
  int r = 0;
  int shift;

  if (val < 0) {
    return 0;
  }

  for (shift = 0; shift < 32; shift += 2) {
    int x = 0x40000000l >> shift;
    if (x + r <= val) {
      val -= x + r;
      r = (r >> 1) | x;
    } else {
      r = r >> 1;
    }
  }

  return r;
}

int msa_temp_calibrate(int *x, int *y, int *z)
{
  int tem_z = 0;
  int cus = MSA_OFFSET_MAX - MSA_OFFSET_CUS;
  int is_static = 0;
  short lz_offset;

	  if (*x == 0 && *y == 0 && *z == 0) {
		return -1;
	  }
  
    if((x_offset !=0) || (y_offset !=0) || (z_offset !=0))
	{
		*x = *x + x_offset;
		*y = *y + y_offset;
		*z = *z + z_offset;
		return 0;
	}

    lz_offset  =  (*z)%10; 

    if((abs(*x)<MSA_OFFSET_MAX)&&(abs(*y)<MSA_OFFSET_MAX))
    {		
        is_static = msa_temp_calibrate_detect_static(*x,*y,*z-z_offset); 
        tem_z = squareRoot(MSA_OFFSET_SEN*MSA_OFFSET_SEN - (*x)*(*x) - (*y)*(*y)) + lz_offset;
        if(z_offset==0)
        {			  
            if(is_static == 1)
            {
                z_offset = (*z>=0) ? (tem_z-*z) : (-tem_z-*z);
				x_offset = -*x;
				y_offset = -*y;
            }

            *z = ((*z>=0)?(1):(-1))*tem_z;	
        }		
        else if(is_static)
        {
            if(abs(abs(*z)-MSA_OFFSET_SEN)>MSA_OFFSET_CUS)
            {
                *z = ((*z>=0)?(1):(-1))*tem_z;
                z_offset=0;
            }
        }
		
        *z = ((*z>=0)?(1):(-1))*tem_z;
	}
	else if((abs((abs(*x)-MSA_OFFSET_SEN))<MSA_OFFSET_MAX)&&(abs(*y)<MSA_OFFSET_MAX)&&(z_offset))
	{
		if(abs(*x)>MSA_OFFSET_SEN)
		{
		    *x = (*x>0) ? (*x-(abs(*x)-MSA_OFFSET_SEN)*cus/MSA_OFFSET_MAX):(*x+(abs(*x)-MSA_OFFSET_SEN)*cus/MSA_OFFSET_MAX);
		}
		else
		{
		    *x = (*x>0) ? (*x+(MSA_OFFSET_SEN-abs(*x))*cus/MSA_OFFSET_MAX):(*x-(MSA_OFFSET_SEN-abs(*x))*cus/MSA_OFFSET_MAX);
		}
		*y = (*y)*MSA_OFFSET_CUS/MSA_OFFSET_MAX;
	}
	else if((abs((abs(*y)-MSA_OFFSET_SEN))<MSA_OFFSET_MAX)&&(abs(*x)<MSA_OFFSET_MAX)&&(z_offset))
	{
		if(abs(*y)>MSA_OFFSET_SEN)
		{
	   		*y = (*y>0) ? (*y-(abs(*y)-MSA_OFFSET_SEN)*cus/MSA_OFFSET_MAX):(*y+(abs(*y)-MSA_OFFSET_SEN)*cus/MSA_OFFSET_MAX);
		}
		else
		{
			*y = (*y>0) ? (*y+(MSA_OFFSET_SEN-abs(*y))*cus/MSA_OFFSET_MAX):(*y-(MSA_OFFSET_SEN-abs(*y))*cus/MSA_OFFSET_MAX);
		}
		*x = (*x)*MSA_OFFSET_CUS/MSA_OFFSET_MAX;	
	}
	else if(z_offset==0)
	{
	    if((abs(*x)<MSA_OFFSET_MAX)&&(abs((*y > 0) ? (MSA_OFFSET_SEN-*y):(MSA_OFFSET_SEN+*y))<MSA_OFFSET_MAX))
	    {
			*z = ((*z>=0)?(1):(-1))*abs(*x)*MSA_OFFSET_CUS/MSA_OFFSET_MAX;
	    }
		else if((abs(*y)<MSA_OFFSET_MAX)&&(abs((*x > 0) ? (MSA_OFFSET_SEN-*x):(MSA_OFFSET_SEN+*x))<MSA_OFFSET_MAX))
		{
			*z = ((*z>=0)?(1):(-1))*abs(*y)*MSA_OFFSET_CUS/MSA_OFFSET_MAX;
		}
		else
		{
			tem_z = squareRoot(MSA_OFFSET_SEN*MSA_OFFSET_SEN - (*x)*(*x) - (*y)*(*y)) + lz_offset;
			*z = ((*z>=0)?(1):(-1))*tem_z;
		}
	}	

	if(z_offset){
		return 0;
	}
	else
		return -1;
}

static int MSA321_ReadData(struct i2c_client *client, s16 data[MSA321_AXES_NUM])
{
	u8 addr = MSA321_REG_X;
	u8 buf[MSA321_DATA_LEN] = {0};
	int err = 0;
	int tmp_x=0,tmp_y=0,tmp_z=0;

#ifdef USE_DELAY
	if(delay_state)
	{
	    GSE_INFO("sleep 300ms\n");
		msleep(300);
		delay_state = 0;
	}
#endif

	if(NULL == client)
	{
        	GSE_ERR("client is null\n");
		err = -EINVAL;
	}
	if((err = msa321_i2c_read_block(client, addr, buf, MSA321_DATA_LEN))!=0)
	{
		GSE_ERR("error: %d\n", err);
	}
	else
	{
		data[MSA321_AXIS_X] = (s16)(buf[1] << 8 | buf[0]) >> 4;
		data[MSA321_AXIS_Y] = (s16)(buf[3] << 8 | buf[2]) >> 4;
		data[MSA321_AXIS_Z] = (s16)(buf[5] << 8 | buf[4]) >> 4;
		//GSE_ERR("reg data x = %d %d y = %d %d z = %d %d\n", buf[0], buf[1], buf[2], buf[3], buf[4], buf[5]);
               // GSE_ERR("gsensor raw  x = %d, y = %d, z = %d\n", data[MSA321_AXIS_X], data[MSA321_AXIS_Y],data[MSA321_AXIS_Z]);
		tmp_x=data[MSA321_AXIS_X];
		tmp_y=data[MSA321_AXIS_Y];
		tmp_z=data[MSA321_AXIS_Z];					
		msa_temp_calibrate(&tmp_x,&tmp_y,&tmp_z);					
		data[MSA321_AXIS_X] = tmp_x;
		data[MSA321_AXIS_Y] = tmp_y;
		data[MSA321_AXIS_Z] = tmp_z;	   
	}
	return err;
}

/*
static int MSA321_ReadOffset(struct i2c_client *client, s8 ofs[MSA321_AXES_NUM])
{
	int err;
	err = 0;
#ifdef SW_CALIBRATION
	ofs[0]=ofs[1]=ofs[2]=0x0;
#endif
	return err;
}
*/

static int MSA321_ResetCalibration(struct i2c_client *client)
{
	struct msa321_i2c_data *obj = i2c_get_clientdata(client);

	memset(obj->cali_sw, 0x00, sizeof(obj->cali_sw));
	return 0;
}
/*----------------------------------------------------------------------------*/
static int MSA321_ReadCalibration(struct i2c_client *client, int dat[MSA321_AXES_NUM])
{
	struct msa321_i2c_data *obj = i2c_get_clientdata(client);

	dat[obj->cvt.map[MSA321_AXIS_X]] = obj->cvt.sign[MSA321_AXIS_X] * obj->cali_sw[MSA321_AXIS_X];
	dat[obj->cvt.map[MSA321_AXIS_Y]] = obj->cvt.sign[MSA321_AXIS_Y] * obj->cali_sw[MSA321_AXIS_Y];
	dat[obj->cvt.map[MSA321_AXIS_Z]] = obj->cvt.sign[MSA321_AXIS_Z] * obj->cali_sw[MSA321_AXIS_Z];

	return 0;
}

static int MSA321_WriteCalibration(struct i2c_client *client, int dat[MSA321_AXES_NUM])
{
	struct msa321_i2c_data *obj = i2c_get_clientdata(client);
	int err = 0;
	/* int cali[MSA321_AXES_NUM]; */

	if (!obj || !dat) {
		err = -EINVAL;
	} else {
		s16 cali[MSA321_AXES_NUM];

		cali[obj->cvt.map[MSA321_AXIS_X]] = obj->cvt.sign[MSA321_AXIS_X] * obj->cali_sw[MSA321_AXIS_X];
		cali[obj->cvt.map[MSA321_AXIS_Y]] = obj->cvt.sign[MSA321_AXIS_Y] * obj->cali_sw[MSA321_AXIS_Y];
		cali[obj->cvt.map[MSA321_AXIS_Z]] = obj->cvt.sign[MSA321_AXIS_Z] * obj->cali_sw[MSA321_AXIS_Z];
		cali[MSA321_AXIS_X] += dat[MSA321_AXIS_X];
		cali[MSA321_AXIS_Y] += dat[MSA321_AXIS_Y];
		cali[MSA321_AXIS_Z] += dat[MSA321_AXIS_Z];

		obj->cali_sw[MSA321_AXIS_X] += obj->cvt.sign[MSA321_AXIS_X] * dat[obj->cvt.map[MSA321_AXIS_X]];
		obj->cali_sw[MSA321_AXIS_Y] += obj->cvt.sign[MSA321_AXIS_Y] * dat[obj->cvt.map[MSA321_AXIS_Y]];
		obj->cali_sw[MSA321_AXIS_Z] += 0;//obj->cvt.sign[MSA321_AXIS_Z] * dat[obj->cvt.map[MSA321_AXIS_Z]];
	}

	return err;
}
static int MSA321_CheckDeviceID(struct i2c_client *client)
{
	u8 databuf[10];
	int res = 0;

	memset(databuf, 0, sizeof(u8)*10);
	databuf[0] = MSA321_REG_CHIP_ID;
	msleep(12);
	res = msa321_i2c_read_block(client,MSA321_REG_CHIP_ID,databuf,0x01);
	if (res)
	{
		GSE_ERR("MSA321 Device ID read faild\n");
		return MSA321_ERR_I2C;
	}

    GSE_ERR("MSA321 Device ID before is %x\n", databuf[0]);
	databuf[0]= (databuf[0]&0x3f);
    GSE_ERR("MSA321 Device ID is %x\n", databuf[0]);

	if(databuf[0]!= MSA321_ID)
	{
		return MSA321_ERR_IDENTIFICATION;
	}

	GSE_INFO("MSA321_CheckDeviceID %d done!\n ", databuf[0]);

	return MSA321_SUCCESS;
}

static int MSA321_SetPowerMode(struct i2c_client *client, bool enable)
{
	u8 databuf[2] = {0};
	int res = 0, i=0;

	if(enable == 1)
	{
		databuf[1] = MSA321_AWAKE;
	}
	else
	{
		databuf[1] = MSA321_SLEEP;
	}
	msleep(MSA321_STABLE_DELAY);
	databuf[0] = MSA321_REG_PWR;
	while (i++ < 3)
	{
		res = i2c_master_send(client, databuf, 0x2);
		msleep(5);
		if(res > 0)
			break;
	}

	if(res <= 0)
	{
		GSE_ERR("memsic set power mode failed!\n");
		return MSA321_ERR_I2C;
	}
	sensor_power = enable;
#ifdef USE_DELAY
	delay_state = enable;
#else
	msleep(300);
#endif
	return MSA321_SUCCESS;
}
static int MSA321_SetDataFormat(struct i2c_client *client, u8 dataformat)
{
	struct msa321_i2c_data *obj = i2c_get_clientdata(client);
	u8 databuf[10];
	int res = 0;

	memset(databuf, 0, sizeof(u8)*10);
	databuf[0] = MSA321_REG_RANG;
	databuf[1] = MSA321_RANGE_2G;

	res = i2c_master_send(client, databuf, 0x2);
	if(res <= 0)
	{
		GSE_ERR("set power mode failed!\n");
		return MSA321_ERR_I2C;
	}

	return MSA321_SetDataResolution(obj);
}
static int MSA321_SetBWRate(struct i2c_client *client, u8 bwrate)
{
	u8 databuf[10];

	memset(databuf, 0, sizeof(u8)*10);

	return MSA321_SUCCESS;
}

static int msa321_init_client(struct i2c_client *client, int reset_cali)
{
	 struct msa321_i2c_data *obj = i2c_get_clientdata(client);
	 int res = 0;

	 GSE_DEBUG_FUNC();
	 res = MSA321_SetPowerMode(client, true);
	 if(res != MSA321_SUCCESS)
	 {
		return res;
	 }
	 res = MSA321_CheckDeviceID(client);
	 if(res != MSA321_SUCCESS)
	 {
	 	 GSE_ERR("MSA321 check device id failed\n");
	 	 return res;
	 }

	res = MSA321_SetBWRate(client, MSA321_BW_50HZ);
	if(res != MSA321_SUCCESS )
	{
		GSE_ERR("MSA321 Set BWRate failed\n");
		return res;
	}

	res = MSA321_SetDataFormat(client, MSA321_RANGE_2G);
	if(res != MSA321_SUCCESS)
	{
		return res;
	}

	gsensor_gain.x = gsensor_gain.y = gsensor_gain.z = obj->reso->sensitivity;

	if(0 != reset_cali)
	{
	 	/*reset calibration only in power on*/
		 res = MSA321_ResetCalibration(client);
		 if(res != MSA321_SUCCESS)
		 {
			 return res;
		 }
	}
	GSE_INFO("msa321_init_client OK!\n");
	MSA321_SetPowerMode(client, 0);
	msleep(20);

	return MSA321_SUCCESS;
}

static int MSA321_ReadChipInfo(struct i2c_client *client, char *buf, int bufsize)
{
	u8 databuf[10];

	memset(databuf, 0, sizeof(u8)*10);

	if((NULL == buf)||(bufsize<=30))
	{
		return -1;
	}

	if(NULL == client)
	{
		*buf = 0;
		return -2;
	}

	sprintf(buf, "MSA321 Chip");
	return 0;
}

static int MSA321_ReadSensorDataFactory(struct i2c_client *client, char *buf, int bufsize)
{
	struct msa321_i2c_data *obj = (struct msa321_i2c_data*)i2c_get_clientdata(client);
	u8 databuf[20];
	int acc[MSA321_AXES_NUM] = {0};
	int res = 0;

	GSE_DEBUG_FUNC();
	memset(databuf, 0, sizeof(u8)*10);

	if(NULL == buf)
	{
		GSE_ERR("msa321 buf is null !!!\n");
		return -1;
	}
	if(NULL == client)
	{
		*buf = 0;
		GSE_ERR("msa321 client is null !!!\n");
		return MSA321_ERR_STATUS;
	}

	if (atomic_read(&obj->suspend)&& !enable_status)
	{
		GSE_ERR("msa321 sensor in suspend read not data!\n");
		return MSA321_ERR_GETGSENSORDATA;
	}

	if((res = MSA321_ReadData(client, obj->data)))
	{
		GSE_ERR("msa321 I2C error: ret value=%d", res);
		return MSA321_ERR_I2C;
	}
	else
	{
		obj->data[MSA321_AXIS_X] += obj->cali_sw[MSA321_AXIS_X];
		obj->data[MSA321_AXIS_Y] += obj->cali_sw[MSA321_AXIS_Y];
		obj->data[MSA321_AXIS_Z] += obj->cali_sw[MSA321_AXIS_Z];

		/*remap coordinate*/
		acc[obj->cvt.map[MSA321_AXIS_X]] = obj->cvt.sign[MSA321_AXIS_X]*obj->data[MSA321_AXIS_X];
		acc[obj->cvt.map[MSA321_AXIS_Y]] = obj->cvt.sign[MSA321_AXIS_Y]*obj->data[MSA321_AXIS_Y];
		acc[obj->cvt.map[MSA321_AXIS_Z]] = obj->cvt.sign[MSA321_AXIS_Z]*obj->data[MSA321_AXIS_Z];

		//Out put the mg
		acc[MSA321_AXIS_X] = acc[MSA321_AXIS_X] * GRAVITY_EARTH_1000 / obj->reso->sensitivity;
		acc[MSA321_AXIS_Y] = acc[MSA321_AXIS_Y] * GRAVITY_EARTH_1000 / obj->reso->sensitivity;
		acc[MSA321_AXIS_Z] = acc[MSA321_AXIS_Z] * GRAVITY_EARTH_1000 / obj->reso->sensitivity;
		sprintf(buf, "%04x %04x %04x",acc[MSA321_AXIS_X],acc[MSA321_AXIS_Y],cali_sensor_data);

	}
	return res;
}



static int MSA321_ReadSensorData(struct i2c_client *client, int *buf, int bufsize)
{
	struct msa321_i2c_data *obj = (struct msa321_i2c_data*)i2c_get_clientdata(client);
	u8 databuf[20];
	int acc[MSA321_AXES_NUM] = {0};
	int res = 0;

	memset(databuf, 0, sizeof(u8)*10);

	if(NULL == buf)
	{
		GSE_ERR("buf is null !!!\n");
		return -1;
	}
	if(NULL == client)
	{
		*buf = 0;
		GSE_ERR("client is null !!!\n");
		return MSA321_ERR_STATUS;
	}

	if (atomic_read(&obj->suspend) && !enable_status)
	{
		GSE_DEBUG("sensor in suspend read not data!\n");
		return MSA321_ERR_GETGSENSORDATA;
	}


	if((res = MSA321_ReadData(client, obj->data)))
	{
		GSE_ERR("I2C error: ret value=%d", res);
		return MSA321_ERR_I2C;
	}
	else
	{
		obj->data[MSA321_AXIS_X] += obj->cali_sw[MSA321_AXIS_X];
		obj->data[MSA321_AXIS_Y] += obj->cali_sw[MSA321_AXIS_Y];
		obj->data[MSA321_AXIS_Z] += obj->cali_sw[MSA321_AXIS_Z];

		/*remap coordinate*/
		acc[obj->cvt.map[MSA321_AXIS_X]] = obj->cvt.sign[MSA321_AXIS_X]*obj->data[MSA321_AXIS_X];
		acc[obj->cvt.map[MSA321_AXIS_Y]] = obj->cvt.sign[MSA321_AXIS_Y]*obj->data[MSA321_AXIS_Y];
		acc[obj->cvt.map[MSA321_AXIS_Z]] = obj->cvt.sign[MSA321_AXIS_Z]*obj->data[MSA321_AXIS_Z];

		//Out put the mg
		acc[MSA321_AXIS_X] = acc[MSA321_AXIS_X] * GRAVITY_EARTH_1000 / obj->reso->sensitivity;
		acc[MSA321_AXIS_Y] = acc[MSA321_AXIS_Y] * GRAVITY_EARTH_1000 / obj->reso->sensitivity;
		acc[MSA321_AXIS_Z] = acc[MSA321_AXIS_Z] * GRAVITY_EARTH_1000 / obj->reso->sensitivity;
		buf[0] = acc[MSA321_AXIS_X];
		buf[1] = acc[MSA321_AXIS_Y];
		buf[2] = acc[MSA321_AXIS_Z];

	}

	return res;
}

static int MSA321_ReadRawData(struct i2c_client *client, char *buf)
{
	struct msa321_i2c_data *obj = (struct msa321_i2c_data*)i2c_get_clientdata(client);
	int res = 0;

	if (!buf || !client)
	{
        GSE_ERR(" buf or client is null !!\n");
		return EINVAL;
	}

	if((res = MSA321_ReadData(client, obj->data)))
	{
		GSE_ERR("I2C error: ret value=%d\n", res);
		return EIO;
	}
	else
	{
		buf[0] = (int)obj->data[MSA321_AXIS_X];
		buf[1] = (int)obj->data[MSA321_AXIS_Y];
		buf[2] = (int)obj->data[MSA321_AXIS_Z];
	}

	return 0;
}


static ssize_t show_chipinfo_value(struct device_driver *ddri, char *buf)
{
	struct i2c_client *client = msa321_i2c_client;
	char strbuf[MSA321_BUFSIZE];
	if(NULL == client)
	{
		GSE_ERR("i2c client is null!!\n");
		return 0;
	}

	MSA321_ReadChipInfo(client, strbuf, MSA321_BUFSIZE);
	return snprintf(buf, PAGE_SIZE, "%s\n", (char*)strbuf);
}

static ssize_t show_sensordata_value(struct device_driver *ddri, char *buf)
{
	struct i2c_client *client = msa321_i2c_client;
	int strbuf[MSA321_BUFSIZE] = {0};

	if(NULL == client)
	{
		GSE_ERR("i2c client is null!!\n");
		return 0;
	}
	MSA321_ReadSensorData(client, strbuf, MSA321_BUFSIZE);
	return snprintf(buf, PAGE_SIZE, "%s\n", (char*)strbuf);
}

static ssize_t show_cali_value(struct device_driver *ddri, char *buf)
{
	struct i2c_client *client = msa321_i2c_client;
	struct msa321_i2c_data *obj;
	int err, len, mul;
	int tmp[MSA321_AXES_NUM];

	len = 0;

	if (client == NULL) {
		return 0;
	}

	obj = i2c_get_clientdata(client);

	err = MSA321_ReadCalibration(client, tmp);
	if (!err) {
		mul = obj->reso->sensitivity/msa321_offset_resolution.sensitivity;
		len += snprintf(buf+len, PAGE_SIZE-len, "[HW ][%d] (%+3d, %+3d, %+3d) : (0x%02X, 0x%02X, 0x%02X)\n",
			mul,
			obj->offset[MSA321_AXIS_X], obj->offset[MSA321_AXIS_Y], obj->offset[MSA321_AXIS_Z],
			obj->offset[MSA321_AXIS_X], obj->offset[MSA321_AXIS_Y], obj->offset[MSA321_AXIS_Z]);
		len += snprintf(buf+len, PAGE_SIZE-len, "[SW ][%d] (%+3d, %+3d, %+3d)\n", 1,
			obj->cali_sw[MSA321_AXIS_X], obj->cali_sw[MSA321_AXIS_Y], obj->cali_sw[MSA321_AXIS_Z]);

		len += snprintf(buf+len, PAGE_SIZE-len, "[ALL]    (%+3d, %+3d, %+3d) : (%+3d, %+3d, %+3d)\n",
			obj->offset[MSA321_AXIS_X] * mul + obj->cali_sw[MSA321_AXIS_X],
			obj->offset[MSA321_AXIS_Y] * mul + obj->cali_sw[MSA321_AXIS_Y],
			obj->offset[MSA321_AXIS_Z] * mul + obj->cali_sw[MSA321_AXIS_Z],
			tmp[MSA321_AXIS_X], tmp[MSA321_AXIS_Y], tmp[MSA321_AXIS_Z]);

		return len;
	} else
		return -EINVAL;
}
/*----------------------------------------------------------------------------*/
static ssize_t store_cali_value(struct device_driver *ddri, const char *buf, size_t count)
{
	struct i2c_client *client = msa321_i2c_client;
	int err, x, y, z;
	int dat[MSA321_AXES_NUM];

	if (!strncmp(buf, "rst", 3)) {
		err = MSA321_ResetCalibration(client);
		if (err)
			GSE_INFO("reset offset err = %d\n", err);
	} else if (sscanf(buf, "0x%02X 0x%02X 0x%02X", &x, &y, &z) == 3) {
		dat[MSA321_AXIS_X] = x;
		dat[MSA321_AXIS_Y] = y;
		dat[MSA321_AXIS_Z] = z;
		err = MSA321_WriteCalibration(client, dat);
		if (err)
			GSE_INFO("write calibration err = %d\n", err);
	} else
		GSE_INFO("invalid format\n");

	return count;
}

static ssize_t show_firlen_value(struct device_driver *ddri, char *buf)
{
	return snprintf(buf, PAGE_SIZE, "not support\n");
}
/*----------------------------------------------------------------------------*/
static ssize_t store_firlen_value(struct device_driver *ddri, const char *buf, size_t count)
{
	return 0;
}
static ssize_t show_trace_value(struct device_driver *ddri, char *buf)
{
	ssize_t res;
	struct msa321_i2c_data *obj = obj_i2c_data;
	if (obj == NULL)
	{
		GSE_ERR("i2c_data obj is null!!\n");
		return 0;
	}

	res = snprintf(buf, PAGE_SIZE, "0x%04X\n", atomic_read(&obj->trace));
	return res;
}
static ssize_t store_trace_value(struct device_driver *ddri, const char *buf, size_t count)
{
	struct msa321_i2c_data *obj = obj_i2c_data;
	int trace;
	if (obj == NULL)
	{
		GSE_ERR("i2c_data obj is null!!\n");
		return 0;
	}

	if(1 == sscanf(buf, "0x%x", &trace))
	{
		atomic_set(&obj->trace, trace);
	}
	else
	{
		GSE_ERR("invalid content: '%s', length = %d\n", buf, (int)count);
	}
	return count;
}
static ssize_t show_status_value(struct device_driver *ddri, char *buf)
{
	ssize_t len = 0;
	struct msa321_i2c_data *obj = obj_i2c_data;
	if (obj == NULL)
	{
		GSE_ERR("i2c_data obj is null!!\n");
		return 0;
	}

	if(obj->hw)
	{
		len += snprintf(buf+len, PAGE_SIZE-len, "CUST: %d %d (%d %d)\n",
	            obj->hw->i2c_num, obj->hw->direction, obj->hw->power_id, obj->hw->power_vol);
	}
	else
	{
		len += snprintf(buf+len, PAGE_SIZE-len, "CUST: NULL\n");
	}
	return len;
}
/*----------------------------------------------------------------------------*/
static ssize_t show_power_status_value(struct device_driver *ddri, char *buf)
{

	if(sensor_power)
		GSE_INFO("G sensor is in work mode, sensor_power = %d\n", sensor_power);
	else
		GSE_INFO("G sensor is in standby mode, sensor_power = %d\n", sensor_power);

	return snprintf(buf, PAGE_SIZE, "%x\n", sensor_power);
}
static ssize_t show_layout_value(struct device_driver *ddri, char *buf)
{
	struct i2c_client *client = msa321_i2c_client;
	struct msa321_i2c_data *data = i2c_get_clientdata(client);

	return sprintf(buf, "(%d, %d)\n[%+2d %+2d %+2d]\n[%+2d %+2d %+2d]\n",
		data->hw->direction,atomic_read(&data->layout),	data->cvt.sign[0], data->cvt.sign[1],
		data->cvt.sign[2],data->cvt.map[0], data->cvt.map[1], data->cvt.map[2]);
}
/*----------------------------------------------------------------------------*/
static ssize_t store_layout_value(struct device_driver *ddri, const char *buf, size_t count)
{
	struct i2c_client *client = msa321_i2c_client;
	struct msa321_i2c_data *data = i2c_get_clientdata(client);
	int layout = 0;

	if(1 == sscanf(buf, "%d", &layout))
	{
		atomic_set(&data->layout, layout);
		if(!hwmsen_get_convert(layout, &data->cvt))
		{
			GSE_ERR("HWMSEN_GET_CONVERT function error!\r\n");
		}
		else if(!hwmsen_get_convert(data->hw->direction, &data->cvt))
		{
			GSE_ERR("invalid layout: %d, restore to %d\n", layout, data->hw->direction);
		}
		else
		{
			GSE_ERR("invalid layout: (%d, %d)\n", layout, data->hw->direction);
			hwmsen_get_convert(0, &data->cvt);
		}
	}
	else
	{
		GSE_ERR("invalid format = '%s'\n", buf);
	}

	return count;
}
static DRIVER_ATTR(chipinfo,	S_IWUSR | S_IRUGO, show_chipinfo_value,		NULL);
static DRIVER_ATTR(sensordata,	S_IWUSR | S_IRUGO, show_sensordata_value,	NULL);
static DRIVER_ATTR(cali,	S_IWUSR | S_IRUGO, show_cali_value,		store_cali_value);
static DRIVER_ATTR(firlen,	S_IWUSR | S_IRUGO, show_firlen_value,		store_firlen_value);
static DRIVER_ATTR(trace,	S_IWUSR | S_IRUGO, show_trace_value,		store_trace_value);
static DRIVER_ATTR(layout, 	S_IRUGO | S_IWUSR, show_layout_value,		store_layout_value);
static DRIVER_ATTR(status, 	S_IRUGO, show_status_value,			NULL);
static DRIVER_ATTR(powerstatus, S_IRUGO, show_power_status_value,		NULL);

static struct driver_attribute *msa321_attr_list[] = {
	&driver_attr_chipinfo,     /*chip information*/
	&driver_attr_sensordata,   /*dump sensor data*/
	&driver_attr_cali,         /*show calibration data*/
	&driver_attr_firlen,	   /*filter length: 0: disable, others: enable*/
	&driver_attr_trace,		   /*trace log*/
	&driver_attr_layout,
	&driver_attr_status,
	&driver_attr_powerstatus,
};
static int msa321_create_attr(struct device_driver *driver)
{
	int idx, err = 0;
	int num = (int)(sizeof(msa321_attr_list)/sizeof(msa321_attr_list[0]));
	if (driver == NULL)
	{
		return -EINVAL;
	}

	for(idx = 0; idx < num; idx++)
	{
		if((err = driver_create_file(driver, msa321_attr_list[idx])))
		{
			GSE_ERR("driver_create_file (%s) = %d\n", msa321_attr_list[idx]->attr.name, err);
			break;
		}
	}
	return err;
}
static int msa321_delete_attr(struct device_driver *driver)
{
	int idx ,err = 0;
	int num = (int)(sizeof(msa321_attr_list)/sizeof(msa321_attr_list[0]));

	if(driver == NULL)
	{
		return -EINVAL;
	}


	for(idx = 0; idx < num; idx++)
	{
		driver_remove_file(driver, msa321_attr_list[idx]);
	}

	return err;
}
/******************************************************************************
 * Function Configuration
******************************************************************************/
static int msa321_open(struct inode *inode, struct file *file)
{
	 file->private_data = msa321_i2c_client;

	 if(file->private_data == NULL)
	 {
		 GSE_ERR("null msa321!!\n");
		 return -EINVAL;
	 }
	 return nonseekable_open(inode, file);
}
/*----------------------------------------------------------------------------*/
static int msa321_release(struct inode *inode, struct file *file)
{
	 file->private_data = NULL;
	 return 0;
}

#ifdef CONFIG_COMPAT
static long msa321_compat_ioctl(struct file *file, unsigned int cmd,
       unsigned long arg)
{
    long err = 0;

	void __user *arg64 = compat_ptr(arg);

	if (!file->f_op || !file->f_op->unlocked_ioctl)
		return -ENOTTY;

	switch (cmd)
	{
		case COMPAT_GSENSOR_IOCTL_INIT:
			err = file->f_op->unlocked_ioctl(file, GSENSOR_IOCTL_INIT, (unsigned long)arg64);
			if(err < 0)
				{
					GSE_ERR("COMPAT_MMC31XX_IOC_TM is failed!\n");
				}
				break;
		case COMPAT_GSENSOR_IOCTL_READ_CHIPINFO:
			err = file->f_op->unlocked_ioctl(file, GSENSOR_IOCTL_READ_CHIPINFO, (unsigned long)arg64);
			if(err < 0)
				{
					GSE_ERR("COMPAT_GSENSOR_IOCTL_READ_CHIPINFO is failed!\n");
				}
				break;
		case COMPAT_GSENSOR_IOCTL_READ_SENSORDATA:
			err = file->f_op->unlocked_ioctl(file, GSENSOR_IOCTL_READ_SENSORDATA, (unsigned long)arg64);
			if(err < 0)
				{
					GSE_ERR("COMPAT_GSENSOR_IOCTL_READ_SENSORDATA is failed!\n");
				}
				break;
		case COMPAT_GSENSOR_IOCTL_READ_GAIN:
			err = file->f_op->unlocked_ioctl(file, GSENSOR_IOCTL_READ_GAIN, (unsigned long)arg64);
			if(err < 0)
				{
					GSE_ERR("COMPAT_GSENSOR_IOCTL_READ_GAIN is failed!\n");
				}
				break;
		case COMPAT_GSENSOR_IOCTL_READ_RAW_DATA:
			err = file->f_op->unlocked_ioctl(file, GSENSOR_IOCTL_READ_RAW_DATA, (unsigned long)arg64);
			if(err < 0)
				{
					GSE_ERR("COMPAT_GSENSOR_IOCTL_READ_RAW_DATA is failed!\n");
				}
				break;

		case COMPAT_GSENSOR_IOCTL_SET_CALI:
			err = file->f_op->unlocked_ioctl(file, GSENSOR_IOCTL_SET_CALI, (unsigned long)arg64);
			if(err < 0)
				{
					GSE_ERR("COMPAT_GSENSOR_IOCTL_SET_CALI is failed!\n");
				}
				break;
		case COMPAT_GSENSOR_IOCTL_CLR_CALI:
			err = file->f_op->unlocked_ioctl(file, GSENSOR_IOCTL_CLR_CALI, (unsigned long)arg64);
			if(err < 0)
				{
					GSE_ERR("COMPAT_GSENSOR_IOCTL_CLR_CALI is failed!\n");
				}
				break;

		case COMPAT_GSENSOR_IOCTL_GET_CALI:
			err = file->f_op->unlocked_ioctl(file, GSENSOR_IOCTL_GET_CALI, (unsigned long)arg64);
			if(err < 0)
				{
					GSE_ERR("COMPAT_GSENSOR_IOCTL_GET_CALI is failed!\n");
				}
				break;
		default:
			 GSE_ERR("%s not supported = 0x%04x", __FUNCTION__, cmd);
			 return -ENOIOCTLCMD;
			 break;
	}
	return 0;
}
#endif

static long msa321_unlocked_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{

	struct i2c_client *client = (struct i2c_client*)file->private_data;
	struct msa321_i2c_data *obj = (struct msa321_i2c_data*)i2c_get_clientdata(client);
	char strbuf[MSA321_BUFSIZE] = {0};
	void __user *data;
	struct SENSOR_DATA sensor_data;
	int err = 0;
	int cali[3];
	int value[3] = {0};
	int vec[3] = {0};
	//s8 temp = 0 ;
	int dant[4] = {0};
	u8 test=0;
	u8 buf;
	u8 reg[2];
	int temp_flag = 0;


	if(_IOC_DIR(cmd) & _IOC_READ)
	{
		err = !access_ok((void __user *)arg, _IOC_SIZE(cmd));
	}
	else if(_IOC_DIR(cmd) & _IOC_WRITE)
	{
		err = !access_ok((void __user *)arg, _IOC_SIZE(cmd));
	}

	if(err)
	{
		GSE_ERR("access error: %08X, (%2d, %2d)\n", cmd, _IOC_DIR(cmd), _IOC_SIZE(cmd));
		return -EFAULT;
	}
	switch (cmd) {
		case GSENSOR_IOCTL_INIT:
			MSA321_SetPowerMode(client, true);
			break;

		case GSENSOR_IOCTL_READ_CHIPINFO:
			data = (void __user *) arg;
			if(data == NULL)
			{
				err = -EINVAL;
				break;
			}

			MSA321_ReadChipInfo(client, strbuf, MSA321_BUFSIZE);
			if(copy_to_user(data, strbuf, strlen(strbuf)+1))
			{
				err = -EFAULT;
				break;
			}
			break;

		case GSENSOR_IOCTL_READ_SENSORDATA:
			data = (void __user *) arg;
			if(data == NULL)
			{
				err = -EINVAL;
				break;
			}
			if(!sensor_power)
			{
				MSA321_SetPowerMode(client,true);
			}
			MSA321_ReadSensorDataFactory(client, strbuf, MSA321_BUFSIZE);
			if(copy_to_user(data, strbuf, strlen(strbuf)+1))
			{
				err = -EFAULT;
				break;
			}
			break;
		case GSENSOR_IOCTL_READ_GAIN:
			data = (void __user *) arg;
			if(data == NULL)
			{
				err = -EINVAL;
				break;
			}

			if(copy_to_user(data, &gsensor_gain, sizeof(struct GSENSOR_VECTOR3D)))
			{
				err = -EFAULT;
				break;
			}
			break;
		case GSENSOR_IOCTL_READ_RAW_DATA:
			data = (void __user *) arg;
			if(data == NULL)
			{
				err = -EINVAL;
				break;
			}
			MSA321_ReadRawData(client, strbuf);
			if(copy_to_user(data, strbuf, strlen(strbuf)+1))
			{
				err = -EFAULT;
				break;
			}
			break;
		case GSENSOR_IOCTL_SET_CALI:
			data = (void __user*)arg;
			if(data == NULL)
			{
				err = -EINVAL;
				break;
			}
			if(copy_from_user(&sensor_data, data, sizeof(sensor_data)))
			{
				err = -EFAULT;
				break;
			}
			if(atomic_read(&obj->suspend))
			{
				GSE_ERR("Perform calibration in suspend state!!\n");
				err = -EINVAL;
			}
			else
			{
				cali[MSA321_AXIS_X] = sensor_data.x * obj->reso->sensitivity / GRAVITY_EARTH_1000;
				cali[MSA321_AXIS_Y] = sensor_data.y * obj->reso->sensitivity / GRAVITY_EARTH_1000;
				cali[MSA321_AXIS_Z] = sensor_data.z * obj->reso->sensitivity / GRAVITY_EARTH_1000;
				err = MSA321_WriteCalibration(client, cali);
			}
			break;
		case GSENSOR_IOCTL_CLR_CALI:
			err = MSA321_ResetCalibration(client);
			break;
		case GSENSOR_IOCTL_GET_CALI:
			data = (void __user*)arg;
			if(data == NULL)
			{
				err = -EINVAL;
				break;
			}
			if((err = MSA321_ReadCalibration(client, cali)))
			{
				break;
			}

			sensor_data.x = cali[MSA321_AXIS_X] * GRAVITY_EARTH_1000 / obj->reso->sensitivity;
			sensor_data.y = cali[MSA321_AXIS_Y] * GRAVITY_EARTH_1000 / obj->reso->sensitivity;
			sensor_data.z = cali[MSA321_AXIS_Z] * GRAVITY_EARTH_1000 / obj->reso->sensitivity;
			if(copy_to_user(data, &sensor_data, sizeof(sensor_data)))
			{
				err = -EFAULT;
				break;
			}
			break;
		case GSENSOR_IOCTL_GET_DELAY:
			break;
		case GSENSOR_IOCTL_GET_STATUS:
			temp_flag = ((sensor_power << 1) | (enable_status & 0x1));
			data = (void __user *) arg;
			if(data == NULL)
			{
				err = -EINVAL;
				break;
			}
			mutex_lock(&msa321_mutex);
			if(copy_to_user(data, &temp_flag, sizeof(temp_flag)))
			{
				err = -EFAULT;
				temp_flag = 0;
				mutex_unlock(&msa321_mutex);
				break;
			}
			temp_flag = 0;
			mutex_unlock(&msa321_mutex);
			break;
		case GSENSOR_IOCTL_GET_LAYOUT:
			data = (void __user *) arg;
			if(data == NULL)
			{
				err = -EINVAL;
				break;
			}
			if(copy_to_user(data, &(obj->hw->direction), sizeof(obj->hw->direction)))
			{
				err = -EFAULT;
				break;
			}
			break;
		case GSENSOR_IOCTL_GET_DATA:
			data = (void __user *) arg;
			if(data == NULL)
			{
				err = -EINVAL;
				break;
			}

			err = MSA321_ReadSensorData(client, vec,MSA321_AXES_NUM);

			if(copy_to_user(data, vec, sizeof(vec)))
			{
				err = -EFAULT;
				break;
			}
			break;
		case GSENSOR_IOCTL_SET_DATA:
			data = (void __user *) arg;
			if(data == NULL)
			{
				err = -EINVAL;
				break;
			}

			if(copy_from_user(value, data, sizeof(value)))
			{
				err = -EFAULT;
				break;
			}

			MSA321_SaveData(value);
			break;
		case GSENSOR_IOCTL_GET_TEMP:
			/*data = (void __user *) arg;
			if(data == NULL)
			{
				err = -EINVAL;
				break;
			}

			err = MSA321_ReadTemp(client, &temp);

			if(copy_to_user(data, &temp, sizeof(temp)))
			{
				err = -EFAULT;
				break;
			}*/
			break;
		case GSENSOR_IOCTL_GET_DANT:
			data = (void __user *) arg;
			if(data == NULL)
			{
				err = -EINVAL;
				break;
			}
			err = MSA321_ReadDant(client, dant);
			if(copy_to_user(data, dant, sizeof(dant)))
			{
				err = -EFAULT;
				break;
			}
			break;
		case GSENSOR_IOCTL_READ_REG:
			data = (void __user *) arg;
			if(data == NULL)
			{
				err = -EINVAL;
				break;
			}
			if(copy_from_user(&test, data, sizeof(test)))
			{
				err = -EFAULT;
				break;
			}
			if((err = msa321_i2c_read_block(client, test, &buf, 1)))
			{
				GSE_ERR("error: %d\n", err);
			}
			if(copy_to_user(data, &buf, 1))
			{
				err = -EFAULT;
				break;
			}
			break;

		case GSENSOR_IOCTL_WRITE_REG:
			data = (void __user *) arg;
			if(data == NULL)
			{
				err = -EINVAL;
				break;
			}
			if(copy_from_user(reg, data, sizeof(reg)))
			{
				err = -EFAULT;
				break;
			}
			err = i2c_master_send(client, reg, 0x2);
			if(err <= 0)
			{
				GSE_ERR("write reg failed!\n");
				err = -EFAULT;
				break;
			}
			break;
		default:
			GSE_ERR("unknown IOCTL: 0x%08x\n", cmd);
			err = -ENOIOCTLCMD;
			break;
	}

	return err;
}

static struct file_operations msa321_fops = {
		 .owner = THIS_MODULE,
		 .open = msa321_open,
		 .release = msa321_release,
		 .unlocked_ioctl = msa321_unlocked_ioctl,
#ifdef CONFIG_COMPAT
		.compat_ioctl = msa321_compat_ioctl,
#endif
};

static struct miscdevice msa321_device = {
		 .minor = MISC_DYNAMIC_MINOR,
		 .name = "msa321x",
		 .fops = &msa321_fops,
};


// if use  this typ of enable , Gsensor should report inputEvent(x, y, z ,stats, div) to HAL
static int msa321_open_report_data(int open)
{
	//should queuq work to report event if  is_report_input_direct=true
	return 0;
}

#if 0
static void cali_acc_frm_nv(void);
static void cali_acc_frm_nv()
{
    int cali_buff[3];
    int cali_err = 0;
    int ps_cali_offset=2690;

    memset(cali_buff, 0, sizeof(cali_buff));

    cali_err = get_calidata_frm_nvfile(&cali_buff[0], ps_cali_offset);
    if (cali_err < 0)
    {        
        GSE_ERR("cali_acc_frm_nv:read nvdata failed!"); 
        return;    
    }

    GSE_DEBUG("cali_acc_frm_nv is cali_buff[0] = %d, cali_buff[1] = %d, cali_buff[2] = %d",
        cali_buff[0], cali_buff[1], cali_buff[2]);

    msa321_factory_set_cali(cali_buff);
}
#endif
// if use  this typ of enable , Gsensor only enabled but not report inputEvent to HAL

static int msa321_enable_nodata(int en)
{
	int res =0;
	bool power = false;

	if(1==en)
	{
		power = true;
	}
	if(0==en)
	{
		power = false;
	}
#if 0
	if(obj_i2c_data->first_boot)
	{
		GSE_DEBUG("first_boot is true");
        cali_acc_frm_nv();
		obj_i2c_data->first_boot = false;	
	}
#endif
	res = MSA321_SetPowerMode(obj_i2c_data->client, power);
	if(res != MSA321_SUCCESS)
	{
		GSE_ERR("MSA321_SetPowerMode fail!\n");
		return -1;
	}
	GSE_DEBUG("MSA321_enable_nodata OK en = %d sensor_power = %d\n", en, sensor_power);
	enable_status = en;
	return 0;
}

static int msa321_set_delay(u64 ns)
{
	return 0;
}

static int gsensor_batch(int flag, int64_t samplingPeriodNs, int64_t maxBatchReportLatencyNs)
{
	//int value = 0;

	//value = (int)samplingPeriodNs/1000/1000;

	return msa321_set_delay(samplingPeriodNs);
}

static int gsensor_flush(void)
{
	return acc_flush_report();
}

static int msa321_get_data(int* x ,int* y,int* z, int* status)
{
	int buff[MSA321_BUFSIZE] = {0};
	MSA321_ReadSensorData(obj_i2c_data->client, buff, MSA321_BUFSIZE);
	*x = buff[0];
	*y = buff[1];
	*z = buff[2];//cali_sensor_data;
	//printk( "msa321_get_data entry");
	*status = SENSOR_STATUS_ACCURACY_MEDIUM;

	return 0;
}


/*******add factory function***********/
static int msa321_factory_enable_sensor(bool enabledisable, int64_t sample_periods_ms)
{
	int en = (true == enabledisable ? 1 : 0);

	if (msa321_enable_nodata(en))
	{
		GSE_DEBUG("enable sensor failed");
		return -1;
	}

	return 0;
}

static int msa321_factory_get_data(int32_t data[3], int *status)
{
	return msa321_get_data(&data[0], &data[1], &data[2], status);

}

static int msa321_factory_get_raw_data(int32_t data[3])
{

//	MSA321_ReadRawData(msa321_i2c_client, data);
	struct i2c_client *client = msa321_i2c_client;
	char buff[3]= {0};

	printk( "msa321_factory_get_raw_data entry");	
	MSA321_ReadRawData(client, buff);

    data[0] = (int32_t)buff[0];
    data[1] = (int32_t)buff[1];
    data[2] = (int32_t)buff[2];

	return 0;
}
static int msa321_factory_enable_calibration(void)
{
	int i; 
	int sample_no=15;
	struct i2c_client *client = msa321_i2c_client;
	struct msa321_i2c_data *obj;
	s16 buff[3] = {0};
	int axis = 0;
	s16 data[3] = {0};
	int32_t acc_ave[3] = {0};

	obj = i2c_get_clientdata(client);
	for (i = 0; i < sample_no; i++)
	{
		msleep(10);

		MSA321_ReadData(msa321_i2c_client, buff);
		//MSA321_ReadRawData(msa321_i2c_client, buff);

		data[0] = buff[0];
		data[1] = buff[1];
		data[2] = buff[2];
		   
		printk("msa321_factory_enable_calibration raw: %d %d %d",data[0],data[1],data[2]);

		acc_ave[0] += data[0];
		acc_ave[1] += data[1];
		acc_ave[2] += data[2];
	}

	for (i = 0; i < 3; i++)
	{
		acc_ave[i] =(acc_ave[i] + sample_no/2) / sample_no;
	}

	for (axis = 0;axis< 3; axis++)
	{  
		if(axis<=1)
		obj->cali_sw[axis] = -acc_ave[axis];
   		else
   		{
	   		obj->cali_sw[2] = 0;//-(256-acc_ave[2]);
   		}   
	}

    printk( "msa321_factory_enable_calibration offset: %d %d %d",obj->cali_sw[0],obj->cali_sw[1],obj->cali_sw[2]);
	
	
	return 0;
}
static int msa321_factory_clear_cali(void)
{
	int err = 0;

	err = MSA321_ResetCalibration(msa321_i2c_client);
	if (err) {
		return -1;
	}
	return 0;
}
static int msa321_factory_set_cali(int32_t data[3])
{
	struct i2c_client *client = msa321_i2c_client;
	struct msa321_i2c_data *obj;	
	int i = 0;
	obj = i2c_get_clientdata(client);

	GSE_DEBUG("ori0 x:%d, y:%d, z:%d", data[0], data[1], data[2]);
	for(i = 0; i < 3; i++)
	{
		if((data[i]&0x8000) == 0x8000)//¸ºÊý
		{
			data[i] = data[i] - 65536;
		}
	}

	GSE_DEBUG("ori x:%d, y:%d, z:%d", data[0], data[1], data[2]);

	obj->cali_sw[0]  = data[0] * obj->reso->sensitivity / GRAVITY_EARTH_1000;
	obj->cali_sw[1]  = data[1] * obj->reso->sensitivity / GRAVITY_EARTH_1000;
    obj->cali_sw[2]  = data[2] * obj->reso->sensitivity / GRAVITY_EARTH_1000;

	GSE_DEBUG("new x:%d, y:%d, z:%d", obj->cali_sw[0], obj->cali_sw[1], obj->cali_sw[2]);

#if 0
	int err = 0;
	int cali[3] = {0};
	if (atomic_read(&obj_i2c_data->suspend))
	{
		GSE_ERR("Perform calibration in suspend state!!\n");
		err = -EINVAL;
	}
	else
	{
		cali[MSA321_AXIS_X] = data[0] * obj_i2c_data->reso->sensitivity / GRAVITY_EARTH_1000;
		cali[MSA321_AXIS_Y] = data[1] * obj_i2c_data->reso->sensitivity / GRAVITY_EARTH_1000;
		cali[MSA321_AXIS_Z] = data[2] * obj_i2c_data->reso->sensitivity / GRAVITY_EARTH_1000;
		
		err = MSA321_WriteCalibration(msa321_i2c_client, cali);
		if (err)
		{
			return -1;
		}
	}
#endif
	return 0;
}
static int msa321_factory_get_cali(int32_t data[3])
{
	
	struct i2c_client *client = msa321_i2c_client;
	struct msa321_i2c_data *obj;
	obj = i2c_get_clientdata(client);
	
	GSE_DEBUG("orig x:%d, y:%d, z:%d", obj->cali_sw[0], obj->cali_sw[1], obj->cali_sw[2]);
	data[0] = obj->cali_sw[0] * GRAVITY_EARTH_1000 / obj->reso->sensitivity;
	data[1] = obj->cali_sw[1] * GRAVITY_EARTH_1000 / obj->reso->sensitivity;
	data[2] = obj->cali_sw[2] * GRAVITY_EARTH_1000 / obj->reso->sensitivity;
	
	GSE_DEBUG("new x:%d, y:%d, z:%d", data[0], data[1], data[2]);

	return 0;
}

static int msa321_factory_do_self_test(void)
{
	GSE_DEBUG("entry msa321_factory_do_self_test\n");
    return 0;
}

static struct accel_factory_fops msa321_factory_fops = {
	.enable_sensor = msa321_factory_enable_sensor,
	.get_data = msa321_factory_get_data,
	.get_raw_data = msa321_factory_get_raw_data,
	.enable_calibration = msa321_factory_enable_calibration,
	.clear_cali =msa321_factory_clear_cali,
	.set_cali = msa321_factory_set_cali,
	.get_cali = msa321_factory_get_cali,
	.do_self_test = msa321_factory_do_self_test,
};

static struct accel_factory_public msa321_factory_device =
{
    .gain = 1,
    .sensitivity = 1,
    .fops = &msa321_factory_fops,
};

/*----------------------------------------------------------------------------*/
//static int msa321_i2c_probe(struct i2c_client *client)
static int msa321_i2c_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	struct i2c_client *new_client;
	struct msa321_i2c_data *obj;

	int err = 0;

	struct acc_control_path ctl={0};
	struct acc_data_path data={0};
	GSE_DEBUG_FUNC();
	GSE_INFO("driver version = %s\n",DRIVER_VERSION);

	printk("msa321_i2c_probe\n");
	
	err = get_accel_dts_func(client->dev.of_node, hw);
	if (err < 0) {
	
		goto exit;
	}

	if(!(obj = kzalloc(sizeof(*obj), GFP_KERNEL)))
	{
		err = -ENOMEM;
		goto exit;
	}

	memset(obj, 0, sizeof(struct msa321_i2c_data));

	obj->hw = hw;
        //obj->hw->direction = 6;
	if((err = hwmsen_get_convert(obj->hw->direction, &obj->cvt)))
	{
		GSE_ERR("invalid direction: %d\n", obj->hw->direction);
		goto exit;
	}
        GSE_ERR(" direction=%d\n", obj->hw->direction);
	obj_i2c_data = obj;
	client->addr=0x62;
	
	obj->client = client;
	new_client = obj->client;
	i2c_set_clientdata(new_client,obj);

	atomic_set(&obj->trace, 0);
	atomic_set(&obj->suspend, 0);

	msa321_i2c_client = new_client;
	
	if((err = msa321_init_client(new_client, 1)))
	{
		goto exit_init_failed;
	}
#ifdef XUNHU_LPS_TEKHW_SUPPORT
	accel_compatible_flag=1;
#endif
	if((err = misc_register(&msa321_device)))
	{
		GSE_ERR("msa321_device register failed\n");
		printk("msa321_device register failed\n");
		goto exit_misc_device_register_failed;
	}
/* factory */
	err = accel_factory_device_register(&msa321_factory_device);
	if (err) {
		GSE_ERR("acc_factory register failed.\n");
		goto exit_misc_device_register_failed;
	}
/***end factory********/	
	if((err = msa321_create_attr(&msa321_init_info.platform_diver_addr->driver)))
	{
		GSE_ERR("create attribute err = %d\n", err);
		goto exit_create_attr_failed;
	}

	ctl.open_report_data = msa321_open_report_data;
	ctl.enable_nodata = msa321_enable_nodata;
	ctl.set_delay  = msa321_set_delay;
	
	ctl.batch = gsensor_batch;
	ctl.flush = gsensor_flush;
	ctl.is_report_input_direct = false;
#ifdef CUSTOM_KERNEL_SENSORHUB
	ctl.is_support_batch = obj->hw.is_batch_supported;
#else
	ctl.is_support_batch = false;
#endif

	err = acc_register_control_path(&ctl);
	if(err)
	{
		GSE_ERR("register acc control path err\n");
		goto exit_create_attr_failed;
	}

	data.get_data = msa321_get_data;
	data.vender_div = 1000;
	err = acc_register_data_path(&data);
	if(err)
	{
		GSE_ERR("register acc data path err\n");
		goto exit_create_attr_failed;
	}
	msa321_init_flag = 0;
	GSE_INFO("%s: OK\n", __func__);
	return 0;

exit_create_attr_failed:
	misc_deregister(&msa321_device);
exit_misc_device_register_failed:
exit_init_failed:
	kfree(obj);
exit:
	GSE_ERR("%s: err = %d\n", __func__, err);
	msa321_init_flag = -1;
	return err;
}

//static void msa321_i2c_remove(struct i2c_client *client)
static int msa321_i2c_remove(struct i2c_client *client)
{
	 int err = 0;

	 if((err = msa321_delete_attr(&msa321_init_info.platform_diver_addr->driver)))
	 {
		 GSE_ERR("msa321_delete_attr fail: %d\n", err);
	 }

	if(err) {
		misc_deregister(&msa321_device);
	}

	accel_factory_device_deregister(&msa321_factory_device);

	 //if((err = hwmsen_detach(ID_ACCELEROMETER)))

	 msa321_i2c_client = NULL;
	 i2c_unregister_device(client);
	 accel_factory_device_deregister(&msa321_factory_device);
	 kfree(i2c_get_clientdata(client));
	 return 0;
}


static int msa321_local_init(void)
{
	GSE_DEBUG_FUNC();
	msa321_power(hw, 1);

	if(i2c_add_driver(&msa321_i2c_driver))
	{
		 GSE_ERR("add driver error\n");
		 return -1;
	}
	if(-1 == msa321_init_flag)
	{
		GSE_ERR("msa321_local_init failed msa321_init_flag=%d\n",msa321_init_flag);
	   	return -1;
	}
	return 0;
}
static int msa321_remove(void)
{
	 GSE_DEBUG_FUNC();
	 msa321_power(hw, 0);
	 i2c_del_driver(&msa321_i2c_driver);
	 return 0;
}

static int __init msa321_driver_init(void)
{
	printk("msa321_driver_init\n");
	acc_driver_add(&msa321_init_info);

	mutex_init(&msa321_mutex);

	return 0;
}

static void __exit msa321_driver_exit(void)
{
	GSE_DEBUG_FUNC();
	mutex_destroy(&msa321_mutex);
}

module_init(msa321_driver_init);
module_exit(msa321_driver_exit);


MODULE_AUTHOR("LINCHENG TANG<lctang@memsensing.com>");
MODULE_DESCRIPTION("MEMSIC MSA321 Accelerometer Sensor Driver");
MODULE_LICENSE("GPL");
