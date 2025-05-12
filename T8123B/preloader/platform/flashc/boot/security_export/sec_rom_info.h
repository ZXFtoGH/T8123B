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

#ifndef _ROMINFO_H_
#define _ROMINFO_H_

#include "sys/types.h"
#include "boot/hal.h"

#define SOURCE_TYPE_ROM  0
#define SOURCE_TYPE_RAM  1

#define ROM_INFO_V2 0x2
#define GET_ROM_INFO_VER_MASK 0x0000FFFF


#define SEC_BUF_LEN (0x3000)
extern uint8_t g_sec_buf[SEC_BUF_LEN];

extern status_t update_rom_info(void);
extern status_t update_rom_info_ram(uint8_t *buf, uint32_t buf_sz);
extern status_t get_rom_info_sdl_state(uint32_t *susbdl);
extern status_t get_rom_info_sboot_state(uint32_t *sboot);
extern status_t get_sboot_state(uint32_t *sboot);
extern status_t get_rom_info_ram_version(uint32_t *version);
extern status_t get_rom_info_version(uint32_t *version);
extern status_t get_customer_name(uint8_t **cust_name, uint32_t *cust_name_size, uint8_t source_type);
extern status_t get_acc_setting(uint8_t *acc_en, uint32_t *acc_offset, uint32_t *acc_len, uint8_t source_type);
extern status_t get_pl_search_offset(uint64_t *search_offset, struct partition_info_struct *pl_part_info);

extern uint32_t  sec_sej_key_init();

extern uint32_t efuse_sbc_enabled();
extern int sec_memcmp(uint8_t *, uint8_t *,uint32_t);
extern int sec_memcpy(void *,void *,uint32_t);
extern int seclib_get_devinfo_with_index(int);
extern uint32_t get_lock_state(uint32_t *);

#endif /* _ROMINFO_H_ */
