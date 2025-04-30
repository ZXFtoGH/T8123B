// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2020 MediaTek Inc.
 */

#include <linux/extcon-provider.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/of_gpio.h>
#include <linux/of_platform.h>
#include <linux/platform_device.h>
#include <linux/power_supply.h>
#include <linux/regulator/consumer.h>
#include <linux/slab.h>
#include <linux/usb/role.h>
#include <linux/workqueue.h>
#include <linux/proc_fs.h>
#include <linux/usb.h>
#include <linux/notifier.h>
#include <linux/miscdevice.h>

#include "extcon-mtk-usb.h"

#if IS_ENABLED(CONFIG_TCPC_CLASS)
#include "tcpm.h"
#endif
 
#if IS_ENABLED(CONFIG_LEDS_MTK)
#include <linux/leds-mtk.h>
#endif

static int total_usb_cnt = 0; 
static const unsigned int usb_extcon_cable[] = {
	EXTCON_USB,
	EXTCON_USB_HOST,
	EXTCON_NONE,
};

static struct mtk_extcon_info *g_extcon;

static void mtk_usb_extcon_update_role(struct work_struct *work)
{
	struct usb_role_info *role = container_of(to_delayed_work(work),
					struct usb_role_info, dwork);
	struct mtk_extcon_info *extcon = role->extcon;
	unsigned int cur_dr, new_dr;

	cur_dr = extcon->c_role;
	new_dr = role->d_role;

	dev_info(extcon->dev, "cur_dr(%d) new_dr(%d)\n", cur_dr, new_dr);

	/* none -> device */
	if (cur_dr == USB_ROLE_NONE &&
			new_dr == USB_ROLE_DEVICE) {
		extcon_set_state_sync(extcon->edev, EXTCON_USB, true);
	/* none -> host */
	} else if (cur_dr == USB_ROLE_NONE &&
			new_dr == USB_ROLE_HOST) {
		extcon_set_state_sync(extcon->edev, EXTCON_USB_HOST, true);
	/* device -> none */
	} else if (cur_dr == USB_ROLE_DEVICE &&
			new_dr == USB_ROLE_NONE) {
		extcon_set_state_sync(extcon->edev, EXTCON_USB, false);
	/* host -> none */
	} else if (cur_dr == USB_ROLE_HOST &&
			new_dr == USB_ROLE_NONE) {
		extcon_set_state_sync(extcon->edev, EXTCON_USB_HOST, false);
	/* device -> host */
	} else if (cur_dr == USB_ROLE_DEVICE &&
			new_dr == USB_ROLE_HOST) {
		extcon_set_state_sync(extcon->edev, EXTCON_USB, false);
		extcon_set_state_sync(extcon->edev, EXTCON_USB_HOST, true);
	/* host -> device */
	} else if (cur_dr == USB_ROLE_HOST &&
			new_dr == USB_ROLE_DEVICE) {
		extcon_set_state_sync(extcon->edev, EXTCON_USB_HOST, false);
		extcon_set_state_sync(extcon->edev, EXTCON_USB, true);
	}

	/* usb role switch */
	if (extcon->role_sw)
		usb_role_switch_set_role(extcon->role_sw, new_dr);

	extcon->c_role = new_dr;
	kfree(role);
}

static int mtk_usb_extcon_set_role(struct mtk_extcon_info *extcon,
						unsigned int role)
{
	struct usb_role_info *role_info;

	/* create and prepare worker */
	role_info = kzalloc(sizeof(*role_info), GFP_ATOMIC);
	if (!role_info)
		return -ENOMEM;

	INIT_DELAYED_WORK(&role_info->dwork, mtk_usb_extcon_update_role);

	role_info->extcon = extcon;
	role_info->d_role = role;
	/* issue connection work */
	queue_delayed_work(extcon->extcon_wq, &role_info->dwork, 0);

	return 0;
}

void mt_usbhost_connect(void)
{
	mtk_usb_extcon_set_role(g_extcon, USB_ROLE_HOST);
}
EXPORT_SYMBOL(mt_usbhost_connect);

