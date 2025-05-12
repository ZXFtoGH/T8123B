#ifndef __SPM_REGS_H__
#define __SPM_REGS_H__

#define SPM_BASE_ADDRESS 0x1C001000

#define SPM_REG_POWERON_CONFIG_EN                              (SPM_BASE_ADDRESS + 0x00000)
    #define POWERON_CONFIG_EN_PROJECT_CODE                     Fld(16, 16) //[31:16]
    #define POWERON_CONFIG_EN_BCLK_CG_EN                       Fld(1, 0) //[0:0]

#define SPM_REG_SPM_POWER_ON_VAL0                              (SPM_BASE_ADDRESS + 0x00004)
    #define SPM_POWER_ON_VAL0_POWER_ON_VAL0                    Fld(32, 0) //[31:0]

#define SPM_REG_SPM_POWER_ON_VAL1                              (SPM_BASE_ADDRESS + 0x00008)
    #define SPM_POWER_ON_VAL1_POWER_ON_VAL1                    Fld(32, 0) //[31:0]
    #define SPM_POWER_ON_VAL1_PWRAP_SLEEP_REQ                  Fld(1,0) //cc from MT6877. Same below
    #define SPM_POWER_ON_VAL1_EMI_CLK_OFF_REQ                  Fld(1,1)
    #define SPM_POWER_ON_VAL1_PCM_BUS_PROTECT_REQ              Fld(1,2)
    #define SPM_POWER_ON_VAL1_SPM_CK_UPDATE                    Fld(1,3)
    #define SPM_POWER_ON_VAL1_SPM_CK_SEL0                      Fld(1,4)
    #define SPM_POWER_ON_VAL1_SPM_CK_SEL1                      Fld(1,5)
    #define SPM_POWER_ON_VAL1_SPM_LEAVE_DEEPIDLE_REQ           Fld(1,6)
    #define SPM_POWER_ON_VAL1_FREQH_PAUSE_MPLL                 Fld(1,7)
    #define SPM_POWER_ON_VAL1_SC_26M_CK_SEL                    Fld(1,8)
    #define SPM_POWER_ON_VAL1_PCM_TIMER_SET                    Fld(1,9)
    #define SPM_POWER_ON_VAL1_PCM_TIMER_CLR                    Fld(1,10)
    #define SPM_POWER_ON_VAL1_SPM_LEAVE_SUSPEND_REQ            Fld(1,11)
    #define SPM_POWER_ON_VAL1_CSYSPWRUPACK                     Fld(1,12)
    #define SPM_POWER_ON_VAL1_IM_SLEEP_ENABLE                  Fld(1,13)
    #define SPM_POWER_ON_VAL1_SRCCLKENO                        Fld(1,14)
    #define SPM_POWER_ON_VAL1_FORCE_DDREN_ON                   Fld(1,15)
    #define SPM_POWER_ON_VAL1_SPM_APSRC_INTERNAL_ACK           Fld(1,16)
    #define SPM_POWER_ON_VAL1_CPU_SYS_TIMER_CLK_SEL            Fld(1,17)
    #define SPM_POWER_ON_VAL1_SC_AXI_DCM_DIS                   Fld(1,18)
    #define SPM_POWER_ON_VAL1_FREQH_PAUSE_MEM                  Fld(1,19)
    #define SPM_POWER_ON_VAL1_FREQH_PAUSE_MAIN                 Fld(1,20)
    #define SPM_POWER_ON_VAL1_SRCCLKENO1                       Fld(1,21)
    #define SPM_POWER_ON_VAL1_WDT_KICK_P                       Fld(1,22)
    #define SPM_POWER_ON_VAL1_SPM2EMI_S1_MODE_ASYNC            Fld(1,23)
    #define SPM_POWER_ON_VAL1_SC_DDR_PST_REQ                   Fld(1,24)
    #define SPM_POWER_ON_VAL1_SC_DDR_PST_ABORT_REQ             Fld(1,25)
    #define SPM_POWER_ON_VAL1_PMIC_IRQ_REQ_EN                  Fld(1,26)
    #define SPM_POWER_ON_VAL1_FORCE_VTCXO_ON                   Fld(1,27)
    #define SPM_POWER_ON_VAL1_FORCE_APSRC_ON                   Fld(1,28)
    #define SPM_POWER_ON_VAL1_FORCE_INFRA_ON                   Fld(1,29)
    #define SPM_POWER_ON_VAL1_FORCE_VRF18_ON                   Fld(1,30)
    #define SPM_POWER_ON_VAL1_SPM_DDR_EN_INTERNAL_ACK          Fld(1,31)

#define SPM_REG_SPM_POWER_ON_VAL2                              (SPM_BASE_ADDRESS + 0x0000C)
    #define SPM_POWER_ON_VAL2_POWER_ON_VAL2                    Fld(32, 0) //[31:0]

#define SPM_REG_SPM_POWER_ON_VAL3                              (SPM_BASE_ADDRESS + 0x00010)
    #define SPM_POWER_ON_VAL3_POWER_ON_VAL3                    Fld(32, 0) //[31:0]

#define SPM_REG_PCM_PWR_IO_EN                                  (SPM_BASE_ADDRESS + 0x00014)
    #define PCM_PWR_IO_EN_PCM_PWR_IO_EN                        Fld(8, 0) //[7:0]

#define SPM_REG_PCM_CON0                                       (SPM_BASE_ADDRESS + 0x00018)
    #define PCM_CON0_PROJECT_CODE                              Fld(16, 16) //[31:16]
    #define PCM_CON0_PCM_SW_RESET                              Fld(1, 15) //[15:15]
    #define PCM_CON0_PCM_CK_EN                                 Fld(1, 2) //[2:2]

#define SPM_REG_PCM_CON1                                       (SPM_BASE_ADDRESS + 0x0001C)
    #define PCM_CON1_PROJECT_CODE                              Fld(16, 16) //[31:16]
    #define PCM_CON1_RG_PCM_IRQ_MSK                            Fld(1, 15) //[15:15]
    #define PCM_CON1_REG_RSV_APB_P2P_EN                        Fld(1, 12) //[12:12]
    #define PCM_CON1_REG_MCUPM_APB_P2P_EN                      Fld(1, 11) //[11:11]
    #define PCM_CON1_REG_SSPM_APB_P2P_EN                       Fld(1, 10) //[10:10]
    #define PCM_CON1_REG_PCM_WDT_WAKE                          Fld(1, 9) //[9:9]
    #define PCM_CON1_REG_PCM_WDT_EN                            Fld(1, 8) //[8:8]
    #define PCM_CON1_REG_PCM_TIMER_EN                          Fld(1, 5) //[5:5]
    #define PCM_CON1_REG_SPM_APB_INTERNAL_EN                   Fld(1, 3) //[3:3]

#define SPM_REG_SPM_SRAM_SLEEP_CTRL                            (SPM_BASE_ADDRESS + 0x00020)
    #define SPM_SRAM_SLEEP_CTRL_REG_SPM_SRAM_CTRL_MUX          Fld(1, 16) //[16:16]
    #define SPM_SRAM_SLEEP_CTRL_REG_SRAM_SLP2ISO_TIME          Fld(8, 8) //[15:8]
    #define SPM_SRAM_SLEEP_CTRL_REG_SRAM_ISO_ACTIVE            Fld(8, 0) //[7:0]

#define SPM_REG_SPM_CLK_CON                                    (SPM_BASE_ADDRESS + 0x00024)
    #define SPM_CLK_CON_REG_SRCLKENO2_SRC_MASK_B               Fld(8, 24) //[31:24]
    #define SPM_CLK_CON_REG_SRCLKENO1_SRC_MASK_B               Fld(8, 16) //[23:16]
    #define SPM_CLK_CON_REG_SRCLKENO0_SRC_MASK_B               Fld(8, 8) //[15:8]
    #define SPM_CLK_CON_REG_SPM_LEAVE_SUSPEND_MERGE_MASK       Fld(3, 4) //[6:4]
    #define SPM_CLK_CON_REG_CXO32K_REMOVE_EN                   Fld(1, 1) //[1:1]
    #define SPM_CLK_CON_REG_SPM_LOCK_INFRA_DCM                 Fld(1, 0) //[0:0]

#define SPM_REG_SPM_CLK_SETTLE                                 (SPM_BASE_ADDRESS + 0x00028)
    #define SPM_CLK_SETTLE_SYSCLK_SETTLE                       Fld(28, 0) //[27:0]

#define SPM_REG_SPM_SW_RST_CON                                 (SPM_BASE_ADDRESS + 0x00040)
    #define SPM_SW_RST_CON_PROJECT_CODE                        Fld(16, 16) //[31:16]
    #define SPM_SW_RST_CON_SPM_SW_RST_CON                      Fld(16, 0) //[15:0]

#define SPM_REG_SPM_SW_RST_CON_SET                             (SPM_BASE_ADDRESS + 0x00044)
    #define SPM_SW_RST_CON_SET_PROJECT_CODE                    Fld(16, 16) //[31:16]
    #define SPM_SW_RST_CON_SET_SPM_SW_RST_CON_SET              Fld(16, 0) //[15:0]

#define SPM_REG_SPM_SW_RST_CON_CLR                             (SPM_BASE_ADDRESS + 0x00048)
    #define SPM_SW_RST_CON_CLR_PROJECT_CODE                    Fld(16, 16) //[31:16]
    #define SPM_SW_RST_CON_CLR_SPM_SW_RST_CON_CLR              Fld(16, 0) //[15:0]

#define SPM_REG_SSPM_CLK_CON                                   (SPM_BASE_ADDRESS + 0x00084)
    #define SSPM_CLK_CON_REG_SSPM_DCM_EN                       Fld(1, 1) //[1:1]
    #define SSPM_CLK_CON_REG_SSPM_26M_CK_SEL                   Fld(1, 0) //[0:0]

#define SPM_REG_SCP_CLK_CON                                    (SPM_BASE_ADDRESS + 0x00088)
    #define SCP_CLK_CON_SCP_SLP_ACK                            Fld(1, 4) //[4:4]
    #define SCP_CLK_CON_SCP_SLP_REQ                            Fld(1, 3) //[3:3]
    #define SCP_CLK_CON_SCP_SECURE_VREQ_MASK                   Fld(1, 2) //[2:2]
    #define SCP_CLK_CON_REG_SCP_DCM_EN                         Fld(1, 1) //[1:1]
    #define SCP_CLK_CON_REG_SCP_26M_CK_SEL                     Fld(1, 0) //[0:0]

#define SPM_REG_SPM_SWINT                                      (SPM_BASE_ADDRESS + 0x00090)
    #define SPM_SWINT_SPM_SWINT                                Fld(32, 0) //[31:0]

#define SPM_REG_SPM_SWINT_SET                                  (SPM_BASE_ADDRESS + 0x00094)
    #define SPM_SWINT_SET_SPM_SWINT_SET                        Fld(32, 0) //[31:0]

#define SPM_REG_SPM_SWINT_CLR                                  (SPM_BASE_ADDRESS + 0x00098)
    #define SPM_SWINT_CLR_SPM_SWINT_CLR                        Fld(32, 0) //[31:0]

#define SPM_REG_SPM_CPU_WAKEUP_EVENT                           (SPM_BASE_ADDRESS + 0x000B0)
    #define SPM_CPU_WAKEUP_EVENT_REG_CPU_WAKEUP                Fld(1, 0) //[0:0]

#define SPM_REG_SPM_IRQ_MASK                                   (SPM_BASE_ADDRESS + 0x000B4)
    #define SPM_IRQ_MASK_REG_SPM_IRQ_MASK                      Fld(32, 0) //[31:0]

#define SPM_REG_MD32PCM_SCU_CTRL0                              (SPM_BASE_ADDRESS + 0x00100)
    #define MD32PCM_SCU_CTRL0_MD32PCM_CTRL0                    Fld(32, 0) //[31:0]

#define SPM_REG_MD32PCM_SCU_CTRL1                              (SPM_BASE_ADDRESS + 0x00104)
    #define MD32PCM_SCU_CTRL1_MD32PCM_CTRL1                    Fld(32, 0) //[31:0]

#define SPM_REG_MD32PCM_SCU_CTRL2                              (SPM_BASE_ADDRESS + 0x00108)
    #define MD32PCM_SCU_CTRL2_MD32PCM_CTRL2                    Fld(32, 0) //[31:0]

#define SPM_REG_MD32PCM_SCU_CTRL3                              (SPM_BASE_ADDRESS + 0x0010C)
    #define MD32PCM_SCU_CTRL3_MD32PCM_CTRL3                    Fld(32, 0) //[31:0]

#define SPM_REG_MD32PCM_SCU_STA0                               (SPM_BASE_ADDRESS + 0x00110)
    #define MD32PCM_SCU_STA0_MD32PCM_STA0                      Fld(32, 0) //[31:0]

#define SPM_REG_MD32PCM_WAKEUP_STA                             (SPM_BASE_ADDRESS + 0x00130)
    #define MD32PCM_WAKEUP_STA_MD32PCM_WAKEUP_STA              Fld(32, 0) //[31:0]

#define SPM_REG_MD32PCM_EVENT_STA                              (SPM_BASE_ADDRESS + 0x00134)
    #define MD32PCM_EVENT_STA_MD32PCM_EVENT_STA                Fld(32, 0) //[31:0]

#define SPM_REG_SPM_IRQ_STA                                    (SPM_BASE_ADDRESS + 0x00128)
    #define SPM_IRQ_STA_PCM_IRQ                                Fld(1, 3) //[3:3]

#define SPM_REG_SPM_WAKEUP_MISC                                (SPM_BASE_ADDRESS + 0x00140)
    #define SPM_WAKEUP_MISC_PMIC_SCP_IRQ                       Fld(1, 31) //[31:31]
    #define SPM_WAKEUP_MISC_PMIC_IRQ_ACK                       Fld(1, 30) //[30:30]
    #define SPM_WAKEUP_MISC_PMIC_EINT_OUT                      Fld(2, 19) //[20:19]
    #define SPM_WAKEUP_MISC_PCM_TIMER_EVENT                    Fld(1, 16) //[16:16]
    #define SPM_WAKEUP_MISC_VLP_BUS_TIMEOUT_IRQ                Fld(1, 11) //[11:11]
    #define SPM_WAKEUP_MISC_SPM_ACK_CHK_WAKEUP_ALL             Fld(1, 10) //[10:10]
    #define SPM_WAKEUP_MISC_SPM_ACK_CHK_WAKEUP_3               Fld(1, 9) //[9:9]
    #define SPM_WAKEUP_MISC_SPM_ACK_CHK_WAKEUP_2               Fld(1, 8) //[8:8]
    #define SPM_WAKEUP_MISC_SPM_ACK_CHK_WAKEUP_1               Fld(1, 7) //[7:7]
    #define SPM_WAKEUP_MISC_SPM_ACK_CHK_WAKEUP_0               Fld(1, 6) //[6:6]
    #define SPM_WAKEUP_MISC_TWAM_IRQ_B                         Fld(1, 5) //[5:5]
    #define SPM_WAKEUP_MISC_DVFSRC_IRQ                         Fld(1, 4) //[4:4]
    #define SPM_WAKEUP_MISC_SPM_TIMEOUT_WAKEUP_2               Fld(1, 3) //[3:3]
    #define SPM_WAKEUP_MISC_SPM_TIMEOUT_WAKEUP_1               Fld(1, 2) //[2:2]
    #define SPM_WAKEUP_MISC_SPM_TIMEOUT_WAKEUP_0               Fld(1, 1) //[1:1]
    #define SPM_WAKEUP_MISC_SRCLKEN_RC_ERR_INT                 Fld(1, 0) //[0:0]

#define SPM_REG_SPM_CK_STA                                     (SPM_BASE_ADDRESS + 0x00164)
    #define SPM_CK_STA_EXT_SRC_STA                             Fld(3, 4) //[6:4]
    #define SPM_CK_STA_PCM_CK_SEL_O                            Fld(4, 0) //[3:0]

#define SPM_REG_MD32PCM_STA                                    (SPM_BASE_ADDRESS + 0x00190)
    #define MD32PCM_STA_MD32PCM_GATED                          Fld(1, 1) //[1:1]
    #define MD32PCM_STA_MD32PCM_HALT                           Fld(1, 0) //[0:0]

#define SPM_REG_MD32PCM_PC                                     (SPM_BASE_ADDRESS + 0x00194)
    #define MD32PCM_PC_MON_PC                                  Fld(32, 0) //[31:0]

#define SPM_REG_SPM_AP_STANDBY_CON                             (SPM_BASE_ADDRESS + 0x00200)
    #define SPM_AP_STANDBY_CON_REG_WFI_OP                      Fld(1, 0) //[0:0]
    #define SPM_AP_STANDBY_CON_REG_WFI_TYPE                    Fld(1, 1) //[1:1]
    #define SPM_AP_STANDBY_CON_REG_MP0_CPUTOP_IDLE_MASK        Fld(1, 2) //[2:2]
    #define SPM_AP_STANDBY_CON_REG_MP1_CPUTOP_IDLE_MASK        Fld(1, 3) //[3:3]
    #define SPM_AP_STANDBY_CON_REG_MCUSYS_IDLE_MASK            Fld(1, 4) //[4:4]
    #define SPM_AP_STANDBY_CON_REG_CSYSPWRUP_REQ_MASK          Fld(1, 5) //[5:5]
    #define SPM_AP_STANDBY_CON_WFI_AF_SEL                      Fld(8, 16) //[23:16]
    #define SPM_AP_STANDBY_CON_CPU_SLEEP_WFI                   Fld(1, 31) //[31:31]

#define SPM_REG_CPU_WFI_EN                                     (SPM_BASE_ADDRESS + 0x00204)
    #define CPU_WFI_EN_CPU_WFI_EN                              Fld(8, 0) //[7:0]

#define SPM_REG_CPU_WFI_EN_SET                                 (SPM_BASE_ADDRESS + 0x00208)
    #define CPU_WFI_EN_SET_CPU_WFI_EN_SET                      Fld(8, 0) //[7:0]

#define SPM_REG_CPU_WFI_EN_CLR                                 (SPM_BASE_ADDRESS + 0x0020C)
    #define CPU_WFI_EN_CLR_CPU_WFI_EN_CLR                      Fld(8, 0) //[7:0]

#define SPM_REG_EXT_INT_WAKEUP_REQ                             (SPM_BASE_ADDRESS + 0x00210)
    #define EXT_INT_WAKEUP_REQ_EXT_INT_WAKEUP_REQ              Fld(10, 0) //[9:0]

#define SPM_REG_EXT_INT_WAKEUP_REQ_SET                         (SPM_BASE_ADDRESS + 0x00214)
    #define EXT_INT_WAKEUP_REQ_SET_EXT_INT_WAKEUP_REQ_SET      Fld(10, 0) //[9:0]

#define SPM_REG_EXT_INT_WAKEUP_REQ_CLR                         (SPM_BASE_ADDRESS + 0x00218)
    #define EXT_INT_WAKEUP_REQ_CLR_EXT_INT_WAKEUP_REQ_CLR      Fld(10, 0) //[9:0]

#define SPM_REG_MCUSYS_IDLE_STA                                (SPM_BASE_ADDRESS + 0x0021C)
    #define MCUSYS_IDLE_STA_MP0_CPU_IDLE_TO_PWR_OFF            Fld(8, 16) //[23:16]
    #define MCUSYS_IDLE_STA_MP0_CLUSTER_IDLE_TO_PWR_OFF        Fld(1, 9) //[9:9]
    #define MCUSYS_IDLE_STA_ARMBUS_IDLE_TO_26M                 Fld(1, 8) //[8:8]
    #define MCUSYS_IDLE_STA_MCUSYS_DDREN                       Fld(8, 0) //[7:0]

#define SPM_REG_CPU_PWR_STATUS                                 (SPM_BASE_ADDRESS + 0x00220)
    #define CPU_PWR_STATUS_MP0_SPMC_PWR_ON_ACK_CPU0            Fld(1, 0) //[0:0]
    #define CPU_PWR_STATUS_MP0_SPMC_PWR_ON_ACK_CPU1            Fld(1, 1) //[1:1]
    #define CPU_PWR_STATUS_MP0_SPMC_PWR_ON_ACK_CPU2            Fld(1, 2) //[2:2]
    #define CPU_PWR_STATUS_MP0_SPMC_PWR_ON_ACK_CPU3            Fld(1, 3) //[3:3]
    #define CPU_PWR_STATUS_MP0_SPMC_PWR_ON_ACK_CPU4            Fld(1, 4) //[4:4]
    #define CPU_PWR_STATUS_MP0_SPMC_PWR_ON_ACK_CPU5            Fld(1, 5) //[5:5]
    #define CPU_PWR_STATUS_MP0_SPMC_PWR_ON_ACK_CPU6            Fld(1, 6) //[6:6]
    #define CPU_PWR_STATUS_MP0_SPMC_PWR_ON_ACK_CPU7            Fld(1, 7) //[7:7]
    #define CPU_PWR_STATUS_MP0_SPMC_PWR_ON_ACK_CPUTOP          Fld(1, 8) //[8:8]
    #define CPU_PWR_STATUS_MCUSYS_SPMC_PWR_ON_ACK              Fld(1, 9) //[9:9]

#define SPM_REG_SW2SPM_WAKEUP                                  (SPM_BASE_ADDRESS + 0x00224)
    #define SW2SPM_WAKEUP_SW2SPM_WAKEUP                        Fld(4, 0) //[3:0]

#define SPM_REG_SW2SPM_WAKEUP_SET                              (SPM_BASE_ADDRESS + 0x00228)
    #define SW2SPM_WAKEUP_SET_SW2SPM_WAKEUP_SET                Fld(4, 0) //[3:0]

#define SPM_REG_SW2SPM_WAKEUP_CLR                              (SPM_BASE_ADDRESS + 0x0022C)
    #define SW2SPM_WAKEUP_CLR_SW2SPM_WAKEUP_CLR                Fld(4, 0) //[3:0]

#define SPM_REG_SW2SPM_MAILBOX_0                               (SPM_BASE_ADDRESS + 0x00230)
    #define SW2SPM_MAILBOX_0_SW2SPM_MAILBOX_0                  Fld(32, 0) //[31:0]

#define SPM_REG_SW2SPM_MAILBOX_1                               (SPM_BASE_ADDRESS + 0x00234)
    #define SW2SPM_MAILBOX_1_SW2SPM_MAILBOX_1                  Fld(32, 0) //[31:0]

#define SPM_REG_SW2SPM_MAILBOX_2                               (SPM_BASE_ADDRESS + 0x00238)
    #define SW2SPM_MAILBOX_2_SW2SPM_MAILBOX_2                  Fld(32, 0) //[31:0]

#define SPM_REG_SW2SPM_MAILBOX_3                               (SPM_BASE_ADDRESS + 0x0023C)
    #define SW2SPM_MAILBOX_3_SW2SPM_MAILBOX_3                  Fld(32, 0) //[31:0]

#define SPM_REG_SPM2SW_MAILBOX_0                               (SPM_BASE_ADDRESS + 0x00240)
    #define SPM2SW_MAILBOX_0_SPM2SW_MAILBOX_0                  Fld(32, 0) //[31:0]

#define SPM_REG_SPM2SW_MAILBOX_1                               (SPM_BASE_ADDRESS + 0x00244)
    #define SPM2SW_MAILBOX_1_SPM2SW_MAILBOX_1                  Fld(32, 0) //[31:0]

#define SPM_REG_SPM2SW_MAILBOX_2                               (SPM_BASE_ADDRESS + 0x00248)
    #define SPM2SW_MAILBOX_2_SPM2SW_MAILBOX_2                  Fld(32, 0) //[31:0]

#define SPM_REG_SPM2SW_MAILBOX_3                               (SPM_BASE_ADDRESS + 0x0024C)
    #define SPM2SW_MAILBOX_3_SPM2SW_MAILBOX_3                  Fld(32, 0) //[31:0]

#define SPM_REG_SPM2MCUPM_CON                                  (SPM_BASE_ADDRESS + 0x00250)
    #define SPM2MCUPM_CON_MCUPM_WFI                            Fld(1, 16) //[16:16]
    #define SPM2MCUPM_CON_SPM2MCUPM_SW_INT                     Fld(1, 1) //[1:1]
    #define SPM2MCUPM_CON_SPM2MCUPM_SW_RST_B                   Fld(1, 0) //[0:0]

#define SPM_REG_SPM_MCUSYS_PWR_CON                             (SPM_BASE_ADDRESS + 0x00260)
    #define SPM_MCUSYS_PWR_CON_MCUSYS_SPMC_PWR_ON              Fld(1, 2) //[2:2]
    #define SPM_MCUSYS_PWR_CON_MCUSYS_SPMC_RESET_PWRON_CONFIG  Fld(1, 5) //[5:5]
    #define SPM_MCUSYS_PWR_CON_MCUSYS_SPMC_PWR_ON_ACK          Fld(1, 31) //[31:31]

#define SPM_REG_SPM_CPUTOP_PWR_CON                             (SPM_BASE_ADDRESS + 0x00264)
    #define SPM_CPUTOP_PWR_CON_MP0_SPMC_PWR_ON_CPUTOP          Fld(1, 2) //[2:2]
    #define SPM_CPUTOP_PWR_CON_MP0_SPMC_RESET_PWRON_CONFIG_CPUTOP Fld(1, 5) //[5:5]
    #define SPM_CPUTOP_PWR_CON_MP0_SPMC_PWR_ON_ACK_CPUTOP      Fld(1, 31) //[31:31]

#define SPM_REG_SPM_CPU0_PWR_CON                               (SPM_BASE_ADDRESS + 0x00268)
    #define SPM_CPU0_PWR_CON_MP0_SPMC_PWR_ON_CPU0              Fld(1, 2) //[2:2]
    #define SPM_CPU0_PWR_CON_MP0_SPMC_RESET_PWRON_CONFIG_CPU0  Fld(1, 5) //[5:5]
    #define SPM_CPU0_PWR_CON_MP0_SPMC_PWR_ON_ACK_CPU0          Fld(1, 31) //[31:31]

#define SPM_REG_SPM_CPU1_PWR_CON                               (SPM_BASE_ADDRESS + 0x0026C)
    #define SPM_CPU1_PWR_CON_MP0_SPMC_PWR_ON_CPU1              Fld(1, 2) //[2:2]
    #define SPM_CPU1_PWR_CON_MP0_SPMC_RESET_PWRON_CONFIG_CPU1  Fld(1, 5) //[5:5]
    #define SPM_CPU1_PWR_CON_MP0_SPMC_PWR_ON_ACK_CPU1          Fld(1, 31) //[31:31]

#define SPM_REG_SPM_CPU2_PWR_CON                               (SPM_BASE_ADDRESS + 0x00270)
    #define SPM_CPU2_PWR_CON_MP0_SPMC_PWR_ON_CPU2              Fld(1, 2) //[2:2]
    #define SPM_CPU2_PWR_CON_MP0_SPMC_RESET_PWRON_CONFIG_CPU2  Fld(1, 5) //[5:5]
    #define SPM_CPU2_PWR_CON_MP0_SPMC_PWR_ON_ACK_CPU2          Fld(1, 31) //[31:31]

#define SPM_REG_SPM_CPU3_PWR_CON                               (SPM_BASE_ADDRESS + 0x00274)
    #define SPM_CPU3_PWR_CON_MP0_SPMC_PWR_ON_CPU3              Fld(1, 2) //[2:2]
    #define SPM_CPU3_PWR_CON_MP0_SPMC_RESET_PWRON_CONFIG_CPU3  Fld(1, 5) //[5:5]
    #define SPM_CPU3_PWR_CON_MP0_SPMC_PWR_ON_ACK_CPU3          Fld(1, 31) //[31:31]

#define SPM_REG_SPM_CPU4_PWR_CON                               (SPM_BASE_ADDRESS + 0x00278)
    #define SPM_CPU4_PWR_CON_MP0_SPMC_PWR_ON_CPU4              Fld(1, 2) //[2:2]
    #define SPM_CPU4_PWR_CON_MP0_SPMC_RESET_PWRON_CONFIG_CPU4  Fld(1, 5) //[5:5]
    #define SPM_CPU4_PWR_CON_MP0_SPMC_PWR_ON_ACK_CPU4          Fld(1, 31) //[31:31]

#define SPM_REG_SPM_CPU5_PWR_CON                               (SPM_BASE_ADDRESS + 0x0027C)
    #define SPM_CPU5_PWR_CON_MP0_SPMC_PWR_ON_CPU5              Fld(1, 2) //[2:2]
    #define SPM_CPU5_PWR_CON_MP0_SPMC_RESET_PWRON_CONFIG_CPU5  Fld(1, 5) //[5:5]
    #define SPM_CPU5_PWR_CON_MP0_SPMC_PWR_ON_ACK_CPU5          Fld(1, 31) //[31:31]

#define SPM_REG_SPM_CPU6_PWR_CON                               (SPM_BASE_ADDRESS + 0x00280)
    #define SPM_CPU6_PWR_CON_MP0_SPMC_PWR_ON_CPU6              Fld(1, 2) //[2:2]
    #define SPM_CPU6_PWR_CON_MP0_SPMC_RESET_PWRON_CONFIG_CPU6  Fld(1, 5) //[5:5]
    #define SPM_CPU6_PWR_CON_MP0_SPMC_PWR_ON_ACK_CPU6          Fld(1, 31) //[31:31]

#define SPM_REG_SPM_CPU7_PWR_CON                               (SPM_BASE_ADDRESS + 0x00284)
    #define SPM_CPU7_PWR_CON_MP0_SPMC_PWR_ON_CPU7              Fld(1, 2) //[2:2]
    #define SPM_CPU7_PWR_CON_MP0_SPMC_RESET_PWRON_CONFIG_CPU7  Fld(1, 5) //[5:5]
    #define SPM_CPU7_PWR_CON_MP0_SPMC_PWR_ON_ACK_CPU7          Fld(1, 31) //[31:31]

#define SPM_REG_SPM_DPM_P2P_STA                                (SPM_BASE_ADDRESS + 0x002A0)
    #define SPM_DPM_P2P_STA_P2P_TX_STA                         Fld(32, 0) //[31:0]

#define SPM_REG_SPM_DPM_P2P_CON                                (SPM_BASE_ADDRESS + 0x002A4)
    #define SPM_DPM_P2P_CON_REG_P2P_TX_ERROR_FLAG_EN           Fld(1, 0) //[0:0]

#define SPM_REG_SPM_DPM_INTF_STA                               (SPM_BASE_ADDRESS + 0x002A8)
    #define SPM_DPM_INTF_STA_SC_HW_S1_ACK                      Fld(4, 8) //[11:8]
    #define SPM_DPM_INTF_STA_REG_HW_S1_ACK_MASK                Fld(4, 4) //[7:4]
    #define SPM_DPM_INTF_STA_SC_HW_S1_REQ                      Fld(1, 0) //[0:0]

#define SPM_REG_SPM_PWRAP_CON                                  (SPM_BASE_ADDRESS + 0x00300)
    #define SPM_PWRAP_CON_SPM_PWRAP_CON                        Fld(32, 0) //[31:0]

#define SPM_REG_SPM_PWRAP_CON_STA                              (SPM_BASE_ADDRESS + 0x00304)
    #define SPM_PWRAP_CON_STA_SPM_PWRAP_CON_STA                Fld(32, 0) //[31:0]

#define SPM_REG_SPM_PMIC_SPMI_CON                              (SPM_BASE_ADDRESS + 0x00308)
    #define SPM_PMIC_SPMI_CON_SPM_PMIC_SPMI_DBCNT              Fld(1, 7) //[7:7]
    #define SPM_PMIC_SPMI_CON_SPM_PMIC_SPMI_PMIFID             Fld(1, 6) //[6:6]
    #define SPM_PMIC_SPMI_CON_SPM_PMIC_SPMI_SLAVEID            Fld(4, 2) //[5:2]
    #define SPM_PMIC_SPMI_CON_SPM_PMIC_SPMI_CMD                Fld(2, 0) //[1:0]

#define SPM_REG_SPM_PWRAP_CMD0                                 (SPM_BASE_ADDRESS + 0x00310)
    #define SPM_PWRAP_CMD0_SPM_PWRAP_CMD0                      Fld(32, 0) //[31:0]

#define SPM_REG_SPM_PWRAP_CMD1                                 (SPM_BASE_ADDRESS + 0x00314)
    #define SPM_PWRAP_CMD1_SPM_PWRAP_CMD1                      Fld(32, 0) //[31:0]

#define SPM_REG_SPM_PWRAP_CMD2                                 (SPM_BASE_ADDRESS + 0x00318)
    #define SPM_PWRAP_CMD2_SPM_PWRAP_CMD2                      Fld(32, 0) //[31:0]

#define SPM_REG_SPM_PWRAP_CMD3                                 (SPM_BASE_ADDRESS + 0x0031C)
    #define SPM_PWRAP_CMD3_SPM_PWRAP_CMD3                      Fld(32, 0) //[31:0]

