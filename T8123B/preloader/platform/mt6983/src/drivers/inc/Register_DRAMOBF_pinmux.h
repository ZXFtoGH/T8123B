#ifndef __DRAMOBF_CH0_REGS_H__
#define __DRAMOBF_CH0_REGS_H__

//#define DRAMOBF_CH0_BASE_ADDRESS 0x10232000
#define  RT_DRAMOBF_CH0    REG_TYPE_DRAMOBF //Select correct REG_TYPE_T

#define DRAMOBF_CH0_REG_REG_0000_DRAMOBF                                        Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00000)
    #define REG_0000_DRAMOBF_REG_DRAMOBF_BYPASS_EN                              Fld(0, 0, 0, 1, 15) //[15:15]
    #define REG_0000_DRAMOBF_REG_DRAMOBF_TRNG_SECURE_EN                         Fld(0, 0, 0, 1, 14) //[14:14]
    #define REG_0000_DRAMOBF_REG_DRAMOBF_CKG_EN                                 Fld(0, 0, 0, 1, 13) //[13:13]
    #define REG_0000_DRAMOBF_REG_DRAMOBF_KEY_EQUAL                              Fld(0, 0, 0, 1, 6) //[6:6]
    #define REG_0000_DRAMOBF_REG_DRAMOBF_SECURE_RANGE_CONSTANT_KEY              Fld(0, 0, 0, 1, 5) //[5:5]
    #define REG_0000_DRAMOBF_REG_DRAMOBF_NON_SECURE_RANGE_CONSTANT_KEY          Fld(0, 0, 0, 1, 4) //[4:4]
    #define REG_0000_DRAMOBF_REG_DRAMOBF_SECURE_RANGE_15_BYPASS_EN              Fld(0, 0, 0, 1, 2) //[2:2]
    #define REG_0000_DRAMOBF_REG_DRAMOBF_BYPASS_MASK                            Fld(0, 0, 0, 1, 1) //[1:1]
    #define REG_0000_DRAMOBF_REG_DRAMOBF_ENABLE                                 Fld(0, 0, 0, 1, 0) //[0:0]

#define DRAMOBF_CH0_REG_REG_0004_DRAMOBF                                        Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00004)
    #define REG_0004_DRAMOBF_REG_SW_NONCE_READ_MASK                             Fld(0, 0, 0, 1, 0) //[0:0]

#define DRAMOBF_CH0_REG_REG_0008_DRAMOBF                                        Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00008)
    #define REG_0008_DRAMOBF_REG_LATCH_NEW_SR_NONCE_0                           Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_000C_DRAMOBF                                        Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x0000C)
    #define REG_000C_DRAMOBF_REG_LATCH_NEW_SR_NONCE_1                           Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0010_DRAMOBF                                        Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00010)
    #define REG_0010_DRAMOBF_REG_SW_NONCE_A_0                                   Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0014_DRAMOBF                                        Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00014)
    #define REG_0014_DRAMOBF_REG_SW_NONCE_A_1                                   Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0018_DRAMOBF                                        Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00018)
    #define REG_0018_DRAMOBF_REG_SW_NONCE_A_2                                   Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_001C_DRAMOBF                                        Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x0001C)
    #define REG_001C_DRAMOBF_REG_SW_NONCE_A_3                                   Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0020_DRAMOBF                                        Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00020)
    #define REG_0020_DRAMOBF_REG_SW_NONCE_B_0                                   Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0024_DRAMOBF                                        Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00024)
    #define REG_0024_DRAMOBF_REG_SW_NONCE_B_1                                   Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0028_DRAMOBF                                        Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00028)
    #define REG_0028_DRAMOBF_REG_SW_NONCE_B_2                                   Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_002C_DRAMOBF                                        Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x0002C)
    #define REG_002C_DRAMOBF_REG_SW_NONCE_B_3                                   Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0030_DRAMOBF                                        Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00030)
    #define REG_0030_DRAMOBF_REG_LATCH_NEW_SR_NONCE_1                           Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0040_DRAMOBF                                        Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00040)
    #define REG_0040_DRAMOBF_REG_SR8_NONCE_SEL                                  Fld(0, 0, 0, 2, 14) //[15:14]
    #define REG_0040_DRAMOBF_REG_SR7_NONCE_SEL                                  Fld(0, 0, 0, 2, 12) //[13:12]
    #define REG_0040_DRAMOBF_REG_SR6_NONCE_SEL                                  Fld(0, 0, 0, 2, 10) //[11:10]
    #define REG_0040_DRAMOBF_REG_SR5_NONCE_SEL                                  Fld(0, 0, 0, 2, 8) //[9:8]
    #define REG_0040_DRAMOBF_REG_SR4_NONCE_SEL                                  Fld(0, 0, 0, 2, 6) //[7:6]
    #define REG_0040_DRAMOBF_REG_SR3_NONCE_SEL                                  Fld(0, 0, 0, 2, 4) //[5:4]
    #define REG_0040_DRAMOBF_REG_SR2_NONCE_SEL                                  Fld(0, 0, 0, 2, 2) //[3:2]
    #define REG_0040_DRAMOBF_REG_SR1_NONCE_SEL                                  Fld(0, 0, 0, 2, 0) //[1:0]

#define DRAMOBF_CH0_REG_REG_0044_DRAMOBF                                        Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00044)
    #define REG_0044_DRAMOBF_REG_SR16_NONCE_SEL                                 Fld(0, 0, 0, 2, 14) //[15:14]
    #define REG_0044_DRAMOBF_REG_SR15_NONCE_SEL                                 Fld(0, 0, 0, 2, 12) //[13:12]
    #define REG_0044_DRAMOBF_REG_SR14_NONCE_SEL                                 Fld(0, 0, 0, 2, 10) //[11:10]
    #define REG_0044_DRAMOBF_REG_SR13_NONCE_SEL                                 Fld(0, 0, 0, 2, 8) //[9:8]
    #define REG_0044_DRAMOBF_REG_SR12_NONCE_SEL                                 Fld(0, 0, 0, 2, 6) //[7:6]
    #define REG_0044_DRAMOBF_REG_SR11_NONCE_SEL                                 Fld(0, 0, 0, 2, 4) //[5:4]
    #define REG_0044_DRAMOBF_REG_SR10_NONCE_SEL                                 Fld(0, 0, 0, 2, 2) //[3:2]
    #define REG_0044_DRAMOBF_REG_SR9_NONCE_SEL                                  Fld(0, 0, 0, 2, 0) //[1:0]

#define DRAMOBF_CH0_REG_REG_0048_DRAMOBF                                        Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00048)
    #define REG_0048_DRAMOBF_REG_SR24_NONCE_SEL                                 Fld(0, 0, 0, 2, 14) //[15:14]
    #define REG_0048_DRAMOBF_REG_SR23_NONCE_SEL                                 Fld(0, 0, 0, 2, 12) //[13:12]
    #define REG_0048_DRAMOBF_REG_SR22_NONCE_SEL                                 Fld(0, 0, 0, 2, 10) //[11:10]
    #define REG_0048_DRAMOBF_REG_SR21_NONCE_SEL                                 Fld(0, 0, 0, 2, 8) //[9:8]
    #define REG_0048_DRAMOBF_REG_SR20_NONCE_SEL                                 Fld(0, 0, 0, 2, 6) //[7:6]
    #define REG_0048_DRAMOBF_REG_SR19_NONCE_SEL                                 Fld(0, 0, 0, 2, 4) //[5:4]
    #define REG_0048_DRAMOBF_REG_SR18_NONCE_SEL                                 Fld(0, 0, 0, 2, 2) //[3:2]
    #define REG_0048_DRAMOBF_REG_SR17_NONCE_SEL                                 Fld(0, 0, 0, 2, 0) //[1:0]

#define DRAMOBF_CH0_REG_REG_004C_DRAMOBF                                        Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x0004C)
    #define REG_004C_DRAMOBF_REG_SR32_NONCE_SEL                                 Fld(0, 0, 0, 2, 14) //[15:14]
    #define REG_004C_DRAMOBF_REG_SR31_NONCE_SEL                                 Fld(0, 0, 0, 2, 12) //[13:12]
    #define REG_004C_DRAMOBF_REG_SR30_NONCE_SEL                                 Fld(0, 0, 0, 2, 10) //[11:10]
    #define REG_004C_DRAMOBF_REG_SR29_NONCE_SEL                                 Fld(0, 0, 0, 2, 8) //[9:8]
    #define REG_004C_DRAMOBF_REG_SR28_NONCE_SEL                                 Fld(0, 0, 0, 2, 6) //[7:6]
    #define REG_004C_DRAMOBF_REG_SR27_NONCE_SEL                                 Fld(0, 0, 0, 2, 4) //[5:4]
    #define REG_004C_DRAMOBF_REG_SR26_NONCE_SEL                                 Fld(0, 0, 0, 2, 2) //[3:2]
    #define REG_004C_DRAMOBF_REG_SR25_NONCE_SEL                                 Fld(0, 0, 0, 2, 0) //[1:0]

