#ifndef __MT_I2C_H__
#define __MT_I2C_H__

#include "platform.h"
//#include <mt_reg_base.h>
#include "pal_log.h" /* For print */

#define I2C_NR		10 /* Number of I2C controllers */

#define I2CTAG                "[I2C] "
#if 0
#define I2CLOG(fmt, arg...)   dprintf(INFO,I2CTAG fmt, ##arg)
#define I2CMSG(fmt, arg...)   dprintf(SPEW,fmt, ##arg)
#define I2CERR(fmt, arg...)   dprintf(ALWAYS,I2CTAG "%d: "fmt, __LINE__, ##arg)
#define I2CFUC(fmt, arg...)   dprintf(I2CTAG "%s\n", __FUNCTION__)
#endif
#define I2CLOG(fmt, arg...)   pal_log_info(I2CTAG fmt, ##arg)
#define I2CMSG(fmt, arg...)   pal_log_info(fmt, ##arg)
#define I2CERR(fmt, arg...)   pal_log_err(I2CTAG "%d: "fmt, __LINE__, ##arg)
#define I2CFUC(fmt, arg...)   pal_log_info(I2CTAG "%s\n", __FUNCTION__)
#if (CFG_FPGA_PLATFORM)
  #define CONFIG_MT_I2C_FPGA_ENABLE
#endif

#ifdef CONFIG_MT_I2C_FPGA_ENABLE
#define FPGA_CLOCK		10000 /* FPGA crystal frequency (KHz) */
#define I2C_CLK_DIV		(2*5) /* frequency divisor */
#define I2C_CLK_RATE		(FPGA_CLOCK / I2C_CLK_DIV) /* I2C base clock (KHz) */
#define SCP_I2C_CLK		(FPGA_CLOCK / 16)
#else
#define I2C_CLK_RATE		124800 / I2C_CLK_DIV /* TODO: Calculate from bus clock */
#define I2C_CLK_DIV		(5) /* frequency divisor */
#define SCP_I2C_CLK		(26000 / 2) /* TODO: Check the correct frequency */
#endif

#define I2C_MB()
#define I2C_BUG_ON(a)
#define I2C_M_RD       0x0001

#define SCP_I2C1_MULTI_CH_EN 1

#define SCP_I3C_BCLK_SW         (0x1C721050)
#define SCP_CLK_CG_CTRL         (0x1C721030)
#define SCP_CLK_CG_CTRL1        (0x1C721078)
#define SCP_CLK_CG_W1C_CEN      (0x1C7B7E14)
#define SCP_CLK_W1C_CEN_ALL     (0x7F)
#define SCP_CLK_W1C_CEN_I2C1    (0x2)
#define SCP_BCLK_CK_STA_MASK    (0xf << 8)
#define SCP_I3C_BCLK_CK_STA_ULPOSC_DIV4 (0x4 << 8)
#define SCP_I3C_BCLK_CK_STA_ULPOSC_DIV2 (0x8 << 8)
#define SCP_BCLK_SW_ULPOSC_MASK    (0x3)
#define SCP_I3C_BCLK_SW_ULPOSC_DIV4     (0x2)
#define SCP_I3C_BCLK_SW_ULPOSC_DIV2     (0x3)
#define I2C_BCLK_CG_BIT         (1 << 21)
#define I2C_DMA_CG_BIT         (1 << 20)
#define I2C_PCLK_CG_BIT         (1 << 3)
#define I2C_I3C2_1_CG_BIT       (1 << 2)
#define I2C_I3C1_1_CG_BIT       (1 << 1)

#ifdef I2C_MULTI_CH_MASK
#define CH_OFFSET    0x0
#else
#define CH_OFFSET    0x100
#endif

