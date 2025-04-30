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
#include <linux/fb.h>
#include <linux/notifier.h>
#include <linux/input.h>
#include <linux/pm_runtime.h>
#include <asm/arch_timer.h>
#define POGO_DEBUG
#ifdef POGO_DEBUG
#define POGO_LOG(fmt, args...)  pr_err(fmt, ##args)
#else
#define POGO_LOG(fmt, args...) pr_debug(fmt, ##args)
#endif

#define EM_POGO_PROC_FILE "driver/cust_pogo_support"
static unsigned int EINT_POGO_PIN;
struct work_struct work_pogo;
static int pogo_irq; 
struct mutex pogo_access_lock;
struct wakeup_source *pogo_in_ws;
static struct timer_list timer_usb_pogo_det;
//static int pogo_suspend_flag=0; 
// extern void mt_gpio_opt_pullen(int gpio, int val);
// extern void mt_gpio_opt_pullsel(int gpio, int val);
//extern void kpd_pwrkey_pmic_handler(unsigned long pressed);
//extern int irq_set_irq_type(unsigned int irq, unsigned int type);

//static struct input_dev *pogo_dev = NULL;

static struct pinctrl_state *usb_sel_high = NULL;
static struct pinctrl_state *usb_sel_low = NULL;
static struct pinctrl_state *usb_sel_default = NULL;
static struct pinctrl *pinctrl;
static struct pinctrl_state *pinctrl_pogo = NULL;
static struct pinctrl_state *pinctrl_pogo_low = NULL;
static struct pinctrl_state *pinctrl_pogo_high = NULL;
static struct pinctrl_state *pogo_det_defaults = NULL;
static struct pinctrl_state *pogo_det_pins = NULL;

int pogo_power_high(void);
int pogo_power_low(void);
int usb_switch_high(void);
int usb_switch_low(void);

int cur_state = 0;
extern void mt_usbhost_connect(void);
extern void mt_usbhost_disconnect(void);

int pogo_plug_in(void)
{
	//irq_set_irq_type(pogo_irq, IRQ_TYPE_LEVEL_HIGH);
	usb_switch_high();
	pogo_power_high();
	mt_usbhost_connect();
	__pm_stay_awake(pogo_in_ws);
	POGO_LOG("em>>>plug in usb pogo \n");
	return 0;
}
EXPORT_SYMBOL(pogo_plug_in);

int pogo_plug_out(void)
{
	//irq_set_irq_type(pogo_irq, IRQ_TYPE_LEVEL_LOW);		
	usb_switch_low();
	
	pogo_power_low();
	mt_usbhost_disconnect();
	POGO_LOG("em>>>plug out usb pogo \n");		
	return 0;
}
EXPORT_SYMBOL(pogo_plug_out);

int check_pogo_eint_gpio(void)
{
	
	//mutex_lock(&pogo_access_lock);
	cur_state = __gpio_get_value(EINT_POGO_PIN);
	//mutex_unlock(&pogo_access_lock);
	POGO_LOG("em>>>usb pogo_irq_handler>>> cur_state =%d\n", cur_state);
	
	return cur_state;
}
EXPORT_SYMBOL(check_pogo_eint_gpio);

static void pogo_switch(void)
{
	if (0 == check_pogo_eint_gpio()) {//plug in pogo		
		pogo_plug_in();
	} else {//plug out pogo
		//pogo_plug_out();
		usb_switch_low();
		pogo_power_low();
	}
	
}

static void work_pogo_switch(struct work_struct *work)
{
	pogo_switch();
}
static irqreturn_t pogo_irq_handler(int irq, void *data)
{
	cur_state = check_pogo_eint_gpio();
	if (0 == cur_state) {//plug in pogo		
		irq_set_irq_type(pogo_irq, IRQ_TYPE_LEVEL_HIGH);
		//pogo_plug_in();
	} else {//plug out pogo
		irq_set_irq_type(pogo_irq, IRQ_TYPE_LEVEL_LOW);	
		//pogo_plug_out();
	}
	
	schedule_work(&work_pogo);  
	return IRQ_HANDLED;
}

static void munual_trigger_irq(struct timer_list *t)
{
	if (0 == check_pogo_eint_gpio()) 	
		pogo_plug_in();
	mod_timer(&timer_usb_pogo_det,jiffies + 5 * HZ);
	pr_info("munual_trigger_irq when boot on!!\n");
	del_timer(&timer_usb_pogo_det);
}

