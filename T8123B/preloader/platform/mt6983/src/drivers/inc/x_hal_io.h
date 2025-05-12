/*----------------------------------------------------------------------------*
 * Copyright Statement:                                                       *
 *                                                                            *
 *   This software/firmware and related documentation ("MediaTek Software")   *
 * are protected under international and related jurisdictions'copyright laws *
 * as unpublished works. The information contained herein is confidential and *
 * proprietary to MediaTek Inc. Without the prior written permission of       *
 * MediaTek Inc., any reproduction, modification, use or disclosure of        *
 * MediaTek Software, and information contained herein, in whole or in part,  *
 * shall be strictly prohibited.                                              *
 * MediaTek Inc. Copyright (C) 2010. All rights reserved.                     *
 *                                                                            *
 *   BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND     *
 * AGREES TO THE FOLLOWING:                                                   *
 *                                                                            *
 *   1)Any and all intellectual property rights (including without            *
 * limitation, patent, copyright, and trade secrets) in and to this           *
 * Software/firmware and related documentation ("MediaTek Software") shall    *
 * remain the exclusive property of MediaTek Inc. Any and all intellectual    *
 * property rights (including without limitation, patent, copyright, and      *
 * trade secrets) in and to any modifications and derivatives to MediaTek     *
 * Software, whoever made, shall also remain the exclusive property of        *
 * MediaTek Inc.  Nothing herein shall be construed as any transfer of any    *
 * title to any intellectual property right in MediaTek Software to Receiver. *
 *                                                                            *
 *   2)This MediaTek Software Receiver received from MediaTek Inc. and/or its *
 * representatives is provided to Receiver on an "AS IS" basis only.          *
 * MediaTek Inc. expressly disclaims all warranties, expressed or implied,    *
 * including but not limited to any implied warranties of merchantability,    *
 * non-infringement and fitness for a particular purpose and any warranties   *
 * arising out of course of performance, course of dealing or usage of trade. *
 * MediaTek Inc. does not provide any warranty whatsoever with respect to the *
 * software of any third party which may be used by, incorporated in, or      *
 * supplied with the MediaTek Software, and Receiver agrees to look only to   *
 * such third parties for any warranty claim relating thereto.  Receiver      *
 * expressly acknowledges that it is Receiver's sole responsibility to obtain *
 * from any third party all proper licenses contained in or delivered with    *
 * MediaTek Software.  MediaTek is not responsible for any MediaTek Software  *
 * releases made to Receiver's specifications or to conform to a particular   *
 * standard or open forum.                                                    *
 *                                                                            *
 *   3)Receiver further acknowledge that Receiver may, either presently       *
 * and/or in the future, instruct MediaTek Inc. to assist it in the           *
 * development and the implementation, in accordance with Receiver's designs, *
 * of certain softwares relating to Receiver's product(s) (the "Services").   *
 * Except as may be otherwise agreed to in writing, no warranties of any      *
 * kind, whether express or implied, are given by MediaTek Inc. with respect  *
 * to the Services provided, and the Services are provided on an "AS IS"      *
 * basis. Receiver further acknowledges that the Services may contain errors  *
 * that testing is important and it is solely responsible for fully testing   *
 * the Services and/or derivatives thereof before they are used, sublicensed  *
 * or distributed. Should there be any third party action brought against     *
 * MediaTek Inc. arising out of or relating to the Services, Receiver agree   *
 * to fully indemnify and hold MediaTek Inc. harmless.  If the parties        *
 * mutually agree to enter into or continue a business relationship or other  *
 * arrangement, the terms and conditions set forth herein shall remain        *
 * effective and, unless explicitly stated otherwise, shall prevail in the    *
 * event of a conflict in the terms in any agreements entered into between    *
 * the parties.                                                               *
 *                                                                            *
 *   4)Receiver's sole and exclusive remedy and MediaTek Inc.'s entire and    *
 * cumulative liability with respect to MediaTek Software released hereunder  *
 * will be, at MediaTek Inc.'s sole discretion, to replace or revise the      *
 * MediaTek Software at issue.                                                *
 *                                                                            *
 *   5)The transaction contemplated hereunder shall be construed in           *
 * accordance with the laws of Singapore, excluding its conflict of laws      *
 * principles.  Any disputes, controversies or claims arising thereof and     *
 * related thereto shall be settled via arbitration in Singapore, under the   *
 * then current rules of the International Chamber of Commerce (ICC).  The    *
 * arbitration shall be conducted in English. The awards of the arbitration   *
 * shall be final and binding upon both parties and shall be entered and      *
 * enforceable in any court of competent jurisdiction.                        *
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 *
 * Description:
 *
 *---------------------------------------------------------------------------*/