#define DRAMOBF_CH0_REG_REG_0050_DRAMOBF                                        Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00050)
    #define REG_0050_DRAMOBF_REG_SR40_NONCE_SEL                                 Fld(0, 0, 0, 2, 14) //[15:14]
    #define REG_0050_DRAMOBF_REG_SR39_NONCE_SEL                                 Fld(0, 0, 0, 2, 12) //[13:12]
    #define REG_0050_DRAMOBF_REG_SR38_NONCE_SEL                                 Fld(0, 0, 0, 2, 10) //[11:10]
    #define REG_0050_DRAMOBF_REG_SR37_NONCE_SEL                                 Fld(0, 0, 0, 2, 8) //[9:8]
    #define REG_0050_DRAMOBF_REG_SR36_NONCE_SEL                                 Fld(0, 0, 0, 2, 6) //[7:6]
    #define REG_0050_DRAMOBF_REG_SR35_NONCE_SEL                                 Fld(0, 0, 0, 2, 4) //[5:4]
    #define REG_0050_DRAMOBF_REG_SR34_NONCE_SEL                                 Fld(0, 0, 0, 2, 2) //[3:2]
    #define REG_0050_DRAMOBF_REG_SR33_NONCE_SEL                                 Fld(0, 0, 0, 2, 0) //[1:0]

#define DRAMOBF_CH0_REG_REG_0054_DRAMOBF                                        Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00054)
    #define REG_0054_DRAMOBF_REG_SR48_NONCE_SEL                                 Fld(0, 0, 0, 2, 14) //[15:14]
    #define REG_0054_DRAMOBF_REG_SR47_NONCE_SEL                                 Fld(0, 0, 0, 2, 12) //[13:12]
    #define REG_0054_DRAMOBF_REG_SR46_NONCE_SEL                                 Fld(0, 0, 0, 2, 10) //[11:10]
    #define REG_0054_DRAMOBF_REG_SR45_NONCE_SEL                                 Fld(0, 0, 0, 2, 8) //[9:8]
    #define REG_0054_DRAMOBF_REG_SR44_NONCE_SEL                                 Fld(0, 0, 0, 2, 6) //[7:6]
    #define REG_0054_DRAMOBF_REG_SR43_NONCE_SEL                                 Fld(0, 0, 0, 2, 4) //[5:4]
    #define REG_0054_DRAMOBF_REG_SR42_NONCE_SEL                                 Fld(0, 0, 0, 2, 2) //[3:2]
    #define REG_0054_DRAMOBF_REG_SR41_NONCE_SEL                                 Fld(0, 0, 0, 2, 0) //[1:0]

#define DRAMOBF_CH0_REG_REG_0080_DRAMOBF                                        Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00080)
    #define REG_0080_DRAMOBF_REG_SR2_NONCE_VALID_CHN_0                          Fld(0, 0, 0, 1, 8) //[8:8]
    #define REG_0080_DRAMOBF_REG_SR1_NONCE_VALID_CHN_0                          Fld(0, 0, 0, 1, 0) //[0:0]

#define DRAMOBF_CH0_REG_REG_0084_DRAMOBF                                        Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00084)
    #define REG_0084_DRAMOBF_REG_SR4_NONCE_VALID_CHN_0                          Fld(0, 0, 0, 1, 8) //[8:8]
    #define REG_0084_DRAMOBF_REG_SR3_NONCE_VALID_CHN_0                          Fld(0, 0, 0, 1, 0) //[0:0]

#define DRAMOBF_CH0_REG_REG_0088_DRAMOBF                                        Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00088)
    #define REG_0088_DRAMOBF_REG_SR6_NONCE_VALID_CHN_0                          Fld(0, 0, 0, 1, 8) //[8:8]
    #define REG_0088_DRAMOBF_REG_SR5_NONCE_VALID_CHN_0                          Fld(0, 0, 0, 1, 0) //[0:0]

#define DRAMOBF_CH0_REG_REG_008C_DRAMOBF                                        Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x0008C)
    #define REG_008C_DRAMOBF_REG_SR8_NONCE_VALID_CHN_0                          Fld(0, 0, 0, 1, 8) //[8:8]
    #define REG_008C_DRAMOBF_REG_SR7_NONCE_VALID_CHN_0                          Fld(0, 0, 0, 1, 0) //[0:0]

#define DRAMOBF_CH0_REG_REG_0090_DRAMOBF                                        Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00090)
    #define REG_0090_DRAMOBF_REG_SR10_NONCE_VALID_CHN_0                         Fld(0, 0, 0, 1, 8) //[8:8]
    #define REG_0090_DRAMOBF_REG_SR9_NONCE_VALID_CHN_0                          Fld(0, 0, 0, 1, 0) //[0:0]

#define DRAMOBF_CH0_REG_REG_0094_DRAMOBF                                        Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00094)
    #define REG_0094_DRAMOBF_REG_SR12_NONCE_VALID_CHN_0                         Fld(0, 0, 0, 1, 8) //[8:8]
    #define REG_0094_DRAMOBF_REG_SR11_NONCE_VALID_CHN_0                         Fld(0, 0, 0, 1, 0) //[0:0]

#define DRAMOBF_CH0_REG_REG_0098_DRAMOBF                                        Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00098)
    #define REG_0098_DRAMOBF_REG_SR14_NONCE_VALID_CHN_0                         Fld(0, 0, 0, 1, 8) //[8:8]
    #define REG_0098_DRAMOBF_REG_SR13_NONCE_VALID_CHN_0                         Fld(0, 0, 0, 1, 0) //[0:0]

#define DRAMOBF_CH0_REG_REG_009C_DRAMOBF                                        Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x0009C)
    #define REG_009C_DRAMOBF_REG_SR16_NONCE_VALID_CHN_0                         Fld(0, 0, 0, 1, 8) //[8:8]
    #define REG_009C_DRAMOBF_REG_SR15_NONCE_VALID_CHN_0                         Fld(0, 0, 0, 1, 0) //[0:0]

#define DRAMOBF_CH0_REG_REG_00A0_DRAMOBF                                        Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x000A0)
    #define REG_00A0_DRAMOBF_REG_SR18_NONCE_VALID_CHN_0                         Fld(0, 0, 0, 1, 8) //[8:8]
    #define REG_00A0_DRAMOBF_REG_SR17_NONCE_VALID_CHN_0                         Fld(0, 0, 0, 1, 0) //[0:0]

#define DRAMOBF_CH0_REG_REG_00A4_DRAMOBF                                        Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x000A4)
    #define REG_00A4_DRAMOBF_REG_SR20_NONCE_VALID_CHN_0                         Fld(0, 0, 0, 1, 8) //[8:8]
    #define REG_00A4_DRAMOBF_REG_SR19_NONCE_VALID_CHN_0                         Fld(0, 0, 0, 1, 0) //[0:0]

#define DRAMOBF_CH0_REG_REG_00A8_DRAMOBF                                        Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x000A8)
    #define REG_00A8_DRAMOBF_REG_SR22_NONCE_VALID_CHN_0                         Fld(0, 0, 0, 1, 8) //[8:8]
    #define REG_00A8_DRAMOBF_REG_SR21_NONCE_VALID_CHN_0                         Fld(0, 0, 0, 1, 0) //[0:0]

#define DRAMOBF_CH0_REG_REG_00AC_DRAMOBF                                        Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x000AC)
    #define REG_00AC_DRAMOBF_REG_SR24_NONCE_VALID_CHN_0                         Fld(0, 0, 0, 1, 8) //[8:8]
    #define REG_00AC_DRAMOBF_REG_SR23_NONCE_VALID_CHN_0                         Fld(0, 0, 0, 1, 0) //[0:0]

#define DRAMOBF_CH0_REG_REG_00B0_DRAMOBF                                        Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x000B0)
    #define REG_00B0_DRAMOBF_REG_SR26_NONCE_VALID_CHN_0                         Fld(0, 0, 0, 1, 8) //[8:8]
    #define REG_00B0_DRAMOBF_REG_SR25_NONCE_VALID_CHN_0                         Fld(0, 0, 0, 1, 0) //[0:0]

#define DRAMOBF_CH0_REG_REG_00B4_DRAMOBF                                        Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x000B4)
    #define REG_00B4_DRAMOBF_REG_SR28_NONCE_VALID_CHN_0                         Fld(0, 0, 0, 1, 8) //[8:8]
    #define REG_00B4_DRAMOBF_REG_SR27_NONCE_VALID_CHN_0                         Fld(0, 0, 0, 1, 0) //[0:0]

#define DRAMOBF_CH0_REG_REG_00B8_DRAMOBF                                        Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x000B8)
    #define REG_00B8_DRAMOBF_REG_SR30_NONCE_VALID_CHN_0                         Fld(0, 0, 0, 1, 8) //[8:8]
    #define REG_00B8_DRAMOBF_REG_SR29_NONCE_VALID_CHN_0                         Fld(0, 0, 0, 1, 0) //[0:0]