#define SPM_REG_SPM_PWRAP_CMD4                                 (SPM_BASE_ADDRESS + 0x00320)
    #define SPM_PWRAP_CMD4_SPM_PWRAP_CMD4                      Fld(32, 0) //[31:0]

#define SPM_REG_SPM_PWRAP_CMD5                                 (SPM_BASE_ADDRESS + 0x00324)
    #define SPM_PWRAP_CMD5_SPM_PWRAP_CMD5                      Fld(32, 0) //[31:0]

#define SPM_REG_SPM_PWRAP_CMD6                                 (SPM_BASE_ADDRESS + 0x00328)
    #define SPM_PWRAP_CMD6_SPM_PWRAP_CMD6                      Fld(32, 0) //[31:0]

#define SPM_REG_SPM_PWRAP_CMD7                                 (SPM_BASE_ADDRESS + 0x0032C)
    #define SPM_PWRAP_CMD7_SPM_PWRAP_CMD7                      Fld(32, 0) //[31:0]

#define SPM_REG_SPM_PWRAP_CMD8                                 (SPM_BASE_ADDRESS + 0x00330)
    #define SPM_PWRAP_CMD8_SPM_PWRAP_CMD8                      Fld(32, 0) //[31:0]

#define SPM_REG_SPM_PWRAP_CMD9                                 (SPM_BASE_ADDRESS + 0x00334)
    #define SPM_PWRAP_CMD9_SPM_PWRAP_CMD9                      Fld(32, 0) //[31:0]

#define SPM_REG_SPM_PWRAP_CMD10                                (SPM_BASE_ADDRESS + 0x00338)
    #define SPM_PWRAP_CMD10_SPM_PWRAP_CMD10                    Fld(32, 0) //[31:0]

#define SPM_REG_SPM_PWRAP_CMD11                                (SPM_BASE_ADDRESS + 0x0033C)
    #define SPM_PWRAP_CMD11_SPM_PWRAP_CMD11                    Fld(32, 0) //[31:0]

#define SPM_REG_SPM_PWRAP_CMD12                                (SPM_BASE_ADDRESS + 0x00340)
    #define SPM_PWRAP_CMD12_SPM_PWRAP_CMD12                    Fld(32, 0) //[31:0]

#define SPM_REG_SPM_PWRAP_CMD13                                (SPM_BASE_ADDRESS + 0x00344)
    #define SPM_PWRAP_CMD13_SPM_PWRAP_CMD13                    Fld(32, 0) //[31:0]

#define SPM_REG_SPM_PWRAP_CMD14                                (SPM_BASE_ADDRESS + 0x00348)
    #define SPM_PWRAP_CMD14_SPM_PWRAP_CMD14                    Fld(32, 0) //[31:0]

#define SPM_REG_SPM_PWRAP_CMD15                                (SPM_BASE_ADDRESS + 0x0034C)
    #define SPM_PWRAP_CMD15_SPM_PWRAP_CMD15                    Fld(32, 0) //[31:0]

#define SPM_REG_SPM_PWRAP_CMD16                                (SPM_BASE_ADDRESS + 0x00350)
    #define SPM_PWRAP_CMD16_SPM_PWRAP_CMD16                    Fld(32, 0) //[31:0]

#define SPM_REG_SPM_PWRAP_CMD17                                (SPM_BASE_ADDRESS + 0x00354)
    #define SPM_PWRAP_CMD17_SPM_PWRAP_CMD17                    Fld(32, 0) //[31:0]

#define SPM_REG_SPM_PWRAP_CMD18                                (SPM_BASE_ADDRESS + 0x00358)
    #define SPM_PWRAP_CMD18_SPM_PWRAP_CMD18                    Fld(32, 0) //[31:0]

#define SPM_REG_SPM_PWRAP_CMD19                                (SPM_BASE_ADDRESS + 0x0035C)
    #define SPM_PWRAP_CMD19_SPM_PWRAP_CMD19                    Fld(32, 0) //[31:0]

#define SPM_REG_SPM_PWRAP_CMD20                                (SPM_BASE_ADDRESS + 0x00360)
    #define SPM_PWRAP_CMD20_SPM_PWRAP_CMD20                    Fld(32, 0) //[31:0]

#define SPM_REG_SPM_PWRAP_CMD21                                (SPM_BASE_ADDRESS + 0x00364)
    #define SPM_PWRAP_CMD21_SPM_PWRAP_CMD21                    Fld(32, 0) //[31:0]

#define SPM_REG_SPM_PWRAP_CMD22                                (SPM_BASE_ADDRESS + 0x00368)
    #define SPM_PWRAP_CMD22_SPM_PWRAP_CMD22                    Fld(32, 0) //[31:0]

#define SPM_REG_SPM_PWRAP_CMD23                                (SPM_BASE_ADDRESS + 0x0036C)
    #define SPM_PWRAP_CMD23_SPM_PWRAP_CMD23                    Fld(32, 0) //[31:0]

#define SPM_REG_DVFSRC_EVENT_STA                               (SPM_BASE_ADDRESS + 0x00380)
    #define DVFSRC_EVENT_STA_DVFSRC_EVENT                      Fld(32, 0) //[31:0]

#define SPM_REG_SPM_FORCE_DVFS                                 (SPM_BASE_ADDRESS + 0x00384)
    #define SPM_FORCE_DVFS_FORCE_DVFS_LEVEL                    Fld(32, 0) //[31:0]

#define SPM_REG_SPM_DVFS_STA                                   (SPM_BASE_ADDRESS + 0x00388)
    #define SPM_DVFS_STA_TARGET_DVFS_LEVEL                     Fld(32, 0) //[31:0]

#define SPM_REG_SPM_DVS_DFS_LEVEL                              (SPM_BASE_ADDRESS + 0x0038C)
    #define SPM_DVS_DFS_LEVEL_SPM_DVS_LEVEL                    Fld(16, 16) //[31:16]
    #define SPM_DVS_DFS_LEVEL_SPM_DFS_LEVEL                    Fld(16, 0) //[15:0]

#define SPM_REG_SPM_DVFS_LEVEL                                 (SPM_BASE_ADDRESS + 0x00390)
    #define SPM_DVFS_LEVEL_SPM_DVFS_LEVEL                      Fld(32, 0) //[31:0]

#define SPM_REG_SPM_DVFS_OPP                                   (SPM_BASE_ADDRESS + 0x00394)
    #define SPM_DVFS_OPP_SPM_DVFS_OPP                          Fld(5, 0) //[4:0]

#define SPM_REG_SPM_ULTRA_REQ                                  (SPM_BASE_ADDRESS + 0x00398)
    #define SPM_ULTRA_REQ_SPM2MD_ULTRAACK_D2T                  Fld(1, 20) //[20:20]
    #define SPM_ULTRA_REQ_SPM2MM_ULTRAACK_D2T                  Fld(1, 19) //[19:19]
    #define SPM_ULTRA_REQ_SPM2ISP_ULTRAACK_D2T                 Fld(1, 18) //[18:18]
    #define SPM_ULTRA_REQ_SPM2ISP_ULTRAREQ                     Fld(1, 4) //[4:4]
    #define SPM_ULTRA_REQ_SPM2MD_ULTRAREQ                      Fld(1, 3) //[3:3]
    #define SPM_ULTRA_REQ_SPM2MM_ULTRAREQ                      Fld(1, 2) //[2:2]
    #define SPM_ULTRA_REQ_SPM2MM_DBL_OSTD_ACT                  Fld(1, 1) //[1:1]
    #define SPM_ULTRA_REQ_SPM2MM_FORCE_ULTRA                   Fld(1, 0) //[0:0]

#define SPM_REG_SPM_DVFS_CON                                   (SPM_BASE_ADDRESS + 0x0039C)
    #define SPM_DVFS_CON_DVFSRC_LEVEL_ACK                      Fld(1, 8) //[8:8]
    #define SPM_DVFS_CON_SPM2RC_EVENT_ABORT                    Fld(1, 7) //[7:7]
    #define SPM_DVFS_CON_DVFSRC_WAKEUP_EVENT_MASK              Fld(1, 6) //[6:6]
    #define SPM_DVFS_CON_SPM_DVFSRC_ENABLE                     Fld(1, 4) //[4:4]
    #define SPM_DVFS_CON_FORCE_DVFS_WAKE                       Fld(1, 3) //[3:3]
    #define SPM_DVFS_CON_SPM_DVFS_FORCE_ENABLE                 Fld(1, 2) //[2:2]

#define SPM_REG_ULPOSC_CON                                     (SPM_BASE_ADDRESS + 0x00400)
    #define ULPOSC_CON_ULPOSC_CLK_SEL                          Fld(1, 3) //[3:3]
    #define ULPOSC_CON_ULPOSC_CG_EN                            Fld(1, 2) //[2:2]
    #define ULPOSC_CON_ULPOSC_RST                              Fld(1, 1) //[1:1]
    #define ULPOSC_CON_ULPOSC_EN                               Fld(1, 0) //[0:0]

#define SPM_REG_AP_MDSRC_REQ                                   (SPM_BASE_ADDRESS + 0x00404)
    #define AP_MDSRC_REQ_AP_L1SMSRC_ACK                        Fld(1, 5) //[5:5]
    #define AP_MDSRC_REQ_AP_MDSMSRC_ACK                        Fld(1, 4) //[4:4]
    #define AP_MDSRC_REQ_AP2MD_PEER_WAKEUP                     Fld(1, 3) //[3:3]
    #define AP_MDSRC_REQ_AP_L1SMSRC_REQ                        Fld(1, 1) //[1:1]
    #define AP_MDSRC_REQ_AP_MDSMSRC_REQ                        Fld(1, 0) //[0:0]

#define SPM_REG_SPM2MD_SWITCH_CTRL                             (SPM_BASE_ADDRESS + 0x00408)
    #define SPM2MD_SWITCH_CTRL_SPM2MD_SWITCH_CTRL              Fld(10, 0) //[9:0]

#define SPM_REG_RC_SPM_CTRL                                    (SPM_BASE_ADDRESS + 0x0040C)
    #define RC_SPM_CTRL_RC2SPM_SRCCLKENO_0_ACK                 Fld(1, 16) //[16:16]
    #define RC_SPM_CTRL_SPM2RC_DMY_CTRL                        Fld(6, 2) //[7:2]
    #define RC_SPM_CTRL_SPM_AP_26M_RDY                         Fld(1, 0) //[0:0]

#define SPM_REG_SPM2GPUPM_CON                                  (SPM_BASE_ADDRESS + 0x00410)
    #define SPM2GPUPM_CON_GPUEB_WFI                            Fld(1, 16) //[16:16]
    #define SPM2GPUPM_CON_SC_MFG_PLL_EN                        Fld(1, 4) //[4:4]
    #define SPM2GPUPM_CON_SPM2GPUEB_SW_INT                     Fld(1, 1) //[1:1]
    #define SPM2GPUPM_CON_SPM2GPUEB_SW_RST_B                   Fld(1, 0) //[0:0]

#define SPM_REG_SPM2APU_CON                                    (SPM_BASE_ADDRESS + 0x00414)
    #define SPM2APU_CON_APU_ACTIVE_STATE                       Fld(1, 9) //[9:9]
    #define SPM2APU_CON_APU_ARE_ACK                            Fld(1, 8) //[8:8]
    #define SPM2APU_CON_APU_ARE_REQ                            Fld(1, 4) //[4:4]
    #define SPM2APU_CON_APU_VCORE_OFF_ISO_EN                   Fld(1, 1) //[1:1]
    #define SPM2APU_CON_RPC_SRAM_CTRL_MUX_SEL                  Fld(1, 0) //[0:0]

#define SPM_REG_SPM2EFUSE_CON                                  (SPM_BASE_ADDRESS + 0x00418)
    #define SPM2EFUSE_CON_AOC_EFUSE_RESTORE_RDY                Fld(1, 1) //[1:1]
    #define SPM2EFUSE_CON_AOC_EFUSE_EN                         Fld(1, 0) //[0:0]

#define SPM_REG_SPM2DFD_CON                                    (SPM_BASE_ADDRESS + 0x0041C)
    #define SPM2DFD_CON_DFD_SOC_MTCMOS_REQ                     Fld(1, 1) //[1:1]
    #define SPM2DFD_CON_DFD_SOC_MTCMOS_ACK                     Fld(1, 0) //[0:0]

#define SPM_REG_REG_MODULE_SW_CG_DDREN_REQ_MASK_0              (SPM_BASE_ADDRESS + 0x004A0)
    #define REG_MODULE_SW_CG_DDREN_REQ_MASK_0_REG_MODULE_SW_CG_DDREN_REQ_MASK_0 Fld(32, 0) //[31:0]

#define SPM_REG_REG_MODULE_SW_CG_DDREN_REQ_MASK_1              (SPM_BASE_ADDRESS + 0x004A4)
    #define REG_MODULE_SW_CG_DDREN_REQ_MASK_1_REG_MODULE_SW_CG_DDREN_REQ_MASK_1 Fld(32, 0) //[31:0]

#define SPM_REG_REG_MODULE_SW_CG_DDREN_REQ_MASK_2              (SPM_BASE_ADDRESS + 0x004A8)
    #define REG_MODULE_SW_CG_DDREN_REQ_MASK_2_REG_MODULE_SW_CG_DDREN_REQ_MASK_2 Fld(32, 0) //[31:0]

#define SPM_REG_REG_MODULE_SW_CG_DDREN_REQ_MASK_3              (SPM_BASE_ADDRESS + 0x004AC)
    #define REG_MODULE_SW_CG_DDREN_REQ_MASK_3_REG_MODULE_SW_CG_DDREN_REQ_MASK_3 Fld(32, 0) //[31:0]

#define SPM_REG_REG_MODULE_SW_CG_VRF18_REQ_MASK_0              (SPM_BASE_ADDRESS + 0x004B0)
    #define REG_MODULE_SW_CG_VRF18_REQ_MASK_0_REG_MODULE_SW_CG_VRF18_REQ_MASK_0 Fld(32, 0) //[31:0]

#define SPM_REG_REG_MODULE_SW_CG_VRF18_REQ_MASK_1              (SPM_BASE_ADDRESS + 0x004B4)
    #define REG_MODULE_SW_CG_VRF18_REQ_MASK_1_REG_MODULE_SW_CG_VRF18_REQ_MASK_1 Fld(32, 0) //[31:0]

#define SPM_REG_REG_MODULE_SW_CG_VRF18_REQ_MASK_2              (SPM_BASE_ADDRESS + 0x004B8)
    #define REG_MODULE_SW_CG_VRF18_REQ_MASK_2_REG_MODULE_SW_CG_VRF18_REQ_MASK_2 Fld(32, 0) //[31:0]

#define SPM_REG_REG_MODULE_SW_CG_VRF18_REQ_MASK_3              (SPM_BASE_ADDRESS + 0x004BC)
    #define REG_MODULE_SW_CG_VRF18_REQ_MASK_3_REG_MODULE_SW_CG_VRF18_REQ_MASK_3 Fld(32, 0) //[31:0]

#define SPM_REG_REG_MODULE_SW_CG_F26M_REQ_MASK_0               (SPM_BASE_ADDRESS + 0x004C0)
    #define REG_MODULE_SW_CG_F26M_REQ_MASK_0_REG_MODULE_SW_CG_F26M_REQ_MASK_0 Fld(32, 0) //[31:0]

#define SPM_REG_REG_MODULE_SW_CG_F26M_REQ_MASK_1               (SPM_BASE_ADDRESS + 0x004C4)
    #define REG_MODULE_SW_CG_F26M_REQ_MASK_1_REG_MODULE_SW_CG_F26M_REQ_MASK_1 Fld(32, 0) //[31:0]

#define SPM_REG_REG_MODULE_SW_CG_F26M_REQ_MASK_2               (SPM_BASE_ADDRESS + 0x004C8)
    #define REG_MODULE_SW_CG_F26M_REQ_MASK_2_REG_MODULE_SW_CG_F26M_REQ_MASK_2 Fld(32, 0) //[31:0]

#define SPM_REG_REG_MODULE_SW_CG_F26M_REQ_MASK_3               (SPM_BASE_ADDRESS + 0x004CC)
    #define REG_MODULE_SW_CG_F26M_REQ_MASK_3_REG_MODULE_SW_CG_F26M_REQ_MASK_3 Fld(32, 0) //[31:0]

#define SPM_REG_REG_MODULE_SW_CG_VCORE_REQ_MASK_0              (SPM_BASE_ADDRESS + 0x004D0)
    #define REG_MODULE_SW_CG_VCORE_REQ_MASK_0_REG_MODULE_SW_CG_VCORE_REQ_MASK_0 Fld(32, 0) //[31:0]

#define SPM_REG_REG_MODULE_SW_CG_VCORE_REQ_MASK_1              (SPM_BASE_ADDRESS + 0x004D4)
    #define REG_MODULE_SW_CG_VCORE_REQ_MASK_1_REG_MODULE_SW_CG_VCORE_REQ_MASK_1 Fld(32, 0) //[31:0]

#define SPM_REG_REG_MODULE_SW_CG_VCORE_REQ_MASK_2              (SPM_BASE_ADDRESS + 0x004D8)
    #define REG_MODULE_SW_CG_VCORE_REQ_MASK_2_REG_MODULE_SW_CG_VCORE_REQ_MASK_2 Fld(32, 0) //[31:0]

#define SPM_REG_REG_MODULE_SW_CG_VCORE_REQ_MASK_3              (SPM_BASE_ADDRESS + 0x004DC)
    #define REG_MODULE_SW_CG_VCORE_REQ_MASK_3_REG_MODULE_SW_CG_VCORE_REQ_MASK_3 Fld(32, 0) //[31:0]

#define SPM_REG_REG_PWR_STATUS_DDREN_REQ_MASK                  (SPM_BASE_ADDRESS + 0x004E0)
    #define REG_PWR_STATUS_DDREN_REQ_MASK_REG_PWR_STATUS_DDREN_REQ_MASK Fld(32, 0) //[31:0]

#define SPM_REG_REG_PWR_STATUS_VRF18_REQ_MASK                  (SPM_BASE_ADDRESS + 0x004E4)
    #define REG_PWR_STATUS_VRF18_REQ_MASK_REG_PWR_STATUS_VRF18_REQ_MASK Fld(32, 0) //[31:0]

#define SPM_REG_REG_PWR_STATUS_F26M_REQ_MASK                   (SPM_BASE_ADDRESS + 0x004E8)
    #define REG_PWR_STATUS_F26M_REQ_MASK_REG_PWR_STATUS_F26M_REQ_MASK Fld(32, 0) //[31:0]

#define SPM_REG_REG_PWR_STATUS_VCORE_REQ_MASK                  (SPM_BASE_ADDRESS + 0x004EC)
    #define REG_PWR_STATUS_VCORE_REQ_MASK_REG_PWR_STATUS_VCORE_REQ_MASK Fld(32, 0) //[31:0]

#define SPM_REG_REG_MODULE_BUSY_DDREN_REQ_MASK                 (SPM_BASE_ADDRESS + 0x004F0)
    #define REG_MODULE_BUSY_DDREN_REQ_MASK_REG_MODULE_BUSY_DDREN_REQ_MASK Fld(32, 0) //[31:0]

#define SPM_REG_REG_MODULE_BUSY_VRF18_REQ_MASK                 (SPM_BASE_ADDRESS + 0x004F4)
    #define REG_MODULE_BUSY_VRF18_REQ_MASK_REG_MODULE_BUSY_VRF18_REQ_MASK Fld(32, 0) //[31:0]

#define SPM_REG_REG_MODULE_BUSY_F26M_REQ_MASK                  (SPM_BASE_ADDRESS + 0x004F8)
    #define REG_MODULE_BUSY_F26M_REQ_MASK_REG_MODULE_BUSY_F26M_REQ_MASK Fld(32, 0) //[31:0]

#define SPM_REG_REG_MODULE_BUSY_VCORE_REQ_MASK                 (SPM_BASE_ADDRESS + 0x004FC)
    #define REG_MODULE_BUSY_VCORE_REQ_MASK_REG_MODULE_BUSY_VCORE_REQ_MASK Fld(32, 0) //[31:0]

#define SPM_REG_SPM_SW_FLAG_0                                  (SPM_BASE_ADDRESS + 0x00600)
    #define SPM_SW_FLAG_0_SPM_SW_FLAG                          Fld(32, 0) //[31:0]

#define SPM_REG_SPM_SW_DEBUG_0                                 (SPM_BASE_ADDRESS + 0x00604)
    #define SPM_SW_DEBUG_0_SPM_SW_DEBUG_0                      Fld(32, 0) //[31:0]

#define SPM_REG_SPM_SW_FLAG_1                                  (SPM_BASE_ADDRESS + 0x00608)
    #define SPM_SW_FLAG_1_SPM_SW_FLAG_1                        Fld(32, 0) //[31:0]

#define SPM_REG_SPM_SW_DEBUG_1                                 (SPM_BASE_ADDRESS + 0x0060C)
    #define SPM_SW_DEBUG_1_SPM_SW_DEBUG_1                      Fld(32, 0) //[31:0]

#define SPM_REG_SPM_SW_RSV_0                                   (SPM_BASE_ADDRESS + 0x00610)
    #define SPM_SW_RSV_0_SPM_SW_RSV_0                          Fld(32, 0) //[31:0]

#define SPM_REG_SPM_SW_RSV_1                                   (SPM_BASE_ADDRESS + 0x00614)
    #define SPM_SW_RSV_1_SPM_SW_RSV_1                          Fld(32, 0) //[31:0]

#define SPM_REG_SPM_SW_RSV_2                                   (SPM_BASE_ADDRESS + 0x00618)
    #define SPM_SW_RSV_2_SPM_SW_RSV_2                          Fld(32, 0) //[31:0]

#define SPM_REG_SPM_SW_RSV_3                                   (SPM_BASE_ADDRESS + 0x0061C)
    #define SPM_SW_RSV_3_SPM_SW_RSV_3                          Fld(32, 0) //[31:0]

#define SPM_REG_SPM_SW_RSV_4                                   (SPM_BASE_ADDRESS + 0x00620)
    #define SPM_SW_RSV_4_SPM_SW_RSV_4                          Fld(32, 0) //[31:0]

#define SPM_REG_SPM_SW_RSV_5                                   (SPM_BASE_ADDRESS + 0x00624)
    #define SPM_SW_RSV_5_SPM_SW_RSV_5                          Fld(32, 0) //[31:0]

#define SPM_REG_SPM_SW_RSV_6                                   (SPM_BASE_ADDRESS + 0x00628)
    #define SPM_SW_RSV_6_SPM_SW_RSV_6                          Fld(32, 0) //[31:0]

#define SPM_REG_SPM_SW_RSV_7                                   (SPM_BASE_ADDRESS + 0x0062C)
    #define SPM_SW_RSV_7_SPM_SW_RSV_7                          Fld(32, 0) //[31:0]

#define SPM_REG_SPM_SW_RSV_8                                   (SPM_BASE_ADDRESS + 0x00630)
    #define SPM_SW_RSV_8_SPM_SW_RSV_8                          Fld(32, 0) //[31:0]

#define SPM_REG_SPM_BK_WAKE_EVENT                              (SPM_BASE_ADDRESS + 0x00634)
    #define SPM_BK_WAKE_EVENT_SPM_BK_WAKE_EVENT                Fld(32, 0) //[31:0]

#define SPM_REG_SPM_BK_VTCXO_DUR                               (SPM_BASE_ADDRESS + 0x00638)
    #define SPM_BK_VTCXO_DUR_SPM_BK_VTCXO_DUR                  Fld(32, 0) //[31:0]

#define SPM_REG_SPM_BK_WAKE_MISC                               (SPM_BASE_ADDRESS + 0x0063C)
    #define SPM_BK_WAKE_MISC_SPM_BK_WAKE_MISC                  Fld(32, 0) //[31:0]

#define SPM_REG_SPM_BK_PCM_TIMER                               (SPM_BASE_ADDRESS + 0x00640)
    #define SPM_BK_PCM_TIMER_SPM_BK_PCM_TIMER                  Fld(32, 0) //[31:0]

#define SPM_REG_SPM_RSV_CON_0                                  (SPM_BASE_ADDRESS + 0x00650)
    #define SPM_RSV_CON_0_SPM_RSV_CON_0                        Fld(32, 0) //[31:0]

#define SPM_REG_SPM_RSV_CON_1                                  (SPM_BASE_ADDRESS + 0x00654)
    #define SPM_RSV_CON_1_SPM_RSV_CON_1                        Fld(32, 0) //[31:0]

#define SPM_REG_SPM_RSV_STA_0                                  (SPM_BASE_ADDRESS + 0x00658)
    #define SPM_RSV_STA_0_SPM_RSV_STA_0                        Fld(32, 0) //[31:0]

#define SPM_REG_SPM_RSV_STA_1                                  (SPM_BASE_ADDRESS + 0x0065C)
    #define SPM_RSV_STA_1_SPM_RSV_STA_1                        Fld(32, 0) //[31:0]

#define SPM_REG_SPM_SPARE_CON                                  (SPM_BASE_ADDRESS + 0x00660)
    #define SPM_SPARE_CON_SPM_SPARE_CON                        Fld(32, 0) //[31:0]

#define SPM_REG_SPM_SPARE_CON_SET                              (SPM_BASE_ADDRESS + 0x00664)
    #define SPM_SPARE_CON_SET_SPM_SPARE_CON_SET                Fld(32, 0) //[31:0]

#define SPM_REG_SPM_SPARE_CON_CLR                              (SPM_BASE_ADDRESS + 0x00668)
    #define SPM_SPARE_CON_CLR_SPM_SPARE_CON_CLR                Fld(32, 0) //[31:0]

#define SPM_REG_SPM_CROSS_WAKE_M00_REQ                         (SPM_BASE_ADDRESS + 0x0066C)
    #define SPM_CROSS_WAKE_M00_REQ_SPM_M0_CROSS_WAKE_REQ       Fld(4, 0) //[3:0]
    #define SPM_CROSS_WAKE_M00_REQ_SPM_CROSS_WAKE_M0_CHK       Fld(4, 4) //[7:4]

#define SPM_REG_SPM_CROSS_WAKE_M01_REQ                         (SPM_BASE_ADDRESS + 0x00670)
    #define SPM_CROSS_WAKE_M01_REQ_SPM_M1_CROSS_WAKE_REQ       Fld(4, 0) //[3:0]
    #define SPM_CROSS_WAKE_M01_REQ_SPM_CROSS_WAKE_M1_CHK       Fld(4, 4) //[7:4]

#define SPM_REG_SPM_CROSS_WAKE_M02_REQ                         (SPM_BASE_ADDRESS + 0x00674)
    #define SPM_CROSS_WAKE_M02_REQ_SPM_M2_CROSS_WAKE_REQ       Fld(4, 0) //[3:0]
    #define SPM_CROSS_WAKE_M02_REQ_SPM_CROSS_WAKE_M2_CHK       Fld(4, 4) //[7:4]

#define SPM_REG_SPM_CROSS_WAKE_M03_REQ                         (SPM_BASE_ADDRESS + 0x00678)
    #define SPM_CROSS_WAKE_M03_REQ_SPM_M3_CROSS_WAKE_REQ       Fld(4, 0) //[3:0]
    #define SPM_CROSS_WAKE_M03_REQ_SPM_CROSS_WAKE_M3_CHK       Fld(4, 4) //[7:4]

#define SPM_REG_SCP_VCORE_LEVEL                                (SPM_BASE_ADDRESS + 0x0067C)
    #define SCP_VCORE_LEVEL_SCP_VCORE_LEVEL                    Fld(16, 0) //[15:0]

#define SPM_REG_SPM_DV_CON_0                                   (SPM_BASE_ADDRESS + 0x0068C)
    #define SPM_DV_CON_0_SPM_DV_CON_0                          Fld(32, 0) //[31:0]

#define SPM_REG_SPM_DV_CON_1                                   (SPM_BASE_ADDRESS + 0x00690)
    #define SPM_DV_CON_1_SPM_DV_CON_1                          Fld(32, 0) //[31:0]

#define SPM_REG_SPM_SEMA_M0                                    (SPM_BASE_ADDRESS + 0x0069C)
    #define SPM_SEMA_M0_SPM_SEMA_M0                            Fld(8, 0) //[7:0]

#define SPM_REG_SPM_SEMA_M1                                    (SPM_BASE_ADDRESS + 0x006A0)
    #define SPM_SEMA_M1_SPM_SEMA_M1                            Fld(8, 0) //[7:0]

#define SPM_REG_SPM_SEMA_M2                                    (SPM_BASE_ADDRESS + 0x006A4)
    #define SPM_SEMA_M2_SPM_SEMA_M2                            Fld(8, 0) //[7:0]

#define SPM_REG_SPM_SEMA_M3                                    (SPM_BASE_ADDRESS + 0x006A8)
    #define SPM_SEMA_M3_SPM_SEMA_M3                            Fld(8, 0) //[7:0]

#define SPM_REG_SPM_SEMA_M4                                    (SPM_BASE_ADDRESS + 0x006AC)
    #define SPM_SEMA_M4_SPM_SEMA_M4                            Fld(8, 0) //[7:0]

#define SPM_REG_SPM_SEMA_M5                                    (SPM_BASE_ADDRESS + 0x006B0)
    #define SPM_SEMA_M5_SPM_SEMA_M5                            Fld(8, 0) //[7:0]

#define SPM_REG_SPM_SEMA_M6                                    (SPM_BASE_ADDRESS + 0x006B4)
    #define SPM_SEMA_M6_SPM_SEMA_M6                            Fld(8, 0) //[7:0]

#define SPM_REG_SPM_SEMA_M7                                    (SPM_BASE_ADDRESS + 0x006B8)
    #define SPM_SEMA_M7_SPM_SEMA_M7                            Fld(8, 0) //[7:0]

#define SPM_REG_SPM2ADSP_MAILBOX                               (SPM_BASE_ADDRESS + 0x006BC)
    #define SPM2ADSP_MAILBOX_SPM2ADSP_MAILBOX                  Fld(32, 0) //[31:0]

#define SPM_REG_ADSP2SPM_MAILBOX                               (SPM_BASE_ADDRESS + 0x006C0)
    #define ADSP2SPM_MAILBOX_ADSP2SPM_MAILBOX                  Fld(32, 0) //[31:0]

#define SPM_REG_SPM2MCUPM_MAILBOX_0                             (SPM_BASE_ADDRESS + 0x006CC)
    #define SPM2MCUPM_MAILBOX_0_SPM2MCUPM_MAILBOX_0              Fld(32, 0) //[31:0]

#define SPM_REG_SPM2MCUPM_MAILBOX_1                             (SPM_BASE_ADDRESS + 0x006D0)
    #define SPM2MCUPM_MAILBOX_1_SPM2MCUPM_MAILBOX_1              Fld(32, 0) //[31:0]

#define SPM_REG_SPM2MCUPM_MAILBOX_2                             (SPM_BASE_ADDRESS + 0x006D4)
    #define SPM2MCUPM_MAILBOX_2_SPM2MCUPM_MAILBOX_2              Fld(32, 0) //[31:0]

#define SPM_REG_SPM2MCUPM_MAILBOX_3                             (SPM_BASE_ADDRESS + 0x006D8)
    #define SPM2MCUPM_MAILBOX_3_SPM2MCUPM_MAILBOX_3              Fld(32, 0) //[31:0]

#define SPM_REG_MCUPM2SPM_MAILBOX_0                             (SPM_BASE_ADDRESS + 0x006DC)
    #define MCUPM2SPM_MAILBOX_0_MCUPM2SPM_MAILBOX_0              Fld(32, 0) //[31:0]

#define SPM_REG_MCUPM2SPM_MAILBOX_1                             (SPM_BASE_ADDRESS + 0x006E0)
    #define MCUPM2SPM_MAILBOX_1_MCUPM2SPM_MAILBOX_1              Fld(32, 0) //[31:0]

#define SPM_REG_MCUPM2SPM_MAILBOX_2                             (SPM_BASE_ADDRESS + 0x006E4)
    #define MCUPM2SPM_MAILBOX_2_MCUPM2SPM_MAILBOX_2              Fld(32, 0) //[31:0]

#define SPM_REG_MCUPM2SPM_MAILBOX_3                             (SPM_BASE_ADDRESS + 0x006E8)
    #define MCUPM2SPM_MAILBOX_3_MCUPM2SPM_MAILBOX_3              Fld(32, 0) //[31:0]

#define SPM_REG_SPM2SCP_MAILBOX                                (SPM_BASE_ADDRESS + 0x006EC)
    #define SPM2SCP_MAILBOX_SPM_SCP_MAILBOX                    Fld(32, 0) //[31:0]

