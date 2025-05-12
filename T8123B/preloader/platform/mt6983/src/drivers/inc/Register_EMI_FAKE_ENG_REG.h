#ifndef __FKE_REGS_H__
#define __FKE_REGS_H__

#define FKE_BASE_ADDRESS FAKE_ENGINE_BASE_VIRTUAL

#define FKE_REG_FAKE_ENG_EN                                    (FKE_BASE_ADDRESS + 0x00000)
    #define FAKE_ENG_EN_FAKE_ENG_ENABLE                        Fld(1, 0) //[0:0]

#define FKE_REG_FAKE_ENG_RST                                   (FKE_BASE_ADDRESS + 0x00004)
    #define FAKE_ENG_RST_FAKE_ENG_RESET                        Fld(1, 0) //[0:0]

#define FKE_REG_FAKE_ENG_DONE                                  (FKE_BASE_ADDRESS + 0x00008)
    #define FAKE_ENG_DONE_FAKE_ENGINE_DONE                     Fld(1, 0) //[0:0]

#define FKE_REG_FAKE_ENG_CON0                                  (FKE_BASE_ADDRESS + 0x0000C)
    #define FAKE_ENG_CON0_PAT_MODE                             Fld(4, 13) //[16:13]
    #define FAKE_ENG_CON0_ULTRA_CNT                            Fld(3, 10) //[12:10]
    #define FAKE_ENG_CON0_ULTRA_EN                             Fld(1, 9) //[9:9]
    #define FAKE_ENG_CON0_PREULTRA_CNT                         Fld(3, 6) //[8:6]
    #define FAKE_ENG_CON0_PREULTRA_EN                          Fld(1, 5) //[5:5]
    #define FAKE_ENG_CON0_DATA_CMP_EN                          Fld(1, 4) //[4:4]
    #define FAKE_ENG_CON0_CROSS_RANK_EN                        Fld(1, 3) //[3:3]
    #define FAKE_ENG_CON0_LOOP_MODE_EN                         Fld(1, 2) //[2:2]
    #define FAKE_ENG_CON0_WR_DIS                               Fld(1, 1) //[1:1]
    #define FAKE_ENG_CON0_RD_DIS                               Fld(1, 0) //[0:0]

#define FKE_REG_FAKE_ENG_CON1                                  (FKE_BASE_ADDRESS + 0x00010)
    #define FAKE_ENG_CON1_SLOW_DOWN_GRP                        Fld(10, 20) //[29:20]
    #define FAKE_ENG_CON1_SLOW_DOWN                            Fld(10, 10) //[19:10]
    #define FAKE_ENG_CON1_WR_CMD_AMOUNT                        Fld(5, 5) //[9:5]
    #define FAKE_ENG_CON1_RD_CMD_AMOUNT                        Fld(5, 0) //[4:0]

#define FKE_REG_FAKE_ENG_CON2                                  (FKE_BASE_ADDRESS + 0x00014)
    #define FAKE_ENG_CON2_AR_SLC                               Fld(5, 18) //[22:18]
    #define FAKE_ENG_CON2_AW_SLC                               Fld(5, 13) //[17:13]
    #define FAKE_ENG_CON2_AXI_DOMAIN                           Fld(3, 10) //[12:10]
    #define FAKE_ENG_CON2_PROT                                 Fld(3, 7) //[9:7]
    #define FAKE_ENG_CON2_BURST_SIZE                           Fld(3, 4) //[6:4]
    #define FAKE_ENG_CON2_BURST_LEN                            Fld(4, 0) //[3:0]

#define FKE_REG_FAKE_ENG_CON3                                  (FKE_BASE_ADDRESS + 0x00018)
    #define FAKE_ENG_CON3_GROUP_AMOUNT                         Fld(32, 0) //[31:0]

#define FKE_REG_FAKE_ENG_START_ADDR                            (FKE_BASE_ADDRESS + 0x0001C)
    #define FAKE_ENG_START_ADDR_START_ADDR                     Fld(32, 0) //[31:0]

#define FKE_REG_FAKE_ENG_START_ADDR_2ND                        (FKE_BASE_ADDRESS + 0x00020)
    #define FAKE_ENG_START_ADDR_2ND_START_ADDR_2ND             Fld(32, 0) //[31:0]

#define FKE_REG_FAKE_ENG_ADDR                                  (FKE_BASE_ADDRESS + 0x00024)
    #define FAKE_ENG_ADDR_ADDR_OFFSET2                         Fld(10, 18) //[27:18]
    #define FAKE_ENG_ADDR_ADDR_OFFSET1                         Fld(10, 8) //[17:8]
    #define FAKE_ENG_ADDR_START_ADDR_2ND                       Fld(4, 4) //[7:4]
    #define FAKE_ENG_ADDR_START_ADDR                           Fld(4, 0) //[3:0]

#define FKE_REG_FAKE_ENG_INIT_PAT0                             (FKE_BASE_ADDRESS + 0x00028)
    #define FAKE_ENG_INIT_PAT0_INIT_PAT                        Fld(32, 0) //[31:0]