void mt_usbhost_disconnect(void)
{
	mtk_usb_extcon_set_role(g_extcon, USB_ROLE_DEVICE);
}
EXPORT_SYMBOL(mt_usbhost_disconnect);

static bool usb_is_online(struct mtk_extcon_info *extcon)
{
	union power_supply_propval pval;
	union power_supply_propval tval;
	int ret;

	ret = power_supply_get_property(extcon->usb_psy,
				POWER_SUPPLY_PROP_ONLINE, &pval);
	if (ret < 0) {
		dev_info(extcon->dev, "failed to get online prop\n");
		return false;
	}

	ret = power_supply_get_property(extcon->usb_psy,
				POWER_SUPPLY_PROP_TYPE, &tval);
	if (ret < 0) {
		dev_info(extcon->dev, "failed to get usb type\n");
		return false;
	}

	dev_info(extcon->dev, "online=%d, type=%d\n", pval.intval, tval.intval);

	if (pval.intval && (tval.intval == POWER_SUPPLY_TYPE_USB ||
			tval.intval == POWER_SUPPLY_TYPE_USB_CDP))
		return true;
	else
		return false;
}

static void mtk_usb_extcon_psy_detector(struct work_struct *work)
{
	struct mtk_extcon_info *extcon = container_of(to_delayed_work(work),
		struct mtk_extcon_info, wq_psy);

	/* Workaround for PR_SWAP, IF tcpc_dev, then do not switch role. */
	/* Since we will set USB to none when type-c plug out */
#if IS_ENABLED(CONFIG_TCPC_CLASS)
	if (extcon->tcpc_dev) {
		if (usb_is_online(extcon) && extcon->c_role == USB_ROLE_NONE)
			mtk_usb_extcon_set_role(extcon, USB_ROLE_DEVICE);
	} else {
#endif
		if (usb_is_online(extcon))
			mtk_usb_extcon_set_role(extcon, USB_ROLE_DEVICE);
		else
			mtk_usb_extcon_set_role(extcon, USB_ROLE_NONE);
#if IS_ENABLED(CONFIG_TCPC_CLASS)
	}
#endif

}

static int mtk_usb_extcon_psy_notifier(struct notifier_block *nb,
				unsigned long event, void *data)
{
	struct power_supply *psy = data;
	struct mtk_extcon_info *extcon = container_of(nb,
					struct mtk_extcon_info, psy_nb);

	if (event != PSY_EVENT_PROP_CHANGED || psy != extcon->usb_psy)
		return NOTIFY_DONE;

	queue_delayed_work(system_power_efficient_wq, &extcon->wq_psy, 0);

	return NOTIFY_DONE;
}

static int mtk_usb_extcon_psy_init(struct mtk_extcon_info *extcon)
{
	int ret = 0;
	struct device *dev = extcon->dev;

	extcon->usb_psy = devm_power_supply_get_by_phandle(dev, "charger");
	if (IS_ERR_OR_NULL(extcon->usb_psy)) {
	#if IS_ENABLED(CONFIG_CHARGER_SECOND_MAINCHG)
		dev_err(dev, "fail to get usb_psy from charger node \n");
		extcon->usb_psy = devm_power_supply_get_by_phandle(dev, "charger_second");
		if (IS_ERR_OR_NULL(extcon->usb_psy)) {
				dev_err(dev, "fail to get usb_psy from charger_second node \n");
				return -EINVAL;
		}
	#else

		dev_err(dev, "fail to get usb_psy\n");
		return -EINVAL;
	#endif
	}

	INIT_DELAYED_WORK(&extcon->wq_psy, mtk_usb_extcon_psy_detector);

	extcon->psy_nb.notifier_call = mtk_usb_extcon_psy_notifier;
	ret = power_supply_reg_notifier(&extcon->psy_nb);
	if (ret)
		dev_err(dev, "fail to register notifer\n");
/*
	if (usb_is_online(extcon))
		mtk_usb_extcon_set_role(extcon, USB_ROLE_DEVICE);
	else
		mtk_usb_extcon_set_role(extcon, USB_ROLE_NONE);
*/
	return ret;
}

#if IS_ENABLED(CONFIG_CHARGER_RT9458)
/* ADAPT_CHARGER_V1 */
#include <charger_class.h>
static struct charger_device *primary_charger;