#define SPM_REG_SCP2SPM_MAILBOX                                (SPM_BASE_ADDRESS + 0x006F0)
    #define SCP2SPM_MAILBOX_SCP_SPM_MAILBOX                    Fld(32, 0) //[31:0]

#define SPM_REG_SYS_TIMER_CON                                  (SPM_BASE_ADDRESS + 0x00500)
    #define SYS_TIMER_CON_SYS_TIMER_VALID                      Fld(1, 31) //[31:31]
    #define SYS_TIMER_CON_SYS_TIMER_ID                         Fld(8, 8) //[15:8]
    #define SYS_TIMER_CON_SYS_TIMER_LATCH_EN                   Fld(1, 1) //[1:1]
    #define SYS_TIMER_CON_SYS_TIMER_START_EN                   Fld(1, 0) //[0:0]

#define SPM_REG_SYS_TIMER_VALUE_L                              (SPM_BASE_ADDRESS + 0x00504)
    #define SYS_TIMER_VALUE_L_SYS_TIMER_VALUE_L                Fld(32, 0) //[31:0]

#define SPM_REG_SYS_TIMER_VALUE_H                              (SPM_BASE_ADDRESS + 0x00508)
    #define SYS_TIMER_VALUE_H_SYS_TIMER_VALUE_H                Fld(32, 0) //[31:0]

#define SPM_REG_SYS_TIMER_START_L                              (SPM_BASE_ADDRESS + 0x0050C)
    #define SYS_TIMER_START_L_SYS_TIMER_START_L                Fld(32, 0) //[31:0]

#define SPM_REG_SYS_TIMER_START_H                              (SPM_BASE_ADDRESS + 0x00510)
    #define SYS_TIMER_START_H_SYS_TIMER_START_H                Fld(32, 0) //[31:0]

#define SPM_REG_SYS_TIMER_LATCH_L_00                           (SPM_BASE_ADDRESS + 0x00514)
    #define SYS_TIMER_LATCH_L_00_SYS_TIMER_LATCH_L_00          Fld(32, 0) //[31:0]

#define SPM_REG_SYS_TIMER_LATCH_H_00                           (SPM_BASE_ADDRESS + 0x00518)
    #define SYS_TIMER_LATCH_H_00_SYS_TIMER_LATCH_H_00          Fld(32, 0) //[31:0]

#define SPM_REG_SYS_TIMER_LATCH_L_01                           (SPM_BASE_ADDRESS + 0x0051C)
    #define SYS_TIMER_LATCH_L_01_SYS_TIMER_LATCH_L_01          Fld(32, 0) //[31:0]

#define SPM_REG_SYS_TIMER_LATCH_H_01                           (SPM_BASE_ADDRESS + 0x00520)
    #define SYS_TIMER_LATCH_H_01_SYS_TIMER_LATCH_H_01          Fld(32, 0) //[31:0]

#define SPM_REG_SYS_TIMER_LATCH_L_02                           (SPM_BASE_ADDRESS + 0x00524)
    #define SYS_TIMER_LATCH_L_02_SYS_TIMER_LATCH_L_02          Fld(32, 0) //[31:0]

#define SPM_REG_SYS_TIMER_LATCH_H_02                           (SPM_BASE_ADDRESS + 0x00528)
    #define SYS_TIMER_LATCH_H_02_SYS_TIMER_LATCH_H_02          Fld(32, 0) //[31:0]

#define SPM_REG_SYS_TIMER_LATCH_L_03                           (SPM_BASE_ADDRESS + 0x0052C)
    #define SYS_TIMER_LATCH_L_03_SYS_TIMER_LATCH_L_03          Fld(32, 0) //[31:0]

#define SPM_REG_SYS_TIMER_LATCH_H_03                           (SPM_BASE_ADDRESS + 0x00530)
    #define SYS_TIMER_LATCH_H_03_SYS_TIMER_LATCH_H_03          Fld(32, 0) //[31:0]

#define SPM_REG_SYS_TIMER_LATCH_L_04                           (SPM_BASE_ADDRESS + 0x00534)
    #define SYS_TIMER_LATCH_L_04_SYS_TIMER_LATCH_L_04          Fld(32, 0) //[31:0]

#define SPM_REG_SYS_TIMER_LATCH_H_04                           (SPM_BASE_ADDRESS + 0x00538)
    #define SYS_TIMER_LATCH_H_04_SYS_TIMER_LATCH_H_04          Fld(32, 0) //[31:0]

#define SPM_REG_SYS_TIMER_LATCH_L_05                           (SPM_BASE_ADDRESS + 0x0053C)
    #define SYS_TIMER_LATCH_L_05_SYS_TIMER_LATCH_L_05          Fld(32, 0) //[31:0]

#define SPM_REG_SYS_TIMER_LATCH_H_05                           (SPM_BASE_ADDRESS + 0x00540)
    #define SYS_TIMER_LATCH_H_05_SYS_TIMER_LATCH_H_05          Fld(32, 0) //[31:0]

#define SPM_REG_SYS_TIMER_LATCH_L_06                           (SPM_BASE_ADDRESS + 0x00544)
    #define SYS_TIMER_LATCH_L_06_SYS_TIMER_LATCH_L_06          Fld(32, 0) //[31:0]

#define SPM_REG_SYS_TIMER_LATCH_H_06                           (SPM_BASE_ADDRESS + 0x00548)
    #define SYS_TIMER_LATCH_H_06_SYS_TIMER_LATCH_H_06          Fld(32, 0) //[31:0]

#define SPM_REG_SYS_TIMER_LATCH_L_07                           (SPM_BASE_ADDRESS + 0x0054C)
    #define SYS_TIMER_LATCH_L_07_SYS_TIMER_LATCH_L_07          Fld(32, 0) //[31:0]

#define SPM_REG_SYS_TIMER_LATCH_H_07                           (SPM_BASE_ADDRESS + 0x00550)
    #define SYS_TIMER_LATCH_H_07_SYS_TIMER_LATCH_H_07          Fld(32, 0) //[31:0]

#define SPM_REG_SYS_TIMER_LATCH_L_08                           (SPM_BASE_ADDRESS + 0x00554)
    #define SYS_TIMER_LATCH_L_08_SYS_TIMER_LATCH_L_08          Fld(32, 0) //[31:0]

#define SPM_REG_SYS_TIMER_LATCH_H_08                           (SPM_BASE_ADDRESS + 0x00558)
    #define SYS_TIMER_LATCH_H_08_SYS_TIMER_LATCH_H_08          Fld(32, 0) //[31:0]

#define SPM_REG_SYS_TIMER_LATCH_L_09                           (SPM_BASE_ADDRESS + 0x0055C)
    #define SYS_TIMER_LATCH_L_09_SYS_TIMER_LATCH_L_09          Fld(32, 0) //[31:0]

#define SPM_REG_SYS_TIMER_LATCH_H_09                           (SPM_BASE_ADDRESS + 0x00560)
    #define SYS_TIMER_LATCH_H_09_SYS_TIMER_LATCH_H_09          Fld(32, 0) //[31:0]

#define SPM_REG_SYS_TIMER_LATCH_L_10                           (SPM_BASE_ADDRESS + 0x00564)
    #define SYS_TIMER_LATCH_L_10_SYS_TIMER_LATCH_L_10          Fld(32, 0) //[31:0]

#define SPM_REG_SYS_TIMER_LATCH_H_10                           (SPM_BASE_ADDRESS + 0x00568)
    #define SYS_TIMER_LATCH_H_10_SYS_TIMER_LATCH_H_10          Fld(32, 0) //[31:0]

#define SPM_REG_SYS_TIMER_LATCH_L_11                           (SPM_BASE_ADDRESS + 0x0056C)
    #define SYS_TIMER_LATCH_L_11_SYS_TIMER_LATCH_L_11          Fld(32, 0) //[31:0]

#define SPM_REG_SYS_TIMER_LATCH_H_11                           (SPM_BASE_ADDRESS + 0x00570)
    #define SYS_TIMER_LATCH_H_11_SYS_TIMER_LATCH_H_11          Fld(32, 0) //[31:0]

#define SPM_REG_SYS_TIMER_LATCH_L_12                           (SPM_BASE_ADDRESS + 0x00574)
    #define SYS_TIMER_LATCH_L_12_SYS_TIMER_LATCH_L_12          Fld(32, 0) //[31:0]

#define SPM_REG_SYS_TIMER_LATCH_H_12                           (SPM_BASE_ADDRESS + 0x00578)
    #define SYS_TIMER_LATCH_H_12_SYS_TIMER_LATCH_H_12          Fld(32, 0) //[31:0]

#define SPM_REG_SYS_TIMER_LATCH_L_13                           (SPM_BASE_ADDRESS + 0x0057C)
    #define SYS_TIMER_LATCH_L_13_SYS_TIMER_LATCH_L_13          Fld(32, 0) //[31:0]

#define SPM_REG_SYS_TIMER_LATCH_H_13                           (SPM_BASE_ADDRESS + 0x00580)
    #define SYS_TIMER_LATCH_H_13_SYS_TIMER_LATCH_H_13          Fld(32, 0) //[31:0]

#define SPM_REG_SYS_TIMER_LATCH_L_14                           (SPM_BASE_ADDRESS + 0x00584)
    #define SYS_TIMER_LATCH_L_14_SYS_TIMER_LATCH_L_14          Fld(32, 0) //[31:0]

#define SPM_REG_SYS_TIMER_LATCH_H_14                           (SPM_BASE_ADDRESS + 0x00588)
    #define SYS_TIMER_LATCH_H_14_SYS_TIMER_LATCH_H_14          Fld(32, 0) //[31:0]

#define SPM_REG_SYS_TIMER_LATCH_L_15                           (SPM_BASE_ADDRESS + 0x0058C)
    #define SYS_TIMER_LATCH_L_15_SYS_TIMER_LATCH_L_15          Fld(32, 0) //[31:0]

#define SPM_REG_SYS_TIMER_LATCH_H_15                           (SPM_BASE_ADDRESS + 0x00590)
    #define SYS_TIMER_LATCH_H_15_SYS_TIMER_LATCH_H_15          Fld(32, 0) //[31:0]

#define SPM_REG_PCM_TIMER_VAL                                  (SPM_BASE_ADDRESS + 0x00594)
    #define PCM_TIMER_VAL_REG_PCM_TIMER_VAL                    Fld(32, 0) //[31:0]

#define SPM_REG_PCM_TIMER_OUT                                  (SPM_BASE_ADDRESS + 0x00598)
    #define PCM_TIMER_OUT_PCM_TIMER                            Fld(32, 0) //[31:0]

#define SPM_REG_SPM_COUNTER_0                                  (SPM_BASE_ADDRESS + 0x0059C)
    #define SPM_COUNTER_0_SPM_COUNTER_WAKEUP_EN_0              Fld(1, 31) //[31:31]
    #define SPM_COUNTER_0_SPM_COUNTER_TIMEOUT_0                Fld(1, 30) //[30:30]
    #define SPM_COUNTER_0_SPM_COUNTER_CLR_0                    Fld(1, 29) //[29:29]
    #define SPM_COUNTER_0_SPM_COUNTER_EN_0                     Fld(1, 28) //[28:28]
    #define SPM_COUNTER_0_SPM_COUNTER_OUT_0                    Fld(14, 14) //[27:14]
    #define SPM_COUNTER_0_SPM_COUNTER_VAL_0                    Fld(14, 0) //[13:0]

#define SPM_REG_SPM_COUNTER_1                                  (SPM_BASE_ADDRESS + 0x005A0)
    #define SPM_COUNTER_1_SPM_COUNTER_WAKEUP_EN_1              Fld(1, 31) //[31:31]
    #define SPM_COUNTER_1_SPM_COUNTER_TIMEOUT_1                Fld(1, 30) //[30:30]
    #define SPM_COUNTER_1_SPM_COUNTER_CLR_1                    Fld(1, 29) //[29:29]
    #define SPM_COUNTER_1_SPM_COUNTER_EN_1                     Fld(1, 28) //[28:28]
    #define SPM_COUNTER_1_SPM_COUNTER_OUT_1                    Fld(14, 14) //[27:14]
    #define SPM_COUNTER_1_SPM_COUNTER_VAL_1                    Fld(14, 0) //[13:0]

#define SPM_REG_SPM_COUNTER_2                                  (SPM_BASE_ADDRESS + 0x005A4)
    #define SPM_COUNTER_2_SPM_COUNTER_WAKEUP_EN_2              Fld(1, 31) //[31:31]
    #define SPM_COUNTER_2_SPM_COUNTER_TIMEOUT_2                Fld(1, 30) //[30:30]
    #define SPM_COUNTER_2_SPM_COUNTER_CLR_2                    Fld(1, 29) //[29:29]
    #define SPM_COUNTER_2_SPM_COUNTER_EN_2                     Fld(1, 28) //[28:28]
    #define SPM_COUNTER_2_SPM_COUNTER_OUT_2                    Fld(14, 14) //[27:14]
    #define SPM_COUNTER_2_SPM_COUNTER_VAL_2                    Fld(14, 0) //[13:0]

#define SPM_REG_PCM_WDT_VAL                                    (SPM_BASE_ADDRESS + 0x005A8)
    #define PCM_WDT_VAL_REG_PCM_WDT_VAL                        Fld(32, 0) //[31:0]

#define SPM_REG_PCM_WDT_OUT                                    (SPM_BASE_ADDRESS + 0x005AC)
    #define PCM_WDT_OUT_PCM_WDT_TIMER_VAL_OUT                  Fld(32, 0) //[31:0]

#define SPM_REG_SPM_WAKEUP_STA                                 (SPM_BASE_ADDRESS + 0x00800)
    #define SPM_WAKEUP_STA_SPM_WAKEUP_EVENT_L                  Fld(32, 0) //[31:0]

#define SPM_REG_SPM_WAKEUP_EXT_STA                             (SPM_BASE_ADDRESS + 0x00804)
    #define SPM_WAKEUP_EXT_STA_EXT_WAKEUP_EVENT                Fld(32, 0) //[31:0]

#define SPM_REG_SPM_WAKEUP_EVENT_MASK                          (SPM_BASE_ADDRESS + 0x00808)
    #define SPM_WAKEUP_EVENT_MASK_REG_WAKEUP_EVENT_MASK        Fld(32, 0) //[31:0]

#define SPM_REG_SPM_WAKEUP_EVENT_EXT_MASK                      (SPM_BASE_ADDRESS + 0x0080C)
    #define SPM_WAKEUP_EVENT_EXT_MASK_REG_EXT_WAKEUP_EVENT_MASK Fld(32, 0) //[31:0]

#define SPM_REG_SPM_WAKEUP_EVENT_SENS                          (SPM_BASE_ADDRESS + 0x00810)
    #define SPM_WAKEUP_EVENT_SENS_REG_WAKEUP_EVENT_SENS        Fld(32, 0) //[31:0]

#define SPM_REG_SPM_WAKEUP_EVENT_CLEAR                         (SPM_BASE_ADDRESS + 0x00814)
    #define SPM_WAKEUP_EVENT_CLEAR_REG_WAKEUP_EVENT_CLR        Fld(32, 0) //[31:0]

#define SPM_REG_SPM_SRC_REQ                                    (SPM_BASE_ADDRESS + 0x00818)
    #define SPM_SRC_REQ_VRF18_STATE                            Fld(1, 26) //[26:26]
    #define SPM_SRC_REQ_VCORE_STATE                            Fld(1, 25) //[25:25]
    #define SPM_SRC_REQ_SW_MAILBOX_STATE                       Fld(1, 24) //[24:24]
    #define SPM_SRC_REQ_SSPM_MAILBOX_STATE                     Fld(1, 23) //[23:23]
    #define SPM_SRC_REQ_SCP_MAILBOX_STATE                      Fld(1, 22) //[22:22]
    #define SPM_SRC_REQ_INFRA_STATE                            Fld(1, 21) //[21:21]
    #define SPM_SRC_REQ_F26M_STATE                             Fld(1, 20) //[20:20]
    #define SPM_SRC_REQ_DVFS_STATE                             Fld(1, 19) //[19:19]
    #define SPM_SRC_REQ_DDREN_STATE                            Fld(1, 18) //[18:18]
    #define SPM_SRC_REQ_APSRC_STATE                            Fld(1, 17) //[17:17]
    #define SPM_SRC_REQ_ADSP_MAILBOX_STATE                     Fld(1, 16) //[16:16]
    #define SPM_SRC_REQ_REG_SPM_VRF18_REQ                      Fld(1, 10) //[10:10]
    #define SPM_SRC_REQ_REG_SPM_VCORE_REQ                      Fld(1, 9) //[9:9]
    #define SPM_SRC_REQ_REG_SPM_SW_MAILBOX_REQ                 Fld(1, 8) //[8:8]
    #define SPM_SRC_REQ_REG_SPM_SSPM_MAILBOX_REQ               Fld(1, 7) //[7:7]
    #define SPM_SRC_REQ_REG_SPM_SCP_MAILBOX_REQ                Fld(1, 6) //[6:6]
    #define SPM_SRC_REQ_REG_SPM_INFRA_REQ                      Fld(1, 5) //[5:5]
    #define SPM_SRC_REQ_REG_SPM_F26M_REQ                       Fld(1, 4) //[4:4]
    #define SPM_SRC_REQ_REG_SPM_DVFS_REQ                       Fld(1, 3) //[3:3]
    #define SPM_SRC_REQ_REG_SPM_DDREN_REQ                      Fld(1, 2) //[2:2]
    #define SPM_SRC_REQ_REG_SPM_APSRC_REQ                      Fld(1, 1) //[1:1]
    #define SPM_SRC_REQ_REG_SPM_ADSP_MAILBOX_REQ               Fld(1, 0) //[0:0]

#define SPM_REG_SPM_SRC_MASK_0                                 (SPM_BASE_ADDRESS + 0x0081C)
    #define SPM_SRC_MASK_0_REG_CCIF_APSRC_REQ_MASK_B           Fld(12, 17) //[28:17]
    #define SPM_SRC_MASK_0_REG_CAM_DDREN_REQ_MASK_B            Fld(1, 16) //[16:16]
    #define SPM_SRC_MASK_0_REG_CAM_APSRC_REQ_MASK_B            Fld(1, 15) //[15:15]
    #define SPM_SRC_MASK_0_REG_AUDIO_DSP_VRF18_REQ_MASK_B      Fld(1, 14) //[14:14]
    #define SPM_SRC_MASK_0_REG_AUDIO_DSP_SRCCLKENA_MASK_B      Fld(1, 13) //[13:13]
    #define SPM_SRC_MASK_0_REG_AUDIO_DSP_INFRA_REQ_MASK_B      Fld(1, 12) //[12:12]
    #define SPM_SRC_MASK_0_REG_AUDIO_DSP_DDREN_REQ_MASK_B      Fld(1, 11) //[11:11]
    #define SPM_SRC_MASK_0_REG_AUDIO_DSP_APSRC_REQ_MASK_B      Fld(1, 10) //[10:10]
    #define SPM_SRC_MASK_0_REG_APU_VRF18_REQ_MASK_B            Fld(1, 9) //[9:9]
    #define SPM_SRC_MASK_0_REG_APU_SRCCLKENA_MASK_B            Fld(1, 8) //[8:8]
    #define SPM_SRC_MASK_0_REG_APU_INFRA_REQ_MASK_B            Fld(1, 7) //[7:7]
    #define SPM_SRC_MASK_0_REG_APU_DDREN_REQ_MASK_B            Fld(1, 6) //[6:6]
    #define SPM_SRC_MASK_0_REG_APU_APSRC_REQ_MASK_B            Fld(1, 5) //[5:5]
    #define SPM_SRC_MASK_0_REG_AFE_VRF18_REQ_MASK_B            Fld(1, 4) //[4:4]
    #define SPM_SRC_MASK_0_REG_AFE_SRCCLKENA_MASK_B            Fld(1, 3) //[3:3]
    #define SPM_SRC_MASK_0_REG_AFE_INFRA_REQ_MASK_B            Fld(1, 2) //[2:2]
    #define SPM_SRC_MASK_0_REG_AFE_DDREN_REQ_MASK_B            Fld(1, 1) //[1:1]
    #define SPM_SRC_MASK_0_REG_AFE_APSRC_REQ_MASK_B            Fld(1, 0) //[0:0]

#define SPM_REG_SPM_SRC_MASK_1                                 (SPM_BASE_ADDRESS + 0x00820)
    #define SPM_SRC_MASK_1_REG_CONN_INFRA_REQ_MASK_B           Fld(1, 31) //[31:31]
    #define SPM_SRC_MASK_1_REG_CONN_DDREN_REQ_MASK_B           Fld(1, 30) //[30:30]
    #define SPM_SRC_MASK_1_REG_CONN_APSRC_REQ_MASK_B           Fld(1, 29) //[29:29]
    #define SPM_SRC_MASK_1_REG_CG_CHECK_VRF18_REQ_MASK_B       Fld(1, 28) //[28:28]
    #define SPM_SRC_MASK_1_REG_CG_CHECK_VCORE_REQ_MASK_B       Fld(1, 27) //[27:27]
    #define SPM_SRC_MASK_1_REG_CG_CHECK_SRCCLKENA_MASK_B       Fld(1, 26) //[26:26]
    #define SPM_SRC_MASK_1_REG_CG_CHECK_DDREN_REQ_MASK_B       Fld(1, 25) //[25:25]
    #define SPM_SRC_MASK_1_REG_CG_CHECK_APSRC_REQ_MASK_B       Fld(1, 24) //[24:24]
    #define SPM_SRC_MASK_1_REG_CCIF_SRCCLKENA_MASK_B           Fld(12, 12) //[23:12]
    #define SPM_SRC_MASK_1_REG_CCIF_INFRA_REQ_MASK_B           Fld(12, 0) //[11:0]

#define SPM_REG_SPM_SRC_MASK_2                                 (SPM_BASE_ADDRESS + 0x00824)
    #define SPM_SRC_MASK_2_REG_GCE_APSRC_REQ_MASK_B            Fld(1, 31) //[31:31]
    #define SPM_SRC_MASK_2_REG_DVFSRC_LEVEL_REQ_MASK_B         Fld(1, 30) //[30:30]
    #define SPM_SRC_MASK_2_REG_DPMAIF_VRF18_REQ_MASK_B         Fld(1, 29) //[29:29]
    #define SPM_SRC_MASK_2_REG_DPMAIF_SRCCLKENA_MASK_B         Fld(1, 28) //[28:28]
    #define SPM_SRC_MASK_2_REG_DPMAIF_INFRA_REQ_MASK_B         Fld(1, 27) //[27:27]
    #define SPM_SRC_MASK_2_REG_DPMAIF_DDREN_REQ_MASK_B         Fld(1, 26) //[26:26]
    #define SPM_SRC_MASK_2_REG_DPMAIF_APSRC_REQ_MASK_B         Fld(1, 25) //[25:25]
    #define SPM_SRC_MASK_2_REG_DPM_VRF18_REQ_MASK_B            Fld(4, 21) //[24:21]
    #define SPM_SRC_MASK_2_REG_DPM_INFRA_REQ_MASK_B            Fld(4, 17) //[20:17]
    #define SPM_SRC_MASK_2_REG_DPM_APSRC_REQ_MASK_B            Fld(4, 13) //[16:13]
    #define SPM_SRC_MASK_2_REG_DISP1_DDREN_REQ_MASK_B          Fld(1, 12) //[12:12]
    #define SPM_SRC_MASK_2_REG_DISP1_APSRC_REQ_MASK_B          Fld(1, 11) //[11:11]
    #define SPM_SRC_MASK_2_REG_DISP0_DDREN_REQ_MASK_B          Fld(1, 10) //[10:10]
    #define SPM_SRC_MASK_2_REG_DISP0_APSRC_REQ_MASK_B          Fld(1, 9) //[9:9]
    #define SPM_SRC_MASK_2_REG_MCUPM_VRF18_REQ_MASK_B          Fld(1, 8) //[8:8]
    #define SPM_SRC_MASK_2_REG_MCUPM_SRCCLKENA_MASK_B          Fld(1, 7) //[7:7]
    #define SPM_SRC_MASK_2_REG_MCUPM_INFRA_REQ_MASK_B          Fld(1, 6) //[6:6]
    #define SPM_SRC_MASK_2_REG_MCUPM_DDREN_REQ_MASK_B          Fld(1, 5) //[5:5]
    #define SPM_SRC_MASK_2_REG_MCUPM_APSRC_REQ_MASK_B          Fld(1, 4) //[4:4]
    #define SPM_SRC_MASK_2_REG_CONN_VRF18_REQ_MASK_B           Fld(1, 3) //[3:3]
    #define SPM_SRC_MASK_2_REG_CONN_VCORE_REQ_MASK_B           Fld(1, 2) //[2:2]
    #define SPM_SRC_MASK_2_REG_CONN_SRCCLKENB_MASK_B           Fld(1, 1) //[1:1]
    #define SPM_SRC_MASK_2_REG_CONN_SRCCLKENA_MASK_B           Fld(1, 0) //[0:0]

#define SPM_REG_SPM_SRC_MASK_3                                 (SPM_BASE_ADDRESS + 0x00828)
    #define SPM_SRC_MASK_3_REG_MD_DDREN_REQ_MASK_B             Fld(1, 31) //[31:31]
    #define SPM_SRC_MASK_3_REG_MD_APSRC_REQ_MASK_B             Fld(1, 30) //[30:30]
    #define SPM_SRC_MASK_3_REG_MCUSYS_DDREN_REQ_MASK_B         Fld(8, 22) //[29:22]
    #define SPM_SRC_MASK_3_REG_MCUSYS_APSRC_REQ_MASK_B         Fld(8, 14) //[21:14]
    #define SPM_SRC_MASK_3_REG_IPIC_VRF18_REQ_MASK_B           Fld(1, 13) //[13:13]
    #define SPM_SRC_MASK_3_REG_IPIC_INFRA_REQ_MASK_B           Fld(1, 12) //[12:12]
    #define SPM_SRC_MASK_3_REG_INFRASYS_DDREN_REQ_MASK_B       Fld(1, 11) //[11:11]
    #define SPM_SRC_MASK_3_REG_INFRASYS_APSRC_REQ_MASK_B       Fld(1, 10) //[10:10]
    #define SPM_SRC_MASK_3_REG_IMG_DDREN_REQ_MASK_B            Fld(1, 9) //[9:9]
    #define SPM_SRC_MASK_3_REG_IMG_APSRC_REQ_MASK_B            Fld(1, 8) //[8:8]
    #define SPM_SRC_MASK_3_REG_GPUEB_VRF18_REQ_MASK_B          Fld(1, 7) //[7:7]
    #define SPM_SRC_MASK_3_REG_GPUEB_SRCCLKENA_MASK_B          Fld(1, 6) //[6:6]
    #define SPM_SRC_MASK_3_REG_GPUEB_INFRA_REQ_MASK_B          Fld(1, 5) //[5:5]
    #define SPM_SRC_MASK_3_REG_GPUEB_DDREN_REQ_MASK_B          Fld(1, 4) //[4:4]
    #define SPM_SRC_MASK_3_REG_GPUEB_APSRC_REQ_MASK_B          Fld(1, 3) //[3:3]
    #define SPM_SRC_MASK_3_REG_GCE_VRF18_REQ_MASK_B            Fld(1, 2) //[2:2]
    #define SPM_SRC_MASK_3_REG_GCE_INFRA_REQ_MASK_B            Fld(1, 1) //[1:1]
    #define SPM_SRC_MASK_3_REG_GCE_DDREN_REQ_MASK_B            Fld(1, 0) //[0:0]

#define SPM_REG_SPM_SRC_MASK_4                                 (SPM_BASE_ADDRESS + 0x0082C)
    #define SPM_SRC_MASK_4_REG_PCIE1_DDREN_REQ_MASK_B          Fld(1, 31) //[31:31]
    #define SPM_SRC_MASK_4_REG_PCIE1_APSRC_REQ_MASK_B          Fld(1, 30) //[30:30]
    #define SPM_SRC_MASK_4_REG_PCIE0_VRF18_REQ_MASK_B          Fld(1, 29) //[29:29]
    #define SPM_SRC_MASK_4_REG_PCIE0_SRCCLKENA_MASK_B          Fld(1, 28) //[28:28]
    #define SPM_SRC_MASK_4_REG_PCIE0_INFRA_REQ_MASK_B          Fld(1, 27) //[27:27]
    #define SPM_SRC_MASK_4_REG_PCIE0_DDREN_REQ_MASK_B          Fld(1, 26) //[26:26]
    #define SPM_SRC_MASK_4_REG_PCIE0_APSRC_REQ_MASK_B          Fld(1, 25) //[25:25]
    #define SPM_SRC_MASK_4_REG_MSDC2_VRF18_REQ_MASK_B          Fld(1, 24) //[24:24]
    #define SPM_SRC_MASK_4_REG_MSDC2_SRCCLKENA_MASK_B          Fld(1, 23) //[23:23]
    #define SPM_SRC_MASK_4_REG_MSDC2_INFRA_REQ_MASK_B          Fld(1, 22) //[22:22]
    #define SPM_SRC_MASK_4_REG_MSDC2_DDREN_REQ_MASK_B          Fld(1, 21) //[21:21]
    #define SPM_SRC_MASK_4_REG_MSDC2_APSRC_REQ_MASK_B          Fld(1, 20) //[20:20]
    #define SPM_SRC_MASK_4_REG_MSDC1_VRF18_REQ_MASK_B          Fld(1, 19) //[19:19]
    #define SPM_SRC_MASK_4_REG_MSDC1_SRCCLKENA_MASK_B          Fld(1, 18) //[18:18]
    #define SPM_SRC_MASK_4_REG_MSDC1_INFRA_REQ_MASK_B          Fld(1, 17) //[17:17]
    #define SPM_SRC_MASK_4_REG_MSDC1_DDREN_REQ_MASK_B          Fld(1, 16) //[16:16]
    #define SPM_SRC_MASK_4_REG_MSDC1_APSRC_REQ_MASK_B          Fld(1, 15) //[15:15]
    #define SPM_SRC_MASK_4_REG_MMSYS_VRF18_REQ_MASK_B          Fld(1, 14) //[14:14]
    #define SPM_SRC_MASK_4_REG_MMSYS_DDREN_REQ_MASK_B          Fld(1, 13) //[13:13]
    #define SPM_SRC_MASK_4_REG_MMSYS_APSRC_REQ_MASK_B          Fld(1, 12) //[12:12]
    #define SPM_SRC_MASK_4_REG_MM_PROC_VRF18_REQ_MASK_B        Fld(1, 11) //[11:11]
    #define SPM_SRC_MASK_4_REG_MM_PROC_SRCCLKENA_MASK_B        Fld(1, 10) //[10:10]
    #define SPM_SRC_MASK_4_REG_MM_PROC_INFRA_REQ_MASK_B        Fld(1, 9) //[9:9]
    #define SPM_SRC_MASK_4_REG_MM_PROC_DDREN_REQ_MASK_B        Fld(1, 8) //[8:8]
    #define SPM_SRC_MASK_4_REG_MM_PROC_APSRC_REQ_MASK_B        Fld(1, 7) //[7:7]
    #define SPM_SRC_MASK_4_REG_MDP_DDREN_REQ_MASK_B            Fld(1, 6) //[6:6]
    #define SPM_SRC_MASK_4_REG_MDP_APSRC_REQ_MASK_B            Fld(1, 5) //[5:5]
    #define SPM_SRC_MASK_4_REG_MD_VRF18_REQ_MASK_B             Fld(1, 4) //[4:4]
    #define SPM_SRC_MASK_4_REG_MD_VCORE_REQ_MASK_B             Fld(1, 3) //[3:3]
    #define SPM_SRC_MASK_4_REG_MD_SRCCLKENA1_MASK_B            Fld(1, 2) //[2:2]
    #define SPM_SRC_MASK_4_REG_MD_SRCCLKENA_MASK_B             Fld(1, 1) //[1:1]
    #define SPM_SRC_MASK_4_REG_MD_INFRA_REQ_MASK_B             Fld(1, 0) //[0:0]