#define FKE_REG_FAKE_ENG_INIT_PAT1                             (FKE_BASE_ADDRESS + 0x0002C)
    #define FAKE_ENG_INIT_PAT1_INIT_PAT                        Fld(32, 0) //[31:0]

#define FKE_REG_FAKE_ENG_INIT_PAT2                             (FKE_BASE_ADDRESS + 0x00030)
    #define FAKE_ENG_INIT_PAT2_INIT_PAT                        Fld(32, 0) //[31:0]

#define FKE_REG_FAKE_ENG_INIT_PAT3                             (FKE_BASE_ADDRESS + 0x00034)
    #define FAKE_ENG_INIT_PAT3_INIT_PAT                        Fld(32, 0) //[31:0]

#define FKE_REG_FAKE_ENG_INIT_PAT4                             (FKE_BASE_ADDRESS + 0x00038)
    #define FAKE_ENG_INIT_PAT4_INIT_PAT                        Fld(32, 0) //[31:0]

#define FKE_REG_FAKE_ENG_INIT_PAT5                             (FKE_BASE_ADDRESS + 0x0003C)
    #define FAKE_ENG_INIT_PAT5_INIT_PAT                        Fld(32, 0) //[31:0]

#define FKE_REG_FAKE_ENG_INIT_PAT6                             (FKE_BASE_ADDRESS + 0x00040)
    #define FAKE_ENG_INIT_PAT6_INIT_PAT                        Fld(32, 0) //[31:0]

#define FKE_REG_FAKE_ENG_INIT_PAT7                             (FKE_BASE_ADDRESS + 0x00044)
    #define FAKE_ENG_INIT_PAT7_INIT_PAT                        Fld(32, 0) //[31:0]

#define FKE_REG_FAKE_ENG_CMP_RESULT                            (FKE_BASE_ADDRESS + 0x00048)
    #define FAKE_ENG_CMP_RESULT_CMP_FAIL                       Fld(1, 1) //[1:1]
    #define FAKE_ENG_CMP_RESULT_CMP_PASS                       Fld(1, 0) //[0:0]

#define FKE_REG_FAKE_ENG_FAIL_ADDR                             (FKE_BASE_ADDRESS + 0x0004C)
    #define FAKE_ENG_FAIL_ADDR_FAIL_ADDR                       Fld(32, 0) //[31:0]

#define FKE_REG_FAKE_ENG_FAIL_ADDR_MSB                         (FKE_BASE_ADDRESS + 0x00050)
    #define FAKE_ENG_FAIL_ADDR_MSB_FAIL_ADDR                   Fld(4, 0) //[3:0]

#define FKE_REG_FAKE_ENG_EXP_DATA0                             (FKE_BASE_ADDRESS + 0x00054)
    #define FAKE_ENG_EXP_DATA0_EXP_DATA                        Fld(32, 0) //[31:0]

#define FKE_REG_FAKE_ENG_EXP_DATA1                             (FKE_BASE_ADDRESS + 0x00058)
    #define FAKE_ENG_EXP_DATA1_EXP_DATA                        Fld(32, 0) //[31:0]

#define FKE_REG_FAKE_ENG_EXP_DATA2                             (FKE_BASE_ADDRESS + 0x0005C)
    #define FAKE_ENG_EXP_DATA2_EXP_DATA                        Fld(32, 0) //[31:0]

#define FKE_REG_FAKE_ENG_EXP_DATA3                             (FKE_BASE_ADDRESS + 0x00060)
    #define FAKE_ENG_EXP_DATA3_EXP_DATA                        Fld(32, 0) //[31:0]

#define FKE_REG_FAKE_ENG_EXP_DATA4                             (FKE_BASE_ADDRESS + 0x00064)
    #define FAKE_ENG_EXP_DATA4_EXP_DATA                        Fld(32, 0) //[31:0]

#define FKE_REG_FAKE_ENG_EXP_DATA5                             (FKE_BASE_ADDRESS + 0x00068)
    #define FAKE_ENG_EXP_DATA5_EXP_DATA                        Fld(32, 0) //[31:0]

#define FKE_REG_FAKE_ENG_EXP_DATA6                             (FKE_BASE_ADDRESS + 0x0006C)
    #define FAKE_ENG_EXP_DATA6_EXP_DATA                        Fld(32, 0) //[31:0]

#define FKE_REG_FAKE_ENG_EXP_DATA7                             (FKE_BASE_ADDRESS + 0x00070)
    #define FAKE_ENG_EXP_DATA7_EXP_DATA                        Fld(32, 0) //[31:0]

#define FKE_REG_FAKE_ENG_FAIL_DATA0                            (FKE_BASE_ADDRESS + 0x00074)
    #define FAKE_ENG_FAIL_DATA0_FAIL_DATA                      Fld(32, 0) //[31:0]

#define FKE_REG_FAKE_ENG_FAIL_DATA1                            (FKE_BASE_ADDRESS + 0x00078)
    #define FAKE_ENG_FAIL_DATA1_FAIL_DATA                      Fld(32, 0) //[31:0]

#define FKE_REG_FAKE_ENG_FAIL_DATA2                            (FKE_BASE_ADDRESS + 0x0007C)
    #define FAKE_ENG_FAIL_DATA2_FAIL_DATA                      Fld(32, 0) //[31:0]