#define DRAMOBF_CH0_REG_REG_00BC_DRAMOBF                                        Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x000BC)
    #define REG_00BC_DRAMOBF_REG_SR32_NONCE_VALID_CHN_0                         Fld(0, 0, 0, 1, 8) //[8:8]
    #define REG_00BC_DRAMOBF_REG_SR31_NONCE_VALID_CHN_0                         Fld(0, 0, 0, 1, 0) //[0:0]

#define DRAMOBF_CH0_REG_REG_00C0_DRAMOBF                                        Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x000C0)
    #define REG_00C0_DRAMOBF_REG_KEY_VALID_FROM_PM_CHN_0                        Fld(0, 0, 0, 1, 8) //[8:8]
    #define REG_00C0_DRAMOBF_REG_DRAMOBF_STATUS_CHN_0                           Fld(0, 0, 0, 1, 0) //[0:0]

#define DRAMOBF_CH0_REG_REG_00C4_DRAMOBF                                        Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x000C4)
    #define REG_00C4_DRAMOBF_REG_RN_VALID_CHN_0                                 Fld(0, 0, 0, 1, 8) //[8:8]
    #define REG_00C4_DRAMOBF_REG_ROOT_KEY_VALID_CHN_0                           Fld(0, 0, 0, 1, 0) //[0:0]

#define DRAMOBF_CH0_REG_REG_00C8_DRAMOBF                                        Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x000C8)
    #define REG_00C8_DRAMOBF_REG_SEC_OS_FW_VALID_CHN_0                          Fld(0, 0, 0, 1, 8) //[8:8]
    #define REG_00C8_DRAMOBF_REG_NSR_NONCE_VALID_CHN_0                          Fld(0, 0, 0, 1, 0) //[0:0]

#define DRAMOBF_CH0_REG_REG_00CC_DRAMOBF                                        Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x000CC)
    #define REG_00CC_DRAMOBF_REG_RNG_SRC1_RDY_CHN_0                             Fld(0, 0, 0, 1, 8) //[8:8]
    #define REG_00CC_DRAMOBF_REG_RNG_STATUS_CHN_0                               Fld(0, 0, 0, 1, 0) //[0:0]

#define DRAMOBF_CH0_REG_REG_00D0_DRAMOBF                                        Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x000D0)
    #define REG_00D0_DRAMOBF_REG_RNG_SRC3_RDY_CHN_0                             Fld(0, 0, 0, 1, 8) //[8:8]
    #define REG_00D0_DRAMOBF_REG_RNG_SRC2_RDY_CHN_0                             Fld(0, 0, 0, 1, 0) //[0:0]

#define DRAMOBF_CH0_REG_REG_00D4_DRAMOBF                                        Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x000D4)
    #define REG_00D4_DRAMOBF_REG_RNG_RC_RDY_CHN_0                               Fld(0, 0, 0, 1, 8) //[8:8]
    #define REG_00D4_DRAMOBF_REG_RNG_SRC4_RDY_CHN_0                             Fld(0, 0, 0, 1, 0) //[0:0]

#define DRAMOBF_CH0_REG_REG_00D8_DRAMOBF                                        Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x000D8)
    #define REG_00D8_DRAMOBF_REG_RNG_SW_RST_CHN_0                               Fld(0, 0, 0, 1, 8) //[8:8]
    #define REG_00D8_DRAMOBF_REG_RNG_IP_RDY_CHN_0                               Fld(0, 0, 0, 1, 0) //[0:0]

#define DRAMOBF_CH0_REG_REG_0100_DRAMOBF                                        Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00100)
    #define REG_0100_DRAMOBF_REG_CUR_ST_CHN_0                                   Fld(0, 0, 0, 7, 8) //[14:8]

#define DRAMOBF_CH0_REG_REG_0104_DRAMOBF                                        Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00104)
    #define REG_0104_DRAMOBF_REG_STR_KEY_CNT_CHN_0                              Fld(0, 0, 0, 7, 0) //[6:0]

#define DRAMOBF_CH0_REG_REG_0108_DRAMOBF                                        Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00108)
    #define REG_0108_DRAMOBF_REG_DUMP_MODE_CHN_0                                Fld(0, 0, 0, 8, 8) //[15:8]
    #define REG_0108_DRAMOBF_REG_DUMP_MODE_LOCK_CHN_0                           Fld(0, 0, 0, 1, 7) //[7:7]
    #define REG_0108_DRAMOBF_REG_DUMP_MODE_OUT_READ_MASK_CHN_0                  Fld(0, 0, 0, 1, 2) //[2:2]
    #define REG_0108_DRAMOBF_REG_DUMP_MODE_ENABLE_CHN_0                         Fld(0, 0, 0, 1, 1) //[1:1]
    #define REG_0108_DRAMOBF_REG_DBG_RNG_VALID_CHN_0                            Fld(0, 0, 0, 1, 0) //[0:0]

#define DRAMOBF_CH0_REG_REG_010C_DRAMOBF                                        Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x0010C)
    #define REG_010C_DRAMOBF_REG_DUMP_MODE_OUT_CHN_0                            Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0180_DRAMOBF                                        Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00180)
    #define REG_0180_DRAMOBF_REG_SR34_NONCE_VALID_CHN_0                         Fld(0, 0, 0, 1, 8) //[8:8]
    #define REG_0180_DRAMOBF_REG_SR33_NONCE_VALID_CHN_0                         Fld(0, 0, 0, 1, 0) //[0:0]

#define DRAMOBF_CH0_REG_REG_0184_DRAMOBF                                        Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00184)
    #define REG_0184_DRAMOBF_REG_SR36_NONCE_VALID_CHN_0                         Fld(0, 0, 0, 1, 8) //[8:8]
    #define REG_0184_DRAMOBF_REG_SR35_NONCE_VALID_CHN_0                         Fld(0, 0, 0, 1, 0) //[0:0]

#define DRAMOBF_CH0_REG_REG_0188_DRAMOBF                                        Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00188)
    #define REG_0188_DRAMOBF_REG_SR38_NONCE_VALID_CHN_0                         Fld(0, 0, 0, 1, 8) //[8:8]
    #define REG_0188_DRAMOBF_REG_SR37_NONCE_VALID_CHN_0                         Fld(0, 0, 0, 1, 0) //[0:0]

#define DRAMOBF_CH0_REG_REG_018C_DRAMOBF                                        Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x0018C)
    #define REG_018C_DRAMOBF_REG_SR40_NONCE_VALID_CHN_0                         Fld(0, 0, 0, 1, 8) //[8:8]
    #define REG_018C_DRAMOBF_REG_SR39_NONCE_VALID_CHN_0                         Fld(0, 0, 0, 1, 0) //[0:0]

#define DRAMOBF_CH0_REG_REG_0190_DRAMOBF                                        Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00190)
    #define REG_0190_DRAMOBF_REG_SR42_NONCE_VALID_CHN_0                         Fld(0, 0, 0, 1, 8) //[8:8]
    #define REG_0190_DRAMOBF_REG_SR41_NONCE_VALID_CHN_0                         Fld(0, 0, 0, 1, 0) //[0:0]

#define DRAMOBF_CH0_REG_REG_0194_DRAMOBF                                        Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00194)
    #define REG_0194_DRAMOBF_REG_SR44_NONCE_VALID_CHN_0                         Fld(0, 0, 0, 1, 8) //[8:8]
    #define REG_0194_DRAMOBF_REG_SR43_NONCE_VALID_CHN_0                         Fld(0, 0, 0, 1, 0) //[0:0]

#define DRAMOBF_CH0_REG_REG_0198_DRAMOBF                                        Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00198)
    #define REG_0198_DRAMOBF_REG_SR46_NONCE_VALID_CHN_0                         Fld(0, 0, 0, 1, 8) //[8:8]
    #define REG_0198_DRAMOBF_REG_SR45_NONCE_VALID_CHN_0                         Fld(0, 0, 0, 1, 0) //[0:0]

#define DRAMOBF_CH0_REG_REG_019C_DRAMOBF                                        Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x0019C)
    #define REG_019C_DRAMOBF_REG_SR48_NONCE_VALID_CHN_0                         Fld(0, 0, 0, 1, 8) //[8:8]
    #define REG_019C_DRAMOBF_REG_SR47_NONCE_VALID_CHN_0                         Fld(0, 0, 0, 1, 0) //[0:0]

#define DRAMOBF_CH0_REG_REG_01A0_DRAMOBF                                        Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x001A0)
    #define REG_01A0_DRAMOBF_REG_ALL_SR_NONCE_VALID_CHN_0                       Fld(0, 0, 0, 1, 0) //[0:0]