#ifndef X_HAL_IO_H
#define X_HAL_IO_H

#include <stdarg.h>
#ifndef FPC_TEST
#include "dramc_pi_api.h"
#endif

typedef enum
{
    IO_META_CHANNEL = 0,
    IO_META_RANK,
    IO_META_CONF,
    IO_META_FAKE_ID,
    IO_META_NUMBER,
} IO_META_T;

/* Internal used by Hal */
extern void __meta_set(DRAMC_CTX_T *p, U8 meta_type, U8 value);
extern void __meta_restore(DRAMC_CTX_T *p, U8 meta_type);
extern void __meta_backup(DRAMC_CTX_T *p, U8 meta_type);
extern void __meta_backup_and_set(DRAMC_CTX_T *p, U8 meta_type, U8 value);
extern void __meta_advance(DRAMC_CTX_T *p, U8 meta_type);
extern U8 __meta_process_complete(DRAMC_CTX_T *p, U8 meta_type);
extern U8 __meta_loop_complete_for_next(DRAMC_CTX_T *p, U8 meta_type, U8 value);
extern U8 __meta_get(DRAMC_CTX_T *p, U8 meta_type);

/* For external use */
#define channel_set(_p, _channel) __meta_set(_p, IO_META_CHANNEL, _channel)
#define channel_backup(_p) __meta_backup(_p, IO_META_CHANNEL)
#define channel_restore(_p) __meta_restore(_p, IO_META_CHANNEL)
#define channel_backup_and_set(_p, _channel) __meta_backup_and_set(_p, IO_META_CHANNEL, _channel)
#define channel_advance(_p) __meta_advance(_p, IO_META_CHANNEL)
#define channel_process_complete(_p) __meta_process_complete(_p, IO_META_CHANNEL)
#define channel_get(_p) __meta_get(_p, IO_META_CHANNEL)

#define rank_set(_p, _rank) __meta_set(_p, IO_META_RANK, _rank)
#define rank_backup(_p) __meta_backup(_p, IO_META_RANK)
#define rank_restore(_p) __meta_restore(_p, IO_META_RANK)
#define rank_backup_and_set(_p, _rank) __meta_backup_and_set(_p, IO_META_RANK, _rank)
#define rank_advance(_p) __meta_advance(_p, IO_META_RANK)
#define rank_process_complete(_p) __meta_process_complete(_p, IO_META_RANK)
#define rank_loop_complete_for_next(_p, _rank) __meta_loop_complete_for_next(_p, IO_META_RANK, _rank)
#define rank_get(_p) __meta_get(_p, IO_META_RANK)

#define conf_set(_p, _conf) __meta_set(_p, IO_META_CONF, _conf)
#define conf_backup(_p) __meta_backup(_p, IO_META_CONF)
#define conf_restore(_p) __meta_restore(_p, IO_META_CONF)
#define conf_backup_and_set(_p, _conf) __meta_backup_and_set(_p, IO_META_CONF, _conf)
#define conf_advance(_p) __meta_advance(_p, IO_META_CONF)
#define conf_get(_p) __meta_get(_p, IO_META_CONF)

#define fake_id_set(_p, _fake_id) __meta_set(_p, IO_META_FAKE_ID, _fake_id);
#define fake_id_backup_and_set(p, _fake_id) __meta_backup_and_set(p, IO_META_FAKE_ID, _fake_id)
#define fake_id_restore(p) __meta_restore(p, IO_META_FAKE_ID)
#define fake_id_get(_p) __meta_get(_p, IO_META_FAKE_ID)

#define channel_rank_backup_and_set(_p, _channel, _rank) \
{ \
    channel_backup_and_set(_p, _channel); \
    rank_backup_and_set(_p, _rank); \
}

#define channel_rank_restore(_p) \
{ \
	channel_restore(_p); \
	rank_restore(_p); \
}

