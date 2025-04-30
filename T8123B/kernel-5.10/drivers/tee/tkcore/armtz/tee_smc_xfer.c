/*
 * Copyright (c) 2015-2018 TrustKernel Incorporated
 * All Rights Reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */
#include <linux/spinlock.h>
#include <linux/wait.h>
#include <linux/slab.h>
#include <linux/cpu.h>
#include <linux/list.h>
#include <linux/atomic.h>
#include <linux/hrtimer.h>
#include <linux/ktime.h>

#include <linux/version.h>

#define TKCORE_BL

#ifdef TKCORE_BL
#include <linux/kthread.h>
#include <linux/cpumask.h>
#include <linux/sched.h>
#include <asm/topology.h>
#endif

#include <linux/tee_clkmgr.h>
#include <linux/tee_core.h>

#include <tee_kernel_lowlevel_api.h>
#include <arm_common/teesmc.h>
#include <arm_common/teesmc_st.h>

#ifdef TKCORE_BL

static int nr_cpus __read_mostly;

/* if a smc_task waits over 2ms
   for one available core, we give up
   and choose any core that is available
   at this moment of time */
static const s64 tee_task_timeout_us = 2000LL;

struct tee_task {
	struct smc_param *param;
	struct smc_param *last_param;

	struct list_head entry;
	struct completion task_done_comp;
};

struct tee_task_thread {
	struct task_struct *thread;
	wait_queue_head_t thread_wq;
};

#endif

struct tee_task_ctl {
	/* guarantee the mutual-exlusiveness of smc */
	struct mutex g_lock;

	/* cmds that wait for
	 * available TEE thread slots
	 */
	atomic_t nr_waiting_cmds;
	struct completion smc_comp;

#ifdef TKCORE_BL
	/*
	 * records information for the big cpus that are locked
	 * in the online state due to running smc commands
	 */
	uint32_t cpu_lock_mask;

	struct list_head cmd_head;
	spinlock_t task_lock;

	enum cpuhp_state tkcore_cpuhp_state;

	struct tee_task_thread *threads;
	int threads_count;
#endif

	/* statistics information */
	s64 max_smc_time;
	s64 max_task_time;
};

static struct tee_task_ctl tee_task_ctl;

static inline void trace_tee_smc(struct tee_task_ctl *ctl, int rv,
				 s64 time_start, s64 time_end)
{
	s64 duration = time_end - time_start;

	if (duration > 1000000LL) {
		pr_warn("WARNING SMC[0x%x] %sDURATION %lld us\n", rv,
			rv == TEESMC_RPC_FUNC_IRQ ? "IRQ " : "", duration);
	}

	/* we needn't handle concurrency here. */
	if (duration > ctl->max_smc_time)
		ctl->max_smc_time = duration;
}

static inline void trace_tee_smc_done(struct tee_task_ctl *ctl,
				s64 time_start,
				s64 time_end)
{
	s64 duration = time_end - time_start;

	if (duration > ctl->max_task_time)
		ctl->max_task_time = duration;
}

/* return 0 for nonpreempt rpc, 1 for others */
static int handle_nonpreempt_rpc(struct smc_param *p)
{
	uint32_t func_id = TEESMC_RETURN_GET_RPC_FUNC(p->a0);

#if IS_ENABLED(CONFIG_TRUSTKERNEL_TEE_FP_SUPPORT)
	/* for compatibility with legacy tee-os which
	 * does not support clkmgr
	 */
	if (func_id == T6SMC_RPC_CLKMGR_LEGACY_CMD) {
		p->a1 = tee_clkmgr_handle(p->a1, p->a2);
		return 0;
	}
#endif

	if ((func_id & 0xff) != T6SMC_RPC_NONPREEMPT_CMD)
		return 1;

	switch (T6SMC_RPC_NONPREEMPT_GET_FUNC(p->a0)) {
#if IS_ENABLED(CONFIG_TRUSTKERNEL_TEE_FP_SUPPORT)
	case T6SMC_RPC_CLKMGR_CMD:
		/* compatible with old interface */
		p->a1 = tee_clkmgr_handle(p->a1,
			(p->a1 & TEE_CLKMGR_TOKEN_NOT_LEGACY) ?
				p->a2 : (p->a2 | TEE_CLKMGR_OP_ENABLE));
		break;
#endif
	default:
		pr_err("Unknown non-preempt rpc cmd: 0x%llx\n",
			(unsigned long long) p->a0);
	}

	return 0;
}