#define DRAMOBF_CH0_REG_REG_01C0_DRAMOBF                                        Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x001C0)
    #define REG_01C0_DRAMOBF_REG_IO_PROTECT_MODE                                Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_01C4_DRAMOBF                                        Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x001C4)
    #define REG_01C4_DRAMOBF_REG_DRAMOBF_RESERVED1                              Fld(0, 0, 0, 8, 0) //[7:0]

#define DRAMOBF_CH0_REG_REG_01C8_DRAMOBF                                        Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x001C8)
    #define REG_01C8_DRAMOBF_REG_DRAMOBF_RESERVED2                              Fld(0, 0, 0, 8, 0) //[7:0]

#define DRAMOBF_CH0_REG_REG_01CC_DRAMOBF                                        Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x001CC)
    #define REG_01CC_DRAMOBF_REG_DRAMOBF_RESERVED3                              Fld(0, 0, 0, 8, 0) //[7:0]

#define DRAMOBF_CH0_REG_REG_01D0_DRAMOBF                                        Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x001D0)
    #define REG_01D0_DRAMOBF_REG_DRAMOBF_RESERVED4                              Fld(0, 0, 0, 8, 0) //[7:0]

#define DRAMOBF_CH0_REG_REG_01D4_DRAMOBF                                        Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x001D4)
    #define REG_01D4_DRAMOBF_REG_DRAMOBF_RESERVED5                              Fld(0, 0, 0, 8, 0) //[7:0]

#define DRAMOBF_CH0_REG_REG_01D8_DRAMOBF                                        Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x001D8)
    #define REG_01D8_DRAMOBF_REG_DRAMOBF_RESERVED6                              Fld(0, 0, 0, 8, 0) //[7:0]

#define DRAMOBF_CH0_REG_REG_01E0_DRAMOBF                                        Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x001E0)
    #define REG_01E0_DRAMOBF_REG_TEE_DRAMOBF_RESERVED1                          Fld(0, 0, 0, 8, 0) //[7:0]

#define DRAMOBF_CH0_REG_REG_01E4_DRAMOBF                                        Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x001E4)
    #define REG_01E4_DRAMOBF_REG_TEE_DRAMOBF_RESERVED2                          Fld(0, 0, 0, 8, 0) //[7:0]

#define DRAMOBF_CH0_REG_REG_01E8_DRAMOBF                                        Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x001E8)
    #define REG_01E8_DRAMOBF_REG_TEE_DRAMOBF_RESERVED3                          Fld(0, 0, 0, 8, 0) //[7:0]

#define DRAMOBF_CH0_REG_REG_01EC_DRAMOBF                                        Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x001EC)
    #define REG_01EC_DRAMOBF_REG_TEE_DRAMOBF_RESERVED4                          Fld(0, 0, 0, 8, 0) //[7:0]

#define DRAMOBF_CH0_REG_REG_01F0_DRAMOBF                                        Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x001F0)
    #define REG_01F0_DRAMOBF_REG_TEE_DRAMOBF_RESERVED5                          Fld(0, 0, 0, 8, 0) //[7:0]

#define DRAMOBF_CH0_REG_REG_01F4_DRAMOBF                                        Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x001F4)
    #define REG_01F4_DRAMOBF_REG_TEE_DRAMOBF_RESERVED6                          Fld(0, 0, 0, 8, 0) //[7:0]

#define DRAMOBF_CH0_REG_REG_0200_DRAMOBF_PM                                     Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00200)
    #define REG_0200_DRAMOBF_PM_REG_SW_KEY_PARAM_0                              Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0204_DRAMOBF_PM                                     Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00204)
    #define REG_0204_DRAMOBF_PM_REG_SW_KEY_PARAM_1                              Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0208_DRAMOBF_PM                                     Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00208)
    #define REG_0208_DRAMOBF_PM_REG_SW_KEY_PARAM_2                              Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_020C_DRAMOBF_PM                                     Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x0020C)
    #define REG_020C_DRAMOBF_PM_REG_SW_KEY_PARAM_3                              Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0210_DRAMOBF_PM                                     Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00210)
    #define REG_0210_DRAMOBF_PM_REG_SW_KEY_PARAM_4                              Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0214_DRAMOBF_PM                                     Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00214)
    #define REG_0214_DRAMOBF_PM_REG_SW_KEY_PARAM_5                              Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0218_DRAMOBF_PM                                     Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00218)
    #define REG_0218_DRAMOBF_PM_REG_SW_KEY_PARAM_6                              Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_021C_DRAMOBF_PM                                     Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x0021C)
    #define REG_021C_DRAMOBF_PM_REG_SW_KEY_PARAM_7                              Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0220_DRAMOBF_PM                                     Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00220)
    #define REG_0220_DRAMOBF_PM_REG_SW_KEY_PARAM_8                              Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0224_DRAMOBF_PM                                     Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00224)
    #define REG_0224_DRAMOBF_PM_REG_SW_KEY_PARAM_9                              Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0228_DRAMOBF_PM                                     Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00228)
    #define REG_0228_DRAMOBF_PM_REG_SW_KEY_PARAM_10                             Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_022C_DRAMOBF_PM                                     Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x0022C)
    #define REG_022C_DRAMOBF_PM_REG_SW_KEY_PARAM_11                             Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0230_DRAMOBF_PM                                     Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00230)
    #define REG_0230_DRAMOBF_PM_REG_SW_KEY_PARAM_12                             Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0234_DRAMOBF_PM                                     Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00234)
    #define REG_0234_DRAMOBF_PM_REG_SW_KEY_PARAM_13                             Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0238_DRAMOBF_PM                                     Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00238)
    #define REG_0238_DRAMOBF_PM_REG_SW_KEY_PARAM_14                             Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_023C_DRAMOBF_PM                                     Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x0023C)
    #define REG_023C_DRAMOBF_PM_REG_SW_KEY_PARAM_15                             Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0240_DRAMOBF_PM                                     Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00240)
    #define REG_0240_DRAMOBF_PM_REG_SW_KEY_PARAM_16                             Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0244_DRAMOBF_PM                                     Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00244)
    #define REG_0244_DRAMOBF_PM_REG_SW_KEY_PARAM_17                             Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0248_DRAMOBF_PM                                     Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00248)
    #define REG_0248_DRAMOBF_PM_REG_SW_KEY_PARAM_18                             Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_024C_DRAMOBF_PM                                     Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x0024C)
    #define REG_024C_DRAMOBF_PM_REG_SW_KEY_PARAM_19                             Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0250_DRAMOBF_PM                                     Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00250)
    #define REG_0250_DRAMOBF_PM_REG_SW_KEY_PARAM_20                             Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0254_DRAMOBF_PM                                     Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00254)
    #define REG_0254_DRAMOBF_PM_REG_SW_KEY_PARAM_21                             Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0258_DRAMOBF_PM                                     Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00258)
    #define REG_0258_DRAMOBF_PM_REG_SW_KEY_PARAM_22                             Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_025C_DRAMOBF_PM                                     Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x0025C)
    #define REG_025C_DRAMOBF_PM_REG_SW_KEY_PARAM_23                             Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0260_DRAMOBF_PM                                     Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00260)
    #define REG_0260_DRAMOBF_PM_REG_SW_KEY_PARAM_24                             Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0264_DRAMOBF_PM                                     Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00264)
    #define REG_0264_DRAMOBF_PM_REG_SW_KEY_PARAM_25                             Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0268_DRAMOBF_PM                                     Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00268)
    #define REG_0268_DRAMOBF_PM_REG_SW_KEY_PARAM_26                             Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_026C_DRAMOBF_PM                                     Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x0026C)
    #define REG_026C_DRAMOBF_PM_REG_SW_KEY_PARAM_LOCK                           Fld(0, 0, 0, 1, 15) //[15:15]
    #define REG_026C_DRAMOBF_PM_REG_LATCH_NEW_KEY_PARAM                         Fld(0, 0, 0, 1, 2) //[2:2]
    #define REG_026C_DRAMOBF_PM_REG_SW_KEY_PARAM_SEL                            Fld(0, 0, 0, 1, 1) //[1:1]
    #define REG_026C_DRAMOBF_PM_REG_SW_KEY_PARAM_READ_MASK                      Fld(0, 0, 0, 1, 0) //[0:0]

#define DRAMOBF_CH0_REG_REG_0270_DRAMOBF_PM                                     Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00270)
    #define REG_0270_DRAMOBF_PM_REG_RNG_SW_RST                                  Fld(0, 0, 0, 1, 8) //[8:8]
    #define REG_0270_DRAMOBF_PM_REG_RNG_IP_RDY                                  Fld(0, 0, 0, 1, 7) //[7:7]
    #define REG_0270_DRAMOBF_PM_REG_RNG_RC_RDY                                  Fld(0, 0, 0, 1, 6) //[6:6]
    #define REG_0270_DRAMOBF_PM_REG_RNG_SRC4_RDY                                Fld(0, 0, 0, 1, 5) //[5:5]
    #define REG_0270_DRAMOBF_PM_REG_RNG_SRC3_RDY                                Fld(0, 0, 0, 1, 4) //[4:4]
    #define REG_0270_DRAMOBF_PM_REG_RNG_SRC2_RDY                                Fld(0, 0, 0, 1, 3) //[3:3]
    #define REG_0270_DRAMOBF_PM_REG_RNG_SRC1_RDY                                Fld(0, 0, 0, 1, 2) //[2:2]
    #define REG_0270_DRAMOBF_PM_REG_RNG_STATUS                                  Fld(0, 0, 0, 1, 1) //[1:1]
    #define REG_0270_DRAMOBF_PM_REG_KEY_VALID_IN_PM                             Fld(0, 0, 0, 1, 0) //[0:0]

