// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2019 MediaTek Inc.
 * Author Wy Chuang<wy.chuang@mediatek.com>
 */

 /*
功能：提供一组通用接口供不同充电算法使用，包括注册、注销、属性设置、事件通知等。
设备模型：通过 class_create() 创建了一个名为 "Charger Algorithm" 的设备类，并实现了相关 sysfs 接口。
通知机制：基于内核的通知链（notifier chain），允许其他模块监听充电算法的事件（如插拔、满电等）。
导出符号：所有关键函数都通过 EXPORT_SYMBOL 导出，可供其他模块调用。

六、核心设计特点
面向对象设计：
	通过struct chg_alg_device封装设备状态
	通过struct chg_alg_ops定义算法操作接口
	实现了 "接口与实现分离" 的设计原则
事件通知机制：
	使用notifier_block实现观察者模式
	支持异步事件通知（如充电状态变化）
灵活的加载方式：
	支持作为内置模块或可加载模块两种部署方式
	通过条件编译适配不同场景
错误处理机制：
	严格检查指针有效性
	对未实现的接口返回标准错误码
	提供清晰的错误日志
总结
这段代码实现了 MediaTek 充电算法的核心框架，定义了一套标准化的充电算法接口。它通过设备类机制将充电算法抽象为内核设备，提供了统一的管理接口。该模块既可以作为内置模块在内核启动时初始化，也可以作为可加载模块动态加载，体现了 Linux 内核驱动开发的灵活性和规范性。

实际使用时，具体的充电算法（如 PE5、PD 等）需要实现struct chg_alg_ops中的接口，并通过chg_alg_device_register注册到系统中，从而接入这套充电管理框架。
 */
#include <linux/module.h>
#include <linux/stat.h>
#include <linux/init.h>
#include <linux/ctype.h>
#include <linux/err.h>
#include <linux/slab.h>
#include "mtk_charger_algorithm_class.h"


static struct class *charger_algorithm_class;

/*
将每个事件编号（如 EVT_PLUG_IN）映射为字符串名，方便调试打印。
*/
static const char *const chg_alg_notify_evt_name[EVT_MAX] = {
	[EVT_PLUG_IN] = "EVT_PLUG_IN",
	[EVT_PLUG_OUT] = "EVT_PLUG_OUT",
	[EVT_FULL] = "EVT_FULL",
	[EVT_RECHARGE] = "EVT_RECHARGE",
	[EVT_DETACH] = "EVT_DETACH",
	[EVT_HARDRESET] = "EVT_HARDRESET",
	[EVT_VBUSOVP] = "EVT_VBUSOVP",
	[EVT_IBUSOCP] = "EVT_IBUSOCP",
	[EVT_IBUSUCP_FALL] = "EVT_IBUSUCP_FALL",
	[EVT_VBATOVP] = "EVT_VBATOVP",
	[EVT_IBATOCP] = "EVT_IBATOCP",
	[EVT_VOUTOVP] = "EVT_VOUTOVP",
	[EVT_VDROVP] = "EVT_VDROVP",
	[EVT_VBATOVP_ALARM] = "EVT_VBATOVP_ALARM",
	[EVT_VBUSOVP_ALARM] = "EVT_VBUSOVP_ALARM",
	[EVT_BATPRO_DONE] = "EVT_BATPRO_DONE",
	[EVT_ALGO_STOP] = "EVT_ALGO_STOP",
};

static void chg_alg_device_release(struct device *dev)
{
	struct chg_alg_device *chg_dev = to_chg_alg_dev(dev);

	kfree(chg_dev);
}

int chg_alg_init_algo(struct chg_alg_device *alg_dev)
{
	if (alg_dev != NULL && alg_dev->ops != NULL &&
	    alg_dev->ops->init_algo)
		return alg_dev->ops->init_algo(alg_dev);

	return -EOPNOTSUPP;
}
EXPORT_SYMBOL(chg_alg_init_algo);

int chg_alg_is_algo_ready(struct chg_alg_device *alg_dev)
{
	if (alg_dev != NULL && alg_dev->ops != NULL &&
	    alg_dev->ops->is_algo_ready)
		return alg_dev->ops->is_algo_ready(alg_dev);

	return -EOPNOTSUPP;
}
EXPORT_SYMBOL(chg_alg_is_algo_ready);

int chg_alg_is_algo_running(struct chg_alg_device *alg_dev)
{
	if (alg_dev != NULL && alg_dev->ops != NULL &&
	    alg_dev->ops->is_algo_running)
		return alg_dev->ops->is_algo_running(alg_dev);

	return -EOPNOTSUPP;
}
EXPORT_SYMBOL(chg_alg_is_algo_running);

int chg_alg_start_algo(struct chg_alg_device *alg_dev)
{
	if (alg_dev != NULL && alg_dev->ops != NULL &&
	    alg_dev->ops->start_algo)
		return alg_dev->ops->start_algo(alg_dev);

	return -EOPNOTSUPP;
}
EXPORT_SYMBOL(chg_alg_start_algo);

