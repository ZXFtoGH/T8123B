#include "dramc_common.h"

//U8 const (*uiLPDDR_MRR_Mapping_POP)[DQ_DATA_WIDTH_LP4];
//U8 const (*uiLPDDR_O1_Mapping_POP)[DQ_DATA_WIDTH_LP4];
#if SUPPORT_TYPE_LPDDR4
U8 const (*uiLPDDR4_CA_Mapping_POP)[CATRAINING_NUM_LP4];
#endif
#if SUPPORT_TYPE_LPDDR5
U8 const (*uiLPDDR5_CA_Mapping_POP)[CATRAINING_NUM_LP5];
U8 const (*uiLPDDR5_PAD_Mapping_POP)[DQ_DATA_WIDTH_LP4 + DQS_NUMBER];
#endif

#if SUPPORT_TYPE_LPDDR4
#if (CA_PER_BIT_DELAY_CELL || PINMUX_AUTO_TEST_PER_BIT_CA)
static const U8 uiLPDDR4_CA_DRAM_Pinmux[PINMUX_MAX][CHANNEL_NUM][CATRAINING_NUM_LP4] = {
#if (fcFOR_CHIP_ID == fcMT8139)
    [PINMUX_DSC] = {
        [CHANNEL_A] = {
            0, 1, 2, 3, 4, 5,
        },
        [CHANNEL_B] = {
            0, 1, 2, 3, 4, 5,
        },
#if CHANNEL_NUM > 2
        [CHANNEL_C] = {
            0, 1, 2, 3, 4, 5,
        },
        [CHANNEL_D] = {
            0, 1, 2, 3, 4, 5,
        },
#endif
    },

    [PINMUX_EMCP] = {
        [CHANNEL_A] = {
            0, 1, 2, 3, 4, 5,
        },
        [CHANNEL_B] = {
            0, 1, 2, 3, 4, 5,
        },
#if CHANNEL_NUM > 2
        [CHANNEL_C] = {
            0, 1, 2, 3, 4, 5,
        },
        [CHANNEL_D] = {
            0, 1, 2, 3, 4, 5,
        },
#endif
    },

    [PINMUX_LPBK] ={ /*TBD */ },

#elif (fcFOR_CHIP_ID == fcFigeac)
    [PINMUX_DSC] = {
        [CHANNEL_A] = {
            0, 1, 8, 7, 2, 3,
        },
        [CHANNEL_B] = {
            1, 0, 8, 2, 4 ,3,
        },
#if CHANNEL_NUM > 2
        [CHANNEL_C] = {
            0, 1, 2, 3, 4, 5,
        },
        [CHANNEL_D] = {
            0, 1, 2, 3, 4, 5,
        },
#endif
    },

    [PINMUX_EMCP] = {
        [CHANNEL_A] = {
            0, 1, 8, 3, 7, 5,
        },
        [CHANNEL_B] = {
            1, 0, 8, 2, 3, 5,
        },
#if CHANNEL_NUM > 2
        [CHANNEL_C] = {
            0, 1, 2, 3, 4, 5,
        },
        [CHANNEL_D] = {
            0, 1, 2, 3, 4, 5,
        },
#endif
    },

    [PINMUX_LPBK] ={ /*TBD */ },

#endif
};
#endif