static void init_usb_detect_timer(void)
{
	timer_setup(&timer_usb_pogo_det, munual_trigger_irq, 0);
	timer_usb_pogo_det.expires = jiffies + 10 * HZ;
	add_timer(&timer_usb_pogo_det);
	mod_timer(&timer_usb_pogo_det,jiffies + 10 * HZ);
}

#if 0
 static int pogo_suspend(void)
{
	POGO_LOG("em>>> usb%s \n",__FUNCTION__);
	pogo_power_low();
	
	return 0;
}

static int pogo_resume(void)
{
	POGO_LOG("em>>> usb%s \n",__FUNCTION__);
	pogo_power_high();
	if(check_pogo_eint_gpio() == 0){
		pogo_plug_out();
		pogo_plug_in();
	}else {
		//pogo_plug_out();
	}
	return 0;
}
#endif
static int pogo_event_notifier_callback(struct notifier_block *self,
				unsigned long action, void *data)
{
	struct fb_event *event = data;
	int blank_mode = 0;

//	if (action != FB_EARLY_EVENT_BLANK)
//		return 0;

	if ((event == NULL) || (event->data == NULL))
		return 0;

	blank_mode = *((int *)event->data);

		switch (blank_mode) {
		case FB_BLANK_UNBLANK:
		case FB_BLANK_NORMAL:
			//pogo_resume();
			break;
		case FB_BLANK_VSYNC_SUSPEND:
		case FB_BLANK_HSYNC_SUSPEND:
			break;
		case FB_BLANK_POWERDOWN:
			//pogo_suspend();
			break;
		default:
			return -EINVAL;
		}
	return 0;
}

static struct notifier_block pogo_event_notifier = {
	.notifier_call  = pogo_event_notifier_callback,
};
/*
static int em_pogo_read_proc(struct seq_file *seq,void *v)
{
	seq_printf(seq, "Hall support: Yes\n");

	return 0;
}

static int em_pogo_open(struct inode *inode,struct file *file)
{
	return single_open(file,em_pogo_read_proc,inode->i_private);
}

static const struct file_operations em_pogo_seq_fops = {
	.open = em_pogo_open,
	.read = seq_read,
	.release = single_release,
	.write = NULL,
	.owner = THIS_MODULE,
};
*/
int pogo_get_gpio(struct device *dev)
{
       int ret = 0;
       pinctrl = devm_pinctrl_get(dev);
          if (IS_ERR(pinctrl) || NULL == pinctrl) {
               dev_err(dev, "Cannot find hub rest pinctrl!");
               ret = PTR_ERR(pinctrl);
               return ret;
       }
/*usb sel status gpio 103*/
       usb_sel_default = pinctrl_lookup_state(pinctrl, "usb_sel_init");
       if (IS_ERR(usb_sel_default) || NULL == usb_sel_default) {
               ret = PTR_ERR(usb_sel_default);
               pr_err("%s : pinctrl err, usb_sel_default\n", __func__);
       }
       usb_sel_high = pinctrl_lookup_state(pinctrl, "usb_sel_high");
       if (IS_ERR(usb_sel_high) || NULL == usb_sel_high) {
               ret = PTR_ERR(usb_sel_high);
               pr_err("%s : pinctrl err, usb_sel_high\n", __func__);
       }
       usb_sel_low = pinctrl_lookup_state(pinctrl, "usb_sel_low");
       if (IS_ERR(usb_sel_low) || NULL == usb_sel_low) {
               ret = PTR_ERR(usb_sel_low);
               pr_err("%s : pinctrl err, usb_sel_low\n", __func__);
       }          
/*pogo pin status gpio 42*/       
       pinctrl_pogo = pinctrl_lookup_state(pinctrl, "pogo_init");
       if (IS_ERR(pinctrl_pogo)) {
               ret = PTR_ERR(pinctrl_pogo);
               dev_err(dev, "Cannot find usb pinctrl pogo\n");
       }
       pinctrl_pogo_low = pinctrl_lookup_state(pinctrl, "pogo_low");
       if (IS_ERR(pinctrl_pogo_low)) {
               ret = PTR_ERR(pinctrl_pogo_low);
               dev_err(dev, "Cannot find usb pinctrl pogo_low\n");
       }
       pinctrl_pogo_high = pinctrl_lookup_state(pinctrl, "pogo_high");
       if (IS_ERR(pinctrl_pogo_high)) {
               ret = PTR_ERR(pinctrl_pogo_high);
               dev_err(dev, "Cannot find usb pinctrl pogo_high\n");
       }
	   
/*pogo int status gpio 47*/       
       pinctrl_pogo = pinctrl_lookup_state(pinctrl, "pogo_init");
       if (IS_ERR(pinctrl_pogo)) {
               ret = PTR_ERR(pinctrl_pogo);
               dev_err(dev, "Cannot find usb pinctrl pogo\n");
       }
       pinctrl_pogo_low = pinctrl_lookup_state(pinctrl, "pogo_low");
       if (IS_ERR(pinctrl_pogo_low)) {
               ret = PTR_ERR(pinctrl_pogo_low);
               dev_err(dev, "Cannot find usb pinctrl pogo_low\n");
       }
       pinctrl_pogo_high = pinctrl_lookup_state(pinctrl, "pogo_high");
       if (IS_ERR(pinctrl_pogo_high)) {
               ret = PTR_ERR(pinctrl_pogo_high);
               dev_err(dev, "Cannot find usb pinctrl pogo_high\n");
       }

		pogo_det_defaults = pinctrl_lookup_state(pinctrl, "pogo_det_default");
		if (IS_ERR(pogo_det_defaults))
		{
			ret = PTR_ERR(pogo_det_defaults);
			POGO_LOG("Cannot find pogo pinctrl pogo_det_defaults!\n");
		}

		pogo_det_pins = pinctrl_lookup_state(pinctrl, "pogo_det_pin");
		if (IS_ERR(pogo_det_pins))
		{
			ret = PTR_ERR(pogo_det_pins);
			POGO_LOG("%s;Cannot find pogo pinctrl pogo_det_pins!\n",__FUNCTION__);
		}
		
	   printk("%s pass \n ",__func__);
       return ret;
}

