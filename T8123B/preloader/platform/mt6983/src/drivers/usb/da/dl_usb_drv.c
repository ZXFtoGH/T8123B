/*******************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2013
*
*  History:
*  YYYYMMDD MTK12345 Description
*  --------------------
*  20131116 MTK81353 Delete unused code.
*
*
*******************************************************************************/
//#define USB_DBG_ON

#include "kal_release.h"
#include "register_act_util.h"
#include "usb_comm.h"
#include "usb_hw.h"					// related mu3d include file add here, 04404
#include "usb_drv.h"
#include "usb_drv_pri.h"
#include "usb_custom.h"
#include "usb_debug_tool.h"
#include "lib/string.h"

#ifndef CFG_FPGA_PLATFORM
#include "register_base.h"
#endif

//044045
#include "usb.h"			// struct for Usb_Device definition
#include "assert.h"

#include <debug.h>
//#include "dev/gpt_timer/gpt_timer.h"
extern void USB_Reset(void);

/* usb driver information*/
USB_Drv_Info g_UsbDrvInfo;
extern Usb_Device gUsbDevice;


static kal_bool g_bQMU_Init_Done = KAL_FALSE;
static kal_uint32 g_TX_FIFOadd = 0;
static kal_uint32 g_RX_FIFOadd = 0;
gpd_pool RX_GPD_POOL[MAX_EP_NUM+1], TX_GPD_POOL[MAX_EP_NUM+1];
kal_uint8 g_enable_hs = 1;

extern kal_bool gUSBAcm_IsQMUDone;
extern kal_bool gUSBAcm_IsZLPErr;
extern kal_bool gUSBAcm_IsAcmOpen;


#if 0
void dump_csr(void)	//04404_test
{
   LOGD("U3D_RX1CSR0 :%x\n",USB_ReadCsr32(U3D_RX1CSR0, 1));
   LOGD("U3D_RX1CSR1 :%x\n",USB_ReadCsr32(U3D_RX1CSR1, 1));
   LOGD("U3D_RX1CSR2 :%x\n",USB_ReadCsr32(U3D_RX1CSR2, 1));

   LOGD("U3D_TX1CSR0 :%x\n",USB_ReadCsr32(U3D_TX1CSR0, 1));
   LOGD("U3D_TX1CSR1 :%x\n",USB_ReadCsr32(U3D_TX1CSR1, 1));
   LOGD("U3D_TX1CSR2 :%x\n",USB_ReadCsr32(U3D_TX1CSR2, 1));

   LOGD("USB_QMU_TQCSR :%x\n",DRV_Reg32(USB_QMU_TQCSR(1)));
   LOGD("USB_QMU_RQCSR :%x\n",DRV_Reg32(USB_QMU_RQCSR(1)));

   LOGD("USB_QMU_RQSAR :%x\n",DRV_Reg32(USB_QMU_RQSAR(1)));
   LOGD("USB_QMU_TQSAR :%x\n",DRV_Reg32(USB_QMU_TQSAR(1)));

   LOGD("U3D_QGCSR : %x\n", DRV_Reg32(U3D_QGCSR));
   LOGD("U3D_QIESR0 : %x\n", DRV_Reg32(U3D_QIESR0));
   LOGD("U3D_QIESR1 : %x\n", DRV_Reg32(U3D_QIESR1));
   LOGD("U3D_EPIER : %x\n", DRV_Reg32(U3D_EPIER));
   LOGD("U3D_QCR0 : %x\n", DRV_Reg32(U3D_QCR0));
   LOGD("U3D_QCR1 : %x\n", DRV_Reg32(U3D_QCR1));
   LOGD("U3D_QCR2 : %x\n", DRV_Reg32(U3D_QCR2));
   LOGD("U3D_QCR3 : %x\n", DRV_Reg32(U3D_QCR3));
}
#endif

/************************************************************
driver info functions
*************************************************************/

kal_bool USB_IS_QMU_Init_Done(void)
{
   return g_bQMU_Init_Done;
}

kal_uint8 USB_QMU_Cal_Checksum(kal_uint8 *data, kal_uint32 len){

   kal_uint8 *uDataPtr, ckSum;
   kal_uint32 i;

   *(data + 1) = 0x0;
   uDataPtr = data;
   ckSum = 0;
   for (i = 0; i < len; i++){
      ckSum += *(uDataPtr + i);
   }
   return 0xFF - ckSum;
}

void USB_Stop_QMU(kal_int32 Q_num, USB_DIR dir){
   if(dir == USB_TX){
      if(!(DRV_Reg32(USB_QMU_TQCSR(Q_num)) & (QMU_Q_ACTIVE))){
         //LOGD("Tx %d inActive Now!\n", Q_num);
         return;
      }
      DRV_WriteReg32(USB_QMU_TQCSR(Q_num), QMU_Q_STOP);
      while((DRV_Reg32(USB_QMU_TQCSR(Q_num)) & (QMU_Q_ACTIVE)));
      //LOGD("Tx %d stop Now!\n", Q_num);
   }
   else if(dir == USB_RX){
      if(!(DRV_Reg32(USB_QMU_RQCSR(Q_num))&QMU_Q_ACTIVE)){
         //LOGD("Rx %d inActive Now!\n", Q_num);
         return;
      }
      DRV_WriteReg32(USB_QMU_RQCSR(Q_num), QMU_Q_STOP);
      while((DRV_Reg32(USB_QMU_RQCSR(Q_num)) & (QMU_Q_ACTIVE)));
      //LOGD("Rx %d stop Now!\n", Q_num);
   }

}



void USB_Start_QMU(kal_int32 Q_num, USB_DIR dir){

   kal_uint32 QCR;
   if(dir == USB_RX)
   {

      USB_WriteCsr32(U3D_RX1CSR0, Q_num, (USB_ReadCsr32(U3D_RX1CSR0, Q_num)& ~RX_RXPKTRDY) |(RX_DMAREQEN) );
      QCR = DRV_Reg32(U3D_QCR0);
      DRV_WriteReg32(U3D_QCR0, QCR|QMU_RX_CS_EN(Q_num));

#ifdef CFG_RX_ZLP_EN
      QCR = DRV_Reg32(U3D_QCR3);
      DRV_WriteReg32(U3D_QCR3, QCR|QMU_RX_ZLP(Q_num));
#else
      QCR = DRV_Reg32(U3D_QCR3);
      DRV_WriteReg32(U3D_QCR3, QCR&~(QMU_RX_ZLP(Q_num)));
#endif


#ifdef CFG_RX_COZ_EN
      QCR = DRV_Reg32(U3D_QCR3);
      DRV_WriteReg32(U3D_QCR3, QCR|QMU_RX_COZ(Q_num));
#else
      QCR = DRV_Reg32(U3D_QCR3);
      DRV_WriteReg32(U3D_QCR3, QCR&~(QMU_RX_COZ(Q_num)));
#endif

      DRV_WriteReg32(U3D_QEMIESR, DRV_Reg32(U3D_QEMIESR) | QMU_RX_EMPTY(Q_num));
      DRV_WriteReg32(U3D_RQERRIESR0, QMU_RX_LEN_ERR(Q_num)|QMU_RX_CS_ERR(Q_num));
      DRV_WriteReg32(U3D_RQERRIESR1, QMU_RX_EP_ERR(Q_num)|QMU_RX_ZLP_ERR(Q_num));



      if(DRV_Reg32(USB_QMU_RQCSR(Q_num))&QMU_Q_ACTIVE){
         LOGD("Rx %d Active Now!\n", Q_num);
         return;
      }

      DRV_WriteReg32(USB_QMU_RQCSR(Q_num), QMU_Q_START);
   }
   else		// TX FLOW
   {
      kal_uint32 txcsr, Temp;
      txcsr = USB_ReadCsr32(U3D_TX1CSR0, Q_num) & 0xFFFEFFFF;
      USB_WriteCsr32(U3D_TX1CSR0, Q_num, txcsr | TX_DMAREQEN);
      QCR = DRV_Reg32(U3D_QCR0);
      DRV_WriteReg32(U3D_QCR0, QCR|QMU_TX_CS_EN(Q_num));
#if (TXZLP==HW_MODE)
      QCR = DRV_Reg32(U3D_QCR1);
      DRV_WriteReg32(U3D_QCR1, QCR &~ QMU_TX_ZLP(Q_num));
      QCR = DRV_Reg32(U3D_QCR2);
      DRV_WriteReg32(U3D_QCR2, QCR|QMU_TX_ZLP(Q_num));
#elif (TXZLP==GPD_MODE)
      QCR = DRV_Reg32(U3D_QCR1);
      DRV_WriteReg32(U3D_QCR1, QCR|QMU_TX_ZLP(Q_num));
#endif
      DRV_WriteReg32(U3D_QEMIESR, DRV_Reg32(U3D_QEMIESR) | QMU_TX_EMPTY(Q_num));
      DRV_WriteReg32(U3D_TQERRIESR0, QMU_TX_LEN_ERR(Q_num)|QMU_TX_CS_ERR(Q_num));
      Temp = DRV_Reg32(USB_QMU_TQCSR(Q_num));
      //LOGD("USB_QMU_TQCSR:0x%X\n", Temp);

      if(DRV_Reg32(USB_QMU_TQCSR(Q_num))&QMU_Q_ACTIVE){
         //LOGD("Tx %d Active Now!\n", Q_num);
         return;
      }

      DRV_WriteReg32(USB_QMU_TQCSR(Q_num), QMU_Q_START);
   }

#if (CHECKSUM_TYPE==CS_16B)
   DRV_WriteReg32(U3D_QCR0, DRV_Reg32(U3D_QCR0)|CS16B_EN);
#else
   DRV_WriteReg32(U3D_QCR0, DRV_Reg32(U3D_QCR0)&~CS16B_EN);
#endif

}

void USB_Restart_QMU(kal_int32 Q_num, USB_DIR dir){

   kal_uint32 ep_rst;

   if(dir == USB_TX){
      ep_rst = BIT16<<Q_num;
      //LOGD("ep_rst : [%x] \n",ep_rst);
      DRV_WriteReg32(U3D_EP_RST, ep_rst);
      //LOGD("ep_rst \n");
      mdelay(1);
      DRV_WriteReg32(U3D_EP_RST, 0);
      //LOGD("clear ep_rst \n");

   }
   else{
      ep_rst = 1<<Q_num;
      DRV_WriteReg32(U3D_EP_RST, ep_rst);
      mdelay(1);
      DRV_WriteReg32(U3D_EP_RST, 0);
   }
   USB_Start_QMU(Q_num, dir);

}
void USB_QMU_Resource_reset(kal_int32 Q_num, USB_DIR dir)
{
   kal_uint32 uQ_num = (kal_uint32)Q_num;
   if(dir == USB_RX)
   {

      RX_GPD_POOL[uQ_num].cur_ptr_gpd= &(RX_GPD_POOL[uQ_num].gpd[0]);
      RX_GPD_POOL[uQ_num].cur_gpd_index = 0;

      arch_clean_invalidate_cache_range((addr_t)&RX_GPD_POOL[uQ_num], sizeof(RX_GPD_POOL));
      TGPD_CLR_FLAGS_HWO(RX_GPD_POOL[uQ_num].cur_ptr_gpd);
      DRV_WriteReg32(USB_QMU_RQSAR(uQ_num), &(RX_GPD_POOL[uQ_num].gpd[0]));
   }
   else
   {

      TX_GPD_POOL[uQ_num].cur_ptr_gpd= &(TX_GPD_POOL[uQ_num].gpd[0]);
      TX_GPD_POOL[uQ_num].cur_gpd_index = 0;

      arch_clean_invalidate_cache_range((addr_t)&TX_GPD_POOL[uQ_num], sizeof(TX_GPD_POOL));
      TGPD_CLR_FLAGS_HWO(TX_GPD_POOL[uQ_num].cur_ptr_gpd);
      DRV_WriteReg32(USB_QMU_TQSAR(uQ_num), &(TX_GPD_POOL[uQ_num].gpd[0]));
   }

}