//O1 DRAM->APHY
static const U8 uiLPDDR4_O1_DRAM_Pinmux[PINMUX_MAX][CHANNEL_NUM][DQ_DATA_WIDTH_LP4] = {
#if (fcFOR_CHIP_ID == fcMT8139)
    [PINMUX_DSC] = {
        [CHANNEL_A] = {
            0, 1, 2, 3, 4, 5, 6, 7,
            8, 9, 10, 11, 12, 13, 14, 15
        },
        [CHANNEL_B] = {
            0, 1, 2, 3, 4, 5, 6, 7,
            8, 9, 10, 11, 12, 13, 14, 15
        },
#if CHANNEL_NUM > 2
        [CHANNEL_C] = {
            0, 1, 2, 3, 4, 5, 6, 7,
            8, 9, 10, 11, 12, 13, 14, 15
        },
        [CHANNEL_D] = {
            0, 1, 2, 3, 4, 5, 6, 7,
            8, 9, 10, 11, 12, 13, 14, 15
        },
#endif
    },
    [PINMUX_EMCP] = {
        [CHANNEL_A] = {
            0, 1, 2, 3, 4, 5, 6, 7,
            8, 9, 10, 11, 12, 13, 14, 15
        },
        [CHANNEL_B] = {
            0, 1, 2, 3, 4, 5, 6, 7,
            8, 9, 10, 11, 12, 13, 14, 15
        },
#if CHANNEL_NUM > 2
        [CHANNEL_C] = {
            0, 1, 2, 3, 4, 5, 6, 7,
            8, 9, 10, 11, 12, 13, 14, 15
        },
        [CHANNEL_D] = {
            0, 1, 2, 3, 4, 5, 6, 7,
            8, 9, 10, 11, 12, 13, 14, 15
        },
#endif
    },
    [PINMUX_LPBK] = { /* TBD */ },

#elif (fcFOR_CHIP_ID == fcFigeac)
    [PINMUX_DSC] = {
        [CHANNEL_A] = {
            8, 9, 10, 11, 12, 13, 14, 15,
            0, 1, 2, 3, 4, 5, 6, 7
        },
        [CHANNEL_B] = {
            8, 9, 10, 11, 12, 13, 14, 15,
            0, 1, 2, 3, 4, 5, 6, 7
        },
#if CHANNEL_NUM > 2
        [CHANNEL_C] = {
            0, 1, 2, 3, 4, 5, 6, 7,
            8, 9, 10, 11, 12, 13, 14, 15
        },
        [CHANNEL_D] = {
            0, 1, 2, 3, 4, 5, 6, 7,
            8, 9, 10, 11, 12, 13, 14, 15
        },
#endif
    },
    [PINMUX_EMCP] = {
        [CHANNEL_A] = {
            0, 1, 2, 3, 4, 5, 6, 7,
            8, 9, 10, 11, 12, 13, 14, 15
        },
        [CHANNEL_B] = {
            0, 1, 2, 3, 4, 5, 6, 7,
            8, 9, 10, 11, 12, 13, 14, 15
        },
#if CHANNEL_NUM > 2
        [CHANNEL_C] = {
            0, 1, 2, 3, 4, 5, 6, 7,
            8, 9, 10, 11, 12, 13, 14, 15
        },
        [CHANNEL_D] = {
            0, 1, 2, 3, 4, 5, 6, 7,
            8, 9, 10, 11, 12, 13, 14, 15
        },
#endif
    },
    [PINMUX_LPBK] = { /* TBD */ },
#endif
};

//MRR DRAM->DRAMC
static const U8 uiLPDDR4_MRR_DRAM_Pinmux[PINMUX_MAX][CHANNEL_NUM][DQ_DATA_WIDTH_LP4] = {
#if (fcFOR_CHIP_ID == fcMT8139)
    [PINMUX_DSC] = {
        [CHANNEL_A] = {
            0, 1, 2, 3, 4, 5, 6, 7,
            8, 9, 10, 11, 12, 13, 14, 15
        },
        [CHANNEL_B] = {
            0, 1, 2, 3, 4, 5, 6, 7,
            8, 9, 10, 11, 12, 13, 14, 15
        },
#if CHANNEL_NUM > 2
        [CHANNEL_C] = {
            0, 1, 2, 3, 4, 5, 6, 7,
            8, 9, 10, 11, 12, 13, 14, 15
        },
        [CHANNEL_D] = {
            0, 1, 2, 3, 4, 5, 6, 7,
            8, 9, 10, 11, 12, 13, 14, 15
        },
#endif
    },
    [PINMUX_EMCP] = {
        [CHANNEL_A] = {
            0, 1, 2, 3, 4, 5, 6, 7,
            8, 9, 10, 11, 12, 13, 14, 15
        },
        [CHANNEL_B] = {
            0, 1, 2, 3, 4, 5, 6, 7,
            8, 9, 10, 11, 12, 13, 14, 15
        },
#if CHANNEL_NUM > 2
        [CHANNEL_C] = {
            0, 1, 2, 3, 4, 5, 6, 7,
            8, 9, 10, 11, 12, 13, 14, 15
        },
        [CHANNEL_D] = {
            0, 1, 2, 3, 4, 5, 6, 7,
            8, 9, 10, 11, 12, 13, 14, 15
        },
#endif
    },
    [PINMUX_LPBK] = { /* TBD */ },
#elif (fcFOR_CHIP_ID == fcFigeac)
    [PINMUX_DSC] = {
        [CHANNEL_A] = {
            8, 9, 10, 11, 12, 13, 14, 15,
            0, 1, 2, 3, 4, 5, 6, 7
        },
        [CHANNEL_B] = {
            8, 9, 10, 11, 12, 13, 14, 15,
            0, 1, 2, 3, 4, 5, 6, 7
        },
#if CHANNEL_NUM > 2
        [CHANNEL_C] = {
            0, 1, 2, 3, 4, 5, 6, 7,
            8, 9, 10, 11, 12, 13, 14, 15
        },
        [CHANNEL_D] = {
            0, 1, 2, 3, 4, 5, 6, 7,
            8, 9, 10, 11, 12, 13, 14, 15
        },
#endif
    },
    [PINMUX_EMCP] = {
        [CHANNEL_A] = {
            0, 1, 2, 3, 4, 5, 6, 7,
            8, 9, 10, 11, 12, 13, 14, 15
        },
        [CHANNEL_B] = {
            0, 1, 2, 3, 4, 5, 6, 7,
            8, 9, 10, 11, 12, 13, 14, 15
        },
#if CHANNEL_NUM > 2
        [CHANNEL_C] = {
            0, 1, 2, 3, 4, 5, 6, 7,
            8, 9, 10, 11, 12, 13, 14, 15
        },
        [CHANNEL_D] = {
            0, 1, 2, 3, 4, 5, 6, 7,
            8, 9, 10, 11, 12, 13, 14, 15
        },
#endif
    },
    [PINMUX_LPBK] = { /* TBD */ },
#endif
};
#endif /* SUPPORT_TYPE_LPDDR4 */