static inline unsigned long rpc_ret_by_cmd_id(unsigned long cmd_id)
{
	return cmd_id == TEESMC32_FASTCALL_WITH_ARG ?
		 TEESMC32_FASTCALL_RETURN_FROM_RPC : TEESMC32_CALL_RETURN_FROM_RPC;
}

static void backup_call(struct smc_param *call, struct smc_param *last_call)
{
	last_call->a0 = call->a0;
	last_call->a1 = call->a1;
}

static void do_smc(struct tee_task_ctl *ctl,
				struct smc_param *call, struct smc_param *last_call)
{
	unsigned long a0 = rpc_ret_by_cmd_id(call->a0);
	s64 task_start, task_end, call_start, call_end;

	mutex_lock(&ctl->g_lock);

	task_start = ktime_to_us(ktime_get());

	for (;;) {
		/* backup a0 and a1 in case TEESMC_RETURN_EAFFINITY */
		backup_call(call, last_call);

		call_start = ktime_to_us(ktime_get());
		tee_smc_call(call);
		call_end = ktime_to_us(ktime_get());

		trace_tee_smc(ctl, TEESMC_RETURN_GET_RPC_FUNC(call->a0),
			call_start, call_end);

		if (!TEESMC_RETURN_IS_RPC(call->a0))
			goto out;

		if (handle_nonpreempt_rpc(call)) {
			if (TEESMC_RETURN_GET_RPC_FUNC(call->a0) != TEESMC_RPC_FUNC_IRQ)
				goto out;
		}

		call->a0 = a0;
	}

	task_end = ktime_to_us(ktime_get());
	trace_tee_smc_done(ctl, task_start, task_end);

out:
	mutex_unlock(&ctl->g_lock);
}

#ifdef TKCORE_BL

/* callers shall hold cpu->task_lock */
static inline void lock_cpu(struct tee_task_ctl *ctl, int cpu)
{
	ctl->cpu_lock_mask |= (1U << cpu);
	/* prevent re-ordering */
	smp_mb();
}

/* callers shall hold cpu->task_lock */
static inline void unlock_cpu(struct tee_task_ctl *ctl, int cpu)
{
	smp_mb();
	/* prevent re-ordering */
	ctl->cpu_lock_mask &= ~(1U << cpu);
}

/* can only be called in cpu hp callback or w/ ctl->task_lock */
static inline bool test_cpu_locked(struct tee_task_ctl *ctl, int cpu)
{
	return !!(ctl->cpu_lock_mask & (1U << cpu));
}

static void task_thread_pause(struct tee_task_ctl *ctl, int cpu)
{
	/*
	 * kthread_bind() will call wait_task_inactive(),
	 * which is exactly what we need to guarantee that the task
	 * is in inactive state
	 */
	kthread_bind(ctl->threads[cpu].thread, cpu);
}

#if 0
static void task_thread_resume(struct tee_task_ctl *ctl, int cpu)
{
	/* do nothing. Since the thread remains
	   on the bound CPU, it will be woken up by
	   someone who issues an smc task request.
	*/
}
#endif

static int tee_cpu_online(unsigned int cpu)
{
	(void) cpu;

	return 0;
}

static int tee_cpu_prepare_down(unsigned int cpu)
{
	struct tee_task_ctl *ctl = &tee_task_ctl;

	if (cpu >= nr_cpus) {
		pr_err("Bad cpu: %d\n", cpu);
		return -1;
	}

	if (test_cpu_locked(ctl, cpu))
		return -1;

	task_thread_pause(ctl, cpu);
	return 0;
}

