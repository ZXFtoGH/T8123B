/** @file hal_io.cpp
 *  hal_io.cpp provides functions of register access
 */

#ifndef FPC_TEST
#include "dramc_common.h"
#include "dramc_int_global.h"
#else
#include "FT_Common_functions.h"
#endif

#include "x_hal_io.h"
#include "dramc_register.h"

#if __ETT__
#include <barriers.h>
#endif

#define DUMP_IO_WRITE 0

#ifdef DUMP_INIT_RG_LOG_TO_DE
    U8  gDUMP_INIT_RG_LOG_TO_DE_RG_log_flag = 0;
#endif

#if REG_ACCESS_DBG_EN
    U8 RegLogEnable = 0;
    U8 gu1IsRankRG=0;
    U16 gu2RankOfst=0;
#endif

#define META_RECURSIVE_DEPTH    5
#define META_DBG_ENABLE         0

/* Manage factors that may affect IO behavior, addr_translate & byte_swap, for example. */
struct dramc_io_manange {
    /* Meta data used when translating address */
    U8 meta_backup[META_RECURSIVE_DEPTH][IO_META_NUMBER];
    U8 meta_curr[META_RECURSIVE_DEPTH][IO_META_NUMBER];
    U8 meta_reference[META_RECURSIVE_DEPTH][IO_META_NUMBER];

    U8 meta_recursive_depth[IO_META_NUMBER];

#if META_DBG_ENABLE
    U32 count_backup[IO_META_NUMBER];
    U32 count_restore[IO_META_NUMBER];
#endif

    /* Bitmap channel setting */
    U32 channel_msk;
    U8 channel_msk_bakcup;
    U8 channel_msk_reference;
    U8 enable_channel_msk_operation;

    U8 u1ByteSwapEnable;
};

static struct dramc_io_manange dramc_io_manager = {
    .meta_curr = { 0 },
    .meta_reference = { 0 },
    .meta_recursive_depth = { 0 },

#if META_DBG_ENABLE
    .count_backup = { 0 },
    .count_restore = { 0 },
#endif

    .enable_channel_msk_operation = FALSE,
    .channel_msk_reference = 0,
    .u1ByteSwapEnable = ON,
};

#if META_DBG_ENABLE
static char *str_meta_type[IO_META_NUMBER] = {
    "IO_META_CHANNEL",
    "IO_META_RANK",
    "IO_META_CONF",
    "IO_META_FAKE_ID",
};
#endif

#define DEPTH(_type) dramc_io_manager.meta_recursive_depth[_type]

void __meta_set(DRAMC_CTX_T *p, U8 meta_type, U8 value)
{
    dramc_io_manager.meta_curr[DEPTH(meta_type)][meta_type] = value;

    /* Tmp: update to DRAMC_CTX_T */
    if (meta_type == IO_META_CHANNEL)
        vSetPHY2ChannelMapping(p, value);
    else if (meta_type == IO_META_RANK)
        vSetRank(p, value);
    else if (meta_type == IO_META_CONF)
        p->ShuRGAccessIdx = value;
}

void __meta_restore(DRAMC_CTX_T *p, U8 meta_type)
{
    ASSERT(dramc_io_manager.meta_reference[DEPTH(meta_type)][meta_type] == 1);
    __meta_set(p, meta_type, dramc_io_manager.meta_backup[DEPTH(meta_type)][meta_type]);
    dramc_io_manager.meta_reference[DEPTH(meta_type)][meta_type]--;

#if META_DBG_ENABLE
    dramc_io_manager.count_restore[meta_type]++;
    mcSHOW_DBG_MSG("META_DBG[RESTORE]: %s, depth: %d, restore_count: %d\n",
        str_meta_type[meta_type], DEPTH(meta_type), dramc_io_manager.count_restore[meta_type]);
#endif

    if (DEPTH(meta_type) > 0)
        dramc_io_manager.meta_recursive_depth[meta_type]--;
}

void __meta_backup(DRAMC_CTX_T *p, U8 meta_type)
{
    if (dramc_io_manager.meta_reference[DEPTH(meta_type)][meta_type] == 1) {
        if (DEPTH(meta_type) < META_RECURSIVE_DEPTH) {
        #if __ETT__
            mcSHOW_ERR_MSG("%s: [WARNING] recursive occur!!\n");
        #endif
            dramc_io_manager.meta_recursive_depth[meta_type]++;
        } else {
            ASSERT(0);
        }
    }
    
    /* Tmp: update to DRAMC_CTX_T */
    if (meta_type == IO_META_CHANNEL)
        dramc_io_manager.meta_curr[DEPTH(meta_type)][meta_type] = vGetPHY2ChannelMapping(p);
    else if (meta_type == IO_META_RANK)
        dramc_io_manager.meta_curr[DEPTH(meta_type)][meta_type] = u1GetRank(p);
    else if (meta_type == IO_META_CONF)
        dramc_io_manager.meta_curr[DEPTH(meta_type)][meta_type] = p->ShuRGAccessIdx;

    dramc_io_manager.meta_backup[DEPTH(meta_type)][meta_type] = dramc_io_manager.meta_curr[DEPTH(meta_type)][meta_type];
    dramc_io_manager.meta_reference[DEPTH(meta_type)][meta_type]++;

#if META_DBG_ENABLE
    dramc_io_manager.count_backup[meta_type]++;
    mcSHOW_DBG_MSG("META_DBG[BACKUP]: %s, depth: %d, backup_count: %d\n",
        str_meta_type[meta_type], DEPTH(meta_type), dramc_io_manager.count_backup[meta_type]);
#endif
}

void __meta_backup_and_set(DRAMC_CTX_T *p, U8 meta_type, U8 value)
{
    __meta_backup(p, meta_type);
    __meta_set(p, meta_type, value);
}

void __meta_advance(DRAMC_CTX_T *p, U8 meta_type)
{
    dramc_io_manager.meta_curr[DEPTH(meta_type)][meta_type]++;
    __meta_set(p, meta_type, dramc_io_manager.meta_curr[DEPTH(meta_type)][meta_type]);
}

U8 __meta_process_complete(DRAMC_CTX_T *p, U8 meta_type)
{
    U8 support_num;

    support_num = (meta_type == IO_META_CHANNEL) ? p->support_channel_num : p->support_rank_num;

    return (dramc_io_manager.meta_curr[DEPTH(meta_type)][meta_type] >= support_num);
}

