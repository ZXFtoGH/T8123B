/* SPDX-License-Identifier: GPL-2.0 */
/*
 *	Routines to manage notifier chains for passing status changes to any
 *	interested routines. We need this instead of hard coded call lists so
 *	that modules can poke their nose into the innards. The network devices
 *	needed them so here they are for the rest of you.
 *
 *				Alan Cox <Alan.Cox@linux.org>
 */
 
#ifndef _LINUX_NOTIFIER_H
#define _LINUX_NOTIFIER_H
#include <linux/errno.h>
#include <linux/mutex.h>
#include <linux/rwsem.h>
#include <linux/srcu.h>

/*
 * Notifier chains are of four types:
 *
 *	Atomic notifier chains: Chain callbacks run in interrupt/atomic
 *		context. Callouts are not allowed to block.
 *	Blocking notifier chains: Chain callbacks run in process context.
 *		Callouts are allowed to block.
 *	Raw notifier chains: There are no restrictions on callbacks,
 *		registration, or unregistration.  All locking and protection
 *		must be provided by the caller.
 *	SRCU notifier chains: A variant of blocking notifier chains, with
 *		the same restrictions.
 *
 * atomic_notifier_chain_register() may be called from an atomic context,
 * but blocking_notifier_chain_register() and srcu_notifier_chain_register()
 * must be called from a process context.  Ditto for the corresponding
 * _unregister() routines.
 *
 * atomic_notifier_chain_unregister(), blocking_notifier_chain_unregister(),
 * and srcu_notifier_chain_unregister() _must not_ be called from within
 * the call chain.
 *
 * SRCU notifier chains are an alternative form of blocking notifier chains.
 * They use SRCU (Sleepable Read-Copy Update) instead of rw-semaphores for
 * protection of the chain links.  This means there is _very_ low overhead
 * in srcu_notifier_call_chain(): no cache bounces and no memory barriers.
 * As compensation, srcu_notifier_chain_unregister() is rather expensive.
 * SRCU notifier chains should be used when the chain will be called very
 * often but notifier_blocks will seldom be removed.
 * 
核心概念回顾
通知链 (Notifier Chain): 一种内核事件通知机制。当一个子系统（如 CPU 状态改变、网络设备上线）发生特定事件时，它可以通知所有“订阅”了该事件的其他子系统。
struct notifier_block: 代表一个“订阅者”或“观察者”。它包含一个回调函数指针、优先级和指向链表中下一个节点的指针。
通知链头 (Notifier Head): 是通知链的入口点，管理着所有注册到该链上的 notifier_block。不同的头结构体使用不同的同步机制来保护链表的并发访问（读和写）
 */

/*
总结
这段代码提供了一个灵活、高效且类型安全的内核事件通知框架：

选择合适的头类型: 根据你的使用场景（是否可睡眠、读写频率、性能要求）选择 atomic, blocking, raw, 或 srcu 类型的通知链头。
定义和初始化头: 使用相应的宏（如 ATOMIC_NOTIFIER_HEAD(my_chain);）定义并初始化一个全局的链表头。
定义并注册观察者: 创建 struct notifier_block 实例，实现你的 notifier_call 回调函数，设置 priority，然后调用 xxx_notifier_chain_register(&my_chain, &my_nb) 进行注册。
触发通知: 当事件发生时，调用 xxx_notifier_call_chain(&my_chain, action, data)。
注销: 不再需要时，调用 xxx_notifier_chain_unregister() 注销。
这种机制极大地降低了内核各子系统之间的耦合度，是 Linux 内核设计中一个非常经典的解耦模式。


使用示例：
// 1. 定义回调函数
int my_notifier_cb(struct notifier_block *nb, unsigned long action, void *data)
{
    if (action == EVENT_TYPE) {
        // 处理事件
        return NOTIFY_OK;
    }
    return NOTIFY_DONE;
}

// 2. 定义 notifier_block
static struct notifier_block my_nb = {
    .notifier_call = my_notifier_cb,
    .priority = 100,
};

// 3. 注册到通知链（在模块初始化时）
atomic_notifier_chain_register(&some_atomic_chain, &my_nb);

// 4. 注销（在模块退出时）
atomic_notifier_chain_unregister(&some_atomic_chain, &my_nb);
*/

 /*
 这是一个结构体的前向声明（Forward Declaration）。
它告诉编译器存在一个名为 notifier_block 的结构体类型。
这样做是为了在定义 notifier_fn_t 类型时，可以在函数指针参数中使用 struct notifier_block *nb，
即使此时 notifier_block 的完整定义尚未给出。这解决了函数指针参数类型依赖于尚未定义的结构体的问题。

简单来说，struct notifier_block 就像一个“订阅者”卡片，上面写着（notifier_call）“当发生XX事时请打这个电话”，
卡片按重要性排序（priority），所有卡片串成一串（next 指针）。当“XX事”（action）发生时，相关人员（data）的信
息会沿着这串卡片依次传递，并拨打电话通知每个订阅者。
*/
struct notifier_block;