/* callers shall guarantee ctl->task_lock is held */
static inline void task_enqueue(struct tee_task_ctl *ctl, struct tee_task *task)
{
	list_add_tail(&task->entry, &ctl->cmd_head);
}

/* callers shall guarantee ctl->task_lock is held */
static inline struct tee_task *task_dequeue(struct tee_task_ctl *ctl)
{
	struct tee_task *task;

	if (list_empty(&ctl->cmd_head))
		return NULL;

	task = list_first_entry(&ctl->cmd_head, struct tee_task, entry);
	list_del(&task->entry);
	return task;
}

static struct tee_task *wait_task(struct tee_task_ctl *ctl, int cpu)
{
	int idx;
	struct tee_task *task = NULL;

	spin_lock(&ctl->task_lock);
	idx = cpu;

wait_start:
	{
		DEFINE_WAIT(wait);
		prepare_to_wait(&ctl->threads[idx].thread_wq, &wait,
			TASK_INTERRUPTIBLE);

		task = task_dequeue(ctl);
		if (task)
			goto wait_end;

		unlock_cpu(ctl, cpu);
		spin_unlock(&ctl->task_lock);

		schedule();

		spin_lock(&ctl->task_lock);
		finish_wait(&ctl->threads[idx].thread_wq, &wait);
		goto wait_start;

wait_end:
		finish_wait(&ctl->threads[idx].thread_wq, &wait);
		spin_unlock(&ctl->task_lock);
	}

	return task;
}

static int tkcore_smc_task(void *data)
{
	struct tee_task_ctl *ctl = &tee_task_ctl;
	int cpu = (int) (unsigned long) data;

	if (cpu < 0 || cpu >= nr_cpus) {
		pr_err("bad cpu: %d\n", cpu);
		return 0;
	}

	while (!kthread_should_stop()) {
		struct tee_task *task;

		task = wait_task(ctl, cpu);
		do_smc(ctl, task->param, task->last_param);
		complete(&task->task_done_comp);
	}

	return 0;
}

static int find_candidate_cpu(struct tee_task_ctl *ctl,
				const struct tee_task_cpu_affinity *aff,
				bool *need_wakeup)
{
	int i, cpu;

	if (ctl->cpu_lock_mask) {
		for (i = 0; i < nr_cpus; i++) {
			if (ctl->cpu_lock_mask & (1U << i)) {
				*need_wakeup = false;
				return i;
			}
		}
	}

	for_each_online_cpu(cpu) {
		if (aff->ops->match_cpu(aff->priv, cpu)) {
			*need_wakeup = true;
			return cpu;
		}
	}

	return -1;
}

static void init_tee_task(struct tee_task *task,
				struct smc_param *param, struct smc_param *last_param)
{
	// initialize call to tee
	task->param = param;
	task->last_param = last_param;
	init_completion(&task->task_done_comp);
	INIT_LIST_HEAD(&task->entry);
}

static int post_tee_task(struct tee_task_ctl *ctl,
			const struct tee_task_cpu_affinity *aff,
			struct smc_param *call,
			struct smc_param *last_call)
{
	int picked_cpu = -1;
	bool need_wakeup = false;
	s64 tee_task_start, tee_task_current;

	struct tee_task task;

	init_tee_task(&task, call, last_call);
	tee_task_start = ktime_to_us(ktime_get());

	while (picked_cpu < 0) {
		int r, candidate_cpu;

		get_online_cpus();
		spin_lock(&ctl->task_lock);
		picked_cpu = find_candidate_cpu(ctl, aff, &need_wakeup);
		if (picked_cpu >= 0 && need_wakeup) {
			// lock cpu in ctl->task_lock to prevent it from being turned off
			lock_cpu(ctl, picked_cpu);
		}
		put_online_cpus();

		if (picked_cpu >= 0) {
			task_enqueue(ctl, &task);
			spin_unlock(&ctl->task_lock);
		} else {
			spin_unlock(&ctl->task_lock);

			tee_task_current = ktime_to_us(ktime_get());
			if (tee_task_current - tee_task_start >= tee_task_timeout_us)
				break;

			candidate_cpu = aff->ops->get_candidate_cpu(aff->priv);
			r = add_cpu(candidate_cpu);
			if (r != 0) {
				pr_warn("tkcore: cpu_on(%d) failed with ret: %d\n",
					candidate_cpu, r);
			}
		}
	}