kal_bool USB_Is_QMU_Enabled(void)
{
   kal_uint32 QCR;
   QCR = DRV_Reg32(U3D_QGCSR);
   LOGD("QMU QCR : %x\n", QCR);
   if(QCR == 0)
      return KAL_FALSE;
   else
      return KAL_TRUE;
}
void USB_QMU_Init(void)
{
   kal_uint32 i;
   kal_uint32 QCR = 0;

   LOGD("USB_QMU_Init begin, QMU QCR : %x\n", DRV_Reg32(U3D_QGCSR));
   for(i=1; i<=MAX_EP_NUM; i++){

      USB_QMU_Resource_reset(i, USB_RX);
      QCR|=QMU_RX_EN(i);

      USB_QMU_Resource_reset(i, USB_TX);
      QCR|=QMU_TX_EN(i);

   }

   /* Enable QMU Tx/Rx. */
   DRV_WriteReg32(U3D_QGCSR, QCR);
   DRV_WriteReg32(U3D_QIESR0, QCR);

   /* Enable QMU interrupt. */
   DRV_WriteReg32(U3D_QIESR1, TXQ_EMPTY_IESR|TXQ_CSERR_IESR|TXQ_LENERR_IESR|RXQ_EMPTY_IESR|RXQ_CSERR_IESR|RXQ_LENERR_IESR|RXQ_ZLPERR_IESR);
   DRV_WriteReg32(U3D_EPIESR, EP0ISR);

   LOGD("USB_QMU_Init end, QMU QCR : %x\n", DRV_Reg32(U3D_QGCSR));
}

void USB_EP_Disable_InterR(kal_uint8 epnum,  USB_DIR dir);
void USB_QMU_Flush_and_Restart(void)
{
   kal_uint32 i;
   for(i=1; i<=MAX_EP_NUM; i++){
      USB_Stop_QMU(i, USB_RX);
      USB_QMU_Resource_reset(i, USB_RX);
      USB_Restart_QMU(i, USB_RX);

      USB_Stop_QMU(i, USB_TX);
      USB_QMU_Resource_reset(i, USB_TX);
      USB_Restart_QMU(i, USB_TX);
   }

   for(i=1; i<=MAX_EP_NUM; i++){
      USB_EP_Disable_InterR((kal_uint8)i, USB_RX);
      USB_EP_Disable_InterR((kal_uint8)i, USB_TX);
   }

   g_bQMU_Init_Done = KAL_TRUE;
}



void USB_QMU_Init_and_Start(void)
{
#if ( BUS_MODE == QMU_MODE )
   kal_uint32 i;

   //LOGD("USB_QMU_Init_and_Start\n");

   if(g_bQMU_Init_Done == KAL_TRUE)
   {
      LOGD("QMU already inited by DA\n");
      return;
   }

   USB_QMU_Init();
   for(i=1; i<=MAX_EP_NUM; i++){
      USB_Start_QMU(i, USB_RX);
      USB_Start_QMU(i, USB_TX);
   }

   for(i=1; i<=MAX_EP_NUM; i++){
      USB_EP_Disable_InterR((kal_uint8)i, USB_RX);
      USB_EP_Disable_InterR((kal_uint8)i, USB_TX);
   }

   g_bQMU_Init_Done = KAL_TRUE;
#else
   if(0)
      LOGD("USB_QMU_Init_and_Start skipped, BUS_MODE != QMU_MODE\n");
#endif
}


void USB_QMU_Prepare_GPD(kal_uint8 ep_num, kal_uint8* pBuf, kal_uint32 length, USB_DIR dir)
{
   if(dir == USB_RX)
   {
      TGPD *gpd = RX_GPD_POOL[ep_num].cur_ptr_gpd;
      arch_clean_invalidate_cache_range((addr_t)gpd, sizeof(TGPD));
      arch_clean_invalidate_cache_range((addr_t)pBuf, length);
      TGPD_SET_DATA(gpd, pBuf);
      TGPD_CLR_FORMAT_BDP(gpd);

      TGPD_SET_DataBUF_LEN(gpd, length);
      TGPD_SET_BUF_LEN(gpd, 0);


      TGPD_CLR_FORMAT_BPS(gpd);
      TGPD_SET_FORMAT_IOC(gpd);


      /* get next gpd , i.e. call get_gpd*/
      if(RX_GPD_POOL[ep_num].cur_gpd_index == 0)
      {
         RX_GPD_POOL[ep_num].cur_ptr_gpd = &(RX_GPD_POOL[ep_num].gpd[1]);
         RX_GPD_POOL[ep_num].cur_gpd_index = 1;
      }
      else
      {
         RX_GPD_POOL[ep_num].cur_ptr_gpd = &(RX_GPD_POOL[ep_num].gpd[0]);
         RX_GPD_POOL[ep_num].cur_gpd_index = 0;
      }

      memset(RX_GPD_POOL[ep_num].cur_ptr_gpd, 0 , sizeof(TGPD));
      arch_clean_invalidate_cache_range((addr_t)RX_GPD_POOL[ep_num].cur_ptr_gpd, sizeof(TGPD));
      TGPD_CLR_FLAGS_HWO(RX_GPD_POOL[ep_num].cur_ptr_gpd);
      TGPD_SET_NEXT(gpd, RX_GPD_POOL[ep_num].cur_ptr_gpd);

      TGPD_SET_CHKSUM(gpd, CHECKSUM_LENGTH);
      TGPD_SET_FLAGS_HWO(gpd);
      arch_clean_invalidate_cache_range((addr_t)gpd, sizeof(TGPD));


   }
   else // TX FLOW
   {
      TGPD *gpd = TX_GPD_POOL[ep_num].cur_ptr_gpd;
      arch_clean_invalidate_cache_range((addr_t)gpd, sizeof(TGPD));
      arch_clean_invalidate_cache_range((addr_t)pBuf, length);

      TGPD_SET_DATA(gpd, pBuf);
      TGPD_CLR_FORMAT_BDP(gpd);

      TGPD_SET_BUF_LEN(gpd, length);
      TGPD_SET_EXT_LEN(gpd, 0);


      TGPD_SET_FORMAT_ZLP(gpd);
      TGPD_CLR_FORMAT_BPS(gpd);
      TGPD_SET_FORMAT_IOC(gpd);

      /* get next gpd , i.e. call get_gpd*/
      if(TX_GPD_POOL[ep_num].cur_gpd_index == 0)
      {
         TX_GPD_POOL[ep_num].cur_ptr_gpd = &(TX_GPD_POOL[ep_num].gpd[1]);
         TX_GPD_POOL[ep_num].cur_gpd_index = 1;
      }
      else
      {
         TX_GPD_POOL[ep_num].cur_ptr_gpd = &(TX_GPD_POOL[ep_num].gpd[0]);
         TX_GPD_POOL[ep_num].cur_gpd_index = 0;
      }

      memset(TX_GPD_POOL[ep_num].cur_ptr_gpd, 0 , sizeof(TGPD));
      arch_clean_invalidate_cache_range((addr_t)TX_GPD_POOL[ep_num].cur_ptr_gpd, sizeof(TGPD));
      TGPD_CLR_FLAGS_HWO(TX_GPD_POOL[ep_num].cur_ptr_gpd);
      TGPD_SET_NEXT(gpd, TX_GPD_POOL[ep_num].cur_ptr_gpd);

      TGPD_SET_CHKSUM(gpd, CHECKSUM_LENGTH);
      TGPD_SET_FLAGS_HWO(gpd);
      arch_clean_invalidate_cache_range((addr_t)gpd, sizeof(TGPD));
   }

}


void USB_QMU_Resume(kal_uint8 ep_num, USB_DIR dir)
{
   if(dir == USB_RX)
   {
      DRV_WriteReg32(USB_QMU_RQCSR(ep_num), QMU_Q_RESUME);
      if(!DRV_Reg32( USB_QMU_RQCSR(ep_num))){
         DRV_WriteReg32( USB_QMU_RQCSR(ep_num), QMU_Q_RESUME);
      }
   }
   else	// TX FLOW
   {
      DRV_WriteReg32(USB_QMU_TQCSR(ep_num), QMU_Q_RESUME);
      if(!DRV_Reg32( USB_QMU_TQCSR(ep_num))){
         DRV_WriteReg32( USB_QMU_TQCSR(ep_num), QMU_Q_RESUME);
      }
   }
}

void USB_Poll_QMU_Done(void)
{
   kal_uint32 wIntrQMUDoneValue, wIntrQMUValue;
   kal_bool qmu_poll_done = KAL_FALSE;

   while(1)
   {
      wIntrQMUDoneValue = DRV_Reg32(U3D_QISAR0) & DRV_Reg32(U3D_QIER0);
      /* clear interrupt*/
      DRV_WriteReg32(U3D_QISAR0, wIntrQMUDoneValue);


      wIntrQMUValue = DRV_Reg32(U3D_QISAR1);

      if( (wIntrQMUValue & RXQ_CSERR_INT) || (wIntrQMUValue & RXQ_LENERR_INT))
      {
         kal_uint32 wErrVal = DRV_Reg32(U3D_RQERRIR0);
         DRV_WriteReg32(U3D_RQERRIR0, wErrVal);
         qmu_poll_done = KAL_TRUE;			//04404_test
      }

      if( (wIntrQMUValue & RXQ_ZLPERR_INT) )
      {
         kal_uint32 wErrVal = DRV_Reg32(U3D_RQERRIR1);
         DRV_WriteReg32(U3D_RQERRIR1, wErrVal);
         qmu_poll_done = KAL_TRUE;					//04404_test
      }

      if( (wIntrQMUValue & RXQ_EMPTY_INT) || (wIntrQMUValue & TXQ_EMPTY_INT))
      {
         kal_uint32 wEmptyVal = DRV_Reg32(U3D_QEMIR);
         DRV_WriteReg32(U3D_QEMIR, wEmptyVal);
      }

      if(wIntrQMUDoneValue)
      {
         qmu_poll_done = KAL_TRUE;

      }

      if(qmu_poll_done == KAL_TRUE)
      {
         break;
      }
   }
}


