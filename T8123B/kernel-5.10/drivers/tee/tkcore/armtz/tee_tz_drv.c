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

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/io.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include <linux/version.h>
#include <linux/init.h>

#ifdef CONFIG_OF
#include <linux/of.h>
#include <linux/of_irq.h>
#include <linux/of_reserved_mem.h>
#endif

#include <linux/tee_core.h>
#include <linux/tee_ioc.h>

#include <linux/tee_clkmgr.h>

#include <tee_shm.h>
#include <tee_supp_com.h>
#include <tee_wait_queue.h>

#include <arm_common/teesmc.h>
#include <arm_common/teesmc_st.h>

#include "asm/io.h"
#include "tee_mem.h"
#include "tee_tz_op.h"
#include "tee_tz_priv.h"
#include "handle.h"
#include "tee_smc_xfer.h"

#include "tee_procfs.h"

#define _TEE_TZ_NAME "tkcoredrv"
#define DEV (ptee->tee->dev)

/* #define TEE_STRESS_OUTERCACHE_FLUSH */

/* magic config: bit 1 is set, Secure TEE shall handler NSec IRQs */
#define SEC_ROM_NO_FLAG_MASK	0x0000
#define SEC_ROM_IRQ_ENABLE_MASK	0x0001
#define SEC_ROM_DEFAULT		SEC_ROM_IRQ_ENABLE_MASK
#define TEE_RETURN_BUSY		0x3
#define ALLOC_ALIGN		PAGE_SIZE

#define CAPABLE(tee) !(tee->conf & TEE_CONF_FW_NOT_CAPABLE)

static struct tee_tz *tee_tz;

static struct handle_db shm_handle_db = HANDLE_DB_INITIALIZER;

/*******************************************************************
 * Calling TEE
 *******************************************************************/

static void handle_rpc_func_cmd_wait_queue(struct tee_tz *ptee,
		struct teesmc32_arg *arg32)
{
	struct teesmc32_param *params;

	if (arg32->num_params != 2)
		goto bad;

	params = TEESMC32_GET_PARAMS(arg32);

	if ((params[0].attr & TEESMC_ATTR_TYPE_MASK) !=
		TEESMC_ATTR_TYPE_VALUE_INPUT)
		goto bad;
	if ((params[1].attr & TEESMC_ATTR_TYPE_MASK) !=
		TEESMC_ATTR_TYPE_NONE)
		goto bad;

	switch (arg32->cmd) {
	case TEE_RPC_WAIT_QUEUE_SLEEP:
		tee_wait_queue_sleep(DEV, &ptee->wait_queue,
					params[0].u.value.a);
		break;
	case TEE_RPC_WAIT_QUEUE_WAKEUP:
		tee_wait_queue_wakeup(DEV, &ptee->wait_queue,
					params[0].u.value.a);
		break;
	default:
		goto bad;
	}

	arg32->ret = TEEC_SUCCESS;
	return;
bad:
	arg32->ret = TEEC_ERROR_BAD_PARAMETERS;
}

static void handle_rpc_func_cmd_wait(struct teesmc32_arg *arg32)
{
	struct teesmc32_param *params;
	unsigned long usec_wait;

	if (arg32->num_params != 1)
		goto bad;

	params = TEESMC32_GET_PARAMS(arg32);
	usec_wait = ((unsigned long ) params[0].u.value.a) * 1000;

	usleep_range(usec_wait, usec_wait + USEC_PER_MSEC);

	arg32->ret = TEEC_SUCCESS;
	return;
bad:
	arg32->ret = TEEC_ERROR_BAD_PARAMETERS;
}

static void handle_rpc_func_cmd_to_supplicant(struct tee_tz *ptee,
		struct teesmc32_arg *arg32)
{
	struct teesmc32_param *params;
	struct tee_rpc_invoke inv;
	size_t n;
	uint32_t ret;

	if (arg32->num_params > TEE_RPC_BUFFER_NUMBER) {
		arg32->ret = TEEC_ERROR_GENERIC;
		return;
	}

	params = TEESMC32_GET_PARAMS(arg32);

	memset(&inv, 0, sizeof(inv));
	inv.cmd = arg32->cmd;
	/*
	 * Set a suitable error code in case teed
	 * ignores the request.
	 */
	inv.res = TEEC_ERROR_NOT_IMPLEMENTED;
	inv.nbr_bf = arg32->num_params;
	for (n = 0; n < arg32->num_params; n++) {
		switch (params[n].attr & TEESMC_ATTR_TYPE_MASK) {
		case TEESMC_ATTR_TYPE_VALUE_INPUT:
		/* fall-through */
		case TEESMC_ATTR_TYPE_VALUE_INOUT:
			inv.cmds[n].fd = (int) params[n].u.value.a;
			inv.cmds[n].type = TEE_RPC_VALUE;
			break;
		case TEESMC_ATTR_TYPE_VALUE_OUTPUT:
			inv.cmds[n].type = TEE_RPC_VALUE;
			break;
		case TEESMC_ATTR_TYPE_MEMREF_INPUT:
		/* fall-through */
		case TEESMC_ATTR_TYPE_MEMREF_OUTPUT:
		/* fall-through */
		case TEESMC_ATTR_TYPE_MEMREF_INOUT:
			inv.cmds[n].buffer =
				(void *)(uintptr_t)params[n].u.memref.buf_ptr;
			inv.cmds[n].size = params[n].u.memref.size;
			inv.cmds[n].type = TEE_RPC_BUFFER;
			break;
		default:
			arg32->ret = TEEC_ERROR_GENERIC;
			return;
		}
	}

	ret = tee_supp_cmd(ptee->tee, TEE_RPC_ICMD_INVOKE,
			   &inv, sizeof(inv));
	if (ret == TEEC_RPC_OK)
		arg32->ret = inv.res;

