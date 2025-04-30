#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/ctype.h>
#include <linux/uaccess.h>
#include <linux/io.h>
#include <linux/delay.h>
#include <linux/device.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/string.h>
#include <linux/i2c.h>
#include <linux/of_gpio.h>
#include <linux/interrupt.h>
#include <linux/input.h>
#include <linux/workqueue.h>


#define IT6151_TEST_NAME   "it6151_test"


struct it6151_device {
	dev_t dev;
	struct cdev *cdev;
	struct class *class;
	struct device *device; 
};

struct it6151_device *it6151;

extern void it6151_dumpreg(void);

#if 1
/****************************show store**********************************/
static ssize_t it6151_show(struct device *d, struct device_attribute *attr, char *buf) // cat
{	

	it6151_dumpreg();
	
	return sprintf(buf, "%d\n", 1);
}

static ssize_t it6151_store(struct device *d, struct device_attribute *attr, const char *buf, size_t count) // echo reg val > allreg
{
	unsigned int databuf;;
	
	if(1 == sscanf(buf, "%x", &databuf)) {
		if(databuf == 1)
			pr_err(">>>>emdoor 1\n");
	}
	
    return count;
	
}

static DEVICE_ATTR(allreg, S_IWUSR | S_IRUGO, it6151_show, it6151_store);
#endif
	
/****************************fops**********************************/	
static int it6151_open(struct inode *inode, struct file *file) 
{   
     return 0;  
}  

static ssize_t it6151_read(struct file *filp, char __user *buf, size_t count, loff_t *offp)  
{  
	ssize_t ret = 0;

	return ret;
} 

static ssize_t it6151_write(struct file *filp, const char __user *buf, size_t count, loff_t *offp)  
{  
	ssize_t ret = 0;

	return ret;
} 


static long it6151_unlocked_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	int retvalue = 0;
	
	return retvalue;
}


static int it6151_release(struct inode *inode, struct file *file)
{   
     return 0;  
}


static struct file_operations it6151_fops = {
      .owner = THIS_MODULE,  
      .read = it6151_read,  
      .write = it6151_write,  
      .open = it6151_open,
      .unlocked_ioctl = it6151_unlocked_ioctl,
      .release = it6151_release,  
};  


static int it6151_init_test(void)
{
	int ret = -1;
	
	it6151 = kzalloc(sizeof(*it6151), GFP_KERNEL);
	if (!it6151) {
		pr_err(">>>>it6151 kzalloc failed!\n");
		return -ENOMEM;
	}

	ret = alloc_chrdev_region(&it6151->dev, 0, 1, IT6151_TEST_NAME);
	if(ret) {
		pr_err(">>>>alloc_chrdev_region it6151 failed\n");
		return ret;
	}

	it6151->cdev = cdev_alloc();
	it6151->cdev->owner = THIS_MODULE;
	it6151->cdev->ops = &it6151_fops;
	
	ret = cdev_add(it6151->cdev, it6151->dev, 1);
	if(ret) {
		printk(">>>>cdev_add it6151 failed\n");
		unregister_chrdev_region(it6151->dev,1);
		return ret;
	}

	it6151->class = class_create(THIS_MODULE, IT6151_TEST_NAME);

	it6151->device = device_create(it6151->class, NULL, it6151->dev, NULL, IT6151_TEST_NAME);
	
#if 1
	ret = sysfs_create_file(&(it6151->device->kobj), &dev_attr_allreg.attr);
	if(ret)
		printk(">>>>it6151 sysfs_create_file failed\n");
#endif
	

	printk(">>>>%s ok\n", __func__);
	
	return 0;
}

static void it6151_exit_test(void)
{
	class_destroy(it6151->class);
	cdev_del(it6151->cdev);
	unregister_chrdev_region(it6151->dev,1);
	printk(">>>>%s\n", __func__);
}

MODULE_LICENSE("GPL");
module_init(it6151_init_test);
module_exit(it6151_exit_test);