kal_uint32 USB_QMU_Dump_RecvLen(void)
{
   kal_uint32 recivedlength;
   TGPD *last_gpd;

   last_gpd = &(RX_GPD_POOL[1].gpd[1]);
   recivedlength = (kal_uint32)TGPD_GET_BUF_LEN(last_gpd);
   //LOGD("recivedlength RX_GPD_POOL[ep_num].gpd[1]: %d\n", recivedlength);

   last_gpd = &(RX_GPD_POOL[1].gpd[0]);
   recivedlength = (kal_uint32)TGPD_GET_BUF_LEN(last_gpd);
   //LOGD("recivedlength RX_GPD_POOL[ep_num].gpd[0]: %d\n", recivedlength);

   return recivedlength;
}
kal_uint32 USB_QMU_Update_Gpd(kal_uint8 ep_num)
{
   kal_uint32 recivedlength;
   TGPD *last_gpd;

   if(RX_GPD_POOL[ep_num].cur_gpd_index == 0)
   {
      last_gpd = &(RX_GPD_POOL[ep_num].gpd[1]);
   }
   else
   {
      last_gpd = &(RX_GPD_POOL[ep_num].gpd[0]);
   }

   recivedlength = (kal_uint32)TGPD_GET_BUF_LEN(last_gpd);


   return recivedlength;
}

kal_uint32 USB_QMU_Read(kal_uint8 ep_num, kal_uint8* addr, kal_uint32 length)
{
   kal_uint32 recivedlength;
   kal_uint8 *ptr;
   kal_uint32 len;
   TGPD *last_gpd;

#if 0						//04404_test
#ifndef CFG_RX_ZLP_EN
   /* may get ZLPERR intr becoz last gpd, clear it */
   kal_uint32 wIntrQMUValue;
   wIntrQMUValue = DRV_Reg32(U3D_QISAR1);
   if(wIntrQMUValue)
      DRV_WriteReg32(U3D_QISAR1, wIntrQMUValue);
#endif
#endif
   len = length;
   ptr = addr;
   USB_QMU_Prepare_GPD(ep_num, ptr, len, USB_RX);
   USB_QMU_Resume(ep_num, USB_RX);
   USB_Poll_QMU_Done();

   if(RX_GPD_POOL[ep_num].cur_gpd_index == 0)
   {
      last_gpd = &(RX_GPD_POOL[ep_num].gpd[1]);
   }
   else
   {
      last_gpd = &(RX_GPD_POOL[ep_num].gpd[0]);
   }

   recivedlength = (kal_uint32)TGPD_GET_BUF_LEN(last_gpd);

   return recivedlength;
}

kal_uint32 USB_QMU_Write(kal_uint8 ep_num, kal_uint8* addr, kal_uint32 length)
{
   kal_uint8 *ptr;
   kal_uint32 len;

   len = length;
   ptr = addr;


   //LOGD("USB_QMU_Write, ep_num : %d, addr : %x, length : %d\n", ep_num, addr, length);//04404_test
   USB_QMU_Prepare_GPD(ep_num, ptr, len, USB_TX);
   USB_QMU_Resume(ep_num, USB_TX);
   USB_Poll_QMU_Done();

   // must write success ???
   return length;
}


void USB_QMU_Check_and_Set(void)
{
   if(USB_Is_QMU_Enabled() == KAL_TRUE)
   {
      LOGD("QMU alread enabled by BROM/PL, perform USB_QMU_Flush_and_Restart\n");
      USB_QMU_Flush_and_Restart();
      g_bQMU_Init_Done = KAL_TRUE;
   }
   else
   {
      LOGD("QMU not enabled by BROM/PL\n");
   }
}
void USB_Enable_HS(bool enable_hs)
{
   g_enable_hs = enable_hs ? 1 : 0;
}

void USB_Reset_FIFO(void)
{
   g_TX_FIFOadd = USB_TX_FIFO_START_ADDRESS;
   g_RX_FIFOadd = USB_RX_FIFO_START_ADDRESS;
}

void USB_Enable_DMA(bool enabled)
{
   //g_enable_dma = enabled;
}

/* initialize usb driver information*/
void USB_Init_Drv_Info(void)
{
   kal_uint32 index;

   USB_Dbg_Trace(DEBUG_USB_Init_Drv_Info, 0, 0,0);

   g_UsbDrvInfo.power_state = USB_STATE_NORMAL;
   g_UsbDrvInfo.is_unMaskUSB = KAL_TRUE;
   g_UsbDrvInfo.reset_hdlr = NULL;
   g_UsbDrvInfo.ep0_hdlr = NULL;

   for(index=0; index< MAX_INTR_EP_NUM; index++)
   {
      g_UsbDrvInfo.ep_in_hdlr[index] = NULL;
      g_UsbDrvInfo.ep_out_hdlr[index] = NULL;
      g_UsbDrvInfo.ep_in_stall_status[index] = KAL_FALSE;
      g_UsbDrvInfo.ep_out_stall_status[index] = KAL_FALSE;
   }
}

void USB_ResetDrv_Info(void)
{
   kal_uint32 index;

   g_UsbDrvInfo.power_state = USB_STATE_NORMAL;
   g_UsbDrvInfo.is_unMaskUSB = KAL_TRUE;

   for(index=0; index< MAX_INTR_EP_NUM; index++)
   {
      g_UsbDrvInfo.ep_in_stall_status[index] = KAL_FALSE;
      g_UsbDrvInfo.ep_out_stall_status[index] = KAL_FALSE;
   }
}

/* register interrupt handler*/
void USB_Register_Drv_Info(USB_DRV_HDLR_TYPE type, kal_uint32 ep_num, usb_drv_intr_handler_ptr hdlr)
{
   switch(type)
   {
   case USB_DRV_HDLR_RESET:
      g_UsbDrvInfo.reset_hdlr = hdlr;
      break;
   case USB_DRV_HDLR_EP0:
      g_UsbDrvInfo.ep0_hdlr = hdlr;
      break;
   case USB_DRV_HDLR_EP_IN:
      g_UsbDrvInfo.ep_in_hdlr[ep_num-1]= hdlr;
      break;
   case USB_DRV_HDLR_EP_OUT:
      g_UsbDrvInfo.ep_out_hdlr[ep_num-1]= hdlr;
      break;
   }
}



/* get stall status for endpoint */
kal_bool USB_Get_EP_Stall_Status(kal_uint8 ep_num, USB_EP_TYPE type)
{
   if(type == USB_IN_EP_TYPE)
   {
      return g_UsbDrvInfo.ep_in_stall_status[ep_num-1];
   }
   else
   {
      return g_UsbDrvInfo.ep_out_stall_status[ep_num-1];
   }
}


void USB_Set_DP_Pull_High(kal_bool bset)
{
   LOGD("USB_Set_DP_Pull_High, %d\n", bset); //04404_test
   if(bset == KAL_TRUE)	/* connect */
   {
#ifdef SUPPORT_U3
      DRV_WriteReg32(U3D_POWER_MANAGEMENT, (DRV_Reg32(U3D_POWER_MANAGEMENT) | HS_ENABLE) & ~SOFT_CONN );

      /*enable u3 port */
      DRV_WriteReg32(U3D_USB3_CONFIG, USB3_EN);

#else
      /* HS or FS flow, choose one ?*/
      DRV_WriteReg32(U3D_POWER_MANAGEMENT, (DRV_Reg32(U3D_POWER_MANAGEMENT) | HS_ENABLE) | SOFT_CONN );
      //DRV_WriteReg32(U3D_POWER_MANAGEMENT, (DRV_Reg32(U3D_POWER_MANAGEMENT) & ~HS_ENABLE) | SOFT_CONN );

      /* disalbe u3 port */
      DRV_WriteReg32(U3D_USB3_CONFIG, 0);
#endif

      /* U2 U3 detected by HW *, DEVICE_CONF.USB_3_SEL_EN = 0, from programming guide */
      DRV_WriteReg32(U3D_DEVICE_CONF, 0);
   }
   else	/* disconnect */
   {
#ifdef SUPPORT_U3
      /* disconnect u2*/
      DRV_WriteReg32(U3D_POWER_MANAGEMENT, DRV_Reg32(U3D_POWER_MANAGEMENT) & ~SOFT_CONN & ~HS_ENABLE);

      /* disconnect u3 */
      DRV_WriteReg32(U3D_USB3_CONFIG, 0);			//04404_test
#else
      /* disconnect u2*/
      DRV_WriteReg32(U3D_POWER_MANAGEMENT, DRV_Reg32(U3D_POWER_MANAGEMENT) & ~SOFT_CONN & ~HS_ENABLE);

#endif
   }
}

/************************************************************
system ctrl functions
*************************************************************/


/* U3 api */
int wait_for_value(kal_int32 addr, kal_int32 msk, kal_int32 value, kal_int32 ms_intvl, kal_int32 count)
{
   unsigned int i;

   for (i = 0; i < count; i++)
   {
      if ((DRV_Reg32(addr) & msk) == value)
         return RET_SUCCESS;
      mdelay(ms_intvl);
   }

   return RET_FAIL;
}

kal_int32 mu3d_hal_check_clk_sts(void){
   kal_int32 ret;

   LOGD("mu3d_hal_check_clk_sts\n");

   ret = wait_for_value(U3D_SSUSB_IP_PW_STS1, SSUSB_SYS125_RST_B_STS, SSUSB_SYS125_RST_B_STS, 1, 10);
   if (ret == RET_FAIL)
   {
      LOGD("SSUSB_SYS125_RST_B_STS NG\n");
      return RET_FAIL;
   }

   ret = wait_for_value(U3D_SSUSB_IP_PW_STS1, SSUSB_U3_MAC_RST_B_STS, SSUSB_U3_MAC_RST_B_STS, 1, 10);
   if (ret == RET_FAIL)
   {
      LOGD("SSUSB_U3_MAC_RST_B_STS NG\n");
      return RET_FAIL;
   }

   ret = wait_for_value(U3D_SSUSB_IP_PW_STS2, SSUSB_U2_MAC_SYS_RST_B_STS, SSUSB_U2_MAC_SYS_RST_B_STS, 1, 10);
   if (ret == RET_FAIL)
   {
      LOGD("SSUSB_U2_MAC_SYS_RST_B_STS NG\n");
      return RET_FAIL;
   }

   LOGD("check clk pass!!\n");
   return RET_SUCCESS;
}


