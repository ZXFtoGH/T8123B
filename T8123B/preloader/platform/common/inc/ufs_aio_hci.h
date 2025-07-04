/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */
/* MediaTek Inc. (C) 2015. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
 * AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 * NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 * SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 * SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
 * THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
 * THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
 * CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
 * SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
 * CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
 * MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 */

#ifndef _UFS_AIO_HCI_H
#define _UFS_AIO_HCI_H

#include "ufs_aio_cfg.h"
#include "ufs_aio_types.h"

/* DMA related configurations */

/*
 * Maximum data length in one command.
 *
 * It can be changed according to scenario but notice that RAM usage
 * will also be increased (for PRDT table) with larger data length.
 */
#define UFS_AIO_MAX_DATA_SIZE_PER_CMD_MB (32)
#define UFS_AIO_MAX_SIZE_PER_SG_SEGMENT  (256 * 1024)    // UFSHCI 2.1 spec: 256 KB
#define UFS_AIO_MAX_SG_SEGMENTS          (UFS_AIO_MAX_DATA_SIZE_PER_CMD_MB * 1024 * 1024 / UFS_AIO_MAX_SIZE_PER_SG_SEGMENT)

enum {
	TASK_REQ_UPIU_SIZE_DWORDS   = 8,
	TASK_RSP_UPIU_SIZE_DWORDS   = 8,
	ALIGNED_UPIU_SIZE       = 512,
};

/* UFSHCI Registers */
enum {
	REG_CONTROLLER_CAPABILITIES     = 0x00,
	REG_UFS_VERSION             = 0x08,
	REG_CONTROLLER_DEV_ID           = 0x10,
	REG_CONTROLLER_PROD_ID          = 0x14,
	REG_AHIT                        = 0x18,
	REG_INTERRUPT_STATUS            = 0x20,
	REG_INTERRUPT_ENABLE            = 0x24,
	REG_CONTROLLER_STATUS           = 0x30,
	REG_CONTROLLER_ENABLE           = 0x34,
	REG_UIC_ERROR_CODE_PHY_ADAPTER_LAYER    = 0x38,
	REG_UIC_ERROR_CODE_DATA_LINK_LAYER  = 0x3C,
	REG_UIC_ERROR_CODE_NETWORK_LAYER    = 0x40,
	REG_UIC_ERROR_CODE_TRANSPORT_LAYER  = 0x44,
	REG_UIC_ERROR_CODE_DME          = 0x48,
	REG_UTP_TRANSFER_REQ_INT_AGG_CONTROL    = 0x4C,
	REG_UTP_TRANSFER_REQ_LIST_BASE_L    = 0x50,
	REG_UTP_TRANSFER_REQ_LIST_BASE_H    = 0x54,
	REG_UTP_TRANSFER_REQ_DOOR_BELL      = 0x58,
	REG_UTP_TRANSFER_REQ_LIST_CLEAR     = 0x5C,
	REG_UTP_TRANSFER_REQ_LIST_RUN_STOP  = 0x60,
	REG_UTP_TRANSFER_REQ_LIST_COMP_NOTIFY   = 0x64,
	REG_UTP_TASK_REQ_LIST_BASE_L        = 0x70,
	REG_UTP_TASK_REQ_LIST_BASE_H        = 0x74,
	REG_UTP_TASK_REQ_DOOR_BELL      = 0x78,
	REG_UTP_TASK_REQ_LIST_CLEAR     = 0x7C,
	REG_UTP_TASK_REQ_LIST_RUN_STOP      = 0x80,
	REG_UIC_COMMAND             = 0x90,
	REG_UIC_COMMAND_ARG_1           = 0x94,
	REG_UIC_COMMAND_ARG_2           = 0x98,
	REG_UIC_COMMAND_ARG_3           = 0x9C,
	REG_UMABA               = 0xB0,
	REG_UMABAU              = 0xB4,
	REG_UMAOMAX             = 0xB8,
	REG_UMACONF             = 0xBC,
	REG_XCNF                = 0XC8,
	REG_CDACFG          = 0xD0,
	REG_CDATX1          = 0xD4,
	REG_CDATX2          = 0xD8,
	REG_CDARX1          = 0xDC,
	REG_CDARX2          = 0xE0,
	REG_CDASTA              = 0xE4,
	REG_CRYPTO_CAPABILITY   = 0x100,
	REG_CRYPTO_CAPABILITY_X = 0x104,
	REG_XOUFS_CTRL          = 0x140,
	REG_EXTREG1             = 0x2100,
	REG_MPHYCTRL            = 0x2200,
	REG_MTK_AUTO_DEEP_STALL = 0x2210,
	REG_UFS_MTK_HW_VER = 0x2240,
	REG_UFS_MTK_OCS_ERR_STATUS = 0x2244,
};