	for (n = 0; n < arg32->num_params; n++) {
		switch (params[n].attr & TEESMC_ATTR_TYPE_MASK) {
		case TEESMC_ATTR_TYPE_MEMREF_OUTPUT:
		case TEESMC_ATTR_TYPE_MEMREF_INOUT:
			/*
			 * Allow teed to assign a new pointer
			 * to an out-buffer. Needed when the
			 * teed allocates a new buffer, for
			 * instance when loading a TA.
			 */
			params[n].u.memref.buf_ptr =
				(uint32_t)(uintptr_t)inv.cmds[n].buffer;
			params[n].u.memref.size = inv.cmds[n].size;
			break;
		case TEESMC_ATTR_TYPE_VALUE_OUTPUT:
		case TEESMC_ATTR_TYPE_VALUE_INOUT:
			params[n].u.value.a = inv.cmds[n].fd;
			break;
		default:
			break;
		}
	}
}

#if IS_ENABLED(CONFIG_TRUSTKERNEL_TEE_RPMB_SUPPORT)

#include "linux/tee_rpmb.h"

static int check_rpmb_request(struct teesmc32_arg *arg32)
{
	struct teesmc32_param *params;

	if (arg32->num_params != TEE_RPMB_BUFFER_NUMBER) {
		arg32->ret = TEEC_ERROR_BAD_PARAMETERS;
		return -1;
	}

	params = TEESMC32_GET_PARAMS(arg32);

	if (((params[0].attr & TEESMC_ATTR_TYPE_MASK)
				!= TEESMC_ATTR_TYPE_MEMREF_INPUT) ||
			((params[1].attr & TEESMC_ATTR_TYPE_MASK)
				!= TEESMC_ATTR_TYPE_MEMREF_OUTPUT)) {
		arg32->ret = TEEC_ERROR_BAD_PARAMETERS;
		return -1;
	}

	return 0;
}

static void handle_rpmb_cmd(struct tee_tz *ptee,
				struct teesmc32_arg *arg32)
{
	uint32_t ret;
	struct tee_rpc_invoke inv;

	struct teesmc32_param *params;

	if (check_rpmb_request(arg32) < 0)
		return;

	params = TEESMC32_GET_PARAMS(arg32);

	memset(&inv, 0, sizeof(inv));
	inv.cmd = arg32->cmd;

	inv.res = TEEC_ERROR_NOT_IMPLEMENTED;

	/*
	 * in current TEE's implementation,
	 * all rpmb parameters are consequently
	 * placed in ONE piece of shared memory,
	 * in the following way,
	 *
	 * [ rpmb request buffer ] [ rpmb response buffer ]
	 */
	inv.nbr_bf = 1;
	inv.cmds[0].buffer =
		(void *) (uintptr_t) params[0].u.memref.buf_ptr;
	inv.cmds[0].type = TEE_RPC_BUFFER;
	inv.cmds[0].size = params[0].u.memref.size
		+ params[1].u.memref.size;

	ret = tee_supp_cmd(ptee->tee, TEE_RPC_ICMD_INVOKE,
			&inv, sizeof(inv));

	if (ret != TEEC_RPC_OK)
		arg32->ret = ret;
	else
		arg32->ret = inv.res;
}

#else

static void handle_rpmb_cmd(struct tee_tz *ptee __always_unused,
				struct teesmc32_arg *arg32)
{
	arg32->ret = TEEC_ERROR_NOT_IMPLEMENTED;
}

#endif

#if IS_ENABLED(CONFIG_TRUSTKERNEL_TEE_FP_SUPPORT)

static void handle_clkmgr_cmd(struct tee_tz *ptee __always_unused,
				struct teesmc32_arg *arg)
{
	struct teesmc32_param *params = TEESMC32_GET_PARAMS(arg);

	if ((arg->num_params < 1) ||
			(params[0].attr & TEESMC_ATTR_TYPE_MASK) != TEESMC_ATTR_TYPE_VALUE_INPUT) {
		arg->ret = TEEC_ERROR_BAD_PARAMETERS;
		return;
	}

	arg->ret = tee_clkmgr_handle(params[0].u.value.a, params[0].u.value.b);
}

#else

static void handle_clkmgr_cmd(struct tee_tz *ptee __always_unused,
				struct teesmc32_arg *arg)
{
	arg->ret = TEEC_ERROR_NOT_IMPLEMENTED;
}

#endif

static void handle_rpc_func_cmd(struct tee_tz *ptee, u32 parg32)
{
	struct teesmc32_arg *arg32;

	arg32 = tee_shm_pool_p2v(DEV, ptee->shm_pool, parg32);
	if (!arg32)
		return;

	switch (arg32->cmd) {
	case TEE_RPC_WAIT_QUEUE_SLEEP:
	case TEE_RPC_WAIT_QUEUE_WAKEUP:
		handle_rpc_func_cmd_wait_queue(ptee, arg32);
		break;
	case TEE_RPC_WAIT:
		handle_rpc_func_cmd_wait(arg32);
		break;
	case TEE_RPC_RPMB_CMD:
		handle_rpmb_cmd(ptee, arg32);
		break;
	case TEE_RPC_CLKMGR_CMD:
		handle_clkmgr_cmd(ptee, arg32);
		break;
	default:
		handle_rpc_func_cmd_to_supplicant(ptee, arg32);
	}
}

static struct tee_shm *handle_rpc_alloc(struct tee_tz *ptee, size_t size)
{
	struct tee_rpc_alloc rpc_alloc;

	memset((void *) &rpc_alloc, 0, sizeof(struct tee_rpc_alloc));