void check_rst_value(void)			//04404_test, add for monitoring
{
#if 1
   kal_uint32 ret;

   ret = DRV_Reg32(U3D_SSUSB_IP_PW_CTRL0);
   LOGD("U3D_SSUSB_IP_PW_CTRL0 : %x, ret : %x\n", U3D_SSUSB_IP_PW_CTRL0, ret);

   ret = DRV_Reg32(U3D_SSUSB_IP_PW_CTRL1);
   LOGD("U3D_SSUSB_IP_PW_CTRL1 : %x, ret : %x\n", U3D_SSUSB_IP_PW_CTRL1, ret);


   ret = DRV_Reg32(U3D_SSUSB_IP_PW_CTRL2);
   LOGD("U3D_SSUSB_IP_PW_CTRL2 : %x, ret : %x\n", U3D_SSUSB_IP_PW_CTRL2, ret);

   ret = DRV_Reg32(U3D_SSUSB_IP_PW_CTRL3);
   LOGD("U3D_SSUSB_IP_PW_CTRL3 : %x, ret : %x\n", U3D_SSUSB_IP_PW_CTRL3, ret);

   ret = DRV_Reg32(U3D_SSUSB_IP_PW_STS1);
   LOGD("U3D_SSUSB_IP_PW_STS1 : %x, ret : %x\n", U3D_SSUSB_IP_PW_STS1, ret);

   ret = DRV_Reg32(U3D_SSUSB_IP_PW_STS2);
   LOGD("U3D_SSUSB_IP_PW_STS2 : %x, ret : %x\n", U3D_SSUSB_IP_PW_STS2, ret);
#endif


}
/* reset hw power*/
void mu3d_hal_rst_dev(void){
   kal_int32 ret;

   DRV_WriteReg32(U3D_SSUSB_DEV_RST_CTRL, SSUSB_DEV_SW_RST);
   DRV_WriteReg32(U3D_SSUSB_DEV_RST_CTRL, 0);

   //do not check when SSUSB_U2_PORT_DIS = 1, because U2 port stays in reset state
   if (!(DRV_Reg32(U3D_SSUSB_U2_CTRL_0P) & SSUSB_U2_PORT_DIS))
   {
      ret = wait_for_value(U3D_SSUSB_IP_PW_STS2, SSUSB_U2_MAC_SYS_RST_B_STS, SSUSB_U2_MAC_SYS_RST_B_STS, 1, 10);
      if (ret == RET_FAIL)
         LOGD("SSUSB_U2_MAC_SYS_RST_B_STS NG\n");
   }

#ifdef SUPPORT_U3
   //do not check when SSUSB_U3_PORT_PDN = 1, because U3 port stays in reset state
   if (!(DRV_Reg32(U3D_SSUSB_U3_CTRL_0P) & SSUSB_U3_PORT_PDN))
   {
      ret = wait_for_value(U3D_SSUSB_IP_PW_STS1, SSUSB_U3_MAC_RST_B_STS, SSUSB_U3_MAC_RST_B_STS, 1, 10);
      if (ret == RET_FAIL)
         LOGD("SSUSB_U3_MAC_RST_B_STS NG\n");
   }
#endif

   ret = wait_for_value(U3D_SSUSB_IP_PW_STS1, SSUSB_DEV_QMU_RST_B_STS, SSUSB_DEV_QMU_RST_B_STS, 1, 10);
   if (ret == RET_FAIL)
      LOGD("SSUSB_DEV_QMU_RST_B_STS NG\n");

   ret = wait_for_value(U3D_SSUSB_IP_PW_STS1, SSUSB_DEV_BMU_RST_B_STS, SSUSB_DEV_BMU_RST_B_STS, 1, 10);
   if (ret == RET_FAIL)
      LOGD("SSUSB_DEV_BMU_RST_B_STS NG\n");

   ret = wait_for_value(U3D_SSUSB_IP_PW_STS1, SSUSB_DEV_RST_B_STS, SSUSB_DEV_RST_B_STS, 1, 10);
   if (ret == RET_FAIL)
      LOGD("SSUSB_DEV_RST_B_STS NG\n");
}

/**
* mu3d_hal_ssusb_en - disable ssusb power down & enable u2/u3 ports
*
*/
void mu3d_hal_ssusb_en(void){

   /* If SUPPORT_U3, set VA09 on */
#ifdef SUPPORT_U3
   USB_SetMsk(U3D_USB30_PHYA_REG1, RG_VA09_ON);
#else
   USB_ClrMsk(U3D_USB30_PHYA_REG1, RG_VA09_ON);
#endif

   USB_ClrMsk(U3D_SSUSB_IP_PW_CTRL0, SSUSB_IP_SW_RST);
   USB_ClrMsk(U3D_SSUSB_IP_PW_CTRL2, SSUSB_IP_DEV_PDN);

#if 1//#ifdef SUPPORT_U3, // MUST DO THIS, even only want to use u3, from designer
   USB_ClrMsk(U3D_SSUSB_U3_CTRL_0P, (SSUSB_U3_PORT_DIS | SSUSB_U3_PORT_PDN | SSUSB_U3_PORT_HOST_SEL));
#endif
   USB_ClrMsk(U3D_SSUSB_U2_CTRL_0P, (SSUSB_U2_PORT_DIS | SSUSB_U2_PORT_PDN | SSUSB_U2_PORT_HOST_SEL));

   USB_SetMsk(U3D_SSUSB_REF_CK_CTRL, (SSUSB_REF_MAC_CK_GATE_EN | SSUSB_REF_PHY_CK_GATE_EN | SSUSB_REF_CK_GATE_EN | SSUSB_REF_MAC3_CK_GATE_EN));

   /* check U3D sys125,u3 mac,u2 mac clock status. */
   mu3d_hal_check_clk_sts();
}
/**
* mu3d_hal_u2dev_connect - u2 device softconnect
*
*/
void mu3d_hal_u2dev_connect(){
   DRV_WriteReg32(U3D_POWER_MANAGEMENT, DRV_Reg32(U3D_POWER_MANAGEMENT) | SOFT_CONN);
   //04404
   //DRV_WriteReg32(U3D_POWER_MANAGEMENT, DRV_Reg32(U3D_POWER_MANAGEMENT) | SUSPENDM_ENABLE);
}

void mu3d_hal_u2dev_disconn(){
   DRV_WriteReg32(U3D_POWER_MANAGEMENT, DRV_Reg32(U3D_POWER_MANAGEMENT) & ~SOFT_CONN);
   //04404
   //DRV_WriteReg32(U3D_POWER_MANAGEMENT, DRV_Reg32(U3D_POWER_MANAGEMENT) & ~SUSPENDM_ENABLE);
}

/**
* mu3d_hal_u3dev_en - enable U3D ss dev
*
*/
void mu3d_hal_u3dev_en(void){
   DRV_WriteReg32(U3D_USB3_CONFIG, USB3_EN);
}


/**
* mu3d_hal_set_speed - enable ss or connect to hs/fs
*@args - arg1: speed
*/
void mu3d_hal_set_speed(USB_SPEED speed){
#ifndef EXT_VBUS_DET
   DRV_WriteReg32(U3D_MISC_CTRL, 0);
#else
   DRV_WriteReg32(U3D_MISC_CTRL, 0x3);
#endif

   /* clear ltssm state*/
   if(speed == SSUSB_SPEED_FULL){
      DRV_WriteReg32(U3D_POWER_MANAGEMENT, DRV_Reg32(U3D_POWER_MANAGEMENT) & ~HS_ENABLE);
      mu3d_hal_u2dev_connect();
      //g_u3d_setting.speed = SSUSB_SPEED_FULL;
   }
   else if(speed == SSUSB_SPEED_HIGH){
      DRV_WriteReg32(U3D_POWER_MANAGEMENT, DRV_Reg32(U3D_POWER_MANAGEMENT) | HS_ENABLE);
      mu3d_hal_u2dev_connect();
      //g_u3d_setting.speed = SSUSB_SPEED_HIGH;
   }
#ifdef SUPPORT_U3
   else if(speed == SSUSB_SPEED_SUPER){
      //g_u3d_setting.speed = SSUSB_SPEED_SUPER;
      mu3d_hal_u2dev_disconn();
      mu3d_hal_u3dev_en();
   }
#endif
   else{
      LOGD("Unsupported speed!!\n");
      ASSERT(0);
   }
}

/**
* mu3d_hal_det_speed - detect device speed
*@args - arg1: speed
*/
void mu3d_hal_det_speed(USB_SPEED speed, kal_uint8 det_speed){
   kal_uint8 temp;
   kal_uint16 cnt_down = 50;
   int time_count = 0;
   int timeout_s = 5;

#ifdef EXT_VBUS_DET
   if (!det_speed)
   {
      while (!(DRV_Reg32(U3D_DEV_LINK_INTR) & SSUSB_DEV_SPEED_CHG_INTR))
      {
         mdelay(1);
         cnt_down--;

         if (cnt_down == 0)
            return;
      }
   }
   else
   {
      while(!(DRV_Reg32(U3D_DEV_LINK_INTR) & SSUSB_DEV_SPEED_CHG_INTR));
   }
#else
   while(!(DRV_Reg32(U3D_DEV_LINK_INTR) & SSUSB_DEV_SPEED_CHG_INTR));
#endif
   DRV_WriteReg32(U3D_DEV_LINK_INTR, SSUSB_DEV_SPEED_CHG_INTR);

   temp = (DRV_Reg32(U3D_DEVICE_CONF) & SSUSB_DEV_SPEED);
   switch (temp)
   {
   case SSUSB_SPEED_SUPER:
      LOGD("Device: SUPER SPEED\n");
      break;
   case SSUSB_SPEED_HIGH:
      LOGD("Device: HIGH SPEED\n");
      break;
   case SSUSB_SPEED_FULL:
      LOGD("Device: FULL SPEED\n");
      break;
   case SSUSB_SPEED_INACTIVE:
      LOGD("Device: INACTIVE\n");
      break;
   }

   if (temp != speed)
   {
      /* timeout after 5 sec */
      while (time_count < timeout_s) {
         time_count++;
         LOGD("desired speed: %d, detected speed: %d\n", speed, temp);
         sleep(1);
      }
   }
}