static int mtk_usb_extcon_set_vbus_v1(struct mtk_extcon_info *extcon, bool is_on)
{
	struct device *dev = extcon->dev;
	if (!primary_charger) {
		primary_charger = get_charger_by_name("primary_chg");
		if (!primary_charger) {
			dev_info(dev, "%s : get primary charger device failed\n", __func__);
			return -ENODEV;
		}
	}
#if IS_ENABLED(CONFIG_MTK_GAUGE_VERSION) && (CONFIG_MTK_GAUGE_VERSION == 30)
	dev_info(dev, "%s vbus turn %s\n", __func__, is_on ? "on" : "off");
	if (is_on) {
		charger_dev_enable_otg(primary_charger, true);
		charger_dev_set_boost_current_limit(primary_charger,
			1500000);
		#if 0
		{// # workaround
			charger_dev_kick_wdt(primary_charger);
			enable_boost_polling(true);
		}
		#endif
	} else {
		charger_dev_enable_otg(primary_charger, false);
		#if 0
			//# workaround
			enable_boost_polling(false);
		#endif
	}
#else
	if (is_on) {
		charger_dev_enable_otg(primary_charger, true);
		charger_dev_set_boost_current_limit(primary_charger,
			1500000);
	} else {
		charger_dev_enable_otg(primary_charger, false);
	}
#endif
		return 0;
}
#endif

static int mtk_usb_extcon_set_vbus(struct mtk_extcon_info *extcon,
							bool is_on)
{
	int ret;
#if IS_ENABLED(CONFIG_CHARGER_RT9458)
	ret = mtk_usb_extcon_set_vbus_v1(extcon, is_on);
#else
	struct regulator *vbus = extcon->vbus;
	struct device *dev = extcon->dev;

	/* vbus is optional */
	if (!vbus || extcon->vbus_on == is_on)
		return 0;

	dev_info(dev, "vbus turn %s\n", is_on ? "on" : "off");

	if (is_on) {
		if (extcon->vbus_vol) {
			ret = regulator_set_voltage(vbus,
					extcon->vbus_vol, extcon->vbus_vol);
			if (ret) {
				dev_err(dev, "vbus regulator set voltage failed\n");
				return ret;
			}
		}

		if (extcon->vbus_cur) {
			ret = regulator_set_current_limit(vbus,
					extcon->vbus_cur, extcon->vbus_cur);
			if (ret) {
				dev_err(dev, "vbus regulator set current failed\n");
				return ret;
			}
		}

		ret = regulator_enable(vbus);
		if (ret) {
			dev_err(dev, "vbus regulator enable failed\n");
			return ret;
		}
	} else {
		regulator_disable(vbus);
	}

	extcon->vbus_on = is_on;

	ret = 0;
#endif
	return ret;
}
static void extcon_vbus_on_off(void)
{	
	printk("%s %d\n", __func__,g_extcon->c_role);
	if(g_extcon->c_role == USB_ROLE_HOST && total_usb_cnt >= 1){
		printk("%s %d\n", __func__,g_extcon->c_role);
		mtk_usb_extcon_set_vbus(g_extcon, false);
		mdelay(500);
		mtk_usb_extcon_set_vbus(g_extcon, true);
	}
}	


ssize_t extcon_vbus_write(struct file *filp,
				const char __user *buf, size_t count,
			    loff_t *ppos)
{

	printk("%s \n", __func__);

	extcon_vbus_on_off();

	return count;
}

static const struct file_operations extcon_vbus_fops = {
	.owner = THIS_MODULE,
	.write = extcon_vbus_write,
};