U8 __meta_loop_complete_for_next(DRAMC_CTX_T *p, U8 meta_type, U8 value)
{
    U8 ret;

    ret = __meta_process_complete(p, meta_type);
    if (ret)
        __meta_set(p, meta_type, value);

    return ret;
}

U8 __meta_get(DRAMC_CTX_T *p, U8 meta_type)
{
    /* Tmp: update to DRAMC_CTX_T */
    if (meta_type == IO_META_CHANNEL)
        dramc_io_manager.meta_curr[DEPTH(meta_type)][meta_type] = vGetPHY2ChannelMapping(p);
    else if (meta_type == IO_META_RANK)
        dramc_io_manager.meta_curr[DEPTH(meta_type)][meta_type] = u1GetRank(p);
    else if (meta_type == IO_META_CONF)
        dramc_io_manager.meta_curr[DEPTH(meta_type)][meta_type] = p->ShuRGAccessIdx;

    return dramc_io_manager.meta_curr[DEPTH(meta_type)][meta_type];
}

void channel_msk_set(DRAMC_CTX_T *p, U32 channel_msk)
{
    dramc_io_manager.channel_msk = channel_msk;
}

void channel_msk_backup(DRAMC_CTX_T *p)
{
    ASSERT(dramc_io_manager.channel_msk_reference == 0);
    dramc_io_manager.channel_msk_bakcup = dramc_io_manager.channel_msk;
    dramc_io_manager.channel_msk_reference++;
}

void channel_msk_restore(DRAMC_CTX_T *p)
{
    ASSERT(dramc_io_manager.channel_msk_reference == 1);
    dramc_io_manager.channel_msk = dramc_io_manager.channel_msk_bakcup;
    dramc_io_manager.channel_msk_reference--;
}

void channel_msk_backup_and_set(DRAMC_CTX_T *p, U32 channel_msk)
{
    channel_msk_backup(p);
    channel_msk_set(p, channel_msk);
}

void channel_msk_op_enable(void)
{
    dramc_io_manager.enable_channel_msk_operation = TRUE;
}

void channel_msk_op_disable(void)
{
    dramc_io_manager.enable_channel_msk_operation = FALSE;
}


#if FOR_DV_SIMULATION_USED
#define IO_PHYS       0x00000000
#else
#ifndef IO_PHYS
#define IO_PHYS       0x10000000
#endif
#endif
// === 4 channels: physical base address =======
#if (FOR_DV_SIMULATION_USED)
const PTR_T REG_BASE_ADDR_LIST[REG_TYPE_MAX][4] ={
    [REG_TYPE_DRAMC_AO] =  {0x00010000, 0x00040000, 0x00000000, 0x00000000},  // DRAM AO
    [REG_TYPE_DRAMC_NAO] = {0x00020000, 0x00050000, 0x00000000, 0x00000000},   // DRAMC NAO
    [REG_TYPE_PHY_AO] =    {0x00030000, 0x00060000, 0x00000000, 0x00000000},   // PHY AO
    [REG_TYPE_PHY_NAO] =   {0x00070000, 0x00080000, 0x00000000, 0x00000000},   // PHY NAO
    [REG_TYPE_DPM] =       {0x00100000, 0x00200000, 0x00000000, 0x00000000},   // DPM
};
#else  // real chip
// HW Physical base address
const PTR_T REG_BASE_ADDR_LIST[REG_TYPE_MAX][4] ={
    [REG_TYPE_DRAMC_AO] =  {0x00230000, 0x00240000, 0x00250000, 0x00260000},  // DRAM AO
    [REG_TYPE_DRAMC_NAO] = {0x00234000, 0x00244000, 0x00254000, 0x00264000},   // DRAMC NAO
    [REG_TYPE_PHY_AO] =    {0x00238000, 0x00248000, 0x00258000, 0x00268000},   // PHY AO
    [REG_TYPE_PHY_NAO] =   {0x00236000, 0x00246000, 0x00256000, 0x00266000},   // PHY NAO
    [REG_TYPE_DPM] =       {0x00900000, 0x00A00000, 0x00B00000, 0x00C00000},   // DPM
#if (fcFOR_CHIP_ID == fcMT8139)
    [REG_TYPE_SPM] =       {0x00006000, 0x00000000, 0x00000000, 0x00000000},
#elif (fcFOR_CHIP_ID == fcFigeac || fcFOR_CHIP_ID == fcLepin)
    [REG_TYPE_SPM] =       {0x0C001000, 0x00000000, 0x00000000, 0x00000000},
#endif
#if __ETT__
#if (fcFOR_CHIP_ID == fcLepin)
    [REG_TYPE_FKE] =       {0x0026C000, 0x0026D000, 0x00310000, 0x00311000},
#else
    [REG_TYPE_FKE] =       {0x0026D000, 0x0026C000, 0x00000000, 0x00000000},
#endif
    [REG_TYPE_APBMIX] =    {0x0000C000, 0x00000000, 0x00000000, 0x00000000},
#endif
#if ENABLE_DRAMOBF
    [REG_TYPE_DRAMOBF] =  {0x00232000, 0x00242000, 0x00252000, 0x00262000},  // OBF
#endif
};
#endif