/**
* mu3d_hal_dft_reg - apply default register settings
*/
void mu3d_hal_dft_reg(void){

   /* set sys_ck related registers */
   //sys_ck = OSC 125MHz/2 = 62.5MHz
   USB_SetMsk(U3D_SSUSB_SYS_CK_CTRL, SSUSB_SYS_CK_DIV2_EN);
   //U2 MAC sys_ck = ceil(62.5) = 63
   USB_WriteMsk(U3D_USB20_TIMING_PARAMETER, 63, TIME_VALUE_1US);
#ifdef SUPPORT_U3
   //U3 MAC sys_ck = ceil(62.5) = 63
   USB_WriteMsk(U3D_TIMING_PULSE_CTRL, 63, CNT_1US_VALUE);
#endif

   /* set ref_ck related registers */
   //U2 ref_ck = OSC 20MHz/2 = 10MHz
   USB_WriteMsk(U3D_SSUSB_U2_PHY_PLL, 10, SSUSB_U2_PORT_1US_TIMER);
   //>600ns
   USB_WriteMsk(U3D_SSUSB_IP_PW_CTRL0,
      (7<<SSUSB_IP_U2_ENTER_SLEEP_CNT_OFST), SSUSB_IP_U2_ENTER_SLEEP_CNT);
#ifdef SUPPORT_U3
   //U3 ref_ck = 20MHz/2 = 10MHz
   USB_WriteMsk(U3D_REF_CK_PARAMETER, 10, REF_1000NS);
   //>=300ns
   USB_WriteMsk(U3D_UX_EXIT_LFPS_TIMING_PARAMETER,
      (3<<RX_UX_EXIT_LFPS_REF_OFST), RX_UX_EXIT_LFPS_REF);
#endif


   /* code to override HW default values, FPGA ONLY */

   //enable debug probe
   DRV_WriteReg32(U3D_SSUSB_PRB_CTRL0, 0xffff);


   //for better USB 2.0 electrical compliance
   USB_WriteMsk(U3D_LINK_RESET_INFO, 0x0, WRFSSE0); //EL_28 Device CHIRP Response to Reset from Suspend
   USB_WriteMsk(U3D_LINK_RESET_INFO, 0x4<<WTCHRP_OFST, WTCHRP); //EL_28 Measure Device CHIRP-K Latency

   //response STALL to host if LPM BESL value is not in supporting range
   USB_SetMsk(U3D_POWER_MANAGEMENT, (LPM_BESL_STALL | LPM_BESLD_STALL));


#ifdef SUPPORT_U3
   //device responses to u3_exit from host automatically
   DRV_WriteReg32(U3D_LTSSM_CTRL, DRV_Reg32(U3D_LTSSM_CTRL) &~ SOFT_U3_EXIT_EN);


   //DRV_WriteReg32(U3D_PIPE_LATCH_SELECT, 1);	//04404_test
   DRV_WriteReg32(U3D_PIPE_LATCH_SELECT, 0);	//04404_test


   //set TX/RX latch select for D60802A
   //to do need to check the u3phy version YC
   /*
   if (u3phy->phy_version == 0xd60802a)
   DRV_WriteReg32(U3D_PIPE_LATCH_SELECT, 1);
   */
#ifdef DIS_ZLP_CHECK_CRC32
   //disable CRC check of ZLP, for compatibility concern
   DRV_WriteReg32(U3D_LINK_CAPABILITY_CTRL, ZLP_CRC32_CHK_DIS);
#endif
#endif

#if ISO_UPDATE_TEST
#if ISO_UPDATE_MODE
   DRV_WriteReg32(U3D_POWER_MANAGEMENT, DRV_Reg32(U3D_POWER_MANAGEMENT) | ISO_UPDATE);
#else
   DRV_WriteReg32(U3D_POWER_MANAGEMENT, DRV_Reg32(U3D_POWER_MANAGEMENT) &~ ISO_UPDATE);
#endif
#endif
}


#ifndef CFG_FPGA_PLATFORM
#define AP_PLL_CON0 (APMIXED_BASE + 0x00)
#define AP_PLL_CON2 (APMIXED_BASE + 0x08)
void enable_ssusb_xtal_clock(kal_bool b_enable)
{
   LOGD("enable_ssusb_xtal_clock for u3 (%d)\n", b_enable);

   if (b_enable) {
      /*
      * 1 *AP_PLL_CON0 =| 0x1 [0]=1: RG_LTECLKSQ_EN
      * 2 Wait PLL stable (100us)
      * 3 *AP_PLL_CON0 =| 0x2 [1]=1: RG_LTECLKSQ_LPF_EN
      * 4 *AP_PLL_CON2 =| 0x1 [0]=1: DA_REF2USB_TX_EN
      * 5 Wait PLL stable (100us)
      * 6 *AP_PLL_CON2 =| 0x2 [1]=1: DA_REF2USB_TX_LPF_EN
      * 7 *AP_PLL_CON2 =| 0x4 [2]=1: DA_REF2USB_TX_OUT_EN
      */
      USB_SetMsk(AP_PLL_CON0, (0x1));

      /*Wait 100 usec*/
      udelay(100);

      USB_SetMsk(AP_PLL_CON0, (0x2));

      USB_SetMsk(AP_PLL_CON2, (0x1));

      /*Wait 100 usec*/
      udelay(100);

      USB_SetMsk(AP_PLL_CON2, (0x2));

      USB_SetMsk(AP_PLL_CON2, (0x4));

   } else {
      /*
      * AP_PLL_CON2 &= 0xFFFFFFF8 [2]=0: DA_REF2USB_TX_OUT_EN
      *                   [1]=0: DA_REF2USB_TX_LPF_EN
      *                   [0]=0: DA_REF2USB_TX_EN
      */
      //DRV_WriteReg32(DRV_Reg32((void __iomem *)AP_PLL_CON2)&~(0x00000007),
      //     (void __iomem *)AP_PLL_CON2);
   }
}
#endif

/* reset hw power*/
void USB_Reset_Drv(void)
{

#ifndef CFG_FPGA_PLATFORM
#ifdef USB_U3_XTAL_CLOCK_EN
   /* BROM may not enable u3 ability, need to add this ??? */
   enable_ssusb_xtal_clock(KAL_TRUE);
#endif
#endif
   //04404, use sw_rst from reset_dev() in sample code

   //USB_QMU_Init_and_Start();//04404_test
   //USB_Is_QMU_Enabled();
   //LOGD("before mu3d_hal_rst_dev()\n");


   //reset
   mu3d_hal_rst_dev();

   /* it seems taht rst_dev reset QMU also*/
   //LOGD("after mu3d_hal_rst_dev()\n");		//04404_test
   //USB_Is_QMU_Enabled();

   g_bQMU_Init_Done = KAL_FALSE;

   mdelay(50);

   mu3d_hal_ssusb_en();

   //mu3d_hal_dft_reg();

   //04404, marked first, this should be done in DP_PULL_HIGH(True) ?
#if 0
   //set device speed
   mu3d_hal_set_speed(U3D_DFT_SPEED);

   //detect connect speed
   //mu3d_hal_det_speed(U3D_DFT_SPEED, 0);
#endif



}

/* initialize drv registers, including system global interrupt and EP0 interrupt*/
void USB_Initialize_Drv(void)
{
   /* LV1 intR */
   DRV_WriteReg32(U3D_LV1IESR, 0xFFFFFFFF);

   //04404
   /*clear LPM_MODE set first */
   DRV_WriteReg32(U3D_POWER_MANAGEMENT, DRV_Reg32(U3D_POWER_MANAGEMENT) & ~LPM_MODE);
   /* set LPM_MODE to always reject LPM request*/
   DRV_WriteReg32(U3D_POWER_MANAGEMENT, DRV_Reg32(U3D_POWER_MANAGEMENT) | (LPM_MODE & (0x1<<8)));

   //04404, added becoz marked set_speed in USB_Reset_Drv()
#ifndef EXT_VBUS_DET
   DRV_WriteReg32(U3D_MISC_CTRL, 0);
#else
   DRV_WriteReg32(U3D_MISC_CTRL, 0x3);
#endif

   /* enable system interrupts, but disable all ep interrupts */
   USB_EnSysIntr();//reference mu3d_hal_system_intr_en();

   /* enable EP0 interrupt */
   USB_EP0En();

   //04404
   //USB_QMU_Init_and_Start();		//04404_test

}


/* enable system global interrupt*/
void USB_EnSysIntr(void)
{
   /* maybe not so many interrupt enable here */
   kal_uint16 int_en;
#ifdef SUPPORT_U3
   kal_uint32 ltssm_int_en;
#endif

   DRV_WriteReg32(U3D_EPIECR, DRV_Reg32(U3D_EPIER));
   DRV_WriteReg32(U3D_DMAIECR, DRV_Reg32(U3D_DMAIER));

   //clear and enable common USB interrupts
   DRV_WriteReg32(U3D_COMMON_USB_INTR_ENABLE, 0x00);
   DRV_WriteReg32(U3D_COMMON_USB_INTR, DRV_Reg32(U3D_COMMON_USB_INTR));
   int_en = SUSPEND_INTR_EN|RESUME_INTR_EN|RESET_INTR_EN|CONN_INTR_EN|DISCONN_INTR_EN \
      |VBUSERR_INTR_EN|LPM_INTR_EN|LPM_RESUME_INTR_EN;
   DRV_WriteReg32(U3D_COMMON_USB_INTR_ENABLE, int_en);

#ifdef SUPPORT_U3
   //clear and enable LTSSM interrupts
   DRV_WriteReg32(U3D_LTSSM_INTR_ENABLE, 0x00);
   DRV_WriteReg32(U3D_LTSSM_INTR, DRV_Reg32(U3D_LTSSM_INTR));
   ltssm_int_en = SS_INACTIVE_INTR_EN|SS_DISABLE_INTR_EN|COMPLIANCE_INTR_EN|LOOPBACK_INTR_EN \
      |HOT_RST_INTR_EN|WARM_RST_INTR_EN|RECOVERY_INTR_EN|ENTER_U0_INTR_EN|ENTER_U1_INTR_EN \
      |ENTER_U2_INTR_EN|ENTER_U3_INTR_EN|EXIT_U1_INTR_EN|EXIT_U2_INTR_EN|EXIT_U3_INTR_EN \
      |RXDET_SUCCESS_INTR_EN|VBUS_RISE_INTR_EN|VBUS_FALL_INTR_EN|U3_LFPS_TMOUT_INTR_EN|U3_RESUME_INTR_EN;
   DRV_WriteReg32(U3D_LTSSM_INTR_ENABLE, ltssm_int_en);
#endif

   //04404, mark ep0 DMA
   //DRV_WriteReg32(U3D_DMAIESR, EP0DMAIESR);
   DRV_WriteReg32(U3D_DEV_LINK_INTR_ENABLE, SSUSB_DEV_SPEED_CHG_INTR_EN);
}


/* set address for hareware*/
void USB_SetAddress(kal_uint8 addr, USB_SET_ADDR_STATE state)
{

   if(state == USB_SET_ADDR_STATUS)
      DRV_WriteReg32(U3D_DEVICE_CONF, (addr<<DEV_ADDR_OFST));

}

kal_uint8 USB_ReadAddress(void)
{
   return (kal_uint8)(DRV_Reg32(U3D_DEVICE_CONF)>>DEV_ADDR_OFST);
}




/************************************************************
EP ctrl functinos
*************************************************************/