	if (picked_cpu < 0)
		return -1;

	if (need_wakeup)
		wake_up(&ctl->threads[picked_cpu].thread_wq);

	wait_for_completion(&task.task_done_comp);
	return 0;
}

static bool tee_bind_cpu_request_need_bind_cpu(void *priv)
{
	(void) priv;
	return true;
}

static bool tee_bind_cpu_request_match_cpu(void *priv, int cpu)
{
	uint32_t cpumask = ((uint32_t) (unsigned long) priv);

	return !!(cpumask & (1U << cpu));
}

static int tee_bind_cpu_request_get_candidate_cpu(void *priv)
{
	uint32_t cpumask = ((uint32_t) (unsigned long) priv);

	if (cpumask == 0)
		return -1;

	return __builtin_ffs(cpumask) - 1;
}

const struct tee_task_cpu_affinity_operations tee_bind_cpu_request_operations = {
	.need_bind_cpu = tee_bind_cpu_request_need_bind_cpu,
	.match_cpu = tee_bind_cpu_request_match_cpu,
	.get_candidate_cpu = tee_bind_cpu_request_get_candidate_cpu,
};

static void update_tee_task_affinity(struct tee_task_cpu_affinity *aff, uint32_t cpu_mask)
{
	aff->ops = &tee_bind_cpu_request_operations;
	aff->priv = (void *) ((unsigned long) cpu_mask);
}

static void restore_call(struct smc_param *call, struct smc_param *last_call)
{
	call->a0 = last_call->a0;
	call->a1 = last_call->a1;
}

static void submit_tee_task(struct tee_task_ctl *ctl,
			struct tee_task_cpu_affinity *aff,
			struct smc_param *call)
{
	int r;
	struct smc_param last_call;
	unsigned long cmd_id = call->a0;

	for (;;) {
		r = aff->ops->need_bind_cpu(aff->priv) ?
			post_tee_task(ctl, aff, call, &last_call) : -1;
		if (r < 0)
			do_smc(ctl, call, &last_call);

		if (call->a0 == TEESMC_RETURN_TKCORE_RPC_BIND_CPU) {
			update_tee_task_affinity(aff, (uint32_t) call->a1);
			call->a0 = rpc_ret_by_cmd_id(cmd_id);
			call->a1 = 0;
		} else if (call->a0 == TEESMC_RETURN_EAFFINITY) {
			update_tee_task_affinity(aff, (uint32_t) call->a1);
			restore_call(call, &last_call);
		} else {
			break;
		}
	}
}