int pogo_power_high(void)//gpio 42
{
	struct pinctrl_state * pCtrl = NULL;
	char * pStrDebug = NULL;
	if ( NULL == pinctrl || IS_ERR(pinctrl)) {
		   return false;
	}	
	
	pCtrl = pinctrl_pogo_high;
	pStrDebug = "pogo_en";
	
	if ( pCtrl == NULL || IS_ERR(pCtrl)) return false;
	pinctrl_select_state(pinctrl, pCtrl);
	pr_err("usb: %s set power on\n", pStrDebug);
	
	return 0;
	
}
EXPORT_SYMBOL(pogo_power_high);

int pogo_power_low(void)//gpio 42
{
	struct pinctrl_state * pCtrl = NULL;
	char * pStrDebug = NULL;
	if ( NULL == pinctrl || IS_ERR(pinctrl)) {
		   return false;
	}	
	
	pCtrl = pinctrl_pogo_low;
	pStrDebug = "pogo_en";
	
	if ( pCtrl == NULL || IS_ERR(pCtrl)) return false;
	pinctrl_select_state(pinctrl, pCtrl);
	pr_err("usb: %s set power off\n", pStrDebug);
	__pm_relax(pogo_in_ws);
	return 0;
}
EXPORT_SYMBOL(pogo_power_low);
int usb_switch_high(void)//gpio 103
{
	struct pinctrl_state * pCtrl = NULL;
	char * pStrDebug = NULL;
	if ( NULL == pinctrl || IS_ERR(pinctrl)) {
		   return false;
	}	
	
	pCtrl = usb_sel_high;
	pStrDebug = "usb_sel";
	
	if ( pCtrl == NULL || IS_ERR(pCtrl)) return false;
	pinctrl_select_state(pinctrl, pCtrl);
	pr_err("usb: %s set power on\n", pStrDebug);
	return 0;
}

int usb_switch_low(void)//gpio 103
{
	struct pinctrl_state * pCtrl = NULL;
	char * pStrDebug = NULL;
	if ( NULL == pinctrl || IS_ERR(pinctrl)) {
		   return false;
	}	
	
	pCtrl = usb_sel_low;
	pStrDebug = "usb_sel";
	
	if ( pCtrl == NULL || IS_ERR(pCtrl)) return false;
	pinctrl_select_state(pinctrl, pCtrl);
	pr_err("usb: %s set power off\n", pStrDebug);
	return 0;
	
}
EXPORT_SYMBOL(usb_switch_low);

