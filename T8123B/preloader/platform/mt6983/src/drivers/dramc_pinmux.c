#include "dramc_common.h"

U8 const (*uiLPDDR_MRR_Mapping_POP)[DQ_DATA_WIDTH];
U8 const (*uiLPDDR_O1_Mapping_POP)[DQ_DATA_WIDTH];
U8 const (*uiLPDDR_PAD_TA2_Mapping_POP)[DQ_DATA_WIDTH + DQS_NUMBER];
#if SUPPORT_TYPE_LPDDR4
U8 const (*uiLPDDR4_CA_Mapping_POP)[CATRAINING_NUM_LP4];
#endif
#if SUPPORT_TYPE_LPDDR5
U8 const (*uiLPDDR5_CA_Mapping_POP)[CATRAINING_NUM_LP5];
U8 const (*uiLPDDR5_PAD_Mapping_POP)[DQ_DATA_WIDTH + DQS_NUMBER];
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
static const U8 uiLPDDR4_O1_DRAM_Pinmux[PINMUX_MAX][CHANNEL_NUM][DQ_DATA_WIDTH] = {
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

// TA2->APHY: TA2 bit mapping to aphy RG that doesn't have HW pinmux. Equil to uiLPDDR5_PAD_DRAM_Pinmux without byte swap.
static const U8 uiLPDDR4_PAD_TA2_Pinmux[PINMUX_MAX][CHANNEL_NUM][DQ_DATA_WIDTH + DQS_NUMBER] = {
#if (fcFOR_CHIP_ID == fcMT8139) //TODO
    [PINMUX_EMCP] = {
        [CHANNEL_A] = {
            0, 1, 2, 3, 7, 4, 16 /* B1_DQ8 */, 6,
            8, 9, 10, 11, 12, 13, 17 /* B0_DQ8 */, 14,
            5, /* DQM0 */
            15 , /* DQM1 */
        }
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
            0, 1, 2, 3, 7, 4, 16 /* B1_DQ8 */, 6,
            8, 9, 10, 11, 12, 13, 17 /* B0_DQ8 */, 14,
            5, /* DQM0 */
            15 , /* DQM1 */
        },
#endif
    },
#elif (fcFOR_CHIP_ID == fcFigeac) //TODO
    [PINMUX_EMCP] = {
        [CHANNEL_A] = {
            1, 0, 7, 16/* B0_DQ8 */, 3, 2, 5, 6,
            8, 9, 13, 12, 11, 17/* B1_DQ8 */, 10, 15,
            4, /* DQM0 */
            14 , /* DQM1 */
        },
#if (CHANNEL_NUM>1)
        [CHANNEL_B] = {
            1, 0, 7, 6, 3, 2, 4, 16/* B0_DQ8 */,
            9, 8, 13, 14, 11, 17/* B1_DQ8 */, 12, 10,
            5, /* DQM0 */
            15, /* DQM1 */
        },
#endif
    },
    [PINMUX_DSC] = {
        [CHANNEL_A] = {
            1, 0, 5, 16/* B0_DQ8 */, 4, 2, 7, 6 ,
            8, 9, 14, 12, 10, 13 , 11, 17/* B1_DQ8 */,
            3, /* DQM0 */
            15 , /* DQM1 */
        },
#if (CHANNEL_NUM>1)
        [CHANNEL_B] = {
            1, 0, 7, 6, 5, 4, 2 , 16/* B0_DQ8 */,
            9, 8, 12, 14, 13, 10, 15, 17 /* B1_DQ8 */,
            3, /* DQM0 */
            11, /* DQM1 */
        },
#endif
    }
#endif
};
//MRR DRAM->DRAMC
static const U8 uiLPDDR4_MRR_DRAM_Pinmux[PINMUX_MAX][CHANNEL_NUM][DQ_DATA_WIDTH] = {
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

static const U8 uiLPDDR5_O1_DRAM_Pinmux[PINMUX_MAX][CHANNEL_NUM][DQ_DATA_WIDTH] = {
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
static const U8 uiLPDDR5_PAD_DRAM_Pinmux[PINMUX_MAX][CHANNEL_NUM][DQ_DATA_WIDTH + DQS_NUMBER] = {
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

// TA2->APHY: TA2 bit mapping to aphy RG that doesn't have HW pinmux. Equil to uiLPDDR5_PAD_DRAM_Pinmux without byte swap.
static const U8 uiLPDDR5_PAD_TA2_Pinmux[PINMUX_MAX][CHANNEL_NUM][DQ_DATA_WIDTH + DQS_NUMBER] = {
#if (fcFOR_CHIP_ID == fcLepin)
    [PINMUX_EMCP] = {
        [CHANNEL_A] = {
            0, 1, 2, 3, 5, 16 /* B0_DQ8 */, 7, 4,
            8, 9, 10, 11, 12, 13, 15, 17 /* B1_DQ8 */,
            6, /* DQM0 */
            14 , /* DQM1 */
        },
#if (CHANNEL_NUM>1)
        [CHANNEL_B] = {
            0, 1, 2, 3, 5, 16 /* B0_DQ8 */, 7 , 4,
            8, 9, 10, 11, 13, 12, 17 /* B1_DQ8 */, 15,
            6, /* DQM0 */
            14, /* DQM1 */
        },
#endif
#if (CHANNEL_NUM>2)
        [CHANNEL_C] = {
            0, 1, 2, 3, 5, 16 /* B0_DQ8 */, 7, 4,
            8, 9, 10, 11, 13, 12, 17 /* B1_DQ8 */, 15,
            6, /* DQM0 */
            14, /* DQM1 */
        },
        [CHANNEL_D] = {
            0, 1, 2, 3, 5, 16 /* B0_DQ8 */, 7, 4,
            8, 9, 10, 11, 12, 13, 15, 17 /* B1_DQ8 */,
            6, /* DQM0 */
            14 , /* DQM1 */
        },
#endif
    },
#elif (fcFOR_CHIP_ID == fcFigeac)
    [PINMUX_EMCP] = {
        [CHANNEL_A] = {
            1, 0, 3, 4, 7, 16 /* B0_DQ8 */, 6, 2,
            9, 8, 11, 12, 13, 15 , 14, 17/* B1_DQ8 */,
            5, /* DQM0 */
            10, /* DQM1 */
        },
#if (CHANNEL_NUM>1)
        [CHANNEL_B] = {
            1, 0, 3, 5, 7, 6, 16 /* B0_DQ8 */, 2,
            8, 9, 11, 14, 13, 10, 15, 17 /* B1_DQ8 */,
            4, /* DQM0 */
            12, /* DQM1 */
        },
#endif
    },

    [PINMUX_DSC] = {
        [CHANNEL_A] = {
            1, 0, 5, 3, 7, 6, 4, 16 /* B0_DQ8 */,
            9, 8, 11, 15, 12, 10 , 13, 14,
            5, /* DQM0 */
            17/* B1_DQ8 */, /* DQM1 */
        },
#if (CHANNEL_NUM>1)
        [CHANNEL_B] = {
            1, 0, 4, 3, 7, 16 /* B0_DQ8 */, 5, 6 ,
            8, 9, 10, 11, 14, 12, 13, 15 ,
            2, /* DQM0 */
            17/* B1_DQ8 */, /* DQM1 */
        },
#endif
    },
#endif
};

static const U8 uiLPDDR5_MRR_DRAM_Pinmux[PINMUX_MAX][CHANNEL_NUM][DQ_DATA_WIDTH] = {
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

#if RDDQC_PINMUX_WORKAROUND
void RDDQCPinmuxWorkaround(DRAMC_CTX_T *p)
{
    U8 *uiLPDDR_RDDQC_Mapping;
#if SUPPORT_TYPE_LPDDR5
    const U8 uiLPDDR5_RDDQC_Mapping_POP[CHANNEL_NUM][16] =
    {
        {
            8, 9, 10, 11, 12, 15, 14, 13,
            0, 1, 2, 3, 4, 7, 6, 5,
        },
        #if (CHANNEL_NUM>1)
        {
            8, 9, 10, 11, 12, 15, 14, 13,
            0, 1, 2, 3, 4, 7, 6, 5,
        },
        #endif
    };

#endif
#if SUPPORT_TYPE_LPDDR4
    const U8 uiLPDDR4_RDDQC_Mapping_POP[PINMUX_MAX][CHANNEL_NUM][16] =
    {
        {
        // for DSC
            //CH-A
            {
                0, 1, 6, 7, 4, 5, 3, 2,
                9, 8, 11, 10, 15, 14, 12, 13
            },
            #if (CHANNEL_NUM>1)
            //CH-B
            {
                1, 0, 5, 4, 7, 2, 3, 6,
                8, 9, 11, 10, 12, 14, 13, 15
            },
            #endif
            #if (CHANNEL_NUM>2)
            //CH-C
            {
                0, 1, 6, 7, 4, 5, 3, 2,
                9, 8, 11, 10, 15, 14, 12, 13
            },
            //CH-D
            {
                1, 0, 5, 4, 7, 2, 3, 6,
                8, 9, 11, 10, 12, 14, 13, 15
            },
            #endif
        },
        {
        // for LPBK
            // TODO: need porting
        },
        {
        // for EMCP
            //CH-A
            {
                1, 0, 3, 2, 4, 7, 6, 5,
                8, 9, 10, 12, 15, 14, 11, 13
            },
            #if (CHANNEL_NUM>1)
            //CH-B
            {
                0, 1, 7, 4, 2, 5, 6, 3,
                9, 8, 10, 12, 11, 14, 13, 15
            },
            #endif
            #if (CHANNEL_NUM>2)
            //CH-C
            {
                1, 0, 3, 2, 4, 7, 6, 5,
                8, 9, 10, 12, 15, 14, 11, 13
            },
            //CH-D
            {
                0, 1, 7, 4, 2, 5, 6, 3,
                9, 8, 10, 12, 11, 14, 13, 15
            },
            #endif
        }
    };
#endif

    #if SUPPORT_TYPE_LPDDR5
    if (is_lp5_family(p))
    {
        uiLPDDR_RDDQC_Mapping = (U8 *)uiLPDDR5_RDDQC_Mapping_POP[p->channel];
    }
    else
    #endif
    #if SUPPORT_TYPE_LPDDR4
    if (is_lp4_family(p))
    {
        uiLPDDR_RDDQC_Mapping = (U8 *)uiLPDDR4_RDDQC_Mapping_POP[p->DRAMPinmux][p->channel];
    }
    else
    #endif
    {
        /* yr: avoid build error */
        ASSERT(0);
    }

    //Set RDDQC pinmux
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_MRR_PINMUX_CTRL0), P_Fld(uiLPDDR_RDDQC_Mapping[0], MISC_MRR_PINMUX_CTRL0_MRR_PINMUX_SEL_DQ0)
                                                                        | P_Fld(uiLPDDR_RDDQC_Mapping[1], MISC_MRR_PINMUX_CTRL0_MRR_PINMUX_SEL_DQ1)
                                                                        | P_Fld(uiLPDDR_RDDQC_Mapping[2], MISC_MRR_PINMUX_CTRL0_MRR_PINMUX_SEL_DQ2)
                                                                        | P_Fld(uiLPDDR_RDDQC_Mapping[3], MISC_MRR_PINMUX_CTRL0_MRR_PINMUX_SEL_DQ3)
                                                                        | P_Fld(uiLPDDR_RDDQC_Mapping[4], MISC_MRR_PINMUX_CTRL0_MRR_PINMUX_SEL_DQ4)
                                                                        | P_Fld(uiLPDDR_RDDQC_Mapping[5], MISC_MRR_PINMUX_CTRL0_MRR_PINMUX_SEL_DQ5)
                                                                        | P_Fld(uiLPDDR_RDDQC_Mapping[6], MISC_MRR_PINMUX_CTRL0_MRR_PINMUX_SEL_DQ6)
                                                                        | P_Fld(uiLPDDR_RDDQC_Mapping[7], MISC_MRR_PINMUX_CTRL0_MRR_PINMUX_SEL_DQ7));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_MRR_PINMUX_CTRL1), P_Fld(uiLPDDR_RDDQC_Mapping[8], MISC_MRR_PINMUX_CTRL1_MRR_PINMUX_SEL_DQ8)
                                                                        | P_Fld(uiLPDDR_RDDQC_Mapping[9], MISC_MRR_PINMUX_CTRL1_MRR_PINMUX_SEL_DQ9)
                                                                        | P_Fld(uiLPDDR_RDDQC_Mapping[10], MISC_MRR_PINMUX_CTRL1_MRR_PINMUX_SEL_DQ10)
                                                                        | P_Fld(uiLPDDR_RDDQC_Mapping[11], MISC_MRR_PINMUX_CTRL1_MRR_PINMUX_SEL_DQ11)
                                                                        | P_Fld(uiLPDDR_RDDQC_Mapping[12], MISC_MRR_PINMUX_CTRL1_MRR_PINMUX_SEL_DQ12)
                                                                        | P_Fld(uiLPDDR_RDDQC_Mapping[13], MISC_MRR_PINMUX_CTRL1_MRR_PINMUX_SEL_DQ13)
                                                                        | P_Fld(uiLPDDR_RDDQC_Mapping[14], MISC_MRR_PINMUX_CTRL1_MRR_PINMUX_SEL_DQ14)
                                                                        | P_Fld(uiLPDDR_RDDQC_Mapping[15], MISC_MRR_PINMUX_CTRL1_MRR_PINMUX_SEL_DQ15));

}
#endif