/*
这行代码定义了一个函数指针类型，名为 notifier_fn_t。
typedef: 用于创建类型别名。
int (*notifier_fn_t)(...): 定义了一个指向函数的指针类型 notifier_fn_t。
函数签名:
返回值: int。通常，通知函数返回 NOTIFY_OK (0) 表示成功处理，NOTIFY_BAD (-1) 表示处理失败，或者 NOTIFY_STOP_MASK (0x80) 等值来控制通知链的后续执行。
参数:
struct notifier_block *nb: 指向调用该函数的 notifier_block 结构体本身的指针。这使得函数可以访问自己的数据（比如 priority 或嵌入该结构的更大结构体）。
unsigned long action: 一个数值，表示具体发生的事件或动作。例如，在 CPU 热插拔通知链中，action 可能是 CPU_UP_PREPARE (准备上线)、CPU_ONLINE (已上线)、CPU_DOWN_PREPARE (准备下线)、CPU_DEAD (已下线) 等。通知函数根据 action 值来决定执行何种逻辑。
void *data: 一个指向与事件相关的数据的指针。其具体类型取决于通知链的类型。例如，在 CPU 热插拔中，data 可能指向一个包含 CPU 编号的 unsigned long 变量；在网络设备通知链中，data 可能指向一个 struct net_device 结构体。接收者需要根据 action 和通知链的上下文来正确解析这个指针。
*/
typedef	int (*notifier_fn_t)(struct notifier_block *nb,
			unsigned long action, void *data);

/*
这是通知链机制的核心数据结构，代表了一个注册到通知链上的观察者（或监听者）。
notifier_fn_t notifier_call;
	这是一个 notifier_fn_t 类型的函数指针。
	它指向当事件发生时，需要被调用的回调函数。
	当通知链被触发（call_notifier_chain 或类似函数被调用）时，链上每个 notifier_block 的 notifier_call 函数都会被依次调用，并传入 action 和 data 参数。
struct notifier_block __rcu *next;
	这是一个指向下一个 notifier_block 结构体的指针。
	多个 notifier_block 通过 next 指针链接起来，形成一个单向链表，这个链表就是“通知链”。
	__rcu 是一个内核注释（注解），表明这个指针在 RCU (Read-Copy-Update) 机制下被访问。RCU 是一种内核同步机制，允许多个读者同时安全地遍历链表，而写者（添加/删除节点）可以在不阻塞读者的情况下进行修改，从而在读多写少的场景下提供高性能。这暗示了通知链的遍历（读操作）非常频繁，而注册/注销（写操作）相对较少。
int priority;
	这个字段表示该通知处理函数的优先级。
	当事件发生时，通知链上的处理函数会按照 priority 的值从高到低的顺序被调用。
	优先级高的处理函数先执行。这允许关键的、必须首先处理的逻辑被优先执行。
	通常，优先级数值越大，优先级越高（但具体实现可能有差异，需看具体通知链的排序规则）。
*/
struct notifier_block {
	notifier_fn_t notifier_call;
	struct notifier_block __rcu *next;
	int priority;
};

/*
atomic_notifier_head (原子通知链)
同步机制: 自旋锁 (spinlock_t)。
特点:
	原子上下文: 通知链的调用者（触发事件的代码）和注册/注销者（添加/移除订阅者的代码）都必须在原子上下文（不能睡眠，如中断处理程序、软中断、自旋锁持有期间）运行。
	高性能: 自旋锁在短时间临界区下性能很好。
	限制: 由于不能睡眠，回调函数 (notifier_call) 也必须是原子的，不能进行可能导致睡眠的操作（如内存分配、访问用户空间、调用 schedule()）。
适用场景: 事件触发非常频繁，且需要在中断上下文处理的场景。
*/
struct atomic_notifier_head {
	spinlock_t lock;
	struct notifier_block __rcu *head;
};