#ifndef I2C0_BASE
#define I2C0_BASE (0x11E00000)
#endif
#ifndef I2C1_BASE
#define I2C1_BASE (0x11D00000)
#endif
#ifndef I2C2_BASE
#define I2C2_BASE (I2C2_BASE_SE + CH_OFFSET)
#endif
#ifndef I2C2_BASE_SE
#define I2C2_BASE_SE (0x11D01000)
#endif
#ifndef I2C3_BASE
#define I2C3_BASE (0x11D02000)
#endif
#ifndef I2C4_BASE
#define I2C4_BASE (I2C4_BASE_SE + CH_OFFSET)
#endif
#ifndef I2C4_BASE_SE
#define I2C4_BASE_SE (0x11D03000)
#endif
#ifndef I2C5_BASE
#define I2C5_BASE (0x11E01000)
#endif
#ifndef I2C6_BASE
#define I2C6_BASE (0x11E02000)
#endif
#ifndef I2C7_BASE
#define I2C7_BASE (I2C7_BASE_SE + CH_OFFSET)
#endif
#ifndef I2C7_BASE_SE
#define I2C7_BASE_SE (0x11D04000)
#endif
#ifndef I2C8_BASE
#define I2C8_BASE (I2C8_BASE_SE + CH_OFFSET)
#endif
#ifndef I2C8_BASE_SE
#define I2C8_BASE_SE (0x11D05000)
#endif
#ifndef I2C9_BASE
#define I2C9_BASE (I2C9_BASE_SE + CH_OFFSET)
#endif
#ifndef I2C9_BASE_SE
#define I2C9_BASE_SE (0x11D06000)
#endif
#ifndef I2C10_BASE
#define I2C10_BASE (0x11280000)
#endif
#ifndef I2C11_BASE
#define I2C11_BASE (0x11281000)
#endif
#ifndef I2C12_BASE
#define I2C12_BASE (I2C12_BASE_SE + CH_OFFSET)
#endif
#ifndef I2C12_BASE_SE
#define I2C12_BASE_SE (0x11282000)
#endif
#ifndef I2C13_BASE
#define I2C13_BASE (I2C13_BASE_SE + CH_OFFSET)
#endif
#ifndef I2C13_BASE_SE
#define I2C13_BASE_SE (0x11283000)
#endif
#ifndef SCP_I2C1_BASE
#if SCP_I2C1_MULTI_CH_EN
#define SCP_I2C1_BASE (SCP_I2C1_BASE_SE + 0x200)
#else
#define SCP_I2C1_BASE (SCP_I2C1_BASE_SE)
#endif
#endif
#ifndef SCP_I2C1_BASE_SE
#define SCP_I2C1_BASE_SE (0x1c7b1000)
#endif

#ifndef SCP_GLOBAL_DMA_SEC
#define SCP_GLOBAL_DMA_SEC (0x1c753010)
#endif

#define I2C0_APDMA_BASE (0x11300200)
#define I2C1_APDMA_BASE (0x11300280)
#define I2C2_APDMA_BASE (0x11300300)
#define I2C3_APDMA_BASE (0x11300480)
#define I2C4_APDMA_BASE (0x11300500)
#define I2C5_APDMA_BASE (0x11300680)
#define I2C6_APDMA_BASE (0x11300700)
#define I2C7_APDMA_BASE (0x11300780)
#define I2C8_APDMA_BASE (0x11300900)
#define I2C9_APDMA_BASE (0x11300A80)
#define I2C10_APDMA_BASE (0x11300C00)
#define I2C11_APDMA_BASE (0x11300C80)
#define I2C12_APDMA_BASE (0x11300D00)
#define I2C13_APDMA_BASE (0x11300E80)
#define SCP_I2C0_APDMA_BASE (0x1c753080)
#define SCP_I2C1_CH1_APDMA_BASE (0x1c753180)
#define SCP_I2C2_APDMA_BASE (0x1c753280)
#define SCP_I2C3_APDMA_BASE (0x1c753380)
#define SCP_I2C4_APDMA_BASE (0x1c753480)
#define SCP_I2C5_APDMA_BASE (0x1c753580)
#define SCP_I2C6_APDMA_BASE (0x1c753600)

#define MODULE_CLK_SEL_BASE (0x10001098)
#define USE_PLL_MUX         0x800

#define PERICFG_I2C_ARBITRATION	(PERICFG_BASE+0x428)
#define INFRA_MODULE_CLK_SEL	(INFRACFG_AO_BASE+0x98)
#define I2C_CLK_SEL_OFST	(11)

#define I2C_OK                              0x0000
#define EAGAIN_I2C                          11  /* Try again */
#define EINVAL_I2C                          22  /* Invalid argument */
#define EOPNOTSUPP_I2C                      95  /* Operation not supported on transport endpoint */
#define ETIMEDOUT_I2C                       110 /* Connection timed out */
#define EREMOTEIO_I2C                       121 /* Remote I/O error */
#define ENOTSUPP_I2C                        524 /* Remote I/O error */
#define I2C_WRITE_FAIL_HS_NACKERR           0xA013
#define I2C_WRITE_FAIL_ACKERR               0xA014
#define I2C_WRITE_FAIL_TIMEOUT              0xA015
#define DUTY_CYCLE			    45