//AUTO GEN by Coda. Move here to avoid multi-definition error when linking.
U32 arByteMapTable[][2]= {
	{ /* DDRPHY_REG_CA_CMD2,          CA_CMD2_RG_TX_ARCLK_OE_TIE_SEL_CA,         */ DDRPHY_REG_SHU_B1_DQ13,           SHU_B1_DQ13_RG_TX_ARDQS_OE_TIE_SEL_B1 },
	{ /* DDRPHY_REG_CA_CMD2,          CA_CMD2_RG_TX_ARCLK_OE_TIE_EN_CA,          */ DDRPHY_REG_SHU_B1_DQ13,           SHU_B1_DQ13_RG_TX_ARDQS_OE_TIE_EN_B1 },
	{ /* DDRPHY_REG_CA_CMD7,          CA_CMD7_RG_TX_ARCS1_PULL_DN,               */ DDRPHY_REG_B1_DQ7,                B1_DQ7_RG_TX_ARDQP0_PULL_DN_B1 },
	{ /* DDRPHY_REG_CA_CMD7,          CA_CMD7_RG_TX_ARCS1_PULL_UP,               */ DDRPHY_REG_B1_DQ7,                B1_DQ7_RG_TX_ARDQP0_PULL_UP_B1 },
	{ /* DDRPHY_REG_CA_CMD8,          CA_CMD8_RG_RX_ARCLK_SER_RST_MODE,          */ DDRPHY_REG_B1_DQ8,                B1_DQ8_RG_RX_ARDQS_SER_RST_MODE_B1 },
	{ /* DDRPHY_REG_B1_DQ7,           B1_DQ7_RG_TX_ARDQP0_PULL_DN_B1,            */ DDRPHY_REG_CA_CMD7,               CA_CMD7_RG_TX_ARCS1_PULL_DN },
	{ /* DDRPHY_REG_B1_DQ7,           B1_DQ7_RG_TX_ARDQP0_PULL_UP_B1,            */ DDRPHY_REG_CA_CMD7,               CA_CMD7_RG_TX_ARCS1_PULL_UP },
	{ /* DDRPHY_REG_B1_DQ8,           B1_DQ8_RG_RX_ARDQS_SER_RST_MODE_B1,        */ DDRPHY_REG_CA_CMD8,               CA_CMD8_RG_RX_ARCLK_SER_RST_MODE },
	{ /* DDRPHY_REG_SHU_CA_DLL_ARPI3, SHU_CA_DLL_ARPI3_RG_ARPI_CS_EN,            */ DDRPHY_REG_SHU_B1_DLL_ARPI3,      SHU_B1_DLL_ARPI3_RG_ARPI_DQM_EN_B1 },
	{ /* DDRPHY_REG_SHU_CA_CMD13,     SHU_CA_CMD13_RG_TX_ARCS1_MCKIO_SEL_CA,     */ DDRPHY_REG_SHU_B1_DQ15,           SHU_B1_DQ15_RG_TX_ARDQP_MCKIO_SEL_B1 },
	{ /* DDRPHY_REG_SHU_CA_CMD13,     SHU_CA_CMD13_RG_TX_ARCS_MCKIO_SEL_CA,      */ DDRPHY_REG_SHU_B1_DQ15,           SHU_B1_DQ15_RG_TX_ARDQM_MCKIO_SEL_B1 },
	{ /* DDRPHY_REG_SHU_CA_CMD16,     SHU_CA_CMD16_RG_TX_ARWCK_PRE_EN_C0,        */ DDRPHY_REG_SHU_B1_DQ14,           SHU_B1_DQ14_RG_TX_ARWCK_PRE_EN_B1 },
	{ /* DDRPHY_REG_SHU_CA_CMD16,     SHU_CA_CMD16_RG_TX_ARWCK_PRE_DATA_SEL_C0,  */ DDRPHY_REG_SHU_B1_DQ14,           SHU_B1_DQ14_RG_TX_ARWCK_PRE_DATA_SEL_B1 },
	{ /* DDRPHY_REG_SHU_CA_CMD16,     SHU_CA_CMD16_RG_TX_ARWCK_OE_TIE_SEL_C0,    */ DDRPHY_REG_SHU_B1_DQ13,           SHU_B1_DQ13_RG_TX_ARWCK_OE_TIE_SEL_B1 },
	{ /* DDRPHY_REG_SHU_CA_CMD16,     SHU_CA_CMD16_RG_TX_ARWCK_OE_TIE_EN_C0,     */ DDRPHY_REG_SHU_B1_DQ13,           SHU_B1_DQ13_RG_TX_ARWCK_OE_TIE_EN_B1 },
	{ /* DDRPHY_REG_SHU_CA_CMD16,     SHU_CA_CMD16_RG_TX_ARWCKB_OE_TIE_SEL_C0,   */ DDRPHY_REG_SHU_B1_DQ13,           SHU_B1_DQ13_RG_TX_ARWCKB_OE_TIE_SEL_B1 },
	{ /* DDRPHY_REG_SHU_CA_CMD16,     SHU_CA_CMD16_RG_TX_ARWCKB_OE_TIE_EN_C0,    */ DDRPHY_REG_SHU_B1_DQ13,           SHU_B1_DQ13_RG_TX_ARWCKB_OE_TIE_EN_B1 },
	{ /* DDRPHY_REG_SHU_CA_CMD17,     SHU_CA_CMD17_RG_RX_ARWCK_EXT_LPBK_EN_C0,   */ DDRPHY_REG_SHU_B1_DQ17,           SHU_B1_DQ17_RG_RX_ARWCK_EXT_LPBK_EN_B1 },
	{ /* DDRPHY_REG_SHU_B1_DQ13,      SHU_B1_DQ13_RG_TX_ARDQS_OE_TIE_SEL_B1,     */ DDRPHY_REG_CA_CMD2,               CA_CMD2_RG_TX_ARCLK_OE_TIE_SEL_CA },
	{ /* DDRPHY_REG_SHU_B1_DQ13,      SHU_B1_DQ13_RG_TX_ARDQS_OE_TIE_EN_B1,      */ DDRPHY_REG_CA_CMD2,               CA_CMD2_RG_TX_ARCLK_OE_TIE_EN_CA },
	{ /* DDRPHY_REG_SHU_B1_DQ13,      SHU_B1_DQ13_RG_TX_ARWCKB_OE_TIE_SEL_B1,    */ DDRPHY_REG_SHU_CA_CMD16,          SHU_CA_CMD16_RG_TX_ARWCKB_OE_TIE_SEL_C0 },
	{ /* DDRPHY_REG_SHU_B1_DQ13,      SHU_B1_DQ13_RG_TX_ARWCKB_OE_TIE_EN_B1,     */ DDRPHY_REG_SHU_CA_CMD16,          SHU_CA_CMD16_RG_TX_ARWCKB_OE_TIE_EN_C0 },
	{ /* DDRPHY_REG_SHU_B1_DQ13,      SHU_B1_DQ13_RG_TX_ARWCK_OE_TIE_SEL_B1,     */ DDRPHY_REG_SHU_CA_CMD16,          SHU_CA_CMD16_RG_TX_ARWCK_OE_TIE_SEL_C0 },
	{ /* DDRPHY_REG_SHU_B1_DQ13,      SHU_B1_DQ13_RG_TX_ARWCK_OE_TIE_EN_B1,      */ DDRPHY_REG_SHU_CA_CMD16,          SHU_CA_CMD16_RG_TX_ARWCK_OE_TIE_EN_C0 },
	{ /* DDRPHY_REG_SHU_B1_DQ14,      SHU_B1_DQ14_RG_TX_ARWCK_PRE_EN_B1,         */ DDRPHY_REG_SHU_CA_CMD16,          SHU_CA_CMD16_RG_TX_ARWCK_PRE_EN_C0 },
	{ /* DDRPHY_REG_SHU_B1_DQ14,      SHU_B1_DQ14_RG_TX_ARWCK_PRE_DATA_SEL_B1,   */ DDRPHY_REG_SHU_CA_CMD16,          SHU_CA_CMD16_RG_TX_ARWCK_PRE_DATA_SEL_C0 },
	{ /* DDRPHY_REG_SHU_B1_DQ17,      SHU_B1_DQ17_RG_RX_ARWCK_EXT_LPBK_EN_B1,    */ DDRPHY_REG_SHU_CA_CMD17,          SHU_CA_CMD17_RG_RX_ARWCK_EXT_LPBK_EN_C0 },
};