static struct miscdevice extcon_vbus_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "extcon_vbus",
	.fops = &extcon_vbus_fops,
};
#if IS_ENABLED(CONFIG_TCPC_CLASS)
static int mtk_extcon_tcpc_notifier(struct notifier_block *nb,
		unsigned long event, void *data)
{
	struct tcp_notify *noti = data;
	struct mtk_extcon_info *extcon =
			container_of(nb, struct mtk_extcon_info, tcpc_nb);
	struct device *dev = extcon->dev;
	bool vbus_on;

	switch (event) {
	case TCP_NOTIFY_SOURCE_VBUS:
		dev_info(dev, "source vbus = %dmv\n",
				 noti->vbus_state.mv);
		vbus_on = (noti->vbus_state.mv) ? true : false;
		mtk_usb_extcon_set_vbus(extcon, vbus_on);
		break;
	case TCP_NOTIFY_TYPEC_STATE:
		dev_info(dev, "old_state=%d, new_state=%d\n",
				noti->typec_state.old_state,
				noti->typec_state.new_state);
		if (noti->typec_state.old_state == TYPEC_UNATTACHED &&
			noti->typec_state.new_state == TYPEC_ATTACHED_SRC) {
			dev_info(dev, "Type-C SRC plug in\n");
			mtk_usb_extcon_set_role(extcon, USB_ROLE_HOST);
		} else if (!(extcon->bypss_typec_sink) &&
			noti->typec_state.old_state == TYPEC_UNATTACHED &&
			(noti->typec_state.new_state == TYPEC_ATTACHED_SNK ||
			noti->typec_state.new_state == TYPEC_ATTACHED_NORP_SRC ||
			noti->typec_state.new_state == TYPEC_ATTACHED_CUSTOM_SRC ||
			noti->typec_state.new_state == TYPEC_ATTACHED_DBGACC_SNK)) {
			dev_info(dev, "Type-C SINK plug in\n");
			mtk_usb_extcon_set_role(extcon, USB_ROLE_DEVICE);
		} else if ((noti->typec_state.old_state == TYPEC_ATTACHED_SRC ||
			noti->typec_state.old_state == TYPEC_ATTACHED_SNK ||
			noti->typec_state.old_state == TYPEC_ATTACHED_NORP_SRC ||
			noti->typec_state.old_state == TYPEC_ATTACHED_CUSTOM_SRC ||
			noti->typec_state.old_state == TYPEC_ATTACHED_DBGACC_SNK) &&
			noti->typec_state.new_state == TYPEC_UNATTACHED) {
			dev_info(dev, "Type-C plug out\n");
			mtk_usb_extcon_set_role(extcon, USB_ROLE_NONE);
		}
		break;
	case TCP_NOTIFY_DR_SWAP:
		dev_info(dev, "%s dr_swap, new role=%d\n",
				__func__, noti->swap_state.new_role);
		if (noti->swap_state.new_role == PD_ROLE_UFP &&
				extcon->c_role != USB_ROLE_DEVICE) {
			dev_info(dev, "switch role to device\n");
			mtk_usb_extcon_set_role(extcon, USB_ROLE_NONE);
			mtk_usb_extcon_set_role(extcon, USB_ROLE_DEVICE);
		} else if (noti->swap_state.new_role == PD_ROLE_DFP &&
				extcon->c_role != USB_ROLE_HOST) {
			dev_info(dev, "switch role to host\n");
			mtk_usb_extcon_set_role(extcon, USB_ROLE_NONE);
			mtk_usb_extcon_set_role(extcon, USB_ROLE_HOST);
		}
		break;
	}

	return NOTIFY_OK;
}

static int mtk_usb_extcon_tcpc_init(struct mtk_extcon_info *extcon)
{
	struct tcpc_device *tcpc_dev;
	struct device_node *np = extcon->dev->of_node;
	const char *tcpc_name;
	int ret;

	ret = of_property_read_string(np, "tcpc", &tcpc_name);
	if (ret < 0)
		return -ENODEV;

	tcpc_dev = tcpc_dev_get_by_name(tcpc_name);
	if (!tcpc_dev) {
		dev_err(extcon->dev, "get tcpc device fail\n");
		return -ENODEV;
	}

	extcon->tcpc_nb.notifier_call = mtk_extcon_tcpc_notifier;
	ret = register_tcp_dev_notifier(tcpc_dev, &extcon->tcpc_nb,
		TCP_NOTIFY_TYPE_USB | TCP_NOTIFY_TYPE_VBUS |
		TCP_NOTIFY_TYPE_MISC);
	if (ret < 0) {
		dev_err(extcon->dev, "register notifer fail\n");
		return -EINVAL;
	}

	extcon->tcpc_dev = tcpc_dev;

	return 0;
}
#endif