/* For Operating Partial Channels. i.e, Write CHA/C, or CHB/D */
#define CHANNEL_A_MSK (1 << 0) /* cha ID */
#define CHANNEL_B_MSK (1 << 1)
#define CHANNEL_C_MSK (1 << 2)
#define CHANNEL_D_MSK (1 << 3)
#define CHANNEL_MSK(__ch) (1 << (__ch))
#if (fcFOR_CHIP_ID == fcLepin)
#define MASTER_CH_MSK (CHANNEL_A_MSK | CHANNEL_D_MSK)
#define SLAVE_CH_MSK (CHANNEL_B_MSK | CHANNEL_C_MSK)
#else
#define MASTER_CH_MSK (CHANNEL_A_MSK | CHANNEL_C_MSK)
#define SLAVE_CH_MSK (CHANNEL_B_MSK | CHANNEL_D_MSK)
#endif
extern void channel_msk_set(DRAMC_CTX_T *p, U32 channel_msk);
extern void channel_msk_backup(DRAMC_CTX_T *p);
extern void channel_msk_restore(DRAMC_CTX_T *p);
extern void channel_msk_backup_and_set(DRAMC_CTX_T *p, U32 channel_bm);
extern void channel_msk_op_enable(void);
extern void channel_msk_op_disable(void);

/* AUTO generated by CODA tools. May need mofidy by PORJ */
#define PHY_BYTE_MAP_NUM 27

#define REG_ACCESS_NAO_DGB 0
#define REG_SHUFFLE_REG_CHECK 0

#define REG_ACCESS_DBG_EN   0  //0: disable,  1: enable custom rg dump,  2: init + calibration setting dump 

#if REG_ACCESS_DBG_EN
#include "dramc_int_global.h"
#define mcSHOW_REG_ACCESS_MSG(funcName, freq, onOff) \
{ \
    mcSHOW_DBG_MSG("\n[REG_ACCESS_PORTING_DBG_INFO][%s] FREQ=%d, %s \n", funcName , freq, onOff ? "begin" : "end"); \
    RegLogEnable =onOff; \
}
#else
#define mcSHOW_REG_ACCESS_MSG(funcName, freq, onOff) 
#endif


// Must match with Rinfo.rgtype, generated by CODA tools 
typedef enum
{
    REG_TYPE_DRAMC_AO = 0,
    REG_TYPE_DRAMC_NAO,
    REG_TYPE_PHY_AO,
    REG_TYPE_PHY_NAO,
#if ENABLE_DRAMOBF
    REG_TYPE_DRAMOBF,
#endif
    REG_TYPE_DPM, // 4
    REG_TYPE_SPM,
    REG_TYPE_SYSTEM,
#if __ETT__
    REG_TYPE_FKE,
    REG_TYPE_APBMIX,
#endif
    REG_TYPE_MAX
} REG_TYPE_T;

typedef enum
{
    REG_BYTE_NONE= 0,
	REG_BYTE_CA,
    REG_BYTE_B0,
    REG_BYTE_B1,
    REG_BYTE_B2
} REG_BYTE_T;

typedef enum
{
    REG_W_SWAP_NONE= 0,
	REG_W_SWAP_ALL,
    REG_W_SWAP_MIX,
} REG_WHOLE_SWAP_STATUS_T;


typedef enum
{
    REG_IS_SHU_N = 0,
	REG_IS_SHU_Y,
} REG_IS_SHUFFLE_T;

typedef enum
{
    REG_BY_RANK_NONE = 0,
	REG_BY_RANK, 
} REG_BY_RANK_T;

typedef enum
{
    FIELD_BYTE_SWAP_NONE = 0,
	FIELD_BYTE_SWAP_APHY,   // SW pinmux 
	FIELD_BYTE_SWAP_DPHY    // HW pinmux
} FIELD_BYTE_SWAP_TYPE_T;

typedef enum
{
    FIELD_SWAP_LTABLE_NONE = 0,
	FIELD_SWAP_LTABLE_NEED,   // Byte offset is not direct mapping, need to look up table. 
} FIELD_SWAP_LOOK_TABLE_T;

// field access macro-----------------------------------------------------------

/* register macros */
#define Rinfo(wswap, rank, byte, shu, rgtype, addr)  ((U32)((U32)(wswap) <<30 | (U32)(byte) <<27 | (U32)(rgtype)<<23 | (U32)(shu)<<21 | (U32)(rank) <<22 |(addr)))
#define RInfo_wswap(info)			((UINT8)((info >> 30) & 0x3))// 32bit whole reg swap status
#define RInfo_byte(info)			((UINT8)((info >> 27) & 0x7))
#define RInfo_rgtype(info)			((UINT8)((info >> 23) & 0xf))
#define RInfo_shu(info)				((UINT8)((info >> 21) & 0x1))
#define RInfo_rank(info)			((UINT8)((info >> 22) & 0x1))
#define RInfo_addr(info)			((U32)(info & 0xfffff))


