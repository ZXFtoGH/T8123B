/* Copyright Statement:
*
* This software/firmware and related documentation ("MediaTek Software") are
* protected under relevant copyright laws. The information contained herein
* is confidential and proprietary to MediaTek Inc. and/or its licensors.
* Without the prior written permission of MediaTek inc. and/or its licensors,
* any reproduction, modification, use or disclosure of MediaTek Software,
* and information contained herein, in whole or in part, shall be strictly prohibited.
*/
/* MediaTek Inc. (C) 2011. All rights reserved.
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
*
* The following software/firmware and/or related documentation ("MediaTek Software")
* have been modified by MediaTek Inc. All revisions are subject to any receiver's
* applicable license agreements with MediaTek Inc.
*/


#ifndef CUST_SEC_CTRL_H
#define CUST_SEC_CTRL_H

#include "typedefs.h"
#include "proj_cfg.h"
#include "keypad.h"

/**************************************************************************
 * [ROM INFO]
 **************************************************************************/
#define PROJECT_NAME                        "CUST"
#define PLATFORM_NAME                       "MT6752"


/**************************************************************************
 * [CRYPTO SEED]
 **************************************************************************/
#define CUSTOM_CRYPTO_SEED_SIZE             (16)
#define CUSTOM_CRYPTO_SEED                  "1A52A367CB12C458"

/**************************************************************************
 * [SML AES KEY CONTROL]
 **************************************************************************/
/* It can be enabled only if SUSBDL is turned on */
/* Please make sure SUSBDL is on before enabling this flag */
//#define SML_AES_KEY_ANTICLONE_EN

/**************************************************************************
 * [S-USBDL]
 **************************************************************************/
/* S-USBDL Attribute */
#define ATTR_SUSBDL_DISABLE                 0x00
#define ATTR_SUSBDL_ENABLE                  0x11
#define ATTR_SUSBDL_ONLY_ENABLE_ON_SCHIP    0x22
/* S-USBDL Control */
#define SEC_USBDL_CFG                       CUSTOM_SUSBDL_CFG

/**************************************************************************
 * [FLASHTOOL SECURE CONFIG for (for both of SLA and NON-SLA mode], 32bits
 * It's not recommended to use 32 bits (v3) mode now. Please use FLASHTOOL_SEC_CFG_64 instead
 **************************************************************************/
//#define FLASHTOOL_SEC_CFG
//#define BYPASS_CHECK_IMAGE_0_NAME           ""
//#define BYPASS_CHECK_IMAGE_0_OFFSET         0x0
//#define BYPASS_CHECK_IMAGE_0_LENGTH         0x0
//#define BYPASS_CHECK_IMAGE_1_NAME           ""
//#define BYPASS_CHECK_IMAGE_1_OFFSET         0x0
//#define BYPASS_CHECK_IMAGE_1_LENGTH         0x0
//#define BYPASS_CHECK_IMAGE_2_NAME           ""
//#define BYPASS_CHECK_IMAGE_2_OFFSET         0x0
//#define BYPASS_CHECK_IMAGE_2_LENGTH         0x0

/**************************************************************************
 * [FLASHTOOL SECURE CONFIG (for both of SLA and NON-SLA mode], 64 bits for v4 sign format
 **************************************************************************/
//#define FLASHTOOL_SEC_CFG_64
#ifdef FLASHTOOL_SEC_CFG_64
#define BYPASS_CHECK_IMAGE_0_NAME           ""
#define BYPASS_CHECK_IMAGE_0_OFFSET         0x0
#define BYPASS_CHECK_IMAGE_1_NAME           ""
#define BYPASS_CHECK_IMAGE_1_OFFSET         0x0
#define BYPASS_CHECK_IMAGE_2_NAME           ""
#define BYPASS_CHECK_IMAGE_2_OFFSET         0x0
#endif
/**************************************************************************
 * [FLASHTOOL FORBIT DOWNLOAD CONFIG (for NSLA mode only)] , 32 bits
 * It's not recommended to use 32 bits (v3) mode now. Please use FLASHTOOL_FORBID_DL_NSLA_CFG_64 instead
 **************************************************************************/