	rpc_alloc.size = size;
	tee_supp_cmd(ptee->tee, TEE_RPC_ICMD_ALLOCATE,
		&rpc_alloc, sizeof(rpc_alloc));
	return rpc_alloc.shm;
}

static void handle_rpc_free(struct tee_tz *ptee, struct tee_shm *shm)
{
	struct tee_rpc_free rpc_free;

	if (!shm)
		return;
	rpc_free.shm = shm;
	tee_supp_cmd(ptee->tee, TEE_RPC_ICMD_FREE, &rpc_free, sizeof(rpc_free));
}

static u32 handle_rpc(struct tee_tz *ptee, struct smc_param *param)
{
	struct tee_shm *shm;
	int cookie;

	switch (TEESMC_RETURN_GET_RPC_FUNC(param->a0)) {
	case TEESMC_RPC_FUNC_ALLOC_ARG:
		param->a1 =
			tkcore_shm_pool_alloc(DEV,
			ptee->shm_pool, param->a1, 4);

		break;
	case TEESMC_RPC_FUNC_ALLOC_PAYLOAD:
		/* Can't support payload shared memory with this interface */
		param->a2 = 0;
		break;
	case TEESMC_RPC_FUNC_FREE_ARG:
		tkcore_shm_pool_free(DEV, ptee->shm_pool, param->a1, 0);
		break;
	case TEESMC_RPC_FUNC_FREE_PAYLOAD:
		/* Can't support payload shared memory with this interface */
		break;
	case TEESMC_ST_RPC_FUNC_ALLOC_PAYLOAD:
		shm = handle_rpc_alloc(ptee, param->a1);
		if (IS_ERR_OR_NULL(shm)) {
			param->a1 = 0;
			break;
		}
		cookie = handle_get(&shm_handle_db, shm);
		if (cookie < 0) {
			handle_rpc_free(ptee, shm);
			param->a1 = 0;
			break;
		}
		param->a1 = shm->static_shm.paddr;
		param->a2 = cookie;
		break;
	case TEESMC_ST_RPC_FUNC_FREE_PAYLOAD:
		shm = handle_put(&shm_handle_db, param->a1);
		handle_rpc_free(ptee, shm);
		break;
	case TEESMC_RPC_FUNC_CMD:
		handle_rpc_func_cmd(ptee, param->a1);
		break;
	default:
		pr_warn("Unknown RPC func 0x%x\n",
			 (u32) TEESMC_RETURN_GET_RPC_FUNC(param->a0));
		break;
	}

	/* TODO refine this piece of logic. the irq status
	 * can no longer determine whether it's a fastcall
	 * or not
	 */
	if (irqs_disabled())
		return TEESMC32_FASTCALL_RETURN_FROM_RPC;
	else
		return TEESMC32_CALL_RETURN_FROM_RPC;
}

static bool default_affinity_need_bind_cpu(void *priv)
{
	(void) priv;
	return false;
}

static bool default_affinity_match_cpu(void *priv, int cpu)
{
	(void) priv;
	(void) cpu;
	return true;
}

static int default_affinity_get_candidate_cpu(void *priv)
{
	(void) priv;
	return 0;
}

static const struct tee_task_cpu_affinity_operations default_affinity_operations = {
	.need_bind_cpu = default_affinity_need_bind_cpu,
	.match_cpu = default_affinity_match_cpu,
	.get_candidate_cpu = default_affinity_get_candidate_cpu,
};

void raw_call_tee(struct smc_param *p)
{
	struct tee_task_cpu_affinity aff = {
		.ops = &default_affinity_operations,
		.priv = NULL,
	};

	run_tee_task_nowait(p, &aff);
}

static void call_tee(struct tee_tz *ptee,
			const struct tee_task_cpu_affinity *ctx_task_cpu_affinity,
			uintptr_t parg32, struct teesmc32_arg *arg32, u32 funcid)
{
	u32 ret;
	struct tee_task_cpu_affinity aff;

	struct smc_param param = {
		.a1 = parg32,
	};

	if (ctx_task_cpu_affinity) {
		aff = *ctx_task_cpu_affinity;
	} else {
		aff.ops = &default_affinity_operations;
		aff.priv = NULL;
	}

	for (;;) {
		param.a0 = funcid;

		run_tee_task(&param, &aff);
		if (!TEESMC_RETURN_IS_RPC(param.a0))
			break;

		funcid = handle_rpc(ptee, &param);
	}

	ret = param.a0;

	if (unlikely(ret != TEESMC_RETURN_OK &&
			ret != TEESMC_RETURN_UNKNOWN_FUNCTION)) {
		arg32->ret = TEEC_ERROR_COMMUNICATION;
		arg32->ret_origin = TEEC_ORIGIN_COMMS;
	}
}

static void stdcall_tee(struct tee_tz *ptee,
			const struct tee_task_cpu_affinity *aff,
			uintptr_t parg32, struct teesmc32_arg *arg32)
{
	call_tee(ptee, aff, parg32, arg32, TEESMC32_CALL_WITH_ARG);
}

static void fastcall_tee(struct tee_tz *ptee, uintptr_t parg32,
			struct teesmc32_arg *arg32)
{
	call_tee(ptee, NULL, parg32, arg32, TEESMC32_FASTCALL_WITH_ARG);
}

/*******************************************************************
 * TEE service invoke formating
 *******************************************************************/

/* allocate tee service argument buffer and return virtual address */
static void *alloc_tee_arg(struct tee_tz *ptee, unsigned long *p, size_t l)
{
	void *vaddr;

	WARN_ON(!CAPABLE(ptee->tee));

	if ((p == NULL) || (l == 0))
		return NULL;

	/* assume a 4 bytes aligned is sufficient */
	*p = tkcore_shm_pool_alloc(DEV, ptee->shm_pool, l, ALLOC_ALIGN);
	if (*p == 0)
		return NULL;

	vaddr = tee_shm_pool_p2v(DEV, ptee->shm_pool, *p);


	return vaddr;
}