static int pogo_probe(struct platform_device *pdev)
{
	int ret = 0;
	struct device *dev = &pdev->dev;
	struct device_node *node;

	node = of_find_compatible_node(NULL, NULL, "emdoor,pogo");
	POGO_LOG("em>>>%s\n",__FUNCTION__);
	if(node)
	{
		POGO_LOG("em>>>%s;node is not null\n",__FUNCTION__);
		POGO_LOG("em>>>%s;node name=%s,full_name=%s\n",__FUNCTION__,node->name,node->full_name);

		if(pogo_get_gpio(dev))//request gpio 
			printk("pogo_hub_get_gpio is failed!!\n");

		if(NULL != pogo_det_pins && !(IS_ERR(pogo_det_pins)))
			pinctrl_select_state(pinctrl, pogo_det_pins);

		EINT_POGO_PIN = of_get_named_gpio(node, "gpio_det_pogo_pin1", 0);
		printk("EINT_POGO_PIN=%d \n",EINT_POGO_PIN);
		ret=gpio_request(EINT_POGO_PIN, "EINT_POGO_PIN");
		
		if(ret<0)
		{
			POGO_LOG("%s;Failed to request EINT_POGO_PIN gpio\n",__FUNCTION__);
		}
		gpio_direction_input(EINT_POGO_PIN);
		gpio_set_debounce(EINT_POGO_PIN,500);
	
		pogo_irq =irq_of_parse_and_map(node, 0);
		if (pogo_irq < 0) 
		{
			POGO_LOG("%s;Failed to irq_of_parse_and_map EINT_POGO_PIN irq\n",__FUNCTION__);
		}	
		ret = request_irq(pogo_irq , pogo_irq_handler, IRQ_TYPE_LEVEL_LOW, "pogo-eint", NULL);
		if (ret < 0) 
		{
			POGO_LOG("%s;Failed to request_irq EINT_POGO_PIN irq\n",__FUNCTION__);
		}
	}
	pogo_in_ws =
		wakeup_source_register(dev,
				       devm_kasprintf(dev, GFP_KERNEL,
				       "pogo_switch_dwork.%s",
				       dev_name(dev)));
	INIT_WORK(&work_pogo, work_pogo_switch);
	mutex_init(&pogo_access_lock);
	fb_register_client(&pogo_event_notifier);
	init_usb_detect_timer();
	//proc_create(EM_POGO_PROC_FILE,0,NULL,&em_pogo_seq_fops);
	POGO_LOG("%s; EINT_POGO_PIN gpio=%d\n",__FUNCTION__,EINT_POGO_PIN);
	POGO_LOG("%s; pogo_irq gpio=%d\n",__FUNCTION__,pogo_irq);
	POGO_LOG("%s; pogo_probe success !\n",__FUNCTION__);
	return 0;

}

 static int pogo_remove(struct platform_device *pdev)
 {

	 cancel_work_sync(&work_pogo);
	 gpio_free(EINT_POGO_PIN);
	 return 0;
 }

#ifdef CONFIG_OF
static const struct of_device_id pogo_of_match[] = {
	{.compatible = "emdoor,pogo",},
	{},
};
#endif

static struct platform_driver pogo_driver = {
	.probe		= pogo_probe,
	.remove		= pogo_remove,
//	.suspend    =pogo_suspend,
//	.resume     =pogo_resume,
	.driver		= {
		.name	= "pogo",
#ifdef CONFIG_OF
		 .of_match_table = pogo_of_match,
#endif
			
		.owner	= THIS_MODULE,
	},
};


static int __init pogo_switch_init(void)
{
	int ret = 0;
 
	POGO_LOG("%s\n",__FUNCTION__);
	
	ret = platform_driver_register(&pogo_driver);
	if (ret) 
	{
        POGO_LOG("failed to register pogo_device\n");
	}

	return 0;
}

static void __exit pogo_switch_exit(void)
{
	platform_driver_unregister(&pogo_driver);
}

late_initcall(pogo_switch_init);
module_exit(pogo_switch_exit);


MODULE_DESCRIPTION("POGO Switch Driver");
MODULE_LICENSE("GPL");