#define UFS_BIT(x)  (1UL << (x))

#define UTP_TRANSFER_REQ_COMPL          UFS_BIT(0)
#define UIC_DME_END_PT_RESET            UFS_BIT(1)
#define UIC_ERROR               UFS_BIT(2)
#define UIC_TEST_MODE               UFS_BIT(3)
#define UIC_POWER_MODE              UFS_BIT(4)
#define UIC_HIBERNATE_EXIT          UFS_BIT(5)
#define UIC_HIBERNATE_ENTER         UFS_BIT(6)
#define UIC_LINK_LOST               UFS_BIT(7)
#define UIC_LINK_STARTUP            UFS_BIT(8)
#define UTP_TASK_REQ_COMPL          UFS_BIT(9)
#define UIC_COMMAND_COMPL           UFS_BIT(10)
#define DEVICE_FATAL_ERROR          UFS_BIT(11)
#define CONTROLLER_FATAL_ERROR          UFS_BIT(16)
#define SYSTEM_BUS_FATAL_ERROR          UFS_BIT(17)

#define UFSHCD_UIC_PWR_MASK (UIC_HIBERNATE_ENTER |\
                UIC_HIBERNATE_EXIT |\
                UIC_POWER_MODE)

#define UFSHCD_UIC_MASK     (UIC_COMMAND_COMPL | UFSHCD_UIC_PWR_MASK)

#define UFSHCD_ERROR_MASK   (UIC_ERROR |\
                DEVICE_FATAL_ERROR |\
                CONTROLLER_FATAL_ERROR |\
                SYSTEM_BUS_FATAL_ERROR)

#define INT_FATAL_ERRORS    (DEVICE_FATAL_ERROR |\
                CONTROLLER_FATAL_ERROR |\
                SYSTEM_BUS_FATAL_ERROR)

/* HCS - Host Controller Status 30h */
#define DEVICE_PRESENT              UFS_BIT(0)
#define UTP_TRANSFER_REQ_LIST_READY     UFS_BIT(1)
#define UTP_TASK_REQ_LIST_READY         UFS_BIT(2)
#define UIC_COMMAND_READY           UFS_BIT(3)
#define HOST_ERROR_INDICATOR            UFS_BIT(4)
#define DEVICE_ERROR_INDICATOR          UFS_BIT(5)
#define UIC_POWER_MODE_CHANGE_REQ_STATUS_MASK   UFS_MASK(0x7, 8)

enum {
	PWR_OK      = 0x0,
	PWR_LOCAL   = 0x01,
	PWR_REMOTE  = 0x02,
	PWR_BUSY    = 0x03,
	PWR_ERROR_CAP   = 0x04,
	PWR_FATAL_ERROR = 0x05,
};

/* HCE - Host Controller Enable 34h */
#define CONTROLLER_ENABLE   UFS_BIT(0)
#define CONTROLLER_DISABLE  0x0

/* UECPA - Host UIC Error Code PHY Adapter Layer 38h */
#define UIC_PHY_ADAPTER_LAYER_ERROR         UFS_BIT(31)
#define UIC_PHY_ADAPTER_LAYER_ERROR_CODE_MASK       0x1F

/* UECDL - Host UIC Error Code Data Link Layer 3Ch */
#define UIC_DATA_LINK_LAYER_ERROR       UFS_BIT(31)
#define UIC_DATA_LINK_LAYER_ERROR_CODE_MASK 0x7FFF
#define UIC_DATA_LINK_LAYER_ERROR_PA_INIT   0x2000

/* UECN - Host UIC Error Code Network Layer 40h */
#define UIC_NETWORK_LAYER_ERROR         UFS_BIT(31)
#define UIC_NETWORK_LAYER_ERROR_CODE_MASK   0x7

/* UECT - Host UIC Error Code Transport Layer 44h */
#define UIC_TRANSPORT_LAYER_ERROR       UFS_BIT(31)
#define UIC_TRANSPORT_LAYER_ERROR_CODE_MASK 0x7F

/* UECDME - Host UIC Error Code DME 48h */
#define UIC_DME_ERROR           UFS_BIT(31)
#define UIC_DME_ERROR_CODE_MASK     0x1

#define INT_AGGR_TIMEOUT_VAL_MASK       0xFF
#define INT_AGGR_COUNTER_THRESHOLD_MASK     UFS_MASK(0x1F, 8)
#define INT_AGGR_COUNTER_AND_TIMER_RESET    UFS_BIT(16)
#define INT_AGGR_STATUS_BIT         UFS_BIT(20)
#define INT_AGGR_PARAM_WRITE            UFS_BIT(24)
#define INT_AGGR_ENABLE             UFS_BIT(31)