/*
blocking_notifier_head (阻塞通知链)
同步机制: 读写信号量 (rw_semaphore)。
特点:
	可睡眠上下文: 通知链的调用者和注册/注销者可以在进程上下文（可以睡眠）运行。
	读写分离: 多个读操作（调用通知链）可以并发进行（共享读锁），但写操作（注册/注销）需要独占锁。这提高了读多写少场景下的并发性。
	灵活性: 回调函数可以执行可能导致睡眠的操作。
适用场景: 事件触发不那么频繁，且回调函数可能需要执行较重或可睡眠操作的场景。
*/
struct blocking_notifier_head {
	struct rw_semaphore rwsem;
	struct notifier_block __rcu *head;
};

/*
raw_notifier_head (原始通知链)
同步机制: 无内置同步。
特点:
	裸通知链: 结构体本身不包含任何锁。同步责任完全由使用者承担。
	高性能: 没有锁开销，性能最高。
	高风险: 使用者必须确保在遍历 (call_chain) 和修改 (register/unregister) 链表时，通过外部机制（如已经持有的锁）保证线程安全。
适用场景: 使用者已经在一个大锁的保护范围内，或者有非常特殊的同步需求。
*/
struct raw_notifier_head {
	struct notifier_block __rcu *head;
};

/*
srcu_notifier_head (SRCU通知链)
同步机制: SRCU (Sleepable Read-Copy-Update)。
特点:
	可睡眠的读操作: 这是 SRCU 的最大优势。通知链的调用者（读方）可以在可睡眠的上下文中运行，并且不会阻塞写方（注册/注销）。
	高性能读: 读操作（调用通知链）非常轻量，通常只是一个内存读取和一个 per-cpu 计数器增加。
	延迟释放: 写方（注销）删除节点后，需要等待一个“宽限期”（Grace Period），确保所有可能正在遍历旧链表的读方都已完成，然后才能安全释放被删除节点的内存。srcu_struct 用于管理这个宽限期。
	互斥写: mutex 用于保护链表的修改操作（注册/注销），确保同一时间只有一个写方。
适用场景: 读操作（调用通知链）极其频繁，而写操作（注册/注销）相对较少的场景。能提供比 blocking_notifier_head 更好的读性能。
*/
struct srcu_notifier_head {
	struct mutex mutex;
	struct srcu_struct srcu;
	struct notifier_block __rcu *head;
};

//在代码中动态初始化一个 atomic_notifier_head，初始化自旋锁并设置 head 为 NULL。
#define ATOMIC_INIT_NOTIFIER_HEAD(name) do {	\
		spin_lock_init(&(name)->lock);	\
		(name)->head = NULL;		\
	} while (0)
//动态初始化 blocking_notifier_head，初始化读写信号量并设置 head 为 NULL。
#define BLOCKING_INIT_NOTIFIER_HEAD(name) do {	\
		init_rwsem(&(name)->rwsem);	\
		(name)->head = NULL;		\
	} while (0)
//动态初始化 raw_notifier_head，仅设置 head 为 NULL（无锁）。
#define RAW_INIT_NOTIFIER_HEAD(name) do {	\
		(name)->head = NULL;		\
	} while (0)

/* srcu_notifier_heads must be cleaned up dynamically */
//srcu_init_notifier_head(nh): 外部函数，用于动态初始化 srcu_notifier_head，需要调用 init_srcu_struct() 来初始化 srcu 成员。srcu_cleanup_notifier_head(name) 用于清理。
extern void srcu_init_notifier_head(struct srcu_notifier_head *nh);
#define srcu_cleanup_notifier_head(name)	\
		cleanup_srcu_struct(&(name)->srcu);

/*
ATOMIC_NOTIFIER_INIT(name) / BLOCKING_NOTIFIER_INIT(name) / RAW_NOTIFIER_INIT(name) / SRCU_NOTIFIER_INIT(name, pcpu): 
用于在声明结构体变量时进行静态初始化的宏。它们使用内核的初始化宏（如 __SPIN_LOCK_UNLOCKED, __RWSEM_INITIALIZER）来初始化锁和 srcu 结构。
*/
#define ATOMIC_NOTIFIER_INIT(name) {				\
		.lock = __SPIN_LOCK_UNLOCKED(name.lock),	\
		.head = NULL }
#define BLOCKING_NOTIFIER_INIT(name) {				\
		.rwsem = __RWSEM_INITIALIZER((name).rwsem),	\
		.head = NULL }
#define RAW_NOTIFIER_INIT(name)	{				\
		.head = NULL }