/* NEW API */
//EXTERN bool gDPMst_flag;

static U32 u4ByteSwapOfst(U32 u4Vaddr, PHY_BYTE_SWAP_RULE_T swapRule)
{
    U32 u4VaddrSwap = u4Vaddr;
    U8 u1ByteType = (U8)RInfo_byte(u4Vaddr);

    if(swapRule ==  PHY_SWAP_RULE_CA_B1)
    {
        if(u1ByteType == REG_BYTE_CA)
        {
            u4VaddrSwap += (DDRPHY_AO_BYTE_OFFSET<<1); // CA -> B1
        }
        else if(u1ByteType == REG_BYTE_B1)            
        {
            u4VaddrSwap -= (DDRPHY_AO_BYTE_OFFSET<<1); // B1 -> CA
        }
    }
    else if(swapRule ==  PHY_SWAP_RULE_B0_B1)
    {
        if(u1ByteType == REG_BYTE_B1)            
        {
            u4VaddrSwap -= DDRPHY_AO_BYTE_OFFSET; // B1 -> B0
        } 
        else if (u1ByteType == REG_BYTE_B0)
        {
            u4VaddrSwap += DDRPHY_AO_BYTE_OFFSET; // B0 -> B1
        }
    }

    mcSHOW_DBG_MSG5("[%s] 0x%X -> 0x%X\n", __func__, u4Vaddr, u4VaddrSwap);

    return u4VaddrSwap;
}

U8 u1PhyByteSwapOnOff(U8 u1OnOff)
{
    dramc_io_manager.u1ByteSwapEnable = u1OnOff;

	return 0;
}

U8 u4PhyAoByteSwap(U32 *u4VirtualAddr, U32 *fldInfo, PHY_BYTE_SWAP_RULE_T swapRule)
{
    PTR_T u4Vaddr = *u4VirtualAddr;
    U32 u4Fld;
    U8 u1SwapType,ltable;
    U8 u1Ret = 0;

    if(dramc_io_manager.u1ByteSwapEnable == OFF)
        return DRAM_OK;

    if(swapRule == PHY_SWAP_RULE_NONE)
        return DRAM_OK; 

    if(fldInfo==NULL)  //Read/write 4B, no field information
    {
        u1SwapType = RInfo_wswap(u4Vaddr);
        if(u1SwapType ==REG_W_SWAP_ALL)
        {
            u4Vaddr = u4ByteSwapOfst(u4Vaddr, swapRule);
            u1Ret =1;
            
            mcSHOW_DBG_MSG5("[%s] APHY reg swap(%d) Addr(0x%X => 0x%X)\n",
                            __func__, u1Ret, *u4VirtualAddr, u4Vaddr);

            *u4VirtualAddr = u4Vaddr;
            
        }
        #if __ETT__
        else if(u1SwapType ==REG_W_SWAP_MIX)
        {
            mcSHOW_ERR_MSG("[%s] REG_W_SWAP_MIX error\n", __func__);
            while(1);
        }
        #endif
    }
    else
    {
        u4Fld = *fldInfo;
        u1SwapType = (U8)Fld_pbyt(u4Fld);
        
        // APHY need CA <->B1 SW swap for DSC dram
        if(u1SwapType == FIELD_BYTE_SWAP_APHY)  
        {
            if(swapRule == PHY_SWAP_RULE_CA_B1) /* @cc: lookup table is only for CA<->B1 SWAP CASE */
            {
                ltable = (U8)Fld_ltble(u4Fld);

                if(ltable== FIELD_SWAP_LTABLE_NONE)  // direct byte swap mapping
                {
                    u4Vaddr = u4ByteSwapOfst(u4Vaddr, swapRule);
                    u1Ret =2;
                }
                else if(ltable < PHY_BYTE_MAP_NUM)//FIELD_SWAP_LTABLE_NEED, cannot use direct mapping
                {
                    ltable --;
                    u4Vaddr = (u4Vaddr & 0xfff00000) | (arByteMapTable[ltable][0]);
                    u4Fld = arByteMapTable[ltable][1];
                    u1Ret =3;
                }
                else
                {  
                    #if __ETT__
                    mcSHOW_ERR_MSG("[%s] APHY reg map table miss! 0x%X\n", __func__, u4Vaddr);
                    while(1);
                    #endif
                    u1Ret =4;
                }
            }
            else{
                /* B0<->B1 swap. Dont care ltable, directly remapping if is APHY RG */
                u4Vaddr = u4ByteSwapOfst(u4Vaddr, swapRule);
                u1Ret = 5;
            }
        }

        if(u1Ret>0)
        {
            mcSHOW_DBG_MSG5("[%s] APHY reg swap(%d) Addr(0x%X => 0x%X) Msk(0x%X => 0x%X)\n",
                        __func__, u1Ret, *u4VirtualAddr, u4Vaddr, *fldInfo, u4Fld);
        }
        
        *u4VirtualAddr = u4Vaddr;
        *fldInfo = u4Fld;
    }

    return DRAM_OK;
}