/* free tee service argument buffer (from its physical address) */
static void free_tee_arg(struct tee_tz *ptee, unsigned long p)
{
	WARN_ON(!CAPABLE(ptee->tee));

	if (p)
		tkcore_shm_pool_free(DEV, ptee->shm_pool, p, 0);

}

static uint32_t get_cache_attrs(struct tee_tz *ptee)
{
	if (tee_shm_pool_is_cached(ptee->shm_pool))
		return TEESMC_ATTR_CACHE_DEFAULT << TEESMC_ATTR_CACHE_SHIFT;
	else
		return TEESMC_ATTR_CACHE_NONCACHE << TEESMC_ATTR_CACHE_SHIFT;
}

static uint32_t param_type_teec2teesmc(uint8_t type)
{
	switch (type) {
	case TEEC_NONE:
		return TEESMC_ATTR_TYPE_NONE;
	case TEEC_VALUE_INPUT:
		return TEESMC_ATTR_TYPE_VALUE_INPUT;
	case TEEC_VALUE_OUTPUT:
		return TEESMC_ATTR_TYPE_VALUE_OUTPUT;
	case TEEC_VALUE_INOUT:
		return TEESMC_ATTR_TYPE_VALUE_INOUT;
	case TEEC_MEMREF_TEMP_INPUT:
	case TEEC_MEMREF_PARTIAL_INPUT:
		return TEESMC_ATTR_TYPE_MEMREF_INPUT;
	case TEEC_MEMREF_TEMP_OUTPUT:
	case TEEC_MEMREF_PARTIAL_OUTPUT:
		return TEESMC_ATTR_TYPE_MEMREF_OUTPUT;
	case TEEC_MEMREF_WHOLE:
	case TEEC_MEMREF_TEMP_INOUT:
	case TEEC_MEMREF_PARTIAL_INOUT:
		return TEESMC_ATTR_TYPE_MEMREF_INOUT;
	default:
		WARN_ON(true);
		return 0;
	}
}

static void set_params(struct tee_tz *ptee,
	struct teesmc32_param params32[TEEC_CONFIG_PAYLOAD_REF_COUNT],
	uint32_t param_types,
	struct tee_data *data)
{
	size_t n;
	struct tee_shm *shm;
	struct TEEC_Value *value;

	for (n = 0; n < TEEC_CONFIG_PAYLOAD_REF_COUNT; n++) {
		uint32_t type = TEEC_PARAM_TYPE_GET(param_types, n);

		params32[n].attr = param_type_teec2teesmc(type);
		if (params32[n].attr == TEESMC_ATTR_TYPE_NONE)
			continue;
		if (params32[n].attr < TEESMC_ATTR_TYPE_MEMREF_INPUT) {
			value = (struct TEEC_Value *)&data->params[n];
			params32[n].u.value.a = value->a;
			params32[n].u.value.b = value->b;
			continue;
		}
		shm = data->params[n].shm;
		params32[n].attr |= get_cache_attrs(ptee);
		params32[n].u.memref.buf_ptr = shm->static_shm.paddr;
		params32[n].u.memref.size = shm->size_req;
	}
}

static void get_params(struct tee_data *data,
	struct teesmc32_param params32[TEEC_CONFIG_PAYLOAD_REF_COUNT])
{
	size_t n;
	struct tee_shm *shm;
	struct TEEC_Value *value;

	for (n = 0; n < TEEC_CONFIG_PAYLOAD_REF_COUNT; n++) {
		if (params32[n].attr == TEESMC_ATTR_TYPE_NONE)
			continue;
		if (params32[n].attr < TEESMC_ATTR_TYPE_MEMREF_INPUT) {
			value = &data->params[n].value;
			value->a = params32[n].u.value.a;
			value->b = params32[n].u.value.b;
			continue;
		}
		shm = data->params[n].shm;
		shm->size_req = params32[n].u.memref.size;
	}
}

/*
 * tee_open_session - invoke TEE to open a GP TEE session
 */