#define SRCU_NOTIFIER_INIT(name, pcpu)				\
	{							\
		.mutex = __MUTEX_INITIALIZER(name.mutex),	\
		.head = NULL,					\
		.srcu = __SRCU_STRUCT_INIT(name.srcu, pcpu),	\
	}

/*
ATOMIC_NOTIFIER_HEAD(name) / BLOCKING_NOTIFIER_HEAD(name) / RAW_NOTIFIER_HEAD(name) / SRCU_NOTIFIER_HEAD(name): 
这些宏结合了结构体声明和静态初始化。例如，ATOMIC_NOTIFIER_HEAD(foo); 等价于 struct atomic_notifier_head foo = ATOMIC_NOTIFIER_INIT(foo);。
SRCU_NOTIFIER_HEAD_STATIC 用于声明 static 的 SRCU 头。
*/
#define ATOMIC_NOTIFIER_HEAD(name)				\
	struct atomic_notifier_head name =			\
		ATOMIC_NOTIFIER_INIT(name)
#define BLOCKING_NOTIFIER_HEAD(name)				\
	struct blocking_notifier_head name =			\
		BLOCKING_NOTIFIER_INIT(name)
#define RAW_NOTIFIER_HEAD(name)					\
	struct raw_notifier_head name =				\
		RAW_NOTIFIER_INIT(name)