/* initialize EP IN */
void USB_InEPInit(kal_uint8 epno,kal_uint16 maxp, USB_ENDPT_TXFER_TYPE type)
{
   kal_uint32 csr0, csr1, csr2;
   kal_uint8 binterval = EPX_bINTERVAL, slot = EPX_SLOT, burst = EPX_BURST, mult= EPX_MULT, max_pkt, fifosz=USB_FIFOSZ_SIZE_16;


   switch(maxp){
        case 8:
           fifosz = USB_FIFOSZ_SIZE_16;
           break;
        case 16:
           fifosz = USB_FIFOSZ_SIZE_16;
           break;
        case 32:
           fifosz = USB_FIFOSZ_SIZE_32;
           break;
        case 64:
           fifosz = USB_FIFOSZ_SIZE_64;
           break;
        case 128:
           fifosz = USB_FIFOSZ_SIZE_128;
           break;
        case 256:
           fifosz = USB_FIFOSZ_SIZE_256;
           break;
        case 512:
           fifosz = USB_FIFOSZ_SIZE_512;
           break;
        case 1024:
           fifosz = USB_FIFOSZ_SIZE_1024;
           break;
   }

   //CSR0
   csr0 = USB_ReadCsr32(U3D_TX1CSR0, epno) &~ TX_TXMAXPKTSZ;
   csr0 |= (maxp & TX_TXMAXPKTSZ);


   if(USB_IS_QMU_Init_Done() == KAL_TRUE)
      csr0 |= TX_DMAREQEN;
   else
      csr0 &= ~TX_DMAREQEN;


   //CSR1
   max_pkt = (burst+1)*(mult+1)-1;
   csr1 = (burst & SS_TX_BURST);
   csr1 |= (slot<<TX_SLOT_OFST) & TX_SLOT;
   csr1 |= (max_pkt<<TX_MAX_PKT_OFST) & TX_MAX_PKT;
   csr1 |= (mult<<TX_MULT_OFST) & TX_MULT;

   //CSR2
   csr2 = (g_TX_FIFOadd>>4) & TXFIFOADDR;
   csr2 |= (fifosz<<TXFIFOSEGSIZE_OFST) & TXFIFOSEGSIZE;

   if(type==USB_ENDPT_BULK){
      csr1 |= TYPE_BULK;
   }
   else if(type==USB_ENDPT_INTR){
      csr1 |= TYPE_INT;
      csr2 |= (binterval<<TXBINTERVAL_OFST)&TXBINTERVAL;
   }
   else if(type==USB_ENDPT_ISO){
      csr1 |= TYPE_ISO;
      csr2 |= (binterval<<TXBINTERVAL_OFST)&TXBINTERVAL;
   }
   DRV_WriteReg32(U3D_EPIECR, DRV_Reg32(U3D_EPIECR)|(BIT0<<epno));
   USB_WriteCsr32(U3D_TX1CSR0, epno, csr0);
   USB_WriteCsr32(U3D_TX1CSR1, epno, csr1);
   USB_WriteCsr32(U3D_TX1CSR2, epno, csr2);

#if 0
   LOGD("USB_InEPInit, epno :%d, maxp :%d, binterval : %d, slot : %d, burst : %d, mult : %d, max_pkt : %d, fifosz : %d, type :%d\n", epno, maxp, binterval, slot, burst, mult, max_pkt, fifosz, type);

   //LOGD("[CSR]U3D_TX1CSR0 :%x\n",USB_ReadCsr32(U3D_TX1CSR0, epno));
   //LOGD("[CSR]U3D_TX1CSR1 :%x\n",USB_ReadCsr32(U3D_TX1CSR1, epno));
   //LOGD("[CSR]U3D_TX1CSR2 :%x\n",USB_ReadCsr32(U3D_TX1CSR2, epno));
#endif

   g_TX_FIFOadd += (maxp * (slot + 1));
}


/* initialize EP OUT */
void USB_OutEPInit(kal_uint8 epno,kal_uint16 maxp, USB_ENDPT_TXFER_TYPE type)
{
   kal_uint32 csr0, csr1, csr2;
   kal_uint8 binterval = EPX_bINTERVAL, slot = EPX_SLOT, burst = EPX_BURST, mult= EPX_MULT, max_pkt, fifosz=USB_FIFOSZ_SIZE_16;


   switch(maxp){
        case 8:
           fifosz = USB_FIFOSZ_SIZE_16;
           break;
        case 16:
           fifosz = USB_FIFOSZ_SIZE_16;
           break;
        case 32:
           fifosz = USB_FIFOSZ_SIZE_32;
           break;
        case 64:
           fifosz = USB_FIFOSZ_SIZE_64;
           break;
        case 128:
           fifosz = USB_FIFOSZ_SIZE_128;
           break;
        case 256:
           fifosz = USB_FIFOSZ_SIZE_256;
           break;
        case 512:
           fifosz = USB_FIFOSZ_SIZE_512;
           break;
        case 1024:
           fifosz = USB_FIFOSZ_SIZE_1024;
           break;
   }

   //CSR0
   csr0 = USB_ReadCsr32(U3D_RX1CSR0, epno) &~ RX_RXMAXPKTSZ;
   csr0 |= (maxp & RX_RXMAXPKTSZ);


   if(USB_IS_QMU_Init_Done() == KAL_TRUE)
      csr0 |= RX_DMAREQEN;
   else
      csr0 &= ~RX_DMAREQEN;

   //CSR1
   max_pkt = (burst+1)*(mult+1)-1;
   csr1 = (burst & SS_RX_BURST);
   csr1 |= (slot<<RX_SLOT_OFST) & RX_SLOT;
   csr1 |= (max_pkt<<RX_MAX_PKT_OFST) & RX_MAX_PKT;
   csr1 |= (mult<<RX_MULT_OFST) & RX_MULT;

   //CSR2
   csr2 = (g_RX_FIFOadd>>4) & RXFIFOADDR;
   csr2 |= (fifosz<<RXFIFOSEGSIZE_OFST) & RXFIFOSEGSIZE;

   if(type==USB_ENDPT_BULK){
      csr1 |= TYPE_BULK;
   }
   else if(type==USB_ENDPT_INTR){
      csr1 |= TYPE_INT;
      csr2 |= (binterval<<RXBINTERVAL_OFST)&RXBINTERVAL;
   }
   else if(type==USB_ENDPT_ISO){
      csr1 |= TYPE_ISO;
      csr2 |= (binterval<<RXBINTERVAL_OFST)&RXBINTERVAL;
   }


   DRV_WriteReg32(U3D_EPIECR, DRV_Reg32(U3D_EPIECR)|(BIT16<<epno));
   USB_WriteCsr32(U3D_RX1CSR0, epno, csr0);
   USB_WriteCsr32(U3D_RX1CSR1, epno, csr1);
   USB_WriteCsr32(U3D_RX1CSR2, epno, csr2);

#if 0
   LOGD("USB_OutEPInit, epno :%d, maxp :%d, binterval : %d, slot : %d, burst : %d, mult : %d, max_pkt : %d, fifosz : %d, type :%d\n", epno, maxp, binterval, slot, burst, mult, max_pkt, fifosz, type);
   //LOGD("[CSR]U3D_RX1CSR0 :%x\n",USB_ReadCsr32(U3D_RX1CSR0, epno));
   //LOGD("[CSR]U3D_RX1CSR1 :%x\n",USB_ReadCsr32(U3D_RX1CSR1, epno));
   //LOGD("[CSR]U3D_RX1CSR2 :%x\n",USB_ReadCsr32(U3D_RX1CSR2, epno));
#endif

   //04404
   //os_setmsk(U3D_USB2_RX_EP_DATAERR_INTR, BIT16<<ep_num); //EPn data error interrupt

   g_RX_FIFOadd += (maxp * (slot + 1));
}
/*  to save memory size (TX/RX setting must be the same)
kal_uint16 USB_InEP_PK_SIZE(kal_uint8 no)
{
}
*/
kal_uint16 USB_OutEP_PK_SIZE(kal_uint8 no)
{
   kal_uint16  maxp;

   maxp = USB_ReadCsr32(U3D_RX1CSR0, no) & RX_RXMAXPKTSZ;
   return maxp;
}

void USB_EP0En(void)
{
   /* this function is called in reset hdlr, speed should be done before calling this ? */
   USB_Dbg_Trace(DEBUG_USB_EP0En, 0, 0,0);

   if((DRV_Reg32(U3D_DEVICE_CONF) & SSUSB_DEV_SPEED) == SSUSB_SPEED_SUPER)
   {
      DRV_WriteReg32(U3D_EP0CSR, USB_EP0_SS_MAXP);
      gUsbDevice.devdscr.bMaxPacketSize0 = USB_EP0_SS_MAXP_EXP; /* note here goes the exp */
   }
   else{
      DRV_WriteReg32(U3D_EP0CSR, USB_EP0_MAXP);
      gUsbDevice.devdscr.bMaxPacketSize0 = USB_EP0_MAXP;
   }

   DRV_WriteReg32(U3D_EPIESR, DRV_Reg32(U3D_EPIESR)|(EP0ISR | SETUPENDISR));

}

void USB_InEPEn(kal_uint8 epnum, kal_bool bDMA)
{
   kal_uint32 int_txe;

   USB_Dbg_Trace(DEBUG_USB_InEPEn, 0, 0,0);

   int_txe = DRV_Reg32(U3D_EPIER);
   int_txe |= (1 << epnum);

   if(USB_IS_QMU_Init_Done() == KAL_TRUE){
      if(0)
         LOGD("USB_InEPEn, epnum :%d, intR enable skipped coz QMU has been inited\n", epnum);
   }
   else{
      //LOGD("USB_InEPEn, epnum :%d, intR enable\n", epnum);
      DRV_WriteReg32(U3D_EPIESR, int_txe);
   }



}


void USB_OutEPEn(kal_uint8 epnum, kal_bool bDMA)
{
   kal_uint32 int_rxe;

   USB_Dbg_Trace(DEBUG_USB_OutEPEn, 0, 0,0);

   int_rxe = DRV_Reg32(U3D_EPIER);
   int_rxe |= (BIT16 << epnum);

   if(USB_IS_QMU_Init_Done() == KAL_TRUE){
      if(0)
         LOGD("USB_OutEPEn, epnum :%d, intR enable skipped coz QMU has been inited\n", epnum);
   }
   else{
      //LOGD("USB_OutEPEn, epnum :%d, intR enable\n", epnum);
      DRV_WriteReg32(U3D_EPIESR, int_rxe);
   }
}

void USB_EP_Disable_InterR(kal_uint8 epnum,  USB_DIR dir)
{
   kal_uint32 int_e;
   int_e = DRV_Reg32(U3D_EPIECR);

   //LOGD("USB_EP_Disable_InterR, epnum : %d, USB_DIR : %d\n", epnum, dir);
   if(dir == USB_RX)
   {
      int_e |= (BIT16 << epnum);

   }
   else
   {
      int_e |= (1 << epnum);
   }

   DRV_WriteReg32(U3D_EPIECR, int_e);

}


/*  read fifo data from EP nEP*/
void USB_EPFIFORead(kal_uint8 nEP, kal_uint16 nBytes, void *pDst)
{
   kal_uint16 nCount = nBytes;
   kal_uint8   *pby;
   kal_uint32  nAddr;
   kal_uint32 temp;

   nAddr= USB_FIFO(nEP);
   pby = (kal_uint8 *)pDst;

   /* read 4bytes at once, IP restriction */
   while (nCount > 0) {
      temp = DRV_Reg32(nAddr);
      *pby = temp&0xFF;

      if(nCount > 1)
         *(pby+1) = (temp>>8)&0xFF;
      if(nCount > 2)
         *(pby+2) = (temp>>16)&0xFF;
      if(nCount > 3)
         *(pby+3) = (temp>>24)&0xFF;

      if( nCount > 4){
         pby+=4;
         nCount -= 4;
      }
      else
         nCount = 0;
   }

}