#define SPM_REG_SPM_SRC_MASK_5                                 (SPM_BASE_ADDRESS + 0x00830)
    #define SPM_SRC_MASK_5_REG_UFS_VRF18_REQ_MASK_B            Fld(1, 31) //[31:31]
    #define SPM_SRC_MASK_5_REG_UFS_SRCCLKENA_MASK_B            Fld(1, 30) //[30:30]
    #define SPM_SRC_MASK_5_REG_UFS_INFRA_REQ_MASK_B            Fld(1, 29) //[29:29]
    #define SPM_SRC_MASK_5_REG_UFS_DDREN_REQ_MASK_B            Fld(1, 28) //[28:28]
    #define SPM_SRC_MASK_5_REG_UFS_APSRC_REQ_MASK_B            Fld(1, 27) //[27:27]
    #define SPM_SRC_MASK_5_REG_SSUSB1_VRF18_REQ_MASK_B         Fld(1, 26) //[26:26]
    #define SPM_SRC_MASK_5_REG_SSUSB1_SRCCLKENA_MASK_B         Fld(1, 25) //[25:25]
    #define SPM_SRC_MASK_5_REG_SSUSB1_INFRA_REQ_MASK_B         Fld(1, 24) //[24:24]
    #define SPM_SRC_MASK_5_REG_SSUSB1_DDREN_REQ_MASK_B         Fld(1, 23) //[23:23]
    #define SPM_SRC_MASK_5_REG_SSUSB1_APSRC_REQ_MASK_B         Fld(1, 22) //[22:22]
    #define SPM_SRC_MASK_5_REG_SSUSB0_VRF18_REQ_MASK_B         Fld(1, 21) //[21:21]
    #define SPM_SRC_MASK_5_REG_SSUSB0_SRCCLKENA_MASK_B         Fld(1, 20) //[20:20]
    #define SPM_SRC_MASK_5_REG_SSUSB0_INFRA_REQ_MASK_B         Fld(1, 19) //[19:19]
    #define SPM_SRC_MASK_5_REG_SSUSB0_DDREN_REQ_MASK_B         Fld(1, 18) //[18:18]
    #define SPM_SRC_MASK_5_REG_SSUSB0_APSRC_REQ_MASK_B         Fld(1, 17) //[17:17]
    #define SPM_SRC_MASK_5_REG_SSPM_VRF18_REQ_MASK_B           Fld(1, 16) //[16:16]
    #define SPM_SRC_MASK_5_REG_SSPM_SRCCLKENA_MASK_B           Fld(1, 15) //[15:15]
    #define SPM_SRC_MASK_5_REG_SSPM_INFRA_REQ_MASK_B           Fld(1, 14) //[14:14]
    #define SPM_SRC_MASK_5_REG_SSPM_DDREN_REQ_MASK_B           Fld(1, 13) //[13:13]
    #define SPM_SRC_MASK_5_REG_SSPM_APSRC_REQ_MASK_B           Fld(1, 12) //[12:12]
    #define SPM_SRC_MASK_5_REG_SRCCLKENI_SRCCLKENA_MASK_B      Fld(2, 10) //[11:10]
    #define SPM_SRC_MASK_5_REG_SRCCLKENI_INFRA_REQ_MASK_B      Fld(2, 8) //[9:8]
    #define SPM_SRC_MASK_5_REG_SCP_VRF18_REQ_MASK_B            Fld(1, 7) //[7:7]
    #define SPM_SRC_MASK_5_REG_SCP_SRCCLKENA_MASK_B            Fld(1, 6) //[6:6]
    #define SPM_SRC_MASK_5_REG_SCP_INFRA_REQ_MASK_B            Fld(1, 5) //[5:5]
    #define SPM_SRC_MASK_5_REG_SCP_DDREN_REQ_MASK_B            Fld(1, 4) //[4:4]
    #define SPM_SRC_MASK_5_REG_SCP_APSRC_REQ_MASK_B            Fld(1, 3) //[3:3]
    #define SPM_SRC_MASK_5_REG_PCIE1_VRF18_REQ_MASK_B          Fld(1, 2) //[2:2]
    #define SPM_SRC_MASK_5_REG_PCIE1_SRCCLKENA_MASK_B          Fld(1, 1) //[1:1]
    #define SPM_SRC_MASK_5_REG_PCIE1_INFRA_REQ_MASK_B          Fld(1, 0) //[0:0]

#define SPM_REG_SPM_SRC_MASK_6                                 (SPM_BASE_ADDRESS + 0x00834)
    #define SPM_SRC_MASK_6_REG_VENC_DDREN_REQ_MASK_B           Fld(1, 3) //[3:3]
    #define SPM_SRC_MASK_6_REG_VENC_APSRC_REQ_MASK_B           Fld(1, 2) //[2:2]
    #define SPM_SRC_MASK_6_REG_VDEC_DDREN_REQ_MASK_B           Fld(1, 1) //[1:1]
    #define SPM_SRC_MASK_6_REG_VDEC_APSRC_REQ_MASK_B           Fld(1, 0) //[0:0]

#define SPM_REG_SPM_REQ_STA_0                                  (SPM_BASE_ADDRESS + 0x00838)
    #define SPM_REQ_STA_0_CCIF_APSRC_REQ                       Fld(12, 17) //[28:17]
    #define SPM_REQ_STA_0_CAM_DDREN_REQ                        Fld(1, 16) //[16:16]
    #define SPM_REQ_STA_0_CAM_APSRC_REQ                        Fld(1, 15) //[15:15]
    #define SPM_REQ_STA_0_AUDIO_DSP_VRF18_REQ                  Fld(1, 14) //[14:14]
    #define SPM_REQ_STA_0_AUDIO_DSP_SRCCLKENA                  Fld(1, 13) //[13:13]
    #define SPM_REQ_STA_0_AUDIO_DSP_INFRA_REQ                  Fld(1, 12) //[12:12]
    #define SPM_REQ_STA_0_AUDIO_DSP_DDREN_REQ                  Fld(1, 11) //[11:11]
    #define SPM_REQ_STA_0_AUDIO_DSP_APSRC_REQ                  Fld(1, 10) //[10:10]
    #define SPM_REQ_STA_0_APU_VRF18_REQ                        Fld(1, 9) //[9:9]
    #define SPM_REQ_STA_0_APU_SRCCLKENA                        Fld(1, 8) //[8:8]
    #define SPM_REQ_STA_0_APU_INFRA_REQ                        Fld(1, 7) //[7:7]
    #define SPM_REQ_STA_0_APU_DDREN_REQ                        Fld(1, 6) //[6:6]
    #define SPM_REQ_STA_0_APU_APSRC_REQ                        Fld(1, 5) //[5:5]
    #define SPM_REQ_STA_0_AFE_VRF18_REQ                        Fld(1, 4) //[4:4]
    #define SPM_REQ_STA_0_AFE_SRCCLKENA                        Fld(1, 3) //[3:3]
    #define SPM_REQ_STA_0_AFE_INFRA_REQ                        Fld(1, 2) //[2:2]
    #define SPM_REQ_STA_0_AFE_DDREN_REQ                        Fld(1, 1) //[1:1]
    #define SPM_REQ_STA_0_AFE_APSRC_REQ                        Fld(1, 0) //[0:0]

#define SPM_REG_SPM_REQ_STA_1                                  (SPM_BASE_ADDRESS + 0x0083C)
    #define SPM_REQ_STA_1_CONN_INFRA_REQ                       Fld(1, 31) //[31:31]
    #define SPM_REQ_STA_1_CONN_DDREN_REQ                       Fld(1, 30) //[30:30]
    #define SPM_REQ_STA_1_CONN_APSRC_REQ                       Fld(1, 29) //[29:29]
    #define SPM_REQ_STA_1_CG_CHECK_VRF18_REQ                   Fld(1, 28) //[28:28]
    #define SPM_REQ_STA_1_CG_CHECK_VCORE_REQ                   Fld(1, 27) //[27:27]
    #define SPM_REQ_STA_1_CG_CHECK_SRCCLKENA                   Fld(1, 26) //[26:26]
    #define SPM_REQ_STA_1_CG_CHECK_DDREN_REQ                   Fld(1, 25) //[25:25]
    #define SPM_REQ_STA_1_CG_CHECK_APSRC_REQ                   Fld(1, 24) //[24:24]
    #define SPM_REQ_STA_1_CCIF_SRCCLKENA                       Fld(12, 12) //[23:12]
    #define SPM_REQ_STA_1_CCIF_INFRA_REQ                       Fld(12, 0) //[11:0]

#define SPM_REG_SPM_REQ_STA_2                                  (SPM_BASE_ADDRESS + 0x00840)
    #define SPM_REQ_STA_2_GCE_APSRC_REQ                        Fld(1, 31) //[31:31]
    #define SPM_REQ_STA_2_DVFSRC_LEVEL_REQ                     Fld(1, 30) //[30:30]
    #define SPM_REQ_STA_2_DPMAIF_VRF18_REQ                     Fld(1, 29) //[29:29]
    #define SPM_REQ_STA_2_DPMAIF_SRCCLKENA                     Fld(1, 28) //[28:28]
    #define SPM_REQ_STA_2_DPMAIF_INFRA_REQ                     Fld(1, 27) //[27:27]
    #define SPM_REQ_STA_2_DPMAIF_DDREN_REQ                     Fld(1, 26) //[26:26]
    #define SPM_REQ_STA_2_DPMAIF_APSRC_REQ                     Fld(1, 25) //[25:25]
    #define SPM_REQ_STA_2_DPM_VRF18_REQ                        Fld(4, 21) //[24:21]
    #define SPM_REQ_STA_2_DPM_INFRA_REQ                        Fld(4, 17) //[20:17]
    #define SPM_REQ_STA_2_DPM_APSRC_REQ                        Fld(4, 13) //[16:13]
    #define SPM_REQ_STA_2_DISP1_DDREN_REQ                      Fld(1, 12) //[12:12]
    #define SPM_REQ_STA_2_DISP1_APSRC_REQ                      Fld(1, 11) //[11:11]
    #define SPM_REQ_STA_2_DISP0_DDREN_REQ                      Fld(1, 10) //[10:10]
    #define SPM_REQ_STA_2_DISP0_APSRC_REQ                      Fld(1, 9) //[9:9]
    #define SPM_REQ_STA_2_MCUPM_VRF18_REQ                      Fld(1, 8) //[8:8]
    #define SPM_REQ_STA_2_MCUPM_SRCCLKENA                      Fld(1, 7) //[7:7]
    #define SPM_REQ_STA_2_MCUPM_INFRA_REQ                      Fld(1, 6) //[6:6]
    #define SPM_REQ_STA_2_MCUPM_DDREN_REQ                      Fld(1, 5) //[5:5]
    #define SPM_REQ_STA_2_MCUPM_APSRC_REQ                      Fld(1, 4) //[4:4]
    #define SPM_REQ_STA_2_CONN_VRF18_REQ                       Fld(1, 3) //[3:3]
    #define SPM_REQ_STA_2_CONN_VCORE_REQ                       Fld(1, 2) //[2:2]
    #define SPM_REQ_STA_2_CONN_SRCCLKENB                       Fld(1, 1) //[1:1]
    #define SPM_REQ_STA_2_CONN_SRCCLKENA                       Fld(1, 0) //[0:0]

#define SPM_REG_SPM_REQ_STA_3                                  (SPM_BASE_ADDRESS + 0x00844)
    #define SPM_REQ_STA_3_MD_DDREN_REQ                         Fld(1, 31) //[31:31]
    #define SPM_REQ_STA_3_MD_APSRC_REQ                         Fld(1, 30) //[30:30]
    #define SPM_REQ_STA_3_MCUSYS_DDREN_REQ                     Fld(8, 22) //[29:22]
    #define SPM_REQ_STA_3_MCUSYS_APSRC_REQ                     Fld(8, 14) //[21:14]
    #define SPM_REQ_STA_3_IPIC_VRF18_REQ                       Fld(1, 13) //[13:13]
    #define SPM_REQ_STA_3_IPIC_INFRA_REQ                       Fld(1, 12) //[12:12]
    #define SPM_REQ_STA_3_INFRASYS_DDREN_REQ                   Fld(1, 11) //[11:11]
    #define SPM_REQ_STA_3_INFRASYS_APSRC_REQ                   Fld(1, 10) //[10:10]
    #define SPM_REQ_STA_3_IMG_DDREN_REQ                        Fld(1, 9) //[9:9]
    #define SPM_REQ_STA_3_IMG_APSRC_REQ                        Fld(1, 8) //[8:8]
    #define SPM_REQ_STA_3_GPUEB_VRF18_REQ                      Fld(1, 7) //[7:7]
    #define SPM_REQ_STA_3_GPUEB_SRCCLKENA                      Fld(1, 6) //[6:6]
    #define SPM_REQ_STA_3_GPUEB_INFRA_REQ                      Fld(1, 5) //[5:5]
    #define SPM_REQ_STA_3_GPUEB_DDREN_REQ                      Fld(1, 4) //[4:4]
    #define SPM_REQ_STA_3_GPUEB_APSRC_REQ                      Fld(1, 3) //[3:3]
    #define SPM_REQ_STA_3_GCE_VRF18_REQ                        Fld(1, 2) //[2:2]
    #define SPM_REQ_STA_3_GCE_INFRA_REQ                        Fld(1, 1) //[1:1]
    #define SPM_REQ_STA_3_GCE_DDREN_REQ                        Fld(1, 0) //[0:0]

#define SPM_REG_SPM_REQ_STA_4                                  (SPM_BASE_ADDRESS + 0x00848)
    #define SPM_REQ_STA_4_PCIE1_DDREN_REQ                      Fld(1, 31) //[31:31]
    #define SPM_REQ_STA_4_PCIE1_APSRC_REQ                      Fld(1, 30) //[30:30]
    #define SPM_REQ_STA_4_PCIE0_VRF18_REQ                      Fld(1, 29) //[29:29]
    #define SPM_REQ_STA_4_PCIE0_SRCCLKENA                      Fld(1, 28) //[28:28]
    #define SPM_REQ_STA_4_PCIE0_INFRA_REQ                      Fld(1, 27) //[27:27]
    #define SPM_REQ_STA_4_PCIE0_DDREN_REQ                      Fld(1, 26) //[26:26]
    #define SPM_REQ_STA_4_PCIE0_APSRC_REQ                      Fld(1, 25) //[25:25]
    #define SPM_REQ_STA_4_MSDC2_VRF18_REQ                      Fld(1, 24) //[24:24]
    #define SPM_REQ_STA_4_MSDC2_SRCCLKENA                      Fld(1, 23) //[23:23]
    #define SPM_REQ_STA_4_MSDC2_INFRA_REQ                      Fld(1, 22) //[22:22]
    #define SPM_REQ_STA_4_MSDC2_DDREN_REQ                      Fld(1, 21) //[21:21]
    #define SPM_REQ_STA_4_MSDC2_APSRC_REQ                      Fld(1, 20) //[20:20]
    #define SPM_REQ_STA_4_MSDC1_VRF18_REQ                      Fld(1, 19) //[19:19]
    #define SPM_REQ_STA_4_MSDC1_SRCCLKENA                      Fld(1, 18) //[18:18]
    #define SPM_REQ_STA_4_MSDC1_INFRA_REQ                      Fld(1, 17) //[17:17]
    #define SPM_REQ_STA_4_MSDC1_DDREN_REQ                      Fld(1, 16) //[16:16]
    #define SPM_REQ_STA_4_MSDC1_APSRC_REQ                      Fld(1, 15) //[15:15]
    #define SPM_REQ_STA_4_MMSYS_VRF18_REQ                      Fld(1, 14) //[14:14]
    #define SPM_REQ_STA_4_MMSYS_DDREN_REQ                      Fld(1, 13) //[13:13]
    #define SPM_REQ_STA_4_MMSYS_APSRC_REQ                      Fld(1, 12) //[12:12]
    #define SPM_REQ_STA_4_MM_PROC_VRF18_REQ                    Fld(1, 11) //[11:11]
    #define SPM_REQ_STA_4_MM_PROC_SRCCLKENA                    Fld(1, 10) //[10:10]
    #define SPM_REQ_STA_4_MM_PROC_INFRA_REQ                    Fld(1, 9) //[9:9]
    #define SPM_REQ_STA_4_MM_PROC_DDREN_REQ                    Fld(1, 8) //[8:8]
    #define SPM_REQ_STA_4_MM_PROC_APSRC_REQ                    Fld(1, 7) //[7:7]
    #define SPM_REQ_STA_4_MDP_DDREN_REQ                        Fld(1, 6) //[6:6]
    #define SPM_REQ_STA_4_MDP_APSRC_REQ                        Fld(1, 5) //[5:5]
    #define SPM_REQ_STA_4_MD_VRF18_REQ                         Fld(1, 4) //[4:4]
    #define SPM_REQ_STA_4_MD_VCORE_REQ                         Fld(1, 3) //[3:3]
    #define SPM_REQ_STA_4_MD_SRCCLKENA1                        Fld(1, 2) //[2:2]
    #define SPM_REQ_STA_4_MD_SRCCLKENA                         Fld(1, 1) //[1:1]
    #define SPM_REQ_STA_4_MD_INFRA_REQ                         Fld(1, 0) //[0:0]

#define SPM_REG_SPM_REQ_STA_5                                  (SPM_BASE_ADDRESS + 0x0084C)
    #define SPM_REQ_STA_5_UFS_VRF18_REQ                        Fld(1, 31) //[31:31]
    #define SPM_REQ_STA_5_UFS_SRCCLKENA                        Fld(1, 30) //[30:30]
    #define SPM_REQ_STA_5_UFS_INFRA_REQ                        Fld(1, 29) //[29:29]
    #define SPM_REQ_STA_5_UFS_DDREN_REQ                        Fld(1, 28) //[28:28]
    #define SPM_REQ_STA_5_UFS_APSRC_REQ                        Fld(1, 27) //[27:27]
    #define SPM_REQ_STA_5_SSUSB1_VRF18_REQ                     Fld(1, 26) //[26:26]
    #define SPM_REQ_STA_5_SSUSB1_SRCCLKENA                     Fld(1, 25) //[25:25]
    #define SPM_REQ_STA_5_SSUSB1_INFRA_REQ                     Fld(1, 24) //[24:24]
    #define SPM_REQ_STA_5_SSUSB1_DDREN_REQ                     Fld(1, 23) //[23:23]
    #define SPM_REQ_STA_5_SSUSB1_APSRC_REQ                     Fld(1, 22) //[22:22]
    #define SPM_REQ_STA_5_SSUSB0_VRF18_REQ                     Fld(1, 21) //[21:21]
    #define SPM_REQ_STA_5_SSUSB0_SRCCLKENA                     Fld(1, 20) //[20:20]
    #define SPM_REQ_STA_5_SSUSB0_INFRA_REQ                     Fld(1, 19) //[19:19]
    #define SPM_REQ_STA_5_SSUSB0_DDREN_REQ                     Fld(1, 18) //[18:18]
    #define SPM_REQ_STA_5_SSUSB0_APSRC_REQ                     Fld(1, 17) //[17:17]
    #define SPM_REQ_STA_5_SSPM_VRF18_REQ                       Fld(1, 16) //[16:16]
    #define SPM_REQ_STA_5_SSPM_SRCCLKENA                       Fld(1, 15) //[15:15]
    #define SPM_REQ_STA_5_SSPM_INFRA_REQ                       Fld(1, 14) //[14:14]
    #define SPM_REQ_STA_5_SSPM_DDREN_REQ                       Fld(1, 13) //[13:13]
    #define SPM_REQ_STA_5_SSPM_APSRC_REQ                       Fld(1, 12) //[12:12]
    #define SPM_REQ_STA_5_SRCCLKENI_SRCCLKENA                  Fld(2, 10) //[11:10]
    #define SPM_REQ_STA_5_SRCCLKENI_INFRA_REQ                  Fld(2, 8) //[9:8]
    #define SPM_REQ_STA_5_SCP_VRF18_REQ                        Fld(1, 7) //[7:7]
    #define SPM_REQ_STA_5_SCP_SRCCLKENA                        Fld(1, 6) //[6:6]
    #define SPM_REQ_STA_5_SCP_INFRA_REQ                        Fld(1, 5) //[5:5]
    #define SPM_REQ_STA_5_SCP_DDREN_REQ                        Fld(1, 4) //[4:4]
    #define SPM_REQ_STA_5_SCP_APSRC_REQ                        Fld(1, 3) //[3:3]
    #define SPM_REQ_STA_5_PCIE1_VRF18_REQ                      Fld(1, 2) //[2:2]
    #define SPM_REQ_STA_5_PCIE1_SRCCLKENA                      Fld(1, 1) //[1:1]
    #define SPM_REQ_STA_5_PCIE1_INFRA_REQ                      Fld(1, 0) //[0:0]

#define SPM_REG_SPM_REQ_STA_6                                  (SPM_BASE_ADDRESS + 0x00850)
    #define SPM_REQ_STA_6_VENC_DDREN_REQ                       Fld(1, 3) //[3:3]
    #define SPM_REQ_STA_6_VENC_APSRC_REQ                       Fld(1, 2) //[2:2]
    #define SPM_REQ_STA_6_VDEC_DDREN_REQ                       Fld(1, 1) //[1:1]
    #define SPM_REQ_STA_6_VDEC_APSRC_REQ                       Fld(1, 0) //[0:0]

#define SPM_REG_SPM_IPC_WAKEUP_REQ                             (SPM_BASE_ADDRESS + 0x00854)
    #define SPM_IPC_WAKEUP_REQ_SPM2ADSP_WAKEUP                 Fld(1, 2) //[2:2]
    #define SPM_IPC_WAKEUP_REQ_SPM2SCP_WAKEUP                  Fld(1, 1) //[1:1]
    #define SPM_IPC_WAKEUP_REQ_SPM2SSPM_WAKEUP                 Fld(1, 0) //[0:0]

#define SPM_REG_IPC_WAKEUP_REQ_MASK_STA                        (SPM_BASE_ADDRESS + 0x00858)
    #define IPC_WAKEUP_REQ_MASK_STA_ADSP2SPM_WAKEUP            Fld(1, 22) //[22:22]
    #define IPC_WAKEUP_REQ_MASK_STA_SCP2SPM_WAKEUP             Fld(1, 21) //[21:21]
    #define IPC_WAKEUP_REQ_MASK_STA_SSPM2SPM_WAKEUP            Fld(1, 20) //[20:20]
    #define IPC_WAKEUP_REQ_MASK_STA_REG_ADSP2SPM_WAKEUP_MASK_B Fld(1, 6) //[6:6]
    #define IPC_WAKEUP_REQ_MASK_STA_REG_SCP2SPM_WAKEUP_MASK_B  Fld(1, 5) //[5:5]
    #define IPC_WAKEUP_REQ_MASK_STA_REG_SSPM2SPM_WAKEUP_MASK_B Fld(1, 4) //[4:4]
    #define IPC_WAKEUP_REQ_MASK_STA_REG_SW2SPM_WAKEUP_MASK_B   Fld(4, 0) //[3:0]

#define SPM_REG_SPM_EVENT_CON_MISC                             (SPM_BASE_ADDRESS + 0x0085C)
    #define SPM_EVENT_CON_MISC_REG_CSYSPWRUP_ACK_MASK          Fld(1, 1) //[1:1]
    #define SPM_EVENT_CON_MISC_REG_SRCCLKEN_FAST_RESP          Fld(1, 0) //[0:0]

#define SPM_REG_DDREN_DBC_CON                                  (SPM_BASE_ADDRESS + 0x00860)
    #define DDREN_DBC_CON_REG_DDREN_DBC_EN                     Fld(1, 16) //[16:16]
    #define DDREN_DBC_CON_REG_DDREN_DBC_LEN                    Fld(10, 0) //[9:0]

#define SPM_REG_SPM_RESOURCE_ACK_CON_0                         (SPM_BASE_ADDRESS + 0x00864)
    #define SPM_RESOURCE_ACK_CON_0_SPM_VRF18_ACK_WAIT_CYCLE    Fld(8, 24) //[31:24]
    #define SPM_RESOURCE_ACK_CON_0_SPM_APSRC_ACK_WAIT_CYCLE    Fld(8, 16) //[23:16]
    #define SPM_RESOURCE_ACK_CON_0_SPM_INFRA_ACK_WAIT_CYCLE    Fld(8, 8) //[15:8]
    #define SPM_RESOURCE_ACK_CON_0_SPM_SRCCLKENA_ACK_WAIT_CYCLE Fld(8, 0) //[7:0]

#define SPM_REG_SPM_RESOURCE_ACK_CON_1                         (SPM_BASE_ADDRESS + 0x00868)
    #define SPM_RESOURCE_ACK_CON_1_SPM_VCORE_ACK_WAIT_CYCLE    Fld(8, 8) //[15:8]
    #define SPM_RESOURCE_ACK_CON_1_SPM_DDREN_ACK_WAIT_CYCLE    Fld(8, 0) //[7:0]

#define SPM_REG_SPM_RESOURCE_ACK_MASK_0                        (SPM_BASE_ADDRESS + 0x0086C)
    #define SPM_RESOURCE_ACK_MASK_0_REG_GPUEB_DDREN_ACK_MASK   Fld(1, 31) //[31:31]
    #define SPM_RESOURCE_ACK_MASK_0_REG_GPUEB_APSRC_ACK_MASK   Fld(1, 30) //[30:30]
    #define SPM_RESOURCE_ACK_MASK_0_REG_GCE_VRF18_ACK_MASK     Fld(1, 29) //[29:29]
    #define SPM_RESOURCE_ACK_MASK_0_REG_GCE_INFRA_ACK_MASK     Fld(1, 28) //[28:28]
    #define SPM_RESOURCE_ACK_MASK_0_REG_GCE_DDREN_ACK_MASK     Fld(1, 27) //[27:27]
    #define SPM_RESOURCE_ACK_MASK_0_REG_GCE_APSRC_ACK_MASK     Fld(1, 26) //[26:26]
    #define SPM_RESOURCE_ACK_MASK_0_REG_MCUPM_VRF18_ACK_MASK   Fld(1, 25) //[25:25]
    #define SPM_RESOURCE_ACK_MASK_0_REG_MCUPM_SRCCLKENA_ACK_MASK Fld(1, 24) //[24:24]
    #define SPM_RESOURCE_ACK_MASK_0_REG_MCUPM_INFRA_ACK_MASK   Fld(1, 23) //[23:23]
    #define SPM_RESOURCE_ACK_MASK_0_REG_MCUPM_DDREN_ACK_MASK   Fld(1, 22) //[22:22]
    #define SPM_RESOURCE_ACK_MASK_0_REG_MCUPM_APSRC_ACK_MASK   Fld(1, 21) //[21:21]
    #define SPM_RESOURCE_ACK_MASK_0_REG_CONN_VRF18_ACK_MASK    Fld(1, 20) //[20:20]
    #define SPM_RESOURCE_ACK_MASK_0_REG_CONN_VCORE_ACK_MASK    Fld(1, 19) //[19:19]
    #define SPM_RESOURCE_ACK_MASK_0_REG_CONN_SRCCLKENA_ACK_MASK Fld(1, 18) //[18:18]
    #define SPM_RESOURCE_ACK_MASK_0_REG_CONN_INFRA_ACK_MASK    Fld(1, 17) //[17:17]
    #define SPM_RESOURCE_ACK_MASK_0_REG_CONN_DDREN_ACK_MASK    Fld(1, 16) //[16:16]
    #define SPM_RESOURCE_ACK_MASK_0_REG_CONN_APSRC_ACK_MASK    Fld(1, 15) //[15:15]
    #define SPM_RESOURCE_ACK_MASK_0_REG_AUDIO_DSP_VRF18_ACK_MASK Fld(1, 14) //[14:14]
    #define SPM_RESOURCE_ACK_MASK_0_REG_AUDIO_DSP_SRCCLKENA_ACK_MASK Fld(1, 13) //[13:13]
    #define SPM_RESOURCE_ACK_MASK_0_REG_AUDIO_DSP_INFRA_ACK_MASK Fld(1, 12) //[12:12]
    #define SPM_RESOURCE_ACK_MASK_0_REG_AUDIO_DSP_DDREN_ACK_MASK Fld(1, 11) //[11:11]
    #define SPM_RESOURCE_ACK_MASK_0_REG_AUDIO_DSP_APSRC_ACK_MASK Fld(1, 10) //[10:10]
    #define SPM_RESOURCE_ACK_MASK_0_REG_APU_VRF18_ACK_MASK     Fld(1, 9) //[9:9]
    #define SPM_RESOURCE_ACK_MASK_0_REG_APU_SRCCLKENA_ACK_MASK Fld(1, 8) //[8:8]
    #define SPM_RESOURCE_ACK_MASK_0_REG_APU_INFRA_ACK_MASK     Fld(1, 7) //[7:7]
    #define SPM_RESOURCE_ACK_MASK_0_REG_APU_DDREN_ACK_MASK     Fld(1, 6) //[6:6]
    #define SPM_RESOURCE_ACK_MASK_0_REG_APU_APSRC_ACK_MASK     Fld(1, 5) //[5:5]
    #define SPM_RESOURCE_ACK_MASK_0_REG_AFE_VRF18_ACK_MASK     Fld(1, 4) //[4:4]
    #define SPM_RESOURCE_ACK_MASK_0_REG_AFE_SRCCLKENA_ACK_MASK Fld(1, 3) //[3:3]
    #define SPM_RESOURCE_ACK_MASK_0_REG_AFE_INFRA_ACK_MASK     Fld(1, 2) //[2:2]
    #define SPM_RESOURCE_ACK_MASK_0_REG_AFE_DDREN_ACK_MASK     Fld(1, 1) //[1:1]
    #define SPM_RESOURCE_ACK_MASK_0_REG_AFE_APSRC_ACK_MASK     Fld(1, 0) //[0:0]

#define SPM_REG_SPM_RESOURCE_ACK_MASK_1                        (SPM_BASE_ADDRESS + 0x00870)
    #define SPM_RESOURCE_ACK_MASK_1_REG_SSPM_APSRC_ACK_MASK    Fld(1, 31) //[31:31]
    #define SPM_RESOURCE_ACK_MASK_1_REG_SCP_VRF18_ACK_MASK     Fld(1, 30) //[30:30]
    #define SPM_RESOURCE_ACK_MASK_1_REG_SCP_SRCCLKENA_ACK_MASK Fld(1, 29) //[29:29]
    #define SPM_RESOURCE_ACK_MASK_1_REG_SCP_INFRA_ACK_MASK     Fld(1, 28) //[28:28]
    #define SPM_RESOURCE_ACK_MASK_1_REG_SCP_DDREN_ACK_MASK     Fld(1, 27) //[27:27]
    #define SPM_RESOURCE_ACK_MASK_1_REG_SCP_APSRC_ACK_MASK     Fld(1, 26) //[26:26]
    #define SPM_RESOURCE_ACK_MASK_1_REG_PCIE1_VRF18_ACK_MASK   Fld(1, 25) //[25:25]
    #define SPM_RESOURCE_ACK_MASK_1_REG_PCIE1_SRCCLKENA_ACK_MASK Fld(1, 24) //[24:24]
    #define SPM_RESOURCE_ACK_MASK_1_REG_PCIE1_INFRA_ACK_MASK   Fld(1, 23) //[23:23]
    #define SPM_RESOURCE_ACK_MASK_1_REG_PCIE1_DDREN_ACK_MASK   Fld(1, 22) //[22:22]
    #define SPM_RESOURCE_ACK_MASK_1_REG_PCIE1_APSRC_ACK_MASK   Fld(1, 21) //[21:21]
    #define SPM_RESOURCE_ACK_MASK_1_REG_PCIE0_VRF18_ACK_MASK   Fld(1, 20) //[20:20]
    #define SPM_RESOURCE_ACK_MASK_1_REG_PCIE0_SRCCLKENA_ACK_MASK Fld(1, 19) //[19:19]
    #define SPM_RESOURCE_ACK_MASK_1_REG_PCIE0_INFRA_ACK_MASK   Fld(1, 18) //[18:18]
    #define SPM_RESOURCE_ACK_MASK_1_REG_PCIE0_DDREN_ACK_MASK   Fld(1, 17) //[17:17]
    #define SPM_RESOURCE_ACK_MASK_1_REG_PCIE0_APSRC_ACK_MASK   Fld(1, 16) //[16:16]
    #define SPM_RESOURCE_ACK_MASK_1_REG_MM_PROC_VRF18_ACK_MASK Fld(1, 15) //[15:15]
    #define SPM_RESOURCE_ACK_MASK_1_REG_MM_PROC_SRCCLKENA_ACK_MASK Fld(1, 14) //[14:14]
    #define SPM_RESOURCE_ACK_MASK_1_REG_MM_PROC_INFRA_ACK_MASK Fld(1, 13) //[13:13]
    #define SPM_RESOURCE_ACK_MASK_1_REG_MM_PROC_DDREN_ACK_MASK Fld(1, 12) //[12:12]
    #define SPM_RESOURCE_ACK_MASK_1_REG_MM_PROC_APSRC_ACK_MASK Fld(1, 11) //[11:11]
    #define SPM_RESOURCE_ACK_MASK_1_REG_MDP_DDREN_ACK_MASK     Fld(1, 10) //[10:10]
    #define SPM_RESOURCE_ACK_MASK_1_REG_MDP_APSRC_ACK_MASK     Fld(1, 9) //[9:9]
    #define SPM_RESOURCE_ACK_MASK_1_REG_MD_VRF18_ACK_MASK      Fld(1, 8) //[8:8]
    #define SPM_RESOURCE_ACK_MASK_1_REG_MD_VCORE_ACK_MASK      Fld(1, 7) //[7:7]
    #define SPM_RESOURCE_ACK_MASK_1_REG_MD_SRCCLKENA_ACK_MASK  Fld(1, 6) //[6:6]
    #define SPM_RESOURCE_ACK_MASK_1_REG_MD_INFRA_ACK_MASK      Fld(1, 5) //[5:5]
    #define SPM_RESOURCE_ACK_MASK_1_REG_MD_DDREN_ACK_MASK      Fld(1, 4) //[4:4]
    #define SPM_RESOURCE_ACK_MASK_1_REG_MD_APSRC_ACK_MASK      Fld(1, 3) //[3:3]
    #define SPM_RESOURCE_ACK_MASK_1_REG_GPUEB_VRF18_ACK_MASK   Fld(1, 2) //[2:2]
    #define SPM_RESOURCE_ACK_MASK_1_REG_GPUEB_SRCCLKENA_ACK_MASK Fld(1, 1) //[1:1]
    #define SPM_RESOURCE_ACK_MASK_1_REG_GPUEB_INFRA_ACK_MASK   Fld(1, 0) //[0:0]