#ifdef CONFIG_TREE_SRCU
#define _SRCU_NOTIFIER_HEAD(name, mod)				\
	static DEFINE_PER_CPU(struct srcu_data, name##_head_srcu_data); \
	mod struct srcu_notifier_head name =			\
			SRCU_NOTIFIER_INIT(name, name##_head_srcu_data)

#else
#define _SRCU_NOTIFIER_HEAD(name, mod)				\
	mod struct srcu_notifier_head name =			\
			SRCU_NOTIFIER_INIT(name, name)

#endif

#define SRCU_NOTIFIER_HEAD(name)				\
	_SRCU_NOTIFIER_HEAD(name, /* not static */)

#define SRCU_NOTIFIER_HEAD_STATIC(name)				\
	_SRCU_NOTIFIER_HEAD(name, static)

#ifdef __KERNEL__

/*
3. 操作函数声明
这些是实际用于操作通知链的内核 API 函数。

注册/注销:
	xxx_notifier_chain_register(struct xxx_notifier_head *nh, struct notifier_block *nb): 将 nb 按 priority 顺序插入到 nh 指向的链表中。
	xxx_notifier_chain_unregister(...): 从链表中移除指定的 nb。
调用 (触发通知):
	xxx_notifier_call_chain(struct xxx_notifier_head *nh, unsigned long val, void *v): 遍历 nh 链表上的所有 notifier_block，调用它们的 notifier_call 函数，并传入 val (action) 和 v (data)。返回值通常是链上所有回调函数返回值的聚合（如遇到 NOTIFY_BAD 或 NOTIFY_STOP 则停止并返回）。
健壮调用 (Robust Call):
	xxx_notifier_call_chain_robust(...): 一种特殊的调用模式，通常用于状态转换（如 UP/DOWN）。它会先用 val_up 调用所有注册者，如果某个返回 NOTIFY_BAD，则用 val_down 调用已经成功处理了 val_up 的注册者进行回滚。这确保了状态转换的原子性。
*/
extern int atomic_notifier_chain_register(struct atomic_notifier_head *nh,
		struct notifier_block *nb);
extern int blocking_notifier_chain_register(struct blocking_notifier_head *nh,
		struct notifier_block *nb);
extern int raw_notifier_chain_register(struct raw_notifier_head *nh,
		struct notifier_block *nb);
extern int srcu_notifier_chain_register(struct srcu_notifier_head *nh,
		struct notifier_block *nb);

extern int atomic_notifier_chain_unregister(struct atomic_notifier_head *nh,
		struct notifier_block *nb);
extern int blocking_notifier_chain_unregister(struct blocking_notifier_head *nh,
		struct notifier_block *nb);
extern int raw_notifier_chain_unregister(struct raw_notifier_head *nh,
		struct notifier_block *nb);
extern int srcu_notifier_chain_unregister(struct srcu_notifier_head *nh,
		struct notifier_block *nb);

extern int atomic_notifier_call_chain(struct atomic_notifier_head *nh,
		unsigned long val, void *v);
extern int blocking_notifier_call_chain(struct blocking_notifier_head *nh,
		unsigned long val, void *v);
extern int raw_notifier_call_chain(struct raw_notifier_head *nh,
		unsigned long val, void *v);
extern int srcu_notifier_call_chain(struct srcu_notifier_head *nh,
		unsigned long val, void *v);

extern int atomic_notifier_call_chain_robust(struct atomic_notifier_head *nh,
		unsigned long val_up, unsigned long val_down, void *v);
extern int blocking_notifier_call_chain_robust(struct blocking_notifier_head *nh,
		unsigned long val_up, unsigned long val_down, void *v);
extern int raw_notifier_call_chain_robust(struct raw_notifier_head *nh,
		unsigned long val_up, unsigned long val_down, void *v);

/*
4. 返回值宏
定义了回调函数返回的标准状态码：
NOTIFY_DONE (0x0000): “我不关心这个事件”或“处理完成，但无特别状态”。
NOTIFY_OK (0x0001): “处理成功，一切正常”。
NOTIFY_STOP_MASK (0x8000): 一个掩码，表示“停止后续调用”。
NOTIFY_BAD (0x8002): “处理失败，这是一个坏/Veto 操作”，通常会导致整个通知链停止，并可能触发回滚。
NOTIFY_STOP (0x8001): “处理成功，但请停止调用链上后续的处理函数”。这是 NOTIFY_OK | NOTIFY_STOP_MASK。
notifier_from_errno(err) / notifier_to_errno(ret): 工具函数，用于在内核错误码（如 -ENOMEM, -EPERM）和通知链返回值之间进行转换。
*/
#define NOTIFY_DONE		0x0000		/* Don't care */
#define NOTIFY_OK		0x0001		/* Suits me */
#define NOTIFY_STOP_MASK	0x8000		/* Don't call further */
#define NOTIFY_BAD		(NOTIFY_STOP_MASK|0x0002)
						/* Bad/Veto action */
/*
 * Clean way to return from the notifier and stop further calls.
 */
#define NOTIFY_STOP		(NOTIFY_OK|NOTIFY_STOP_MASK)

/* Encapsulate (negative) errno value (in particular, NOTIFY_BAD <=> EPERM).  // 将错误码转换为通知返回值 */
static inline int notifier_from_errno(int err)
{
	if (err)
		return NOTIFY_STOP_MASK | (NOTIFY_OK - err);

	return NOTIFY_OK;
}

/* Restore (negative) errno value from notify return value. // 从通知返回值中提取错误码 */
static inline int notifier_to_errno(int ret)
{
	ret &= ~NOTIFY_STOP_MASK;
	return ret > NOTIFY_OK ? NOTIFY_OK - ret : 0;
}

/*
 *	Declared notifiers so far. I can imagine quite a few more chains
 *	over time (eg laptop power reset chains, reboot chain (to clean 
 *	device units up), device [un]mount chain, module load/unload chain,
 *	low memory chain, screenblank chain (for plug in modular screenblankers) 
 *	VC switch chains (for loadable kernel svgalib VC switch helpers) etc...
 */
 
/* CPU notfiers are defined in include/linux/cpu.h. */

/* netdevice notifiers are defined in include/linux/netdevice.h */

/* reboot notifiers are defined in include/linux/reboot.h. */

/* Hibernation and suspend events are defined in include/linux/suspend.h. */

/* Virtual Terminal events are defined in include/linux/vt.h. */

#define NETLINK_URELEASE	0x0001	/* Unicast netlink socket released */

/* Console keyboard events.
 * Note: KBD_KEYCODE is always sent before KBD_UNBOUND_KEYCODE, KBD_UNICODE and
 * KBD_KEYSYM. */
#define KBD_KEYCODE		0x0001 /* Keyboard keycode, called before any other */
#define KBD_UNBOUND_KEYCODE	0x0002 /* Keyboard keycode which is not bound to any other */
#define KBD_UNICODE		0x0003 /* Keyboard unicode */
#define KBD_KEYSYM		0x0004 /* Keyboard keysym */
#define KBD_POST_KEYSYM		0x0005 /* Called after keyboard keysym interpretation */

//extern struct blocking_notifier_head reboot_notifier_list;: 这是一个内核中实际存在的全局通知链实例。当系统准备重启时，会调用 blocking_notifier_call_chain(&reboot_notifier_list, SYS_RESTART, NULL)，通知所有注册了重启事件的驱动或子系统进行清理工作。
extern struct blocking_notifier_head reboot_notifier_list;

#endif /* __KERNEL__ */
#endif /* _LINUX_NOTIFIER_H */