static int platform_bl_init(struct tee_task_ctl *ctl)
{
	int r, cpu, i = 0;

	nr_cpus = num_possible_cpus();
	if (nr_cpus > NR_CPUS) {
		pr_err("nr_cpus %d exceeds NR_CPUS %d\n", nr_cpus, NR_CPUS);
		return -1;
	}

	ctl->cpu_lock_mask = 0U;

	INIT_LIST_HEAD(&ctl->cmd_head);
	spin_lock_init(&ctl->task_lock);

	ctl->threads_count = nr_cpus;

	ctl->threads = kzalloc(sizeof(ctl->threads[0]) * nr_cpus, GFP_KERNEL);
	if (ctl->threads == NULL) {
		pr_err("bad kmalloc %zu bytes for threads\n",
			sizeof(ctl->threads[0]) * nr_cpus);
		r = -ENOMEM;
		goto err;
	}

	for_each_possible_cpu(cpu) {
		struct task_struct *task;
		init_waitqueue_head(&ctl->threads[cpu].thread_wq);

		task = kthread_create(tkcore_smc_task,
				(void *) (unsigned long) cpu,
				"tkcore_smc_task.%u", (unsigned int) cpu);
		if (IS_ERR(task)) {
			pr_err("failed to start tkcore_smc_task.%d: %p\n",
				cpu, ctl->threads[cpu].thread);
			r = -1;
			goto err;
		}

		ctl->threads[cpu].thread = task;
		kthread_bind(task, cpu);
	}

	for (i = 0; i < ctl->threads_count; i++)
		wake_up_process(ctl->threads[i].thread);

	r = cpuhp_setup_state(CPUHP_AP_ONLINE_DYN,
		"tee/tkcore_tzdrv:cpu_listener",
		tee_cpu_online, tee_cpu_prepare_down);
	if (r < 0)
		goto err;

	ctl->tkcore_cpuhp_state = r;
	return 0;

err:
	if (ctl->threads) {
		for (i = 0; i < ctl->threads_count; i++) {
			if (!IS_ERR(ctl->threads[i].thread))
				kthread_stop(ctl->threads[i].thread);
			else
				break;
		}
		kfree(ctl->threads);
		ctl->threads = NULL;
	}

	return r;
}

static void platform_bl_deinit(struct tee_task_ctl *ctl)
{
	int i;

	/* unregister cpu cpuhp before others */
	cpuhp_remove_state(ctl->tkcore_cpuhp_state);

	for (i = 0; i < ctl->threads_count; i++) {
		kthread_stop(ctl->threads[i].thread);
	}
	kfree(ctl->threads);
	ctl->threads = NULL;
}

#else

static inline void submit_tee_task(struct tee_task_ctl *ctl,
			const struct tee_task_cpu_affinity *aff,
			struct smc_param *p)
{
	struct smc_param last_call;

	(void) aff;
	do_smc(ctl, p, &last_call);
}

static int platform_bl_init(struct tee_task_ctl *ctl) { return 0; }

static void platform_bl_deinit(struct tee_task_ctl *ctl) { }

#endif

static void waiters_enqueue(struct tee_task_ctl *ctl)
{
	/*TODO handle too long time of waiting */
	atomic_inc(&ctl->nr_waiting_cmds);
	wait_for_completion(&ctl->smc_comp);
}

static void waiters_dequeue(struct tee_task_ctl *ctl)
{
	if (atomic_dec_if_positive(&ctl->nr_waiting_cmds) >= 0)
		complete(&ctl->smc_comp);
}

void run_tee_task(struct smc_param *p, struct tee_task_cpu_affinity *aff)
{
	/* NOTE!!! we remove the e_lock_teez(ptee) here !!!! */
#ifdef ARM64
	uint64_t orig_a0 = p->a0;
#else
	uint32_t orig_a0 = p->a0;
#endif
	for (;;) {
		submit_tee_task(&tee_task_ctl, aff, p);
		if (p->a0 == TEESMC_RETURN_ETHREAD_LIMIT) {
			waiters_enqueue(&tee_task_ctl);
			p->a0 = orig_a0;
		} else {
			if (!TEESMC_RETURN_IS_RPC(p->a0))
				waiters_dequeue(&tee_task_ctl);
			break;
		}
	}
}

void run_tee_task_nowait(struct smc_param *p, struct tee_task_cpu_affinity *aff)
{
	submit_tee_task(&tee_task_ctl, aff, p);
}

int tee_init_task(void)
{
	struct tee_task_ctl *ctl = &tee_task_ctl;

	mutex_init(&ctl->g_lock);

	atomic_set(&ctl->nr_waiting_cmds, 0);
	init_completion(&ctl->smc_comp);

	ctl->max_smc_time = 0LL;
	ctl->max_task_time = 0LL;

	return platform_bl_init(ctl);
}

void tee_exit_task(void)
{
	struct tee_task_ctl *ctl = &tee_task_ctl;

	platform_bl_deinit(ctl);
}