static int tz_open(struct tee_session *sess, struct tee_cmd *cmd)
{
	struct tee *tee;
	struct tee_tz *ptee;
	int ret = 0;

	struct teesmc32_arg *arg32;
	struct teesmc32_param *params32;
	struct teesmc_meta_open_session *meta;
	uintptr_t parg32;
	uintptr_t pmeta;
	size_t num_meta = 1;
	uint8_t *ta;
	struct TEEC_UUID *uuid;

	if (WARN_ON(!sess->ctx->tee) || WARN_ON(!sess->ctx->tee->priv))
		return -1;

	tee = sess->ctx->tee;
	ptee = tee->priv;

	if (cmd->uuid)
		uuid = cmd->uuid->static_shm.kaddr;
	else
		uuid = NULL;

	if (!CAPABLE(ptee->tee)) {
		pr_err("tkcoredrv: %s: not capable\n", __func__);
		return -EBUSY;
	}

	/* case ta binary is inside the open request */
	ta = NULL;
	if (cmd->ta)
		ta = cmd->ta->static_shm.kaddr;
	if (ta)
		num_meta++;

	arg32 = alloc_tee_arg(ptee, &parg32,
		TEESMC32_GET_ARG_SIZE(
		TEEC_CONFIG_PAYLOAD_REF_COUNT + num_meta));
	meta = alloc_tee_arg(ptee, &pmeta, sizeof(*meta));

	if ((arg32 == NULL) || (meta == NULL)) {
		free_tee_arg(ptee, parg32);
		free_tee_arg(ptee, pmeta);
		return -ENOMEM;
	}

	memset(arg32, 0, sizeof(*arg32));
	memset(meta, 0, sizeof(*meta));
	arg32->num_params = TEEC_CONFIG_PAYLOAD_REF_COUNT + num_meta;
	params32 = TEESMC32_GET_PARAMS(arg32);

	arg32->cmd = TEESMC_CMD_OPEN_SESSION;

	params32[0].u.memref.buf_ptr = pmeta;
	params32[0].u.memref.size = sizeof(*meta);
	params32[0].attr = TEESMC_ATTR_TYPE_MEMREF_INPUT |
			   TEESMC_ATTR_META | get_cache_attrs(ptee);

	if (ta) {
		params32[1].u.memref.buf_ptr =
			tee_shm_pool_v2p(DEV, ptee->shm_pool,
				cmd->ta->static_shm.kaddr);
		params32[1].u.memref.size = cmd->ta->size_req;
		params32[1].attr = TEESMC_ATTR_TYPE_MEMREF_INPUT |
				   TEESMC_ATTR_META | get_cache_attrs(ptee);
	}

	if (uuid != NULL)
		memcpy(meta->uuid, uuid, TEESMC_UUID_LEN);

	meta->clnt_login = cmd->login_method;
	memcpy(&meta->clnt_uuid, cmd->login_identity, sizeof(cmd->login_identity));

	params32 += num_meta;
	set_params(ptee, params32, cmd->param.type, &cmd->param);

	stdcall_tee(ptee, sess->ctx->config ?
			sess->ctx->config->aff: NULL, parg32, arg32);

	get_params(&cmd->param, params32);

	if (arg32->ret != TEEC_ERROR_COMMUNICATION) {
		sess->sessid = arg32->session;
		cmd->err = arg32->ret;
		cmd->origin = arg32->ret_origin;
	} else
		ret = -EBUSY;

	free_tee_arg(ptee, parg32);
	free_tee_arg(ptee, pmeta);

	return ret;
}

/*
 * tee_invoke_command - invoke TEE to invoke a GP TEE command
 */
static int tz_invoke(struct tee_session *sess, struct tee_cmd *cmd)
{
	struct tee *tee;
	struct tee_tz *ptee;
	int ret = 0;

	struct teesmc32_arg *arg32;
	uintptr_t parg32;
	struct teesmc32_param *params32;

	if (WARN_ON(!sess->ctx) ||
			WARN_ON(!sess->ctx->tee) ||
			WARN_ON(!sess->ctx->tee->priv))
		return -1;

	tee = sess->ctx->tee;
	ptee = tee->priv;

	if (!CAPABLE(tee)) {
		pr_err("tkcoredrv: %s: not capable\n", __func__);
		return -EBUSY;
	}

	arg32 = (typeof(arg32))alloc_tee_arg(ptee, &parg32,
		TEESMC32_GET_ARG_SIZE(TEEC_CONFIG_PAYLOAD_REF_COUNT));
	if (!arg32) {
		free_tee_arg(ptee, parg32);
		return TEEC_ERROR_OUT_OF_MEMORY;
	}

	memset(arg32, 0, sizeof(*arg32));
	arg32->num_params = TEEC_CONFIG_PAYLOAD_REF_COUNT;
	params32 = TEESMC32_GET_PARAMS(arg32);

	arg32->cmd = TEESMC_CMD_INVOKE_COMMAND;
	arg32->session = sess->sessid;
	arg32->ta_func = cmd->cmd;

	set_params(ptee, params32, cmd->param.type, &cmd->param);

	stdcall_tee(ptee, sess->ctx->config ?
			sess->ctx->config->aff: NULL, parg32, arg32);

	get_params(&cmd->param, params32);

	if (arg32->ret != TEEC_ERROR_COMMUNICATION) {
		cmd->err = arg32->ret;
		cmd->origin = arg32->ret_origin;
	} else
		ret = -EBUSY;

	free_tee_arg(ptee, parg32);

	return ret;
}

/*
 * tee_cancel_command - invoke TEE to cancel a GP TEE command
 */
static int tz_cancel(struct tee_session *sess, struct tee_cmd *cmd)
{
	struct tee *tee;
	struct tee_tz *ptee;
	int ret = 0;

	struct teesmc32_arg *arg32;
	uintptr_t parg32;

	WARN_ON(!sess->ctx->tee);
	WARN_ON(!sess->ctx->tee->priv);
	tee = sess->ctx->tee;
	ptee = tee->priv;


	arg32 = alloc_tee_arg(ptee, &parg32, TEESMC32_GET_ARG_SIZE(0));
	if (arg32 == NULL) {
		free_tee_arg(ptee, parg32);
		return TEEC_ERROR_OUT_OF_MEMORY;
	}

	memset(arg32, 0, sizeof(*arg32));
	arg32->cmd = TEESMC_CMD_CANCEL;
	arg32->session = sess->sessid;

	fastcall_tee(ptee, parg32, arg32);

	if (arg32->ret == TEEC_ERROR_COMMUNICATION)
		ret = -EBUSY;

	free_tee_arg(ptee, parg32);

	return ret;
}

/*
 * tee_close_session - invoke TEE to close a GP TEE session
 */