#define DRAMOBF_CH0_REG_REG_0280_DRAMOBF_PM                                     Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00280)
    #define REG_0280_DRAMOBF_PM_REG_CUR_ST_PM                                   Fld(0, 0, 0, 2, 0) //[1:0]

#define DRAMOBF_CH0_REG_REG_0284_DRAMOBF_PM                                     Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00284)
    #define REG_0284_DRAMOBF_PM_REG_DBG_RNG_VALID_IN_PM                         Fld(0, 0, 0, 1, 0) //[0:0]

#define DRAMOBF_CH0_REG_REG_0400_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00400)
    #define REG_0400_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_ROOTKEY_0        Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0404_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00404)
    #define REG_0404_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_ROOTKEY_1        Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0408_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00408)
    #define REG_0408_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_ROOTKEY_2        Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_040C_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x0040C)
    #define REG_040C_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_ROOTKEY_3        Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0410_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00410)
    #define REG_0410_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_ROOTKEY_4        Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0414_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00414)
    #define REG_0414_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_ROOTKEY_5        Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0418_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00418)
    #define REG_0418_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_ROOTKEY_6        Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_041C_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x0041C)
    #define REG_041C_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_ROOTKEY_7        Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0420_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00420)
    #define REG_0420_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_ROOTKEY_8        Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0424_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00424)
    #define REG_0424_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_ROOTKEY_9        Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0428_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00428)
    #define REG_0428_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_ROOTKEY_10       Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_042C_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x0042C)
    #define REG_042C_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_ROOTKEY_11       Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0430_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00430)
    #define REG_0430_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_ROOTKEY_12       Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0434_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00434)
    #define REG_0434_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_ROOTKEY_13       Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0438_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00438)
    #define REG_0438_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_ROOTKEY_14       Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_043C_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x0043C)
    #define REG_043C_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_ROOTKEY_15       Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0440_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00440)
    #define REG_0440_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_RN_0             Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0444_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00444)
    #define REG_0444_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_RN_1             Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0448_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00448)
    #define REG_0448_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_RN_2             Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0450_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00450)
    #define REG_0450_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_NSR_NONCE_0      Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0454_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00454)
    #define REG_0454_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_NSR_NONCE_1      Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0458_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00458)
    #define REG_0458_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_NSR_NONCE_2      Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_045C_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x0045C)
    #define REG_045C_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_NSR_NONCE_3      Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0460_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00460)
    #define REG_0460_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_SECFW_NONCE_0    Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0464_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00464)
    #define REG_0464_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_SECFW_NONCE_1    Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0468_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00468)
    #define REG_0468_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_SECFW_NONCE_2    Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_046C_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x0046C)
    #define REG_046C_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_SECFW_NONCE_3    Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_047C_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x0047C)
    #define REG_047C_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_RESTORE          Fld(0, 0, 0, 1, 0) //[0:0]