/*********DMA CON**********/
#define I2C_DMA_DIR_CHANGE              (0x1 << 9)
#define I2C_DMA_SKIP_CONFIG             (0x1 << 4)
#define I2C_DMA_ASYNC_MODE              (0x1 << 2)

/***For internal pull-up resister setting***/
#undef IOCFG_LT_BASE
#undef IOCFG_BR_BASE
#undef IOCFG_BL_BASE
#undef IOCFG_TL_BASE

#ifndef IOCFG_LT_BASE
#define IOCFG_LT_BASE			    0x11F30000
#endif
#ifndef IOCFG_BR_BASE
#define IOCFG_BR_BASE			    0x11D40000
#endif
#ifndef IOCFG_BL_BASE
#define IOCFG_BL_BASE			    0x11D10000
#endif
#ifndef IOCFG_TL_BASE
#define IOCFG_TL_BASE			    0x11F20000
#endif

/* the range of drving value is 0~7 */
#define MTK_SCP_I3C0_DRIVING   (0x2)
#define MTK_SCP_I3C2_DRIVING   (0x2)
#define MTK_SCP_I3C3_DRIVING   (0x2)
#define MTK_SCP_I3C4_DRIVING   (0x2)

#define I2C_SET_REG32(reg, field, val) \
    do { \
        uint32 tv = DRV_Reg32(reg); \
        tv &= ~(field); \
        tv |= (val); \
        DRV_WriteReg32(reg, tv); \
    } while(0)

#define SHADOW_REG_MODE           (1 << 1)
/******************************************register operation***********************************/
/* offset is different in mt6983 */
enum I2C_REGS_OFFSET {
  OFFSET_DATA_PORT      = 0x0,
  OFFSET_SLAVE_ADDR     = 0x94,
  OFFSET_INTR_MASK      = 0x08,
  OFFSET_INTR_STAT      = 0x0c,
  OFFSET_CONTROL        = 0x10,
  OFFSET_TRANSFER_LEN   = 0x14,
  OFFSET_TRANSAC_LEN    = 0x18,
  OFFSET_DELAY_LEN      = 0x1c,
  OFFSET_HTIMING         = 0x20,
  OFFSET_START          = 0x24,
  OFFSET_EXT_CONF       = 0x28,
  OFFSET_LTIMING        = 0x2c,
  OFFSET_HS             = 0x30,
  OFFSET_IO_CONFIG      = 0x34,
  OFFSET_FIFO_ADDR_CLR  = 0x38,
  OFFSET_TRANSFER_LEN_AUX = 0x44,
  OFFSET_CLOCK_DIV      = 0x48,
  OFFSET_SOFTRESET      = 0x50,
  OFFSET_DEBUGSTAT      = 0xe4,
  OFFSET_DEBUGCTRL      = 0xe8,
  OFFSET_FIFO_STAT      = 0xf4,
  OFFSET_FIFO_THRESH    = 0xf8,
  OFFSET_DCM_EN         = 0xf88,
  OFFSET_MULTI_DMA      = 0x8c,
  OFFSET_ROLLBACK       = 0xf98,

};

#define I2C_HS_NACKERR            (1 << 2)
#define I2C_ACKERR                (1 << 1)
#define I2C_TRANSAC_COMP          (1 << 0)

#define I2C_FIFO_SIZE             8

#define MAX_ST_MODE_SPEED         100  /* khz */
#define MAX_FS_MODE_SPEED         400  /* khz */
#define MAX_HS_MODE_SPEED         3400 /* khz */

#define MAX_DMA_TRANS_SIZE        65532 /* Max(65535) aligned to 4 bytes = 65532 */
#define MAX_DMA_TRANS_NUM         256

#define MAX_SAMPLE_CNT_DIV        8
#define MAX_STEP_CNT_DIV          64
#define MAX_HS_STEP_CNT_DIV       8

#define DMA_ADDRESS_HIGH          (0xC0000000)
#define DMA_DOMAIN_3           (3 << 20)
#define DMA_DOMAIN_LOCK           (1 << 30)

