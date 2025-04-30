#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
//#include <linux/switch.h>
#include <linux/workqueue.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <linux/proc_fs.h>

#include <linux/delay.h>
#include <linux/irq.h>
#ifdef CONFIG_OF
#include <linux/of.h>
#include <linux/of_irq.h>
#include <linux/of_address.h>
#include <linux/of_device.h>
#include <linux/regulator/consumer.h>
#include <linux/clk.h>
#include <linux/pinctrl/consumer.h>
#include <linux/of_gpio.h>
#endif
#include <linux/notifier.h>
#include <linux/input.h>

#if IS_ENABLED(CONFIG_LEDS_MTK)
#define LEDS_BRIGHTNESS_CHANGED
#include <linux/leds-mtk.h>
#endif

#define HALL_DEBUG
#ifdef HALL_DEBUG
#define HALL_LOG(fmt, args...)  pr_info(fmt, ##args)
#else
#define HALL_LOG(fmt, args...) 
#endif

#define EM_HALL_PROC_FILE "driver/cust_hall_support"
#define EM_HALL_STATE_PROC_FILE "driver/cust_hall_state"
static unsigned int EINT_HALL_PIN;
struct work_struct work_hall;
static int hall_irq; 
extern int irq_set_irq_type(unsigned int irq, unsigned int type);
static int cur_state = 0;

static struct input_dev *hall_dev = NULL;
static int lcm_is_resume = 0;
static int counter = 0;

/*-----------------------------------------------------------------*/
static ssize_t hall_store_active(struct device *dev,
			 struct device_attribute *attr, const char *buf,
			 size_t count)
{
	return count;
}

static ssize_t hall_show_active(struct device *dev,
			struct device_attribute *attr, char *buf)
{
    return snprintf(buf, 256, "%d\n", cur_state==0?0:1);
}

static ssize_t hall_state_read_proc(struct file *file, char *buffer, size_t count, loff_t *ppos)
{
	char *page = NULL;
	char *ptr = NULL;
    int len, err = 0;
	char *temp;
    if(0==cur_state)
        temp="0";
    else
        temp="1";

    page = kmalloc(PAGE_SIZE, GFP_KERNEL);
    if (!page) {
        kfree(page);
        return -ENOMEM;
	}
    ptr = page;
    ptr += sprintf(ptr, "%s\n", temp);
    len = ptr - page;
    if (*ppos >= len) {
        kfree(page);
        return 0;
	}

    err = copy_to_user(buffer, (char *)page, len);
    *ppos += len;
    if (err) {
        kfree(page);
        return err;
    }
    kfree(page);
    return len;
}
/*----------------------------------------------------------------------------*/

static void hall_do_sleep(void)
{
	counter = 0;
	//HALL_LOG("zxl>>> hall_do_sleep, lcm_is_resume_to_hall: %d\n", lcm_is_resume);
	while(counter < 50)
	{
		if(1 == __gpio_get_value(EINT_HALL_PIN) && lcm_is_resume == 0){
			input_report_key(hall_dev, KEY_HALL_ON, 1);
			input_sync(hall_dev);
			input_report_key(hall_dev, KEY_HALL_ON, 0);
			input_sync(hall_dev);
			HALL_LOG("zxl>>> lcm will on\n");
			break;
		}
		if(0 ==__gpio_get_value(EINT_HALL_PIN) && lcm_is_resume == 1) {
			input_report_key(hall_dev, KEY_HALL_OFF, 1);
			input_sync(hall_dev);
			input_report_key(hall_dev, KEY_HALL_OFF, 0);
			input_sync(hall_dev);
			HALL_LOG("zxl>>> lcm will off\n");
			break;
		}
		else	
			counter++;
		msleep(10);
	}
	
}

