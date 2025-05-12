/*****************************************************************************
 *  Copyright Statement:
 *  --------------------
 *  This software is protected by Copyright and the information contained
 *  herein is confidential. The software may not be copied and the information
 *  contained herein may not be used or disclosed except with the written
 *  permission of MediaTek Inc. (C) 2011
 *
 *  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 *  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 *  RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
 *  AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
 *  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 *  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 *  NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 *  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 *  SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
 *  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
 *  NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S
 *  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
 *
 *  BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE
 *  LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 *  AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
 *  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
 *  MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE. 
 *
 *  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
 *  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
 *  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
 *  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
 *  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
 *
 *****************************************************************************/
#include <boot/error_message.h>
#include <stdint.h>
#include <error_code.h>
#include <boot/security_export/remote_sec_policy.h>
#include <boot/security_export/sha256.h>
typedef int status_t;

uint8_t *g_ext_all_in_one_sig;
uint32_t g_ext_all_in_one_sig_sz;

uint8_t *g_dev_fw_info;
uint32_t g_dev_fw_info_sz;

uint8_t *g_remote_sec_policy;
uint32_t g_remote_sec_policy_sz;

/* verify with all in one sig if it exists by default */
static uint32_t g_ext_all_in_one_sig_vfy = 1;

status_t get_all_in_one_ext_sig_exist(uint32_t *exist)
{
	status_t ret = STATUS_OK;

	if (exist == NULL)
		return STATUS_INVALID_PARAMETERS;

	if (g_ext_all_in_one_sig_sz != 0)
		*exist = 1;
	else
		*exist = 0;

	return ret;
}

status_t get_all_in_one_ext_sig_vfy(uint32_t *vfy)
{
	status_t ret = STATUS_OK;
	
	if (vfy == NULL)
		return STATUS_INVALID_PARAMETERS;

	*vfy = g_ext_all_in_one_sig_vfy;

	return ret;
}

status_t set_all_in_one_ext_sig_vfy(uint32_t vfy)
{
	status_t ret = STATUS_OK;
	
	g_ext_all_in_one_sig_vfy = vfy;

	return ret;
}

status_t get_fw_info(uint8_t *info, uint32_t info_sz)
{
	status_t ret = STATUS_OK;
	int sha256_ret = 0;

	if (info == NULL)
		return STATUS_INVALID_PARAMETERS;

	if (info_sz < SHA256_HASH_SZ)
		return STATUS_INVALID_PARAMETERS;

	sha256_ret = sha256(g_ext_all_in_one_sig, g_ext_all_in_one_sig_sz, info);
	if (sha256_ret)
		ret = STATUS_SEC_HASH_OP_FAIL;

	return ret;
}

void set_all_in_one_signature_buffer(uint8_t *sig, uint32_t sz)
{
  if (g_ext_all_in_one_sig != NULL)
    free(g_ext_all_in_one_sig);
  g_ext_all_in_one_sig = sig;
  g_ext_all_in_one_sig_sz = sz;
}