/*
函数功能
	该函数的主要功能是从特定的充电算法中获取指定属性的值，使系统能够了解算法的状态和能力。

参数说明：
	alg_dev：指向充电算法设备的指针
	s：属性类型，通过enum chg_alg_props枚举定义
	value：输出参数，用于存储获取到的属性值
操作函数指针：
	alg_dev->ops：指向算法操作函数集的指针
	alg_dev->ops->get_prop：指向具体算法的属性获取函数
错误处理：
	如果算法设备或其操作函数无效，返回-EOPNOTSUPP（不支持的操作）
	这确保了代码的健壮性，防止空指针引用
*/
int chg_alg_get_prop(struct chg_alg_device *alg_dev,
	enum chg_alg_props s, int *value)
{
	// 检查算法设备结构体和其操作函数指针是否有效
	if (alg_dev != NULL && alg_dev->ops != NULL &&
	    alg_dev->ops->get_prop)
		return alg_dev->ops->get_prop(alg_dev, s, value);	// 调用具体算法的get_prop函数,该实现会将算法支持的 s 值存入value

	return -EOPNOTSUPP;
}
EXPORT_SYMBOL(chg_alg_get_prop);

int chg_alg_set_prop(struct chg_alg_device *alg_dev,
	enum chg_alg_props s, int value)
{
	if (alg_dev != NULL && alg_dev->ops != NULL &&
	    alg_dev->ops->set_prop)
		return alg_dev->ops->set_prop(alg_dev, s, value);

	return -EOPNOTSUPP;
}
EXPORT_SYMBOL(chg_alg_set_prop);

int chg_alg_stop_algo(struct chg_alg_device *alg_dev)
{
	if (alg_dev != NULL && alg_dev->ops != NULL &&
	    alg_dev->ops->stop_algo)
		return alg_dev->ops->stop_algo(alg_dev);

	return -EOPNOTSUPP;
}
EXPORT_SYMBOL(chg_alg_stop_algo);

/*
实现逻辑与流程
参数有效性检查：
	检查alg_dev指针是否为空
	检查设备操作接口alg_dev->ops是否初始化
	检查notifier_call回调函数是否存在
事件分发机制：
	若参数有效，调用具体算法实现的notifier_call回调函数
	该回调函数负责将notify中的事件分发给所有注册的观察者
错误处理：
	若任意检查失败，返回-EOPNOTSUPP（不支持的操作）
	确保代码健壮性，避免空指针解引用
*/
int chg_alg_notifier_call(struct chg_alg_device *alg_dev,
	struct chg_alg_notify *notify)
{
	if (alg_dev != NULL && alg_dev->ops != NULL &&
	    alg_dev->ops->notifier_call)
		return alg_dev->ops->notifier_call(alg_dev, notify);

	return -EOPNOTSUPP;
}
EXPORT_SYMBOL(chg_alg_notifier_call);

int chg_alg_set_current_limit(struct chg_alg_device *alg_dev,
	struct chg_limit_setting *setting)
{
	pr_notice("%s\n", __func__);
	if (alg_dev != NULL && alg_dev->ops != NULL &&
	    alg_dev->ops->set_current_limit)
		return alg_dev->ops->set_current_limit(alg_dev, setting);

	return -EOPNOTSUPP;
}
EXPORT_SYMBOL(chg_alg_set_current_limit);

char *chg_alg_state_to_str(int state)
{
	switch (state) {
	case ALG_INIT_FAIL:
		return "ALG_INIT_FAIL";
	case ALG_TA_CHECKING:
		return "ALG_TA_CHECKING";
	case ALG_TA_NOT_SUPPORT:
		return "ALG_TA_NOT_SUPPORT";
	case ALG_NOT_READY:
		return "ALG_NOT_READY";
	case ALG_WAIVER:
		return "ALG_WAIVER";
	case ALG_READY:
		return "ALG_READY";
	case ALG_RUNNING:
		return "ALG_RUNNING";
	case ALG_DONE:
		return "ALG_DONE";
	default:
		break;
	}
	pr_notice("%s unknown state:%d\n", __func__, state);
	return "chg_alg_state_UNKNOWN";
}
EXPORT_SYMBOL(chg_alg_state_to_str);

extern const char *const
chg_alg_notify_evt_tostring(enum chg_alg_notifier_events evt)
{
	if ((int)evt >= (int)EVT_MAX || (int)evt < 0) {
		pr_notice("%s: chg_algo error\n", __func__);
		return "chg_algo_error";
	}
	return chg_alg_notify_evt_name[evt];

}
EXPORT_SYMBOL(chg_alg_notify_evt_tostring);

int register_chg_alg_notifier(struct chg_alg_device *alg_dev,
				struct notifier_block *nb)
{
	int ret;

	ret = srcu_notifier_chain_register(&alg_dev->evt_nh, nb);
	return ret;
}
EXPORT_SYMBOL(register_chg_alg_notifier);

int unregister_chg_alg_notifier(struct chg_alg_device *alg_dev,
				struct notifier_block *nb)
{
	return srcu_notifier_chain_unregister(&alg_dev->evt_nh, nb);
}
EXPORT_SYMBOL(unregister_chg_alg_notifier);