#define SPM_REG_SPM_RESOURCE_ACK_MASK_2                        (SPM_BASE_ADDRESS + 0x00874)
    #define SPM_RESOURCE_ACK_MASK_2_REG_UFS_VRF18_ACK_MASK     Fld(1, 18) //[18:18]
    #define SPM_RESOURCE_ACK_MASK_2_REG_UFS_SRCCLKENA_ACK_MASK Fld(1, 17) //[17:17]
    #define SPM_RESOURCE_ACK_MASK_2_REG_UFS_INFRA_ACK_MASK     Fld(1, 16) //[16:16]
    #define SPM_RESOURCE_ACK_MASK_2_REG_UFS_DDREN_ACK_MASK     Fld(1, 15) //[15:15]
    #define SPM_RESOURCE_ACK_MASK_2_REG_UFS_APSRC_ACK_MASK     Fld(1, 14) //[14:14]
    #define SPM_RESOURCE_ACK_MASK_2_REG_SSUSB1_VRF18_ACK_MASK  Fld(1, 13) //[13:13]
    #define SPM_RESOURCE_ACK_MASK_2_REG_SSUSB1_SRCCLKENA_ACK_MASK Fld(1, 12) //[12:12]
    #define SPM_RESOURCE_ACK_MASK_2_REG_SSUSB1_INFRA_ACK_MASK  Fld(1, 11) //[11:11]
    #define SPM_RESOURCE_ACK_MASK_2_REG_SSUSB1_DDREN_ACK_MASK  Fld(1, 10) //[10:10]
    #define SPM_RESOURCE_ACK_MASK_2_REG_SSUSB1_APSRC_ACK_MASK  Fld(1, 9) //[9:9]
    #define SPM_RESOURCE_ACK_MASK_2_REG_SSUSB0_VRF18_ACK_MASK  Fld(1, 8) //[8:8]
    #define SPM_RESOURCE_ACK_MASK_2_REG_SSUSB0_SRCCLKENA_ACK_MASK Fld(1, 7) //[7:7]
    #define SPM_RESOURCE_ACK_MASK_2_REG_SSUSB0_INFRA_ACK_MASK  Fld(1, 6) //[6:6]
    #define SPM_RESOURCE_ACK_MASK_2_REG_SSUSB0_DDREN_ACK_MASK  Fld(1, 5) //[5:5]
    #define SPM_RESOURCE_ACK_MASK_2_REG_SSUSB0_APSRC_ACK_MASK  Fld(1, 4) //[4:4]
    #define SPM_RESOURCE_ACK_MASK_2_REG_SSPM_VRF18_ACK_MASK    Fld(1, 3) //[3:3]
    #define SPM_RESOURCE_ACK_MASK_2_REG_SSPM_SRCCLKENA_ACK_MASK Fld(1, 2) //[2:2]
    #define SPM_RESOURCE_ACK_MASK_2_REG_SSPM_INFRA_ACK_MASK    Fld(1, 1) //[1:1]
    #define SPM_RESOURCE_ACK_MASK_2_REG_SSPM_DDREN_ACK_MASK    Fld(1, 0) //[0:0]

#define SPM_REG_SPM_EVENT_COUNTER_CLEAR                        (SPM_BASE_ADDRESS + 0x00878)
    #define SPM_EVENT_COUNTER_CLEAR_REG_SPM_EVENT_COUNTER_CLR  Fld(1, 0) //[0:0]

#define SPM_REG_SPM_SRCCLKENA_EVENT_COUNT_STA                  (SPM_BASE_ADDRESS + 0x0087C)
    #define SPM_SRCCLKENA_EVENT_COUNT_STA_SPM_SRCCLKENA_WAKE_COUNT Fld(16, 16) //[31:16]
    #define SPM_SRCCLKENA_EVENT_COUNT_STA_SPM_SRCCLKENA_SLEEP_COUNT Fld(16, 0) //[15:0]

#define SPM_REG_SPM_INFRA_EVENT_COUNT_STA                      (SPM_BASE_ADDRESS + 0x00880)
    #define SPM_INFRA_EVENT_COUNT_STA_SPM_INFRA_WAKE_COUNT     Fld(16, 16) //[31:16]
    #define SPM_INFRA_EVENT_COUNT_STA_SPM_INFRA_SLEEP_COUNT    Fld(16, 0) //[15:0]

#define SPM_REG_SPM_APSRC_EVENT_COUNT_STA                      (SPM_BASE_ADDRESS + 0x00884)
    #define SPM_APSRC_EVENT_COUNT_STA_SPM_APSRC_WAKE_COUNT     Fld(16, 16) //[31:16]
    #define SPM_APSRC_EVENT_COUNT_STA_SPM_APSRC_SLEEP_COUNT    Fld(16, 0) //[15:0]

#define SPM_REG_SPM_VRF18_EVENT_COUNT_STA                      (SPM_BASE_ADDRESS + 0x00888)
    #define SPM_VRF18_EVENT_COUNT_STA_SPM_VRF18_WAKE_COUNT     Fld(16, 16) //[31:16]
    #define SPM_VRF18_EVENT_COUNT_STA_SPM_VRF18_SLEEP_COUNT    Fld(16, 0) //[15:0]

#define SPM_REG_SPM_DDREN_EVENT_COUNT_STA                      (SPM_BASE_ADDRESS + 0x0088C)
    #define SPM_DDREN_EVENT_COUNT_STA_SPM_DDREN_WAKE_COUNT     Fld(16, 16) //[31:16]
    #define SPM_DDREN_EVENT_COUNT_STA_SPM_DDREN_SLEEP_COUNT    Fld(16, 0) //[15:0]

#define SPM_REG_SPM_VCORE_EVENT_COUNT_STA                      (SPM_BASE_ADDRESS + 0x00890)
    #define SPM_VCORE_EVENT_COUNT_STA_SPM_VCORE_WAKE_COUNT     Fld(16, 16) //[31:16]
    #define SPM_VCORE_EVENT_COUNT_STA_SPM_VCORE_SLEEP_COUNT    Fld(16, 0) //[15:0]

#define SPM_REG_PCM_WDT_LATCH_0                                (SPM_BASE_ADDRESS + 0x00900)
    #define PCM_WDT_LATCH_0_PCM_WDT_LATCH_0                    Fld(32, 0) //[31:0]

#define SPM_REG_PCM_WDT_LATCH_1                                (SPM_BASE_ADDRESS + 0x00904)
    #define PCM_WDT_LATCH_1_PCM_WDT_LATCH_1                    Fld(32, 0) //[31:0]

#define SPM_REG_PCM_WDT_LATCH_2                                (SPM_BASE_ADDRESS + 0x00908)
    #define PCM_WDT_LATCH_2_PCM_WDT_LATCH_2                    Fld(32, 0) //[31:0]

#define SPM_REG_PCM_WDT_LATCH_3                                (SPM_BASE_ADDRESS + 0x0090C)
    #define PCM_WDT_LATCH_3_PCM_WDT_LATCH_3                    Fld(32, 0) //[31:0]

#define SPM_REG_PCM_WDT_LATCH_4                                (SPM_BASE_ADDRESS + 0x00910)
    #define PCM_WDT_LATCH_4_PCM_WDT_LATCH_4                    Fld(32, 0) //[31:0]

#define SPM_REG_PCM_WDT_LATCH_5                                (SPM_BASE_ADDRESS + 0x00914)
    #define PCM_WDT_LATCH_5_PCM_WDT_LATCH_5                    Fld(32, 0) //[31:0]

#define SPM_REG_PCM_WDT_LATCH_6                                (SPM_BASE_ADDRESS + 0x00918)
    #define PCM_WDT_LATCH_6_PCM_WDT_LATCH_6                    Fld(32, 0) //[31:0]

#define SPM_REG_PCM_WDT_LATCH_7                                (SPM_BASE_ADDRESS + 0x0091C)
    #define PCM_WDT_LATCH_7_PCM_WDT_LATCH_7                    Fld(32, 0) //[31:0]

#define SPM_REG_PCM_WDT_LATCH_8                                (SPM_BASE_ADDRESS + 0x00920)
    #define PCM_WDT_LATCH_8_PCM_WDT_LATCH_8                    Fld(32, 0) //[31:0]

#define SPM_REG_PCM_WDT_LATCH_9                                (SPM_BASE_ADDRESS + 0x00924)
    #define PCM_WDT_LATCH_9_PCM_WDT_LATCH_9                    Fld(32, 0) //[31:0]

#define SPM_REG_PCM_WDT_LATCH_10                               (SPM_BASE_ADDRESS + 0x00928)
    #define PCM_WDT_LATCH_10_PCM_WDT_LATCH_10                  Fld(32, 0) //[31:0]

#define SPM_REG_PCM_WDT_LATCH_11                               (SPM_BASE_ADDRESS + 0x0092C)
    #define PCM_WDT_LATCH_11_PCM_WDT_LATCH_11                  Fld(32, 0) //[31:0]

#define SPM_REG_PCM_WDT_LATCH_12                               (SPM_BASE_ADDRESS + 0x00930)
    #define PCM_WDT_LATCH_12_PCM_WDT_LATCH_12                  Fld(32, 0) //[31:0]

#define SPM_REG_PCM_WDT_LATCH_13                               (SPM_BASE_ADDRESS + 0x00934)
    #define PCM_WDT_LATCH_13_PCM_WDT_LATCH_13                  Fld(32, 0) //[31:0]

#define SPM_REG_PCM_WDT_LATCH_14                               (SPM_BASE_ADDRESS + 0x00938)
    #define PCM_WDT_LATCH_14_PCM_WDT_LATCH_14                  Fld(32, 0) //[31:0]

#define SPM_REG_PCM_WDT_LATCH_15                               (SPM_BASE_ADDRESS + 0x0093C)
    #define PCM_WDT_LATCH_15_PCM_WDT_LATCH_15                  Fld(32, 0) //[31:0]

#define SPM_REG_PCM_WDT_LATCH_16                               (SPM_BASE_ADDRESS + 0x00940)
    #define PCM_WDT_LATCH_16_PCM_WDT_LATCH_16                  Fld(32, 0) //[31:0]

#define SPM_REG_PCM_WDT_LATCH_17                               (SPM_BASE_ADDRESS + 0x00944)
    #define PCM_WDT_LATCH_17_PCM_WDT_LATCH_17                  Fld(32, 0) //[31:0]

#define SPM_REG_PCM_WDT_LATCH_18                               (SPM_BASE_ADDRESS + 0x00948)
    #define PCM_WDT_LATCH_18_PCM_WDT_LATCH_18                  Fld(32, 0) //[31:0]

#define SPM_REG_PCM_WDT_LATCH_19                               (SPM_BASE_ADDRESS + 0x0094C)
    #define PCM_WDT_LATCH_19_PCM_WDT_LATCH_19                  Fld(32, 0) //[31:0]

#define SPM_REG_PCM_WDT_LATCH_20                               (SPM_BASE_ADDRESS + 0x00950)
    #define PCM_WDT_LATCH_20_PCM_WDT_LATCH_20                  Fld(32, 0) //[31:0]

#define SPM_REG_PCM_WDT_LATCH_21                               (SPM_BASE_ADDRESS + 0x00954)
    #define PCM_WDT_LATCH_21_PCM_WDT_LATCH_21                  Fld(32, 0) //[31:0]

#define SPM_REG_PCM_WDT_LATCH_22                               (SPM_BASE_ADDRESS + 0x00958)
    #define PCM_WDT_LATCH_22_PCM_WDT_LATCH_22                  Fld(32, 0) //[31:0]

#define SPM_REG_PCM_WDT_LATCH_23                               (SPM_BASE_ADDRESS + 0x0095C)
    #define PCM_WDT_LATCH_23_PCM_WDT_LATCH_23                  Fld(32, 0) //[31:0]

#define SPM_REG_PCM_WDT_LATCH_SPARE_0                          (SPM_BASE_ADDRESS + 0x00960)
    #define PCM_WDT_LATCH_SPARE_0_PCM_WDT_LATCH_SPARE_0        Fld(32, 0) //[31:0]

#define SPM_REG_PCM_WDT_LATCH_SPARE_4                          (SPM_BASE_ADDRESS + 0x00964)
    #define PCM_WDT_LATCH_SPARE_4_PCM_WDT_LATCH_SPARE_4        Fld(32, 0) //[31:0]

#define SPM_REG_PCM_WDT_LATCH_SPARE_6                          (SPM_BASE_ADDRESS + 0x00968)
    #define PCM_WDT_LATCH_SPARE_6_PCM_WDT_LATCH_SPARE_6        Fld(32, 0) //[31:0]

#define SPM_REG_PCM_WDT_LATCH_SPARE_2                          (SPM_BASE_ADDRESS + 0x0096C)
    #define PCM_WDT_LATCH_SPARE_2_PCM_WDT_LATCH_SPARE_2        Fld(32, 0) //[31:0]

#define SPM_REG_PCM_WDT_LATCH_SPARE_5                          (SPM_BASE_ADDRESS + 0x00970)
    #define PCM_WDT_LATCH_SPARE_5_PCM_WDT_LATCH_SPARE_5        Fld(32, 0) //[31:0]

#define SPM_REG_PCM_WDT_LATCH_SPARE_3                          (SPM_BASE_ADDRESS + 0x00974)
    #define PCM_WDT_LATCH_SPARE_3_PCM_WDT_LATCH_SPARE_3        Fld(32, 0) //[31:0]

#define SPM_REG_PCM_WDT_LATCH_SPARE_1                          (SPM_BASE_ADDRESS + 0x00978)
    #define PCM_WDT_LATCH_SPARE_1_PCM_WDT_LATCH_SPARE_1        Fld(32, 0) //[31:0]

#define SPM_REG_DRAMC_GATING_ERR_LATCH_0                       (SPM_BASE_ADDRESS + 0x0097C)
    #define DRAMC_GATING_ERR_LATCH_0_DRAMC_GATING_ERR_LATCH_0  Fld(32, 0) //[31:0]

#define SPM_REG_DRAMC_GATING_ERR_LATCH_1                       (SPM_BASE_ADDRESS + 0x00980)
    #define DRAMC_GATING_ERR_LATCH_1_DRAMC_GATING_ERR_LATCH_1  Fld(32, 0) //[31:0]

#define SPM_REG_DRAMC_GATING_ERR_LATCH_2                       (SPM_BASE_ADDRESS + 0x00984)
    #define DRAMC_GATING_ERR_LATCH_2_DRAMC_GATING_ERR_LATCH_2  Fld(32, 0) //[31:0]

#define SPM_REG_DRAMC_GATING_ERR_LATCH_3                       (SPM_BASE_ADDRESS + 0x00988)
    #define DRAMC_GATING_ERR_LATCH_3_DRAMC_GATING_ERR_LATCH_3  Fld(32, 0) //[31:0]

#define SPM_REG_DRAMC_GATING_ERR_LATCH_4                       (SPM_BASE_ADDRESS + 0x0098C)
    #define DRAMC_GATING_ERR_LATCH_4_DRAMC_GATING_ERR_LATCH_4  Fld(32, 0) //[31:0]

#define SPM_REG_DRAMC_GATING_ERR_LATCH_5                       (SPM_BASE_ADDRESS + 0x00990)
    #define DRAMC_GATING_ERR_LATCH_5_DRAMC_GATING_ERR_LATCH_5  Fld(32, 0) //[31:0]

#define SPM_REG_DRAMC_GATING_ERR_LATCH_SPARE_0                 (SPM_BASE_ADDRESS + 0x00994)
    #define DRAMC_GATING_ERR_LATCH_SPARE_0_DRAMC_GATING_ERR_LATCH_SPARE_0 Fld(32, 0) //[31:0]

#define SPM_REG_SPM_DEBUG_CON                                  (SPM_BASE_ADDRESS + 0x00998)
    #define SPM_DEBUG_CON_SPM_DEBUG_OUT_ENABLE                 Fld(1, 0) //[0:0]

#define SPM_REG_SPM_ACK_CHK_CON_0                              (SPM_BASE_ADDRESS + 0x0099C)
    #define SPM_ACK_CHK_CON_0_SPM_ACK_CHK_SW_EN_0              Fld(1, 0) //[0:0]
    #define SPM_ACK_CHK_CON_0_SPM_ACK_CHK_CLR_ALL_0            Fld(1, 1) //[1:1]
    #define SPM_ACK_CHK_CON_0_SPM_ACK_CHK_CLR_TIMER_0          Fld(1, 2) //[2:2]
    #define SPM_ACK_CHK_CON_0_SPM_ACK_CHK_CLR_IRQ_0            Fld(1, 3) //[3:3]
    #define SPM_ACK_CHK_CON_0_SPM_ACK_CHK_STA_EN_0             Fld(1, 4) //[4:4]
    #define SPM_ACK_CHK_CON_0_SPM_ACK_CHK_WAKEUP_EN_0          Fld(1, 5) //[5:5]
    #define SPM_ACK_CHK_CON_0_SPM_ACK_CHK_WDT_EN_0             Fld(1, 6) //[6:6]
    #define SPM_ACK_CHK_CON_0_SPM_ACK_CHK_SWINT_EN_0           Fld(1, 7) //[7:7]
    #define SPM_ACK_CHK_CON_0_SPM_ACK_CHK_HW_EN_0              Fld(1, 8) //[8:8]
    #define SPM_ACK_CHK_CON_0_SPM_ACK_CHK_HW_MODE_0            Fld(3, 9) //[11:9]
    #define SPM_ACK_CHK_CON_0_SPM_ACK_CHK_FAIL_0               Fld(1, 15) //[15:15]

#define SPM_REG_SPM_ACK_CHK_SEL_0                              (SPM_BASE_ADDRESS + 0x009A0)
    #define SPM_ACK_CHK_SEL_0_SPM_ACK_CHK_HW_TRIG_SIGNAL_SEL_0 Fld(5, 0) //[4:0]
    #define SPM_ACK_CHK_SEL_0_SPM_ACK_CHK_HW_TRIG_GROUP_SEL_0  Fld(3, 5) //[7:5]
    #define SPM_ACK_CHK_SEL_0_SPM_ACK_CHK_HW_TARG_SIGNAL_SEL_0 Fld(5, 16) //[20:16]
    #define SPM_ACK_CHK_SEL_0_SPM_ACK_CHK_HW_TARG_GROUP_SEL_0  Fld(3, 21) //[23:21]

#define SPM_REG_SPM_ACK_CHK_TIMER_0                            (SPM_BASE_ADDRESS + 0x009A4)
    #define SPM_ACK_CHK_TIMER_0_SPM_ACK_CHK_TIMER_VAL_0        Fld(16, 0) //[15:0]
    #define SPM_ACK_CHK_TIMER_0_SPM_ACK_CHK_TIMER_0            Fld(16, 16) //[31:16]

#define SPM_REG_SPM_ACK_CHK_STA_0                              (SPM_BASE_ADDRESS + 0x009A8)
    #define SPM_ACK_CHK_STA_0_SPM_ACK_CHK_STA_0                Fld(32, 0) //[31:0]

#define SPM_REG_SPM_ACK_CHK_CON_1                              (SPM_BASE_ADDRESS + 0x009AC)
    #define SPM_ACK_CHK_CON_1_SPM_ACK_CHK_SW_EN_1              Fld(1, 0) //[0:0]
    #define SPM_ACK_CHK_CON_1_SPM_ACK_CHK_CLR_ALL_1            Fld(1, 1) //[1:1]
    #define SPM_ACK_CHK_CON_1_SPM_ACK_CHK_CLR_TIMER_1          Fld(1, 2) //[2:2]
    #define SPM_ACK_CHK_CON_1_SPM_ACK_CHK_CLR_IRQ_1            Fld(1, 3) //[3:3]
    #define SPM_ACK_CHK_CON_1_SPM_ACK_CHK_STA_EN_1             Fld(1, 4) //[4:4]
    #define SPM_ACK_CHK_CON_1_SPM_ACK_CHK_WAKEUP_EN_1          Fld(1, 5) //[5:5]
    #define SPM_ACK_CHK_CON_1_SPM_ACK_CHK_WDT_EN_1             Fld(1, 6) //[6:6]
    #define SPM_ACK_CHK_CON_1_SPM_ACK_CHK_SWINT_EN_1           Fld(1, 7) //[7:7]
    #define SPM_ACK_CHK_CON_1_SPM_ACK_CHK_HW_EN_1              Fld(1, 8) //[8:8]
    #define SPM_ACK_CHK_CON_1_SPM_ACK_CHK_HW_MODE_1            Fld(3, 9) //[11:9]
    #define SPM_ACK_CHK_CON_1_SPM_ACK_CHK_FAIL_1               Fld(1, 15) //[15:15]

#define SPM_REG_SPM_ACK_CHK_SEL_1                              (SPM_BASE_ADDRESS + 0x009B0)
    #define SPM_ACK_CHK_SEL_1_SPM_ACK_CHK_HW_TRIG_SIGNAL_SEL_1 Fld(5, 0) //[4:0]
    #define SPM_ACK_CHK_SEL_1_SPM_ACK_CHK_HW_TRIG_GROUP_SEL_1  Fld(3, 5) //[7:5]
    #define SPM_ACK_CHK_SEL_1_SPM_ACK_CHK_HW_TARG_SIGNAL_SEL_1 Fld(5, 16) //[20:16]
    #define SPM_ACK_CHK_SEL_1_SPM_ACK_CHK_HW_TARG_GROUP_SEL_1  Fld(3, 21) //[23:21]

#define SPM_REG_SPM_ACK_CHK_TIMER_1                            (SPM_BASE_ADDRESS + 0x009B4)
    #define SPM_ACK_CHK_TIMER_1_SPM_ACK_CHK_TIMER_VAL_1        Fld(16, 0) //[15:0]
    #define SPM_ACK_CHK_TIMER_1_SPM_ACK_CHK_TIMER_1            Fld(16, 16) //[31:16]

#define SPM_REG_SPM_ACK_CHK_STA_1                              (SPM_BASE_ADDRESS + 0x009B8)
    #define SPM_ACK_CHK_STA_1_SPM_ACK_CHK_STA_1                Fld(32, 0) //[31:0]

#define SPM_REG_SPM_ACK_CHK_CON_2                              (SPM_BASE_ADDRESS + 0x009BC)
    #define SPM_ACK_CHK_CON_2_SPM_ACK_CHK_SW_EN_2              Fld(1, 0) //[0:0]
    #define SPM_ACK_CHK_CON_2_SPM_ACK_CHK_CLR_ALL_2            Fld(1, 1) //[1:1]
    #define SPM_ACK_CHK_CON_2_SPM_ACK_CHK_CLR_TIMER_2          Fld(1, 2) //[2:2]
    #define SPM_ACK_CHK_CON_2_SPM_ACK_CHK_CLR_IRQ_2            Fld(1, 3) //[3:3]
    #define SPM_ACK_CHK_CON_2_SPM_ACK_CHK_STA_EN_2             Fld(1, 4) //[4:4]
    #define SPM_ACK_CHK_CON_2_SPM_ACK_CHK_WAKEUP_EN_2          Fld(1, 5) //[5:5]
    #define SPM_ACK_CHK_CON_2_SPM_ACK_CHK_WDT_EN_2             Fld(1, 6) //[6:6]
    #define SPM_ACK_CHK_CON_2_SPM_ACK_CHK_SWINT_EN_2           Fld(1, 7) //[7:7]
    #define SPM_ACK_CHK_CON_2_SPM_ACK_CHK_HW_EN_2              Fld(1, 8) //[8:8]
    #define SPM_ACK_CHK_CON_2_SPM_ACK_CHK_HW_MODE_2            Fld(3, 9) //[11:9]
    #define SPM_ACK_CHK_CON_2_SPM_ACK_CHK_FAIL_2               Fld(1, 15) //[15:15]

#define SPM_REG_SPM_ACK_CHK_SEL_2                              (SPM_BASE_ADDRESS + 0x009C0)
    #define SPM_ACK_CHK_SEL_2_SPM_ACK_CHK_HW_TRIG_SIGNAL_SEL_2 Fld(5, 0) //[4:0]
    #define SPM_ACK_CHK_SEL_2_SPM_ACK_CHK_HW_TRIG_GROUP_SEL_2  Fld(3, 5) //[7:5]
    #define SPM_ACK_CHK_SEL_2_SPM_ACK_CHK_HW_TARG_SIGNAL_SEL_2 Fld(5, 16) //[20:16]
    #define SPM_ACK_CHK_SEL_2_SPM_ACK_CHK_HW_TARG_GROUP_SEL_2  Fld(3, 21) //[23:21]

#define SPM_REG_SPM_ACK_CHK_TIMER_2                            (SPM_BASE_ADDRESS + 0x009C4)
    #define SPM_ACK_CHK_TIMER_2_SPM_ACK_CHK_TIMER_VAL_2        Fld(16, 0) //[15:0]
    #define SPM_ACK_CHK_TIMER_2_SPM_ACK_CHK_TIMER_2            Fld(16, 16) //[31:16]

#define SPM_REG_SPM_ACK_CHK_STA_2                              (SPM_BASE_ADDRESS + 0x009C8)
    #define SPM_ACK_CHK_STA_2_SPM_ACK_CHK_STA_2                Fld(32, 0) //[31:0]

#define SPM_REG_SPM_ACK_CHK_CON_3                              (SPM_BASE_ADDRESS + 0x009CC)
    #define SPM_ACK_CHK_CON_3_SPM_ACK_CHK_SW_EN_3              Fld(1, 0) //[0:0]
    #define SPM_ACK_CHK_CON_3_SPM_ACK_CHK_CLR_ALL_3            Fld(1, 1) //[1:1]
    #define SPM_ACK_CHK_CON_3_SPM_ACK_CHK_CLR_TIMER_3          Fld(1, 2) //[2:2]
    #define SPM_ACK_CHK_CON_3_SPM_ACK_CHK_CLR_IRQ_3            Fld(1, 3) //[3:3]
    #define SPM_ACK_CHK_CON_3_SPM_ACK_CHK_STA_EN_3             Fld(1, 4) //[4:4]
    #define SPM_ACK_CHK_CON_3_SPM_ACK_CHK_WAKEUP_EN_3          Fld(1, 5) //[5:5]
    #define SPM_ACK_CHK_CON_3_SPM_ACK_CHK_WDT_EN_3             Fld(1, 6) //[6:6]
    #define SPM_ACK_CHK_CON_3_SPM_ACK_CHK_SWINT_EN_3           Fld(1, 7) //[7:7]
    #define SPM_ACK_CHK_CON_3_SPM_ACK_CHK_HW_EN_3              Fld(1, 8) //[8:8]
    #define SPM_ACK_CHK_CON_3_SPM_ACK_CHK_HW_MODE_3            Fld(3, 9) //[11:9]
    #define SPM_ACK_CHK_CON_3_SPM_ACK_CHK_FAIL_3               Fld(1, 15) //[15:15]

#define SPM_REG_SPM_ACK_CHK_SEL_3                              (SPM_BASE_ADDRESS + 0x009D0)
    #define SPM_ACK_CHK_SEL_3_SPM_ACK_CHK_HW_TRIG_SIGNAL_SEL_3 Fld(5, 0) //[4:0]
    #define SPM_ACK_CHK_SEL_3_SPM_ACK_CHK_HW_TRIG_GROUP_SEL_3  Fld(3, 5) //[7:5]
    #define SPM_ACK_CHK_SEL_3_SPM_ACK_CHK_HW_TARG_SIGNAL_SEL_3 Fld(5, 16) //[20:16]
    #define SPM_ACK_CHK_SEL_3_SPM_ACK_CHK_HW_TARG_GROUP_SEL_3  Fld(3, 21) //[23:21]

#define SPM_REG_SPM_ACK_CHK_TIMER_3                            (SPM_BASE_ADDRESS + 0x009D4)
    #define SPM_ACK_CHK_TIMER_3_SPM_ACK_CHK_TIMER_VAL_3        Fld(16, 0) //[15:0]
    #define SPM_ACK_CHK_TIMER_3_SPM_ACK_CHK_TIMER_3            Fld(16, 16) //[31:16]

#define SPM_REG_SPM_ACK_CHK_STA_3                              (SPM_BASE_ADDRESS + 0x009D8)
    #define SPM_ACK_CHK_STA_3_SPM_ACK_CHK_STA_3                Fld(32, 0) //[31:0]

#define SPM_REG_MD1_PWR_CON                                    (SPM_BASE_ADDRESS + 0x00E00)
    #define MD1_PWR_CON_SC_MD1_PWR_ACK_2ND                     Fld(1, 31) //[31:31]
    #define MD1_PWR_CON_SC_MD1_PWR_ACK                         Fld(1, 30) //[30:30]
    #define MD1_PWR_CON_MD1_PWR_CLK_DIS                        Fld(1, 4) //[4:4]
    #define MD1_PWR_CON_MD1_PWR_ON_2ND                         Fld(1, 3) //[3:3]
    #define MD1_PWR_CON_MD1_PWR_ON                             Fld(1, 2) //[2:2]
    #define MD1_PWR_CON_MD1_PWR_ISO                            Fld(1, 1) //[1:1]
    #define MD1_PWR_CON_MD1_PWR_RST_B                          Fld(1, 0) //[0:0]

#define SPM_REG_CONN_PWR_CON                                   (SPM_BASE_ADDRESS + 0x00E04)
    #define CONN_PWR_CON_SC_CONN_PWR_ACK_2ND                   Fld(1, 31) //[31:31]
    #define CONN_PWR_CON_SC_CONN_PWR_ACK                       Fld(1, 30) //[30:30]
    #define CONN_PWR_CON_CONN_PWR_CLK_DIS                      Fld(1, 4) //[4:4]
    #define CONN_PWR_CON_CONN_PWR_ON_2ND                       Fld(1, 3) //[3:3]
    #define CONN_PWR_CON_CONN_PWR_ON                           Fld(1, 2) //[2:2]
    #define CONN_PWR_CON_CONN_PWR_ISO                          Fld(1, 1) //[1:1]
    #define CONN_PWR_CON_CONN_PWR_RST_B                        Fld(1, 0) //[0:0]

#define SPM_REG_IFR_PWR_CON                                    (SPM_BASE_ADDRESS + 0x00E08)
    #define IFR_PWR_CON_SC_IFR_PWR_ACK_2ND                     Fld(1, 31) //[31:31]
    #define IFR_PWR_CON_SC_IFR_PWR_ACK                         Fld(1, 30) //[30:30]
    #define IFR_PWR_CON_SC_IFR_SRAM_SLEEP_B_ACK                Fld(1, 13) //[13:13]
    #define IFR_PWR_CON_SC_IFR_SRAM_PDN_ACK                    Fld(1, 12) //[12:12]
    #define IFR_PWR_CON_IFR_SRAM_SLEEP_B                       Fld(1, 9) //[9:9]
    #define IFR_PWR_CON_IFR_SRAM_PDN                           Fld(1, 8) //[8:8]
    #define IFR_PWR_CON_IFR_SRAM_ISOINT_B                      Fld(1, 6) //[6:6]
    #define IFR_PWR_CON_IFR_SRAM_CKISO                         Fld(1, 5) //[5:5]
    #define IFR_PWR_CON_IFR_PWR_CLK_DIS                        Fld(1, 4) //[4:4]
    #define IFR_PWR_CON_IFR_PWR_ON_2ND                         Fld(1, 3) //[3:3]
    #define IFR_PWR_CON_IFR_PWR_ON                             Fld(1, 2) //[2:2]
    #define IFR_PWR_CON_IFR_PWR_ISO                            Fld(1, 1) //[1:1]
    #define IFR_PWR_CON_IFR_PWR_RST_B                          Fld(1, 0) //[0:0]