/* UTRLRSR - UTP Transfer Request Run-Stop Register 60h */
#define UTP_TRANSFER_REQ_LIST_RUN_STOP_BIT  UFS_BIT(0)

/* UTMRLRSR - UTP Task Management Request Run-Stop Register 80h */
#define UTP_TASK_REQ_LIST_RUN_STOP_BIT      UFS_BIT(0)

/* UICCMD - UIC Command */
#define COMMAND_OPCODE_MASK     0xFF
#define GEN_SELECTOR_INDEX_MASK     0xFFFF

/* CDACFG */
#define CDARXALIGN (0x1<<29)
#define CDAEN (0x1<<28)
#define CDAEOM (0x1<<16)

/* CDASTA */
#define FLAG_CDARES (0x3 << 20)
#define FLAG_CDABUSY (0x1 << 19)
#define FLAG_CDASTA (0x1 << 18)

#define MIB_ATTRIBUTE_MASK      UFS_MASK(0xFFFF, 16)
#define RESET_LEVEL         0xFF

#define ATTR_SET_TYPE_MASK      UFS_MASK(0xFF, 16)
#define CONFIG_RESULT_CODE_MASK     0xFF
#define GENERIC_ERROR_CODE_MASK     0xFF

#define UIC_ARG_MIB_SEL(attr, sel)  ((((unsigned)(attr) & 0xFFFF) << 16) |\
                     ((sel) & 0xFFFF))
#define UIC_ARG_MIB(attr)       UIC_ARG_MIB_SEL(attr, 0)
#define UIC_ARG_ATTR_TYPE(t)        (((t) & 0xFF) << 16)
#define UIC_GET_ATTR_ID(v)      (((v) >> 16) & 0xFFFF)

/* UIC Commands */
enum uic_cmd_dme {
	UIC_CMD_DME_GET         = 0x01,
	UIC_CMD_DME_SET         = 0x02,
	UIC_CMD_DME_PEER_GET        = 0x03,
	UIC_CMD_DME_PEER_SET        = 0x04,
	UIC_CMD_DME_POWERON     = 0x10,
	UIC_CMD_DME_POWEROFF        = 0x11,
	UIC_CMD_DME_ENABLE      = 0x12,
	UIC_CMD_DME_RESET       = 0x14,
	UIC_CMD_DME_END_PT_RST      = 0x15,
	UIC_CMD_DME_LINK_STARTUP    = 0x16,
	UIC_CMD_DME_HIBER_ENTER     = 0x17,
	UIC_CMD_DME_HIBER_EXIT      = 0x18,
	UIC_CMD_DME_TEST_MODE       = 0x1A,
};

/* UIC Config result code / Generic error code */
enum {
	UIC_CMD_RESULT_SUCCESS          = 0x00,
	UIC_CMD_RESULT_INVALID_ATTR     = 0x01,
	UIC_CMD_RESULT_FAILURE          = 0x01,
	UIC_CMD_RESULT_INVALID_ATTR_VALUE   = 0x02,
	UIC_CMD_RESULT_READ_ONLY_ATTR       = 0x03,
	UIC_CMD_RESULT_WRITE_ONLY_ATTR      = 0x04,
	UIC_CMD_RESULT_BAD_INDEX        = 0x05,
	UIC_CMD_RESULT_LOCKED_ATTR      = 0x06,
	UIC_CMD_RESULT_BAD_TEST_FEATURE_INDEX   = 0x07,
	UIC_CMD_RESULT_PEER_COMM_FAILURE    = 0x08,
	UIC_CMD_RESULT_BUSY         = 0x09,
	UIC_CMD_RESULT_DME_FAILURE      = 0x0A,
};

#define MASK_UIC_COMMAND_RESULT         0xFF

#define INT_AGGR_COUNTER_THLD_VAL(c)    (((c) & 0x1F) << 8)
#define INT_AGGR_TIMEOUT_VAL(t)     (((t) & 0xFF) << 0)

/* Interrupt disable masks */
enum {
	/* Interrupt disable mask for UFSHCI v1.0 */
	INTERRUPT_MASK_ALL_VER_10   = 0x30FFF,
	INTERRUPT_MASK_RW_VER_10    = 0x30000,

	/* Interrupt disable mask for UFSHCI v1.1 */
	INTERRUPT_MASK_ALL_VER_11   = 0x31FFF,
};

/*
 * Request Descriptor Definitions
 */

/* Transfer request command type */
enum {
	UTP_CMD_TYPE_SCSI       = 0x0,
	UTP_CMD_TYPE_UFS        = 0x1,
	UTP_CMD_TYPE_DEV_MANAGE     = 0x2,
};