/*  write data to fifo EP nEP*/
void USB_EPFIFOWrite (kal_uint8 nEP, kal_uint16 nBytes, void *pSrc)
{
   kal_uint16 residue = nBytes;
   kal_uint32 temp;
   kal_uint8 *pby;
   kal_uint32 nAddr;

   nAddr= USB_FIFO(nEP);
   //LOGD("@USB_EPFIFOWrite nAddr:0x%x\n", nAddr);

   pby = (kal_uint8 *)pSrc;

   while(residue > 0) {
      if(residue==1) {
         temp=((*pby)&0xFF);
         DRV_WriteReg8(nAddr, temp);
         pby += 1;
         residue -= 1;
      } else if(residue==2) {
         temp=((*pby)&0xFF)+(((*(pby+1))<<8)&0xFF00);
         DRV_WriteReg16(nAddr, temp);
         pby += 2;
         residue -= 2;
      } else if(residue==3) {
         temp=((*pby)&0xFF)+(((*(pby+1))<<8)&0xFF00);
         DRV_WriteReg16(nAddr, temp);
         pby += 2;

         temp=((*pby)&0xFF);
         DRV_WriteReg8(nAddr, temp);
         pby += 1;
         residue -= 3;
      } else {
         temp=((*pby)&0xFF)+(((*(pby+1))<<8)&0xFF00)+(((*(pby+2))<<16)&0xFF0000)+(((*(pby+3))<<24)&0xFF000000);
         DRV_WriteReg32(nAddr, temp);
         pby += 4;
         residue -= 4;
      }
   }
   //mdelay(50);	//04404_test
}


/* type == USB_IN_EP_TYPE or USB_OUT_EP_TYPE
en == KAL_TRUE means stall this endpoint */
void USB_CtrlEPStall(kal_uint8 EPno, USB_EP_TYPE type, kal_bool en, USB_CTRL_STALL_ENTRY entry)
{
   kal_uint32  csr;
   if (en == KAL_TRUE)
   {
      /* stall endpoint */
      if(type == USB_OUT_EP_TYPE)
      {
         csr = USB_ReadCsr32(U3D_RX1CSR0, EPno);
         csr &= RX_W1C_BITS;
         csr |= RX_SENDSTALL;

         //LOGD("@@@@@@@@@@@@@@@@@ EP%d(OUT/RX) SEND_STALL\n", EPno);
         //musb_writew(regs, MUSB_RXCSR, csr);

         USB_WriteCsr32(U3D_RX1CSR0, EPno, csr);
         g_UsbDrvInfo.ep_out_stall_status[EPno-1] = KAL_TRUE;
      }
      else
      {
         csr = USB_ReadCsr32(U3D_TX1CSR0, EPno);
         if (!(csr & TX_FIFOEMPTY))
         {
            DRV_WriteReg32(U3D_EP_RST, DRV_Reg32(U3D_EP_RST) | (BIT16<<EPno));//reset TX EP
            DRV_WriteReg32(U3D_EP_RST, DRV_Reg32(U3D_EP_RST) & ~(BIT16<<EPno));//reset reset TX EP

         }

         csr &= TX_W1C_BITS;
         csr |= TX_SENDSTALL;
         //LOGD("@@@@@@@@@@@@@@@@@ EP%d(IN/TX) SEND_STALL\n", EPno);

         //ssusb: need further check. is WZC_BITS needed?
         USB_WriteCsr32(U3D_TX1CSR0, EPno, csr);

         g_UsbDrvInfo.ep_in_stall_status[EPno-1] = KAL_TRUE;
      }
   }
   else
   {
      /* clear stall */
      if (type == USB_OUT_EP_TYPE)
      {
         csr = USB_ReadCsr32(U3D_RX1CSR0, EPno) & RX_W1C_BITS;
         csr = (csr & (~RX_SENDSTALL)) | RX_SENTSTALL;

         USB_WriteCsr32(U3D_RX1CSR0, EPno, csr);

         //LOGD("&&&&&&&&&&&&&&&&&&&&&&&&&& clear rx stall --> write csr[%d] 0x%x. new CSR is: 0x%x\n", EPno, csr, USB_ReadCsr32(U3D_RX1CSR0, EPno) );

         DRV_WriteReg32(U3D_EP_RST, DRV_Reg32(U3D_EP_RST) | (1 << EPno));//reset RX EP
         DRV_WriteReg32(U3D_EP_RST, DRV_Reg32(U3D_EP_RST) & (~(1 << EPno)));//reset reset RX EP
         //LOGD("RST RX%d\n", EPno);
         /* We cannot flush QMU now, because the MSC gadget will not re-submit the CBW request after clear halt. */

         //USB_Stop_QMU(EPno, USB_RX);
         //USB_QMU_Resource_reset(EPno, USB_RX);
         //USB_Restart_QMU(EPno, USB_RX);
         g_UsbDrvInfo.ep_out_stall_status[EPno-1] = KAL_FALSE;
      }
      else
      {
         csr = USB_ReadCsr32(U3D_TX1CSR0, EPno) & TX_W1C_BITS;
         csr = (csr & (~TX_SENDSTALL)) | TX_SENTSTALL;
         //csr = csr & (~TX_SENDSTALL);

         USB_WriteCsr32(U3D_TX1CSR0, EPno, csr);

         //LOGD("&&&&&&&&&&&&&&&&&&&&&&&&&& clear tx stall --> write csr[%d] 0x%x. new CSR is: 0x%x\n", EPno, csr, USB_ReadCsr32(U3D_TX1CSR0, EPno));

         DRV_WriteReg32(U3D_EP_RST, DRV_Reg32(U3D_EP_RST) | (BIT16<<EPno));//reset TX EP
         DRV_WriteReg32(U3D_EP_RST, DRV_Reg32(U3D_EP_RST) & ~(BIT16<<EPno));//reset reset TX EP
         //LOGD("RST TX%d\n", EPno);

         /* We cannot flush QMU now, because the MSC gadget will not re-submit the CBW request after clear halt. */

         //USB_Stop_QMU(EPno, USB_TX);
         //USB_QMU_Resource_reset(EPno, USB_TX);
         //USB_Restart_QMU(EPno, USB_TX);

         g_UsbDrvInfo.ep_in_stall_status[EPno-1] = KAL_FALSE;
      }
   }

}

/* get endpoint 0 status. (if transaction end or sent stall interrupt received)  */
void USB_Get_EP0_Status(kal_bool *p_transaction_end, kal_bool *p_sent_stall)
{
   kal_uint32  	CSR0;
   kal_uint32 		EPISR;
   CSR0 = DRV_Reg32(U3D_EP0CSR);

   if (CSR0 & EP0_SENTSTALL)
      *p_sent_stall = KAL_TRUE;
   else
      *p_sent_stall = KAL_FALSE;

   EPISR = DRV_Reg32(U3D_EPISR);
   if (EPISR & SETUPENDISR)
      *p_transaction_end = KAL_TRUE;
   else
      *p_transaction_end = KAL_FALSE;
}



/* control endpoint 0 registers according to USB_EP0_DRV_STATE */
void USB_U3_Update_EP0_State(USB_EP0_DRV_STATE state, kal_bool stall, kal_bool end)
{

   kal_uint32   reg_state=0;
   kal_uint32   ep0_isr;

   /* clear sent stall */
   if(state == USB_EP0_DRV_STATE_CLEAR_SENT_STALL)
   {
      //04404
      DRV_WriteReg32(U3D_EP0CSR, DRV_Reg32(U3D_EP0CSR) | EP0_SENTSTALL);;
      return;
   }

   /* clear transaction end*/
   if(state == USB_EP0_DRV_STATE_TRANSACTION_END)
   {
      //need to clear the setup end isr
      ep0_isr = DRV_Reg32(U3D_EPISR) & SETUPENDISR;
      DRV_WriteReg32(U3D_EPISR, ep0_isr);
      return;
   }

   /* ep0 read end or write ready*/
   /*USB_EP0_DRV_STATE_SETUP_END imply DATAEND unless need to send stall*/
   if(state == USB_EP0_DRV_STATE_SETUP_END)
   {
      reg_state = EP0_SETUPPKTRDY;
   }
   else if( state == USB_EP0_DRV_STATE_SETUP_TO_TX)
   {
      reg_state = EP0_SETUPPKTRDY | EP0_DPHTX;
   }else if( state == USB_EP0_DRV_STATE_SETUP_TO_RX)
   {
      reg_state = EP0_SETUPPKTRDY;//& (~EP0_DPHTX);
   }else if( state == USB_EP0_DRV_STATE_TX)
   {
      reg_state = EP0_TXPKTRDY;
   }else if( state == USB_EP0_DRV_STATE_RX || state == USB_EP0_DRV_STATE_RX_END)
   {
      reg_state = EP0_RXPKTRDY;
      DRV_WriteReg32(U3D_EP0CSR, DRV_Reg32(U3D_EP0CSR)|reg_state);
      reg_state = 0;
   }else if(state == USB_EP0_DRV_STATE_TX_END)
   {
      reg_state = 0;
   }

   //04404
   /* error occured, sent stall*/
   if(stall == KAL_TRUE)
   {
      DRV_WriteReg32(U3D_EP_RST, DRV_Reg32(U3D_EP_RST) | EP0_RST);//reset EP0
      DRV_WriteReg32(U3D_EP_RST, DRV_Reg32(U3D_EP_RST) & ~EP0_RST );//reset reset  EP0
      DRV_WriteReg32(U3D_EP0CSR, DRV_Reg32(U3D_EP0CSR) | EP0_SENDSTALL);

      /* 04404, another way to do is wait for clear SENTSTALL, here, but state above maybe be obselted*/
#if 0
      while(!(DRV_Reg32(U3D_EP0CSR) & EP0_SENTSTALL));
      DRV_WriteReg32(U3D_EP0CSR, DRV_Reg32(U3D_EP0CSR) | EP0_SENTSTALL);
#endif
      return;
   }

   /* last data for this transaction, set data end bit*/
   if(end == KAL_TRUE)
   {
      reg_state |= EP0_DATAEND;
   }

   //LOGD("U3D_EP0CSR : %x, 1, reg_state : %x\n", DRV_Reg32(U3D_EP0CSR), reg_state);
   DRV_WriteReg32(U3D_EP0CSR, (DRV_Reg32(U3D_EP0CSR)& EP0_W1C_BITS) |reg_state);
   //LOGD("U3D_EP0CSR : %x, 2, reg_state : %x\n", DRV_Reg32(U3D_EP0CSR), reg_state);

}

/* return packet length for endpoint 0*/
kal_uint32 USB_EP0_Pkt_Len(void)
{
   kal_uint32 nCount =0; // CSR0;

   //CSR0 = DRV_Reg32(U3D_EP0CSR);
   DRV_Reg32(U3D_EP0CSR);
   nCount = (kal_uint32)DRV_Reg32(U3D_RXCOUNT0);

   return nCount;
}