enum DMA_REGS_OFFSET {
  OFFSET_INT_FLAG       = 0x0,
  OFFSET_INT_EN         = 0x04,
  OFFSET_EN             = 0x08,
  OFFSET_RST            = 0x0C,
  OFFSET_CON            = 0x18,
  OFFSET_TX_MEM_ADDR    = 0x1C,
  OFFSET_RX_MEM_ADDR    = 0x20,
  OFFSET_TX_LEN         = 0x24,
  OFFSET_RX_LEN         = 0x28,
  OFFSET_SEC_DOMAIN         = 0x7C,
};

enum i2c_trans_st_rs {
    I2C_TRANS_STOP = 0,
    I2C_TRANS_REPEATED_START,
};

typedef enum {
     ST_MODE,
     FS_MODE,
     HS_MODE,
 }I2C_SPEED_MODE;

enum mt_trans_op {
  I2C_MASTER_NONE = 0,
  I2C_MASTER_WR = 1,
  I2C_MASTER_RD,
  I2C_MASTER_WRRD,
};

//CONTROL
#define I2C_CONTROL_RS                    (0x1 << 1)
#define I2C_CONTROL_DMA_EN                (0x1 << 2)
#define I2C_CONTROL_CLK_EXT_EN            (0x1 << 3)
#define I2C_CONTROL_DIR_CHANGE            (0x1 << 4)
#define I2C_CONTROL_ACKERR_DET_EN         (0x1 << 5)
#define I2C_CONTROL_TRANSFER_LEN_CHANGE   (0x1 << 6)
#define I2C_CONTROL_WRAPPER               (0x1 << 0)
/***********************************end of register operation****************************************/
/***********************************I2C Param********************************************************/
struct mt_trans_data {
  U16 trans_num;
  U16 data_size;
  U16 trans_len;
  U16 trans_auxlen;
};

typedef struct mt_i2c_t {
  //==========set in i2c probe============//
  U32      base;    /* i2c base addr */
  U16      id;
  U16      irqnr;    /* i2c interrupt number */
  U16      irq_stat; /* i2c interrupt status */
  U32      clk;     /* host clock speed in khz */
  U32      pdn;     /*clock number*/
  //==========common data define============//
  enum     i2c_trans_st_rs st_rs;
  enum     mt_trans_op op;
  U32      pdmabase;
  U32      speed;   //The speed (khz)
  U16      delay_len;    //number of half pulse between transfers in a trasaction
  U32      msg_len;    //number of half pulse between transfers in a trasaction
  U8       *msg_buf;    /* pointer to msg data      */
  U8       addr;      //The address of the slave device, 7bit,the value include read/write bit.
  U8       master_code;/* master code in HS mode */
  U8       mode;    /* ST/FS/HS mode */
  //==========reserved funtion============//
  U8       is_push_pull_enable; //IO push-pull or open-drain
  U8       is_clk_ext_disable;   //clk entend default enable
  U8       is_dma_enabled;   //Transaction via DMA instead of 8-byte FIFO
  U8       read_flag;//read,write,read_write
  BOOL     dma_en;
  BOOL     poll_en;
  BOOL     pushpull;//open drain
  BOOL     filter_msg;//filter msg error log
  BOOL     i2c_3dcamera_flag;//flag for 3dcamera

  //==========define reg============//
  U16      htiming_reg;
  U16      ltiming_reg;
  U16      high_speed_reg;
  U16      control_reg;
  struct   mt_trans_data trans_data;
}mt_i2c;

//===========================i2c old driver===================================================//
#define I2C_APPM I2C5
enum {
	I2C0 = 0,
	I2C1 = 1,
	I2C2 = 2,
	I2C3 = 3,
	I2C4 = 4,
	I2C5 = 5,
	I2C6 = 6,
	I2C7 = 7,
	I2C8 = 8,
	I2C9 = 9,
	I2C10 = 10,
	I2C11 = 11,
	I2C12 = 12,
	I2C13 = 13,
};

//==============================================================================
// I2C Exported Function
//==============================================================================
extern S32 i2c_read(mt_i2c *i2c,U8 *buffer, U32 len);
extern S32 i2c_write(mt_i2c *i2c,U8  *buffer, U32 len);
extern S32 i2c_write_read(mt_i2c *i2c,U8 *buffer, U32 write_len, U32 read_len);
extern S32 i2c_set_speed(mt_i2c *i2c);

//#define I2C_EARLY_PORTING
#ifdef I2C_EARLY_PORTING
int mt_i2c_test(void);
#endif
#endif /* __MT_I2C_H__ */