/* field macros */
#define Fld(ltble, pbyt, pbit, wid, shft)   ((U32)(((ltble) << 24) | ((pbyt) << 18) | ((pbit) << 16) | ((wid) << 8) | ((shft))))
#define Fld_ltble(fld)		((UINT8)((fld) >> 24) & 0xff)
#define Fld_pbyt(fld)		((UINT8)((fld) >> 18) & 0x3)
#define Fld_pbit(fld)		((UINT8)((fld) >> 16) & 0x3)
#define Fld_wid(fld)		((UINT8)((fld) >> 8) & 0xff)
#define Fld_shft(fld)		((UINT8)((fld) & 0xff))
#define Fld_ac(fld)			((UINT8)(fld))

#define Fld2Msk32(fld)  /*lint -save -e504 */ (((U32)0xffffffff>>(32-Fld_wid(fld)))<<Fld_shft(fld)) /*lint -restore */
#define P_Fld(val, fld) ((sizeof(upk) > 1)? Fld2Msk32(fld): (((U32)(val) & ((U32)0xffffffff>>(32-Fld_wid(fld)))) << Fld_shft(fld)))
#define PB_Fld(val,fld)  val,fld
#define PB_FLD_END 0xfffffff


extern U32 u4Dram_Register_Read(DRAMC_CTX_T *p, U32 u4reg_addr);
extern void vDram_Register_Write(DRAMC_CTX_T *p, U32 u4VirtualAddr, U32 u4reg_value);
extern void vIO32WriteMsk(DRAMC_CTX_T *p, U32 reg32, U32 val32, U32 msk32);
extern void vIO32WriteMsk_All(DRAMC_CTX_T *p, U32 reg32, U32 val32, U32 msk32);
//extern void vIO32Write4B_All2(DRAMC_CTX_T *p, PTR_T reg32, U32 val32);

extern void vPhyByteWriteFldAlign(DRAMC_CTX_T *p, U32 reg32, U32 val32, U32 msk32, CHANNEL_RANK_SEL_T chType);
extern U32 vPhyByteReadFldAlign(DRAMC_CTX_T *p, U32 u4VirtualAddr, U32 msk32);

// only for multi && whole register is not swap mixed
extern void vPhyByteIO32WriteMsk(DRAMC_CTX_T *p, U32 reg32, U32 val32, U32 msk32);
extern void vPhyByteIO32WriteMsk_All(DRAMC_CTX_T *p, U32 reg32, U32 val32, U32 msk32);

// only for multi && whole register IS swap mixed
extern void vPhyByteWriteFldMulti2(DRAMC_CTX_T *p, U32 u4VirtualAddr, CHANNEL_RANK_SEL_T chType, U32 rglist, ...);
extern void vPhyByteWriteFldMulti2_CHs(DRAMC_CTX_T *p, U32 u4VirtualAddr, U32 rglist, ...);
extern void vPhyByteIO32WriteMsk_CHs(DRAMC_CTX_T *p, U32 u4VirtualAddr, U32 val32, U32 msk32);
extern void vIO32WriteMsk_CHs(DRAMC_CTX_T *p, U32 u4VirtualAddr, U32 val32, U32 msk32);
extern U8 u1PhyByteSwapOnOff(U8 u1OnOff);

#define FLD_FULL 0
#define BUILD_BUG_WRONG_REG_MULTI(condition)  ((U8)sizeof(char[1 - 2*!!(condition)]))
#define MIX_RG_CHECK(reg32) \
	((BUILD_BUG_WRONG_REG_MULTI(RInfo_wswap(reg32) == REG_W_SWAP_MIX) ? (reg32) : 0))

// =========================
// public Macro for general use. 
//==========================
#ifdef __DPM__
#define u4IO32Read4B(reg32) u4Dram_Register_Read(p, reg32)
#define vIO32Write4B(reg32, val32) vDram_Register_Write(p, reg32, val32)
#define vIO32Write4B_All(reg32, val32) vIO32Write4B_All2(p, reg32, val32)
#define vIO32Write4BMsk(reg32, val32, msk32) vIO32Write4BMsk2(p, reg32, val32, msk32)
#define vIO32Write4BMsk_All(reg32, val32, msk32) vIO32Write4BMsk_All2(p, reg32, val32, msk32)

