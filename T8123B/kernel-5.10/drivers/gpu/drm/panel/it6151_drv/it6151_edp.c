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
#include <linux/module.h>
#include <linux/init.h>

#include "it6151_edp.h"

/**********************************************************
  *
  *   [I2C Slave Setting] 
  *
  *********************************************************/
#define IT6151_EDP_SLAVE_ADDR_WRITE					(0x5C << 0)
#define IT6151_MIPIRX_SLAVE_ADDR_WRITE 				(0x6C << 0)
#define IT6151_DEBUG 1

static DEFINE_MUTEX(it6151_i2c_access);
#define DEVICE_NAME "it6151_edp"

#ifdef  IT6151_DEBUG
	#define printf_info(fmt,args...)  \
		do {					   \
			printk("[IT6151_DEBUG]"fmt,##args);				   \
			}while(0)
#else
#define printf_info(fmt,args...)
#endif

static struct i2c_client *it6151_edp_client = NULL;

static const struct i2c_device_id it6151_i2c_id[] = 
{
	{"it6151_edp",		0},
	{}
};
#ifdef CONFIG_OF
static const struct of_device_id it6151_of_match[] = {
        {.compatible = "mediatek,it6151_edp"},
        {},
};
#endif

static int it6151_i2c_driver_probe(struct i2c_client *client, const struct i2c_device_id *id);

static struct i2c_driver it6151_i2c_driver = {
	.probe       = it6151_i2c_driver_probe,
	.id_table    = it6151_i2c_id,
	.driver = 
	{
		.name    = DEVICE_NAME,
		#ifdef CONFIG_OF
		.of_match_table = it6151_of_match,
		#endif
	},

};

/**********************************************************
  *
  *   [I2C Function For Read/Write fan5405] 
  *
  *********************************************************/
#if 1
int it6151_i2c_read_byte(unsigned char dev_addr, unsigned char addr, unsigned char *returnData)
{
	char     cmd_buf[1]={0x00};
	char     readData = 0;
	int      ret=0;
	/* dump write_data for check */
	//printf_info("[KE/it6151_read_byte] dev_addr = 0x%x, read_data[0x%x] = 0x%x \n", dev_addr, addr, *returnData);
	
	if(it6151_edp_client == NULL)
		return 0;
	
	mutex_lock(&it6151_i2c_access);
	if(dev_addr == IT6151_EDP_SLAVE_ADDR_WRITE)
	{
		ret = i2c_smbus_read_i2c_block_data(it6151_edp_client,addr,1,cmd_buf);	
		 if (ret < 0)
		 	printf_info("%s:error: ret=%d! \n",__func__,ret);
		  
		readData = cmd_buf[0];
		*returnData = readData;
	}
	else
	{
		ret = -1;
		printf_info("[it6151_i2c_read_byte]error:  no this dev_addr! \n");
	}
	
	mutex_unlock(&it6151_i2c_access);
	return ret;
}
EXPORT_SYMBOL_GPL(it6151_i2c_read_byte);
#else 

int it6151_i2c_read_byte(unsigned char dev_addr, unsigned char addr, unsigned char *returnData)
{
  char     cmd_buf[1]={0x00};
  char     readData = 0;
  int      ret=0;

#ifdef IT6151_DEBUG
	/* dump write_data for check */
  printk("[KE/it6151_read_byte] dev_addr = 0x%x, read_data[0x%x] = 0x%x \n", dev_addr, addr, *returnData);
#endif

  mutex_lock(&it6151_i2c_access);

	if(dev_addr == IT6151_MIPIRX_SLAVE_ADDR_WRITE)
	{		
		it6151_mipirx->ext_flag=((it6151_mipirx->ext_flag) & I2C_MASK_FLAG ) | I2C_WR_FLAG | I2C_DIRECTION_FLAG;
		
		cmd_buf[0] = addr;
		ret = i2c_master_send(it6151_mipirx, &cmd_buf[0], (1<<8 | 1));
		
		if (ret < 0)
		{
			it6151_mipirx->ext_flag=0;
		
			mutex_unlock(&it6151_i2c_access);
		
			return 0;
		}
		
		readData = cmd_buf[0];
		*returnData = readData;
		
		it6151_mipirx->ext_flag=0;
	}
	else if(dev_addr == IT6151_EDP_SLAVE_ADDR_WRITE)
	{
	  it6151_edp->ext_flag=((it6151_edp->ext_flag) & I2C_MASK_FLAG ) | I2C_WR_FLAG | I2C_DIRECTION_FLAG;

	  cmd_buf[0] = addr;
	  ret = i2c_master_send(it6151_edp, &cmd_buf[0], (1<<8 | 1));
		
	  if (ret < 0)
	  {
	    it6151_edp->ext_flag=0;

	    mutex_unlock(&it6151_i2c_access);

			return 0;
	  }
	  
	  readData = cmd_buf[0];
	  *returnData = readData;

	  it6151_edp->ext_flag=0;
	}
	else
	{
		printk("[it6151_i2c_read_byte]error:  no this dev_addr! \n");
	}
	
  mutex_unlock(&it6151_i2c_access);
	
  return 1;
}
EXPORT_SYMBOL_GPL(it6151_i2c_read_byte);
#endif 
int it6151_i2c_write_byte(unsigned char dev_addr, unsigned char addr, unsigned char writeData)
{
	char    write_data[1] = {0};
	int     ret=0;
	/* dump write_data for check */
	//printf_info("[KE/it6151_i2c_write] dev_addr = 0x%x, write_data[0x%x] = 0x%x \n", dev_addr, addr, writeData);

	if(it6151_edp_client == NULL)
		return 0;
	
	mutex_lock(&it6151_i2c_access);
  	write_data[0] = writeData;
	if(dev_addr == IT6151_EDP_SLAVE_ADDR_WRITE)
	{
		ret = i2c_smbus_write_i2c_block_data(it6151_edp_client,addr,1,write_data);
		if (ret < 0)
			printf_info("%s:error: ret=%d! \n",__func__,ret);
	}	
	else
	{
     		printf_info("[it6151_i2c_write_byte]error:  no this dev_addr! \n");
		ret = -1;
	}
	
	mutex_unlock(&it6151_i2c_access);
	return ret;
}
EXPORT_SYMBOL_GPL(it6151_i2c_write_byte);

#define DP_I2C_ADDR 	(0x5C << 0)
static int it6151_i2c_driver_probe(struct i2c_client *client, const struct i2c_device_id *id) 
{
		unsigned char temp;
	it6151_edp_client = client;

	it6151_i2c_read_byte(DP_I2C_ADDR, 0x22, &temp);	
	printf_info("%s: %s i2c ,addr is 0x%02x!\n",__func__, client->name,client->addr);
  	return 0;
}

static int __init it6151_init(void)
{    
	printf_info(" %s:start\n",__func__);

	i2c_add_driver(&it6151_i2c_driver);
	return 0;
}

static void __exit it6151_exit(void)
{
	i2c_del_driver(&it6151_i2c_driver);
	return;
}

late_initcall(it6151_init);
module_exit(it6151_exit);
   
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("I2C it6151 Driver");
MODULE_AUTHOR("James Lo<james.lo@mediatek.com>");
