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
 fix* The following software/firmware and/or related documentation ("MediaTek Software")
 * have been modified by MediaTek Inc. All revisions are subject to any receiver's
 * applicable license agreements with MediaTek Inc.
 */

#ifndef MT6983_H
#define MT6983_H

/*=======================================================================*/
/* Constant Definitions                                                  */
/*=======================================================================*/

#define IO_PHYS             (0x10000000)
#define IO_SIZE             (0x02000000)

#define VER_BASE            (0x08000000)

/*=======================================================================*/
/* Register Bases                                                        */
/*=======================================================================*/
#define TOPCKGEN_BASE       (IO_PHYS)
#define VLPTOPCKGEN_BASE    (IO_PHYS + 0x0C013000)
#define INFRACFG_AO_BASE    (IO_PHYS + 0x00001000)
#define INFRACFG_AO_MEM_BASE      (IO_PHYS + 0x00270000)
#define SUB_INFRACFG_AO_MEM_BASE  (IO_PHYS + 0x0030E000)
#define PERICFG_AO_BASE     (IO_PHYS + 0x01036000)
#define INFRACFG_BASE       (IO_PHYS + 0x0020E000)
#define APMIXED_BASE        (IO_PHYS + 0x0000C000)
#define MMSYS_CONFIG_BASE   (IO_PHYS + 0x04000000)
#define MMSYS1_CONFIG_BASE   (IO_PHYS + 0x04400000)
#define MMINFRA_CONFIG_BASE	(IO_PHYS + 0x0E800000)
#define MDPSYS_CONFIG_BASE  (IO_PHYS + 0x0F000000)
#define MFGPLL_BASE		(IO_PHYS + 0x03FA0000)
#define GPUEBPLL_BASE		(IO_PHYS + 0x03FA0800)
#define MFGSCPLL_BASE		(IO_PHYS + 0x03FA0C00)
#define MCUCFG_BASE         (0x0C530000)

#define IMG_BASE       (0x15000000)
#define DIP_TOP_BASE   (0x15110000)
#define DIP_NR_BASE    (0x15130000)
#define IPE_BASE       (0x15330000)

#define IOCFG_RM_BASE       (0x11C20000)
#define IOCFG_RT_BASE       (0x11C30000)
#define IOCFG_RMM_BASE      (0x11C40000)
#define IOCFG_BL_BASE       (0x11D10000)
#define IOCFG_BM_BASE       (0x11D30000)
#define IOCFG_BR_BASE       (0x11D40000)
#define IOCFG_BRR_BASE      (0x11D50000)
#define IOCFG_LB_BASE       (0x11D60000)
#define IOCFG_LM_BASE       (0x11E10000)
#define IOCFG_RTT_BASE      (0x11EA0000)
#define IOCFG_TR_BASE       (0x11F10000)
#define IOCFG_TL_BASE       (0x11F20000)
#define IOCFG_LT_BASE       (0x11F30000)

#define PERICFG_BASE        (IO_PHYS + 0x00003000)

//#define PERI_CON_BASE       (IO_PHYS + 0x00003000)

#define GPIO_BASE           (IO_PHYS + 0x00005000)
#define SPM_BASE            (IO_PHYS + 0x0C001000)
#define RGU_BASE            (IO_PHYS + 0x0C007000)
#define GPT_BASE            (IO_PHYS + 0x0C008000)
#define SYSTIMER_BASE       (IO_PHYS + 0x0C011000)
#define TIA_BASE            (IO_PHYS + 0x0C015000)
#define PMIF_SPMI_P_BASE    (IO_PHYS + 0x0C014000)
#define PMIF_SPMI_BASE      (IO_PHYS + 0x0C015000)
#define PMICSPI_MST_BASE    (IO_PHYS + 0x00028000)
#define SPMI_MST_P_BASE     (IO_PHYS + 0x0C800000)
#define SPMI_MST_BASE       (IO_PHYS + 0x0C801000)
#define VLPCFG_BASE         (IO_PHYS + 0x0C00C000)
#define DDRPHY_BASE         (IO_PHYS + 0x00330000)
#define KPD_BASE            (IO_PHYS + 0x0C00E000)

#define DEM_BASE            (0x0D0A0000)