#define SPM_REG_PERI_PWR_CON                                   (SPM_BASE_ADDRESS + 0x00E0C)
    #define PERI_PWR_CON_SC_PERI_PWR_ACK_2ND                   Fld(1, 31) //[31:31]
    #define PERI_PWR_CON_SC_PERI_PWR_ACK                       Fld(1, 30) //[30:30]
    #define PERI_PWR_CON_SC_PERI_SRAM_SLEEP_B_ACK              Fld(1, 13) //[13:13]
    #define PERI_PWR_CON_SC_PERI_SRAM_PDN_ACK                  Fld(1, 12) //[12:12]
    #define PERI_PWR_CON_PERI_SRAM_SLEEP_B                     Fld(1, 9) //[9:9]
    #define PERI_PWR_CON_PERI_SRAM_PDN                         Fld(1, 8) //[8:8]
    #define PERI_PWR_CON_PERI_SRAM_ISOINT_B                    Fld(1, 6) //[6:6]
    #define PERI_PWR_CON_PERI_SRAM_CKISO                       Fld(1, 5) //[5:5]
    #define PERI_PWR_CON_PERI_PWR_CLK_DIS                      Fld(1, 4) //[4:4]
    #define PERI_PWR_CON_PERI_PWR_ON_2ND                       Fld(1, 3) //[3:3]
    #define PERI_PWR_CON_PERI_PWR_ON                           Fld(1, 2) //[2:2]
    #define PERI_PWR_CON_PERI_PWR_ISO                          Fld(1, 1) //[1:1]
    #define PERI_PWR_CON_PERI_PWR_RST_B                        Fld(1, 0) //[0:0]

#define SPM_REG_UFS0_PWR_CON                                   (SPM_BASE_ADDRESS + 0x00E10)
    #define UFS0_PWR_CON_SC_UFS0_PWR_ACK_2ND                   Fld(1, 31) //[31:31]
    #define UFS0_PWR_CON_SC_UFS0_PWR_ACK                       Fld(1, 30) //[30:30]
    #define UFS0_PWR_CON_SC_UFS0_SRAM_SLEEP_B_ACK              Fld(1, 13) //[13:13]
    #define UFS0_PWR_CON_SC_UFS0_SRAM_PDN_ACK                  Fld(1, 12) //[12:12]
    #define UFS0_PWR_CON_UFS0_SRAM_SLEEP_B                     Fld(1, 9) //[9:9]
    #define UFS0_PWR_CON_UFS0_SRAM_PDN                         Fld(1, 8) //[8:8]
    #define UFS0_PWR_CON_UFS0_SRAM_ISOINT_B                    Fld(1, 6) //[6:6]
    #define UFS0_PWR_CON_UFS0_SRAM_CKISO                       Fld(1, 5) //[5:5]
    #define UFS0_PWR_CON_UFS0_PWR_CLK_DIS                      Fld(1, 4) //[4:4]
    #define UFS0_PWR_CON_UFS0_PWR_ON_2ND                       Fld(1, 3) //[3:3]
    #define UFS0_PWR_CON_UFS0_PWR_ON                           Fld(1, 2) //[2:2]
    #define UFS0_PWR_CON_UFS0_PWR_ISO                          Fld(1, 1) //[1:1]
    #define UFS0_PWR_CON_UFS0_PWR_RST_B                        Fld(1, 0) //[0:0]

#define SPM_REG_AUDIO_PWR_CON                                  (SPM_BASE_ADDRESS + 0x00E14)
    #define AUDIO_PWR_CON_SC_AUDIO_PWR_ACK_2ND                 Fld(1, 31) //[31:31]
    #define AUDIO_PWR_CON_SC_AUDIO_PWR_ACK                     Fld(1, 30) //[30:30]
    #define AUDIO_PWR_CON_SC_AUDIO_SRAM_PDN_ACK                Fld(1, 12) //[12:12]
    #define AUDIO_PWR_CON_AUDIO_SRAM_PDN                       Fld(1, 8) //[8:8]
    #define AUDIO_PWR_CON_AUDIO_PWR_CLK_DIS                    Fld(1, 4) //[4:4]
    #define AUDIO_PWR_CON_AUDIO_PWR_ON_2ND                     Fld(1, 3) //[3:3]
    #define AUDIO_PWR_CON_AUDIO_PWR_ON                         Fld(1, 2) //[2:2]
    #define AUDIO_PWR_CON_AUDIO_PWR_ISO                        Fld(1, 1) //[1:1]
    #define AUDIO_PWR_CON_AUDIO_PWR_RST_B                      Fld(1, 0) //[0:0]

#define SPM_REG_ADSP_TOP_PWR_CON                               (SPM_BASE_ADDRESS + 0x00E18)
    #define ADSP_TOP_PWR_CON_SC_ADSP_TOP_PWR_ACK_2ND           Fld(1, 31) //[31:31]
    #define ADSP_TOP_PWR_CON_SC_ADSP_TOP_PWR_ACK               Fld(1, 30) //[30:30]
    #define ADSP_TOP_PWR_CON_SC_ADSP_TOP_SRAM_SLEEP_B_ACK      Fld(1, 13) //[13:13]
    #define ADSP_TOP_PWR_CON_SC_ADSP_TOP_SRAM_PDN_ACK          Fld(1, 12) //[12:12]
    #define ADSP_TOP_PWR_CON_ADSP_TOP_SRAM_SLEEP_B             Fld(1, 9) //[9:9]
    #define ADSP_TOP_PWR_CON_ADSP_TOP_SRAM_PDN                 Fld(1, 8) //[8:8]
    #define ADSP_TOP_PWR_CON_ADSP_TOP_SRAM_ISOINT_B            Fld(1, 6) //[6:6]
    #define ADSP_TOP_PWR_CON_ADSP_TOP_SRAM_CKISO               Fld(1, 5) //[5:5]
    #define ADSP_TOP_PWR_CON_ADSP_TOP_PWR_CLK_DIS              Fld(1, 4) //[4:4]
    #define ADSP_TOP_PWR_CON_ADSP_TOP_PWR_ON_2ND               Fld(1, 3) //[3:3]
    #define ADSP_TOP_PWR_CON_ADSP_TOP_PWR_ON                   Fld(1, 2) //[2:2]
    #define ADSP_TOP_PWR_CON_ADSP_TOP_PWR_ISO                  Fld(1, 1) //[1:1]
    #define ADSP_TOP_PWR_CON_ADSP_TOP_PWR_RST_B                Fld(1, 0) //[0:0]

#define SPM_REG_ADSP_INFRA_PWR_CON                             (SPM_BASE_ADDRESS + 0x00E1C)
    #define ADSP_INFRA_PWR_CON_SC_ADSP_INFRA_PWR_ACK_2ND       Fld(1, 31) //[31:31]
    #define ADSP_INFRA_PWR_CON_SC_ADSP_INFRA_PWR_ACK           Fld(1, 30) //[30:30]
    #define ADSP_INFRA_PWR_CON_ADSP_INFRA_PWR_CLK_DIS          Fld(1, 4) //[4:4]
    #define ADSP_INFRA_PWR_CON_ADSP_INFRA_PWR_ON_2ND           Fld(1, 3) //[3:3]
    #define ADSP_INFRA_PWR_CON_ADSP_INFRA_PWR_ON               Fld(1, 2) //[2:2]
    #define ADSP_INFRA_PWR_CON_ADSP_INFRA_PWR_ISO              Fld(1, 1) //[1:1]
    #define ADSP_INFRA_PWR_CON_ADSP_INFRA_PWR_RST_B            Fld(1, 0) //[0:0]

#define SPM_REG_ADSP_AO_PWR_CON                                (SPM_BASE_ADDRESS + 0x00E20)
    #define ADSP_AO_PWR_CON_SC_ADSP_AO_PWR_ACK_2ND             Fld(1, 31) //[31:31]
    #define ADSP_AO_PWR_CON_SC_ADSP_AO_PWR_ACK                 Fld(1, 30) //[30:30]
    #define ADSP_AO_PWR_CON_ADSP_AO_PWR_CLK_DIS                Fld(1, 4) //[4:4]
    #define ADSP_AO_PWR_CON_ADSP_AO_PWR_ON_2ND                 Fld(1, 3) //[3:3]
    #define ADSP_AO_PWR_CON_ADSP_AO_PWR_ON                     Fld(1, 2) //[2:2]
    #define ADSP_AO_PWR_CON_ADSP_AO_PWR_ISO                    Fld(1, 1) //[1:1]
    #define ADSP_AO_PWR_CON_ADSP_AO_PWR_RST_B                  Fld(1, 0) //[0:0]

#define SPM_REG_ISP_MAIN_PWR_CON                               (SPM_BASE_ADDRESS + 0x00E24)
    #define ISP_MAIN_PWR_CON_SC_ISP_MAIN_PWR_ACK_2ND           Fld(1, 31) //[31:31]
    #define ISP_MAIN_PWR_CON_SC_ISP_MAIN_PWR_ACK               Fld(1, 30) //[30:30]
    #define ISP_MAIN_PWR_CON_SC_ISP_MAIN_SRAM_PDN_ACK          Fld(1, 12) //[12:12]
    #define ISP_MAIN_PWR_CON_ISP_MAIN_SRAM_PDN                 Fld(1, 8) //[8:8]
    #define ISP_MAIN_PWR_CON_ISP_MAIN_PWR_CLK_DIS              Fld(1, 4) //[4:4]
    #define ISP_MAIN_PWR_CON_ISP_MAIN_PWR_ON_2ND               Fld(1, 3) //[3:3]
    #define ISP_MAIN_PWR_CON_ISP_MAIN_PWR_ON                   Fld(1, 2) //[2:2]
    #define ISP_MAIN_PWR_CON_ISP_MAIN_PWR_ISO                  Fld(1, 1) //[1:1]
    #define ISP_MAIN_PWR_CON_ISP_MAIN_PWR_RST_B                Fld(1, 0) //[0:0]

#define SPM_REG_ISP_DIP1_PWR_CON                               (SPM_BASE_ADDRESS + 0x00E28)
    #define ISP_DIP1_PWR_CON_SC_ISP_DIP1_PWR_ACK_2ND           Fld(1, 31) //[31:31]
    #define ISP_DIP1_PWR_CON_SC_ISP_DIP1_PWR_ACK               Fld(1, 30) //[30:30]
    #define ISP_DIP1_PWR_CON_SC_ISP_DIP1_SRAM_PDN_ACK          Fld(1, 12) //[12:12]
    #define ISP_DIP1_PWR_CON_ISP_DIP1_SRAM_PDN                 Fld(1, 8) //[8:8]
    #define ISP_DIP1_PWR_CON_ISP_DIP1_PWR_CLK_DIS              Fld(1, 4) //[4:4]
    #define ISP_DIP1_PWR_CON_ISP_DIP1_PWR_ON_2ND               Fld(1, 3) //[3:3]
    #define ISP_DIP1_PWR_CON_ISP_DIP1_PWR_ON                   Fld(1, 2) //[2:2]
    #define ISP_DIP1_PWR_CON_ISP_DIP1_PWR_ISO                  Fld(1, 1) //[1:1]
    #define ISP_DIP1_PWR_CON_ISP_DIP1_PWR_RST_B                Fld(1, 0) //[0:0]

#define SPM_REG_ISP_IPE_PWR_CON                                (SPM_BASE_ADDRESS + 0x00E2C)
    #define ISP_IPE_PWR_CON_SC_ISP_IPE_PWR_ACK_2ND             Fld(1, 31) //[31:31]
    #define ISP_IPE_PWR_CON_SC_ISP_IPE_PWR_ACK                 Fld(1, 30) //[30:30]
    #define ISP_IPE_PWR_CON_SC_ISP_IPE_SRAM_PDN_ACK            Fld(1, 12) //[12:12]
    #define ISP_IPE_PWR_CON_ISP_IPE_SRAM_PDN                   Fld(1, 8) //[8:8]
    #define ISP_IPE_PWR_CON_ISP_IPE_PWR_CLK_DIS                Fld(1, 4) //[4:4]
    #define ISP_IPE_PWR_CON_ISP_IPE_PWR_ON_2ND                 Fld(1, 3) //[3:3]
    #define ISP_IPE_PWR_CON_ISP_IPE_PWR_ON                     Fld(1, 2) //[2:2]
    #define ISP_IPE_PWR_CON_ISP_IPE_PWR_ISO                    Fld(1, 1) //[1:1]
    #define ISP_IPE_PWR_CON_ISP_IPE_PWR_RST_B                  Fld(1, 0) //[0:0]

#define SPM_REG_ISP_VCORE_PWR_CON                              (SPM_BASE_ADDRESS + 0x00E30)
    #define ISP_VCORE_PWR_CON_SC_ISP_VCORE_PWR_ACK_2ND         Fld(1, 31) //[31:31]
    #define ISP_VCORE_PWR_CON_SC_ISP_VCORE_PWR_ACK             Fld(1, 30) //[30:30]
    #define ISP_VCORE_PWR_CON_ISP_VCORE_PWR_CLK_DIS            Fld(1, 4) //[4:4]
    #define ISP_VCORE_PWR_CON_ISP_VCORE_PWR_ON_2ND             Fld(1, 3) //[3:3]
    #define ISP_VCORE_PWR_CON_ISP_VCORE_PWR_ON                 Fld(1, 2) //[2:2]
    #define ISP_VCORE_PWR_CON_ISP_VCORE_PWR_ISO                Fld(1, 1) //[1:1]
    #define ISP_VCORE_PWR_CON_ISP_VCORE_PWR_RST_B              Fld(1, 0) //[0:0]

#define SPM_REG_VDE0_PWR_CON                                   (SPM_BASE_ADDRESS + 0x00E34)
    #define VDE0_PWR_CON_SC_VDE0_PWR_ACK_2ND                   Fld(1, 31) //[31:31]
    #define VDE0_PWR_CON_SC_VDE0_PWR_ACK                       Fld(1, 30) //[30:30]
    #define VDE0_PWR_CON_SC_VDE0_SRAM_PDN_ACK                  Fld(1, 12) //[12:12]
    #define VDE0_PWR_CON_VDE0_SRAM_PDN                         Fld(1, 8) //[8:8]
    #define VDE0_PWR_CON_VDE0_PWR_CLK_DIS                      Fld(1, 4) //[4:4]
    #define VDE0_PWR_CON_VDE0_PWR_ON_2ND                       Fld(1, 3) //[3:3]
    #define VDE0_PWR_CON_VDE0_PWR_ON                           Fld(1, 2) //[2:2]
    #define VDE0_PWR_CON_VDE0_PWR_ISO                          Fld(1, 1) //[1:1]
    #define VDE0_PWR_CON_VDE0_PWR_RST_B                        Fld(1, 0) //[0:0]

#define SPM_REG_VDE1_PWR_CON                                   (SPM_BASE_ADDRESS + 0x00E38)
    #define VDE1_PWR_CON_SC_VDE1_PWR_ACK_2ND                   Fld(1, 31) //[31:31]
    #define VDE1_PWR_CON_SC_VDE1_PWR_ACK                       Fld(1, 30) //[30:30]
    #define VDE1_PWR_CON_SC_VDE1_SRAM_PDN_ACK                  Fld(1, 12) //[12:12]
    #define VDE1_PWR_CON_VDE1_SRAM_PDN                         Fld(1, 8) //[8:8]
    #define VDE1_PWR_CON_VDE1_PWR_CLK_DIS                      Fld(1, 4) //[4:4]
    #define VDE1_PWR_CON_VDE1_PWR_ON_2ND                       Fld(1, 3) //[3:3]
    #define VDE1_PWR_CON_VDE1_PWR_ON                           Fld(1, 2) //[2:2]
    #define VDE1_PWR_CON_VDE1_PWR_ISO                          Fld(1, 1) //[1:1]
    #define VDE1_PWR_CON_VDE1_PWR_RST_B                        Fld(1, 0) //[0:0]

#define SPM_REG_VEN0_PWR_CON                                   (SPM_BASE_ADDRESS + 0x00E3C)
    #define VEN0_PWR_CON_SC_VEN0_PWR_ACK_2ND                   Fld(1, 31) //[31:31]
    #define VEN0_PWR_CON_SC_VEN0_PWR_ACK                       Fld(1, 30) //[30:30]
    #define VEN0_PWR_CON_SC_VEN0_SRAM_PDN_ACK                  Fld(1, 12) //[12:12]
    #define VEN0_PWR_CON_VEN0_SRAM_PDN                         Fld(1, 8) //[8:8]
    #define VEN0_PWR_CON_VEN0_PWR_CLK_DIS                      Fld(1, 4) //[4:4]
    #define VEN0_PWR_CON_VEN0_PWR_ON_2ND                       Fld(1, 3) //[3:3]
    #define VEN0_PWR_CON_VEN0_PWR_ON                           Fld(1, 2) //[2:2]
    #define VEN0_PWR_CON_VEN0_PWR_ISO                          Fld(1, 1) //[1:1]
    #define VEN0_PWR_CON_VEN0_PWR_RST_B                        Fld(1, 0) //[0:0]

#define SPM_REG_VEN1_PWR_CON                                   (SPM_BASE_ADDRESS + 0x00E40)
    #define VEN1_PWR_CON_SC_VEN1_PWR_ACK_2ND                   Fld(1, 31) //[31:31]
    #define VEN1_PWR_CON_SC_VEN1_PWR_ACK                       Fld(1, 30) //[30:30]
    #define VEN1_PWR_CON_SC_VEN1_SRAM_PDN_ACK                  Fld(1, 12) //[12:12]
    #define VEN1_PWR_CON_VEN1_SRAM_PDN                         Fld(1, 8) //[8:8]
    #define VEN1_PWR_CON_VEN1_PWR_CLK_DIS                      Fld(1, 4) //[4:4]
    #define VEN1_PWR_CON_VEN1_PWR_ON_2ND                       Fld(1, 3) //[3:3]
    #define VEN1_PWR_CON_VEN1_PWR_ON                           Fld(1, 2) //[2:2]
    #define VEN1_PWR_CON_VEN1_PWR_ISO                          Fld(1, 1) //[1:1]
    #define VEN1_PWR_CON_VEN1_PWR_RST_B                        Fld(1, 0) //[0:0]

#define SPM_REG_CAM_MAIN_PWR_CON                               (SPM_BASE_ADDRESS + 0x00E44)
    #define CAM_MAIN_PWR_CON_SC_CAM_MAIN_PWR_ACK_2ND           Fld(1, 31) //[31:31]
    #define CAM_MAIN_PWR_CON_SC_CAM_MAIN_PWR_ACK               Fld(1, 30) //[30:30]
    #define CAM_MAIN_PWR_CON_SC_CAM_MAIN_SRAM_PDN_ACK          Fld(1, 12) //[12:12]
    #define CAM_MAIN_PWR_CON_CAM_MAIN_SRAM_PDN                 Fld(1, 8) //[8:8]
    #define CAM_MAIN_PWR_CON_CAM_MAIN_PWR_CLK_DIS              Fld(1, 4) //[4:4]
    #define CAM_MAIN_PWR_CON_CAM_MAIN_PWR_ON_2ND               Fld(1, 3) //[3:3]
    #define CAM_MAIN_PWR_CON_CAM_MAIN_PWR_ON                   Fld(1, 2) //[2:2]
    #define CAM_MAIN_PWR_CON_CAM_MAIN_PWR_ISO                  Fld(1, 1) //[1:1]
    #define CAM_MAIN_PWR_CON_CAM_MAIN_PWR_RST_B                Fld(1, 0) //[0:0]

#define SPM_REG_CAM_MRAW_PWR_CON                               (SPM_BASE_ADDRESS + 0x00E48)
    #define CAM_MRAW_PWR_CON_SC_CAM_MRAW_PWR_ACK_2ND           Fld(1, 31) //[31:31]
    #define CAM_MRAW_PWR_CON_SC_CAM_MRAW_PWR_ACK               Fld(1, 30) //[30:30]
    #define CAM_MRAW_PWR_CON_SC_CAM_MRAW_SRAM_PDN_ACK          Fld(1, 12) //[12:12]
    #define CAM_MRAW_PWR_CON_CAM_MRAW_SRAM_PDN                 Fld(1, 8) //[8:8]
    #define CAM_MRAW_PWR_CON_CAM_MRAW_PWR_CLK_DIS              Fld(1, 4) //[4:4]
    #define CAM_MRAW_PWR_CON_CAM_MRAW_PWR_ON_2ND               Fld(1, 3) //[3:3]
    #define CAM_MRAW_PWR_CON_CAM_MRAW_PWR_ON                   Fld(1, 2) //[2:2]
    #define CAM_MRAW_PWR_CON_CAM_MRAW_PWR_ISO                  Fld(1, 1) //[1:1]
    #define CAM_MRAW_PWR_CON_CAM_MRAW_PWR_RST_B                Fld(1, 0) //[0:0]

#define SPM_REG_CAM_SUBA_PWR_CON                               (SPM_BASE_ADDRESS + 0x00E4C)
    #define CAM_SUBA_PWR_CON_SC_CAM_SUBA_PWR_ACK_2ND           Fld(1, 31) //[31:31]
    #define CAM_SUBA_PWR_CON_SC_CAM_SUBA_PWR_ACK               Fld(1, 30) //[30:30]
    #define CAM_SUBA_PWR_CON_SC_CAM_SUBA_SRAM_PDN_ACK          Fld(1, 12) //[12:12]
    #define CAM_SUBA_PWR_CON_CAM_SUBA_SRAM_PDN                 Fld(1, 8) //[8:8]
    #define CAM_SUBA_PWR_CON_CAM_SUBA_PWR_CLK_DIS              Fld(1, 4) //[4:4]
    #define CAM_SUBA_PWR_CON_CAM_SUBA_PWR_ON_2ND               Fld(1, 3) //[3:3]
    #define CAM_SUBA_PWR_CON_CAM_SUBA_PWR_ON                   Fld(1, 2) //[2:2]
    #define CAM_SUBA_PWR_CON_CAM_SUBA_PWR_ISO                  Fld(1, 1) //[1:1]
    #define CAM_SUBA_PWR_CON_CAM_SUBA_PWR_RST_B                Fld(1, 0) //[0:0]

#define SPM_REG_CAM_SUBB_PWR_CON                               (SPM_BASE_ADDRESS + 0x00E50)
    #define CAM_SUBB_PWR_CON_SC_CAM_SUBB_PWR_ACK_2ND           Fld(1, 31) //[31:31]
    #define CAM_SUBB_PWR_CON_SC_CAM_SUBB_PWR_ACK               Fld(1, 30) //[30:30]
    #define CAM_SUBB_PWR_CON_SC_CAM_SUBB_SRAM_PDN_ACK          Fld(1, 12) //[12:12]
    #define CAM_SUBB_PWR_CON_CAM_SUBB_SRAM_PDN                 Fld(1, 8) //[8:8]
    #define CAM_SUBB_PWR_CON_CAM_SUBB_PWR_CLK_DIS              Fld(1, 4) //[4:4]
    #define CAM_SUBB_PWR_CON_CAM_SUBB_PWR_ON_2ND               Fld(1, 3) //[3:3]
    #define CAM_SUBB_PWR_CON_CAM_SUBB_PWR_ON                   Fld(1, 2) //[2:2]
    #define CAM_SUBB_PWR_CON_CAM_SUBB_PWR_ISO                  Fld(1, 1) //[1:1]
    #define CAM_SUBB_PWR_CON_CAM_SUBB_PWR_RST_B                Fld(1, 0) //[0:0]

#define SPM_REG_CAM_SUBC_PWR_CON                               (SPM_BASE_ADDRESS + 0x00E54)
    #define CAM_SUBC_PWR_CON_SC_CAM_SUBC_PWR_ACK_2ND           Fld(1, 31) //[31:31]
    #define CAM_SUBC_PWR_CON_SC_CAM_SUBC_PWR_ACK               Fld(1, 30) //[30:30]
    #define CAM_SUBC_PWR_CON_SC_CAM_SUBC_SRAM_PDN_ACK          Fld(1, 12) //[12:12]
    #define CAM_SUBC_PWR_CON_CAM_SUBC_SRAM_PDN                 Fld(1, 8) //[8:8]
    #define CAM_SUBC_PWR_CON_CAM_SUBC_PWR_CLK_DIS              Fld(1, 4) //[4:4]
    #define CAM_SUBC_PWR_CON_CAM_SUBC_PWR_ON_2ND               Fld(1, 3) //[3:3]
    #define CAM_SUBC_PWR_CON_CAM_SUBC_PWR_ON                   Fld(1, 2) //[2:2]
    #define CAM_SUBC_PWR_CON_CAM_SUBC_PWR_ISO                  Fld(1, 1) //[1:1]
    #define CAM_SUBC_PWR_CON_CAM_SUBC_PWR_RST_B                Fld(1, 0) //[0:0]

#define SPM_REG_CAM_VCORE_PWR_CON                              (SPM_BASE_ADDRESS + 0x00E58)
    #define CAM_VCORE_PWR_CON_SC_CAM_VCORE_PWR_ACK_2ND         Fld(1, 31) //[31:31]
    #define CAM_VCORE_PWR_CON_SC_CAM_VCORE_PWR_ACK             Fld(1, 30) //[30:30]
    #define CAM_VCORE_PWR_CON_CAM_VCORE_PWR_CLK_DIS            Fld(1, 4) //[4:4]
    #define CAM_VCORE_PWR_CON_CAM_VCORE_PWR_ON_2ND             Fld(1, 3) //[3:3]
    #define CAM_VCORE_PWR_CON_CAM_VCORE_PWR_ON                 Fld(1, 2) //[2:2]
    #define CAM_VCORE_PWR_CON_CAM_VCORE_PWR_ISO                Fld(1, 1) //[1:1]
    #define CAM_VCORE_PWR_CON_CAM_VCORE_PWR_RST_B              Fld(1, 0) //[0:0]

#define SPM_REG_MDP0_PWR_CON                                   (SPM_BASE_ADDRESS + 0x00E5C)
    #define MDP0_PWR_CON_SC_MDP0_PWR_ACK_2ND                   Fld(1, 31) //[31:31]
    #define MDP0_PWR_CON_SC_MDP0_PWR_ACK                       Fld(1, 30) //[30:30]
    #define MDP0_PWR_CON_SC_MDP0_SRAM_PDN_ACK                  Fld(1, 12) //[12:12]
    #define MDP0_PWR_CON_MDP0_SRAM_PDN                         Fld(1, 8) //[8:8]
    #define MDP0_PWR_CON_MDP0_PWR_CLK_DIS                      Fld(1, 4) //[4:4]
    #define MDP0_PWR_CON_MDP0_PWR_ON_2ND                       Fld(1, 3) //[3:3]
    #define MDP0_PWR_CON_MDP0_PWR_ON                           Fld(1, 2) //[2:2]
    #define MDP0_PWR_CON_MDP0_PWR_ISO                          Fld(1, 1) //[1:1]
    #define MDP0_PWR_CON_MDP0_PWR_RST_B                        Fld(1, 0) //[0:0]

#define SPM_REG_MDP1_PWR_CON                                   (SPM_BASE_ADDRESS + 0x00E60)
    #define MDP1_PWR_CON_SC_MDP1_PWR_ACK_2ND                   Fld(1, 31) //[31:31]
    #define MDP1_PWR_CON_SC_MDP1_PWR_ACK                       Fld(1, 30) //[30:30]
    #define MDP1_PWR_CON_SC_MDP1_SRAM_PDN_ACK                  Fld(1, 12) //[12:12]
    #define MDP1_PWR_CON_MDP1_SRAM_PDN                         Fld(1, 8) //[8:8]
    #define MDP1_PWR_CON_MDP1_PWR_CLK_DIS                      Fld(1, 4) //[4:4]
    #define MDP1_PWR_CON_MDP1_PWR_ON_2ND                       Fld(1, 3) //[3:3]
    #define MDP1_PWR_CON_MDP1_PWR_ON                           Fld(1, 2) //[2:2]
    #define MDP1_PWR_CON_MDP1_PWR_ISO                          Fld(1, 1) //[1:1]
    #define MDP1_PWR_CON_MDP1_PWR_RST_B                        Fld(1, 0) //[0:0]

#define SPM_REG_DIS0_PWR_CON                                   (SPM_BASE_ADDRESS + 0x00E64)
    #define DIS0_PWR_CON_SC_DIS0_PWR_ACK_2ND                   Fld(1, 31) //[31:31]
    #define DIS0_PWR_CON_SC_DIS0_PWR_ACK                       Fld(1, 30) //[30:30]
    #define DIS0_PWR_CON_SC_DIS0_SRAM_PDN_ACK                  Fld(1, 12) //[12:12]
    #define DIS0_PWR_CON_DIS0_SRAM_PDN                         Fld(1, 8) //[8:8]
    #define DIS0_PWR_CON_DIS0_PWR_CLK_DIS                      Fld(1, 4) //[4:4]
    #define DIS0_PWR_CON_DIS0_PWR_ON_2ND                       Fld(1, 3) //[3:3]
    #define DIS0_PWR_CON_DIS0_PWR_ON                           Fld(1, 2) //[2:2]
    #define DIS0_PWR_CON_DIS0_PWR_ISO                          Fld(1, 1) //[1:1]
    #define DIS0_PWR_CON_DIS0_PWR_RST_B                        Fld(1, 0) //[0:0]

#define SPM_REG_DIS1_PWR_CON                                   (SPM_BASE_ADDRESS + 0x00E68)
    #define DIS1_PWR_CON_SC_DIS1_PWR_ACK_2ND                   Fld(1, 31) //[31:31]
    #define DIS1_PWR_CON_SC_DIS1_PWR_ACK                       Fld(1, 30) //[30:30]
    #define DIS1_PWR_CON_SC_DIS1_SRAM_PDN_ACK                  Fld(1, 12) //[12:12]
    #define DIS1_PWR_CON_DIS1_SRAM_PDN                         Fld(1, 8) //[8:8]
    #define DIS1_PWR_CON_DIS1_PWR_CLK_DIS                      Fld(1, 4) //[4:4]
    #define DIS1_PWR_CON_DIS1_PWR_ON_2ND                       Fld(1, 3) //[3:3]
    #define DIS1_PWR_CON_DIS1_PWR_ON                           Fld(1, 2) //[2:2]
    #define DIS1_PWR_CON_DIS1_PWR_ISO                          Fld(1, 1) //[1:1]
    #define DIS1_PWR_CON_DIS1_PWR_RST_B                        Fld(1, 0) //[0:0]

#define SPM_REG_MM_INFRA_PWR_CON                               (SPM_BASE_ADDRESS + 0x00E6C)
    #define MM_INFRA_PWR_CON_SC_MM_INFRA_PWR_ACK_2ND           Fld(1, 31) //[31:31]
    #define MM_INFRA_PWR_CON_SC_MM_INFRA_PWR_ACK               Fld(1, 30) //[30:30]
    #define MM_INFRA_PWR_CON_SC_MM_INFRA_SRAM_PDN_ACK          Fld(1, 12) //[12:12]
    #define MM_INFRA_PWR_CON_MM_INFRA_SRAM_PDN                 Fld(1, 8) //[8:8]
    #define MM_INFRA_PWR_CON_MM_INFRA_PWR_CLK_DIS              Fld(1, 4) //[4:4]
    #define MM_INFRA_PWR_CON_MM_INFRA_PWR_ON_2ND               Fld(1, 3) //[3:3]
    #define MM_INFRA_PWR_CON_MM_INFRA_PWR_ON                   Fld(1, 2) //[2:2]
    #define MM_INFRA_PWR_CON_MM_INFRA_PWR_ISO                  Fld(1, 1) //[1:1]
    #define MM_INFRA_PWR_CON_MM_INFRA_PWR_RST_B                Fld(1, 0) //[0:0]

#define SPM_REG_MM_PROC_PWR_CON                                (SPM_BASE_ADDRESS + 0x00E70)
    #define MM_PROC_PWR_CON_SC_MM_PROC_PWR_ACK_2ND             Fld(1, 31) //[31:31]
    #define MM_PROC_PWR_CON_SC_MM_PROC_PWR_ACK                 Fld(1, 30) //[30:30]
    #define MM_PROC_PWR_CON_SC_MM_PROC_SRAM_SLEEP_B_ACK        Fld(1, 13) //[13:13]
    #define MM_PROC_PWR_CON_SC_MM_PROC_SRAM_PDN_ACK            Fld(1, 12) //[12:12]
    #define MM_PROC_PWR_CON_MM_PROC_SRAM_SLEEP_B               Fld(1, 9) //[9:9]
    #define MM_PROC_PWR_CON_MM_PROC_SRAM_PDN                   Fld(1, 8) //[8:8]
    #define MM_PROC_PWR_CON_MM_PROC_SRAM_ISOINT_B              Fld(1, 6) //[6:6]
    #define MM_PROC_PWR_CON_MM_PROC_SRAM_CKISO                 Fld(1, 5) //[5:5]
    #define MM_PROC_PWR_CON_MM_PROC_PWR_CLK_DIS                Fld(1, 4) //[4:4]
    #define MM_PROC_PWR_CON_MM_PROC_PWR_ON_2ND                 Fld(1, 3) //[3:3]
    #define MM_PROC_PWR_CON_MM_PROC_PWR_ON                     Fld(1, 2) //[2:2]
    #define MM_PROC_PWR_CON_MM_PROC_PWR_ISO                    Fld(1, 1) //[1:1]
    #define MM_PROC_PWR_CON_MM_PROC_PWR_RST_B                  Fld(1, 0) //[0:0]