//#define FLASHTOOL_FORBID_DL_NSLA_CFG
//#define FORBID_DL_IMAGE_0_NAME              ""
//#define FORBID_DL_IMAGE_0_OFFSET            0x0
//#define FORBID_DL_IMAGE_0_LENGTH            0x0
//#define FORBID_DL_IMAGE_1_NAME              ""
//#define FORBID_DL_IMAGE_1_OFFSET            0x0
//#define FORBID_DL_IMAGE_1_LENGTH            0x0

/**************************************************************************
 * [FLASHTOOL FORBIT DOWNLOAD CONFIG (for NSLA mode only)], 64 bits for v4 sign format
 **************************************************************************/
//#define FLASHTOOL_FORBID_DL_NSLA_CFG_64
#ifdef FLASHTOOL_FORBID_DL_NSLA_CFG_64
#define FORBID_DL_IMAGE_0_NAME              ""
#define FORBID_DL_IMAGE_0_OFFSET            0x0
#define FORBID_DL_IMAGE_1_NAME              ""
#define FORBID_DL_IMAGE_1_OFFSET            0x0
#endif

#define SEC_USBDL_WITHOUT_SLA_ENABLE

#ifdef SEC_USBDL_WITHOUT_SLA_ENABLE
//#define USBDL_DETECT_VIA_KEY
/* if com port wait key is enabled, define the key*/
#ifdef USBDL_DETECT_VIA_KEY
#define COM_WAIT_KEY    KPD_DL_KEY3
#endif
//#define USBDL_DETECT_VIA_AT_COMMAND
#endif

/**************************************************************************
 * [S-BOOT]
 **************************************************************************/
/* S-BOOT Attribute */
#define ATTR_SBOOT_DISABLE                  0x00
#define ATTR_SBOOT_ENABLE                   0x11
#define ATTR_SBOOT_ONLY_ENABLE_ON_SCHIP     0x22
/* S-BOOT Control */
#define SEC_BOOT_CFG                        CUSTOM_SBOOT_CFG

/* Customized Secure Boot */
//#define CUSTOMIZED_SECURE_PARTITION_SUPPORT
#ifdef CUSTOMIZED_SECURE_PARTITION_SUPPORT
#define SBOOT_CUST_PART1    ""
#define SBOOT_CUST_PART2    ""
#endif

/* For Custom Partition Verification*/
#define VERIFY_PART_CUST                   (FALSE)
#define VERIFY_PART_CUST_NAME              ""

/*
    RSA2048 public key for verifying mtee image
    It should be the same as AUTH_PARAM_N in alps\mediatek\custom\mt8167_evb\trustzone\TRUSTZONE_IMG_PROTECT_CFG.ini
*/
#define MTEE_IMG_VFY_PUBK_SZ 256