#if SUPPORT_TYPE_LPDDR5
// BGA -> PAD
#if (CA_PER_BIT_DELAY_CELL || PINMUX_AUTO_TEST_PER_BIT_CA)
static const U8 uiLPDDR5_CA_DRAM_Pinmux[PINMUX_MAX][CHANNEL_NUM][CATRAINING_NUM_LP5] = {
#if (fcFOR_CHIP_ID == fcFigeac)
    [PINMUX_EMCP] = {
        [CHANNEL_A] = {
            1, 0, 8, 4, 3, 6, 5,
        },
    #if (CHANNEL_NUM>1)
        [CHANNEL_B] = {
            0, 1, 6, 8, 2, 5, 4,
        }
    #endif
    },
#elif (fcFOR_CHIP_ID == fcLepin)
    [PINMUX_EMCP] = {
        [CHANNEL_A] = {
            0, 1, 2, 5, 4, 6, 7,
        },
#if (CHANNEL_NUM>1)
        [CHANNEL_B] = {
            7, 6, 5, 2, 4, 1, 0,
        },
#endif
#if (CHANNEL_NUM>2)
        [CHANNEL_C] = {
            7, 6, 5, 2, 4, 1, 0,
        },
        [CHANNEL_D] = {
            0, 1, 2, 5, 4, 6, 7,
        },
#endif
    },
#endif
};
#endif

static const U8 uiLPDDR5_O1_DRAM_Pinmux[PINMUX_MAX][CHANNEL_NUM][DQ_DATA_WIDTH_LP4] = {
#if (fcFOR_CHIP_ID == fcMT8139)
    /* NOT SUPPORT */
#elif (fcFOR_CHIP_ID == fcLepin || fcFOR_CHIP_ID == fcFigeac)
    [PINMUX_EMCP] = {
    // for EMCP
        [CHANNEL_A] = {
            8, 9, 10, 11, 12, 13, 14, 15,
            0, 1, 2, 3, 4, 5, 6, 7,
        },
#if (CHANNEL_NUM>1)
        [CHANNEL_B] = {
            0, 1, 2, 3, 4, 5, 6, 7,
            8, 9, 10, 11, 12, 13, 14, 15,
        },
#endif
#if (CHANNEL_NUM>2)
        [CHANNEL_C] = {
            0, 1, 2, 3, 4, 5, 6, 7,
            8, 9, 10, 11, 12, 13, 14, 15,
        },
        [CHANNEL_D] = {
            8, 9, 10, 11, 12, 13, 14, 15,
            0, 1, 2, 3, 4, 5, 6, 7,
        },
#endif
    }
#endif
};