#define SPM_REG_DP_TX_PWR_CON                                  (SPM_BASE_ADDRESS + 0x00E74)
    #define DP_TX_PWR_CON_SC_DP_TX_PWR_ACK_2ND                 Fld(1, 31) //[31:31]
    #define DP_TX_PWR_CON_SC_DP_TX_PWR_ACK                     Fld(1, 30) //[30:30]
    #define DP_TX_PWR_CON_SC_DP_TX_SRAM_PDN_ACK                Fld(1, 12) //[12:12]
    #define DP_TX_PWR_CON_DP_TX_SRAM_PDN                       Fld(1, 8) //[8:8]
    #define DP_TX_PWR_CON_DP_TX_PWR_CLK_DIS                    Fld(1, 4) //[4:4]
    #define DP_TX_PWR_CON_DP_TX_PWR_ON_2ND                     Fld(1, 3) //[3:3]
    #define DP_TX_PWR_CON_DP_TX_PWR_ON                         Fld(1, 2) //[2:2]
    #define DP_TX_PWR_CON_DP_TX_PWR_ISO                        Fld(1, 1) //[1:1]
    #define DP_TX_PWR_CON_DP_TX_PWR_RST_B                      Fld(1, 0) //[0:0]

#define SPM_REG_SCP_PWR_CON                                    (SPM_BASE_ADDRESS + 0x00E78)
    #define SCP_PWR_CON_SC_SCP_PWR_ACK_2ND                     Fld(1, 31) //[31:31]
    #define SCP_PWR_CON_SC_SCP_PWR_ACK                         Fld(1, 30) //[30:30]
    #define SCP_PWR_CON_SC_SCP_SRAM_SLEEP_B_ACK                Fld(1, 13) //[13:13]
    #define SCP_PWR_CON_SC_SCP_SRAM_PDN_ACK                    Fld(1, 12) //[12:12]
    #define SCP_PWR_CON_SCP_SRAM_SLEEP_B                       Fld(1, 9) //[9:9]
    #define SCP_PWR_CON_SCP_SRAM_PDN                           Fld(1, 8) //[8:8]
    #define SCP_PWR_CON_SCP_SRAM_ISOINT_B                      Fld(1, 6) //[6:6]
    #define SCP_PWR_CON_SCP_SRAM_CKISO                         Fld(1, 5) //[5:5]
    #define SCP_PWR_CON_SCP_PWR_CLK_DIS                        Fld(1, 4) //[4:4]
    #define SCP_PWR_CON_SCP_PWR_ON_2ND                         Fld(1, 3) //[3:3]
    #define SCP_PWR_CON_SCP_PWR_ON                             Fld(1, 2) //[2:2]
    #define SCP_PWR_CON_SCP_PWR_ISO                            Fld(1, 1) //[1:1]
    #define SCP_PWR_CON_SCP_PWR_RST_B                          Fld(1, 0) //[0:0]

#define SPM_REG_DPYD0_PWR_CON                                  (SPM_BASE_ADDRESS + 0x00E7C)
    #define DPYD0_PWR_CON_SC_DPYD0_PWR_ACK_2ND                 Fld(1, 31) //[31:31]
    #define DPYD0_PWR_CON_SC_DPYD0_PWR_ACK                     Fld(1, 30) //[30:30]
    #define DPYD0_PWR_CON_SC_DPYD0_SRAM_SLEEP_B_ACK            Fld(1, 13) //[13:13]
    #define DPYD0_PWR_CON_SC_DPYD0_SRAM_PDN_ACK                Fld(1, 12) //[12:12]
    #define DPYD0_PWR_CON_DPYD0_SRAM_SLEEP_B                   Fld(1, 9) //[9:9]
    #define DPYD0_PWR_CON_DPYD0_SRAM_PDN                       Fld(1, 8) //[8:8]
    #define DPYD0_PWR_CON_DPYD0_SRAM_ISOINT_B                  Fld(1, 6) //[6:6]
    #define DPYD0_PWR_CON_DPYD0_SRAM_CKISO                     Fld(1, 5) //[5:5]
    #define DPYD0_PWR_CON_DPYD0_PWR_CLK_DIS                    Fld(1, 4) //[4:4]
    #define DPYD0_PWR_CON_DPYD0_PWR_ON_2ND                     Fld(1, 3) //[3:3]
    #define DPYD0_PWR_CON_DPYD0_PWR_ON                         Fld(1, 2) //[2:2]
    #define DPYD0_PWR_CON_DPYD0_PWR_ISO                        Fld(1, 1) //[1:1]
    #define DPYD0_PWR_CON_DPYD0_PWR_RST_B                      Fld(1, 0) //[0:0]

#define SPM_REG_DPYD1_PWR_CON                                  (SPM_BASE_ADDRESS + 0x00E80)
    #define DPYD1_PWR_CON_SC_DPYD1_PWR_ACK_2ND                 Fld(1, 31) //[31:31]
    #define DPYD1_PWR_CON_SC_DPYD1_PWR_ACK                     Fld(1, 30) //[30:30]
    #define DPYD1_PWR_CON_SC_DPYD1_SRAM_SLEEP_B_ACK            Fld(1, 13) //[13:13]
    #define DPYD1_PWR_CON_SC_DPYD1_SRAM_PDN_ACK                Fld(1, 12) //[12:12]
    #define DPYD1_PWR_CON_DPYD1_SRAM_SLEEP_B                   Fld(1, 9) //[9:9]
    #define DPYD1_PWR_CON_DPYD1_SRAM_PDN                       Fld(1, 8) //[8:8]
    #define DPYD1_PWR_CON_DPYD1_SRAM_ISOINT_B                  Fld(1, 6) //[6:6]
    #define DPYD1_PWR_CON_DPYD1_SRAM_CKISO                     Fld(1, 5) //[5:5]
    #define DPYD1_PWR_CON_DPYD1_PWR_CLK_DIS                    Fld(1, 4) //[4:4]
    #define DPYD1_PWR_CON_DPYD1_PWR_ON_2ND                     Fld(1, 3) //[3:3]
    #define DPYD1_PWR_CON_DPYD1_PWR_ON                         Fld(1, 2) //[2:2]
    #define DPYD1_PWR_CON_DPYD1_PWR_ISO                        Fld(1, 1) //[1:1]
    #define DPYD1_PWR_CON_DPYD1_PWR_RST_B                      Fld(1, 0) //[0:0]

#define SPM_REG_DPYD2_PWR_CON                                  (SPM_BASE_ADDRESS + 0x00E84)
    #define DPYD2_PWR_CON_SC_DPYD2_PWR_ACK_2ND                 Fld(1, 31) //[31:31]
    #define DPYD2_PWR_CON_SC_DPYD2_PWR_ACK                     Fld(1, 30) //[30:30]
    #define DPYD2_PWR_CON_SC_DPYD2_SRAM_SLEEP_B_ACK            Fld(1, 13) //[13:13]
    #define DPYD2_PWR_CON_SC_DPYD2_SRAM_PDN_ACK                Fld(1, 12) //[12:12]
    #define DPYD2_PWR_CON_DPYD2_SRAM_SLEEP_B                   Fld(1, 9) //[9:9]
    #define DPYD2_PWR_CON_DPYD2_SRAM_PDN                       Fld(1, 8) //[8:8]
    #define DPYD2_PWR_CON_DPYD2_SRAM_ISOINT_B                  Fld(1, 6) //[6:6]
    #define DPYD2_PWR_CON_DPYD2_SRAM_CKISO                     Fld(1, 5) //[5:5]
    #define DPYD2_PWR_CON_DPYD2_PWR_CLK_DIS                    Fld(1, 4) //[4:4]
    #define DPYD2_PWR_CON_DPYD2_PWR_ON_2ND                     Fld(1, 3) //[3:3]
    #define DPYD2_PWR_CON_DPYD2_PWR_ON                         Fld(1, 2) //[2:2]
    #define DPYD2_PWR_CON_DPYD2_PWR_ISO                        Fld(1, 1) //[1:1]
    #define DPYD2_PWR_CON_DPYD2_PWR_RST_B                      Fld(1, 0) //[0:0]

#define SPM_REG_DPYD3_PWR_CON                                  (SPM_BASE_ADDRESS + 0x00E88)
    #define DPYD3_PWR_CON_SC_DPYD3_PWR_ACK_2ND                 Fld(1, 31) //[31:31]
    #define DPYD3_PWR_CON_SC_DPYD3_PWR_ACK                     Fld(1, 30) //[30:30]
    #define DPYD3_PWR_CON_SC_DPYD3_SRAM_SLEEP_B_ACK            Fld(1, 13) //[13:13]
    #define DPYD3_PWR_CON_SC_DPYD3_SRAM_PDN_ACK                Fld(1, 12) //[12:12]
    #define DPYD3_PWR_CON_DPYD3_SRAM_SLEEP_B                   Fld(1, 9) //[9:9]
    #define DPYD3_PWR_CON_DPYD3_SRAM_PDN                       Fld(1, 8) //[8:8]
    #define DPYD3_PWR_CON_DPYD3_SRAM_ISOINT_B                  Fld(1, 6) //[6:6]
    #define DPYD3_PWR_CON_DPYD3_SRAM_CKISO                     Fld(1, 5) //[5:5]
    #define DPYD3_PWR_CON_DPYD3_PWR_CLK_DIS                    Fld(1, 4) //[4:4]
    #define DPYD3_PWR_CON_DPYD3_PWR_ON_2ND                     Fld(1, 3) //[3:3]
    #define DPYD3_PWR_CON_DPYD3_PWR_ON                         Fld(1, 2) //[2:2]
    #define DPYD3_PWR_CON_DPYD3_PWR_ISO                        Fld(1, 1) //[1:1]
    #define DPYD3_PWR_CON_DPYD3_PWR_RST_B                      Fld(1, 0) //[0:0]

#define SPM_REG_DPYA0_PWR_CON                                  (SPM_BASE_ADDRESS + 0x00E8C)
    #define DPYA0_PWR_CON_SC_DPYA0_PWR_ACK_2ND                 Fld(1, 31) //[31:31]
    #define DPYA0_PWR_CON_SC_DPYA0_PWR_ACK                     Fld(1, 30) //[30:30]
    #define DPYA0_PWR_CON_DPYA0_PWR_CLK_DIS                    Fld(1, 4) //[4:4]
    #define DPYA0_PWR_CON_DPYA0_PWR_ON_2ND                     Fld(1, 3) //[3:3]
    #define DPYA0_PWR_CON_DPYA0_PWR_ON                         Fld(1, 2) //[2:2]
    #define DPYA0_PWR_CON_DPYA0_PWR_ISO                        Fld(1, 1) //[1:1]
    #define DPYA0_PWR_CON_DPYA0_PWR_RST_B                      Fld(1, 0) //[0:0]

#define SPM_REG_DPYA1_PWR_CON                                  (SPM_BASE_ADDRESS + 0x00E90)
    #define DPYA1_PWR_CON_SC_DPYA1_PWR_ACK_2ND                 Fld(1, 31) //[31:31]
    #define DPYA1_PWR_CON_SC_DPYA1_PWR_ACK                     Fld(1, 30) //[30:30]
    #define DPYA1_PWR_CON_DPYA1_PWR_CLK_DIS                    Fld(1, 4) //[4:4]
    #define DPYA1_PWR_CON_DPYA1_PWR_ON_2ND                     Fld(1, 3) //[3:3]
    #define DPYA1_PWR_CON_DPYA1_PWR_ON                         Fld(1, 2) //[2:2]
    #define DPYA1_PWR_CON_DPYA1_PWR_ISO                        Fld(1, 1) //[1:1]
    #define DPYA1_PWR_CON_DPYA1_PWR_RST_B                      Fld(1, 0) //[0:0]

#define SPM_REG_DPYA2_PWR_CON                                  (SPM_BASE_ADDRESS + 0x00E94)
    #define DPYA2_PWR_CON_SC_DPYA2_PWR_ACK_2ND                 Fld(1, 31) //[31:31]
    #define DPYA2_PWR_CON_SC_DPYA2_PWR_ACK                     Fld(1, 30) //[30:30]
    #define DPYA2_PWR_CON_DPYA2_PWR_CLK_DIS                    Fld(1, 4) //[4:4]
    #define DPYA2_PWR_CON_DPYA2_PWR_ON_2ND                     Fld(1, 3) //[3:3]
    #define DPYA2_PWR_CON_DPYA2_PWR_ON                         Fld(1, 2) //[2:2]
    #define DPYA2_PWR_CON_DPYA2_PWR_ISO                        Fld(1, 1) //[1:1]
    #define DPYA2_PWR_CON_DPYA2_PWR_RST_B                      Fld(1, 0) //[0:0]

#define SPM_REG_DPYA3_PWR_CON                                  (SPM_BASE_ADDRESS + 0x00E98)
    #define DPYA3_PWR_CON_SC_DPYA3_PWR_ACK_2ND                 Fld(1, 31) //[31:31]
    #define DPYA3_PWR_CON_SC_DPYA3_PWR_ACK                     Fld(1, 30) //[30:30]
    #define DPYA3_PWR_CON_DPYA3_PWR_CLK_DIS                    Fld(1, 4) //[4:4]
    #define DPYA3_PWR_CON_DPYA3_PWR_ON_2ND                     Fld(1, 3) //[3:3]
    #define DPYA3_PWR_CON_DPYA3_PWR_ON                         Fld(1, 2) //[2:2]
    #define DPYA3_PWR_CON_DPYA3_PWR_ISO                        Fld(1, 1) //[1:1]
    #define DPYA3_PWR_CON_DPYA3_PWR_RST_B                      Fld(1, 0) //[0:0]

#define SPM_REG_DPM0_PWR_CON                                   (SPM_BASE_ADDRESS + 0x00E9C)
    #define DPM0_PWR_CON_SC_DPM0_PWR_ACK_2ND                   Fld(1, 31) //[31:31]
    #define DPM0_PWR_CON_SC_DPM0_PWR_ACK                       Fld(1, 30) //[30:30]
    #define DPM0_PWR_CON_SC_DPM0_SRAM_SLEEP_B_ACK              Fld(1, 13) //[13:13]
    #define DPM0_PWR_CON_SC_DPM0_SRAM_PDN_ACK                  Fld(1, 12) //[12:12]
    #define DPM0_PWR_CON_DPM0_SRAM_SLEEP_B                     Fld(1, 9) //[9:9]
    #define DPM0_PWR_CON_DPM0_SRAM_PDN                         Fld(1, 8) //[8:8]
    #define DPM0_PWR_CON_DPM0_SRAM_ISOINT_B                    Fld(1, 6) //[6:6]
    #define DPM0_PWR_CON_DPM0_SRAM_CKISO                       Fld(1, 5) //[5:5]
    #define DPM0_PWR_CON_DPM0_PWR_CLK_DIS                      Fld(1, 4) //[4:4]
    #define DPM0_PWR_CON_DPM0_PWR_ON_2ND                       Fld(1, 3) //[3:3]
    #define DPM0_PWR_CON_DPM0_PWR_ON                           Fld(1, 2) //[2:2]
    #define DPM0_PWR_CON_DPM0_PWR_ISO                          Fld(1, 1) //[1:1]
    #define DPM0_PWR_CON_DPM0_PWR_RST_B                        Fld(1, 0) //[0:0]

#define SPM_REG_DPM1_PWR_CON                                   (SPM_BASE_ADDRESS + 0x00EA0)
    #define DPM1_PWR_CON_SC_DPM1_PWR_ACK_2ND                   Fld(1, 31) //[31:31]
    #define DPM1_PWR_CON_SC_DPM1_PWR_ACK                       Fld(1, 30) //[30:30]
    #define DPM1_PWR_CON_SC_DPM1_SRAM_SLEEP_B_ACK              Fld(1, 13) //[13:13]
    #define DPM1_PWR_CON_SC_DPM1_SRAM_PDN_ACK                  Fld(1, 12) //[12:12]
    #define DPM1_PWR_CON_DPM1_SRAM_SLEEP_B                     Fld(1, 9) //[9:9]
    #define DPM1_PWR_CON_DPM1_SRAM_PDN                         Fld(1, 8) //[8:8]
    #define DPM1_PWR_CON_DPM1_SRAM_ISOINT_B                    Fld(1, 6) //[6:6]
    #define DPM1_PWR_CON_DPM1_SRAM_CKISO                       Fld(1, 5) //[5:5]
    #define DPM1_PWR_CON_DPM1_PWR_CLK_DIS                      Fld(1, 4) //[4:4]
    #define DPM1_PWR_CON_DPM1_PWR_ON_2ND                       Fld(1, 3) //[3:3]
    #define DPM1_PWR_CON_DPM1_PWR_ON                           Fld(1, 2) //[2:2]
    #define DPM1_PWR_CON_DPM1_PWR_ISO                          Fld(1, 1) //[1:1]
    #define DPM1_PWR_CON_DPM1_PWR_RST_B                        Fld(1, 0) //[0:0]

#define SPM_REG_DPM2_PWR_CON                                   (SPM_BASE_ADDRESS + 0x00EA4)
    #define DPM2_PWR_CON_SC_DPM2_PWR_ACK_2ND                   Fld(1, 31) //[31:31]
    #define DPM2_PWR_CON_SC_DPM2_PWR_ACK                       Fld(1, 30) //[30:30]
    #define DPM2_PWR_CON_SC_DPM2_SRAM_SLEEP_B_ACK              Fld(1, 13) //[13:13]
    #define DPM2_PWR_CON_SC_DPM2_SRAM_PDN_ACK                  Fld(1, 12) //[12:12]
    #define DPM2_PWR_CON_DPM2_SRAM_SLEEP_B                     Fld(1, 9) //[9:9]
    #define DPM2_PWR_CON_DPM2_SRAM_PDN                         Fld(1, 8) //[8:8]
    #define DPM2_PWR_CON_DPM2_SRAM_ISOINT_B                    Fld(1, 6) //[6:6]
    #define DPM2_PWR_CON_DPM2_SRAM_CKISO                       Fld(1, 5) //[5:5]
    #define DPM2_PWR_CON_DPM2_PWR_CLK_DIS                      Fld(1, 4) //[4:4]
    #define DPM2_PWR_CON_DPM2_PWR_ON_2ND                       Fld(1, 3) //[3:3]
    #define DPM2_PWR_CON_DPM2_PWR_ON                           Fld(1, 2) //[2:2]
    #define DPM2_PWR_CON_DPM2_PWR_ISO                          Fld(1, 1) //[1:1]
    #define DPM2_PWR_CON_DPM2_PWR_RST_B                        Fld(1, 0) //[0:0]

#define SPM_REG_DPM3_PWR_CON                                   (SPM_BASE_ADDRESS + 0x00EA8)
    #define DPM3_PWR_CON_SC_DPM3_PWR_ACK_2ND                   Fld(1, 31) //[31:31]
    #define DPM3_PWR_CON_SC_DPM3_PWR_ACK                       Fld(1, 30) //[30:30]
    #define DPM3_PWR_CON_SC_DPM3_SRAM_SLEEP_B_ACK              Fld(1, 13) //[13:13]
    #define DPM3_PWR_CON_SC_DPM3_SRAM_PDN_ACK                  Fld(1, 12) //[12:12]
    #define DPM3_PWR_CON_DPM3_SRAM_SLEEP_B                     Fld(1, 9) //[9:9]
    #define DPM3_PWR_CON_DPM3_SRAM_PDN                         Fld(1, 8) //[8:8]
    #define DPM3_PWR_CON_DPM3_SRAM_ISOINT_B                    Fld(1, 6) //[6:6]
    #define DPM3_PWR_CON_DPM3_SRAM_CKISO                       Fld(1, 5) //[5:5]
    #define DPM3_PWR_CON_DPM3_PWR_CLK_DIS                      Fld(1, 4) //[4:4]
    #define DPM3_PWR_CON_DPM3_PWR_ON_2ND                       Fld(1, 3) //[3:3]
    #define DPM3_PWR_CON_DPM3_PWR_ON                           Fld(1, 2) //[2:2]
    #define DPM3_PWR_CON_DPM3_PWR_ISO                          Fld(1, 1) //[1:1]
    #define DPM3_PWR_CON_DPM3_PWR_RST_B                        Fld(1, 0) //[0:0]

#define SPM_REG_EMI0_PWR_CON                                   (SPM_BASE_ADDRESS + 0x00EAC)
    #define EMI0_PWR_CON_SC_EMI0_PWR_ACK_2ND                   Fld(1, 31) //[31:31]
    #define EMI0_PWR_CON_SC_EMI0_PWR_ACK                       Fld(1, 30) //[30:30]
    #define EMI0_PWR_CON_SC_EMI0_SRAM_SLEEP_B_ACK              Fld(1, 13) //[13:13]
    #define EMI0_PWR_CON_SC_EMI0_SRAM_PDN_ACK                  Fld(1, 12) //[12:12]
    #define EMI0_PWR_CON_EMI0_SRAM_SLEEP_B                     Fld(1, 9) //[9:9]
    #define EMI0_PWR_CON_EMI0_SRAM_PDN                         Fld(1, 8) //[8:8]
    #define EMI0_PWR_CON_EMI0_SRAM_ISOINT_B                    Fld(1, 6) //[6:6]
    #define EMI0_PWR_CON_EMI0_SRAM_CKISO                       Fld(1, 5) //[5:5]
    #define EMI0_PWR_CON_EMI0_PWR_CLK_DIS                      Fld(1, 4) //[4:4]
    #define EMI0_PWR_CON_EMI0_PWR_ON_2ND                       Fld(1, 3) //[3:3]
    #define EMI0_PWR_CON_EMI0_PWR_ON                           Fld(1, 2) //[2:2]
    #define EMI0_PWR_CON_EMI0_PWR_ISO                          Fld(1, 1) //[1:1]
    #define EMI0_PWR_CON_EMI0_PWR_RST_B                        Fld(1, 0) //[0:0]

#define SPM_REG_EMI1_PWR_CON                                   (SPM_BASE_ADDRESS + 0x00EB0)
    #define EMI1_PWR_CON_SC_EMI1_PWR_ACK_2ND                   Fld(1, 31) //[31:31]
    #define EMI1_PWR_CON_SC_EMI1_PWR_ACK                       Fld(1, 30) //[30:30]
    #define EMI1_PWR_CON_SC_EMI1_SRAM_SLEEP_B_ACK              Fld(1, 13) //[13:13]
    #define EMI1_PWR_CON_SC_EMI1_SRAM_PDN_ACK                  Fld(1, 12) //[12:12]
    #define EMI1_PWR_CON_EMI1_SRAM_SLEEP_B                     Fld(1, 9) //[9:9]
    #define EMI1_PWR_CON_EMI1_SRAM_PDN                         Fld(1, 8) //[8:8]
    #define EMI1_PWR_CON_EMI1_SRAM_ISOINT_B                    Fld(1, 6) //[6:6]
    #define EMI1_PWR_CON_EMI1_SRAM_CKISO                       Fld(1, 5) //[5:5]
    #define EMI1_PWR_CON_EMI1_PWR_CLK_DIS                      Fld(1, 4) //[4:4]
    #define EMI1_PWR_CON_EMI1_PWR_ON_2ND                       Fld(1, 3) //[3:3]
    #define EMI1_PWR_CON_EMI1_PWR_ON                           Fld(1, 2) //[2:2]
    #define EMI1_PWR_CON_EMI1_PWR_ISO                          Fld(1, 1) //[1:1]
    #define EMI1_PWR_CON_EMI1_PWR_RST_B                        Fld(1, 0) //[0:0]

#define SPM_REG_MCUPM_PWR_CON                                  (SPM_BASE_ADDRESS + 0x00EB4)
    #define MCUPM_PWR_CON_SC_MCUPM_PWR_ACK_2ND                 Fld(1, 31) //[31:31]
    #define MCUPM_PWR_CON_SC_MCUPM_PWR_ACK                     Fld(1, 30) //[30:30]
    #define MCUPM_PWR_CON_SC_MCUPM_SRAM_SLEEP_B_ACK            Fld(1, 13) //[13:13]
    #define MCUPM_PWR_CON_SC_MCUPM_SRAM_PDN_ACK                Fld(1, 12) //[12:12]
    #define MCUPM_PWR_CON_MCUPM_SRAM_SLEEP_B                   Fld(1, 9) //[9:9]
    #define MCUPM_PWR_CON_MCUPM_SRAM_PDN                       Fld(1, 8) //[8:8]
    #define MCUPM_PWR_CON_MCUPM_SRAM_ISOINT_B                  Fld(1, 6) //[6:6]
    #define MCUPM_PWR_CON_MCUPM_SRAM_CKISO                     Fld(1, 5) //[5:5]
    #define MCUPM_PWR_CON_MCUPM_PWR_CLK_DIS                    Fld(1, 4) //[4:4]
    #define MCUPM_PWR_CON_MCUPM_PWR_ON_2ND                     Fld(1, 3) //[3:3]
    #define MCUPM_PWR_CON_MCUPM_PWR_ON                         Fld(1, 2) //[2:2]
    #define MCUPM_PWR_CON_MCUPM_PWR_ISO                        Fld(1, 1) //[1:1]
    #define MCUPM_PWR_CON_MCUPM_PWR_RST_B                      Fld(1, 0) //[0:0]

#define SPM_REG_MFG0_PWR_CON                                   (SPM_BASE_ADDRESS + 0x00EB8)
    #define MFG0_PWR_CON_SC_MFG0_PWR_ACK_2ND                   Fld(1, 31) //[31:31]
    #define MFG0_PWR_CON_SC_MFG0_PWR_ACK                       Fld(1, 30) //[30:30]
    #define MFG0_PWR_CON_SC_MFG0_SRAM_SLEEP_B_ACK              Fld(1, 13) //[13:13]
    #define MFG0_PWR_CON_SC_MFG0_SRAM_PDN_ACK                  Fld(1, 12) //[12:12]
    #define MFG0_PWR_CON_MFG0_SRAM_SLEEP_B                     Fld(1, 9) //[9:9]
    #define MFG0_PWR_CON_MFG0_SRAM_PDN                         Fld(1, 8) //[8:8]
    #define MFG0_PWR_CON_MFG0_SRAM_ISOINT_B                    Fld(1, 6) //[6:6]
    #define MFG0_PWR_CON_MFG0_SRAM_CKISO                       Fld(1, 5) //[5:5]
    #define MFG0_PWR_CON_MFG0_PWR_CLK_DIS                      Fld(1, 4) //[4:4]
    #define MFG0_PWR_CON_MFG0_PWR_ON_2ND                       Fld(1, 3) //[3:3]
    #define MFG0_PWR_CON_MFG0_PWR_ON                           Fld(1, 2) //[2:2]
    #define MFG0_PWR_CON_MFG0_PWR_ISO                          Fld(1, 1) //[1:1]
    #define MFG0_PWR_CON_MFG0_PWR_RST_B                        Fld(1, 0) //[0:0]

#define SPM_REG_MFG1_PWR_CON                                   (SPM_BASE_ADDRESS + 0x00EBC)
    #define MFG1_PWR_CON_SC_MFG1_PWR_ACK_2ND                   Fld(1, 31) //[31:31]
    #define MFG1_PWR_CON_SC_MFG1_PWR_ACK                       Fld(1, 30) //[30:30]
    #define MFG1_PWR_CON_SC_MFG1_SRAM_PDN_ACK                  Fld(1, 12) //[12:12]
    #define MFG1_PWR_CON_MFG1_SRAM_PDN                         Fld(1, 8) //[8:8]
    #define MFG1_PWR_CON_MFG1_PWR_CLK_DIS                      Fld(1, 4) //[4:4]
    #define MFG1_PWR_CON_MFG1_PWR_ON_2ND                       Fld(1, 3) //[3:3]
    #define MFG1_PWR_CON_MFG1_PWR_ON                           Fld(1, 2) //[2:2]
    #define MFG1_PWR_CON_MFG1_PWR_ISO                          Fld(1, 1) //[1:1]
    #define MFG1_PWR_CON_MFG1_PWR_RST_B                        Fld(1, 0) //[0:0]

#define SPM_REG_MFG2_PWR_CON                                   (SPM_BASE_ADDRESS + 0x00EC0)
    #define MFG2_PWR_CON_SC_MFG2_PWR_ACK_2ND                   Fld(1, 31) //[31:31]
    #define MFG2_PWR_CON_SC_MFG2_PWR_ACK                       Fld(1, 30) //[30:30]
    #define MFG2_PWR_CON_SC_MFG2_SRAM_PDN_ACK                  Fld(1, 12) //[12:12]
    #define MFG2_PWR_CON_MFG2_SRAM_PDN                         Fld(1, 8) //[8:8]
    #define MFG2_PWR_CON_MFG2_PWR_CLK_DIS                      Fld(1, 4) //[4:4]
    #define MFG2_PWR_CON_MFG2_PWR_ON_2ND                       Fld(1, 3) //[3:3]
    #define MFG2_PWR_CON_MFG2_PWR_ON                           Fld(1, 2) //[2:2]
    #define MFG2_PWR_CON_MFG2_PWR_ISO                          Fld(1, 1) //[1:1]
    #define MFG2_PWR_CON_MFG2_PWR_RST_B                        Fld(1, 0) //[0:0]

#define SPM_REG_MFG3_PWR_CON                                   (SPM_BASE_ADDRESS + 0x00EC4)
    #define MFG3_PWR_CON_SC_MFG3_PWR_ACK_2ND                   Fld(1, 31) //[31:31]
    #define MFG3_PWR_CON_SC_MFG3_PWR_ACK                       Fld(1, 30) //[30:30]
    #define MFG3_PWR_CON_SC_MFG3_SRAM_PDN_ACK                  Fld(1, 12) //[12:12]
    #define MFG3_PWR_CON_MFG3_SRAM_PDN                         Fld(1, 8) //[8:8]
    #define MFG3_PWR_CON_MFG3_PWR_CLK_DIS                      Fld(1, 4) //[4:4]
    #define MFG3_PWR_CON_MFG3_PWR_ON_2ND                       Fld(1, 3) //[3:3]
    #define MFG3_PWR_CON_MFG3_PWR_ON                           Fld(1, 2) //[2:2]
    #define MFG3_PWR_CON_MFG3_PWR_ISO                          Fld(1, 1) //[1:1]
    #define MFG3_PWR_CON_MFG3_PWR_RST_B                        Fld(1, 0) //[0:0]

#define SPM_REG_MFG4_PWR_CON                                   (SPM_BASE_ADDRESS + 0x00EC8)
    #define MFG4_PWR_CON_SC_MFG4_PWR_ACK_2ND                   Fld(1, 31) //[31:31]
    #define MFG4_PWR_CON_SC_MFG4_PWR_ACK                       Fld(1, 30) //[30:30]
    #define MFG4_PWR_CON_SC_MFG4_SRAM_PDN_ACK                  Fld(1, 12) //[12:12]
    #define MFG4_PWR_CON_MFG4_SRAM_PDN                         Fld(1, 8) //[8:8]
    #define MFG4_PWR_CON_MFG4_PWR_CLK_DIS                      Fld(1, 4) //[4:4]
    #define MFG4_PWR_CON_MFG4_PWR_ON_2ND                       Fld(1, 3) //[3:3]
    #define MFG4_PWR_CON_MFG4_PWR_ON                           Fld(1, 2) //[2:2]
    #define MFG4_PWR_CON_MFG4_PWR_ISO                          Fld(1, 1) //[1:1]
    #define MFG4_PWR_CON_MFG4_PWR_RST_B                        Fld(1, 0) //[0:0]

#define SPM_REG_MFG5_PWR_CON                                   (SPM_BASE_ADDRESS + 0x00ECC)
    #define MFG5_PWR_CON_SC_MFG5_PWR_ACK_2ND                   Fld(1, 31) //[31:31]
    #define MFG5_PWR_CON_SC_MFG5_PWR_ACK                       Fld(1, 30) //[30:30]
    #define MFG5_PWR_CON_SC_MFG5_SRAM_PDN_ACK                  Fld(1, 12) //[12:12]
    #define MFG5_PWR_CON_MFG5_SRAM_PDN                         Fld(1, 8) //[8:8]
    #define MFG5_PWR_CON_MFG5_PWR_CLK_DIS                      Fld(1, 4) //[4:4]
    #define MFG5_PWR_CON_MFG5_PWR_ON_2ND                       Fld(1, 3) //[3:3]
    #define MFG5_PWR_CON_MFG5_PWR_ON                           Fld(1, 2) //[2:2]
    #define MFG5_PWR_CON_MFG5_PWR_ISO                          Fld(1, 1) //[1:1]
    #define MFG5_PWR_CON_MFG5_PWR_RST_B                        Fld(1, 0) //[0:0]