#define MCUSYS_CFGREG_BASE  (0x0C000000)
#define MCUSYS_PLLREG_BASE  (0x0C030000)

//#define CA7MCUCFG_BASE      (IO_PHYS + 0x00200100)
//#define CA15L_CONFIG_BASE   (IO_PHYS + 0x00200200)
#define SRAMROM_BASE        (IO_PHYS + 0x00214000)
#define GICD_BASE           (0x0c400000)
#define GICR_BASE           (0x0c440000)

#define DEVINFO_BASE        (IO_PHYS + 0x01F10000)
#define UART0_BASE          (IO_PHYS + 0x01001000)
#define UART1_BASE          (IO_PHYS + 0x01002000)
#define UART2_BASE          (IO_PHYS + 0x01003000)
#define UART3_BASE          (IO_PHYS + 0x01004000)
#define SPI_BASE            (IO_PHYS + 0x010F0000)
#define NFI_BASE            (IO_PHYS + 0x0100D000) /* FIXME: not list in memory map */
#define NFIECC_BASE         (IO_PHYS + 0x0100E000) /* FIXME: not list in memory map */
#define MSDC0_TOP_BASE      (IO_PHYS + 0x01F50000)
#define MSDC1_TOP_BASE      (IO_PHYS + 0x01E70000)
#define MSDC2_TOP_BASE      (IO_PHYS + 0x01F60000)
#define MSDC0_BASE          (IO_PHYS + 0x01240000)
#define MSDC1_BASE          (IO_PHYS + 0x01240000)
#define MSDC2_BASE          (IO_PHYS + 0x01242000)
#define MSDC3_BASE          (IO_PHYS + 0x01260000) /* Reserved in mt6761 */
#define U3D_BASE			(IO_PHYS + 0x01200000) /* MAC: 0x1120_0000 */
#define USB_SIF_BASE		(IO_PHYS + 0x01E40000) /* PHY: 0x11E4_0000 */
#define USB1P_SIF_BASE      (IO_PHYS + 0x01C40000)
#define PCIE_BASE           (IO_PHYS + 0x093F0000)
#define PCIE_PHY_BASE       (IO_PHYS + 0x01E20000)

#define CPUXGPT_BASE        (IO_PHYS + 0x00200000)

#define SUB_INFRACFG_AO_BASE  (IO_PHYS + 0x0030E000)

/*=======================================================================*/
/* AP HW code offset                                                     */
/*=======================================================================*/
#define APHW_CODE           (VER_BASE)
#define APHW_SUBCODE        (VER_BASE + 0x04)
#define APHW_VER            (VER_BASE + 0x08)
#define APSW_VER            (VER_BASE + 0x0C)

// #define AMCONFG_BASE        (0xFFFFFFFF)

/*=======================================================================*/
/* USB register offset                                                   */
/*=======================================================================*/
#define SSUSB_DEV_BASE                      (U3D_BASE + 0x1000) /* FIXME: not list in memory map */
#define SSUSB_EPCTL_CSR_BASE                (U3D_BASE + 0x1800) /* FIXME: not list in memory map */
#define SSUSB_USB3_MAC_CSR_BASE             (U3D_BASE + 0x2400) /* FIXME: not list in memory map */
#define SSUSB_USB3_SYS_CSR_BASE             (U3D_BASE + 0x2400) /* FIXME: not list in memory map */
#define SSUSB_USB2_CSR_BASE                 (U3D_BASE + 0x3400) /* FIXME: not list in memory map */

#define SSUSB_SIFSLV_IPPC_BASE              (U3D_BASE + 0x3E00) /* FIXME: not list in memory map */

#define SSUSB_SIFSLV_SPLLC_BASE             (USB_SIF_BASE + 0x700) /* FIXME: not list in memory map */
#define SSUSB_SIFSLV_U2PHY_COM_BASE         (USB_SIF_BASE + 0x300) /* FIXME: not list in memory map */
#define SSUSB_SIFSLV_U3PHYD_BASE            (USB_SIF_BASE + 0x900) /* FIXME: not list in memory map */
#define SSUSB_SIFSLV_U2PHY_COM_SIV_B_BASE   (USB_SIF_BASE + 0x300) /* FIXME: not list in memory map */
#define SSUSB_USB30_PHYA_SIV_B2_BASE        (USB_SIF_BASE + 0xA00) /* FIXME: not list in memory map */
#define SSUSB_USB30_PHYA_SIV_B_BASE         (USB_SIF_BASE + 0xB00) /* FIXME: not list in memory map */
#define SSUSB_SIFSLV_U3PHYA_DA_BASE         (USB_SIF_BASE + 0xC00) /* FIXME: not list in memory map */