// Handle channel/ shuffle/ rank auto offset
PTR_T u4RegBaseAddrTranslate(DRAMC_CTX_T *p, U8 eCh, U32 u4VirtualAddr)
{
	PTR_T u4Offset = RInfo_addr(u4VirtualAddr);
	U32 u4RegType = RInfo_rgtype(u4VirtualAddr);
	U32 u4RegIsShu = RInfo_shu(u4VirtualAddr);
	U32 u4RegByRank = RInfo_rank(u4VirtualAddr);
	PTR_T u4PhysicalAddr , u4BaseAddr = 0;
    DRAM_DFS_REG_SHU_T eShu = p->ShuRGAccessIdx;
    DRAM_RANK_T eRank = p->rank;

    u4BaseAddr = REG_BASE_ADDR_LIST[u4RegType][eCh]; /* Default */

    switch (u4RegType) {
    case REG_TYPE_DRAMC_AO:
    case REG_TYPE_PHY_AO:
        if ((u4RegIsShu == REG_IS_SHU_Y) && (eShu != DRAM_DFS_REG_SHU0))
        {
            if(u4RegType== REG_TYPE_DRAMC_AO)
                u4BaseAddr += (eShu * SHU_GRP_DRAMC_OFFSET);
            else if  (u4RegType== REG_TYPE_PHY_AO)
                u4BaseAddr += (eShu * SHU_GRP_DDRPHY_OFFSET);
        }

        // ====  Decide rank address====
        // if rank reg & current rank is RK1, need to do rank address shift
        if ((u4RegByRank != REG_BY_RANK_NONE) && (eRank == RANK_1))
        {
            if (u4RegType == REG_TYPE_DRAMC_AO)// ChA/B Dramc AO Register
            {
                u4Offset += DRAMC_REG_AO_RANK_OFFSET;                
            }
            else if (u4RegType == REG_TYPE_PHY_AO)// PhyA/B AO Register
            {
                if(u4RegByRank == REG_BY_RANK)
                {
                    if(u4RegIsShu == REG_IS_SHU_Y)
                    {
                        u4Offset += DDRPHY_AO_SHU_RANK_OFFSET;
                    }
                    else
                    {
                        u4Offset += DDRPHY_AO_RANK_OFFSET;
                    }
                }
            }        
        }

        break;

    case REG_TYPE_DRAMC_NAO:
        if (eRank == RANK_1) {
            if (u4Offset >= DRAMC_REG_NAO_RANK0_DQSOSC_STATUS_START &&
                u4Offset < DRAMC_REG_NAO_RANK0_DQSOSC_STATUS_END)
            {
                u4Offset += DRAMC_NAO_DQSOSC_STATUS_RK_OFFSET;
            }
            else if (u4Offset >= DRAMC_REG_NAO_RANK0_ROW_OFFSET_BASE_ADDR &&
                 u4Offset <= DRAMC_REG_NAO_RANK0_ROW_OFFSET_END_ADDR)
            {
                u4Offset += DRAMC_REG_NAO_RANK_OFFSET;
            }
        }
        break;
    case REG_TYPE_PHY_NAO:
        if (eRank == RANK_1) {
            // PhyA/B NAO Register
            if (u4Offset >= DDRPHY_NAO_RANK0_B0_DQSIEN_AUTOK_STATUS_START &&
                u4Offset < DDRPHY_NAO_RANK0_B0_DQSIEN_AUTOK_STATUS_END)
            {
                u4Offset += DDRPHY_NAO_DQSIEN_AUTOK_STATUS_RK_OFFSET;
            }
            else if (u4Offset >= DDRPHY_NAO_RANK0_B1_DQSIEN_AUTOK_STATUS_START &&
                u4Offset < DDRPHY_NAO_RANK0_B1_DQSIEN_AUTOK_STATUS_END)
            {
                u4Offset += DDRPHY_NAO_DQSIEN_AUTOK_STATUS_RK_OFFSET;
            }
            else if (u4Offset >= DDRPHY_NAO_RANK0_CA_DQSIEN_AUTOK_STATUS_START &&
                u4Offset < DDRPHY_NAO_RANK0_CA_DQSIEN_AUTOK_STATUS_END)
            {
                u4Offset += DDRPHY_NAO_DQSIEN_AUTOK_STATUS_RK_OFFSET;
            }
            else if (u4Offset >= DDRPHY_NAO_RANK0_GATING_STATUS_START &&
                u4Offset < DDRPHY_NAO_RANK0_GATING_STATUS_END)
            {
                u4Offset += DDRPHY_NAO_GATING_STATUS_RK_OFFSET;
            }
        }
        break;
    case REG_TYPE_DPM:
        break;
    case REG_TYPE_SPM:
        u4BaseAddr = REG_BASE_ADDR_LIST[u4RegType][0];
        break;
#if __ETT__
    case REG_TYPE_FKE:
        u4BaseAddr = REG_BASE_ADDR_LIST[u4RegType][fake_id_get(p)];
        break;
    case REG_TYPE_APBMIX:
        u4BaseAddr = REG_BASE_ADDR_LIST[u4RegType][0];
        break;
#endif
#if ENABLE_DRAMOBF
    case REG_TYPE_DRAMOBF:
        break;
#endif

    default:
        mcSHOW_ERR_MSG("[%s][Error] Unrecognized REG_TYPE %d\n", __func__, u4RegType);
        ASSERT(0);
        break;
    }

    u4PhysicalAddr = (IO_PHYS + u4BaseAddr + u4Offset);
    
#if REG_ACCESS_DBG_EN
    gu1IsRankRG = u4RegByRank;
    gu2RankOfst = u4Offset - RInfo_addr(u4VirtualAddr);
#endif
#if 0//FOR_DV_SIMULATION_USED
    mcSHOW_DBG_MSG("[%s] 0x%X --> 0x%X (0x%X+0x%X)\n", __func__, u4VirtualAddr, u4PhysicalAddr, u4BaseAddr, u4Offset);
#endif
    return u4PhysicalAddr;
}

/* Note, NO SWAP info considered */
PTR_T DramcVirt2Phys(DRAMC_CTX_T *p, U32 u4Virt)
{
    return  u4RegBaseAddrTranslate(p, channel_get(p), u4Virt);
}