/*
MODULE_IMPORT_NS(VFS_internal_I_am_really_a_filesystem_and_am_NOT_a_driver);

static int brightness_read_file(char *file_name,char buf[],int buf_size)
{
	struct file *fp;
	loff_t pos = 0;

	if(file_name == NULL || buf == NULL){
		pr_err("zxl file_name == NULL || buf == NULL\n");
		return -1;
	}

	fp =filp_open(file_name, O_RDONLY, 0);
	if(IS_ERR(fp))
	{
		pr_err("zxl file not found\n");
		return -1;
	}

	kernel_read(fp, buf, buf_size, &pos);
	
	filp_close(fp,NULL);
	
	return 0;
}
#define BRIGHTNESS_FILE  "/sys/class/leds/lcd-backlight/brightness"
static void get_brightness_by_node(void)
{
	char buf[10] = {0};
	int ret = 0;

	ret = brightness_read_file(BRIGHTNESS_FILE, buf, sizeof(buf));
	if(ret !=0)
	{
		pr_err("zxl 1 get_brightness_by_node failed. ret=%d\n", ret);
		return;
	}
	if(kstrtoint(buf, 0, &ret) >= 0){
		if(ret == 0)
			lcm_is_resume = 0;
		else
			lcm_is_resume = 1;
	}else{
		pr_err("zxl get_brightness_by_node fail\n");
	}
	HALL_LOG("zxl lcm_is_resume: %d, ret: %d\n", lcm_is_resume, ret);
}*/

static void hall_do_sleep_work(struct work_struct *work)
{
	//get_brightness_by_node();
	hall_do_sleep();
}

static irqreturn_t hall_irq_handler(int irq, void *data)
{

	cur_state = __gpio_get_value(EINT_HALL_PIN);
	//HALL_LOG("zxl>>>hall_irq_handler>>> cur_state =%d\n", cur_state);
	
	if (0 == cur_state) {//靠近		
		irq_set_irq_type(hall_irq, IRQ_TYPE_LEVEL_HIGH);		
	} else {//远离
		irq_set_irq_type(hall_irq, IRQ_TYPE_LEVEL_LOW);		
	}
	
	schedule_work(&work_hall);  
	return IRQ_HANDLED;
}

static int em_hall_read_proc(struct seq_file *seq,void *v)
{
	seq_printf(seq, "Hall support: Yes\n");

	return 0;
}

static int em_hall_open(struct inode *inode,struct file *file)
{
	return single_open(file,em_hall_read_proc,inode->i_private);
}

/*-----------------------------------------------------------------*/
static DEVICE_ATTR(hall_active, 0644, hall_show_active, hall_store_active);

static const struct proc_ops em_hall_state_fops = {
	.proc_read = hall_state_read_proc,
	//.owner = THIS_MODULE,
};
/*-----------------------------------------------------------------*/

static const struct proc_ops em_hall_seq_fops = {
	.proc_open = em_hall_open,
	.proc_read = seq_read,
	.proc_release = single_release,
	.proc_write = NULL,
	//.owner = THIS_MODULE,
};

#ifdef LEDS_BRIGHTNESS_CHANGED
int lcd_backlight_changed_event(struct notifier_block *nb, unsigned long event, void *v)
{
	struct led_conf_info *led_conf;

	led_conf = (struct led_conf_info *)v;
	//HALL_LOG("zxl lcd_backlight_changed_event\n");

	switch (event) {
	case LED_BRIGHTNESS_CHANGED:
		//HALL_LOG("zxl lcd_backlight_changed_event, level: %d\n", led_conf->cdev.brightness);
		lcm_is_resume = led_conf->cdev.brightness ? 1 : 0;
		break;
	default:
		break;
	}

	return NOTIFY_DONE;
}

static struct notifier_block lcd_init_notifier = {
	.notifier_call = lcd_backlight_changed_event,
};
#endif

