#include <linux/module.h>
#include <linux/init.h>	
#include <linux/device.h>
#include <linux/of_device.h>
#include <linux/platform_device.h>
#include <linux/of_platform.h>
#include <linux/cdev.h>	
#include <linux/err.h>
#include <linux/vmalloc.h>
#include <linux/of_gpio.h>
#include <linux/gpio.h>

static struct class *sdcard_detect_class;
static struct device *sdcard_detect_device;
static struct cdev *sdcard_detect_cdev;
static dev_t sdcard_detect_devno;
static int sdcard_detect_major;

extern u8 card_slot_present(void);

static ssize_t slot_present_show(struct device *dev,
				   struct device_attribute *attr,
				   char *buf)
{
    if(card_slot_present()) {
        return sprintf(buf, "1\n");
    }else{
         return sprintf(buf, "0\n");   
    }
	
	return sprintf(buf, "0\n");
}
static DEVICE_ATTR_RO(slot_present);

static long sdcarddetect_ioctl(
	struct file *file, unsigned int cmd, unsigned long arg)
{
	return 0;
}    
static int sdcarddetect_open(struct inode *inode, struct file *file)
{
	return 0;
}
static int sdcarddetect_release(struct inode *inode, struct file *file)
{
	return 0;
}

static const struct file_operations sdcarddetect_fops = {
	.owner = THIS_MODULE,
	.unlocked_ioctl = sdcarddetect_ioctl,
	.open = sdcarddetect_open,
	.release = sdcarddetect_release,
};

static int sdcarddetect_probe(struct platform_device *pdev)
{
	int ret;
	//struct device *dev = &pdev->dev;

	pr_err("%s enter\n", __func__);
	
	ret = alloc_chrdev_region(&sdcard_detect_devno, 0, 1, "sdcarddetect");
	if (ret)
		printk("Error: Can't Get Major number for sdcard_detect_devno\n");    
    sdcard_detect_cdev = cdev_alloc();
	sdcard_detect_cdev->owner = THIS_MODULE;
	sdcard_detect_cdev->ops = &sdcarddetect_fops;
	ret = cdev_add(sdcard_detect_cdev, sdcard_detect_devno, 1);
	if (ret)
		printk("sdcard_detect_cdev Error: cdev_add\n");

	sdcard_detect_major = MAJOR(sdcard_detect_devno);    
	sdcard_detect_class = class_create(THIS_MODULE, "sdcard_device");
	if (IS_ERR(sdcard_detect_class)) {
		ret = PTR_ERR(sdcard_detect_class);
		return -EIO;
	}
	sdcard_detect_device = device_create(sdcard_detect_class, NULL, sdcard_detect_devno, NULL,
				    "sdcard_slot");

	if (sdcard_detect_device == NULL)
		return -EIO;

	ret = device_create_file(sdcard_detect_device, &dev_attr_slot_present);
	if (ret)
		return ret;
	
	return 0;
}

static int sdcarddetect_remove(struct platform_device *pdev)
{
    printk("[%s]%d\n",__func__,__LINE__);
    return 0;
}
static const struct of_device_id sdcarddetect_of_match[] = {
	{.compatible = "emdoor,sdcarddetect",},
	{},
};

MODULE_DEVICE_TABLE(of, sdcarddetect_of_match);
static struct platform_driver sdcarddetect_driver = {
	.probe	= sdcarddetect_probe,
	.remove = sdcarddetect_remove,
	.driver = {
		.name	= "sdcarddetect_driver",
        .of_match_table = sdcarddetect_of_match,
	},
};
static int __init sdcarddetect_init(void)
{
    printk("[%s]%d\n",__func__,__LINE__);
	return platform_driver_register(&sdcarddetect_driver);
}
late_initcall(sdcarddetect_init);

static void __exit sdcarddetect_exit(void)
{
    printk("[%s]%d\n",__func__,__LINE__);
	platform_driver_unregister(&sdcarddetect_driver);
}
module_exit(sdcarddetect_exit);
MODULE_AUTHOR("zhong.huang@emdoor.com");
MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("sdcard detect information");