#define DRAMOBF_CH0_REG_REG_0480_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00480)
    #define REG_0480_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_SR1_NONCE_0      Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0484_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00484)
    #define REG_0484_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_SR1_NONCE_1      Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0488_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00488)
    #define REG_0488_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_SR1_NONCE_2      Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_048C_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x0048C)
    #define REG_048C_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_SR1_NONCE_3      Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0490_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00490)
    #define REG_0490_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_SR2_NONCE_0      Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0494_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00494)
    #define REG_0494_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_SR2_NONCE_1      Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0498_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00498)
    #define REG_0498_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_SR2_NONCE_2      Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_049C_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x0049C)
    #define REG_049C_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_SR2_NONCE_3      Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_04A0_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x004A0)
    #define REG_04A0_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_SR3_NONCE_0      Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_04A4_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x004A4)
    #define REG_04A4_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_SR3_NONCE_1      Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_04A8_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x004A8)
    #define REG_04A8_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_SR3_NONCE_2      Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_04AC_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x004AC)
    #define REG_04AC_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_SR3_NONCE_3      Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_04B0_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x004B0)
    #define REG_04B0_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_SR4_NONCE_0      Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_04B4_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x004B4)
    #define REG_04B4_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_SR4_NONCE_1      Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_04B8_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x004B8)
    #define REG_04B8_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_SR4_NONCE_2      Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_04BC_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x004BC)
    #define REG_04BC_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_SR4_NONCE_3      Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_04C0_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x004C0)
    #define REG_04C0_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_SR5_NONCE_0      Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_04C4_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x004C4)
    #define REG_04C4_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_SR5_NONCE_1      Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_04C8_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x004C8)
    #define REG_04C8_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_SR5_NONCE_2      Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_04CC_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x004CC)
    #define REG_04CC_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_SR5_NONCE_3      Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_04D0_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x004D0)
    #define REG_04D0_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_SR6_NONCE_0      Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_04D4_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x004D4)
    #define REG_04D4_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_SR6_NONCE_1      Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_04D8_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x004D8)
    #define REG_04D8_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_SR6_NONCE_2      Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_04DC_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x004DC)
    #define REG_04DC_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_SR6_NONCE_3      Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_04E0_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x004E0)
    #define REG_04E0_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_SR7_NONCE_0      Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_04E4_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x004E4)
    #define REG_04E4_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_SR7_NONCE_1      Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_04E8_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x004E8)
    #define REG_04E8_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_SR7_NONCE_2      Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_04EC_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x004EC)
    #define REG_04EC_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_SR7_NONCE_3      Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_04F0_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x004F0)
    #define REG_04F0_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_SR8_NONCE_0      Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_04F4_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x004F4)
    #define REG_04F4_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_SR8_NONCE_1      Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_04F8_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x004F8)
    #define REG_04F8_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_SR8_NONCE_2      Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_04FC_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x004FC)
    #define REG_04FC_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_SR8_NONCE_3      Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0500_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00500)
    #define REG_0500_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_SR9_NONCE_0      Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0504_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00504)
    #define REG_0504_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_SR9_NONCE_1      Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0508_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00508)
    #define REG_0508_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_SR9_NONCE_2      Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_050C_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x0050C)
    #define REG_050C_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_SR9_NONCE_3      Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0510_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00510)
    #define REG_0510_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_SR10_NONCE_0     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0514_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00514)
    #define REG_0514_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_SR10_NONCE_1     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0518_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00518)
    #define REG_0518_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_SR10_NONCE_2     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_051C_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x0051C)
    #define REG_051C_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_SR10_NONCE_3     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0520_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00520)
    #define REG_0520_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_SR11_NONCE_0     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0524_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00524)
    #define REG_0524_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_SR11_NONCE_1     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0528_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00528)
    #define REG_0528_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_SR11_NONCE_2     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_052C_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x0052C)
    #define REG_052C_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_SR11_NONCE_3     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0530_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00530)
    #define REG_0530_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_SR12_NONCE_0     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0534_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00534)
    #define REG_0534_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_SR12_NONCE_1     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0538_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00538)
    #define REG_0538_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_SR12_NONCE_2     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_053C_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x0053C)
    #define REG_053C_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_SR12_NONCE_3     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0540_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00540)
    #define REG_0540_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_SR13_NONCE_0     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0544_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00544)
    #define REG_0544_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_SR13_NONCE_1     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0548_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00548)
    #define REG_0548_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_SR13_NONCE_2     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_054C_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x0054C)
    #define REG_054C_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_SR13_NONCE_3     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0550_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00550)
    #define REG_0550_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_SR14_NONCE_0     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0554_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00554)
    #define REG_0554_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_SR14_NONCE_1     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0558_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00558)
    #define REG_0558_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_SR14_NONCE_2     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_055C_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x0055C)
    #define REG_055C_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_SR14_NONCE_3     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0560_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00560)
    #define REG_0560_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_SR15_NONCE_0     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0564_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00564)
    #define REG_0564_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_SR15_NONCE_1     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0568_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00568)
    #define REG_0568_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_SR15_NONCE_2     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_056C_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x0056C)
    #define REG_056C_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_SR15_NONCE_3     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0570_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00570)
    #define REG_0570_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_SR16_NONCE_0     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0574_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00574)
    #define REG_0574_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_SR16_NONCE_1     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0578_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00578)
    #define REG_0578_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_SR16_NONCE_2     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_057C_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x0057C)
    #define REG_057C_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_SR16_NONCE_3     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0580_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00580)
    #define REG_0580_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_SR17_NONCE_0     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0584_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00584)
    #define REG_0584_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_SR17_NONCE_1     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0588_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00588)
    #define REG_0588_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_SR17_NONCE_2     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_058C_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x0058C)
    #define REG_058C_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_SR17_NONCE_3     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0590_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00590)
    #define REG_0590_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_SR18_NONCE_0     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0594_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00594)
    #define REG_0594_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_SR18_NONCE_1     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0598_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00598)
    #define REG_0598_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_SR18_NONCE_2     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_059C_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x0059C)
    #define REG_059C_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_SR18_NONCE_3     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_05A0_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x005A0)
    #define REG_05A0_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_SR19_NONCE_0     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_05A4_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x005A4)
    #define REG_05A4_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_SR19_NONCE_1     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_05A8_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x005A8)
    #define REG_05A8_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_SR19_NONCE_2     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_05AC_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x005AC)
    #define REG_05AC_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_SR19_NONCE_3     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_05B0_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x005B0)
    #define REG_05B0_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_SR20_NONCE_0     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_05B4_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x005B4)
    #define REG_05B4_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_SR20_NONCE_1     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_05B8_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x005B8)
    #define REG_05B8_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_SR20_NONCE_2     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_05BC_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x005BC)
    #define REG_05BC_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_SR20_NONCE_3     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_05C0_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x005C0)
    #define REG_05C0_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_SR21_NONCE_0     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_05C4_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x005C4)
    #define REG_05C4_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_SR21_NONCE_1     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_05C8_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x005C8)
    #define REG_05C8_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_SR21_NONCE_2     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_05CC_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x005CC)
    #define REG_05CC_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_SR21_NONCE_3     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_05D0_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x005D0)
    #define REG_05D0_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_SR22_NONCE_0     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_05D4_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x005D4)
    #define REG_05D4_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_SR22_NONCE_1     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_05D8_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x005D8)
    #define REG_05D8_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_SR22_NONCE_2     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_05DC_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x005DC)
    #define REG_05DC_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_SR22_NONCE_3     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_05E0_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x005E0)
    #define REG_05E0_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_SR23_NONCE_0     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_05E4_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x005E4)
    #define REG_05E4_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_SR23_NONCE_1     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_05E8_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x005E8)
    #define REG_05E8_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_SR23_NONCE_2     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_05EC_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x005EC)
    #define REG_05EC_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_SR23_NONCE_3     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_05F0_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x005F0)
    #define REG_05F0_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_SR24_NONCE_0     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_05F4_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x005F4)
    #define REG_05F4_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_SR24_NONCE_1     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_05F8_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x005F8)
    #define REG_05F8_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_SR24_NONCE_2     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_05FC_DRAMOBF_KEY_PARAM_0                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x005FC)
    #define REG_05FC_DRAMOBF_KEY_PARAM_0_REG_DRAMOBF_KEY_PARAM_SR24_NONCE_3     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0600_DRAMOBF_KEY_PARAM_1                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00600)
    #define REG_0600_DRAMOBF_KEY_PARAM_1_REG_DRAMOBF_KEY_PARAM_SR25_NONCE_0     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0604_DRAMOBF_KEY_PARAM_1                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00604)
    #define REG_0604_DRAMOBF_KEY_PARAM_1_REG_DRAMOBF_KEY_PARAM_SR25_NONCE_1     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0608_DRAMOBF_KEY_PARAM_1                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00608)
    #define REG_0608_DRAMOBF_KEY_PARAM_1_REG_DRAMOBF_KEY_PARAM_SR25_NONCE_2     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_060C_DRAMOBF_KEY_PARAM_1                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x0060C)
    #define REG_060C_DRAMOBF_KEY_PARAM_1_REG_DRAMOBF_KEY_PARAM_SR25_NONCE_3     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0610_DRAMOBF_KEY_PARAM_1                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00610)
    #define REG_0610_DRAMOBF_KEY_PARAM_1_REG_DRAMOBF_KEY_PARAM_SR26_NONCE_0     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0614_DRAMOBF_KEY_PARAM_1                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00614)
    #define REG_0614_DRAMOBF_KEY_PARAM_1_REG_DRAMOBF_KEY_PARAM_SR26_NONCE_1     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0618_DRAMOBF_KEY_PARAM_1                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00618)
    #define REG_0618_DRAMOBF_KEY_PARAM_1_REG_DRAMOBF_KEY_PARAM_SR26_NONCE_2     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_061C_DRAMOBF_KEY_PARAM_1                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x0061C)
    #define REG_061C_DRAMOBF_KEY_PARAM_1_REG_DRAMOBF_KEY_PARAM_SR26_NONCE_3     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0620_DRAMOBF_KEY_PARAM_1                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00620)
    #define REG_0620_DRAMOBF_KEY_PARAM_1_REG_DRAMOBF_KEY_PARAM_SR27_NONCE_0     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0624_DRAMOBF_KEY_PARAM_1                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00624)
    #define REG_0624_DRAMOBF_KEY_PARAM_1_REG_DRAMOBF_KEY_PARAM_SR27_NONCE_1     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0628_DRAMOBF_KEY_PARAM_1                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00628)
    #define REG_0628_DRAMOBF_KEY_PARAM_1_REG_DRAMOBF_KEY_PARAM_SR27_NONCE_2     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_062C_DRAMOBF_KEY_PARAM_1                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x0062C)
    #define REG_062C_DRAMOBF_KEY_PARAM_1_REG_DRAMOBF_KEY_PARAM_SR27_NONCE_3     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0630_DRAMOBF_KEY_PARAM_1                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00630)
    #define REG_0630_DRAMOBF_KEY_PARAM_1_REG_DRAMOBF_KEY_PARAM_SR28_NONCE_0     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0634_DRAMOBF_KEY_PARAM_1                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00634)
    #define REG_0634_DRAMOBF_KEY_PARAM_1_REG_DRAMOBF_KEY_PARAM_SR28_NONCE_1     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0638_DRAMOBF_KEY_PARAM_1                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00638)
    #define REG_0638_DRAMOBF_KEY_PARAM_1_REG_DRAMOBF_KEY_PARAM_SR28_NONCE_2     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_063C_DRAMOBF_KEY_PARAM_1                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x0063C)
    #define REG_063C_DRAMOBF_KEY_PARAM_1_REG_DRAMOBF_KEY_PARAM_SR28_NONCE_3     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0640_DRAMOBF_KEY_PARAM_1                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00640)
    #define REG_0640_DRAMOBF_KEY_PARAM_1_REG_DRAMOBF_KEY_PARAM_SR29_NONCE_0     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0644_DRAMOBF_KEY_PARAM_1                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00644)
    #define REG_0644_DRAMOBF_KEY_PARAM_1_REG_DRAMOBF_KEY_PARAM_SR29_NONCE_1     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0648_DRAMOBF_KEY_PARAM_1                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00648)
    #define REG_0648_DRAMOBF_KEY_PARAM_1_REG_DRAMOBF_KEY_PARAM_SR29_NONCE_2     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_064C_DRAMOBF_KEY_PARAM_1                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x0064C)
    #define REG_064C_DRAMOBF_KEY_PARAM_1_REG_DRAMOBF_KEY_PARAM_SR29_NONCE_3     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0650_DRAMOBF_KEY_PARAM_1                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00650)
    #define REG_0650_DRAMOBF_KEY_PARAM_1_REG_DRAMOBF_KEY_PARAM_SR30_NONCE_0     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0654_DRAMOBF_KEY_PARAM_1                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00654)
    #define REG_0654_DRAMOBF_KEY_PARAM_1_REG_DRAMOBF_KEY_PARAM_SR30_NONCE_1     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0658_DRAMOBF_KEY_PARAM_1                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00658)
    #define REG_0658_DRAMOBF_KEY_PARAM_1_REG_DRAMOBF_KEY_PARAM_SR30_NONCE_2     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_065C_DRAMOBF_KEY_PARAM_1                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x0065C)
    #define REG_065C_DRAMOBF_KEY_PARAM_1_REG_DRAMOBF_KEY_PARAM_SR30_NONCE_3     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0660_DRAMOBF_KEY_PARAM_1                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00660)
    #define REG_0660_DRAMOBF_KEY_PARAM_1_REG_DRAMOBF_KEY_PARAM_SR31_NONCE_0     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0664_DRAMOBF_KEY_PARAM_1                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00664)
    #define REG_0664_DRAMOBF_KEY_PARAM_1_REG_DRAMOBF_KEY_PARAM_SR31_NONCE_1     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0668_DRAMOBF_KEY_PARAM_1                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00668)
    #define REG_0668_DRAMOBF_KEY_PARAM_1_REG_DRAMOBF_KEY_PARAM_SR31_NONCE_2     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_066C_DRAMOBF_KEY_PARAM_1                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x0066C)
    #define REG_066C_DRAMOBF_KEY_PARAM_1_REG_DRAMOBF_KEY_PARAM_SR31_NONCE_3     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0670_DRAMOBF_KEY_PARAM_1                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00670)
    #define REG_0670_DRAMOBF_KEY_PARAM_1_REG_DRAMOBF_KEY_PARAM_SR32_NONCE_0     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0674_DRAMOBF_KEY_PARAM_1                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00674)
    #define REG_0674_DRAMOBF_KEY_PARAM_1_REG_DRAMOBF_KEY_PARAM_SR32_NONCE_1     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0678_DRAMOBF_KEY_PARAM_1                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00678)
    #define REG_0678_DRAMOBF_KEY_PARAM_1_REG_DRAMOBF_KEY_PARAM_SR32_NONCE_2     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_067C_DRAMOBF_KEY_PARAM_1                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x0067C)
    #define REG_067C_DRAMOBF_KEY_PARAM_1_REG_DRAMOBF_KEY_PARAM_SR32_NONCE_3     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0680_DRAMOBF_KEY_PARAM_1                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00680)
    #define REG_0680_DRAMOBF_KEY_PARAM_1_REG_DRAMOBF_KEY_PARAM_SR33_NONCE_0     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0684_DRAMOBF_KEY_PARAM_1                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00684)
    #define REG_0684_DRAMOBF_KEY_PARAM_1_REG_DRAMOBF_KEY_PARAM_SR33_NONCE_1     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0688_DRAMOBF_KEY_PARAM_1                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00688)
    #define REG_0688_DRAMOBF_KEY_PARAM_1_REG_DRAMOBF_KEY_PARAM_SR33_NONCE_2     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_068C_DRAMOBF_KEY_PARAM_1                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x0068C)
    #define REG_068C_DRAMOBF_KEY_PARAM_1_REG_DRAMOBF_KEY_PARAM_SR33_NONCE_3     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0690_DRAMOBF_KEY_PARAM_1                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00690)
    #define REG_0690_DRAMOBF_KEY_PARAM_1_REG_DRAMOBF_KEY_PARAM_SR34_NONCE_0     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0694_DRAMOBF_KEY_PARAM_1                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00694)
    #define REG_0694_DRAMOBF_KEY_PARAM_1_REG_DRAMOBF_KEY_PARAM_SR34_NONCE_1     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0698_DRAMOBF_KEY_PARAM_1                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00698)
    #define REG_0698_DRAMOBF_KEY_PARAM_1_REG_DRAMOBF_KEY_PARAM_SR34_NONCE_2     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_069C_DRAMOBF_KEY_PARAM_1                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x0069C)
    #define REG_069C_DRAMOBF_KEY_PARAM_1_REG_DRAMOBF_KEY_PARAM_SR34_NONCE_3     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_06A0_DRAMOBF_KEY_PARAM_1                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x006A0)
    #define REG_06A0_DRAMOBF_KEY_PARAM_1_REG_DRAMOBF_KEY_PARAM_SR35_NONCE_0     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_06A4_DRAMOBF_KEY_PARAM_1                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x006A4)
    #define REG_06A4_DRAMOBF_KEY_PARAM_1_REG_DRAMOBF_KEY_PARAM_SR35_NONCE_1     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_06A8_DRAMOBF_KEY_PARAM_1                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x006A8)
    #define REG_06A8_DRAMOBF_KEY_PARAM_1_REG_DRAMOBF_KEY_PARAM_SR35_NONCE_2     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_06AC_DRAMOBF_KEY_PARAM_1                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x006AC)
    #define REG_06AC_DRAMOBF_KEY_PARAM_1_REG_DRAMOBF_KEY_PARAM_SR35_NONCE_3     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_06B0_DRAMOBF_KEY_PARAM_1                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x006B0)
    #define REG_06B0_DRAMOBF_KEY_PARAM_1_REG_DRAMOBF_KEY_PARAM_SR36_NONCE_0     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_06B4_DRAMOBF_KEY_PARAM_1                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x006B4)
    #define REG_06B4_DRAMOBF_KEY_PARAM_1_REG_DRAMOBF_KEY_PARAM_SR36_NONCE_1     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_06B8_DRAMOBF_KEY_PARAM_1                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x006B8)
    #define REG_06B8_DRAMOBF_KEY_PARAM_1_REG_DRAMOBF_KEY_PARAM_SR36_NONCE_2     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_06BC_DRAMOBF_KEY_PARAM_1                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x006BC)
    #define REG_06BC_DRAMOBF_KEY_PARAM_1_REG_DRAMOBF_KEY_PARAM_SR36_NONCE_3     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_06C0_DRAMOBF_KEY_PARAM_1                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x006C0)
    #define REG_06C0_DRAMOBF_KEY_PARAM_1_REG_DRAMOBF_KEY_PARAM_SR37_NONCE_0     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_06C4_DRAMOBF_KEY_PARAM_1                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x006C4)
    #define REG_06C4_DRAMOBF_KEY_PARAM_1_REG_DRAMOBF_KEY_PARAM_SR37_NONCE_1     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_06C8_DRAMOBF_KEY_PARAM_1                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x006C8)
    #define REG_06C8_DRAMOBF_KEY_PARAM_1_REG_DRAMOBF_KEY_PARAM_SR37_NONCE_2     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_06CC_DRAMOBF_KEY_PARAM_1                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x006CC)
    #define REG_06CC_DRAMOBF_KEY_PARAM_1_REG_DRAMOBF_KEY_PARAM_SR37_NONCE_3     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_06D0_DRAMOBF_KEY_PARAM_1                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x006D0)
    #define REG_06D0_DRAMOBF_KEY_PARAM_1_REG_DRAMOBF_KEY_PARAM_SR38_NONCE_0     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_06D4_DRAMOBF_KEY_PARAM_1                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x006D4)
    #define REG_06D4_DRAMOBF_KEY_PARAM_1_REG_DRAMOBF_KEY_PARAM_SR38_NONCE_1     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_06D8_DRAMOBF_KEY_PARAM_1                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x006D8)
    #define REG_06D8_DRAMOBF_KEY_PARAM_1_REG_DRAMOBF_KEY_PARAM_SR38_NONCE_2     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_06DC_DRAMOBF_KEY_PARAM_1                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x006DC)
    #define REG_06DC_DRAMOBF_KEY_PARAM_1_REG_DRAMOBF_KEY_PARAM_SR38_NONCE_3     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_06E0_DRAMOBF_KEY_PARAM_1                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x006E0)
    #define REG_06E0_DRAMOBF_KEY_PARAM_1_REG_DRAMOBF_KEY_PARAM_SR39_NONCE_0     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_06E4_DRAMOBF_KEY_PARAM_1                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x006E4)
    #define REG_06E4_DRAMOBF_KEY_PARAM_1_REG_DRAMOBF_KEY_PARAM_SR39_NONCE_1     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_06E8_DRAMOBF_KEY_PARAM_1                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x006E8)
    #define REG_06E8_DRAMOBF_KEY_PARAM_1_REG_DRAMOBF_KEY_PARAM_SR39_NONCE_2     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_06EC_DRAMOBF_KEY_PARAM_1                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x006EC)
    #define REG_06EC_DRAMOBF_KEY_PARAM_1_REG_DRAMOBF_KEY_PARAM_SR39_NONCE_3     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_06F0_DRAMOBF_KEY_PARAM_1                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x006F0)
    #define REG_06F0_DRAMOBF_KEY_PARAM_1_REG_DRAMOBF_KEY_PARAM_SR40_NONCE_0     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_06F4_DRAMOBF_KEY_PARAM_1                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x006F4)
    #define REG_06F4_DRAMOBF_KEY_PARAM_1_REG_DRAMOBF_KEY_PARAM_SR40_NONCE_1     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_06F8_DRAMOBF_KEY_PARAM_1                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x006F8)
    #define REG_06F8_DRAMOBF_KEY_PARAM_1_REG_DRAMOBF_KEY_PARAM_SR40_NONCE_2     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_06FC_DRAMOBF_KEY_PARAM_1                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x006FC)
    #define REG_06FC_DRAMOBF_KEY_PARAM_1_REG_DRAMOBF_KEY_PARAM_SR40_NONCE_3     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0700_DRAMOBF_KEY_PARAM_1                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00700)
    #define REG_0700_DRAMOBF_KEY_PARAM_1_REG_DRAMOBF_KEY_PARAM_SR41_NONCE_0     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0704_DRAMOBF_KEY_PARAM_1                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00704)
    #define REG_0704_DRAMOBF_KEY_PARAM_1_REG_DRAMOBF_KEY_PARAM_SR41_NONCE_1     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0708_DRAMOBF_KEY_PARAM_1                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00708)
    #define REG_0708_DRAMOBF_KEY_PARAM_1_REG_DRAMOBF_KEY_PARAM_SR41_NONCE_2     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_070C_DRAMOBF_KEY_PARAM_1                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x0070C)
    #define REG_070C_DRAMOBF_KEY_PARAM_1_REG_DRAMOBF_KEY_PARAM_SR41_NONCE_3     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0710_DRAMOBF_KEY_PARAM_1                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00710)
    #define REG_0710_DRAMOBF_KEY_PARAM_1_REG_DRAMOBF_KEY_PARAM_SR42_NONCE_0     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0714_DRAMOBF_KEY_PARAM_1                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00714)
    #define REG_0714_DRAMOBF_KEY_PARAM_1_REG_DRAMOBF_KEY_PARAM_SR42_NONCE_1     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0718_DRAMOBF_KEY_PARAM_1                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00718)
    #define REG_0718_DRAMOBF_KEY_PARAM_1_REG_DRAMOBF_KEY_PARAM_SR42_NONCE_2     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_071C_DRAMOBF_KEY_PARAM_1                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x0071C)
    #define REG_071C_DRAMOBF_KEY_PARAM_1_REG_DRAMOBF_KEY_PARAM_SR42_NONCE_3     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0720_DRAMOBF_KEY_PARAM_1                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00720)
    #define REG_0720_DRAMOBF_KEY_PARAM_1_REG_DRAMOBF_KEY_PARAM_SR43_NONCE_0     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0724_DRAMOBF_KEY_PARAM_1                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00724)
    #define REG_0724_DRAMOBF_KEY_PARAM_1_REG_DRAMOBF_KEY_PARAM_SR43_NONCE_1     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0728_DRAMOBF_KEY_PARAM_1                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00728)
    #define REG_0728_DRAMOBF_KEY_PARAM_1_REG_DRAMOBF_KEY_PARAM_SR43_NONCE_2     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_072C_DRAMOBF_KEY_PARAM_1                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x0072C)
    #define REG_072C_DRAMOBF_KEY_PARAM_1_REG_DRAMOBF_KEY_PARAM_SR43_NONCE_3     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0730_DRAMOBF_KEY_PARAM_1                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00730)
    #define REG_0730_DRAMOBF_KEY_PARAM_1_REG_DRAMOBF_KEY_PARAM_SR44_NONCE_0     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0734_DRAMOBF_KEY_PARAM_1                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00734)
    #define REG_0734_DRAMOBF_KEY_PARAM_1_REG_DRAMOBF_KEY_PARAM_SR44_NONCE_1     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0738_DRAMOBF_KEY_PARAM_1                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00738)
    #define REG_0738_DRAMOBF_KEY_PARAM_1_REG_DRAMOBF_KEY_PARAM_SR44_NONCE_2     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_073C_DRAMOBF_KEY_PARAM_1                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x0073C)
    #define REG_073C_DRAMOBF_KEY_PARAM_1_REG_DRAMOBF_KEY_PARAM_SR44_NONCE_3     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0740_DRAMOBF_KEY_PARAM_1                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00740)
    #define REG_0740_DRAMOBF_KEY_PARAM_1_REG_DRAMOBF_KEY_PARAM_SR45_NONCE_0     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0744_DRAMOBF_KEY_PARAM_1                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00744)
    #define REG_0744_DRAMOBF_KEY_PARAM_1_REG_DRAMOBF_KEY_PARAM_SR45_NONCE_1     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0748_DRAMOBF_KEY_PARAM_1                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00748)
    #define REG_0748_DRAMOBF_KEY_PARAM_1_REG_DRAMOBF_KEY_PARAM_SR45_NONCE_2     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_074C_DRAMOBF_KEY_PARAM_1                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x0074C)
    #define REG_074C_DRAMOBF_KEY_PARAM_1_REG_DRAMOBF_KEY_PARAM_SR45_NONCE_3     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0750_DRAMOBF_KEY_PARAM_1                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00750)
    #define REG_0750_DRAMOBF_KEY_PARAM_1_REG_DRAMOBF_KEY_PARAM_SR46_NONCE_0     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0754_DRAMOBF_KEY_PARAM_1                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00754)
    #define REG_0754_DRAMOBF_KEY_PARAM_1_REG_DRAMOBF_KEY_PARAM_SR46_NONCE_1     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0758_DRAMOBF_KEY_PARAM_1                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00758)
    #define REG_0758_DRAMOBF_KEY_PARAM_1_REG_DRAMOBF_KEY_PARAM_SR46_NONCE_2     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_075C_DRAMOBF_KEY_PARAM_1                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x0075C)
    #define REG_075C_DRAMOBF_KEY_PARAM_1_REG_DRAMOBF_KEY_PARAM_SR46_NONCE_3     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0760_DRAMOBF_KEY_PARAM_1                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00760)
    #define REG_0760_DRAMOBF_KEY_PARAM_1_REG_DRAMOBF_KEY_PARAM_SR47_NONCE_0     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0764_DRAMOBF_KEY_PARAM_1                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00764)
    #define REG_0764_DRAMOBF_KEY_PARAM_1_REG_DRAMOBF_KEY_PARAM_SR47_NONCE_1     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0768_DRAMOBF_KEY_PARAM_1                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00768)
    #define REG_0768_DRAMOBF_KEY_PARAM_1_REG_DRAMOBF_KEY_PARAM_SR47_NONCE_2     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_076C_DRAMOBF_KEY_PARAM_1                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x0076C)
    #define REG_076C_DRAMOBF_KEY_PARAM_1_REG_DRAMOBF_KEY_PARAM_SR47_NONCE_3     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0770_DRAMOBF_KEY_PARAM_1                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00770)
    #define REG_0770_DRAMOBF_KEY_PARAM_1_REG_DRAMOBF_KEY_PARAM_SR48_NONCE_0     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0774_DRAMOBF_KEY_PARAM_1                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00774)
    #define REG_0774_DRAMOBF_KEY_PARAM_1_REG_DRAMOBF_KEY_PARAM_SR48_NONCE_1     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_0778_DRAMOBF_KEY_PARAM_1                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00778)
    #define REG_0778_DRAMOBF_KEY_PARAM_1_REG_DRAMOBF_KEY_PARAM_SR48_NONCE_2     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_REG_077C_DRAMOBF_KEY_PARAM_1                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x0077C)
    #define REG_077C_DRAMOBF_KEY_PARAM_1_REG_DRAMOBF_KEY_PARAM_SR48_NONCE_3     Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMOBF_CH0_REG_TRNG_CTRL                                               Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00800)
    #define TRNG_CTRL_TRNG_RDY                                                  Fld(0, 0, 0, 1, 31) //[31:31]
    #define TRNG_CTRL_TRNG_FSM                                                  Fld(0, 0, 0, 2, 2) //[3:2]
    #define TRNG_CTRL_FREERUN                                                   Fld(0, 0, 0, 1, 1) //[1:1]
    #define TRNG_CTRL_TRNG_START                                                Fld(0, 0, 0, 1, 0) //[0:0]