/* ep in data prepared ready, set ready bit */
void USB_EP_Bulk_In_Ready(kal_uint8 ep_num)
{
   kal_uint32 CSR, nAddr;

   if (ep_num == 0)
      ASSERT(0);
   nAddr = SSUSB_EP_TXCR0_OFFSET(ep_num, 0);

   CSR = DRV_Reg32(nAddr);
   DRV_WriteReg32(nAddr, CSR|TX_TXPKTRDY);


}

void USB_EP_Poll_Bulk_In_Ready(kal_uint8 ep_num)
{
   kal_uint32 nAddr;
   nAddr = SSUSB_EP_TXCR0_OFFSET(ep_num, 0);

   while(DRV_Reg32(nAddr) & TX_TXPKTRDY);


}

kal_bool USB_EP_Poll_Bulk_Out_Intr(kal_uint8 ep_num)
{
   kal_uint32  dwIntrEPValue, wIntrRxValue;

   dwIntrEPValue = DRV_Reg32(U3D_EPISR)& DRV_Reg32(U3D_EPIER);
   wIntrRxValue = (dwIntrEPValue>>16);

   if(wIntrRxValue & (1<<ep_num)){
      //LOGD("wIntrRxValue :%x\n", wIntrRxValue);
      return KAL_TRUE;
   }
   else
      return KAL_FALSE;
}




/* return ep out received packet length*/
kal_uint32 USB_EP_Out_Pkt_Len(kal_uint8 ep_num)
{
   kal_uint32 nCount = 0, rxcsr0_addr, rxcsr3_addr;

   rxcsr0_addr = SSUSB_EP_RXCR0_OFFSET(ep_num, 0);
   rxcsr3_addr = SSUSB_EP_RXCR3_OFFSET(ep_num, 0);

   if (DRV_Reg32(rxcsr0_addr) & RX_RXPKTRDY)
     nCount = (DRV_Reg32(rxcsr3_addr)>>16);

   return nCount;
}


/* ep out data already read out, clear the data*/
void USB_EP_Bulk_Out_Ready(kal_uint8 ep_num)
{
   kal_uint32 CSR;
   kal_uint32 nAddr;

   if (ep_num == 0)
      ASSERT(0);
   nAddr = SSUSB_EP_RXCR0_OFFSET(ep_num, 0);
   CSR = DRV_Reg32(nAddr);

   DRV_WriteReg32(nAddr, CSR|RX_RXPKTRDY);
}



/************************************************************
HISR/LISR   interrupt handler
*************************************************************/


kal_bool USB_IS_High_Speed(void)
{
   kal_uint32 dwTemp;
   dwTemp = DRV_Reg32(U3D_DEVICE_CONF) & SSUSB_DEV_SPEED;

   if(dwTemp == SSUSB_SPEED_HIGH)
   {
      return KAL_TRUE;
   }
   else
   {
      return KAL_FALSE;
   }
}

kal_bool USB_IS_Super_Speed(void)
{
   kal_uint32 dwTemp;
   dwTemp = DRV_Reg32(U3D_DEVICE_CONF) & SSUSB_DEV_SPEED;

   if(dwTemp == SSUSB_SPEED_SUPER)
   {
      return KAL_TRUE;
   }
   else
   {
      return KAL_FALSE;
   }
}


void USB_HISR(void)
{
   kal_uint32  IntrEP;
   kal_uint32  IntrUSB;
   kal_uint32  ep_num;
   kal_uint32  RXCSR;
#ifdef SUPPORT_U3
   kal_uint32 	dwLtssmValue;
#endif
   kal_uint32 	dwLinkIntValue;
   kal_uint32 	dwTemp;

   kal_uint16  IntrTx;
   kal_uint16  IntrRx;

   kal_uint32 wIntrQMUDoneValue, wIntrQMUValue;
   kal_bool qmu_poll_done = KAL_FALSE;

   dwLinkIntValue = DRV_Reg32(U3D_DEV_LINK_INTR) & DRV_Reg32(U3D_DEV_LINK_INTR_ENABLE);
   // Write 1 clear
   DRV_WriteReg32(U3D_DEV_LINK_INTR, dwLinkIntValue);

   if( DRV_Reg32(U3D_LV1ISR) & MAC2_INTR)
   {
      IntrUSB = DRV_Reg32(U3D_COMMON_USB_INTR) & DRV_Reg32(U3D_COMMON_USB_INTR_ENABLE);
      // Write 1 clear
      DRV_WriteReg32(U3D_COMMON_USB_INTR, IntrUSB);
   }
   else
   {
      IntrUSB = 0;
   }
#if 1 //04404_test QMU processing
   /* QMU processing */
   wIntrQMUDoneValue = DRV_Reg32(U3D_QISAR0) & DRV_Reg32(U3D_QIER0);
   /* clear interrupt*/
   DRV_WriteReg32(U3D_QISAR0, wIntrQMUDoneValue);


   wIntrQMUValue = DRV_Reg32(U3D_QISAR1);

   if( (wIntrQMUValue & RXQ_CSERR_INT) || (wIntrQMUValue & RXQ_LENERR_INT))
   {
      kal_uint32 wErrVal = DRV_Reg32(U3D_RQERRIR0);
      //LOGD("U3D_RQERRIR0 :%x\n",wErrVal);
      DRV_WriteReg32(U3D_RQERRIR0, wErrVal);
      qmu_poll_done = KAL_TRUE;
   }


   if( (wIntrQMUValue & RXQ_ZLPERR_INT) )
   {
      kal_uint32 wErrVal = DRV_Reg32(U3D_RQERRIR1);
      DRV_WriteReg32(U3D_RQERRIR1, wErrVal);
      gUSBAcm_IsZLPErr = KAL_TRUE;
   }

   if( (wIntrQMUValue & RXQ_EMPTY_INT) || (wIntrQMUValue & TXQ_EMPTY_INT))
   {
      kal_uint32 wEmptyVal = DRV_Reg32(U3D_QEMIR);
      DRV_WriteReg32(U3D_QEMIR, wEmptyVal);
   }

   if(wIntrQMUDoneValue)
   {
      qmu_poll_done = KAL_TRUE;

   }

   if(qmu_poll_done == KAL_TRUE)
   {
      gUSBAcm_IsQMUDone = KAL_TRUE;
   }
#endif

#ifdef SUPPORT_U3
   if(DRV_Reg32(U3D_LV1ISR) & MAC3_INTR)
   {
      dwLtssmValue = DRV_Reg32(U3D_LTSSM_INTR) & DRV_Reg32(U3D_LTSSM_INTR_ENABLE);
      // Write 1 clear
      DRV_WriteReg32(U3D_LTSSM_INTR, dwLtssmValue);
      LOGD("dwLtssmValue : %x\n", dwLtssmValue);

   }
   else
   {
      dwLtssmValue = 0;
   }

#endif

   IntrEP = DRV_Reg32(U3D_EPISR) & DRV_Reg32(U3D_EPIER);
   IntrTx = IntrEP & 0xFFFF;
   IntrRx = (IntrEP >> 16);

   // Write 1 clear
   DRV_WriteReg32(U3D_EPISR, IntrEP);

   /* START TO HANDLE INTERRUPT HERE */
   if (dwLinkIntValue & SSUSB_DEV_SPEED_CHG_INTR)
   {
      dwTemp = DRV_Reg32(U3D_DEVICE_CONF) & SSUSB_DEV_SPEED;
      switch (dwTemp)
      {
      case SSUSB_SPEED_FULL:
         LOGD("FS\n");
         break;
      case SSUSB_SPEED_HIGH:
         LOGD("HS\n");
         break;
      case SSUSB_SPEED_SUPER:
         LOGD("SS\n");
         break;
      default:
         LOGD("Invalid\n");
      }
   }

#ifdef SUPPORT_U3
   if (dwLtssmValue){
      if(dwLtssmValue & SS_DISABLE_INTR){
         LOGD("Device: SS Disable, %d\n", (DRV_Reg32(U3D_LTSSM_INFO) & DISABLE_CNT) >> DISABLE_CNT_OFST);
         LOGD("SOFTCONN = 1\n");

         /* u3 switch to u3 flow */
#ifdef U2_U3_SWITCH
         mu3d_hal_u2dev_connect();
#endif
         USB_EP0En();
         dwLtssmValue =0;
      }

      /* u3 success notify */
      if(dwLtssmValue & ENTER_U0_INTR){
         /* u3 don't have reset interrupt */
         LOGD("ENTER_U0_INTR\n");
         USB_Reset();
         LOGD("U0_INTR USB_Reset() done\n");

      }
   }
#endif

   if(IntrUSB & RESET_INTR)
   {
      if(g_UsbDrvInfo.reset_hdlr == NULL)
         ASSERT(0);

      LOGD("RESET intr, IntrUSB:%x\n", IntrUSB );
      /* Set SW_RST bit here, so we do not care about any following ep interrupts */
      g_UsbDrvInfo.reset_hdlr(); 		//04404_test
      DRV_WriteReg32(U3D_DEVICE_CONF,  DRV_Reg32(U3D_DEVICE_CONF) | (0<<DEV_ADDR_OFST));		//04404_test, set addr to 0
      IntrTx = 0;
      IntrRx = 0;
   }


   if ((IntrTx == 0)&& (IntrRx == 0))
      return;

   /* Check for endpoint 0 interrupt */
   if(IntrTx&0x01)
   {
      if(g_UsbDrvInfo.ep0_hdlr == NULL)
         ASSERT(0);

      g_UsbDrvInfo.ep0_hdlr();
      IntrTx =IntrTx & ~0x01;
   }
   /* Check for Bulk TX interrupt */
   /* TX interrupt : 1.TxPktRdy is clear, 2. SENTSTALL is set */


   if (IntrTx != 0)
   {
      for(ep_num = 1; ep_num <= MAX_INTR_EP_NUM; ep_num++)
      {
         if (IntrTx&(0x01 <<ep_num))
         {
            if(g_UsbDrvInfo.ep_in_hdlr[ep_num-1] != NULL)
               g_UsbDrvInfo.ep_in_hdlr[ep_num -1]();
         }
      }
   }
   /* Check for Bulk RX interrupt */
   /* RX interrupt : 1.RxPktRdy is set, 2. SENTSTALL is set */


   if (IntrRx != 0)
   {
      for(ep_num = 1; ep_num <=MAX_INTR_EP_NUM; ep_num++)
      {
         if(IntrRx&(0x01<<ep_num))
         {
            RXCSR = USB_ReadCsr32(U3D_RX1CSR0, ep_num);
            if((RXCSR&RX_RXPKTRDY) != 0)
            {
               if(g_UsbDrvInfo.ep_out_hdlr[ep_num-1] != NULL)
                  g_UsbDrvInfo.ep_out_hdlr[ep_num -1]();
			   //else if (InheritedUSB == KAL_TRUE)
				 //  MSG(USB, "No handler to receive this packet\n");
               else
                  /* No handler to receive this packet */
                  ASSERT(0);
            }
         }
      }
   }
}

void USB_SetDPB(void)
{
   //nothing to do in 6593, no double buffer feature
}