static inline U32 _Register_Read(PTR_T u4reg_addr)
{
    U32 u4reg_value;
#if (FOR_DV_SIMULATION_USED == 0)
    dsb();
#endif

#if QT_GUI_Tool
    ucDramRegRead(u4reg_addr, &u4reg_value);
#elif (FOR_DV_SIMULATION_USED == 1)   //DV
    u4reg_value = register_read_c(u4reg_addr);
#else // real chip
    u4reg_value = *((volatile unsigned int *)u4reg_addr);
#endif

    return u4reg_value;
}

//-------------------------------------------------------------------------
/** ucDram_Register_Read
 *  DRAM register read (32-bit).
 *  @param  u4reg_addr    register address in 32-bit.
 *  @param  pu4reg_value  Pointer of register read value.
 *  @retval 0: OK, 1: FAIL
 */
//-------------------------------------------------------------------------
// This function need to be porting by BU requirement
U32 u4Dram_Register_Read(DRAMC_CTX_T *p, U32 u4VirtualAddr)
{
    PTR_T u4reg_addr;

    u4reg_addr = u4RegBaseAddrTranslate(p, p->channel, u4VirtualAddr);

    return _Register_Read(u4reg_addr);
}


//-------------------------------------------------------------------------
/** ucDram_Register_Write
 *  DRAM register write (32-bit).
 *  @param  u4reg_addr    register address in 32-bit.
 *  @param  u4reg_value   register write value.
 *  @retval 0: OK, 1: FAIL
 */
//-------------------------------------------------------------------------

#if 0//REG_ACCESS_NAO_DGB
#if (fcFOR_CHIP_ID == fcCervino)
U8 Check_RG_Not_AO(PTR_T u4reg_addr)
{
    U8 RegNotAO = 0;
    if ((u4reg_addr >= DRAMC_AO_BASE_ADDRESS) && (u4reg_addr <= DRAMC_REG_SHU4_DQSG_RETRY))
    {
    }
    else if ((u4reg_addr >= DRAMC_AO_BASE_ADDRESS + SHIFT_TO_CHB_ADDR) && (u4reg_addr <= DRAMC_REG_SHU4_DQSG_RETRY + SHIFT_TO_CHB_ADDR))
    {
    }
    else if ((u4reg_addr >= DDRPHY_AO_BASE_ADDR) && (u4reg_addr <= DDRPHY_RFU_0X1FCC))
    {
    }
    else if ((u4reg_addr >= DDRPHY_AO_BASE_ADDR + SHIFT_TO_CHB_ADDR) && (u4reg_addr <= DDRPHY_RFU_0X1FCC + SHIFT_TO_CHB_ADDR))
    {
    }
    else
    {
        RegNotAO = 1;
    }
    return RegNotAO;
}
#endif
#endif

static inline void _Register_Write(PTR_T u4reg_addr, U32 u4reg_value)
{
#if QT_GUI_Tool
     ucDramRegWrite(u4reg_addr, u4reg_value);
#elif (FOR_DV_SIMULATION_USED == 1) //DV
    register_write_c(u4reg_addr, u4reg_value);
#else // real chip
    (*(volatile unsigned int *)u4reg_addr) = u4reg_value;//real chip
    dsb();
#endif

#if DUMP_IO_WRITE
    mcSHOW_DBG_MSG("DUMP_IO_WRITE: 0x%08lx = 0x%08x\n", u4reg_addr, u4reg_value);
#endif

#ifdef DUMP_INIT_RG_LOG_TO_DE
    if (gDUMP_INIT_RG_LOG_TO_DE_RG_log_flag == 1)
    {
        mcSHOW_DUMP_INIT_RG_MSG("*(UINT32P)(0x%lx) = 0x%x;\n",u4reg_addr,u4reg_value);
        gDUMP_INIT_RG_LOG_TO_DE_RG_log_flag = 0;
        mcDELAY_MS(1); // to receive log for log 
        gDUMP_INIT_RG_LOG_TO_DE_RG_log_flag = 1;
    }
#endif 

#if 0//REG_ACCESS_DBG_EN
    if (RegLogEnable)
    {
        mcSHOW_DBG_MSG("\n[REG_ACCESS_PORTING_DBG] %s Reg(0x%lx) = 0x%X\n", __func__, u4reg_addr, u4reg_value);
    }
#endif
}


void vDram_Register_Write(DRAMC_CTX_T *p, U32 u4VirtualAddr, U32 u4reg_value)
{
    PTR_T u4reg_addr;
    //mcSHOW_DBG_MSG("\n[%s] Reg(0x%X) = 0x%X\n", __func__, u4VirtualAddr, u4reg_value);
    
    u4reg_addr = u4RegBaseAddrTranslate(p, p->channel, u4VirtualAddr);
    _Register_Write(u4reg_addr, u4reg_value);
    
#if REG_ACCESS_DBG_EN
    if (RegLogEnable){
        mcSHOW_DBG_MSG("\n[REG_ACCESS_PORTING_DBG][After_tran][%s] CH(%d) Rank(%x), RankOfst(0x%x), BroadC(%x), Reg(0x%X) Mask(0x%X) = 0x%X\n", __func__, p->channel, (gu1IsRankRG ? p->rank : 0xff), gu2RankOfst, GetDramcBroadcast(), u4reg_addr, 0xffffffff, u4reg_value);
    }
#endif
}


void vIO32WriteMsk(DRAMC_CTX_T *p, U32 u4VirtualAddr, U32 val32, U32 msk32)
{
    PTR_T u4reg_addr;
    U32 u4Val;

    u4reg_addr = u4RegBaseAddrTranslate(p, p->channel, u4VirtualAddr);

    val32 &= msk32;
    u4Val = _Register_Read(u4reg_addr);
    u4Val = ((u4Val & ~msk32) | val32);
    _Register_Write(u4reg_addr, u4Val);
    
#if REG_ACCESS_DBG_EN
    if (RegLogEnable){
        mcSHOW_DBG_MSG("\n[REG_ACCESS_PORTING_DBG][After_tran][%s] CH(%d) Rank(%x), RankOfst(0x%x), BroadC(%x), Reg(0x%X) Mask(0x%X) = 0x%X\n", __func__, p->channel, (gu1IsRankRG ? p->rank : 0xff), gu2RankOfst, GetDramcBroadcast(), u4reg_addr, msk32, u4Val);
    }
#endif
}