static void mtk_usb_extcon_detect_cable(struct work_struct *work)
{
	struct mtk_extcon_info *extcon = container_of(to_delayed_work(work),
		struct mtk_extcon_info, wq_detcable);
	int id;

	/* check ID and update cable state */
	id = extcon->id_gpiod ?
		gpiod_get_value_cansleep(extcon->id_gpiod) : 1;

	/* at first we clean states which are no longer active */
	if (id) {
		mtk_usb_extcon_set_vbus(extcon, false);
		mtk_usb_extcon_set_role(extcon, USB_ROLE_NONE);
	} else {
		mtk_usb_extcon_set_vbus(extcon, true);
		mtk_usb_extcon_set_role(extcon, USB_ROLE_HOST);
	}
}

static irqreturn_t mtk_usb_idpin_handle(int irq, void *dev_id)
{
	struct mtk_extcon_info *extcon = dev_id;

	/* issue detection work */
	queue_delayed_work(system_power_efficient_wq, &extcon->wq_detcable, 0);

	return IRQ_HANDLED;
}

static int mtk_usb_extcon_id_pin_init(struct mtk_extcon_info *extcon)
{
	int ret = 0;
	int id;

	extcon->id_gpiod = devm_gpiod_get(extcon->dev, "id", GPIOD_IN);

	if (!extcon->id_gpiod || IS_ERR(extcon->id_gpiod)) {
		dev_info(extcon->dev, "failed to get id gpio\n");
		return -ENODEV;
	}

	extcon->id_irq = gpiod_to_irq(extcon->id_gpiod);
	if (extcon->id_irq < 0) {
		dev_info(extcon->dev, "failed to get ID IRQ\n");
		return extcon->id_irq;
	}

	INIT_DELAYED_WORK(&extcon->wq_detcable, mtk_usb_extcon_detect_cable);

	ret = devm_request_threaded_irq(extcon->dev, extcon->id_irq, NULL,
			mtk_usb_idpin_handle, IRQF_TRIGGER_RISING |
			IRQF_TRIGGER_FALLING | IRQF_ONESHOT,
			dev_name(extcon->dev), extcon);

	if (ret < 0) {
		dev_info(extcon->dev, "failed to request handler for ID IRQ\n");
		return ret;
	}

	/* get id pin value when boot on */
	id = extcon->id_gpiod ?
		gpiod_get_value_cansleep(extcon->id_gpiod) : 1;
	dev_info(extcon->dev, "id value : %d\n", id);
	if (!id) {
		mtk_usb_extcon_set_vbus(extcon, true);
		mtk_usb_extcon_set_role(extcon, USB_ROLE_HOST);
	}

	return 0;
}

#if IS_ENABLED(CONFIG_TCPC_CLASS)
#define PROC_FILE_SMT "mtk_typec"
#define FILE_SMT_U2_CC_MODE "mtk_typec/smt_u2_cc_mode"

static int usb_cc_smt_procfs_show(struct seq_file *s, void *unused)
{
	struct mtk_extcon_info *extcon = s->private;
	struct device_node *np = extcon->dev->of_node;
	const char *tcpc_name;
	uint8_t cc1, cc2;
	char buf[2];
	int ret;

	ret = of_property_read_string(np, "tcpc", &tcpc_name);
	if (ret < 0)
		return -ENODEV;

	extcon->tcpc_dev = tcpc_dev_get_by_name(tcpc_name);
	if (!extcon->tcpc_dev)
		return -ENODEV;

	tcpm_inquire_remote_cc(extcon->tcpc_dev, &cc1, &cc2, false);
	dev_info(extcon->dev, "cc1=%d, cc2=%d\n", cc1, cc2);

	if (cc1 == TYPEC_CC_VOLT_OPEN || cc1 == TYPEC_CC_DRP_TOGGLING)
		seq_puts(s, "0\n");
	else if (cc2 == TYPEC_CC_VOLT_OPEN || cc2 == TYPEC_CC_DRP_TOGGLING)
		seq_puts(s, "0\n");
	else
		seq_puts(s, "1\n");
	buf[1] = '\0';

	return 0;
}