#if !defined(CFG_MTK_IN_HOUSE_TEE_DEV)
#define MTEE_IMG_VFY_PUBK \
0xe7, 0x46, 0x78, 0x0a, 0x78, 0x52, 0xfb, 0x16, \
0x2e, 0xd1, 0x0a, 0x61, 0x34, 0x8f, 0x2b, 0xd6, \
0x1d, 0x28, 0xea, 0x55, 0x09, 0x4e, 0x28, 0x2a, \
0xf8, 0xeb, 0xdb, 0x86, 0xa9, 0xaf, 0xeb, 0x85, \
0xe7, 0x23, 0xe1, 0xce, 0xdf, 0x79, 0xf7, 0x9f, \
0xf6, 0x05, 0x12, 0xb9, 0xb5, 0x71, 0x44, 0x4b, \
0xc4, 0xe7, 0x05, 0xd2, 0x3c, 0x90, 0x3f, 0x7f, \
0xe8, 0x33, 0x62, 0x60, 0x2d, 0x8d, 0xbe, 0xea, \
0xc0, 0x0e, 0x6c, 0xba, 0xb6, 0x48, 0xd0, 0xd5, \
0xdd, 0x54, 0x66, 0x6b, 0xe7, 0xce, 0x01, 0xce, \
0x6c, 0x1b, 0x21, 0x46, 0x78, 0x18, 0x0d, 0x73, \
0x64, 0x00, 0xcb, 0x0f, 0xdf, 0x0b, 0x8b, 0xbf, \
0xf8, 0xe0, 0x33, 0x31, 0xe4, 0xfa, 0x77, 0xac, \
0x9a, 0xbe, 0x47, 0x2f, 0x0f, 0xa5, 0x61, 0xf4, \
0x2c, 0xe3, 0xde, 0xd6, 0x26, 0x57, 0xbe, 0xc4, \
0xe8, 0x02, 0xd8, 0x71, 0x84, 0x5e, 0x16, 0x3d, \
0xb0, 0x51, 0xba, 0x0c, 0x86, 0x29, 0x18, 0xce, \
0x98, 0x2f, 0xd3, 0x72, 0xee, 0xdd, 0x8f, 0xa0, \
0xa9, 0xf7, 0x91, 0x72, 0xac, 0x8a, 0xd4, 0x60, \
0x9f, 0x28, 0xb0, 0x4b, 0x3a, 0xd9, 0xdf, 0x0b, \
0x3a, 0x14, 0x7f, 0xa2, 0xaa, 0xe2, 0xe2, 0x14, \
0xf4, 0x98, 0x03, 0x65, 0xba, 0x03, 0xea, 0xd6, \
0xfa, 0x43, 0x9e, 0x29, 0x61, 0xb5, 0x40, 0x14, \
0x65, 0x6b, 0x2a, 0x4e, 0xc5, 0xc2, 0xdb, 0x32, \
0x4a, 0xe9, 0x4a, 0xcf, 0xf1, 0xbf, 0x5f, 0xcb, \
0x52, 0x92, 0x06, 0xf1, 0xa7, 0x1d, 0xdc, 0x5a, \
0xb7, 0x02, 0x9f, 0xd4, 0x8c, 0xa1, 0xac, 0x8a, \
0x82, 0xbe, 0xc4, 0x0f, 0x5a, 0xbb, 0xfa, 0xb6, \
0x5a, 0x88, 0x4d, 0x63, 0x2d, 0x09, 0xf8, 0x9e, \
0x0e, 0x8d, 0x53, 0xf3, 0x4d, 0x22, 0xa3, 0x0e, \
0xf5, 0x98, 0xa9, 0x66, 0xcb, 0x7b, 0x6d, 0xae, \
0x70, 0xea, 0xcf, 0x98, 0xfc, 0x36, 0xcc, 0xbd
#else
/* CFG_MTK_IN_HOUSE_TEE_DEV is defined,
 * the key is also store in following path for signing.
 * vendor/mediatek/proprietary/trustzone/mtee/build/test/${PLATFORM} */