#if 0
void vIO32Write4B_All2(DRAMC_CTX_T *p, PTR_T u4VirtualAddr, U32 val32)
{
	U8 chIdx, u1AllCount;
	PTR_T u4reg_addr;
	U8 u1BCSupport = TRUE;

#if __ETT__
    if (u1BCSupport && GetDramcBroadcast()==DRAMC_BROADCAST_ON)
    {
        mcSHOW_ERR_MSG("Error! virtual address 0x%x don't have to use write_all when Dramc WBR is on\n", u4VirtualAddr);
        ASSERT(0);
    }
#endif
    
    u1AllCount = CHANNEL_NUM; // for all dramC and PHY

    for (chIdx = 0; chIdx < u1AllCount; chIdx++)
    {
        u4reg_addr = u4RegBaseAddrTranslate(chIdx, p->ShuRGAccessIdx, p->rank, u4VirtualAddr);
        _Register_Write(u4reg_addr, val32);
    }
}
#endif


void vIO32WriteMsk_All(DRAMC_CTX_T *p, U32 u4VirtualAddr, U32 val32, U32 msk32)
{
    PTR_T u4reg_addr;
    U32 u4Val;
    U8 chIdx, u1AllCount;
    U8 u1BCSupport = TRUE;

    u1AllCount = p->support_channel_num; // for all dramC and PHY

    if (RInfo_rgtype(u4VirtualAddr) == REG_TYPE_DPM)
    {
        if (!p->u1SingleTop)
            u1AllCount >>= 1;

        u1BCSupport = FALSE;
    }

#if __ETT__
    if (u1BCSupport && GetDramcBroadcast()==DRAMC_BROADCAST_ON)
    {
        mcSHOW_ERR_MSG("Error! virtual address 0x%x don't have to use write_all when Dramc WBR is on\n", u4VirtualAddr);
        ASSERT(0);
    }
#endif

    val32 &= msk32;
    
    for (chIdx = 0; chIdx < u1AllCount; chIdx++)
    {
        if (dramc_io_manager.enable_channel_msk_operation && !(dramc_io_manager.channel_msk & (1 << chIdx)))
            continue;

        u4reg_addr = u4RegBaseAddrTranslate(p, chIdx, u4VirtualAddr);
        u4Val = _Register_Read(u4reg_addr);
        u4Val = ((u4Val & ~msk32) | val32);
        _Register_Write(u4reg_addr, u4Val);
        
#if REG_ACCESS_DBG_EN
        if (RegLogEnable){
            mcSHOW_DBG_MSG("\n[REG_ACCESS_PORTING_DBG][After_tran][%s] CH(%d) Rank(%x), RankOfst(0x%x), BroadC(%x), Reg(0x%X) Mask(0x%X) = 0x%X\n", __func__, chIdx, (gu1IsRankRG ? p->rank : 0xff), gu2RankOfst, GetDramcBroadcast(), u4reg_addr, msk32, u4Val);
        }
#endif
    }
}


// only for multi && whole register is not swap mixed
void vPhyByteIO32WriteMsk(DRAMC_CTX_T *p, U32 u4VirtualAddr, U32 val32, U32 msk32)
{
    PTR_T u4reg_addr;
    U32 u4Val;

    u4PhyAoByteSwap(&u4VirtualAddr, NULL, p->PhySwapRule[p->channel]);
    u4reg_addr = u4RegBaseAddrTranslate(p, p->channel, u4VirtualAddr);

    val32 &= msk32;
    u4Val = _Register_Read(u4reg_addr);
    u4Val = ((u4Val & ~msk32) | val32);
    _Register_Write(u4reg_addr, u4Val);
    
#if REG_ACCESS_DBG_EN
    if (RegLogEnable){
        mcSHOW_DBG_MSG("\n[REG_ACCESS_PORTING_DBG][After_tran][%s] CH(%d) Rank(%x), RankOfst(0x%x), BroadC(%x), Reg(0x%X) Mask(0x%X) = 0x%X\n", __func__, p->channel, (gu1IsRankRG ? p->rank : 0xff), gu2RankOfst, GetDramcBroadcast(), u4reg_addr, msk32, u4Val);
    }
#endif
}


// only for multi && whole register is not swap mixed
void vPhyByteIO32WriteMsk_All(DRAMC_CTX_T *p, U32 u4VirtualAddr, U32 val32, U32 msk32)
{
    PTR_T u4reg_addr;
    U32 u4Val, u4TmpAddr;
    U8 chIdx, u1AllCount;
    U8 u1BCSupport = TRUE;

    u1AllCount = p->support_channel_num; // for all dramC and PHY
    if (RInfo_rgtype(u4VirtualAddr) == REG_TYPE_DPM)
    {
        if (!p->u1SingleTop)
            u1AllCount >>= 1;

        u1BCSupport = FALSE;
    }

#if __ETT__
    if (u1BCSupport && GetDramcBroadcast()==DRAMC_BROADCAST_ON)
    {
        mcSHOW_ERR_MSG("Error! virtual address 0x%x don't have to use write_all when Dramc WBR is on\n", u4VirtualAddr);
        ASSERT(0);
    }
#endif

    val32 &= msk32;

    #if !SUPPORT_PHY_BTYE_SWAP_BY_CHANNEL
    u4PhyAoByteSwap(&u4VirtualAddr, NULL, p->PhySwapRule[CHANNEL_A]);
    #endif
    
    for (chIdx = 0; chIdx < u1AllCount; chIdx++)
    {
        if (dramc_io_manager.enable_channel_msk_operation && !(dramc_io_manager.channel_msk & (1 << chIdx)))
            continue;

        #if SUPPORT_PHY_BTYE_SWAP_BY_CHANNEL
        u4TmpAddr = u4VirtualAddr;
        u4PhyAoByteSwap(&u4TmpAddr, NULL, p->PhySwapRule[chIdx]);
        u4reg_addr = u4RegBaseAddrTranslate(p, chIdx, u4TmpAddr);
        #else
        u4reg_addr = u4RegBaseAddrTranslate(p, chIdx, u4VirtualAddr);
        #endif
        
        u4Val = _Register_Read(u4reg_addr);
        u4Val = ((u4Val & ~msk32) | val32);
        _Register_Write(u4reg_addr, u4Val);
        
#if REG_ACCESS_DBG_EN
        if (RegLogEnable){
            mcSHOW_DBG_MSG("\n[REG_ACCESS_PORTING_DBG][After_tran][%s] CH(%d) Rank(%x), RankOfst(0x%x), BroadC(%x), Reg(0x%X) Mask(0x%X) = 0x%X\n", __func__, chIdx, (gu1IsRankRG ? p->rank : 0xff), gu2RankOfst, GetDramcBroadcast(), u4reg_addr, msk32, u4Val);
        }
#endif
    }
}