#define FKE_REG_FAKE_ENG_FAIL_DATA3                            (FKE_BASE_ADDRESS + 0x00080)
    #define FAKE_ENG_FAIL_DATA3_FAIL_DATA                      Fld(32, 0) //[31:0]

#define FKE_REG_FAKE_ENG_FAIL_DATA4                            (FKE_BASE_ADDRESS + 0x00084)
    #define FAKE_ENG_FAIL_DATA4_FAIL_DATA                      Fld(32, 0) //[31:0]

#define FKE_REG_FAKE_ENG_FAIL_DATA5                            (FKE_BASE_ADDRESS + 0x00088)
    #define FAKE_ENG_FAIL_DATA5_FAIL_DATA                      Fld(32, 0) //[31:0]

#define FKE_REG_FAKE_ENG_FAIL_DATA6                            (FKE_BASE_ADDRESS + 0x0008C)
    #define FAKE_ENG_FAIL_DATA6_FAIL_DATA                      Fld(32, 0) //[31:0]

#define FKE_REG_FAKE_ENG_FAIL_DATA7                            (FKE_BASE_ADDRESS + 0x00090)
    #define FAKE_ENG_FAIL_DATA7_FAIL_DATA                      Fld(32, 0) //[31:0]

#define FKE_REG_BRIDGE_ADDR_MAP                                (FKE_BASE_ADDRESS + 0x00094)
    #define BRIDGE_ADDR_MAP_ADDR_MAP                           Fld(2, 0) //[1:0]

#define FKE_REG_BRIDGE_LPDDR                                   (FKE_BASE_ADDRESS + 0x00098)
    #define BRIDGE_LPDDR_LPDDR                                 Fld(1, 0) //[0:0]

#define FKE_REG_BRIDGE_RANK0_SIZE                              (FKE_BASE_ADDRESS + 0x0009C)
    #define BRIDGE_RANK0_SIZE_RANK0_SIZE                       Fld(6, 0) //[5:0]

#define FKE_REG_BRIDGE_RANK1_SIZE                              (FKE_BASE_ADDRESS + 0x000A0)
    #define BRIDGE_RANK1_SIZE_RANK1_SIZE                       Fld(6, 0) //[5:0]

#define FKE_REG_BRIDGE_BANK_ARB_EN                             (FKE_BASE_ADDRESS + 0x000A4)
    #define BRIDGE_BANK_ARB_EN_BANK_ARB_EN                     Fld(1, 0) //[0:0]

#define FKE_REG_FAKE_ENG_FAIL_CNT                              (FKE_BASE_ADDRESS + 0x000AC)
    #define FAKE_ENG_FAIL_CNT_FAIL_CNT                         Fld(32, 0) //[31:0]

#define FKE_REG_FAKE_ENG_FREEZE_RESULT                         (FKE_BASE_ADDRESS + 0x000B0)
    #define FAKE_ENG_FREEZE_RESULT_FREEZE_RESULT               Fld(1, 0) //[0:0]

#define FKE_REG_FAKE_ENG_IDLE                                  (FKE_BASE_ADDRESS + 0x000B4)
    #define FAKE_ENG_IDLE_FAKE_ENG_IDLE                        Fld(1, 0) //[0:0]

#define FKE_REG_FAKE_ENG_HASH                                  (FKE_BASE_ADDRESS + 0x000B8)
    #define FAKE_ENG_HASH_HASH_OPT                             Fld(1, 24) //[24:24]
    #define FAKE_ENG_HASH_CHN_POS                              Fld(3, 20) //[22:20]
    #define FAKE_ENG_HASH_CHN_EN                               Fld(1, 16) //[16:16]
    #define FAKE_ENG_HASH_EMI_DISP_EN                          Fld(4, 12) //[15:12]
    #define FAKE_ENG_HASH_CHN_HASH_EN                          Fld(4, 8) //[11:8]
    #define FAKE_ENG_HASH_CHN_NUMBER                           Fld(2, 4) //[5:4]
    #define FAKE_ENG_HASH_DIFF_CHN_EN                          Fld(1, 0) //[0:0]

#define FKE_REG_FAKE_ENG_START_ADDR_RD                         (FKE_BASE_ADDRESS + 0x000BC)
    #define FAKE_ENG_START_ADDR_RD_START_ADDR_RD               Fld(32, 0) //[31:0]

#define FKE_REG_FAKE_ENG_START_ADDR_RD_2ND                     (FKE_BASE_ADDRESS + 0x000C0)
    #define FAKE_ENG_START_ADDR_RD_2ND_START_ADDR_RD_2ND       Fld(32, 0) //[31:0]

#define FKE_REG_FAKE_ENG_ADDR_RD                               (FKE_BASE_ADDRESS + 0x000C4)
    #define FAKE_ENG_ADDR_RD_START_ADDR_RD_2ND                 Fld(4, 4) //[7:4]
    #define FAKE_ENG_ADDR_RD_START_ADDR_RD                     Fld(4, 0) //[3:0]

#endif // __FKE_REGS_H__