#define DRAMOBF_CH0_REG_TRNG_TIME                                               Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00804)
    #define TRNG_TIME_SAMPLE_CNT                                                Fld(0, 0, 0, 8, 24) //[31:24]
    #define TRNG_TIME_UNGATE_CNT                                                Fld(0, 0, 0, 8, 16) //[23:16]
    #define TRNG_TIME_LATCH_CNT                                                 Fld(0, 0, 0, 8, 8) //[15:8]
    #define TRNG_TIME_SYSCLK_CNT                                                Fld(0, 0, 0, 8, 0) //[7:0]

#define DRAMOBF_CH0_REG_TRNG_DATA                                               Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00808)
    #define TRNG_DATA_DATA                                                      Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMOBF_CH0_REG_TRNG_CONF                                               Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x0080C)
    #define TRNG_CONF_RO2_EN                                                    Fld(0, 0, 0, 2, 18) //[19:18]
    #define TRNG_CONF_TIMEOUT_LIMIT                                             Fld(0, 0, 0, 12, 6) //[17:6]
    #define TRNG_CONF_VON_EN                                                    Fld(0, 0, 0, 1, 5) //[5:5]
    #define TRNG_CONF_RO_EN                                                     Fld(0, 0, 0, 3, 2) //[4:2]
    #define TRNG_CONF_RO_OUT_SEL                                                Fld(0, 0, 0, 2, 0) //[1:0]

#define DRAMOBF_CH0_REG_TRNG_INT_SET                                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00810)
    #define TRNG_INT_SET_INT                                                    Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMOBF_CH0_REG_TRNG_INT_CLR                                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00814)
    #define TRNG_INT_CLR_CLR                                                    Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMOBF_CH0_REG_TRNG_INT_MSK                                            Rinfo(0, 0, 0, 0, RT_DRAMOBF_CH0, 0x00818)
    #define TRNG_INT_MSK_IRQ_MSK                                                Fld(0, 0, 0, 2, 0) //[1:0]

#endif // __DRAMOBF_CH0_REGS_H__