void Set_DRAM_Pinmux_Sel(DRAMC_CTX_T *p)
{
    mcSHOW_DBG_MSG2("[Set_DRAM_Pinmux_Sel] DRAMPinmux = %d\n", p->DRAMPinmux);

#if SUPPORT_TYPE_LPDDR4
    if (is_lp4_family(p)) {
        uiLPDDR_MRR_Mapping_POP = uiLPDDR4_MRR_DRAM_Pinmux[p->DRAMPinmux];
        uiLPDDR_O1_Mapping_POP = uiLPDDR4_O1_DRAM_Pinmux[p->DRAMPinmux];
        uiLPDDR_PAD_TA2_Mapping_POP = uiLPDDR4_PAD_TA2_Pinmux[p->DRAMPinmux];
    #if (CA_PER_BIT_DELAY_CELL || PINMUX_AUTO_TEST_PER_BIT_CA)
        uiLPDDR4_CA_Mapping_POP = uiLPDDR4_CA_DRAM_Pinmux[p->DRAMPinmux];
    #endif
    } else
#endif
#if SUPPORT_TYPE_LPDDR5
    if (is_lp5_family(p)) {
        uiLPDDR_MRR_Mapping_POP = uiLPDDR5_MRR_DRAM_Pinmux[p->DRAMPinmux];
        uiLPDDR_O1_Mapping_POP = uiLPDDR5_O1_DRAM_Pinmux[p->DRAMPinmux];
        uiLPDDR_PAD_TA2_Mapping_POP = uiLPDDR5_PAD_TA2_Pinmux[p->DRAMPinmux];
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