#define MTEE_IMG_VFY_PUBK \
0xDA, 0xCD, 0x8B, 0x5F, 0xDA, 0x8A, 0x76, 0x6F, \
0xB7, 0xBC, 0xAA, 0x43, 0xF0, 0xB1, 0x69, 0x15, \
0xCE, 0x7B, 0x47, 0x71, 0x4F, 0x13, 0x95, 0xFD, \
0xEB, 0xCF, 0x12, 0xA2, 0xD4, 0x11, 0x55, 0xB0, \
0xFB, 0x58, 0x7A, 0x51, 0xFE, 0xCC, 0xCB, 0x4D, \
0xDA, 0x1C, 0x8E, 0x5E, 0xB9, 0xEB, 0x69, 0xB8, \
0x6D, 0xAF, 0x2C, 0x62, 0x0F, 0x6C, 0x27, 0x35, \
0x21, 0x5A, 0x5F, 0x22, 0xC0, 0xB6, 0xCE, 0x37, \
0x7A, 0xA0, 0xD0, 0x7E, 0xB3, 0x8E, 0xD3, 0x40, \
0xB5, 0x62, 0x9F, 0xC2, 0x89, 0x04, 0x94, 0xB0, \
0x78, 0xA6, 0x3D, 0x6D, 0x07, 0xFD, 0xEA, 0xCD, \
0xBE, 0x3E, 0x7F, 0x27, 0xFD, 0xE4, 0xB1, 0x43, \
0xF4, 0x9D, 0xB4, 0x97, 0x14, 0x37, 0xE6, 0xD0, \
0x0D, 0x9E, 0x18, 0xB5, 0x6F, 0x02, 0xDA, 0xBE, \
0xB0, 0x00, 0x0B, 0x6E, 0x79, 0x51, 0x6D, 0x0C, \
0x80, 0x74, 0xB5, 0xA4, 0x25, 0x69, 0xFD, 0x0D, \
0x91, 0x96, 0x65, 0x5D, 0x2A, 0x40, 0x30, 0xD4, \
0x2D, 0xFE, 0x05, 0xE9, 0xF6, 0x48, 0x83, 0xE6, \
0xD5, 0xF7, 0x9A, 0x5B, 0xFA, 0x3E, 0x70, 0x14, \
0xC9, 0xA6, 0x28, 0x53, 0xDC, 0x1F, 0x21, 0xD5, \
0xD6, 0x26, 0xF4, 0xD0, 0x84, 0x6D, 0xB1, 0x64, \
0x52, 0x18, 0x7D, 0xD7, 0x76, 0xE8, 0x88, 0x6B, \
0x48, 0xC2, 0x10, 0xC9, 0xE2, 0x08, 0x05, 0x9E, \
0x7C, 0xAF, 0xC9, 0x97, 0xFD, 0x2C, 0xA2, 0x10, \
0x77, 0x5C, 0x1A, 0x5D, 0x9A, 0xA2, 0x61, 0x25, \
0x2F, 0xB9, 0x75, 0x26, 0x8D, 0x97, 0x0C, 0x62, \
0x73, 0x38, 0x71, 0xD5, 0x78, 0x14, 0x09, 0x8A, \
0x45, 0x3D, 0xF9, 0x2B, 0xC6, 0xCA, 0x19, 0x02, \
0x5C, 0xD9, 0xD4, 0x30, 0xF0, 0x2E, 0xE4, 0x6F, \
0x80, 0xDE, 0x6C, 0x63, 0xEA, 0x80, 0x2B, 0xEF, \
0x90, 0x67, 0x3A, 0xAC, 0x4C, 0x66, 0x67, 0xF2, \
0x88, 0x3F, 0xB4, 0x50, 0x1F, 0xA7, 0x74, 0x55
#endif

/**************************************************************************
 * [DEFINITION CHECK]
 **************************************************************************/
#ifdef SML_AES_KEY_ANTICLONE_EN
#ifndef SECRO_IMG_ANTICLONE_EN
#error "SML_AES_KEY_ANTICLONE_EN is defined. Should also enable SECRO_IMG_ANTICLONE_EN"
#endif
#endif

#if MTK_SECURITY_SW_SUPPORT
#ifndef SEC_USBDL_CFG
#error "MTK_SECURITY_SW_SUPPORT is NOT disabled. Should define SEC_USBDL_CFG "
#endif
#endif

#if MTK_SECURITY_SW_SUPPORT
#ifndef SEC_BOOT_CFG
#error "MTK_SECURITY_SW_SUPPORT is NOT disabled. Should define SEC_BOOT_CFG"
#endif
#endif

#if MTK_SECURITY_SW_SUPPORT
#ifndef SEC_USBDL_WITHOUT_SLA_ENABLE
#error "MTK_SECURITY_SW_SUPPORT is NOT disabled. Should define SEC_USBDL_WITHOUT_SLA_ENABLE"
#endif
#endif

#ifdef USBDL_DETECT_VIA_KEY
#ifndef SEC_USBDL_WITHOUT_SLA_ENABLE
#error "USBDL_DETECT_VIA_KEY can only be enabled when SEC_USBDL_WITHOUT_SLA_ENABLE is enabled"
#endif
#ifndef COM_WAIT_KEY
#error "COM_WAIT_KEY is not defined!!"
#endif
#endif

#ifdef USBDL_DETECT_VIA_AT_COMMAND
#ifndef SEC_USBDL_WITHOUT_SLA_ENABLE
#error "USBDL_DETECT_VIA_AT_COMMAND can only be enabled when SEC_USBDL_WITHOUT_SLA_ENABLE is enabled"
#endif
#endif


#endif   /* CUST_SEC_CTRL_H */