static int tz_close(struct tee_session *sess)
{
	struct tee *tee;
	struct tee_tz *ptee;
	int ret = 0;

	struct teesmc32_arg *arg32;
	uintptr_t parg32;

	if (WARN_ON(!sess->ctx) ||
			WARN_ON(!sess->ctx->tee) ||
			WARN_ON(!sess->ctx->tee->priv))
		return 0;

	tee = sess->ctx->tee;
	ptee = tee->priv;

	if (!CAPABLE(tee)) {
		pr_err("tkcoredrv: %s: not capable\n", __func__);
		return -EBUSY;
	}

	arg32 = alloc_tee_arg(ptee, &parg32, TEESMC32_GET_ARG_SIZE(0));
	if (arg32 == NULL) {
		pr_err("tkcoredrv: failed to allocate tee arg\n");
		free_tee_arg(ptee, parg32);
		return TEEC_ERROR_OUT_OF_MEMORY;
	}

	memset(arg32, 0, sizeof(*arg32));
	arg32->cmd = TEESMC_CMD_CLOSE_SESSION;
	arg32->session = sess->sessid;

	stdcall_tee(ptee, sess->ctx->config ?
		sess->ctx->config->aff : NULL, parg32, arg32);

	if (arg32->ret == TEEC_ERROR_COMMUNICATION)
		ret = -EBUSY;

	free_tee_arg(ptee, parg32);

	return ret;
}

static struct tee_shm *tz_alloc(struct tee *tee, size_t size, uint32_t flags)
{
	struct tee_shm *shm = NULL;
	struct tee_tz *ptee;
	size_t size_aligned;

	if (WARN_ON(!tee->priv))
		return NULL;

	ptee = tee->priv;

	size_aligned = ((size / ALLOC_ALIGN) + 1) * ALLOC_ALIGN;
	if (unlikely(size_aligned == 0)) {
		pr_err("requested size too big\n");
		return NULL;
	}

	shm = devm_kzalloc(tee->dev, sizeof(struct tee_shm), GFP_KERNEL);
	if (!shm)
		return ERR_PTR(-ENOMEM);

	shm->size_alloc = ((size / ALLOC_ALIGN) + 1) * ALLOC_ALIGN;
	shm->size_req = size;

	shm->static_shm.paddr = tkcore_shm_pool_alloc(tee->dev, ptee->shm_pool,
						shm->size_alloc, ALLOC_ALIGN);
	if (!shm->static_shm.paddr) {
		pr_err("tkcoredrv: %s cannot alloc memory, size 0x%lx\n",
			__func__, (unsigned long) shm->size_alloc);
		devm_kfree(tee->dev, shm);
		return ERR_PTR(-ENOMEM);
	}
	shm->static_shm.kaddr =
		tee_shm_pool_p2v(tee->dev, ptee->shm_pool, shm->static_shm.paddr);
	if (!shm->static_shm.kaddr) {
		pr_err("tkcoredrv: %s: p2v(%p)=0\n",
			__func__,
			(void *) (unsigned long) shm->static_shm.paddr);
		tkcore_shm_pool_free(tee->dev,
			ptee->shm_pool, shm->static_shm.paddr, NULL);
		devm_kfree(tee->dev, shm);
		return ERR_PTR(-EFAULT);
	}
	shm->flags = flags;
	if (ptee->shm_cached)
		shm->flags |= TEE_SHM_CACHED;

	return shm;
}

static void tz_free(struct tee_shm *shm)
{
	size_t size;
	int ret;
	struct tee *tee;
	struct tee_tz *ptee;

	WARN_ON(!shm->tee);
	WARN_ON(!shm->tee->priv);
	tee = shm->tee;
	ptee = tee->priv;

	ret = tkcore_shm_pool_free(tee->dev, ptee->shm_pool,
		shm->static_shm.paddr, &size);
	if (!ret) {
		devm_kfree(tee->dev, shm);
		shm = NULL;
	}

	//TODO free driver
}

static int tz_shm_inc_ref(struct tee_shm *shm)
{
	struct tee *tee;
	struct tee_tz *ptee;

	WARN_ON(!shm->tee);
	WARN_ON(!shm->tee->priv);
	tee = shm->tee;
	ptee = tee->priv;

	return tee_shm_pool_incref(tee->dev, ptee->shm_pool, shm->static_shm.paddr);
}

#define DEFAULT_SHM_LENGTH_SHIFT	22

#ifdef CONFIG_OF

#define TZDRV_RESERVED_MEM_COMPAT "trustkernel,shared_mem"

static struct reserved_mem *tee_shared_mem = NULL;

#if !defined(MODULE)

static int __init tkcore_shared_mem_setup(struct reserved_mem *rmem)
{
	tee_shared_mem = rmem;
	return 0;
}

RESERVEDMEM_OF_DECLARE(tkcore_shared_mem, TZDRV_RESERVED_MEM_COMPAT,
		tkcore_shared_mem_setup);

static void init_tkcore_shared_mem(void)
{
	// do nothing when built-in linux
}

#else

static void init_tkcore_shared_mem(void)
{
	struct device_node *rmem_node;
	struct reserved_mem *rmem;

	/* Get reserved memory */
	rmem_node = of_find_compatible_node(NULL, NULL, TZDRV_RESERVED_MEM_COMPAT);
	if (!rmem_node) {
		pr_err("tkcoredrv: no node for reserved memory\n");
		return;
	}

	rmem = of_reserved_mem_lookup(rmem_node);
	if (!rmem) {
		pr_err("tkcoredrv: cannot lookup reserved memory\n");
		return;
	}

	tee_shared_mem = rmem;
}

#endif

#endif

static int get_shared_memory(unsigned long *base,
		size_t *length,
		bool *allocated)
{
#ifdef CONFIG_OF

	init_tkcore_shared_mem();

	if (!tee_shared_mem)
		goto alloc_shm;

	*base = tee_shared_mem->base;
	*length = tee_shared_mem->size;

	pr_info("tkcoredrv: reserved shared memory: [ 0x%lx 0x%lx ]\n",
			*base, (unsigned long) *length);

	*allocated = false;
#endif

	return 0;

alloc_shm:
	pr_info("tkcoredrv: alloc shared memory\n");

	*base = __get_free_pages(GFP_DMA32 | GFP_DMA,
		DEFAULT_SHM_LENGTH_SHIFT - PAGE_SHIFT);

	if (*base == 0)
		return -ENOMEM;

	*allocated = true;
	*length = 1ul << DEFAULT_SHM_LENGTH_SHIFT;

	return 0;
}