U32 vPhyByteReadFldAlign(DRAMC_CTX_T *p, U32 u4VirtualAddr, U32 msk32)
{
    PTR_T u4reg_addr;
    U32 u4Val;

    // address translate for PHY AO byte swap
    if(msk32==FLD_FULL)
    {
        u4PhyAoByteSwap(&u4VirtualAddr, NULL, p->PhySwapRule[p->channel]);// Read 4B, no mask information
    }
    else{
        u4PhyAoByteSwap(&u4VirtualAddr, &msk32, p->PhySwapRule[p->channel]);
    }

    // address translate by channel
    u4reg_addr = u4RegBaseAddrTranslate(p, p->channel, u4VirtualAddr);

    if (msk32 == FLD_FULL)
        u4Val = _Register_Read(u4reg_addr);
    else
        u4Val = (_Register_Read(u4reg_addr) & Fld2Msk32(msk32)) >> Fld_shft(msk32);

    return u4Val;
}


void vPhyByteWriteFldAlign(DRAMC_CTX_T *p, U32 u4VirtualAddr, U32 val32, U32 fld32, CHANNEL_RANK_SEL_T chType)
{
    PTR_T u4reg_addr;
    U32 u4Val, msk32, u4TmpAddr;
    U8 chIdx, chBegin, chEnd;
    U8 u1ByteType = RInfo_byte(u4VirtualAddr);

    if(chType == TO_ALL_CHANNEL)
    {
        chBegin = CHANNEL_A;
        chEnd = p->support_channel_num;

        if (RInfo_rgtype(u4VirtualAddr) == REG_TYPE_DPM && !p->u1SingleTop)
            chEnd >>= 1;
    }
    else 
    {
        chBegin = p->channel;
        chEnd = p->channel +1;      
    }
    
    // address translate for PHY AO byte swap
    if(fld32==FLD_FULL)
    {
      // Write 4B, no mask information
      #if !SUPPORT_PHY_BTYE_SWAP_BY_CHANNEL
      u4PhyAoByteSwap(&u4VirtualAddr, NULL, p->PhySwapRule[CHANNEL_A]);
      #endif
      msk32 = 0xffffffff;
    }
    else
    {
      #if !SUPPORT_PHY_BTYE_SWAP_BY_CHANNEL
      u4PhyAoByteSwap(&u4VirtualAddr, &fld32, p->PhySwapRule[CHANNEL_A]);
      #endif
    
      val32 = (val32 << Fld_shft(fld32));
      msk32 = Fld2Msk32(fld32);
      val32 &= msk32;
    }
    
    for (chIdx = chBegin; chIdx < chEnd; chIdx++)
    {
        if (dramc_io_manager.enable_channel_msk_operation && !(dramc_io_manager.channel_msk & (1 << chIdx)))
            continue;

        #if SUPPORT_PHY_BTYE_SWAP_BY_CHANNEL        
        u4TmpAddr = u4VirtualAddr;
        
        // address translate for PHY AO byte swap
        if(fld32==FLD_FULL)
        {
          // Write 4B, no mask information
          u4PhyAoByteSwap(&u4TmpAddr, NULL, p->PhySwapRule[chIdx]);
        }
        else
        {
          u4PhyAoByteSwap(&u4TmpAddr, &fld32, p->PhySwapRule[chIdx]);
        }
        
        // address translate by channel
        u4reg_addr = u4RegBaseAddrTranslate(p, chIdx, u4TmpAddr);
        #else
        u4reg_addr = u4RegBaseAddrTranslate(p, chIdx, u4VirtualAddr);
        #endif

        
        u4Val = _Register_Read(u4reg_addr);
        u4Val = ((u4Val & ~msk32) | val32);
        _Register_Write(u4reg_addr, u4Val);
        
#if REG_ACCESS_DBG_EN
        if (RegLogEnable){
            mcSHOW_DBG_MSG("\n[REG_ACCESS_PORTING_DBG][After_tran][%s] CH(%d) Rank(%x), RankOfst(0x%x), BroadC(%x), Reg(0x%X) Mask(0x%X) = 0x%X\n", __func__, chIdx, (gu1IsRankRG ? p->rank : 0xff), gu2RankOfst, GetDramcBroadcast(), u4reg_addr, msk32, u4Val);
        }
#endif
    }
}


typedef struct _REG_PHY_BYTE_FLD_MULTI
{
    U32 u4Val;
    U32 u4Fld;
} REG_PHY_BYTE_FLD_MULTI_T;

void vPhyByteWriteFldMulti2(DRAMC_CTX_T *p, U32 u4VirtualAddr, CHANNEL_RANK_SEL_T chType, U32 rglist, ...)
{
    U32 temp;
    U8 u1FldIdx, u1FldNum=0;
    REG_PHY_BYTE_FLD_MULTI_T ParseFld[33];
    
    va_list args;
    va_start(args, rglist);

    ParseFld[0].u4Val = rglist;
    
    // Get field array
    while (1)
    {
        if((temp = va_arg(args, U32)) != PB_FLD_END)
        {
            ParseFld[u1FldNum].u4Fld = temp;
            mcSHOW_DBG_MSG5("[%s] u1FldIdx %d , 0x%X  0x%X\n", __func__, u1FldNum ,ParseFld[u1FldNum].u4Val, ParseFld[u1FldNum].u4Fld);
            vPhyByteWriteFldAlign(p, u4VirtualAddr, ParseFld[u1FldNum].u4Val, ParseFld[u1FldNum].u4Fld, chType);
            u1FldNum++;
        }
        else 
            break;

        if((temp = va_arg(args, U32)) != PB_FLD_END)
        {
            ParseFld[u1FldNum].u4Val = temp;
        }
        else 
            break;
    }
    va_end(args);
}