#define SPM_REG_MFG6_PWR_CON                                   (SPM_BASE_ADDRESS + 0x00ED0)
    #define MFG6_PWR_CON_SC_MFG6_PWR_ACK_2ND                   Fld(1, 31) //[31:31]
    #define MFG6_PWR_CON_SC_MFG6_PWR_ACK                       Fld(1, 30) //[30:30]
    #define MFG6_PWR_CON_SC_MFG6_SRAM_PDN_ACK                  Fld(1, 12) //[12:12]
    #define MFG6_PWR_CON_MFG6_SRAM_PDN                         Fld(1, 8) //[8:8]
    #define MFG6_PWR_CON_MFG6_PWR_CLK_DIS                      Fld(1, 4) //[4:4]
    #define MFG6_PWR_CON_MFG6_PWR_ON_2ND                       Fld(1, 3) //[3:3]
    #define MFG6_PWR_CON_MFG6_PWR_ON                           Fld(1, 2) //[2:2]
    #define MFG6_PWR_CON_MFG6_PWR_ISO                          Fld(1, 1) //[1:1]
    #define MFG6_PWR_CON_MFG6_PWR_RST_B                        Fld(1, 0) //[0:0]

#define SPM_REG_MFG7_PWR_CON                                   (SPM_BASE_ADDRESS + 0x00ED4)
    #define MFG7_PWR_CON_SC_MFG7_PWR_ACK_2ND                   Fld(1, 31) //[31:31]
    #define MFG7_PWR_CON_SC_MFG7_PWR_ACK                       Fld(1, 30) //[30:30]
    #define MFG7_PWR_CON_SC_MFG7_SRAM_PDN_ACK                  Fld(1, 12) //[12:12]
    #define MFG7_PWR_CON_MFG7_SRAM_PDN                         Fld(1, 8) //[8:8]
    #define MFG7_PWR_CON_MFG7_PWR_CLK_DIS                      Fld(1, 4) //[4:4]
    #define MFG7_PWR_CON_MFG7_PWR_ON_2ND                       Fld(1, 3) //[3:3]
    #define MFG7_PWR_CON_MFG7_PWR_ON                           Fld(1, 2) //[2:2]
    #define MFG7_PWR_CON_MFG7_PWR_ISO                          Fld(1, 1) //[1:1]
    #define MFG7_PWR_CON_MFG7_PWR_RST_B                        Fld(1, 0) //[0:0]

#define SPM_REG_MFG8_PWR_CON                                   (SPM_BASE_ADDRESS + 0x00ED8)
    #define MFG8_PWR_CON_SC_MFG8_PWR_ACK_2ND                   Fld(1, 31) //[31:31]
    #define MFG8_PWR_CON_SC_MFG8_PWR_ACK                       Fld(1, 30) //[30:30]
    #define MFG8_PWR_CON_SC_MFG8_SRAM_PDN_ACK                  Fld(1, 12) //[12:12]
    #define MFG8_PWR_CON_MFG8_SRAM_PDN                         Fld(1, 8) //[8:8]
    #define MFG8_PWR_CON_MFG8_PWR_CLK_DIS                      Fld(1, 4) //[4:4]
    #define MFG8_PWR_CON_MFG8_PWR_ON_2ND                       Fld(1, 3) //[3:3]
    #define MFG8_PWR_CON_MFG8_PWR_ON                           Fld(1, 2) //[2:2]
    #define MFG8_PWR_CON_MFG8_PWR_ISO                          Fld(1, 1) //[1:1]
    #define MFG8_PWR_CON_MFG8_PWR_RST_B                        Fld(1, 0) //[0:0]

#define SPM_REG_MFG9_PWR_CON                                   (SPM_BASE_ADDRESS + 0x00EDC)
    #define MFG9_PWR_CON_SC_MFG9_PWR_ACK_2ND                   Fld(1, 31) //[31:31]
    #define MFG9_PWR_CON_SC_MFG9_PWR_ACK                       Fld(1, 30) //[30:30]
    #define MFG9_PWR_CON_SC_MFG9_SRAM_PDN_ACK                  Fld(1, 12) //[12:12]
    #define MFG9_PWR_CON_MFG9_SRAM_PDN                         Fld(1, 8) //[8:8]
    #define MFG9_PWR_CON_MFG9_PWR_CLK_DIS                      Fld(1, 4) //[4:4]
    #define MFG9_PWR_CON_MFG9_PWR_ON_2ND                       Fld(1, 3) //[3:3]
    #define MFG9_PWR_CON_MFG9_PWR_ON                           Fld(1, 2) //[2:2]
    #define MFG9_PWR_CON_MFG9_PWR_ISO                          Fld(1, 1) //[1:1]
    #define MFG9_PWR_CON_MFG9_PWR_RST_B                        Fld(1, 0) //[0:0]

#define SPM_REG_MFG10_PWR_CON                                  (SPM_BASE_ADDRESS + 0x00EE0)
    #define MFG10_PWR_CON_SC_MFG10_PWR_ACK_2ND                 Fld(1, 31) //[31:31]
    #define MFG10_PWR_CON_SC_MFG10_PWR_ACK                     Fld(1, 30) //[30:30]
    #define MFG10_PWR_CON_SC_MFG10_SRAM_PDN_ACK                Fld(1, 12) //[12:12]
    #define MFG10_PWR_CON_MFG10_SRAM_PDN                       Fld(1, 8) //[8:8]
    #define MFG10_PWR_CON_MFG10_PWR_CLK_DIS                    Fld(1, 4) //[4:4]
    #define MFG10_PWR_CON_MFG10_PWR_ON_2ND                     Fld(1, 3) //[3:3]
    #define MFG10_PWR_CON_MFG10_PWR_ON                         Fld(1, 2) //[2:2]
    #define MFG10_PWR_CON_MFG10_PWR_ISO                        Fld(1, 1) //[1:1]
    #define MFG10_PWR_CON_MFG10_PWR_RST_B                      Fld(1, 0) //[0:0]

#define SPM_REG_MFG11_PWR_CON                                  (SPM_BASE_ADDRESS + 0x00EE4)
    #define MFG11_PWR_CON_SC_MFG11_PWR_ACK_2ND                 Fld(1, 31) //[31:31]
    #define MFG11_PWR_CON_SC_MFG11_PWR_ACK                     Fld(1, 30) //[30:30]
    #define MFG11_PWR_CON_SC_MFG11_SRAM_PDN_ACK                Fld(1, 12) //[12:12]
    #define MFG11_PWR_CON_MFG11_SRAM_PDN                       Fld(1, 8) //[8:8]
    #define MFG11_PWR_CON_MFG11_PWR_CLK_DIS                    Fld(1, 4) //[4:4]
    #define MFG11_PWR_CON_MFG11_PWR_ON_2ND                     Fld(1, 3) //[3:3]
    #define MFG11_PWR_CON_MFG11_PWR_ON                         Fld(1, 2) //[2:2]
    #define MFG11_PWR_CON_MFG11_PWR_ISO                        Fld(1, 1) //[1:1]
    #define MFG11_PWR_CON_MFG11_PWR_RST_B                      Fld(1, 0) //[0:0]

#define SPM_REG_MFG12_PWR_CON                                  (SPM_BASE_ADDRESS + 0x00EE8)
    #define MFG12_PWR_CON_SC_MFG12_PWR_ACK_2ND                 Fld(1, 31) //[31:31]
    #define MFG12_PWR_CON_SC_MFG12_PWR_ACK                     Fld(1, 30) //[30:30]
    #define MFG12_PWR_CON_SC_MFG12_SRAM_PDN_ACK                Fld(1, 12) //[12:12]
    #define MFG12_PWR_CON_MFG12_SRAM_PDN                       Fld(1, 8) //[8:8]
    #define MFG12_PWR_CON_MFG12_PWR_CLK_DIS                    Fld(1, 4) //[4:4]
    #define MFG12_PWR_CON_MFG12_PWR_ON_2ND                     Fld(1, 3) //[3:3]
    #define MFG12_PWR_CON_MFG12_PWR_ON                         Fld(1, 2) //[2:2]
    #define MFG12_PWR_CON_MFG12_PWR_ISO                        Fld(1, 1) //[1:1]
    #define MFG12_PWR_CON_MFG12_PWR_RST_B                      Fld(1, 0) //[0:0]

#define SPM_REG_MFG13_PWR_CON                                  (SPM_BASE_ADDRESS + 0x00EEC)
    #define MFG13_PWR_CON_SC_MFG13_PWR_ACK_2ND                 Fld(1, 31) //[31:31]
    #define MFG13_PWR_CON_SC_MFG13_PWR_ACK                     Fld(1, 30) //[30:30]
    #define MFG13_PWR_CON_SC_MFG13_SRAM_PDN_ACK                Fld(1, 12) //[12:12]
    #define MFG13_PWR_CON_MFG13_SRAM_PDN                       Fld(1, 8) //[8:8]
    #define MFG13_PWR_CON_MFG13_PWR_CLK_DIS                    Fld(1, 4) //[4:4]
    #define MFG13_PWR_CON_MFG13_PWR_ON_2ND                     Fld(1, 3) //[3:3]
    #define MFG13_PWR_CON_MFG13_PWR_ON                         Fld(1, 2) //[2:2]
    #define MFG13_PWR_CON_MFG13_PWR_ISO                        Fld(1, 1) //[1:1]
    #define MFG13_PWR_CON_MFG13_PWR_RST_B                      Fld(1, 0) //[0:0]

#define SPM_REG_MFG14_PWR_CON                                  (SPM_BASE_ADDRESS + 0x00EF0)
    #define MFG14_PWR_CON_SC_MFG14_PWR_ACK_2ND                 Fld(1, 31) //[31:31]
    #define MFG14_PWR_CON_SC_MFG14_PWR_ACK                     Fld(1, 30) //[30:30]
    #define MFG14_PWR_CON_SC_MFG14_SRAM_PDN_ACK                Fld(1, 12) //[12:12]
    #define MFG14_PWR_CON_MFG14_SRAM_PDN                       Fld(1, 8) //[8:8]
    #define MFG14_PWR_CON_MFG14_PWR_CLK_DIS                    Fld(1, 4) //[4:4]
    #define MFG14_PWR_CON_MFG14_PWR_ON_2ND                     Fld(1, 3) //[3:3]
    #define MFG14_PWR_CON_MFG14_PWR_ON                         Fld(1, 2) //[2:2]
    #define MFG14_PWR_CON_MFG14_PWR_ISO                        Fld(1, 1) //[1:1]
    #define MFG14_PWR_CON_MFG14_PWR_RST_B                      Fld(1, 0) //[0:0]

#define SPM_REG_MFG15_PWR_CON                                  (SPM_BASE_ADDRESS + 0x00EF4)
    #define MFG15_PWR_CON_SC_MFG15_PWR_ACK_2ND                 Fld(1, 31) //[31:31]
    #define MFG15_PWR_CON_SC_MFG15_PWR_ACK                     Fld(1, 30) //[30:30]
    #define MFG15_PWR_CON_SC_MFG15_SRAM_PDN_ACK                Fld(1, 12) //[12:12]
    #define MFG15_PWR_CON_MFG15_SRAM_PDN                       Fld(1, 8) //[8:8]
    #define MFG15_PWR_CON_MFG15_PWR_CLK_DIS                    Fld(1, 4) //[4:4]
    #define MFG15_PWR_CON_MFG15_PWR_ON_2ND                     Fld(1, 3) //[3:3]
    #define MFG15_PWR_CON_MFG15_PWR_ON                         Fld(1, 2) //[2:2]
    #define MFG15_PWR_CON_MFG15_PWR_ISO                        Fld(1, 1) //[1:1]
    #define MFG15_PWR_CON_MFG15_PWR_RST_B                      Fld(1, 0) //[0:0]

#define SPM_REG_MFG16_PWR_CON                                  (SPM_BASE_ADDRESS + 0x00EF8)
    #define MFG16_PWR_CON_SC_MFG16_PWR_ACK_2ND                 Fld(1, 31) //[31:31]
    #define MFG16_PWR_CON_SC_MFG16_PWR_ACK                     Fld(1, 30) //[30:30]
    #define MFG16_PWR_CON_SC_MFG16_SRAM_PDN_ACK                Fld(1, 12) //[12:12]
    #define MFG16_PWR_CON_MFG16_SRAM_PDN                       Fld(1, 8) //[8:8]
    #define MFG16_PWR_CON_MFG16_PWR_CLK_DIS                    Fld(1, 4) //[4:4]
    #define MFG16_PWR_CON_MFG16_PWR_ON_2ND                     Fld(1, 3) //[3:3]
    #define MFG16_PWR_CON_MFG16_PWR_ON                         Fld(1, 2) //[2:2]
    #define MFG16_PWR_CON_MFG16_PWR_ISO                        Fld(1, 1) //[1:1]
    #define MFG16_PWR_CON_MFG16_PWR_RST_B                      Fld(1, 0) //[0:0]

#define SPM_REG_MFG17_PWR_CON                                  (SPM_BASE_ADDRESS + 0x00EFC)
    #define MFG17_PWR_CON_SC_MFG17_PWR_ACK_2ND                 Fld(1, 31) //[31:31]
    #define MFG17_PWR_CON_SC_MFG17_PWR_ACK                     Fld(1, 30) //[30:30]
    #define MFG17_PWR_CON_SC_MFG17_SRAM_PDN_ACK                Fld(1, 12) //[12:12]
    #define MFG17_PWR_CON_MFG17_SRAM_PDN                       Fld(1, 8) //[8:8]
    #define MFG17_PWR_CON_MFG17_PWR_CLK_DIS                    Fld(1, 4) //[4:4]
    #define MFG17_PWR_CON_MFG17_PWR_ON_2ND                     Fld(1, 3) //[3:3]
    #define MFG17_PWR_CON_MFG17_PWR_ON                         Fld(1, 2) //[2:2]
    #define MFG17_PWR_CON_MFG17_PWR_ISO                        Fld(1, 1) //[1:1]
    #define MFG17_PWR_CON_MFG17_PWR_RST_B                      Fld(1, 0) //[0:0]

#define SPM_REG_MFG18_PWR_CON                                  (SPM_BASE_ADDRESS + 0x00F00)
    #define MFG18_PWR_CON_SC_MFG18_PWR_ACK_2ND                 Fld(1, 31) //[31:31]
    #define MFG18_PWR_CON_SC_MFG18_PWR_ACK                     Fld(1, 30) //[30:30]
    #define MFG18_PWR_CON_SC_MFG18_SRAM_PDN_ACK                Fld(1, 12) //[12:12]
    #define MFG18_PWR_CON_MFG18_SRAM_PDN                       Fld(1, 8) //[8:8]
    #define MFG18_PWR_CON_MFG18_PWR_CLK_DIS                    Fld(1, 4) //[4:4]
    #define MFG18_PWR_CON_MFG18_PWR_ON_2ND                     Fld(1, 3) //[3:3]
    #define MFG18_PWR_CON_MFG18_PWR_ON                         Fld(1, 2) //[2:2]
    #define MFG18_PWR_CON_MFG18_PWR_ISO                        Fld(1, 1) //[1:1]
    #define MFG18_PWR_CON_MFG18_PWR_RST_B                      Fld(1, 0) //[0:0]

#define SPM_REG_EFUSE_SRAM_CON                                 (SPM_BASE_ADDRESS + 0x00F04)
    #define EFUSE_SRAM_CON_EFUSE_SRAM_CKISO                    Fld(1, 0) //[0:0]
    #define EFUSE_SRAM_CON_EFUSE_SRAM_ISOINT_B                 Fld(1, 1) //[1:1]
    #define EFUSE_SRAM_CON_EFUSE_SRAM_SLEEP_B                  Fld(1, 4) //[4:4]
    #define EFUSE_SRAM_CON_EFUSE_SRAM_PDN                      Fld(1, 16) //[16:16]

#define SPM_REG_EMI_HRE_SRAM_CON                               (SPM_BASE_ADDRESS + 0x00F08)
    #define EMI_HRE_SRAM_CON_EMI_HRE_SRAM_CKISO                Fld(1, 0) //[0:0]
    #define EMI_HRE_SRAM_CON_EMI_HRE_SRAM_ISOINT_B             Fld(1, 1) //[1:1]
    #define EMI_HRE_SRAM_CON_EMI_HRE_SRAM_SLEEP_B              Fld(8, 4) //[11:4]
    #define EMI_HRE_SRAM_CON_EMI_HRE_SRAM_PDN                  Fld(8, 16) //[23:16]

#define SPM_REG_EMI_SLB_SRAM_CON                               (SPM_BASE_ADDRESS + 0x00F0C)
    #define EMI_SLB_SRAM_CON_SC_EMI_SLB_SRAM_PDN_ACK           Fld(12, 16) //[27:16]
    #define EMI_SLB_SRAM_CON_EMI_SLB_SRAM_PDN                  Fld(12, 0) //[11:0]

#define SPM_REG_INFRA_HRE_SRAM_CON                             (SPM_BASE_ADDRESS + 0x00F10)
    #define INFRA_HRE_SRAM_CON_INFRA_HRE_SRAM_CKISO            Fld(1, 0) //[0:0]
    #define INFRA_HRE_SRAM_CON_INFRA_HRE_SRAM_ISOINT_B         Fld(1, 1) //[1:1]
    #define INFRA_HRE_SRAM_CON_INFRA_HRE_SRAM_SLEEP_B          Fld(6, 4) //[9:4]
    #define INFRA_HRE_SRAM_CON_INFRA_HRE_SRAM_PDN              Fld(6, 16) //[21:16]

#define SPM_REG_INFRA_SLEEP_SRAM_CON                           (SPM_BASE_ADDRESS + 0x00F14)
    #define INFRA_SLEEP_SRAM_CON_SC_INFRA_SLEEP_SRAM_SLEEP_B_ACK Fld(2, 18) //[19:18]
    #define INFRA_SLEEP_SRAM_CON_SC_INFRA_SLEEP_SRAM_PDN_ACK   Fld(2, 16) //[17:16]
    #define INFRA_SLEEP_SRAM_CON_INFRA_SLEEP_SRAM_CKISO        Fld(1, 0) //[0:0]
    #define INFRA_SLEEP_SRAM_CON_INFRA_SLEEP_SRAM_ISOINT_B     Fld(2, 1) //[2:1]
    #define INFRA_SLEEP_SRAM_CON_INFRA_SLEEP_SRAM_SLEEP_B      Fld(2, 4) //[5:4]
    #define INFRA_SLEEP_SRAM_CON_INFRA_SLEEP_SRAM_PDN          Fld(2, 8) //[9:8]

#define SPM_REG_MM_HRE_SRAM_CON                                (SPM_BASE_ADDRESS + 0x00F18)
    #define MM_HRE_SRAM_CON_MM_HRE_SRAM_CKISO                  Fld(1, 0) //[0:0]
    #define MM_HRE_SRAM_CON_MM_HRE_SRAM_ISOINT_B               Fld(3, 1) //[3:1]
    #define MM_HRE_SRAM_CON_MM_HRE_SRAM_SLEEP_B                Fld(3, 4) //[6:4]
    #define MM_HRE_SRAM_CON_MM_HRE_SRAM_PDN                    Fld(3, 16) //[18:16]

#define SPM_REG_SPM_SRAM_CON                                   (SPM_BASE_ADDRESS + 0x00F1C)
    #define SPM_SRAM_CON_SPM_SRAM_CKISO                        Fld(1, 0) //[0:0]
    #define SPM_SRAM_CON_REG_SPM_SRAM_ISOINT_B                 Fld(1, 1) //[1:1]
    #define SPM_SRAM_CON_REG_SPM_SRAM_SLEEP_B                  Fld(2, 4) //[5:4]
    #define SPM_SRAM_CON_SPM_SRAM_PDN                          Fld(2, 16) //[17:16]

#define SPM_REG_SSPM_SRAM_CON                                  (SPM_BASE_ADDRESS + 0x00F20)
    #define SSPM_SRAM_CON_SSPM_SRAM_CKISO                      Fld(1, 0) //[0:0]
    #define SSPM_SRAM_CON_SSPM_SRAM_ISOINT_B                   Fld(1, 1) //[1:1]
    #define SSPM_SRAM_CON_SSPM_SRAM_SLEEP_B                    Fld(1, 4) //[4:4]
    #define SSPM_SRAM_CON_SSPM_SRAM_PDN                        Fld(1, 16) //[16:16]

#define SPM_REG_UFS_PDN_SRAM_CON                               (SPM_BASE_ADDRESS + 0x00F24)
    #define UFS_PDN_SRAM_CON_SC_UFS_PDN_SRAM_PDN_ACK           Fld(1, 8) //[8:8]
    #define UFS_PDN_SRAM_CON_UFS_PDN_SRAM_PDN                  Fld(1, 0) //[0:0]

#define SPM_REG_CPU_BUCK_ISO_CON                               (SPM_BASE_ADDRESS + 0x00F28)
    #define CPU_BUCK_ISO_CON_MP0_VSRAM_EXT_OFF                 Fld(1, 10) //[10:10]
    #define CPU_BUCK_ISO_CON_MP0_VPROC_EXT_OFF_CPU7            Fld(1, 9) //[9:9]
    #define CPU_BUCK_ISO_CON_MP0_VPROC_EXT_OFF_CPU6            Fld(1, 8) //[8:8]
    #define CPU_BUCK_ISO_CON_MP0_VPROC_EXT_OFF_CPU5            Fld(1, 7) //[7:7]
    #define CPU_BUCK_ISO_CON_MP0_VPROC_EXT_OFF_CPU4            Fld(1, 6) //[6:6]
    #define CPU_BUCK_ISO_CON_MP0_VPROC_EXT_OFF_CPU3            Fld(1, 5) //[5:5]
    #define CPU_BUCK_ISO_CON_MP0_VPROC_EXT_OFF_CPU2            Fld(1, 4) //[4:4]
    #define CPU_BUCK_ISO_CON_MP0_VPROC_EXT_OFF_CPU1            Fld(1, 3) //[3:3]
    #define CPU_BUCK_ISO_CON_MP0_VPROC_EXT_OFF_CPU0            Fld(1, 2) //[2:2]
    #define CPU_BUCK_ISO_CON_MP0_VPROC_EXT_OFF                 Fld(1, 1) //[1:1]
    #define CPU_BUCK_ISO_CON_MCUSYS_VPROC_EXT_OFF              Fld(1, 0) //[0:0]

#define SPM_REG_MD_BUCK_ISO_CON                                (SPM_BASE_ADDRESS + 0x00F2C)
    #define MD_BUCK_ISO_CON_AOC_VMODEM_ANA_ISO                 Fld(1, 7) //[7:7]
    #define MD_BUCK_ISO_CON_AOC_VMODEM_SRAM_LATCH_ENB          Fld(1, 6) //[6:6]
    #define MD_BUCK_ISO_CON_AOC_VMODEM_SRAM_ISO_DIN            Fld(1, 5) //[5:5]
    #define MD_BUCK_ISO_CON_VMODEM_EXT_BUCK_ISO                Fld(1, 4) //[4:4]
    #define MD_BUCK_ISO_CON_AOC_VMD_ANA_ISO                    Fld(1, 3) //[3:3]
    #define MD_BUCK_ISO_CON_AOC_VMD_SRAM_LATCH_ENB             Fld(1, 2) //[2:2]
    #define MD_BUCK_ISO_CON_AOC_VMD_SRAM_ISO_DIN               Fld(1, 1) //[1:1]
    #define MD_BUCK_ISO_CON_VMD_EXT_BUCK_ISO                   Fld(1, 0) //[0:0]

#define SPM_REG_SOC_BUCK_ISO_CON                               (SPM_BASE_ADDRESS + 0x00F30)
    #define SOC_BUCK_ISO_CON_AOC_VMM_ANA_ISO                   Fld(1, 19) //[19:19]
    #define SOC_BUCK_ISO_CON_AOC_VMM_SRAM_LATCH_ENB            Fld(1, 18) //[18:18]
    #define SOC_BUCK_ISO_CON_AOC_VMM_SRAM_ISO_DIN              Fld(1, 17) //[17:17]
    #define SOC_BUCK_ISO_CON_VMM_EXT_BUCK_ISO                  Fld(1, 16) //[16:16]
    #define SOC_BUCK_ISO_CON_AOC_VGPUTOP_ANA_ISO               Fld(1, 15) //[15:15]
    #define SOC_BUCK_ISO_CON_AOC_VGPUTOP_SRAM_LATCH_ENB        Fld(1, 14) //[14:14]
    #define SOC_BUCK_ISO_CON_AOC_VGPUTOP_SRAM_ISO_DIN          Fld(1, 13) //[13:13]
    #define SOC_BUCK_ISO_CON_VGPUTOP_EXT_BUCK_ISO              Fld(1, 12) //[12:12]
    #define SOC_BUCK_ISO_CON_AOC_VGPU_ANA_ISO                  Fld(1, 11) //[11:11]
    #define SOC_BUCK_ISO_CON_AOC_VGPU_SRAM_LATCH_ENB           Fld(1, 10) //[10:10]
    #define SOC_BUCK_ISO_CON_AOC_VGPU_SRAM_ISO_DIN             Fld(1, 9) //[9:9]
    #define SOC_BUCK_ISO_CON_VGPU_EXT_BUCK_ISO                 Fld(1, 8) //[8:8]
    #define SOC_BUCK_ISO_CON_AOC_VAPU_ANA_ISO                  Fld(1, 7) //[7:7]
    #define SOC_BUCK_ISO_CON_AOC_VAPU_SRAM_LATCH_ENB           Fld(1, 6) //[6:6]
    #define SOC_BUCK_ISO_CON_AOC_VAPU_SRAM_ISO_DIN             Fld(1, 5) //[5:5]
    #define SOC_BUCK_ISO_CON_VAPU_EXT_BUCK_ISO                 Fld(1, 4) //[4:4]
    #define SOC_BUCK_ISO_CON_AOC_VADSP_ANA_ISO                 Fld(1, 3) //[3:3]
    #define SOC_BUCK_ISO_CON_AOC_VADSP_SRAM_LATCH_ENB          Fld(1, 2) //[2:2]
    #define SOC_BUCK_ISO_CON_AOC_VADSP_SRAM_ISO_DIN            Fld(1, 1) //[1:1]
    #define SOC_BUCK_ISO_CON_VADSP_EXT_BUCK_ISO                Fld(1, 0) //[0:0]

#define SPM_REG_PWR_STATUS                                     (SPM_BASE_ADDRESS + 0x00F34)
    #define PWR_STATUS_PWR_STATUS                              Fld(32, 0) //[31:0]
    #define PWR_STATUS_DRAMC_PWR_ACK                           Fld(1, 4)//[4:4] cc from MT6877. TBC
    #define PWR_STATUS_DPM_PWR_ACK                             Fld(1, 6)//[6:6] cc from MT6877. TBC

#define SPM_REG_PWR_STATUS_2ND                                 (SPM_BASE_ADDRESS + 0x00F38)
    #define PWR_STATUS_2ND_PWR_STATUS_2ND                      Fld(32, 0) //[31:0]
    #define PWR_STATUS_2ND_DRAMC_PWR_ACK                       Fld(1, 4)//[4:4] cc from MT6877. TBC
    #define PWR_STATUS_2ND_DPM_PWR_ACK                         Fld(1, 6)//[6:6] cc from MT6877. TBC

#define SPM_REG_XPU_PWR_STATUS                                 (SPM_BASE_ADDRESS + 0x00F3C)
    #define XPU_PWR_STATUS_XPU_PWR_STATUS                      Fld(32, 0) //[31:0]

#define SPM_REG_XPU_PWR_STATUS_2ND                             (SPM_BASE_ADDRESS + 0x00F40)
    #define XPU_PWR_STATUS_2ND_XPU_PWR_STATUS_2ND              Fld(32, 0) //[31:0]

#define SPM_REG_DFD_SOC_PWR_LATCH                              (SPM_BASE_ADDRESS + 0x00F44)
    #define DFD_SOC_PWR_LATCH_DFD_SOC_PWR_LATCH                Fld(32, 0) //[31:0]

#define SPM_REG_SPM_TWAM_CON                                   (SPM_BASE_ADDRESS + 0x00F80)
    #define SPM_TWAM_CON_TWAM_IRQ                              Fld(1, 24) //[24:24]
    #define SPM_TWAM_CON_REG_TWAM_IRQ_CLEAR                    Fld(1, 16) //[16:16]
    #define SPM_TWAM_CON_REG_TWAM_MON_TYPE_3                   Fld(2, 10) //[11:10]
    #define SPM_TWAM_CON_REG_TWAM_MON_TYPE_2                   Fld(2, 8) //[9:8]
    #define SPM_TWAM_CON_REG_TWAM_MON_TYPE_1                   Fld(2, 6) //[7:6]
    #define SPM_TWAM_CON_REG_TWAM_MON_TYPE_0                   Fld(2, 4) //[5:4]
    #define SPM_TWAM_CON_REG_TWAM_IRQ_MASK                     Fld(1, 3) //[3:3]
    #define SPM_TWAM_CON_SPM_TWAM_EVENT_CLEAR                  Fld(1, 2) //[2:2]
    #define SPM_TWAM_CON_REG_TWAM_SPEED_MODE_EN                Fld(1, 1) //[1:1]
    #define SPM_TWAM_CON_REG_TWAM_ENABLE                       Fld(1, 0) //[0:0]

#define SPM_REG_SPM_TWAM_WINDOW_LEN                            (SPM_BASE_ADDRESS + 0x00F84)
    #define SPM_TWAM_WINDOW_LEN_REG_TWAM_WINDOW_LEN            Fld(32, 0) //[31:0]

#define SPM_REG_SPM_TWAM_IDLE_SEL                              (SPM_BASE_ADDRESS + 0x00F88)
    #define SPM_TWAM_IDLE_SEL_REG_TWAM_SIG_SEL_3               Fld(7, 24) //[30:24]
    #define SPM_TWAM_IDLE_SEL_REG_TWAM_SIG_SEL_2               Fld(7, 16) //[22:16]
    #define SPM_TWAM_IDLE_SEL_REG_TWAM_SIG_SEL_1               Fld(7, 8) //[14:8]
    #define SPM_TWAM_IDLE_SEL_REG_TWAM_SIG_SEL_0               Fld(7, 0) //[6:0]

#define SPM_REG_SPM_TWAM_LAST_STA_0                            (SPM_BASE_ADDRESS + 0x00F8C)
    #define SPM_TWAM_LAST_STA_0_TWAM_LAST_IDLE_CNT_0           Fld(32, 0) //[31:0]

#define SPM_REG_SPM_TWAM_LAST_STA_1                            (SPM_BASE_ADDRESS + 0x00F90)
    #define SPM_TWAM_LAST_STA_1_TWAM_LAST_IDLE_CNT_1           Fld(32, 0) //[31:0]

#define SPM_REG_SPM_TWAM_LAST_STA_2                            (SPM_BASE_ADDRESS + 0x00F94)
    #define SPM_TWAM_LAST_STA_2_TWAM_LAST_IDLE_CNT_2           Fld(32, 0) //[31:0]

#define SPM_REG_SPM_TWAM_LAST_STA_3                            (SPM_BASE_ADDRESS + 0x00F98)
    #define SPM_TWAM_LAST_STA_3_TWAM_LAST_IDLE_CNT_3           Fld(32, 0) //[31:0]

#define SPM_REG_SPM_TWAM_CURR_STA_0                            (SPM_BASE_ADDRESS + 0x00F9C)
    #define SPM_TWAM_CURR_STA_0_TWAM_CURRENT_IDLE_CNT_0        Fld(32, 0) //[31:0]

#define SPM_REG_SPM_TWAM_CURR_STA_1                            (SPM_BASE_ADDRESS + 0x00FA0)
    #define SPM_TWAM_CURR_STA_1_TWAM_CURRENT_IDLE_CNT_1        Fld(32, 0) //[31:0]

#define SPM_REG_SPM_TWAM_CURR_STA_2                            (SPM_BASE_ADDRESS + 0x00FA4)
    #define SPM_TWAM_CURR_STA_2_TWAM_CURRENT_IDLE_CNT_2        Fld(32, 0) //[31:0]

#define SPM_REG_SPM_TWAM_CURR_STA_3                            (SPM_BASE_ADDRESS + 0x00FA8)
    #define SPM_TWAM_CURR_STA_3_TWAM_CURRENT_IDLE_CNT_3        Fld(32, 0) //[31:0]

#define SPM_REG_SPM_TWAM_TIMER_OUT                             (SPM_BASE_ADDRESS + 0x00FAC)
    #define SPM_TWAM_TIMER_OUT_TWAM_TIMER                      Fld(32, 0) //[31:0]

#endif // __SPM_REGS_H__