static unsigned long register_shared_mem(unsigned long base,
											unsigned long length,
											bool cacheable)
{
	struct smc_param cmd;

#if defined(ARM64)
	cmd.a0 = TEESMC64_TKCORE_FASTCALL_ADD_SHM;
#else
	cmd.a0 = TEESMC32_TKCORE_FASTCALL_ADD_SHM;
#endif
	cmd.a1 = base;
	cmd.a2 = length;
	cmd.a3 = cacheable; // whether shared memory is cached

	raw_call_tee(&cmd);
	return cmd.a0;
}

static unsigned long register_log_buffer(unsigned long base,
										unsigned long length,
										bool cacheable)
{
	struct smc_param cmd = { 0 };

#if defined(ARM64)
	cmd.a0 = TEESMC64_TKCORE_FASTCALL_SET_LOG_BUFFER ;
#else
	cmd.a0 = TEESMC32_TKCORE_FASTCALL_SET_LOG_BUFFER ;
#endif
	cmd.a1 = base;
	cmd.a2 = length;
	cmd.a3 = cacheable;

	raw_call_tee(&cmd);

	return cmd.a0;
}

static int retrieve_tos_revision(uint32_t *maj,
								uint32_t *mid,
								uint32_t *min)
{
	struct smc_param cmd = { 0 };

	/* get os revision */
#if defined(ARM64)
	cmd.a0 = TEESMC64_CALL_GET_OS_REVISION;
#else
	cmd.a0 = TEESMC32_CALL_GET_OS_REVISION;
#endif

	raw_call_tee(&cmd);

	if (cmd.a3 == 0) {
		*maj = 0;
		*mid = cmd.a0;
		*min = cmd.a1;
	} else {
		*maj = cmd.a0;
		*mid = cmd.a1;
		*min = cmd.a2;
	}

	return 0;
}

static void init_tos_version(struct tee *tee)
{
	retrieve_tos_revision(&tee->version.maj,
						&tee->version.mid,
						&tee->version.min);

	pr_info("tkcoreos-rev: %d.%d.%d-gp\n",
			tee->version.maj, tee->version.mid ,tee->version.min);
}

/* configure_shm - Negotiate Shared Memory configuration with teetz. */
static int configure_shm(struct tee_tz *ptee)
{
	int ret = 0;

	unsigned long shm_base;
	size_t shm_length;
	bool shm_alloced = false;

	if (get_shared_memory(&shm_base, &shm_length, &shm_alloced)) {
		return -ENOMEM;
	}

	ptee->shm_cached = true;

	if (shm_alloced) {
		ptee->shm_vaddr = (void *) shm_base;
		ptee->shm_paddr = __pa(shm_base);
	} else {
		ptee->shm_paddr = shm_base;
		ptee->shm_vaddr = tee_map_cached_shm(ptee->shm_paddr,
				shm_length);
		if (ptee->shm_vaddr == NULL) {
			pr_warn("tkcoredev: map shared mem failed\n");
			ret = -ENOMEM;
			goto out;
		}
	}

	ptee->shm_pool = tee_shm_pool_create(DEV, shm_length,
		ptee->shm_vaddr, ptee->shm_paddr);
	if (!ptee->shm_pool) {
		pr_warn("tkcoredrv: create shm pool failed (%zu)", shm_length);
		ret = -EINVAL;

		goto out;
	}

	if (ptee->shm_cached)
		tee_shm_pool_set_cached(ptee->shm_pool);

	/*
	 * currently shared memory is
	 * hard coded to cacheable
	 */
	ret = (int) register_shared_mem(ptee->shm_paddr, shm_length, true);
	if (ret != TEESMC_RETURN_OK) {
		pr_warn("tkcoredrv: register shm failed: %d", ret);
		ret = -EINVAL;
		goto out;
	}

out:
	if (ret != 0 && shm_alloced)
		free_pages(shm_base, DEFAULT_SHM_LENGTH_SHIFT - PAGE_SHIFT);

	return ret;
}

#define LOG_BUFFER_SIZE_SHIFT   (17)  //128KiB

static int init_log(struct tee *tee)
{
	int ret = 0, irq_num;

	unsigned long va;
	size_t length = 1ul << LOG_BUFFER_SIZE_SHIFT;

#ifdef CONFIG_OF
	struct device_node *node;

	node = of_find_compatible_node(NULL, NULL,
						"trustkernel,tkcore");
	if (node) {
		irq_num = irq_of_parse_and_map(node, 0);
	} else {
		pr_err("tkcoredrv: node not found\n");
		irq_num = 0;
	}
#else
	irq_num = 0;
#endif

	va = __get_free_pages(GFP_DMA32 | GFP_DMA,
		LOG_BUFFER_SIZE_SHIFT - PAGE_SHIFT);
	if (va == 0ul) {
		pr_warn("tkcoredrv: alloc log buffer failed");
		ret = -ENOMEM;
		goto err;
	}

	ret = (int) register_log_buffer(__pa(va), length, true);
	if (ret) {
		free_pages(va, LOG_BUFFER_SIZE_SHIFT - PAGE_SHIFT);
		goto err;
	}

	tee->log.buffer = (void *) va;
	tee->log.length = length;
	tee->log.irq = irq_num;

	return 0;

err:
	if (va != 0ul)
		free_pages(va, LOG_BUFFER_SIZE_SHIFT - PAGE_SHIFT);

	tee->log.buffer = NULL;
	tee->log.length = 0;
	tee->log.irq = 0;

	return ret;
}