static int hall_probe(struct platform_device *pdev)
{
	int ret = 0;
	struct device_node *node;
	struct pinctrl *pinctrl;
    struct pinctrl_state *pins_default;
    struct pinctrl_state *pins_cfg;

	node = of_find_compatible_node(NULL, NULL, "mediatek,hall");
	HALL_LOG("zxl>>>%s\n",__FUNCTION__);
	if(node)
	{
		HALL_LOG("zxl>>>%s;node name=%s,full_name=%s\n",__FUNCTION__,node->name,node->full_name);
		
		pinctrl = devm_pinctrl_get(&pdev->dev);
		if (IS_ERR(pinctrl))
		{
			ret = PTR_ERR(pinctrl);
			HALL_LOG("Cannot find hall pinctrl!\n");
		}

		pins_default = pinctrl_lookup_state(pinctrl, "pin_default");
		if (IS_ERR(pins_default))
		{
			ret = PTR_ERR(pins_default);
			HALL_LOG("Cannot find hall pinctrl default!\n");
		}

		pins_cfg = pinctrl_lookup_state(pinctrl, "pin_cfg");
		if (IS_ERR(pins_cfg))
		{
			ret = PTR_ERR(pins_cfg);
			HALL_LOG("%s;Cannot find hall pinctrl pin_cfg!\n",__FUNCTION__);
		}
		if(NULL != pins_cfg && !(IS_ERR(pins_cfg)))
			pinctrl_select_state(pinctrl, pins_cfg);
		
		EINT_HALL_PIN = of_get_named_gpio(node, "gpio_det_level_hall_pin1", 0);
		printk("EINT_HALL_PIN=%d \n",EINT_HALL_PIN);
		ret=gpio_request(EINT_HALL_PIN, "EINT_HALL_PIN");
		
		if(ret<0)
		{
			HALL_LOG("%s;Failed to request EINT_HALL_PIN gpio\n",__FUNCTION__);
		}
		gpio_direction_input(EINT_HALL_PIN);
		gpio_set_debounce(EINT_HALL_PIN,500);
	
		hall_irq =irq_of_parse_and_map(node, 0);
		if (hall_irq < 0) 
		{
			HALL_LOG("%s;Failed to irq_of_parse_and_map EINT_HALL_PIN irq\n",__FUNCTION__);
		}	
		ret = request_irq(hall_irq , hall_irq_handler, IRQ_TYPE_LEVEL_LOW, "hall-eint", NULL);
		if (ret < 0) 
		{
			HALL_LOG("%s;Failed to request_irq EINT_HALL_PIN irq\n",__FUNCTION__);
		}
	}
	
	INIT_WORK(&work_hall, hall_do_sleep_work);

	hall_dev = input_allocate_device();	
	hall_dev->name = "hall-switch";
	__set_bit(EV_KEY, hall_dev->evbit);
	__set_bit(KEY_HALL_ON, hall_dev->keybit);
	__set_bit(KEY_HALL_OFF, hall_dev->keybit);
	ret = input_register_device(hall_dev);
	if(ret){
		HALL_LOG("emdoor register hall input err!\n");
	}

	proc_create(EM_HALL_PROC_FILE,0,NULL,&em_hall_seq_fops);
	proc_create(EM_HALL_STATE_PROC_FILE,S_IRUSR |S_IRGRP |S_IROTH,NULL,&em_hall_state_fops);

	ret = device_create_file(&pdev->dev, &dev_attr_hall_active);
	if(ret < 0) {
		pr_err("create hall_active failed\n");
		goto err;
	}

#ifdef LEDS_BRIGHTNESS_CHANGED
	mtk_leds_register_notifier(&lcd_init_notifier);
#endif

	HALL_LOG("%s; EINT_HALL_PIN gpio=%d\n",__FUNCTION__,EINT_HALL_PIN);
	HALL_LOG("%s; hall_irq gpio=%d\n",__FUNCTION__,hall_irq);
	HALL_LOG("%s; hall_probe success !\n",__FUNCTION__);
	return 0;
err:
    return ret;
}

static int hall_remove(struct platform_device *pdev)
{

	cancel_work_sync(&work_hall);
	gpio_free(EINT_HALL_PIN);
#ifdef LEDS_BRIGHTNESS_CHANGED
	mtk_leds_unregister_notifier(&lcd_init_notifier);
#endif
	return 0;
}

static int hall_suspend(struct device *dev)
{
	enable_irq_wake(hall_irq);
	HALL_LOG("zxl>>>%s\n",__FUNCTION__);
	return 0;
}

static int hall_resume(struct device *dev)
{

    disable_irq_wake(hall_irq);
	HALL_LOG("zxl>>>%s\n",__FUNCTION__);
	return 0;
}

#ifdef CONFIG_OF
static const struct of_device_id hall_of_match[] = {
	{.compatible = "mediatek,hall",},
	{}
};

MODULE_DEVICE_TABLE(of, hall_of_match);
#endif

static SIMPLE_DEV_PM_OPS(emdoor_hall_pm_ops,
			 hall_suspend, hall_resume);

static struct platform_driver e3x0_button_driver = {
	.driver		= {
		.name	= "hall",
		.of_match_table = of_match_ptr(hall_of_match),
		.pm	= &emdoor_hall_pm_ops,
	},
	.probe	= hall_probe,
	.remove	= hall_remove,
};

module_platform_driver(e3x0_button_driver);

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("kaicheng <kaicheng.zhang@emdoor.com>");
MODULE_DESCRIPTION("HALL Switch Driver");
MODULE_ALIAS("platform:hall by emdoor");
