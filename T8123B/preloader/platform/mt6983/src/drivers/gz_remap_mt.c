/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein is
 * confidential and proprietary to MediaTek Inc. and/or its licensors. Without
 * the prior written permission of MediaTek inc. and/or its licensors, any
 * reproduction, modification, use or disclosure of MediaTek Software, and
 * information contained herein, in whole or in part, shall be strictly
 * prohibited.
 *
 * MediaTek Inc. (C) 2018. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER
 * ON AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL
 * WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR
 * NONINFRINGEMENT. NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH
 * RESPECT TO THE SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY,
 * INCORPORATED IN, OR SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES
 * TO LOOK ONLY TO SUCH THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO.
 * RECEIVER EXPRESSLY ACKNOWLEDGES THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO
 * OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES CONTAINED IN MEDIATEK
 * SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE
 * RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S
 * ENTIRE AND CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE
 * RELEASED HEREUNDER WILL BE, AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE
 * MEDIATEK SOFTWARE AT ISSUE, OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE
 * CHARGE PAID BY RECEIVER TO MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 * The following software/firmware and/or related documentation ("MediaTek
 * Software") have been modified by MediaTek Inc. All revisions are subject to
 * any receiver's applicable license agreements with MediaTek Inc.
 */

#include <pal_log.h>
#include <gz_remap.h>
#include <gz_remap_mt.h>
#include <typedefs.h>

#define LOG_TAG "GZ_REMAP"

#define GZ_REMAP_DEBUG  (0)

#if GZ_REMAP_DEBUG
#define GZREMAP_ERR(fmt, ...) \
    pal_log_err("[%s][ERROR] %s:%d: " fmt, LOG_TAG, __func__, __LINE__, ##__VA_ARGS__)
#define GZREMAP_INFO(fmt, ...) \
    pal_log_info("[%s] " fmt, LOG_TAG, ##__VA_ARGS__)
#define GZREMAP_DBG(fmt, ...) \
    pal_log_debug("[%s][DEBUG] %s:%d: " fmt, LOG_TAG, __func__, __LINE__, ##__VA_ARGS__)
#else
#define GZREMAP_ERR(fmt, ...) \
    pal_log_err("[%s][ERROR] %s: " fmt, LOG_TAG, __func__, __LINE__, ##__VA_ARGS__)
#define GZREMAP_INFO(fmt, ...) \
    pal_log_info("[%s] " fmt, LOG_TAG, ##__VA_ARGS__)
#define GZREMAP_DBG(fmt, ...) \
    do {} while (0)
#endif

#define get_bit_field(x, msk, off)  ((x & (msk << off)) >> off)

#if GZ_USE_DEFAULT_REMAP_TABLE
int usr_remap_table = 0;
#else
int usr_remap_table = 1;
#endif

static uint64_t get_vm_offset(uint32_t vm_id)
{
    GZREMAP_DBG("vm_id=%u\n", vm_id);

    switch (vm_id) {
        case GZ_REMAP_VMID_AP:
        case GZ_REMAP_VMID_GZ:
            return GZ_REMAP_VM_OFFSET * vm_id;
        default:
            GZREMAP_ERR("Unknown VM ID=%u\n", vm_id);
            return -GZ_REMAP_ERR_INVALID_ARGUMENT;
    }

    /* should not reach */
    GZREMAP_ERR("Unhandled VM ID=%u\n", vm_id);
    ASSERT(0);
}

uint64_t gz_remap_ddr_get_offset(uint32_t vm_id)
{
    GZREMAP_DBG("vm_id=%u\n");
    return get_vm_offset(vm_id);
}

uint64_t gz_remap_io_get_offset(uint32_t vm_id)
{
    GZREMAP_DBG("vm_id=%u\n", vm_id);
    return get_vm_offset(vm_id);
}

uint64_t gz_remap_sec_io_get_offset(uint32_t vm_id)
{
    uint64_t vm_offset;

    GZREMAP_DBG("vm_id=%u\n", vm_id);

    vm_offset = get_vm_offset(vm_id);
    if (vm_offset == -GZ_REMAP_ERR_INVALID_ARGUMENT)
        return vm_offset;

    return vm_offset + GZ_REMAP_VM_SEC_IO_OFFSET;
}

int gz_remap_vm_aid_get(uint32_t vm_id)
{
	if (!usr_remap_table) {
		switch (vm_id) {
			case GZ_REMAP_VMID_AP:
				return 0x8;
			case GZ_REMAP_VMID_GZ:
				return 0x9;
			default:
				return -GZ_REMAP_ERR_INVALID_ARGUMENT;
		}
	}
	return -GZ_REMAP_ERR_INVALID_ARGUMENT;
}

int gz_remap_dump_config(void)
{
	uint32_t i;

	uint32_t gz_sec_common_reg0 = __raw_readl(GZ_SEC_COMMON_REG0);
	uint32_t gz_sec_common_reg1 = __raw_readl(GZ_SEC_COMMON_REG1);
	uint32_t gz_sec_common_lock = __raw_readl(GZ_SEC_COMMON_LOCK);

	/* register raw value */
	GZREMAP_DBG("GZ_SEC_COMMON_REG0=0x%x\n", gz_sec_common_reg0);
	GZREMAP_DBG("GZ_SEC_COMMON_REG1=0x%x\n", gz_sec_common_reg1);
	GZREMAP_DBG("GZ_SEC_COMMON_LOCK=%x\n", gz_sec_common_lock & GZ_SEC_COMMON_LOCK_EN);

	/* gz domain remap enable status */
	GZREMAP_INFO("GZ Domain Remap of EMI: %s\n",
		(gz_sec_common_reg0 & GZ_EMI_REMAP_EN) ? "enabled" : "disabled");
	GZREMAP_INFO("GZ Domain Remap of Infra IO: %s\n",
		(gz_sec_common_reg1 & GZ_INFRA_REMAP_EN) ? "enabled" : "disabled");

	/* ddr, io, sec_io remap offset */
	GZREMAP_INFO("DDR remap offset:\n");
	for (i = 0; i < GZ_REMAP_VM_NUM; ++i )
		GZREMAP_INFO("    VMID%d: 0x%llx\n", i, gz_remap_ddr_get_offset(i));

	GZREMAP_INFO("IO remap offset:\n");
	for (i = 0; i < GZ_REMAP_VM_NUM; ++i )
		GZREMAP_INFO("    VMID%d: 0x%llx\n", i, gz_remap_io_get_offset(i));

	GZREMAP_INFO("IO_VMID_SEC remap offset: \n");
	for (i = 0; i < GZ_REMAP_VM_NUM; ++i )
		GZREMAP_INFO("    VMID%d: 0x%llx\n", i, gz_remap_sec_io_get_offset(i));

	/* domain mapping */
	if (!usr_remap_table) {
		GZREMAP_INFO("GZ Domain remap:\n");
		for (i = 0; i < GZ_REMAP_VM_NUM; ++i )
			GZREMAP_INFO("    VMID(%d) -> AID(%u)\n", i, gz_remap_vm_aid_get(i));
	} else {
		GZREMAP_INFO("User Defined Remap Table not supported now\n");
	}

	return GZ_REMAP_SUCCESS;
}

static int gz_domain_remap_infra_io(int use_usr_remap_table)
{
	int gz_status;
	uint32_t reg_gz_domain_infra_io = __raw_readl(GZ_REMAP_DOMAIN_INFRA_IO);

	if (!use_usr_remap_table) {
		reg_gz_domain_infra_io |= GZ_INFRA_REMAP_EN;
		__raw_writel(reg_gz_domain_infra_io, GZ_REMAP_DOMAIN_INFRA_IO);

		return GZ_REMAP_SUCCESS;
	}

	GZREMAP_ERR("User defined remap table of emi is not supported currently\n");

	return GZ_REMAP_ERR_NOT_IMPLEMENTED;
}

static int gz_domain_remap_emi(int use_usr_remap_table)
{
	int gz_status;
	uint32_t reg_gz_domain_emi = __raw_readl(GZ_REMAP_DOMAIN_EMI);

	if (!use_usr_remap_table) {
		reg_gz_domain_emi |= GZ_EMI_REMAP_EN;
		__raw_writel(reg_gz_domain_emi, GZ_REMAP_DOMAIN_EMI);

		return GZ_REMAP_SUCCESS;
	}

	GZREMAP_ERR("User defined remap table of infra io is not supported currently\n");

	return GZ_REMAP_ERR_NOT_IMPLEMENTED;
}

int gz_remap_init(void)
{
	int rc;
	int use_usr_remap_table = usr_remap_table;

	rc = gz_domain_remap_emi(use_usr_remap_table);
	if (rc != GZ_REMAP_SUCCESS) {
		GZREMAP_ERR("failed to setup gz domain remap of emi, use_usr_remap_table=%d, rc=%d\n",
			use_usr_remap_table, rc);
		return rc;
	}

	rc = gz_domain_remap_infra_io(use_usr_remap_table);
	if (rc != GZ_REMAP_SUCCESS) {
		GZREMAP_ERR("failed to setup gz domain remap of emi, use_usr_remap_table=%d, rc=%d\n",
			use_usr_remap_table, rc);
		return rc;
	}

	if (rc == GZ_REMAP_SUCCESS) {
		uint32_t reg_gz_sec_common_lock = __raw_readl(GZ_SEC_COMMON_LOCK);
		reg_gz_sec_common_lock |= GZ_SEC_COMMON_LOCK_EN;
		__raw_writel(reg_gz_sec_common_lock, GZ_SEC_COMMON_LOCK);
	}

	return GZ_REMAP_SUCCESS;
}