/*=======================================================================*/
/* USB download control                                                  */
/*=======================================================================*/
#define SECURITY_AO                 (0x1C00B000)
#define BOOT_MISC0                  (SECURITY_AO + 0x0080)
#define MISC_LOCK_KEY               (SECURITY_AO + 0x0100)
#define RST_CON                     (SECURITY_AO + 0x0108)

#define MISC_LOCK_KEY_MAGIC    0xAD98
#define USBDL_FLAG    BOOT_MISC0


#define USBDL_BIT_EN        (0x00000001) /* 1: download bit enabled */
#define USBDL_BROM          (0x00000002) /* 0: usbdl by brom; 1: usbdl by bootloader */
#define USBDL_TIMEOUT_MASK  (0x0000FFFC) /* 14-bit timeout: 0x0000~0x3FFE: second; 0x3FFFF: no timeout */
#define USBDL_TIMEOUT_MAX   (USBDL_TIMEOUT_MASK >> 2) /* maximum timeout indicates no timeout */
#define USBDL_MAGIC         (0x444C0000) /* Brom will check this magic number */


#define SRAMROM_USBDL_TO_DIS (SRAMROM_BASE + 0x0054)
#define USBDL_TO_DIS         (0x00000001)

/*=======================================================================*/
/* NAND Control                                                          */
/*=======================================================================*/
#define NAND_PAGE_SIZE                  (2048)  // (Bytes)
#define NAND_BLOCK_BLKS                 (64)    // 64 nand pages = 128KB
#define NAND_PAGE_SHIFT                 (9)
#define NAND_LARGE_PAGE                 (11)    // large page
#define NAND_SMALL_PAGE                 (9)     // small page
#define NAND_BUS_WIDTH_8                (8)
#define NAND_BUS_WIDTH_16               (16)
#define NAND_FDM_SIZE                   (8)
#define NAND_ECC_SW                     (0)
#define NAND_ECC_HW                     (1)

#define NFI_MAX_FDM_SIZE                (8)
#define NFI_MAX_FDM_SEC_NUM             (8)
#define NFI_MAX_LOCK_CHANNEL            (16)

#define ECC_MAX_CORRECTABLE_BITS        (12)
#define ECC_MAX_PARITY_SIZE             (20)    /* in bytes */

#define ECC_ERR_LOCATION_MASK           (0x1FFF)
#define ECC_ERR_LOCATION_SHIFT          (16)

#define NAND_FFBUF_SIZE                 (2048 + 64)

/*=======================================================================*/
/* AOC2.0 & HRE init flow                                                */
/*=======================================================================*/
#define NTH_EMICFG_AO_MEM_REG_BASE    (IO_PHYS + 0x00270000)
#define STH_EMICFG_AO_MEM_REG_BASE    (IO_PHYS + 0x0030E000)
#define VLP_AO_BCRM_BASE              (IO_PHYS + 0x0C017000)
#define DEVAPC_VLP_AO_BASE            (IO_PHYS + 0x0C018000)
#define SSPM_BASE                     (IO_PHYS + 0x0C300000)
#define MMINFRA_HRE_ADDR_BASE         (IO_PHYS + 0x0E900000)
#define MDPSYS_HRE_ADDR_BASE          (IO_PHYS + 0x0F017000)
#define MDPSYS1_HRE_ADDR_BASE         (IO_PHYS + 0x0F817000)

/*=======================================================================*/
/* SW Reset Vector                                                       */
/*=======================================================================*/
/* setup the reset vector base address after warm reset to Aarch64 */
#define RVBADDRESS_CPU0     (MCUSYS_CFGREG_BASE + 0xC900)

/* IRQ */
#define SYS_TIMER_IRQ       (312 + 32)
#endif