static int usb_cc_smt_procfs_open(struct inode *inode, struct file *file)
{
	return single_open(file, usb_cc_smt_procfs_show, PDE_DATA(inode));
}

static const struct  proc_ops usb_cc_smt_procfs_fops = {
	.proc_open = usb_cc_smt_procfs_open,
	.proc_read = seq_read,
	.proc_lseek = seq_lseek,
	.proc_release = single_release,
};

static int mtk_usb_extcon_procfs_init(struct mtk_extcon_info *extcon)
{
	struct proc_dir_entry *file;
	struct proc_dir_entry *root;

	root = proc_mkdir(PROC_FILE_SMT, NULL);
	file = proc_create_data(FILE_SMT_U2_CC_MODE, 0400, NULL,
		&usb_cc_smt_procfs_fops, extcon);

	return 0;
}
#endif

static int usb_notify_callback(struct notifier_block *nb, unsigned long action, void *data)
{
    // struct usb_device *udev = data;

    switch (action) {
    case USB_DEVICE_ADD:
	    total_usb_cnt++;
       // pr_info("11111-0 USB device added: %s  %d\n", dev_name(&udev->dev), total_usb_cnt);
        break;
    case USB_DEVICE_REMOVE:
	    total_usb_cnt--;
       // pr_info("11111-1 USB device removed: %s %d\n", dev_name(&udev->dev), total_usb_cnt);
        break;
    default:
        break;
    }

    return NOTIFY_OK;
}


static struct notifier_block usb_notifier = {
    .notifier_call = usb_notify_callback,
};

#if IS_ENABLED(CONFIG_LEDS_MTK)
int led_backlight_changed_event(struct notifier_block *nb, unsigned long event, void *v)
{
	struct led_conf_info *led_conf;
    static int pre_brightness = 102; 
	led_conf = (struct led_conf_info *)v;

	switch (event) {
	case LED_BRIGHTNESS_CHANGED:
		if(pre_brightness == 0 && g_extcon->c_role == USB_ROLE_HOST){
			if(total_usb_cnt < 1){
				mtk_usb_extcon_set_vbus(g_extcon, false);
				mdelay(50);
				mtk_usb_extcon_set_vbus(g_extcon, true);
				mdelay(50);
			}		 
		}
		//printk("1111 %s, %d  g_extcon->c_role = %d\n",__func__, __LINE__,g_extcon->c_role);
		pre_brightness = led_conf->cdev.brightness;
		break;
	default:
		break;
	}
	return NOTIFY_DONE;
}

static struct notifier_block usb_init_notifier = {
	.notifier_call = led_backlight_changed_event,
};
#endif


static int mtk_usb_extcon_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct mtk_extcon_info *extcon;
#if IS_ENABLED(CONFIG_TCPC_CLASS)
	const char *tcpc_name;