// DRAM->APHY (BGA -> PAD)
static const U8 uiLPDDR5_PAD_DRAM_Pinmux[PINMUX_MAX][CHANNEL_NUM][DQ_DATA_WIDTH_LP4 + DQS_NUMBER] = {
#if (fcFOR_CHIP_ID == fcLepin)
    [PINMUX_EMCP] = {
        [CHANNEL_A] = {
            8, 9, 10, 11, 12, 13, 17 /* B1_DQ8 */, 14,
            0, 1, 2, 3, 7, 4, 16 /* B0_DQ8 */, 6,
            15, /* DQM0 */
            5 , /* DQM1 */
        },
#if (CHANNEL_NUM>1)
        [CHANNEL_B] = {
            0, 1, 2, 3, 7, 4, 16 /* B0_DQ8 */, 6,
            8, 9, 10, 11, 13, 12, 17 /* B1_DQ8 */, 15,
            5, /* DQM0 */
            14, /* DQM1 */
        },
#endif
#if (CHANNEL_NUM>2)
        [CHANNEL_C] = {
            0, 1, 2, 3, 7, 4, 16 /* B0_DQ8 */, 6,
            8, 9, 10, 11, 13, 12, 17 /* B1_DQ8 */, 15,
            5, /* DQM0 */
            14, /* DQM1 */
        },
        [CHANNEL_D] = {
            8, 9, 10, 11, 12, 13, 17 /* B1_DQ8 */, 14,
            0, 1, 2, 3, 7, 4, 16 /* B0_DQ8 */, 6,
            15, /* DQM0 */
            5 , /* DQM1 */
        },
#endif
    },
#elif (fcFOR_CHIP_ID == fcFigeac)
    [PINMUX_EMCP] = {
        [CHANNEL_A] = {
            9, 8, 11, 12, 13, 15, 14, 17 /* B1_DQ8 */,
            1, 0, 3, 4, 7, 16 /* B0_DQ8 */, 6, 2,
            10, /* DQM0 */
            5 , /* DQM1 */
        },
#if (CHANNEL_NUM>1)
        [CHANNEL_B] = {
            1, 0, 3, 5, 7, 6, 16 /* B0_DQ8 */, 2,
            8, 9, 11, 14, 13, 10, 15, 17 /* B1_DQ8 */,
            4, /* DQM0 */
            12, /* DQM1 */
        },
#endif
    }
#endif
};

static const U8 uiLPDDR5_MRR_DRAM_Pinmux[PINMUX_MAX][CHANNEL_NUM][DQ_DATA_WIDTH_LP4] = {
#if (fcFOR_CHIP_ID == fcMT8139)
    /* Not Support */
#elif (fcFOR_CHIP_ID == fcLepin || fcFOR_CHIP_ID == fcFigeac)
    [PINMUX_EMCP] = {
    // for EMCP
        [CHANNEL_A] = {
            8, 9, 10, 11, 12, 13, 14, 15,
            0, 1, 2, 3, 4, 5, 6, 7,
        },
#if (CHANNEL_NUM>1)
        [CHANNEL_B] = {
            0, 1, 2, 3, 4, 5, 6, 7,
            8, 9, 10, 11, 12, 13, 14, 15,
        },
#endif
#if (CHANNEL_NUM>2)
        [CHANNEL_C] = {
            0, 1, 2, 3, 4, 5, 6, 7,
            8, 9, 10, 11, 12, 13, 14, 15,
        },
        [CHANNEL_D] = {
            8, 9, 10, 11, 12, 13, 14, 15,
            0, 1, 2, 3, 4, 5, 6, 7,
        },
#endif
    },
#endif
};

#endif

#if 0
void Set_DRAM_Pinmux_Sel(DRAMC_CTX_T *p)
{
    mcSHOW_DBG_MSG2("[Set_DRAM_Pinmux_Sel] DRAMPinmux = %d\n", p->DRAMPinmux);

#if SUPPORT_TYPE_LPDDR4
    if (u1IsLP4Family(p->dram_type)) {
        uiLPDDR_MRR_Mapping_POP = uiLPDDR4_MRR_DRAM_Pinmux[p->DRAMPinmux];
        uiLPDDR_O1_Mapping_POP = uiLPDDR4_O1_DRAM_Pinmux[p->DRAMPinmux];
    #if (CA_PER_BIT_DELAY_CELL || PINMUX_AUTO_TEST_PER_BIT_CA)
        uiLPDDR4_CA_Mapping_POP = uiLPDDR4_CA_DRAM_Pinmux[p->DRAMPinmux];
    #endif
    } else
#endif
#if SUPPORT_TYPE_LPDDR5
    if (is_lp5_family(p)) {
        uiLPDDR_MRR_Mapping_POP = uiLPDDR5_MRR_DRAM_Pinmux[p->DRAMPinmux];
        uiLPDDR_O1_Mapping_POP = uiLPDDR5_O1_DRAM_Pinmux[p->DRAMPinmux];
        uiLPDDR5_PAD_Mapping_POP = uiLPDDR5_PAD_DRAM_Pinmux[p->DRAMPinmux];
    #if (CA_PER_BIT_DELAY_CELL || PINMUX_AUTO_TEST_PER_BIT_CA)
        uiLPDDR5_CA_Mapping_POP = uiLPDDR5_CA_DRAM_Pinmux[p->DRAMPinmux];
    #endif
    } else
#endif
    {
        mcSHOW_ERR_MSG("[Error] %s: Invalid MEM_TYPE\n", __func__);
        ASSERT(0);
    }
}
#endif