#define u4IO32ReadFldAlign(reg32, fld) /*lint -save -e506 -e504 -e514 -e62 -e737 -e572 -e961 -e648 -e701 -e732 -e571 */ \
	(BUILD_BUG_WRONG_REG_MULTI(Fld_pbyt(fld) == FIELD_BYTE_SWAP_APHY) ? ((u4IO32Read4B(reg32) & Fld2Msk32(fld)) >> Fld_shft(fld)) : (U32)0)

#define vIO32WriteFldAlign(reg32, val, fld) /*lint -save -e506 -e504 -e514 -e62 -e737 -e572 -e961 -e648 -e701 -e732 -e571 */ \
	(BUILD_BUG_WRONG_REG_MULTI(Fld_pbyt(fld) == FIELD_BYTE_SWAP_APHY) ? (vIO32Write4BMsk((reg32), ((U32)(val) << Fld_shft(fld)), Fld2Msk32(fld))) : (void)0)

#define vIO32WriteFldMulti(reg32, list) /*lint -save -e506 -e504 -e514 -e62 -e737 -e572 -e961 -e648 -e701 -e732 -e571 */ \
{ \
	UINT16 upk; \
	INT32 msk = (INT32)(list); \
	{UINT8 upk; \
    BUILD_BUG_WRONG_REG_MULTI(RInfo_wswap(reg32) != REG_W_SWAP_NONE); \
    ((U32)msk == 0xffffffff)? (vIO32Write4B(reg32, (list))): (((U32)msk)? vIO32Write4BMsk(reg32, (list), ((U32)msk)):(U32)0); \
	} \
}/*lint -restore */

//=========================
// Public Macro for write all-dramC or all-PHY registers
//=========================
#define vIO32WriteFldAlign_All(reg32, val, fld) /*lint -save -e506 -e504 -e514 -e62 -e737 -e572 -e961 -e648 -e701 -e732 -e571 */ \
	(BUILD_BUG_WRONG_REG_MULTI(Fld_pbyt(fld) == FIELD_BYTE_SWAP_APHY) ? (vIO32Write4BMsk_All((reg32), ((U32)(val) << Fld_shft(fld)), Fld2Msk32(fld))) : (void)0)

#define vIO32WriteFldMulti_All(reg32, list) /*lint -save -e506 -e504 -e514 -e62 -e737 -e572 -e961 -e648 -e701 -e732 -e571 */ \
{ \
	UINT16 upk; \
	INT32 msk = (INT32)(list); \
	{UINT8 upk; \
     BUILD_BUG_WRONG_REG_MULTI(RInfo_wswap(reg32) != REG_W_SWAP_NONE); \
    ((U32)msk == 0xffffffff)? (vIO32Write4B_All(reg32, (list))): (((U32)msk)? vIO32Write4BMsk_All(reg32, (list), ((U32)msk)): (void)0); \
	} \
}/*lint -restore */
#else
#define u4IO32Read4B(reg32) \
    (BUILD_BUG_WRONG_REG_MULTI(RInfo_wswap(reg32) == REG_W_SWAP_MIX) ? vPhyByteReadFldAlign(p, reg32, FLD_FULL) : 0)
#define vIO32Write4B(reg32, val32) \
    (BUILD_BUG_WRONG_REG_MULTI(RInfo_wswap(reg32) == REG_W_SWAP_MIX) ? vPhyByteWriteFldAlign(p, reg32, val32, FLD_FULL, TO_ONE_CHANNEL) : (void)0)
#define vIO32Write4B_All(reg32, val32) \
    (BUILD_BUG_WRONG_REG_MULTI(RInfo_wswap(reg32) == REG_W_SWAP_MIX) ? vPhyByteWriteFldAlign(p, reg32, val32, FLD_FULL, TO_ALL_CHANNEL) : (void)0)

#define u4IO32ReadFldAlign(reg32, fld) \
    ((Fld_pbyt(fld) != FIELD_BYTE_SWAP_APHY)? (((u4Dram_Register_Read(p, reg32)) & Fld2Msk32(fld)) >> Fld_shft(fld)): (vPhyByteReadFldAlign(p, reg32, fld)))