static int tz_start(struct tee *tee)
{
	int ret;
	struct tee_tz *ptee;

	if (WARN_ON(!tee || !tee->priv))
		return -EINVAL;

	if (WARN_ON(!CAPABLE(tee)))
		return -EBUSY;

	ptee = tee->priv;
	WARN_ON(ptee->started);

	ptee->started = true;

	ret = configure_shm(ptee);
	if (ret)
		goto exit;

exit:
	if (ret)
		ptee->started = false;

	return ret;
}

static int tz_stop(struct tee *tee)
{
	struct tee_tz *ptee;

	WARN_ON(!tee || !tee->priv);

	ptee = tee->priv;

	if (!CAPABLE(tee)) {
		pr_err("tee: bad state\n");
		return -EBUSY;
	}

	tee_shm_pool_destroy(tee->dev, ptee->shm_pool);
	iounmap(ptee->shm_vaddr);
	ptee->started = false;

	return 0;
}

/******************************************************************************/

const struct tee_ops tee_tz_fops = {
	.type = "tz",
	.owner = THIS_MODULE,
	.start = tz_start,
	.stop = tz_stop,
	.invoke = tz_invoke,
	.cancel = tz_cancel,
	.open = tz_open,
	.close = tz_close,
	.alloc = tz_alloc,
	.free = tz_free,
	.shm_inc_ref = tz_shm_inc_ref,
	.call_tee = raw_call_tee,
};

static int tz_tee_init(struct platform_device *pdev)
{
	int ret = 0;

	struct tee *tee = platform_get_drvdata(pdev);
	struct tee_tz *ptee = tee->priv;

	tee_tz = ptee;

	ptee->started = false;

	tee_wait_queue_init(&ptee->wait_queue);

	if (ret) {
		pr_err("dev=%s, Secure failed (%d)\n",
			tee->name, ret);
	}

	init_tos_version(tee);

	ret = init_log(tee);
	if (ret != 0) {
		pr_warn("tkcoredrv: init log buffer failed: %d\n", ret);
		/*
		 * initialization failed of
		 * log buffer shouldn't affect
		 * initialization of teedrv
		 */
		ret = 0;
	}

	return ret;
}

static void tz_tee_deinit(struct platform_device *pdev)
{
	struct tee *tee = platform_get_drvdata(pdev);
	struct tee_tz *ptee = tee->priv;

	if (!CAPABLE(tee))
		return;

	tee_wait_queue_exit(&ptee->wait_queue);
}

static int tz_tee_probe(struct platform_device *pdev)
{
	int ret = 0;
	struct device *dev = &pdev->dev;
	struct tee *tee;
	struct tee_tz *ptee;

	tee = tee_core_alloc(dev, _TEE_TZ_NAME, pdev->id, &tee_tz_fops,
				sizeof(struct tee_tz));
	if (!tee)
		return -ENOMEM;

	ptee = tee->priv;
	ptee->tee = tee;

	platform_set_drvdata(pdev, tee);

	ret = tz_tee_init(pdev);
	if (ret)
		goto bail0;

	ret = tee_core_add(tee);
	if (ret)
		goto bail1;

	ret = __tee_get(tee);
	if (ret) {
		tz_tee_deinit(pdev);
		tee_core_del(tee);

		return ret;
	}

	return 0;

bail1:
	tz_tee_deinit(pdev);
bail0:
	tee_core_free(tee);
	return ret;
}

static int tz_tee_remove(struct platform_device *pdev)
{
	struct tee *tee = platform_get_drvdata(pdev);

	tz_tee_deinit(pdev);
	tee_core_del(tee);
	return 0;
}

static const struct of_device_id tz_tee_match[] = {
	{
		.compatible = "trustkernel,tzdrv",
	},
	{},
};

static struct platform_driver tz_tee_driver = {
	.probe = tz_tee_probe,
	.remove = tz_tee_remove,

	.driver = {
		.name = "tzdrv",
		.owner = THIS_MODULE,
		.of_match_table = tz_tee_match,
	},
};

static struct platform_device tz_0_plt_device = {
	.name = "tzdrv",
	.id = 0,
	.dev = {
		/*	.platform_data = tz_0_tee_data,*/
	},
};

static int __init tee_tz_init(void)
{
	int rc;

	pr_info("TrustKernel TEE Driver initialization\n");

	rc = tee_init_task();
	if (rc < 0)
		return rc;

	rc = platform_device_register(&tz_0_plt_device);
	if (rc < 0) {
		pr_err(
			"failed to register the platform devices 0 (rc=%d)\n",
			rc);
		goto err0;
	}

	rc = platform_driver_register(&tz_tee_driver);
	if (rc < 0) {
		pr_err("failed to probe the platform driver (rc=%d)\n",
			rc);
		goto err1;
	}

	return 0;

err1:
	platform_device_unregister(&tz_0_plt_device);
err0:
	tee_exit_task();

	return rc;
}

static void __exit tee_tz_exit(void)
{
	pr_info("TrustKernel TEE Driver Release\n");

	platform_device_unregister(&tz_0_plt_device);
	platform_driver_unregister(&tz_tee_driver);

	tee_exit_task();
}

#ifndef MODULE
rootfs_initcall(tee_tz_init);
#else
module_init(tee_tz_init);
#endif
module_exit(tee_tz_exit);

MODULE_AUTHOR("TrustKernel");
MODULE_DESCRIPTION("TrustKernel TKCore TZ driver");
MODULE_LICENSE("GPL");
MODULE_VERSION("1.0");