/**
 * chg_alg_device_register - create and register a new object of
 *   charger_device class.
 * @name: the name of the new object
 * @parent: a pointer to the parent device
 * @devdata: an optional pointer to be stored for private driver use.
 * The methods may retrieve it by using charger_get_data(charger_dev).
 * @ops: the charger operations structure.
 *
 * Creates and registers new charger device. Returns either an
 * ERR_PTR() or a pointer to the newly allocated device.
 */
//可参考charger_class.c的介绍
struct chg_alg_device *chg_alg_device_register(const char *name,
		struct device *parent, void *devdata,
		const struct chg_alg_ops *ops,
		const struct chg_alg_properties *props)
{
	struct chg_alg_device *chg_dev;
	static struct lock_class_key key;
	struct srcu_notifier_head *head;
	int rc;
	char *algo_name = NULL;

	pr_debug("%s: name=%s\n", __func__, name);
	chg_dev = kzalloc(sizeof(*chg_dev), GFP_KERNEL);	// 分配设备结构体
	if (!chg_dev)
		return ERR_PTR(-ENOMEM);

	head = &chg_dev->evt_nh;
	srcu_init_notifier_head(head);
	/* Rename srcu's lock to avoid LockProve warning // 初始化通知链和互斥锁*/
	lockdep_init_map(&(&head->srcu)->dep_map, name, &key, 0);
	mutex_init(&chg_dev->ops_lock);
	// 配置设备属性
	chg_dev->dev.class = charger_algorithm_class;
	chg_dev->dev.parent = parent;
	chg_dev->dev.release = chg_alg_device_release;
	algo_name = kasprintf(GFP_KERNEL, "%s", name);
	dev_set_name(&chg_dev->dev, algo_name);
	dev_set_drvdata(&chg_dev->dev, devdata);
	kfree(algo_name);

	/* Copy properties */
	if (props) {
		memcpy(&chg_dev->props, props,
		       sizeof(struct chg_alg_properties));
	}
	// 注册设备到系统
	rc = device_register(&chg_dev->dev);
	if (rc) {
		kfree(chg_dev);
		return ERR_PTR(rc);
	}
	chg_dev->ops = ops;
	return chg_dev;
}
EXPORT_SYMBOL(chg_alg_device_register);

/**
 * chg_alg_device_unregister - unregisters a switching charger device
 * object.
 * @charger_dev: the switching charger device object to be unregistered
 * and freed.
 *
 * Unregisters a previously registered via charger_device_register object.
 */
void chg_alg_device_unregister(struct chg_alg_device *chg_dev)
{
	if (!chg_dev)
		return;

	mutex_lock(&chg_dev->ops_lock);
	chg_dev->ops = NULL;
	mutex_unlock(&chg_dev->ops_lock);
	device_unregister(&chg_dev->dev);
}
EXPORT_SYMBOL(chg_alg_device_unregister);

static int chg_alg_match_device_by_name(struct device *dev,
	const void *data)
{
	const char *name = data;

	return strcmp(dev_name(dev), name) == 0;
}

/*
功能：根据名称查找并返回充电算法设备指针
参数：
name：要查找的充电算法设备名称（如 "pe5"）
返回值：
成功：指向struct chg_alg_device的指针
失败：NULL
*/
struct chg_alg_device *get_chg_alg_by_name(const char *name)
{
	struct device *dev;

	if (!name)
		return (struct chg_alg_device *)NULL;
	dev = class_find_device(charger_algorithm_class, NULL, name,
				chg_alg_match_device_by_name);

	return dev ? to_chg_alg_dev(dev) : NULL;

}
EXPORT_SYMBOL(get_chg_alg_by_name);

static void __exit charger_algorithm_class_exit(void)
{
	class_destroy(charger_algorithm_class);
}

static int __init charger_algorithm_class_init(void)
{
	/*
	t7820_l40mme_T1036_64:/sys/class/Charger Algorithm # ls
	pd  pe  pe2
	*/
	charger_algorithm_class = class_create(THIS_MODULE, "Charger Algorithm");
	if (IS_ERR(charger_algorithm_class)) {
		pr_notice("Unable to create charger algorithm class; errno = %ld\n",
			PTR_ERR(charger_algorithm_class));
		return PTR_ERR(charger_algorithm_class);
	}
	//charger_algorithm_class->dev_groups = adapter_groups;
	//charger_algorithm_class->suspend = charger_algorithm_suspend;
	//charger_algorithm_class->resume = charger_algorithm_resume;
	return 0;
}

#if IS_BUILTIN(CONFIG_MTK_CHARGER)
subsys_initcall(charger_algorithm_class_init);
#else
module_init(charger_algorithm_class_init);
#endif
module_exit(charger_algorithm_class_exit);

MODULE_DESCRIPTION("Charger Algorithm Class Device");
MODULE_AUTHOR("wy.chuang <wy.chuang@mediatek.com>");
MODULE_VERSION("1.0.0");
MODULE_LICENSE("GPL");