#define vIO32WriteFldAlign(reg32, val, fld) \
    ((Fld_pbyt(fld) != FIELD_BYTE_SWAP_APHY)? (vIO32WriteMsk(p, reg32, ((U32)(val) << Fld_shft(fld)), Fld2Msk32(fld))): (vPhyByteWriteFldAlign(p, reg32, val, fld, TO_ONE_CHANNEL)))

#define vIO32WriteFldAlign_All(reg32, val, fld)  \
    ((Fld_pbyt(fld) != FIELD_BYTE_SWAP_APHY)? (vIO32WriteMsk_All(p, reg32, ((U32)(val) << Fld_shft(fld)), Fld2Msk32(fld))): (vPhyByteWriteFldAlign(p, reg32, val, fld, TO_ALL_CHANNEL)))


#define vIO32WriteFldMulti(reg32, list)  \
{ \
	UINT16 upk; \
	INT32 msk = (INT32)(list); \
	{UINT8 upk; \
    BUILD_BUG_WRONG_REG_MULTI(RInfo_wswap(reg32) == REG_W_SWAP_MIX); \
    ((U32)msk == 0xffffffff)? (vIO32Write4B(reg32, (list))): (((U32)msk)? vPhyByteIO32WriteMsk(p, reg32, (list), ((U32)msk)): (void)0); \
	} \
}

#define vIO32WriteFldMulti_All(reg32, list) \
{ \
	UINT16 upk; \
	INT32 msk = (INT32)(list); \
	{UINT8 upk; \
    BUILD_BUG_WRONG_REG_MULTI(RInfo_wswap(reg32) == REG_W_SWAP_MIX); \
    ((U32)msk == 0xffffffff)? (vIO32Write4B_All(reg32, (list))): (((U32)msk)? vPhyByteIO32WriteMsk_All(p, reg32, (list), ((U32)msk)): (void)0); \
	} \
}
#endif
#define vPhyByteWriteFldMulti(reg32, rglist, ...)	\
{\
    BUILD_BUG_WRONG_REG_MULTI(RInfo_wswap(reg32) != REG_W_SWAP_MIX); \
    vPhyByteWriteFldMulti2(p,reg32,TO_ONE_CHANNEL,rglist, __VA_ARGS__, PB_FLD_END); \
}  

#define vPhyByteWriteFldMulti_All(reg32, rglist, ...) 	\
{\
    BUILD_BUG_WRONG_REG_MULTI(RInfo_wswap(reg32) != REG_W_SWAP_MIX); \
    vPhyByteWriteFldMulti2(p,reg32,TO_ALL_CHANNEL,rglist, __VA_ARGS__, PB_FLD_END); \
}

/* --------------------------------------------------------------------
 * ------------------ Parital CHANNEL operatings ----------------------
 * --------------------------------------------------------------------
 */
#define vIO32WriteFldAlign_CHs(reg32, val32, fld) \
{ \
    channel_msk_op_enable(); \
    vIO32WriteFldAlign_All(reg32, val32, fld); \
    channel_msk_op_disable(); \
}

#define vIO32WriteFldMulti_CHs(reg32, list) \
{ \
    channel_msk_op_enable(); \
    vIO32WriteFldMulti_All(reg32, list); \
    channel_msk_op_disable(); \
}

#define vPhyByteWriteFldAlign_CHs(reg32, val32, msk32) \
{ \
    channel_msk_op_enable(); \
    vPhyByteWriteFldAlign(p, reg32, val32, msk32, TO_ALL_CHANNEL); \
    channel_msk_op_disable(); \
}

#define vPhyByteWriteFldMulti_CHs(reg32, rglist, ...) \
{ \
    channel_msk_op_enable(); \
    BUILD_BUG_WRONG_REG_MULTI(RInfo_wswap(reg32) != REG_W_SWAP_MIX); \
    vPhyByteWriteFldMulti2(p, reg32, TO_ALL_CHANNEL, rglist, __VA_ARGS__, PB_FLD_END); \
    channel_msk_op_disable(); \
}
/* ------------------ End of Parital CHANNEL operatings ---------------------- */

/* For Byte shift operation */
extern PTR_T DramcVirt2Phys(DRAMC_CTX_T *p, U32 u4Virt);

#ifdef __DPM__
#include "x_hal_io_dpm.h"
#endif

#endif  // X_HAL_IO_H