enum {
	UTP_SCSI_COMMAND        = 0x00000000,
	UTP_NATIVE_UFS_COMMAND      = 0x10000000,
	UTP_DEVICE_MANAGEMENT_FUNCTION  = 0x20000000,
	UTP_REQ_DESC_INT_CMD        = 0x01000000,
};

/* UTP Transfer Request Data Direction (DD) */
enum {
	UTP_NO_DATA_TRANSFER    = 0x00000000,
	UTP_HOST_TO_DEVICE  = 0x02000000,
	UTP_DEVICE_TO_HOST  = 0x04000000,
};

/* Overall command status values */
enum {
	OCS_SUCCESS         = 0x0,
	OCS_INVALID_CMD_TABLE_ATTR  = 0x1,
	OCS_INVALID_PRDT_ATTR       = 0x2,
	OCS_MISMATCH_DATA_BUF_SIZE  = 0x3,
	OCS_MISMATCH_RESP_UPIU_SIZE = 0x4,
	OCS_PEER_COMM_FAILURE       = 0x5,
	OCS_ABORTED         = 0x6,
	OCS_FATAL_ERROR         = 0x7,
	OCS_INVALID_COMMAND_STATUS  = 0x0F,
	MASK_OCS            = 0x0F,
};

/* The maximum length of the data byte count field in the PRDT is 256KB */
#define PRDT_DATA_BYTE_COUNT_MAX    (256 * 1024)
/* The granularity of the data byte count field in the PRDT is 32-bit */
#define PRDT_DATA_BYTE_COUNT_PAD    4

/**
 * struct ufs_aio_sg_entry - UFSHCI PRD Entry
 * @base_addr: Lower 32bit physical address DW-0
 * @upper_addr: Upper 32bit physical address DW-1
 * @reserved: Reserved for future use DW-2
 * @size: size of physical segment DW-3
 */
struct ufs_aio_sg_entry {
	__le32    base_addr;
	__le32    upper_addr;
	__le32    reserved;
	__le32    size;
};

/**
 * struct utp_transfer_cmd_desc - UFS Command Descriptor structure
 * @command_upiu: Command UPIU Frame address
 * @response_upiu: Response UPIU Frame address
 * @prd_table: Physical Region Descriptor
 */
struct utp_transfer_cmd_desc {
	u8 command_upiu[ALIGNED_UPIU_SIZE];
	u8 response_upiu[ALIGNED_UPIU_SIZE];
	struct ufs_aio_sg_entry    prd_table[UFS_AIO_MAX_SG_SEGMENTS];
};

/**
 * struct request_desc_header - Descriptor Header common to both UTRD and UTMRD
 * @dword0: Descriptor Header DW0
 * @dword1: Descriptor Header DW1
 * @dword2: Descriptor Header DW2
 * @dword3: Descriptor Header DW3
 */
struct request_desc_header {
	__le32 dword_0;
	__le32 dword_1;
	__le32 dword_2;
	__le32 dword_3;
};

/**
 * struct utp_transfer_req_desc - UTRD structure
 * @header: UTRD header DW-0 to DW-3
 * @command_desc_base_addr_lo: UCD base address low DW-4
 * @command_desc_base_addr_hi: UCD base address high DW-5
 * @response_upiu_length: response UPIU length DW-6
 * @response_upiu_offset: response UPIU offset DW-6
 * @prd_table_length: Physical region descriptor length DW-7
 * @prd_table_offset: Physical region descriptor offset DW-7
 */
struct utp_transfer_req_desc {

	/* DW 0-3 */
	struct request_desc_header header;

	/* DW 4-5*/
	__le32  command_desc_base_addr_lo;
	__le32  command_desc_base_addr_hi;

	/* DW 6 */
	__le16  response_upiu_length;
	__le16  response_upiu_offset;

	/* DW 7 */
	__le16  prd_table_length;
	__le16  prd_table_offset;
};

/**
 * struct utp_task_req_desc - UTMRD structure
 * @header: UTMRD header DW-0 to DW-3
 * @task_req_upiu: Pointer to task request UPIU DW-4 to DW-11
 * @task_rsp_upiu: Pointer to task response UPIU DW12 to DW-19
 */
struct utp_task_req_desc {

	/* DW 0-3 */
	struct request_desc_header header;

	/* DW 4-11 */
	__le32 task_req_upiu[TASK_REQ_UPIU_SIZE_DWORDS];

	/* DW 12-19 */
	__le32 task_rsp_upiu[TASK_RSP_UPIU_SIZE_DWORDS];
};

#endif /* End of Header */

