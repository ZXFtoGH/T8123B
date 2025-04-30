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

#include "it6151_mipirx.h"

/**********************************************************
  *
  *   [I2C Slave Setting] 
  *
  *********************************************************/
#define IT6151_EDP_SLAVE_ADDR_WRITE					(0x5C << 0)
#define	IT6151_MIPIRX_SLAVE_ADDR_WRITE 				(0x6C << 0)
#define DEVICE_NAME "it6151_mipirx"
#define IT6151_DEBUG 1

#ifdef  IT6151_DEBUG
	#define printf_info(fmt,args...)  \
		do {					   \
			printk("[IT6151_DEBUG]"fmt,##args);				   \
			}while(0)
#else
#define printf_info(fmt,args...)
#endif


static struct i2c_client *it6151_mipirx_client = NULL;
static DEFINE_MUTEX(it6151_i2c_access);

static const struct i2c_device_id it6151_i2c_id[] = 
{
	{"it6151_mipirx",	0},
	{}
};
#ifdef CONFIG_OF
static const struct of_device_id it6151_of_match[] = {
        {.compatible = "mediatek,it6151_mipirx"},
        {},
};
#endif

/**********************************************************
  *
  *   [I2C Function For Read/Write fan5405] 
  *
  *********************************************************/
int it6151_mipirx_i2c_read_byte(unsigned char dev_addr, unsigned char addr, unsigned char *returnData)
{
  char     cmd_buf[1]={0x00};
  char     readData = 0;
  int      ret=0;

	//ump write_data for check 
	//printf_info("[KE/it6151_read_byte] dev_addr = 0x%x, read_data[0x%x] = 0x%x \n", dev_addr, addr, *returnData);

 	mutex_lock(&it6151_i2c_access);
	if(dev_addr == IT6151_MIPIRX_SLAVE_ADDR_WRITE)
	{		
		ret = i2c_smbus_read_i2c_block_data(it6151_mipirx_client,addr,1,cmd_buf);
		if(ret < 0)
			   printf_info("%s: failed!ret=%d \n",__func__,ret);
		readData = cmd_buf[0];
		*returnData = readData;
	} 
	else 
	{
		printf_info("[it6151_i2c_read_byte]error:  no this dev_addr! \n");
		ret = -1;
	}
  	mutex_unlock(&it6151_i2c_access);
  	return ret ;
}
EXPORT_SYMBOL_GPL(it6151_mipirx_i2c_read_byte);

int it6151_mipirx_i2c_write_byte(unsigned char dev_addr, unsigned char addr, unsigned char writeData)
{
	char    write_data[1] = {0};
	int     ret=0;
  /* dump write_data for check */
	//printf_info("[KE/it6151_i2c_write] dev_addr = 0x%x, write_data[0x%x] = 0x%x \n", dev_addr, addr, writeData);
  
	mutex_lock(&it6151_i2c_access);
	write_data[0] = writeData;
	if(dev_addr == IT6151_MIPIRX_SLAVE_ADDR_WRITE)
	{	
		ret = i2c_smbus_write_i2c_block_data(it6151_mipirx_client,addr,1,write_data);
		if(ret < 0)
			   printf_info("%s: failed!ret=%d \n",__func__,ret);
	} 
	else
	{
		ret = -1;
     		printf_info("[it6151_i2c_write_byte]error:  no this dev_addr! \n");
	}
	
	mutex_unlock(&it6151_i2c_access);
	return ret ;
}
EXPORT_SYMBOL_GPL(it6151_mipirx_i2c_write_byte);

static int it6151_i2c_driver_probe(struct i2c_client *client, const struct i2c_device_id *id) 
{
	printf_info("%s,start name=%s,i2c_addr=%x\n",__FUNCTION__,client->name,client->addr);
	it6151_mipirx_client = client;  
	return 0;
}

static struct i2c_driver it6151_i2c_driver = 
{
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

static int __init it6151_init(void)
{    
	printf_info("[it6151_mipirx_it6151_init] init start\n");
	if(i2c_add_driver(&it6151_i2c_driver)!=0)
	{
		printf_info("[kernel][it6151_init] failed to register it6151 i2c driver.\n");
		return -1;
	}
	printf_info("[kernel][it6151_init] Success to register it6151 i2c driver.\n");
	return 0;
}

static void __exit it6151_exit(void)
{
  	i2c_del_driver(&it6151_i2c_driver);
}

late_initcall(it6151_init);
module_exit(it6151_exit);
   
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("I2C it6151 Driver");
MODULE_AUTHOR("James Lo<james.lo@mediatek.com>");