#endif
	int ret;

	extcon = devm_kzalloc(&pdev->dev, sizeof(*extcon), GFP_KERNEL);
	if (!extcon)
		return -ENOMEM;

	extcon->dev = dev;

	/* extcon */
	extcon->edev = devm_extcon_dev_allocate(dev, usb_extcon_cable);
	if (IS_ERR(extcon->edev)) {
		dev_err(dev, "failed to allocate extcon device\n");
		return -ENOMEM;
	}

	ret = devm_extcon_dev_register(dev, extcon->edev);
	if (ret < 0) {
		dev_info(dev, "failed to register extcon device\n");
		return ret;
	}

	/* usb role switch */
	extcon->role_sw = usb_role_switch_get(extcon->dev);
	if (IS_ERR(extcon->role_sw)) {
		dev_err(dev, "failed to get usb role\n");
		return PTR_ERR(extcon->role_sw);
	}

	/* initial usb role */
	if (extcon->role_sw)
		extcon->c_role = USB_ROLE_NONE;

	/* vbus */
	#if IS_ENABLED(CONFIG_CHARGER_SECOND_MAINCHG)
	extcon->vbus = devm_regulator_get(dev, "usb-otg-vbus");
	if (IS_ERR(extcon->vbus)) {
		dev_err(dev, "failed to get usb-otg-vbus\n");
	#else

	extcon->vbus = devm_regulator_get(dev, "vbus");
	if (IS_ERR(extcon->vbus)) {
		dev_err(dev, "failed to get vbus\n");
	#endif
		return PTR_ERR(extcon->vbus);
	}

	/* sync vbus state */
	if (extcon->vbus) {
		extcon->vbus_on = regulator_is_enabled(extcon->vbus);
		dev_info(dev, "vbus is %s\n", extcon->vbus_on ? "on" : "off");

		if (!of_property_read_u32(dev->of_node, "vbus-voltage",
					&extcon->vbus_vol))
			dev_info(dev, "vbus-voltage=%d", extcon->vbus_vol);

		if (!of_property_read_u32(dev->of_node, "vbus-current",
					&extcon->vbus_cur))
			dev_info(dev, "vbus-current=%d", extcon->vbus_cur);
	}

	extcon->bypss_typec_sink =
		of_property_read_bool(dev->of_node,
			"mediatek,bypss-typec-sink");

#if IS_ENABLED(CONFIG_TCPC_CLASS)
	ret = of_property_read_string(dev->of_node, "tcpc", &tcpc_name);
	if (of_property_read_bool(dev->of_node, "mediatek,u2") && ret == 0
		&& strcmp(tcpc_name, "type_c_port0") == 0) {
		dev_info(dev, "create %d dir\n", PROC_FILE_SMT);
		mtk_usb_extcon_procfs_init(extcon);
	}
#endif

	extcon->extcon_wq = create_singlethread_workqueue("extcon_usb");
	if (!extcon->extcon_wq)
		return -ENOMEM;

	/* get id resources */
	ret = mtk_usb_extcon_id_pin_init(extcon);
	if (ret < 0)
		dev_info(dev, "failed to init id pin\n");

	/* power psy */
	ret = mtk_usb_extcon_psy_init(extcon);
	if (ret < 0)
		dev_err(dev, "failed to init psy\n");

#if IS_ENABLED(CONFIG_TCPC_CLASS)
	/* tcpc */
	ret = mtk_usb_extcon_tcpc_init(extcon);
	if (ret < 0)
		dev_err(dev, "failed to init tcpc\n");
#endif

	platform_set_drvdata(pdev, extcon);
    

    usb_register_notify(&usb_notifier);
    g_extcon = extcon;
	
	ret = misc_register(&extcon_vbus_device);
	if (ret) {
		printk("extcon_vbus_device register failed\n");
	}

#if IS_ENABLED(CONFIG_LEDS_MTK)
		mtk_leds_register_notifier(&usb_init_notifier);
#endif

	return 0;
}

static int mtk_usb_extcon_remove(struct platform_device *pdev)
{
	return 0;
}

static void mtk_usb_extcon_shutdown(struct platform_device *pdev)
{
	struct mtk_extcon_info *extcon = platform_get_drvdata(pdev);

	dev_info(extcon->dev, "%s\n", __func__);

	mtk_usb_extcon_set_vbus(extcon, false);
}

static const struct of_device_id mtk_usb_extcon_of_match[] = {
	{ .compatible = "mediatek,extcon-usb", },
	{ },
};
MODULE_DEVICE_TABLE(of, mtk_usb_extcon_of_match);

static struct platform_driver mtk_usb_extcon_driver = {
	.probe		= mtk_usb_extcon_probe,
	.remove		= mtk_usb_extcon_remove,
	.shutdown	= mtk_usb_extcon_shutdown,
	.driver		= {
		.name	= "mtk-extcon-usb",
		.of_match_table = mtk_usb_extcon_of_match,
	},
};

static int __init mtk_usb_extcon_init(void)
{
	return platform_driver_register(&mtk_usb_extcon_driver);
}
late_initcall(mtk_usb_extcon_init);

static void __exit mtk_usb_extcon_exit(void)
{
	platform_driver_unregister(&mtk_usb_extcon_driver);
}
module_exit(mtk_usb_extcon_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("MediaTek Extcon USB Driver");
