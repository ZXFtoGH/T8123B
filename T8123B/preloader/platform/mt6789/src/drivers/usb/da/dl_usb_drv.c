/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2005
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
*  RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE. 
*
*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
*
*****************************************************************************/

/*****************************************************************************
 *
 * Filename:
 * ---------
 *    usb_drv.c
 *
 * Project:
 * --------
 *   Maui_Software
 *
 * Description:
 * ------------
 *  This file implements usb hardware related functions
 *
 * Author:
 * -------
 *   CJung Chen
 *
 *============================================================================
 *             HISTORY
 * Below this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
 *------------------------------------------------------------------------------
 * $Revision: #1 $
 * $Modtime:   Jul 05 2005 09:42:42  $
 * $Log:   //mtkvs01/vmdata/Maui_sw/archives/mcu/drv/src/usb_drv.c-arc  $
 *
 * Feb 25 2009 MTK02172
 * [STP100001912] [FlashTool] v3.0908.00 release
 * 
 *
 * May 16 2008 mtk01413
 * [STP100001691] [FlashTool] v3.0808.0 release
 * New features:
 *  1. USB Download for End User (MT6230)
 *
 * Dec 2 2007 mtk01413
 * [STP100001568] [FlashTool] v3.0748.0 release
 * New features:
 *  1. USB Download on MT6225/MT6226/MT6227 Series BB
 *------------------------------------------------------------------------------
 * Upper this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
 *============================================================================
 ****************************************************************************/
#include "kal_release.h"
#include "register_act_util.h"
  
#include "usb_comm.h"
#include "usb_hw.h"
#include "usb_drv.h"
#include "usb_drv_pri.h"
#include "usb_custom.h"
#include "usb_debug_tool.h"
#include "assert.h"
#include "lib/string.h"



#define USB_DOUBLE_BUF
#define USB_RX_DMA_MODE_0
#define USB_TX_DMA_MODE_0

#define int int
/* usb driver information*/
USB_Drv_Info g_UsbDrvInfo;

kal_uint32 Arr[1000];
kal_uint32 in=0,count_time = 0;
static kal_uint32 g_FIFOadd = USB_FIFO_START_ADDRESS;
//kal_uint32 Arr2[1];
//kal_uint32 count_time2 = 0;
kal_uint8 g_enable_hs = 1;

#if defined(USB_QMU)
#include "qmu.h"

gpd_pool RX_GPD_POOL[MAX_EP_NUM+1], TX_GPD_POOL[MAX_EP_NUM+1];
kal_bool g_bQMU_Init_Done = KAL_FALSE;
kal_bool gUSBAcm_IsQMUDone = KAL_FALSE;
kal_bool gUSBAcm_IsZLPErr = KAL_FALSE;

/************************************************************
	driver info functions
*************************************************************/
void USB_Poll_QMU_Done(void)
{
	kal_uint8 i/*, result*/;
	kal_uint32 wQmuVal, wRetVal;
	kal_bool qmu_poll_done = KAL_FALSE;
	
	//int status;

	while(1)
	{
		wQmuVal = DRV_Reg32(USB_QISAR);
		
		if(wQmuVal)
		{
			DRV_WriteReg32(USB_QISAR, wQmuVal);
			wQmuVal &= ~(DRV_Reg32(USB_QIMR));

			//MSG(USB, "USB_Poll_QMU_Done wIntrQMUDoneValue :%x, wIntrQMUValue : %x\n",wQmuVal, wRetVal);
			//RXQ GPD DONE
			for (i=1; i<=MAX_RX_EP_NUM; i++)
			{
				if (wQmuVal & DQMU_M_RX_DONE(i))
				{
					//MSG(USB, "DQMU_M_RX_DONE EP :%x\n",i);
					qmu_poll_done = KAL_TRUE;		
				}
			}

			//TXQ GPD DONE
			for (i=1; i<=MAX_TX_EP_NUM; i++)
			{
				if (wQmuVal & DQMU_M_TX_DONE(i))
				{
					//MSG(USB, "DQMU_M_TX_DONE EP :%x \n",i);
					qmu_poll_done = KAL_TRUE;
				}
			}

			//RXQ ERROR
			if (wQmuVal & DQMU_M_RXQ_ERR)
			{
				wRetVal = MGC_ReadQIRQ32(MGC_O_QIRQ_RQEIR) & (~(MGC_ReadQIRQ32(MGC_O_QIRQ_RQEIMR)));
//				MSG(USB, "Rx Queue error in QMU mode![0x%x]\n", wRetVal);

				for (i=1; i<=MAX_RX_EP_NUM; i++)
				{
					if (wRetVal & DQMU_M_RX_GPDCS_ERR(i))
					{
						MSG(USB, "Rx %d GPD checksum error!\n", i);
					}
					if (wRetVal & DQMU_M_RX_LEN_ERR(i))
					{
						MSG(USB, "Rx %d recieve length error!\n", i);
					}
					if (wRetVal & DQMU_M_RX_ZLP_ERR(i))
					{
						gUSBAcm_IsZLPErr = KAL_TRUE;
//						MSG(USB, "Rx %d recieve an zlp packet!\n", i);
					}
				}
				 
				MGC_WriteQIRQ32(MGC_O_QIRQ_RQEIR, wRetVal);
			}

			//TXQ ERROR
			if (wQmuVal & DQMU_M_TXQ_ERR)
			{
				wRetVal = MGC_ReadQIRQ32(MGC_O_QIRQ_TQEIR) & (~(MGC_ReadQIRQ32(MGC_O_QIRQ_TQEIMR)));
//				MSG(USB, "Tx Queue error in QMU mode![0x%x]\n", wRetVal);

				for (i=1; i<=MAX_TX_EP_NUM; i++)
				{
					if (wRetVal & DQMU_M_TX_BDCS_ERR(i))
					{
						MSG(USB, "Tx %d BD checksum error!\n", i);
					}
					if (wRetVal & DQMU_M_TX_GPDCS_ERR(i))
					{
						MSG(USB, "Tx %d GPD checksum error!\n", i);
					}
					if (wRetVal & DQMU_M_TX_LEN_ERR(i))
					{
						MSG(USB, "Tx %d buffer length error!\n", i);
					}
				}

				MGC_WriteQIRQ32(MGC_O_QIRQ_TQEIR, wRetVal);
			}

			//RX EP ERROR
			if (wQmuVal & DQMU_M_RXEP_ERR)
			{
				wRetVal = MGC_ReadQIRQ32(MGC_O_QIRQ_REPEIR) & (~(MGC_ReadQIRQ32(MGC_O_QIRQ_REPEIMR)));
				MSG(USB, "Rx endpoint error in QMU mode![0x%x]\n", wRetVal);

				for (i=1; i<=MAX_RX_EP_NUM; i++)
				{
					if (wRetVal & DQMU_M_RX_EP_ERR(i))
					{
						MSG(USB, "Rx %d Ep Error!\n", i);
					}
				}

				MGC_WriteQIRQ32(MGC_O_QIRQ_REPEIR, wRetVal);
			}

			//TX EP ERROR
			if(wQmuVal & DQMU_M_TXEP_ERR)
			{
				wRetVal = MGC_ReadQIRQ32(MGC_O_QIRQ_TEPEIR)& (~(MGC_ReadQIRQ32(MGC_O_QIRQ_TEPEIMR)));
				MSG(USB, "Tx endpoint error in QMU mode![0x%x]\n", wRetVal);

				for (i=1; i<=MAX_TX_EP_NUM; i++){
					if (wRetVal &DQMU_M_TX_EP_ERR(i))
					{
						MSG(USB, "Tx %d Ep Error!\n", i);
						
					}
				}

				MGC_WriteQIRQ32(MGC_O_QIRQ_TEPEIR, wRetVal);
			}

			//RXQ EMPTY
			if (wQmuVal & DQMU_M_RQ_EMPTY)
			{
				wRetVal = MGC_ReadQIRQ32(MGC_O_QIRQ_REPEMPR)
					& (~(MGC_ReadQIRQ32(MGC_O_QIRQ_REPEMPMR)));
				MSG(USB, "Rx Empty in QMU mode![0x%x]\n", wRetVal);

				for (i=1; i<=MAX_RX_EP_NUM; i++)
				{
					if (wRetVal & DQMU_M_RX_EMPTY(i))
					{
						MSG(USB, "Rx %d Empty!\n", i);
					}
				}

				MGC_WriteQIRQ32(MGC_O_QIRQ_REPEMPR, wRetVal);
			}

			//TXQ EMPTY
			if (wQmuVal & DQMU_M_TQ_EMPTY)
			{
				wRetVal = MGC_ReadQIRQ32(MGC_O_QIRQ_TEPEMPR)
					& (~(MGC_ReadQIRQ32(MGC_O_QIRQ_TEPEMPMR)));
				MSG(USB, "Tx Empty in QMU mode![0x%x]\n", wRetVal);

				for (i=1; i<=MAX_TX_EP_NUM; i++)
				{
					if (wRetVal & DQMU_M_TX_EMPTY(i))
					{
						MSG(USB, "Tx %d Empty!\n", i);
					}
				}

				MGC_WriteQIRQ32(MGC_O_QIRQ_TEPEMPR, wRetVal);
			}

			if(wQmuVal)
			{	
				//MSG(USB, "wIntrQMUDoneValue :%x\n",wIntrQMUDoneValue);
				//qmu_poll_done = KAL_TRUE;
			}

			if(qmu_poll_done == KAL_TRUE)
			{
				//MSG(USB, "break, wIntrQMUDoneValue :%x, wIntrQMUValue : %x\n",wIntrQMUDoneValue, wIntrQMUValue);
				gUSBAcm_IsQMUDone = KAL_TRUE;
				break;
			}
		}
	}
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
	//MSG(USB, "recivedlength : %d, ep_num : %d\n", recivedlength, ep_num);

	return recivedlength;
}

extern void USB_QMU_Prepare_GPD(kal_uint8 ep_num, kal_uint8* pBuf, kal_uint32 length, USB_DIR dir);
extern void arch_clean_invalidate_cache_range(addr_t start, size_t len);
extern void USB_QMU_Resume(kal_uint8 ep_num, USB_DIR dir);

kal_uint32 USB_QMU_Read(kal_uint8 ep_num, kal_uint8* addr, kal_uint32 length)
{
	kal_uint32 recivedlength;
	kal_uint8 *ptr;
	kal_uint32 len;
	TGPD *last_gpd;

	len = length;
	ptr = addr;
	USB_QMU_Prepare_GPD(ep_num, ptr, len, USB_RX);
	arch_clean_invalidate_cache_range((addr_t) ptr, len);
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

	//MSG(USB, "USB_QMU_Write, ep_num : %d, addr : %x, length : %d\n", ep_num, addr, length);//04404_test
	USB_QMU_Prepare_GPD(ep_num, ptr, len, USB_TX);
	arch_clean_invalidate_cache_range((addr_t) ptr, len);
	USB_QMU_Resume(ep_num, USB_TX);
	USB_Poll_QMU_Done();

	return length;
}


void USB_QMU_Prepare_GPD(kal_uint8 ep_num, kal_uint8* pBuf, kal_uint32 length, USB_DIR dir)
{
	if(dir == USB_RX)
	{
		arch_clean_invalidate_cache_range((addr_t) RX_GPD_POOL[ep_num].cur_ptr_gpd, sizeof(struct _TGPD));
		TGPD *gpd = RX_GPD_POOL[ep_num].cur_ptr_gpd;
		
//		MSG(USB, "GPD 0x%x\n", gpd);
		
		TGPD_SET_DATA(gpd, pBuf);
		TGPD_SET_DataBUF_LEN(gpd, length);

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
//		MSG(USB, "Rx Next GPD 0x%x \n",(kal_uint32)RX_GPD_POOL[ep_num].cur_ptr_gpd);
		TGPD_CLR_FLAGS_HWO(RX_GPD_POOL[ep_num].cur_ptr_gpd);
		TGPD_SET_NEXT(gpd, RX_GPD_POOL[ep_num].cur_ptr_gpd);

		TGPD_SET_IOC(gpd);
		TGPD_SET_CHKSUM_HWO(gpd, 16);
		TGPD_SET_FLAGS_HWO(gpd);
		arch_clean_invalidate_cache_range((addr_t) RX_GPD_POOL[ep_num].cur_ptr_gpd, sizeof(struct _TGPD));
#if 0
	    MSG(USB, "Rx gpd info { HWO %d, Next_GPD %x ,DataBufferLength %d, DataBuffer %x, Recived Len %d, Endpoint %d, TGL %d, ZLP %d}\n",
	                    (kal_uint32)TGPD_GET_FLAG(gpd), (kal_uint32)TGPD_GET_NEXT(gpd),
	                    (kal_uint32)TGPD_GET_DataBUF_LEN(gpd), (kal_uint32)TGPD_GET_DATA(gpd),
	                    (kal_uint32)TGPD_GET_BUF_LEN(gpd), (kal_uint32)TGPD_GET_EPaddr(gpd),
	                    (kal_uint32)TGPD_GET_TGL(gpd), (kal_uint32)TGPD_GET_ZLP(gpd));
#endif
	}
	else // TX FLOW
	{
		arch_clean_invalidate_cache_range((addr_t) TX_GPD_POOL[ep_num].cur_ptr_gpd, sizeof(struct _TGPD));
		TGPD *gpd = TX_GPD_POOL[ep_num].cur_ptr_gpd;
		//MSG(USB, "TX GPD data_length %d\n", length);

		TGPD_SET_DATA(gpd, pBuf);
		TGPD_SET_BUF_LEN(gpd, length);

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
//		MSG(USB, "Tx Next GPD 0x%x\n",(kal_uint32)TX_GPD_POOL[ep_num].cur_ptr_gpd);
		TGPD_CLR_FLAGS_HWO(TX_GPD_POOL[ep_num].cur_ptr_gpd);
		TGPD_SET_NEXT(gpd, TX_GPD_POOL[ep_num].cur_ptr_gpd);

		TGPD_SET_IOC(gpd);
		TGPD_SET_CHKSUM_HWO(gpd, 16);
		TGPD_SET_FLAGS_HWO(gpd);
		arch_clean_invalidate_cache_range((addr_t) TX_GPD_POOL[ep_num].cur_ptr_gpd, sizeof(struct _TGPD));
	}
}

void USB_QMU_Resume(kal_uint8 ep_num, USB_DIR dir)
{
	if(dir == USB_RX)
	{
//		MSG(USB, "USB_QMU_Resume USB_RX %x \n", (USB_HW_QMU_OFF + MGC_O_QMU_RQCSR(ep_num)));
		MGC_WriteQMU32(MGC_O_QMU_RQCSR(ep_num), DQMU_QUE_RESUME);
 	} 
	else
	{
//		MSG(USB, "USB_QMU_Resume USB_TX %x \n", (USB_HW_QMU_OFF + MGC_O_QMU_TQCSR(ep_num)));
		MGC_WriteQMU32(MGC_O_QMU_TQCSR(ep_num), DQMU_QUE_RESUME);
 	}

//	return 0;
}
kal_bool USB_IS_QMU_Init_Done(void)
{
	//MSG(USB, "USB_IS_QMU_Init_Done QMU, g_bQMU_Init_Done : %x\n", g_bQMU_Init_Done);
	return g_bQMU_Init_Done;
}

kal_uint8 PDU_calcCksum(kal_uint8 *data, int len)
{
	kal_uint8 *uDataPtr, ckSum;
	int i;

	*(data + 1) = 0x0;
	uDataPtr = data;
	ckSum = 0;
	for (i = 0; i < len; i++)
		ckSum += *(uDataPtr + i);

	return 0xFF - ckSum;
}

int USB_Is_QMU_Enabled(kal_int32 Q_num, USB_DIR dir)
{
	if(dir == USB_RX){
		if(MGC_ReadQUCS32(MGC_O_QUCS_USBGCSR)&(USB_QMU_Rx_EN(Q_num)))
			return KAL_TRUE;
	}
	else{
		if(MGC_ReadQUCS32(MGC_O_QUCS_USBGCSR)&(USB_QMU_Tx_EN(Q_num)))
			return KAL_TRUE;
	}
	return KAL_FALSE;
}

void USB_QMU_Resource_reset(kal_uint32 Q_num, USB_DIR dir)
{
	if(dir == USB_RX)
	{
		RX_GPD_POOL[Q_num].cur_ptr_gpd= &(RX_GPD_POOL[Q_num].gpd[0]);
		RX_GPD_POOL[Q_num].cur_gpd_index = 0;

		TGPD_CLR_FLAGS_HWO(RX_GPD_POOL[Q_num].cur_ptr_gpd);
		MGC_WriteQMU32(MGC_O_QMU_RQSAR(Q_num), &(RX_GPD_POOL[Q_num].gpd[0]));	
	}
	else
	{
		TX_GPD_POOL[Q_num].cur_ptr_gpd= &(TX_GPD_POOL[Q_num].gpd[0]);
		TX_GPD_POOL[Q_num].cur_gpd_index = 0;

		TGPD_CLR_FLAGS_HWO(TX_GPD_POOL[Q_num].cur_ptr_gpd);
		MGC_WriteQMU32(MGC_O_QMU_TQSAR(Q_num), &(TX_GPD_POOL[Q_num].gpd[0]));
	}
	
}


void USB_Start_QMU(kal_int32 Q_num, USB_DIR dir)
{
	kal_uint32 QCR;
	kal_int32 isZLP = 1;
	kal_int32 isCSCheck = 1;
	kal_int32 isEmptyCheck = 0;
			
	if(dir == USB_RX)
	{
		if (isCSCheck)
		{
			QCR = MGC_ReadQMU32(MGC_O_QMU_QCR0);
			MGC_WriteQMU32(MGC_O_QMU_QCR0, QCR | DQMU_RQCS_EN(Q_num));
		}

		//if (isZLP)
		//{
		//	QCR = MGC_ReadQMU32(MGC_O_QMU_QCR3);
		//	MGC_WriteQMU32(MGC_O_QMU_QCR3, QCR | DQMU_RX_ZLP(Q_num));
		//}

		MGC_WriteQIRQ32(MGC_O_QIRQ_QIMCR, DQMU_M_RX_DONE(Q_num)|DQMU_M_RQ_EMPTY|DQMU_M_RXQ_ERR|DQMU_M_RXEP_ERR);

		if(isEmptyCheck){
			MGC_WriteQIRQ32(MGC_O_QIRQ_REPEMPMCR, DQMU_M_RX_EMPTY(Q_num));			
		}else{
			MGC_WriteQIRQ32(MGC_O_QIRQ_QIMSR, DQMU_M_RQ_EMPTY);
		}

		QCR = DQMU_M_RX_LEN_ERR(Q_num);
		QCR |= isCSCheck ? DQMU_M_RX_GPDCS_ERR(Q_num) : 0;
		QCR |= isZLP ? DQMU_M_RX_ZLP_ERR(Q_num) : 0;
		MGC_WriteQIRQ32(MGC_O_QIRQ_RQEIMCR, QCR);


		MGC_WriteQIRQ32(MGC_O_QIRQ_REPEIMCR, DQMU_M_RX_EP_ERR(Q_num));
		MGC_WriteQMU32(MGC_O_QMU_RQCSR(Q_num), DQMU_QUE_START);
	}
	else		// TX FLOW
	{
		if(isCSCheck){
			QCR= MGC_ReadQMU32(MGC_O_QMU_QCR0);
			MGC_WriteQMU32(MGC_O_QMU_QCR0, QCR|DQMU_TQCS_EN(Q_num));
		}

		if(isZLP){
			QCR = MGC_ReadQMU32(MGC_O_QMU_QCR2);
			MGC_WriteQMU32(MGC_O_QMU_QCR2, QCR|DQMU_TX_ZLP(Q_num));
		}

		MGC_WriteQIRQ32(MGC_O_QIRQ_QIMCR, DQMU_M_TX_DONE(Q_num)|DQMU_M_TQ_EMPTY|DQMU_M_TXQ_ERR|DQMU_M_TXEP_ERR);

		if(isEmptyCheck){			
			MGC_WriteQIRQ32(MGC_O_QIRQ_TEPEMPMCR, DQMU_M_TX_EMPTY(Q_num));
		}else{
			MGC_WriteQIRQ32(MGC_O_QIRQ_QIMSR, DQMU_M_TQ_EMPTY);
		}

		QCR = DQMU_M_TX_LEN_ERR(Q_num);
		QCR |= isCSCheck ?
			DQMU_M_TX_GPDCS_ERR(Q_num) | DQMU_M_TX_BDCS_ERR(Q_num) : 0;
		MGC_WriteQIRQ32(MGC_O_QIRQ_TQEIMCR, QCR);

		MGC_WriteQIRQ32(MGC_O_QIRQ_TEPEIMCR, DQMU_M_TX_EP_ERR(Q_num));
		MGC_WriteQMU32(MGC_O_QMU_TQCSR(Q_num), DQMU_QUE_START);

	}	
}

void USB_Flush_EP_Csr(kal_int32 Q_num, USB_DIR dir)
{
	kal_uint32 csr, wCsr;
	DRV_WriteReg8(USB_INDEX_V3, Q_num);

	if(dir == USB_RX)
	{
		csr = DRV_Reg(USB_RXCSR);
		csr |= USB_RXCSR_FLUSHFIFO | USB_RXCSR_RXPKTRDY;

		DRV_WriteReg(USB_RXCSR, csr);
		DRV_WriteReg(USB_RXCSR, csr | USB_RXCSR_CLRDATATOG);
	}
	else
	{
		csr = DRV_Reg(USB_TXCSR);

		if (csr&USB_TXCSR_TXPKTRDY)
		{
			wCsr = csr | USB_TXCSR_FLUSHFIFO | USB_TXCSR_TXPKTRDY;
			DRV_WriteReg(USB_TXCSR, wCsr);
		}

		csr |= USB_TXCSR_FLUSHFIFO &~ USB_TXCSR_TXPKTRDY;

		DRV_WriteReg(USB_TXCSR, csr);
		DRV_WriteReg(USB_TXCSR, csr | USB_TXCSR_CLRDATATOG);
		DRV_WriteReg(USB_INTRTX, 1<<Q_num);
	}
}
void USB_EP_Disable_InterR(kal_int32 Q_num, USB_DIR dir)
{
	kal_uint32 csr/*, wCsr*/;
	kal_uint32 intr_e;

	 DRV_WriteReg8(USB_INDEX_V3, Q_num);

	csr = 0;
	intr_e =0;
	if(dir == USB_RX)
	{
		//DRV_WriteReg(USB_RXMAXP, USB_BULK_FIFO_UNIT_SIZE);
		csr |= USB_RXCSR_DMAREQENAB;
		//csr &= ~USB_RXCSR_ISO;
		//csr &= ~USB_RXCSR_DISNYET;

		DRV_WriteReg(USB_RXCSR, csr);

		//turn off intrRx
		intr_e = DRV_Reg(USB_INTRRXE);
		intr_e = intr_e & (~(1<<(Q_num)));
		DRV_WriteReg(USB_INTRRXE, intr_e);
	}
	else
	{
		//DRV_WriteReg(USB_TXMAXP, USB_BULK_FIFO_UNIT_SIZE);

		csr |= USB_TXCSR_DMAREQENAB;
		//csr &= ~USB_TXCSR_ISO;

		DRV_WriteReg(USB_TXCSR, csr);

		//turn off intrTx
		intr_e = DRV_Reg(USB_INTRTXE);
		intr_e = intr_e & (~(1<<(Q_num)));
		DRV_WriteReg(USB_INTRTXE, intr_e);
	}
}

void USB_QMU_Init(void)
{
	kal_uint32 i;

	for(i=1; i<=MAX_EP_NUM; i++){
		USB_QMU_Resource_reset(i, USB_RX);
		MGC_WriteQUCS32(MGC_O_QUCS_USBGCSR,  MGC_ReadQUCS32(MGC_O_QUCS_USBGCSR)|(USB_QMU_Rx_EN(i)));

		USB_QMU_Resource_reset(i, USB_TX);
		MGC_WriteQUCS32(MGC_O_QUCS_USBGCSR,  MGC_ReadQUCS32(MGC_O_QUCS_USBGCSR)|(USB_QMU_Tx_EN(i)));
	}
}
#endif

void USB_QMU_Init_and_Start(void)
{
	kal_uint32 i;

#if defined(USB_QMU)
	MSG(USB, "USB QMU Init and Start\n");
	if(g_bQMU_Init_Done == KAL_TRUE)
	{
		MSG(USB, "QMU already inited by DA\n");
		return;
	}

	USB_QMU_Init();
	for(i=1; i<=MAX_EP_NUM; i++){	
		USB_Start_QMU(i, USB_RX);
		USB_Start_QMU(i, USB_TX);
	}

	for(i=1; i<=MAX_EP_NUM; i++){	
		USB_EP_Disable_InterR(i, USB_RX);
		USB_EP_Disable_InterR(i, USB_TX);
	}

	g_bQMU_Init_Done = KAL_TRUE;
#else
	MSG(USB, "USB QMU Not Support\n");
#endif
}


void USB_Enable_HS(int enable_hs)
{
  #ifdef  USB_UT
   	g_enable_hs = 1 ;
  #else
  	 g_enable_hs = enable_hs ? 1 : 0;
  #endif
}

void USB_Reset_FIFO(void)
{
	g_FIFOadd = USB_FIFO_START_ADDRESS;
}

void USB_Enable_DMA(kal_bool enabled)
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
#if	defined(USB_IP_V2)	
		g_UsbDrvInfo.dma_tx_ep = 0;
		g_UsbDrvInfo.dma_res_tx_ep = 0;
		g_UsbDrvInfo.fast_mode_tx_ep = 0;

		/* BDT */
		WRITE_EP0_BDT(USB_BDT_RX, USB_BDT_EVEN, (USB_BDT_PTR)USB_BDT_RX0_EVEN);
		WRITE_EP0_BDT(USB_BDT_RX, USB_BDT_ODD, (USB_BDT_PTR)USB_BDT_RX0_ODD);
		WRITE_EP0_BDT(USB_BDT_TX, USB_BDT_EVEN, (USB_BDT_PTR)USB_BDT_TX0_EVEN);
		WRITE_EP0_BDT(USB_BDT_TX, USB_BDT_ODD, (USB_BDT_PTR)USB_BDT_TX0_ODD);
		WRITE_EPN_BDT(1, USB_BDT_RX, (USB_BDT_PTR)USB_BDT_RX1);
		WRITE_EPN_BDT(1, USB_BDT_TX, (USB_BDT_PTR)USB_BDT_TX1);
		WRITE_EPN_BDT(2, USB_BDT_RX, (USB_BDT_PTR)USB_BDT_RX2);
		WRITE_EPN_BDT(2, USB_BDT_TX, (USB_BDT_PTR)USB_BDT_TX2);
		WRITE_EPN_BDT(3, USB_BDT_RX, (USB_BDT_PTR)USB_BDT_RX3);
		WRITE_EPN_BDT(3, USB_BDT_TX, (USB_BDT_PTR)USB_BDT_TX3);

		/* EP0 parametes */
		g_UsbDrvInfo.ep0_rx_even_odd = 0;
		g_UsbDrvInfo.ep0_tx_even_odd = 0;
  		DRV_WriteReg8(USB_CTL, VUSB_CTL_ODD_RST|VUSB_CTL_USB_EN);
		DRV_WriteReg8(USB_CTL, VUSB_CTL_USB_EN);
		g_UsbDrvInfo.ep0_current_dir = 0;
		g_UsbDrvInfo.ep0_tx_data01 = (READ_EP0_BDT_PID(USB_BDT_TX, g_UsbDrvInfo.ep0_tx_even_odd)& 0x00000040)>>VUSB_BDT_DATA01_SHIFT;
		g_UsbDrvInfo.ep0_tx_data01^= 1;
		g_UsbDrvInfo.ep0_transaction_type = USB_EP0_TRANS_NONE;

		/* EPn parametes */
		for(index=0; index< MAX_INTR_EP_NUM; index++)
		{
			g_UsbDrvInfo.ep_in_enb_state[index] = USB_EP_DIS;
			g_UsbDrvInfo.ep_out_enb_state[index] = USB_EP_DIS;
			g_UsbDrvInfo.ep_in_max_data_size[index] = 0;
			g_UsbDrvInfo.ep_out_max_data_size[index] = 0;
			g_UsbDrvInfo.ep_out_data01[index] = (READ_EPN_BDT_PID(index+1, USB_BDT_RX)& 0x00000040)>>VUSB_BDT_DATA01_SHIFT;
			g_UsbDrvInfo.ep_out_data01[index]^=1;
			g_UsbDrvInfo.ep_in_data01[index] = (READ_EPN_BDT_PID(index+1, USB_BDT_TX)& 0x00000040)>>VUSB_BDT_DATA01_SHIFT;
			g_UsbDrvInfo.ep_in_data01[index]^=1;
		}
#endif
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
#if	defined(USB_IP_V3) || defined(USB_IP_V4)
	//clear interrupt. write 0 clear
//	DRV_WriteReg32(USB_DMAINTR, 0x00);

#elif	defined(USB_IP_V2)
		g_UsbDrvInfo.dma_tx_ep = 0;
		g_UsbDrvInfo.dma_res_tx_ep = 0;
		g_UsbDrvInfo.fast_mode_tx_ep = 0;

		/* EP0 parametes */
		g_UsbDrvInfo.ep0_rx_even_odd = 0;
		g_UsbDrvInfo.ep0_tx_even_odd = 0;
		g_UsbDrvInfo.ep0_current_dir = 0;
		g_UsbDrvInfo.ep0_tx_data01 = 0;
		g_UsbDrvInfo.ep0_transaction_type = USB_EP0_TRANS_NONE;

		/* EPn parametes */
		for(index=0; index< MAX_INTR_EP_NUM; index++)
		{
			g_UsbDrvInfo.ep_in_enb_state[index] = USB_EP_DIS;
			g_UsbDrvInfo.ep_out_enb_state[index] = USB_EP_DIS;
			g_UsbDrvInfo.ep_in_max_data_size[index] = 0;
			g_UsbDrvInfo.ep_out_max_data_size[index] = 0;
			g_UsbDrvInfo.ep_in_data01[index] = 0;
			g_UsbDrvInfo.ep_out_data01[index] = 0;
		}	
#endif   	
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

/* set unmask variable to control if unmask USB interruput when leaving USB HISR*/
void USB_Set_UnMask_Irq(kal_bool set)
{
	g_UsbDrvInfo.is_unMaskUSB = set;
}

/* get the irq status */
kal_bool USB_Get_UnMask_Irq(void)
{
	return g_UsbDrvInfo.is_unMaskUSB;
}

/* get variable about if USB HISR has chance to execute at the current time */
kal_bool USB_Get_Pro_USB_HISR(void)
{
	return KAL_TRUE;
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
#if defined(USB_IP_V3) || defined(USB_IP_V4)
	if(bset == KAL_TRUE)
	{	
//0x612 [0]  = 1'b0        ;// force_opmode
//0x612 [1]  = 1'b0        ;//force_term_select
//0x612 [3]  = 1'b0       ;// force_xcvr_select 
//0x613[1:0] = 2'b00    ;//op_mode
//0x613[[2]    =1'b0      ://term_sel            
//0x613[4:5] = 2'b00    ://xcvr_select 
//		DRV_WriteReg8(0x60000612,(DRV_Reg8(0x60000612)&0xF4));		
//		DRV_WriteReg8(0x60000613,(DRV_Reg8(0x60000613)&0xC8));			
		DRV_WriteReg8(USB_POWER_V3, (DRV_Reg8(USB_POWER_V3)|USB_POWER_SOFTCONN));
	}	
	else
	{	
		DRV_WriteReg8(USB_POWER_V3, (DRV_Reg8(USB_POWER_V3)&(~USB_POWER_SOFTCONN)));
//0x612 [0]  = 1'b1        ;// force_opmode
//0x612 [1]  = 1'b1        ;//force_term_select
//0x612 [3]  = 1'b1       ;// force_xcvr_select 
//0x613[1:0] = 2'b01    ;//op_mode
//0x613[[2]    =1'b1      ://term_sel            
//0x613[4:5] = 2'b01    ://xcvr_select      	
//		DRV_WriteReg8(0x60000612,(DRV_Reg8(0x60000612)|0x0B));		
//		DRV_WriteReg8(0x60000613,(DRV_Reg8(0x60000613)&0xC8)|0x15);			
	}

#elif defined(USB_IP_V1_PLUS)
	
#else   //USB_IP_V1 & USB_IP_V2 
//	if(bset == KAL_TRUE)
//		USB_PowerControl(KAL_TRUE);
//	else
//		USB_PowerControl(KAL_FALSE);  //QQ
#endif
}

/************************************************************
	system ctrl functions
*************************************************************/

/* reset hw power*/
void USB_Reset_Drv(void)
{
#if	defined(USB_IP_V3) || defined(USB_IP_V4) 
		/* Enable software reset, USB IP only can be reset by SW when detecting reset signal from bus */
    #if defined(USB_IP_V3)
		DRV_WriteReg16(USB_SWRST, USB_SWRST_DISUSBRESET);
    #elif defined(USB_IP_V4)
        DRV_WriteReg16(USB_SWRST_V4, USB_SWRST_DISUSBRESET);
    #endif
		DRV_WriteReg8(USB_RSTINFO, 0xA6);
		/* Because softconn has be decided to set or not */
		DRV_WriteReg8(USB_POWER_V3, (DRV_Reg8(USB_POWER_V3)|USB_POWER_ISOUPDATE|USB_POWER_ENABLESUSPENDM));
        if (g_enable_hs)
    		DRV_WriteReg8(USB_POWER_V3, (DRV_Reg8(USB_POWER_V3)|USB_POWER_HSENAB));
        else
            DRV_WriteReg8(USB_POWER_V3, (DRV_Reg8(USB_POWER_V3)&(~USB_POWER_HSENAB)));

#elif	defined(USB_IP_V2)  	 
		WRITE_EP0_BDT_ADDR(USB_BDT_RX, USB_BDT_EVEN, USB_FIFO_RX0_EVEN);
		WRITE_EP0_BDT_ADDR(USB_BDT_RX, USB_BDT_ODD, USB_FIFO_RX0_ODD);
		WRITE_EP0_BDT_ADDR(USB_BDT_TX, USB_BDT_EVEN, USB_FIFO_TX0_ODD);
		WRITE_EP0_BDT_ADDR(USB_BDT_TX, USB_BDT_ODD, USB_FIFO_TX0_ODD);
		WRITE_EPN_BDT_ADDR(1, USB_BDT_RX, USB_FIFO_RX1);
		WRITE_EPN_BDT_ADDR(1, USB_BDT_TX, USB_FIFO_TX1);
		WRITE_EPN_BDT_ADDR(2, USB_BDT_RX, USB_FIFO_RX2);
		WRITE_EPN_BDT_ADDR(2, USB_BDT_TX, USB_FIFO_TX2);
		WRITE_EPN_BDT_ADDR(3, USB_BDT_RX, USB_FIFO_RX3);
		WRITE_EPN_BDT_ADDR(3, USB_BDT_TX, USB_FIFO_TX3);
	
		DRV_WriteReg8(USB_CTL, 0);
		DRV_WriteReg8(USB_ENDPT_CTL(0), VUSB_ENDPT_DISABLE);
		DRV_WriteReg8(USB_ENDPT_CTL(1), VUSB_ENDPT_DISABLE);
		DRV_WriteReg8(USB_ENDPT_CTL(2), VUSB_ENDPT_DISABLE);
		DRV_WriteReg8(USB_ENDPT_CTL(3), VUSB_ENDPT_DISABLE);
#elif	defined(USB_IP_V1)||defined(USB_IP_V1_PLUS)  	
		/* enable software reset, USB IP only can be reset by SW when detecting reset signal from bus */
		DRV_WriteReg8(USB_POWER, USB_POWER_SETSUSPEND|USB_POWER_SWRSTENAB);
#endif	
}

/* initialize drv registers, including system global interrupt and EP0 interrupt*/
void USB_Initialize_Drv(void)
{
#if	defined(USB_IP_V3) || defined(USB_IP_V4)	
   		/* software reset */
    #if defined(USB_IP_V3)
		DRV_WriteReg16(USB_SWRST, (USB_SWRST_DISUSBRESET|USB_SWRST_SWRST));
    #elif defined(USB_IP_V4)
		DRV_WriteReg16(USB_SWRST_V4, (USB_SWRST_DISUSBRESET|USB_SWRST_SWRST));
    #endif
		/* enable system interrupts, but disable all ep interrupts */
		USB_EnSysIntr();
		DRV_WriteReg8(USB_INDEX_V3, 0);
		/* flush ep0 FIFO */
		DRV_WriteReg16(USB_CSR0_V3, USB_CSR0_FLUSHFIFO);
		/* enable EP0 interrupt */
		USB_EP0En();
		/* support QMU */
#if defined(USB_QMU)
		USB_QMU_Init_and_Start();
#endif
#elif	defined(USB_IP_V2)  	 
		kal_uint32 index;

		/* This hardware has special hardware token done of 4 bytes deep FIFO  ,Clear the status FIFOs  */
		DRV_WriteReg8(USB_INT_STAT, VUSB_INT_STAT_TOKEN_DONE);
		DRV_WriteReg8(USB_INT_STAT, VUSB_INT_STAT_TOKEN_DONE);
		DRV_WriteReg8(USB_INT_STAT, VUSB_INT_STAT_TOKEN_DONE);
		DRV_WriteReg8(USB_INT_STAT, VUSB_INT_STAT_TOKEN_DONE);

		/* FIFO deep 4*/
		DRV_WriteReg8(USB_FM_ERR_STAT, VUSB_FM_ERR_STAT_TOKEN_DONE);
		DRV_WriteReg8(USB_FM_ERR_STAT, VUSB_FM_ERR_STAT_TOKEN_DONE);
		DRV_WriteReg8(USB_FM_ERR_STAT, VUSB_FM_ERR_STAT_TOKEN_DONE);
		DRV_WriteReg8(USB_FM_ERR_STAT, VUSB_FM_ERR_STAT_TOKEN_DONE);

		/* clear all interrupts*/
		DRV_WriteReg8(USB_INT_STAT, 0xff);

			/* clear all endpoint ctrl */
		for(index=0; index<=MAX_INTR_EP_NUM; index++)
		DRV_WriteReg8(USB_ENDPT_CTL(index), 0);
	
			/* USB addr=0 (default) */
		DRV_WriteReg8(USB_ADDR, 0x0);

		WRITE_EP0_BDT_PID(USB_BDT_RX, USB_BDT_EVEN, 
							(USB_EP0_MAXP<<VUSB_BDT_BC_SHIFT)|VUSB_BDT_OWNS_BIT);
		WRITE_EP0_BDT_PID(USB_BDT_RX, USB_BDT_ODD, (USB_EP0_MAXP<<VUSB_BDT_BC_SHIFT)|VUSB_BDT_OWNS_BIT);	
		WRITE_EP0_BDT_PID(USB_BDT_TX, USB_BDT_EVEN, USB_EP0_MAXP<<VUSB_BDT_BC_SHIFT);
		WRITE_EP0_BDT_PID(USB_BDT_TX, USB_BDT_ODD, USB_EP0_MAXP<<VUSB_BDT_BC_SHIFT);
		WRITE_EPN_BDT_PID(1, USB_BDT_RX, USB_BULK_MAXP<<VUSB_BDT_BC_SHIFT);
		WRITE_EPN_BDT_PID(1, USB_BDT_TX, USB_BULK_MAXP<<VUSB_BDT_BC_SHIFT);
		WRITE_EPN_BDT_PID(2, USB_BDT_RX, USB_BULK_MAXP<<VUSB_BDT_BC_SHIFT);
		WRITE_EPN_BDT_PID(2, USB_BDT_TX, USB_BULK_MAXP<<VUSB_BDT_BC_SHIFT);
		WRITE_EPN_BDT_PID(3, USB_BDT_RX, USB_INTR_MAXP<<VUSB_BDT_BC_SHIFT);
		WRITE_EPN_BDT_PID(3, USB_BDT_TX, USB_INTR_MAXP<<VUSB_BDT_BC_SHIFT);

		DRV_WriteReg8(USB_BDT_PAGE_01, 0xBD);
		DRV_WriteReg8(USB_BDT_PAGE_02, 0xBD);
		DRV_WriteReg8(USB_BDT_PAGE_03, 0xBD);

		DRV_WriteReg8(USB_CTL, VUSB_CTL_ODD_RST|VUSB_CTL_USB_EN);
		DRV_WriteReg8(USB_CTL, VUSB_CTL_USB_EN);
		/* disable Fast mode*/
		DRV_WriteReg8(USB_FM_CTL, (DRV_Reg8(USB_FM_CTL)&(~VUSB_FM_CTL_FMENB)));
		DRV_WriteReg8(USB_FM_PKT_NUML, 0);
		DRV_WriteReg8(USB_FM_PKT_NUMH, 0);
		DRV_WriteReg8(USB_FM_PKT_CNTL, 0);
		DRV_WriteReg8(USB_FM_PKT_CNTH, 0);
		USB_EnSysIntr();
		USB_EP0En();	/* enable EP0 */
#elif	defined(USB_IP_V1)||defined(USB_IP_V1_PLUS) 	 
		/* software reset */
		DRV_WriteReg8(USB_RSTCTRL, USB_RSTCTRL_SWRST);
		DRV_WriteReg8(USB_RSTCTRL, 0);	
		USB_EnSysIntr();
		USB_EP0En();	/* enable EP0 */
#endif   	
}


/* enable system global interrupt*/
void USB_EnSysIntr(void)
{
#if	defined(USB_IP_V3) || defined(USB_IP_V4)
	DRV_WriteReg16(USB_INTRTXE, 0);
	DRV_WriteReg16(USB_INTRRXE, 0);
	DRV_WriteReg8(USB_INTRUSBE_V3, 0);
	DRV_WriteReg8(USB_INTRUSBE_V3, (USB_INTRUSBE_SUSPEND|USB_INTRUSBE_RESUME|USB_INTRUSBE_RESET|USB_INTRUSBE_DISCON));
#elif	defined(USB_IP_V2) 
	DRV_WriteReg8(USB_INT_ENB, 0xff&(~(VUSB_INT_ENB_SOF|VUSB_INT_ENB_RESUME)));
	DRV_WriteReg8(USB_ERR_ENB, 0xff);
	DRV_WriteReg8(USB_PHY_EXTRA, VUSB_PHY_RESUME_INT_ENB);
#elif	defined(USB_IP_V1)||defined(USB_IP_V1_PLUS)     
	DRV_WriteReg8(USB_INTRIN1E, 0);
	DRV_WriteReg8(USB_INTROUT1E, 0);
	DRV_WriteReg8(USB_INTRUSBE, 0);
	DRV_WriteReg8(USB_INTRUSBE, (USB_INTRUSBE_SUSPEND|USB_INTRUSBE_RESUME|USB_INTRUSBE_RESET));
#endif     
}

/* resume device */
void USB_Resume(void)
{
}

/* suspend device */
void USB_Suspend(void)
{
}

void USB_SetAddress(kal_uint8 addr, USB_SET_ADDR_STATE state)
{
#if	defined(USB_IP_V3) || defined(USB_IP_V4)	
		if(state == USB_SET_ADDR_STATUS)	
			DRV_WriteReg8(USB_FADDR_V3, addr);
#elif	defined(USB_IP_V2)     
		if(state==USB_SET_ADDR_STATUS)	
			DRV_WriteReg8(USB_ADDR, addr);
#elif	defined(USB_IP_V1)||defined(USB_IP_V1_PLUS)     
		if(state==USB_SET_ADDR_DATA)	
			DRV_WriteReg8(USB_FADDR, addr);
#endif      
}

kal_uint8 USB_ReadAddress(void)
{
#if	defined(USB_IP_V3) || defined(USB_IP_V4)	
		return DRV_Reg16(USB_FADDR_V3);
#elif	defined(USB_IP_V2)	   
		return DRV_Reg16(USB_ADDR);
#elif	defined(USB_IP_V1)||defined(USB_IP_V1_PLUS) 
		return DRV_Reg16(USB_FADDR);
#endif		
}


kal_uint16 USB_GetFrameCount(void)
{
	kal_uint16 sof;
#if	defined(USB_IP_V3) || defined(USB_IP_V4)
		sof =  DRV_Reg16(USB_FRAME);
#elif	defined(USB_IP_V2)	
		sof = DRV_Reg8(USB_FRM_NUMH)<<8 + DRV_Reg8(USB_FRM_NUML);
#elif	defined(USB_IP_V1)||defined(USB_IP_V1_PLUS)
		sof = DRV_Reg8(USB_FRAME2)<<8 + DRV_Reg8(USB_FRAME1);
#endif	
	return sof;
}


/************************************************************
	EP ctrl functinos
*************************************************************/

/* initialize EP IN */
void USB_InEPInit(kal_uint8 epno,kal_uint16 data_size, USB_ENDPT_TXFER_TYPE type)
{
#if defined(USB_IP_V2)
    kal_uint8	epctl_value; 	
#endif
    USB_Dbg_Trace(DEBUG_USB_InEPInit, 0, 0,0);
    
#if	defined(USB_IP_V3) || defined(USB_IP_V4)
	DRV_WriteReg8(USB_INDEX_V3, epno);
	/* Double buffers, so flush twice */
	DRV_WriteReg16(USB_TXCSR, 0x00);
	DRV_WriteReg16(USB_TXCSR, (USB_TXCSR_FLUSHFIFO|USB_TXCSR_CLRDATATOG));
	DRV_WriteReg16(USB_TXCSR, (USB_TXCSR_FLUSHFIFO|USB_TXCSR_CLRDATATOG));
	DRV_WriteReg16(USB_TXMAXP, USB_BULK_FIFO_UNIT_SIZE); //data_size

	/* Set FIFO address here */
	if(type == USB_ENDPT_BULK)
	{
#if defined(USB_DOUBLE_BUF)
		/* double  buffer */
		DRV_WriteReg8(USB_TXFIFOSZ, (USB_FIFOSZ_SIZE_512|USB_FIFOSZ_DPB));
		DRV_WriteReg16(USB_TXFIFOADD, (g_FIFOadd/8));
		g_FIFOadd += USB_BULK_FIFO_UNIT_SIZE*2;
#else
		/* single  buffer */
		DRV_WriteReg8(USB_TXFIFOSZ, (USB_FIFOSZ_SIZE_512));
		DRV_WriteReg16(USB_TXFIFOADD, (g_FIFOadd/8));
		g_FIFOadd += USB_BULK_FIFO_UNIT_SIZE;
#endif
	}
	else if (type == USB_ENDPT_INTR)
	{
		DRV_WriteReg8(USB_TXFIFOSZ, USB_FIFOSZ_SIZE_64);
		DRV_WriteReg16(USB_TXFIFOADD, (kal_uint16)(g_FIFOadd/8));
		g_FIFOadd += USB_INTR_FIFO_UNIT_SIZE; //64
	}
	
#elif defined(USB_IP_V2)

		switch(type)
		{
		case USB_ENDPT_BULK:
			epctl_value = VUSB_ENDPT_BULK_TX;
			break;
		
		case USB_ENDPT_INTR:
			epctl_value = VUSB_ENDPT_BULK_TX;
			break;
		/* not support */
		case USB_ENDPT_CTRL:
		case USB_ENDPT_ISO:
			ASSERT(0);
			break;
		}
    	 	 g_UsbDrvInfo.ep_in_max_data_size[epno-1] =  data_size;
		DRV_WriteReg8(USB_ENDPT_CTL(epno), DRV_Reg8(USB_ENDPT_CTL(epno))|epctl_value);
#endif	
}


/* initialize EP OUT */
void USB_OutEPInit(kal_uint8 epno,kal_uint16 data_size, USB_ENDPT_TXFER_TYPE type)
{
#if defined(USB_IP_V2)
		kal_uint8	epctl_value;
#endif
		USB_Dbg_Trace(DEBUG_USB_OutEPInit, 0, 0,0);	

#if	defined(USB_IP_V3) || defined(USB_IP_V4)	
	DRV_WriteReg8(USB_INDEX_V3, epno);
	DRV_WriteReg16(USB_RXCSR, 0x00);
	DRV_WriteReg16(USB_RXCSR, (USB_RXCSR_FLUSHFIFO|USB_RXCSR_CLRDATATOG));
	DRV_WriteReg16(USB_RXCSR, (USB_RXCSR_FLUSHFIFO|USB_RXCSR_CLRDATATOG));
	DRV_WriteReg16(USB_RXMAXP, USB_BULK_FIFO_UNIT_SIZE);

	/* Set FIFO address here */
	if(type == USB_ENDPT_BULK)
	{
#if defined(USB_DOUBLE_BUF)
		//double buffer
		DRV_WriteReg8(USB_RXFIFOSZ, (USB_FIFOSZ_SIZE_512|USB_FIFOSZ_DPB));
		DRV_WriteReg16(USB_RXFIFOADD, (g_FIFOadd/8));
		g_FIFOadd += USB_BULK_FIFO_UNIT_SIZE*2;
#else
		//single buffer
		DRV_WriteReg8(USB_RXFIFOSZ, (USB_FIFOSZ_SIZE_512));
		DRV_WriteReg16(USB_RXFIFOADD, (g_FIFOadd/8));
		g_FIFOadd += USB_BULK_FIFO_UNIT_SIZE;
#endif
	}

#elif	defined(USB_IP_V2)	

		switch(type)
		{
		case USB_ENDPT_BULK:
			epctl_value = VUSB_ENDPT_BULK_RX;
			break;
		
		case USB_ENDPT_INTR:
			epctl_value = VUSB_ENDPT_BULK_RX;
			break;
		/* not support */
		case USB_ENDPT_CTRL:
		
		case USB_ENDPT_ISO:
			ASSERT(0);
			break;
		}
		g_UsbDrvInfo.ep_out_max_data_size[epno-1] =  data_size;
		DRV_WriteReg8(USB_ENDPT_CTL(epno), DRV_Reg8(USB_ENDPT_CTL(epno))|epctl_value);
#endif	
}
/*  to save memory size (TX/RX setting must be the same)
kal_uint16 USB_InEP_PK_SIZE(kal_uint8 no)
{
}
*/
kal_uint16 USB_OutEP_PK_SIZE(kal_uint8 no)
{
		kal_uint16  register_value;

#if defined(USB_IP_V3) || defined(USB_IP_V4)
		register_value = DRV_Reg16(USB_RXMAXP); //max 512 = 0x200
#elif defined(USB_IP_V2)
		register_value = USB_BULK_MAXP; //fix Packet Size on ARC IP
#elif defined(USB_IP_V1)
		register_value = DRV_Reg8(USB_OUTMAXP)*8;	//max 64 = 0x40
#elif defined(USB_IP_V1_PLUS)
		register_value = DRV_Reg8(USB_OUTMAXP);		//max 64 = 0x40		
#endif

		return register_value;	
}

void USB_EP0En(void)
{
		USB_Dbg_Trace(DEBUG_USB_EP0En, 0, 0,0);
#if	defined(USB_IP_V3) || defined(USB_IP_V4)
		//DRV_WriteReg8(0x72000062, DRV_Reg8(0x72000062)&0x0F);
		DRV_WriteReg16(USB_INTRTXE, DRV_Reg16(USB_INTRTXE)|USB_INTRE_EPEN);
#elif	defined(USB_IP_V2)		
	  DRV_WriteReg8(USB_ENDPT_CTL(0), VUSB_ENDPT_CONTROL);
#elif	defined(USB_IP_V1)||defined(USB_IP_V1_PLUS)  
    DRV_Reg8(USB_INTRIN1E) |= (USB_INTRIN1E_EP0 << 0);
#endif            
}

void USB_InEPEn(kal_uint8 no, kal_bool bDMA)
{
		USB_Dbg_Trace(DEBUG_USB_InEPEn, 0, 0,0);
#if	defined(USB_IP_V3) || defined(USB_IP_V4)
	DRV_WriteReg8(USB_INDEX_V3, no);

	/* double buffers, so flush twice  */
	DRV_WriteReg16(USB_TXCSR, (USB_TXCSR_FLUSHFIFO/*|USB_TXCSR_CLRDATATOG*/));
	DRV_WriteReg16(USB_TXCSR, (USB_TXCSR_FLUSHFIFO/*|USB_TXCSR_CLRDATATOG*/));

	if(bDMA == KAL_TRUE)
	{
		/* Only configure as multiple packet DMA TX mode */
		DRV_WriteReg16(USB_TXCSR, USB_TXCSR_AUTOSET|USB_TXCSR_DMAREQENAB|USB_TXCSR_DMAREQMODE);
		DRV_WriteReg16(USB_INTRTXE, DRV_Reg16(USB_INTRTXE)&~(USB_INTRE_EPEN<<no));
	}
	else
	{
		DRV_WriteReg16(USB_INTRTXE, DRV_Reg16(USB_INTRTXE)|USB_INTRE_EPEN<<no);
	}

#elif	defined(USB_IP_V2)		
    if(bDMA == KAL_TRUE)
    {
	g_UsbDrvInfo.ep_in_enb_state[no-1] = USB_EP_ENB_DMA;
    }	
    else
    {
	g_UsbDrvInfo.ep_in_enb_state[no-1] = USB_EP_ENB_NORMAL;
    }	
#endif      
}

void USB_InEPDis(kal_uint8 no, kal_bool bDMA)
{
#if	defined(USB_IP_V2)
		if(g_UsbDrvInfo.ep_in_enb_state[no-1]==USB_EP_ENB_DMA)
				g_UsbDrvInfo.ep_in_enb_state[no-1] = USB_EP_DIS;
		WRITE_EPN_BDT_PID(no, USB_BDT_TX, 0);
#elif	defined(USB_IP_V1)||defined(USB_IP_V1_PLUS)	
			DRV_WriteReg8(USB_INDEX, no);
	
			if(bDMA==KAL_TRUE)
      			{
					DRV_Reg8(USB_INTRIN1E) = DRV_Reg8(USB_INTRIN1E) & (~(USB_INTRIN1E_EP0 << no));
					DRV_WriteReg8(USB_INCSR2,0);
			}
			else
			{
					DRV_Reg8(USB_INTRIN1E) = DRV_Reg8(USB_INTRIN1E) & (~(USB_INTRIN1E_EP0 << no));
					DRV_WriteReg8(USB_INCSR2,0);
			}
#endif   	
}

void USB_OutEPEn(kal_uint8 no, kal_bool bDMA)
{
 		USB_Dbg_Trace(DEBUG_USB_OutEPEn, 0, 0,0);
#if	defined(USB_IP_V3) || defined(USB_IP_V4)
	DRV_WriteReg8(USB_INDEX_V3, no);

	if(bDMA == KAL_TRUE)
	{
		DRV_WriteReg16(USB_INTRRXE, DRV_Reg16(USB_INTRRXE)&(~(USB_INTRE_EPEN<<no)));

		/* maybe double buffer, so flush twice */
		DRV_WriteReg16(USB_RXCSR, (USB_RXCSR_FLUSHFIFO/*|USB_RXCSR_CLRDATATOG*/));
		DRV_WriteReg16(USB_RXCSR, (USB_RXCSR_FLUSHFIFO/*|USB_RXCSR_CLRDATATOG*/));
	}
	else
	{
		/* maybe double buffer, so flush twice  */
		DRV_WriteReg16(USB_RXCSR, (USB_RXCSR_FLUSHFIFO/*|USB_RXCSR_CLRDATATOG*/));
		DRV_WriteReg16(USB_RXCSR, (USB_RXCSR_FLUSHFIFO/*|USB_RXCSR_CLRDATATOG*/));

		DRV_WriteReg16(USB_INTRRXE, DRV_Reg16(USB_INTRRXE)|USB_INTRE_EPEN<<no);
	}

#elif	defined(USB_IP_V2)		
		if(bDMA == KAL_TRUE)
		{
			g_UsbDrvInfo.ep_out_enb_state[no-1] = USB_EP_ENB_DMA;
		}	
		else
		{
			g_UsbDrvInfo.ep_out_enb_state[no-1] = USB_EP_ENB_NORMAL;
		}
#endif  		 
}
	
void USB_OutEPDis(kal_uint8 no, kal_bool bDMA)
{
#if	defined(USB_IP_V2)	
			g_UsbDrvInfo.ep_out_enb_state[no-1] = USB_EP_DIS;
			WRITE_EPN_BDT_PID(no, USB_BDT_RX, 0);
#elif	defined(USB_IP_V1)||defined(USB_IP_V1_PLUS)			
			DRV_WriteReg8(USB_INDEX, no);
	
			if(bDMA==KAL_TRUE)
			{
					DRV_Reg8(USB_INTROUT1E) = DRV_Reg8(USB_INTROUT1E) & (~(USB_INTROUT1E_EP0 << no));
					DRV_WriteReg8(USB_OUTCSR2,0);
			}
			else
			{
					DRV_Reg8(USB_INTROUT1E) = DRV_Reg8(USB_INTROUT1E) & (~(USB_INTROUT1E_EP0 << no));
					DRV_WriteReg8(USB_OUTCSR2,0);
			}
#endif	
}	

/* Clear EP data01, note that ep0 can not use this function */
void USB_InEPClearData01(kal_uint8 no)
{
#if	defined(USB_IP_V2)	
			g_UsbDrvInfo.ep_in_data01[no-1] = 0;	
#endif			
}

/* Clear EP data01, note that ep0 can not use this function */
void USB_OutEPClearData01(kal_uint8 no)
{
#if	defined(USB_IP_V2)	
			g_UsbDrvInfo.ep_out_data01[no-1] = 0;
#endif			
}

/*  read fifo data from EP nEP*/
void USB_EPFIFORead(kal_uint8 nEP, kal_uint16 nBytes, void *pDst)
{
#if	defined(USB_IP_V3) || defined(USB_IP_V4)	
	kal_uint16  nCount = nBytes;
	kal_uint8   *pby;
	kal_uint32  nAddr;
			
//	USB_Dbg_Trace(DEBUG_USB_HISR_S, DRV_Reg32(TDMA_RTC_32K), 0,0);
				
	DRV_WriteReg8(USB_INDEX_V3,nEP);
	nAddr= USB_EP0+nEP*4;
	pby = (kal_uint8 *)pDst;

#ifdef USB_RX_DMA_MODE_0
	if(nEP == 0) {
    		/* read byte by byte */
    		while (nCount) {
			*pby++ = DRV_Reg8(nAddr); 
			nCount--;
    		}
	} else {
		kal_uint16 chan_num = 1;
		DRV_WriteReg32(USB_DMAADDR(chan_num), pDst);
		DRV_WriteReg32(USB_DMACNT(chan_num), nBytes);
		DRV_WriteReg16(USB_DMACNTL(chan_num), USB_DMACNTL_BURSTMODE_MASK | USB_DMACNTL_DMAEN | (nEP<<4));
		while(DRV_Reg16(USB_DMACNTL(chan_num)) & USB_DMACNTL_DMAEN);
	}
#else
	/* read byte by byte */
	while (nCount) 
	{
		*pby++ = DRV_Reg8(nAddr); 
		nCount--;
	}
#endif
		
//			USB_Dbg_Trace(DEBUG_USB_HISR_E, DRV_Reg32(TDMA_RTC_32K), 0,0);
#elif	defined(USB_IP_V1)||defined(USB_IP_V1_PLUS)			
			kal_uint16  nCount = nBytes;
			kal_uint8   *pby;
			kal_uint32  nAddr;
			
			DRV_WriteReg8(USB_INDEX,nEP);
			nAddr = USB_EP0+nEP*4;
			pby = (kal_uint8 *)pDst;

			/* read byte by byte */
			while (nCount) 
			{
					*pby++ = DRV_Reg8(nAddr); 
					nCount--;
			}
#elif	defined(USB_IP_V2)

			// make sure this endpoint status belong to MCU
			if(nEP==0)
			{
					if((READ_EP0_BDT_PID(USB_BDT_RX, g_UsbDrvInfo.ep0_rx_even_odd)&VUSB_BDT_OWNS_BIT)!=0)
						ASSERT(0);
					memcpy(pDst,(void*)READ_EP0_BDT_ADDR(USB_BDT_RX, g_UsbDrvInfo.ep0_rx_even_odd),nBytes);  					
			}
			else
			{
					if((READ_EPN_BDT_PID(nEP, USB_BDT_RX)&VUSB_BDT_OWNS_BIT)!=0)						
						ASSERT(0);
					memcpy(pDst,(void*)READ_EPN_BDT_ADDR(nEP, USB_BDT_RX), nBytes);			
			}
#endif  	 
//	USB_Dbg_Trace(DEBUG_USB_EPFIFORead, 0, nEP, nBytes);
}


/*  write data to fifo EP nEP*/
void USB_EPFIFOWrite (kal_uint8 nEP, kal_uint16 nBytes, void *pSrc)
{
#if	defined(USB_IP_V3) || defined(USB_IP_V4)
			kal_uint16 nCount = nBytes;
			kal_uint8 *pby;
			kal_uint32 nAddr;
    			  
			DRV_WriteReg8(USB_INDEX_V3,nEP);
			nAddr = USB_EP0+nEP*4;
			pby = (kal_uint8 *)pSrc;

#ifdef USB_TX_DMA_MODE_0
			if(nEP == 0){
				/* write by byte */
				while (nCount) 
				{
					DRV_WriteReg8(nAddr,*pby++);
					nCount--;
				}
			} else {
				kal_uint16 chan_num = 2;
				DRV_WriteReg32(USB_DMAADDR(chan_num), pSrc);
				DRV_WriteReg32(USB_DMACNT(chan_num), nBytes);
				DRV_WriteReg16(USB_DMACNTL(chan_num), USB_DMACNTL_BURSTMODE_MASK | USB_DMACNTL_DMAEN | USB_DMACNTL_DMADIR | (nEP<<4));				
				while(DRV_Reg16(USB_DMACNTL(chan_num)) & USB_DMACNTL_DMAEN);
			}
#else
			/* 2012.8.14 optimize, write FIFO by word if send mass data in high speed */
			if(nCount == USB_BULK_HS_MAXP)
			{
				nCount = nBytes>>2;
				while (nCount--)
				{
					DRV_WriteReg32(nAddr,*(kal_uint32*)pby);
					pby += 4;
				}
			} else {
				/* write by byte */
				while (nCount) 
				{
					DRV_WriteReg8(nAddr,*pby++);
					nCount--;
				}
			}
#endif
#elif	defined(USB_IP_V1)||defined(USB_IP_V1_PLUS)		
				kal_uint16 nCount = nBytes;
				kal_uint8 *pby;
				kal_uint32 nAddr;
    			  
				DRV_WriteReg8(USB_INDEX,nEP);
				nAddr = USB_EP0+nEP*4;
				pby = (kal_uint8 *)pSrc;

				/* write by byte */
				while (nCount) 
				{
						DRV_WriteReg8(nAddr,*pby++);
						nCount--;
				}
#elif	defined(USB_IP_V2)		
				if(nEP==0)
				{
						// make sure this endpoint status belong to MCU
						if((READ_EP0_BDT_PID(USB_BDT_TX, g_UsbDrvInfo.ep0_tx_even_odd)&VUSB_BDT_OWNS_BIT)!=0)
								ASSERT(0);
						memcpy((void*)READ_EP0_BDT_ADDR(USB_BDT_TX, g_UsbDrvInfo.ep0_tx_even_odd),	pSrc, nBytes);				
						WRITE_EP0_BDT_PID(USB_BDT_TX, g_UsbDrvInfo.ep0_tx_even_odd, nBytes<<VUSB_BDT_BC_SHIFT);
				}
				else
				{		
						// make sure this endpoint status belong to MCU
						if((READ_EPN_BDT_PID(nEP, USB_BDT_TX)&VUSB_BDT_OWNS_BIT)!=0)
								ASSERT(0);
						memcpy((void*)READ_EPN_BDT_ADDR(nEP, USB_BDT_TX), pSrc, nBytes);
						WRITE_EPN_BDT_PID(nEP, USB_BDT_TX, nBytes<<VUSB_BDT_BC_SHIFT);
				}
#endif		
//		USB_Dbg_Trace(DEBUG_USB_EPFIFOWrite, 0, nEP, nBytes);
	
}


/* type == USB_IN_EP_TYPE or USB_OUT_EP_TYPE 
    en == KAL_TRUE means stall this endpoint */
void USB_CtrlEPStall(kal_uint8 EPno, USB_EP_TYPE type, kal_bool en, USB_CTRL_STALL_ENTRY entry)
{
#if	defined(USB_IP_V3) || defined(USB_IP_V4)	
   		kal_uint16  CSR;
		if (en == KAL_TRUE)
		{
			/* stall endpoint */
			if(type == USB_OUT_EP_TYPE)
			{
				DRV_WriteReg8(USB_INDEX_V3,  EPno);
				CSR = DRV_Reg16(USB_RXCSR);
				DRV_WriteReg16(USB_RXCSR, (CSR|USB_RXCSR_FLUSHFIFO|USB_RXCSR_CLRDATATOG|USB_RXCSR_SENDSTALL));
				g_UsbDrvInfo.ep_out_stall_status[EPno-1] = KAL_TRUE;
			}
			else
			{
				DRV_WriteReg8(USB_INDEX_V3, EPno);
				CSR = DRV_Reg16(USB_TXCSR);
				DRV_WriteReg16(USB_TXCSR, (CSR|USB_TXCSR_FLUSHFIFO|USB_TXCSR_CLRDATATOG|USB_TXCSR_SENDSTALL));
				g_UsbDrvInfo.ep_in_stall_status[EPno-1] = KAL_TRUE;
			}
		}
		else
		{
			/* clear stall */
			if (type == USB_OUT_EP_TYPE)
			{
				DRV_WriteReg8(USB_INDEX_V3, EPno);
				CSR = DRV_Reg16(USB_RXCSR);
				CSR &= ~USB_RXCSR_SENDSTALL;			
				DRV_WriteReg16(USB_RXCSR, (CSR|USB_RXCSR_FLUSHFIFO|USB_RXCSR_CLRDATATOG));
				g_UsbDrvInfo.ep_out_stall_status[EPno-1] = KAL_FALSE;
			}
			else
			{
				DRV_WriteReg8(USB_INDEX_V3, EPno);
				CSR = DRV_Reg16(USB_TXCSR);
				CSR &= ~USB_TXCSR_SENDSTALL;
				DRV_WriteReg16(USB_TXCSR,(CSR|USB_TXCSR_FLUSHFIFO|USB_TXCSR_CLRDATATOG));
				g_UsbDrvInfo.ep_in_stall_status[EPno-1] = KAL_FALSE;
			}
		}
#elif	defined(USB_IP_V1)||defined(USB_IP_V1_PLUS)   	
		kal_uint8 CSR1;
		if (en == KAL_TRUE)
		{
				/* stall endpoint */
				if (type == USB_OUT_EP_TYPE)
				{
					DRV_WriteReg8(USB_INDEX, EPno);
					CSR1 = DRV_Reg8(USB_OUTCSR1);
					CSR1 |= USB_OUTCSR1_SENDSTALL;
					DRV_WriteReg8(USB_OUTCSR1,(CSR1|USB_OUTCSR1_FLUSHFIFO|USB_OUTCSR1_CLRDATATOG));
					g_UsbDrvInfo.ep_out_stall_status[EPno-1] = KAL_TRUE;
				}
				else
				{
					DRV_WriteReg8(USB_INDEX, EPno);
					CSR1 = DRV_Reg8(USB_INCSR1);
					CSR1 |= USB_INCSR1_SENDSTALL;
					DRV_WriteReg8(USB_INCSR1,(CSR1|USB_INCSR1_FLUSHFIFO|USB_INCSR1_CLRDATATOG));
					g_UsbDrvInfo.ep_in_stall_status[EPno-1] = KAL_TRUE;
				}
		}
		else
		{
				/* clear stall */
				if (type == USB_OUT_EP_TYPE)
				{
					DRV_WriteReg8(USB_INDEX, EPno);
					CSR1 = DRV_Reg8(USB_OUTCSR1);
					CSR1 &= ~USB_OUTCSR1_SENDSTALL;
					DRV_WriteReg8(USB_OUTCSR1,(CSR1|USB_OUTCSR1_FLUSHFIFO|USB_OUTCSR1_CLRDATATOG));
					g_UsbDrvInfo.ep_out_stall_status[EPno-1] = KAL_FALSE;
				}
				else
				{
					DRV_WriteReg8(USB_INDEX, EPno);
					CSR1 = DRV_Reg8(USB_INCSR1);
					CSR1 &= ~USB_INCSR1_SENDSTALL;
					DRV_WriteReg8(USB_INCSR1,(CSR1|USB_INCSR1_FLUSHFIFO|USB_INCSR1_CLRDATATOG));
					g_UsbDrvInfo.ep_in_stall_status[EPno-1] = KAL_FALSE;
				}
		}
#elif	defined(USB_IP_V2)	
			if (en == KAL_TRUE)
			{
					/* stall endpoint */
					if (type == USB_OUT_EP_TYPE)
					{
						WRITE_EPN_BDT_PID(EPno, USB_BDT_RX, VUSB_BDT_OWNS_BIT|VUSB_BDT_STALL_BIT);
   						 g_UsbDrvInfo.ep_out_stall_status[EPno-1] = KAL_TRUE;
   						 g_UsbDrvInfo.ep_out_data01[EPno-1] = 0;
    					 }
					else
					{
						WRITE_EPN_BDT_PID(EPno, USB_BDT_TX, VUSB_BDT_OWNS_BIT|VUSB_BDT_STALL_BIT);
						g_UsbDrvInfo.ep_in_stall_status[EPno-1] = KAL_TRUE;
						g_UsbDrvInfo.ep_in_data01[EPno-1] = 0;
					}
			}
			else
			{
					/* clear stall */
					if (type == USB_OUT_EP_TYPE)
					{
						WRITE_EPN_BDT_PID(EPno, USB_BDT_RX, 0);
						g_UsbDrvInfo.ep_out_stall_status[EPno-1] = KAL_FALSE;
						g_UsbDrvInfo.ep_out_data01[EPno-1] = 0;
					}
					else
					{
						WRITE_EPN_BDT_PID(EPno, USB_BDT_TX, 0);
						g_UsbDrvInfo.ep_in_stall_status[EPno-1] = KAL_FALSE;
						g_UsbDrvInfo.ep_in_data01[EPno-1] = 0;
					}
			}
#endif        
	
}

/* get endpoint 0 status. (if transaction end or sent stall interrupt received)  */
void USB_Get_EP0_Status(kal_bool *p_transaction_end, kal_bool *p_sent_stall)
{
#if	defined(USB_IP_V3) || defined(USB_IP_V4)		
			kal_uint16  	CSR0;
			DRV_WriteReg8(USB_INDEX_V3, 0);
			CSR0 = DRV_Reg16(USB_CSR0_V3);

			if (CSR0 & USB_CSR0_SENTSTALL) 
					*p_sent_stall = KAL_TRUE;
			else
					*p_sent_stall = KAL_FALSE;
	
			if (CSR0 & USB_CSR0_SETUPEND) 
					*p_transaction_end = KAL_TRUE;
			else
					*p_transaction_end = KAL_FALSE;
#elif	defined(USB_IP_V2)	
			if((READ_EP0_BDT_PID(USB_BDT_RX, g_UsbDrvInfo.ep0_rx_even_odd^1) &VUSB_BDT_STALL_BIT) 
										&& (DRV_Reg8(USB_CTL)&VUSB_CTL_TXD_SUSPEND))
				*p_sent_stall = KAL_TRUE;
			else
				*p_sent_stall = KAL_FALSE;

			if(g_UsbDrvInfo.ep0_transaction_type == USB_EP0_TRANS_END)
				*p_transaction_end = KAL_TRUE;
			else
				*p_transaction_end = KAL_FALSE;
#elif	defined(USB_IP_V1)||defined(USB_IP_V1_PLUS)					
			kal_uint16  	byCSR0;
			DRV_WriteReg8(USB_INDEX,0);
			byCSR0 = DRV_Reg8(USB_CSR0);

			if (byCSR0 & USB_CSR0_SENTSTALL) 
					*p_sent_stall = KAL_TRUE;
			else
					*p_sent_stall = KAL_FALSE;
	
			if (byCSR0 & USB_CSR0_SETUPEND) 
					*p_transaction_end = KAL_TRUE;
			else
					*p_transaction_end = KAL_FALSE;
#endif	
}


/* control endpoint 0 registers according to USB_EP0_DRV_STATE */
void USB_Update_EP0_State(USB_EP0_DRV_STATE state, kal_bool stall, kal_bool end)
{
#if	defined(USB_IP_V3) || defined(USB_IP_V4)	
		kal_uint8   reg_state;
		kal_uint16   CSR0;

		/* clear sent stall */
		if(state == USB_EP0_DRV_STATE_CLEAR_SENT_STALL)
		{
			DRV_WriteReg8(USB_INDEX_V3, 0);   
			CSR0 = DRV_Reg16(USB_CSR0_V3);
			DRV_WriteReg16(USB_CSR0_V3, CSR0&(~USB_CSR0_SENTSTALL));
			return;
		}

		/* clear transaction end*/
		if(state == USB_EP0_DRV_STATE_TRANSACTION_END)
		{
			DRV_WriteReg8(USB_INDEX_V3, 0);   
			DRV_WriteReg16(USB_CSR0_V3, (USB_CSR0_SERVICEDSETUPEND));
			return;
		}

		/* ep0 read end or write ready*/
		if(state == USB_EP0_DRV_STATE_READ_END)
		{
			reg_state = USB_CSR0_SERVICEDRXPKTRDY;
		}
		else
		{
			reg_state = USB_CSR0_TXPKTRDY;
		}

		/* error occured, sent stall*/
		if(stall == KAL_TRUE)
		{
			reg_state |= USB_CSR0_SENDSTALL; 
		}

		/* last data for this transaction, set data end bit*/
		if(end == KAL_TRUE)
		{
			reg_state |= USB_CSR0_DATAEND; 
		}
	
		DRV_WriteReg8(USB_INDEX_V3, 0);
		DRV_WriteReg16(USB_CSR0_V3, reg_state);
#elif	defined(USB_IP_V2)	
		if(state ==USB_EP0_DRV_STATE_CLEAR_SENT_STALL)
		{
					WRITE_EP0_BDT_PID(USB_BDT_RX, USB_BDT_EVEN, 
							READ_EP0_BDT_PID(USB_BDT_RX, USB_BDT_EVEN)&(~VUSB_BDT_STALL_BIT));
					WRITE_EP0_BDT_PID(USB_BDT_RX, USB_BDT_ODD, 
							READ_EP0_BDT_PID(USB_BDT_RX, USB_BDT_ODD)&(~VUSB_BDT_STALL_BIT));
					WRITE_EP0_BDT_PID(USB_BDT_TX, USB_BDT_EVEN, 
							READ_EP0_BDT_PID(USB_BDT_TX, USB_BDT_EVEN)&(~VUSB_BDT_STALL_BIT));
					WRITE_EP0_BDT_PID(USB_BDT_TX, USB_BDT_ODD, 
							READ_EP0_BDT_PID(USB_BDT_TX, USB_BDT_ODD)&(~VUSB_BDT_STALL_BIT));
					return;
		}

			/* clear transaction end*/
		if(state ==USB_EP0_DRV_STATE_TRANSACTION_END)
		{
				/* clear the data in sent buffer */
				WRITE_EP0_BDT_PID(USB_BDT_TX, USB_BDT_EVEN, 0);
				WRITE_EP0_BDT_PID(USB_BDT_TX, USB_BDT_ODD, 0);
				WRITE_EP0_BDT_PID(USB_BDT_RX, g_UsbDrvInfo.ep0_rx_even_odd, 
							(USB_EP0_MAXP << VUSB_BDT_BC_SHIFT) |VUSB_BDT_OWNS_BIT);
				g_UsbDrvInfo.ep0_transaction_type = USB_EP0_TRANS_NONE;
				return;
		}		

		/* ep0 read end or write ready*/
		if((state == USB_EP0_DRV_STATE_READ_END) && (stall==KAL_FALSE))
		{
				/* clear tx suspend for setupt packet*/
						if(DRV_Reg8(USB_CTL)&VUSB_CTL_TXD_SUSPEND)
						{
								g_UsbDrvInfo.ep0_tx_data01 = 0;
								/* clear the data in sent buffer */
								WRITE_EP0_BDT_PID(USB_BDT_TX, USB_BDT_EVEN, 0);
								WRITE_EP0_BDT_PID(USB_BDT_TX, USB_BDT_ODD, 0);
								DRV_Reg8(USB_CTL) = DRV_Reg8(USB_CTL)&(~VUSB_CTL_TXD_SUSPEND);
						}	

						WRITE_EP0_BDT_PID(USB_BDT_RX, g_UsbDrvInfo.ep0_rx_even_odd, 
											(USB_EP0_MAXP << VUSB_BDT_BC_SHIFT) |VUSB_BDT_OWNS_BIT);
						g_UsbDrvInfo.ep0_transaction_type = USB_EP0_TRANS_RX_DATA;
		}
		else if((state == USB_EP0_DRV_STATE_WRITE_RDY) && (stall==KAL_FALSE))
		{
					/* toggle DATA01*/
					g_UsbDrvInfo.ep0_tx_data01^=1;
					WRITE_EP0_BDT_PID(USB_BDT_TX, g_UsbDrvInfo.ep0_tx_even_odd, 
									(READ_EP0_BDT_PID(USB_BDT_TX, g_UsbDrvInfo.ep0_tx_even_odd)&VUSB_BDT_BC_MASK) 
									|(g_UsbDrvInfo.ep0_tx_data01<<VUSB_BDT_DATA01_SHIFT) 
									| VUSB_BDT_DTS_BIT | VUSB_BDT_OWNS_BIT);
					g_UsbDrvInfo.ep0_transaction_type = USB_EP0_TRANS_TX_DATA;
		}
	
			/* error occured, sent stall*/	
		if(stall == KAL_TRUE)
		{
				WRITE_EP0_BDT_PID(USB_BDT_RX, USB_BDT_EVEN, 
							READ_EP0_BDT_PID(USB_BDT_RX, USB_BDT_EVEN)|VUSB_BDT_STALL_BIT);
				WRITE_EP0_BDT_PID(USB_BDT_RX, USB_BDT_ODD, 
							READ_EP0_BDT_PID(USB_BDT_RX, USB_BDT_ODD)|VUSB_BDT_STALL_BIT);
				WRITE_EP0_BDT_PID(USB_BDT_TX, USB_BDT_EVEN, 
							READ_EP0_BDT_PID(USB_BDT_RX, USB_BDT_EVEN)|VUSB_BDT_STALL_BIT);
				WRITE_EP0_BDT_PID(USB_BDT_TX, USB_BDT_ODD, 
							READ_EP0_BDT_PID(USB_BDT_RX, USB_BDT_ODD)|VUSB_BDT_STALL_BIT);
				g_UsbDrvInfo.ep0_transaction_type = USB_EP0_TRANS_NONE;
		}
		/* last data for this transaction, set data end bit*/
		else if(end == KAL_TRUE)
		{
					if(g_UsbDrvInfo.ep0_transaction_type==USB_EP0_TRANS_RX_DATA)
					{
							/* status data01 always 1*/
							g_UsbDrvInfo.ep0_tx_data01 = 1;
							/* byte count 0 */
							WRITE_EP0_BDT_PID(USB_BDT_TX, g_UsbDrvInfo.ep0_tx_even_odd, 
										(g_UsbDrvInfo.ep0_tx_data01<<VUSB_BDT_DATA01_SHIFT) |
										VUSB_BDT_DTS_BIT | VUSB_BDT_OWNS_BIT);
							g_UsbDrvInfo.ep0_transaction_type = USB_EP0_TRANS_RX_STATUS;
					}
					else if(g_UsbDrvInfo.ep0_transaction_type==USB_EP0_TRANS_TX_DATA)
					{					
							g_UsbDrvInfo.ep0_transaction_type = USB_EP0_TRANS_TX_DATA_END;
					}
					else
					{
					}
			}
#elif	defined(USB_IP_V1)||defined(USB_IP_V1_PLUS)	
			kal_uint8   reg_state;
			kal_uint8   byCSR0;

			/* clear sent stall*/
			if(state ==USB_EP0_DRV_STATE_CLEAR_SENT_STALL)
			{
					DRV_WriteReg8(USB_INDEX,0);   
					byCSR0 = DRV_Reg8(USB_CSR0);
					DRV_WriteReg8(USB_CSR0, byCSR0&(~USB_CSR0_SENTSTALL));
					return;
			}

			/* clear transaction end*/
			if(state ==USB_EP0_DRV_STATE_TRANSACTION_END)
			{
					DRV_WriteReg8(USB_INDEX,0);   
					DRV_WriteReg8(USB_CSR0, (USB_CSR0_SERVICESETUPEND));
					return;
			}

			/* ep0 read end or write ready*/
			if(state == USB_EP0_DRV_STATE_READ_END)
			{
					reg_state = USB_CSR0_SERVICEDOUTPKTRDY;
			}
			else
			{
					reg_state = USB_CSR0_INPKTRDY;
			}

			/* error occured, sent stall*/	
			if(stall == KAL_TRUE)
			{
					reg_state |= USB_CSR0_SENDSTALL; 
			}
			/* last data for this transaction, set data end bit*/
			if(end == KAL_TRUE)
			{
					reg_state |= USB_CSR0_DATAEND; 
			}
	
			DRV_WriteReg8(USB_INDEX,0);   
			DRV_WriteReg8(USB_CSR0, reg_state);
#endif		
}

/* return packet length for endpoint 0*/
kal_uint32 USB_EP0_Pkt_Len(void)
{
	kal_uint32 nCount = 0;
#if	defined(USB_IP_V3) || defined(USB_IP_V4)	
		kal_uint16  CSR0;
	
		DRV_WriteReg8(USB_INDEX_V3, 0);
		CSR0 = DRV_Reg16(USB_CSR0_V3);
	
		if(CSR0&USB_CSR0_RXPKTRDY)
			nCount = (kal_uint32)DRV_Reg8(USB_COUNT0_V3);
#elif	defined(USB_IP_V2)	
			if((READ_EP0_BDT_PID(USB_BDT_RX, g_UsbDrvInfo.ep0_rx_even_odd) &VUSB_BDT_OWNS_BIT)==0)
			{
					nCount = (READ_EP0_BDT_PID(USB_BDT_RX, g_UsbDrvInfo.ep0_rx_even_odd) & VUSB_BDT_BC_MASK)>>VUSB_BDT_BC_SHIFT;
			}
#elif	defined(USB_IP_V1)||defined(USB_IP_V1_PLUS)			
			kal_uint8 byCSR0;
	
			DRV_WriteReg8(USB_INDEX,0);
			byCSR0 = DRV_Reg8(USB_CSR0);
	
			if(byCSR0 & USB_CSR0_OUTPKTRDY)
			{
					nCount = (kal_uint32)DRV_Reg8(USB_COUNT0);
			}
#endif	
	return nCount;
}

/* ep in data prepared ready, set ready bit */
void USB_EP_Bulk_In_Ready(kal_uint8 ep_num)
{
#if	defined(USB_IP_V3) || defined(USB_IP_V4)		
    if (ep_num == 0)
    	ASSERT(0);

		DRV_WriteReg8(USB_INDEX_V3, ep_num);
		DRV_WriteReg16(USB_TXCSR, DRV_Reg16(USB_TXCSR)|USB_TXCSR_TXPKTRDY);	
#elif	defined(USB_IP_V2)  	
		WRITE_EPN_BDT_PID(ep_num, USB_BDT_TX, 
							(READ_EPN_BDT_PID(ep_num, USB_BDT_TX) & VUSB_BDT_BC_MASK) |
							(g_UsbDrvInfo.ep_in_data01[ep_num-1]<<VUSB_BDT_DATA01_SHIFT) |
							VUSB_BDT_DTS_BIT | VUSB_BDT_OWNS_BIT);
		g_UsbDrvInfo.ep_in_data01[ep_num-1]^=1;
#elif	defined(USB_IP_V1)||defined(USB_IP_V1_PLUS)  	
		DRV_WriteReg8(USB_INDEX,ep_num);
		DRV_WriteReg8(USB_INCSR1, USB_INCSR1_INPKTRDY);
#endif 	 		
}

void USB_EP_Poll_Bulk_In_Ready(kal_uint8 ep_num)
{
#if	defined(USB_IP_V3) || defined(USB_IP_V4)		
			DRV_WriteReg8(USB_INDEX_V3,ep_num);
			while(DRV_Reg8(USB_TXCSR) & USB_TXCSR_TXPKTRDY);
#elif	defined(USB_IP_V2)    
  			if (ep_num == 0)
  			{
  	  				while((READ_EP0_BDT_PID(USB_BDT_TX, g_UsbDrvInfo.ep0_tx_even_odd)& VUSB_BDT_OWNS_BIT)!=0)
	  					{
	  	  				   USB_HISR();
	      			}
  			}
   			else    
  			{
	  				while((READ_EPN_BDT_PID(ep_num, USB_BDT_TX)& VUSB_BDT_OWNS_BIT)!=0)
	  				{
//		  				  count_time2 ++;
	  	 				   USB_HISR();
	     			 }
//	  				Arr2[0] = count_time2;
//					count_time2 = 0;
    			}    
#elif	defined(USB_IP_V1)||defined(USB_IP_V1_PLUS)    
			DRV_WriteReg8(USB_INDEX,ep_num);
			while(DRV_Reg8(USB_INCSR1) & USB_INCSR1_INPKTRDY);
#endif    
}

kal_bool USB_EP_Poll_Bulk_Out_Intr(kal_uint8 ep_num)
{
#if     defined(USB_IP_V4)
        kal_uint16  IntrRx;
	  	IntrRx = DRV_Reg16(USB_INTRRX);
	  	if(IntrRx&(0x01<<ep_num))
	  	{	
			DRV_WriteReg16(USB_INTRRX, IntrRx);
			return KAL_TRUE;
		}	
		else
			return KAL_FALSE;

#elif	defined(USB_IP_V3)	
	  	kal_uint16  IntrRx;
	  	IntrRx = DRV_Reg16(USB_INTRRX);
	  	if(IntrRx&(0x01<<ep_num))
	  	{	
				DRV_WriteReg16(USB_INTRRX, ~IntrRx);
				return KAL_TRUE;
			}	
			else
				return KAL_FALSE;
//#elif	defined(USB_IP_V1)
#elif	defined(USB_IP_V1)||defined(USB_IP_V1_PLUS)    
	  	kal_uint16  IntrRx;
	  	IntrRx = DRV_Reg8(USB_INTROUT1); //Read Clear
	  	if(IntrRx&(0x01<<ep_num))
	  	{	
			return KAL_TRUE;
		}	
		else
		{
			return KAL_FALSE;	  	
		}
#endif  
}

kal_bool USB_EP_Poll_Bulk_Out_Ready(kal_uint8 ep_num)
{
#if	defined(USB_IP_V3) || defined(USB_IP_V4)	
	  	DRV_WriteReg8(USB_INDEX_V3,ep_num);
	  	return (DRV_Reg8(USB_RXCSR) & USB_RXCSR_RXPKTRDY);  //RX Receive data or not  1 : receive data
#elif	defined(USB_IP_V1)||defined(USB_IP_V1_PLUS)    
	  	DRV_WriteReg8(USB_INDEX,ep_num);
	  	return (DRV_Reg8(USB_OUTCSR1) & USB_OUTCSR1_OUTPKTRDY);
#endif    
}

#if	defined(USB_IP_V2) 
kal_bool USB_Poll_RX_Data(kal_uint8 ep_num,kal_uint32* addr, kal_uint32 length)
{
			volatile kal_uint8	IntrStatus;
			volatile kal_uint8	FMStatus;
			volatile kal_uint8	ErrorStatus;
			volatile kal_uint8	PhyStatus;
			volatile kal_uint8	direction;
			volatile kal_bool	fast_mode_enable;	 
			kal_uint32     loop = 0; 

			if (length > USB_BULK_MAXP)
				ASSERT(0);
			
			while(loop++ < 0xFFFFFFFF)//timeout threshold
			{
				fast_mode_enable = (DRV_Reg8(USB_FM_CTL)&VUSB_FM_CTL_FMENB) ? KAL_TRUE : KAL_FALSE;
				IntrStatus = DRV_Reg8(USB_INT_STAT);
				FMStatus = DRV_Reg8(USB_FM_ERR_STAT);
				ErrorStatus = DRV_Reg8(USB_ERR_STAT);
				PhyStatus = DRV_Reg8(USB_PHY_EXTRA);

			/* Ignore VUSB_INT_STAT_TOKEN_DONE in fast mode */
			if(((IntrStatus&VUSB_INT_STAT_TOKEN_DONE)&&(fast_mode_enable==KAL_FALSE)) || (FMStatus&VUSB_FM_ERR_STAT_TOKEN_DONE))
			{
					direction = (DRV_Reg8(USB_STAT) >> 3) & 0x01;

					DRV_WriteReg8(USB_INT_STAT, (IntrStatus&VUSB_INT_STAT_TOKEN_DONE));

					if(direction == USB_BDT_RX)    /* Receive*/
					{
							USB_EPFIFORead(ep_num, length, addr);
							USB_EP_Bulk_Out_Ready(ep_num);
							DRV_WriteReg8(USB_INT_STAT, (IntrStatus&(~VUSB_INT_STAT_TOKEN_DONE)));
							DRV_WriteReg8(USB_ERR_STAT, ErrorStatus);
							return KAL_TRUE;
					}

			} /* End VUSB_INT_STAT_TOKEN_DONE*/			
				/* Clear interrupt other than token done*/
				DRV_WriteReg8(USB_INT_STAT, (IntrStatus&(~VUSB_INT_STAT_TOKEN_DONE)));
				DRV_WriteReg8(USB_ERR_STAT, ErrorStatus);
	}	

	return KAL_FALSE;	//loop >= 0xFFFFFFFF
}
#endif

/* return ep out received packet length*/
kal_uint32 USB_EP_Out_Pkt_Len(kal_uint8 ep_num)
{
	kal_uint32 nCount = 0;
#if	defined(USB_IP_V3) || defined(USB_IP_V4)	
			kal_uint8 CSR;
			DRV_WriteReg8(USB_INDEX_V3, ep_num);
			CSR = DRV_Reg16(USB_RXCSR);
	
			if(CSR&USB_RXCSR_RXPKTRDY)
				nCount = (kal_uint32)DRV_Reg16(USB_RXCOUNT);
#elif	defined(USB_IP_V1)||defined(USB_IP_V1_PLUS)	
			kal_uint8 byCSR;
	
			DRV_WriteReg8(USB_INDEX,ep_num);
			byCSR = DRV_Reg8(USB_OUTCSR1);
	
			if(byCSR & USB_OUTCSR1_OUTPKTRDY)
				nCount = (kal_uint32)DRV_Reg8(USB_OUTCOUNT1);

#elif	defined(USB_IP_V2)	
			if((READ_EPN_BDT_PID(ep_num, USB_BDT_RX)&VUSB_BDT_OWNS_BIT)==0)
			{
					nCount = (READ_EPN_BDT_PID(ep_num, USB_BDT_RX)&VUSB_BDT_BC_MASK) >> VUSB_BDT_BC_SHIFT;
					Arr[in] = nCount;
					in ++;
					if (in ==1000)
						in =0;
			}

#endif   
	return nCount;
}	

/* ep out data already read out, clear the data*/
void USB_EP_Bulk_Out_Ready(kal_uint8 ep_num)
{
#if	defined(USB_IP_V3) || defined(USB_IP_V4)		
    if (ep_num == 0)
    	ASSERT(0);
    	
			DRV_WriteReg8(USB_INDEX_V3, ep_num);
			DRV_WriteReg16(USB_RXCSR, DRV_Reg16(USB_RXCSR)&(~USB_RXCSR_RXPKTRDY));
#elif	defined(USB_IP_V2)	
				WRITE_EPN_BDT_PID(ep_num, USB_BDT_RX, 
								(g_UsbDrvInfo.ep_out_max_data_size[ep_num-1] << VUSB_BDT_BC_SHIFT) |
								(g_UsbDrvInfo.ep_out_data01[ep_num-1]<<VUSB_BDT_DATA01_SHIFT) |
								VUSB_BDT_DTS_BIT | VUSB_BDT_OWNS_BIT);
				g_UsbDrvInfo.ep_out_data01[ep_num-1]^=1;
#elif	defined(USB_IP_V1)||defined(USB_IP_V1_PLUS)	
				DRV_WriteReg8(USB_INDEX,ep_num);
				DRV_WriteReg8(USB_OUTCSR1, 0);
#endif
}

/* Get status. See if ep in fifo is empty. 
   If false, it means some data in fifo still wait to send out */
kal_bool USB_Is_EP_Bulk_In_Empty(kal_uint8 ep_num)
{
	kal_bool bRet;
#if	defined(USB_IP_V3) || defined(USB_IP_V4)	
			kal_uint8 CSR;
			DRV_WriteReg8(USB_INDEX_V3, ep_num);	
			CSR = DRV_Reg16(USB_TXCSR);
			if(CSR&USB_TXCSR_FIFONOTEMPTY)
				bRet = KAL_FALSE;
			else
				bRet = KAL_TRUE;	
#elif	defined(USB_IP_V1)||defined(USB_IP_V1_PLUS)	
			kal_uint8 byCSR;

			DRV_WriteReg8(USB_INDEX, ep_num);	
			byCSR = DRV_Reg8(USB_INCSR1);
			if(byCSR&USB_INCSR1_FIFONOTEMPTY)
				bRet = KAL_FALSE;
			else
				bRet = KAL_TRUE;
#elif	defined(USB_IP_V2)	
			if((READ_EPN_BDT_PID(ep_num, USB_BDT_TX)&VUSB_BDT_OWNS_BIT)!=0)
				bRet = KAL_FALSE;
			else
				bRet = KAL_TRUE;	
#endif				
	return bRet;
}

/* clear the fifo for ep IN*/
void USB_Clear_IN_EP_FIFO(kal_uint8 ep_num)
{
#if	defined(USB_IP_V3) || defined(USB_IP_V4)		
		DRV_WriteReg8(USB_INDEX_V3, ep_num);
		DRV_WriteReg16(USB_TXCSR, DRV_Reg16(USB_TXCSR)|USB_TXCSR_FLUSHFIFO);
		DRV_WriteReg16(USB_TXCSR, DRV_Reg16(USB_TXCSR)|USB_TXCSR_FLUSHFIFO);
#elif	defined(USB_IP_V1)||defined(USB_IP_V1_PLUS)	 
		DRV_WriteReg8(USB_INDEX, ep_num);
		DRV_WriteReg8(USB_INCSR1, USB_INCSR1_FLUSHFIFO);
		DRV_WriteReg8(USB_INCSR1, USB_INCSR1_FLUSHFIFO);
 #endif 	
}

/* clear the fifo for ep OUT*/
void USB_Clear_OUT_EP_FIFO(kal_uint8 ep_num)
{
#if	defined(USB_IP_V3) || defined(USB_IP_V4)		
		DRV_WriteReg8(USB_INDEX_V3, ep_num);
		DRV_WriteReg16(USB_RXCSR, DRV_Reg16(USB_RXCSR)|USB_RXCSR_FLUSHFIFO);
		DRV_WriteReg16(USB_RXCSR, DRV_Reg16(USB_RXCSR)|USB_RXCSR_FLUSHFIFO);
#elif	defined(USB_IP_V1)||defined(USB_IP_V1_PLUS)
		DRV_WriteReg8(USB_INDEX, ep_num);
		DRV_WriteReg8(USB_OUTCSR1, USB_OUTCSR1_FLUSHFIFO);
		DRV_WriteReg8(USB_OUTCSR1, USB_OUTCSR1_FLUSHFIFO);
#endif 	 
}
/* 
    Add for retrive exception log 
    Polling if there is any data received or other interrupts
*/
void USB_Polling_Recv_Data(kal_uint8 ep_num)
{
			/* This function can only be called after exception*/		
			g_UsbDrvInfo.is_unMaskUSB = KAL_FALSE;
			USB_HISR();
}

/************************************************************
	HISR/LISR   interrupt handler
*************************************************************/
// for debug, must remove it afterword.
typedef struct
{
	kal_uint8 usb;
	kal_uint8 in;
	kal_uint8 out;
}usb_hisr_int;
usb_hisr_int intarray[256];
kal_uint8 int_index;

kal_bool USB_IS_High_Speed(void)
{
#if	defined(USB_IP_V3) || defined(USB_IP_V4)		
	kal_uint8	  usbpower = DRV_Reg8(USB_POWER_V3);

	if((usbpower&USB_POWER_HSMODE) != 0)
	{
		return KAL_TRUE;
	}
	else
	{
		return KAL_FALSE;
	}
#elif	defined(USB_IP_V2)
		return KAL_FALSE;
#elif	defined(USB_IP_V1)||defined(USB_IP_V1_PLUS)
		return KAL_FALSE;
#endif
}
void USB_DMA_Setup(kal_uint8 ep_num, USB_EP_TYPE  direction, kal_uint32 addr, kal_uint32 length)
{	
#if	defined(USB_IP_V3) || defined(USB_IP_V4)		
		kal_uint16  	dma_ctrl;
		kal_uint8    	dma_chan = 1;
		kal_uint32 	dma_burst_mode = 0x03;

		if (direction == USB_IN_EP_TYPE)
		{
			DRV_WriteReg8(USB_INDEX_V3, ep_num);
			DRV_WriteReg16(USB_TXCSR, (USB_TXCSR_AUTOSET|USB_TXCSR_DMAREQENAB|USB_TXCSR_DMAREQMODE));

			DRV_WriteReg32(USB_DMAADDR(dma_chan), addr);
			DRV_WriteReg32(USB_DMACNT(dma_chan), length);

			dma_ctrl = USB_DMACNTL_DMADIR|USB_DMACNTL_DMAMODE|USB_DMACNTL_INTEN|(ep_num<<4);
			dma_ctrl |= ((dma_burst_mode&0x03)<<9)|USB_DMACNTL_DMAEN;

			DRV_WriteReg16(USB_DMACNTL(dma_chan), dma_ctrl);

		}
		else
		{
			DRV_WriteReg8(USB_INDEX_V3, ep_num);
			DRV_WriteReg16(USB_RXCSR, (USB_RXCSR_AUTOCLEAR|USB_RXCSR_DMAREQENAB|USB_RXCSR_RXPKTRDY|USB_RXCSR_DMAREQMODE));

			DRV_WriteReg32(USB_DMAADDR(dma_chan), addr);
			DRV_WriteReg32(USB_DMACNT(dma_chan), length);

			dma_ctrl = USB_DMACNTL_DMAMODE|USB_DMACNTL_INTEN|(ep_num<<4);
			dma_ctrl |= ((dma_burst_mode&0x03)<<9)|USB_DMACNTL_DMAEN;
			DRV_WriteReg16(USB_DMACNTL(dma_chan), dma_ctrl);
		}
#endif	
}

void USBDMA_HISR(void)
{
#if	defined(USB_IP_V3) || defined(USB_IP_V4)		
		kal_uint8  	DMAIntr;
		kal_uint32  	index;
	
		DMAIntr = DRV_Reg8(USB_DMAINTR);

		while(DMAIntr == 0)
		{

			DMAIntr = DRV_Reg8(USB_DMAINTR);
		}

		for (index = 0; index < 4; index++)
		{
			if((DMAIntr&(1<<index)) != 0)
			{
				// write 0 clear first, then do call back function, order is very important !!!
				DRV_WriteReg8(USB_DMAINTR, ~(0x01<<index));
			}
		}
#endif	 
}

void USB_HISR(void)
{
#if defined (USB_IP_V4)
	kal_uint16  IntrTx;
	//kal_uint16  TXCSR;		
	kal_uint16  RXCSR;
	kal_uint16  IntrUSB;
	kal_uint16  IntrRx;
	kal_uint32  ep_num;
	kal_uint8 i/*, result*/;
	kal_uint32 wQmuVal, wRetVal;
	kal_bool qmu_poll_done = KAL_FALSE;

	// Write 1 clear
	IntrUSB = DRV_Reg8(USB_INTRUSB_V3);
	DRV_WriteReg8(USB_INTRUSB_V3, IntrUSB);
	// Write 1 clear
	IntrTx = DRV_Reg16(USB_INTRTX);
	DRV_WriteReg16(USB_INTRTX, IntrTx);
	// Write 1 clear		
	IntrRx = DRV_Reg16(USB_INTRRX);
	DRV_WriteReg16(USB_INTRRX, IntrRx);
#if defined(USB_QMU)
		wQmuVal = DRV_Reg32(USB_QISAR);

		if(wQmuVal)
		{
			DRV_WriteReg32(USB_QISAR, wQmuVal);
			wQmuVal &= ~(DRV_Reg32(USB_QIMR));

			//MSG(USB, "USB_HISR wIntrQMUDoneValue :%x, wIntrQMUValue : %x\n",wQmuVal, wRetVal);
			//RXQ GPD DONE
			for (i=1; i<=RXQ_NUM; i++)
			{
				if (wQmuVal & DQMU_M_RX_DONE(i))
				{
					qmu_poll_done = KAL_TRUE;		
				}
			}

			//TXQ GPD DONE
			for (i=1; i<=TXQ_NUM; i++)
			{
				if (wQmuVal & DQMU_M_TX_DONE(i))
				{
					qmu_poll_done = KAL_TRUE;
				}
			}

			//RXQ ERROR
			if (wQmuVal & DQMU_M_RXQ_ERR)
			{
				wRetVal = MGC_ReadQIRQ32(MGC_O_QIRQ_RQEIR) & (~(MGC_ReadQIRQ32(MGC_O_QIRQ_RQEIMR)));
//				MSG(USB, "Rx Queue error in QMU mode![0x%x]\n", wRetVal);

				for (i=1; i<=RXQ_NUM; i++)
				{
					if (wRetVal & DQMU_M_RX_GPDCS_ERR(i))
					{
						MSG(USB, "Rx %d GPD checksum error!\n", i);
					}
					if (wRetVal & DQMU_M_RX_LEN_ERR(i))
					{
						MSG(USB, "Rx %d recieve length error!\n", i);
					}
					if (wRetVal & DQMU_M_RX_ZLP_ERR(i))
					{
						gUSBAcm_IsZLPErr = KAL_TRUE;
//						MSG(USB, "Rx %d recieve an zlp packet!\n", i);
					}
				}
				 
				MGC_WriteQIRQ32(MGC_O_QIRQ_RQEIR, wRetVal);
			}

			//TXQ ERROR
			if (wQmuVal & DQMU_M_TXQ_ERR)
			{
				wRetVal = MGC_ReadQIRQ32(MGC_O_QIRQ_TQEIR) & (~(MGC_ReadQIRQ32(MGC_O_QIRQ_TQEIMR)));
//				MSG(USB, "Tx Queue error in QMU mode![0x%x]\n", wRetVal);

				for (i=1; i<=MAX_TX_EP_NUM; i++)
				{
					if (wRetVal & DQMU_M_TX_BDCS_ERR(i))
					{
						MSG(USB, "Tx %d BD checksum error!\n", i);
					}
					if (wRetVal & DQMU_M_TX_GPDCS_ERR(i))
					{
						MSG(USB, "Tx %d GPD checksum error!\n", i);
					}
					if (wRetVal & DQMU_M_TX_LEN_ERR(i))
					{
						MSG(USB, "Tx %d buffer length error!\n", i);
					}
				}

				MGC_WriteQIRQ32(MGC_O_QIRQ_TQEIR, wRetVal);
			}

			//RX EP ERROR
			if (wQmuVal & DQMU_M_RXEP_ERR)
			{
				wRetVal = MGC_ReadQIRQ32(MGC_O_QIRQ_REPEIR) & (~(MGC_ReadQIRQ32(MGC_O_QIRQ_REPEIMR)));
				MSG(USB, "Rx endpoint error in QMU mode![0x%x]\n", wRetVal);

				for (i=1; i<=RXQ_NUM; i++)
				{
					if (wRetVal & DQMU_M_RX_EP_ERR(i))
					{
						MSG(USB, "Rx %d Ep Error!\n", i);
					}
				}

				MGC_WriteQIRQ32(MGC_O_QIRQ_REPEIR, wRetVal);
			}

			//TX EP ERROR
			if(wQmuVal & DQMU_M_TXEP_ERR)
			{
				wRetVal = MGC_ReadQIRQ32(MGC_O_QIRQ_TEPEIR)& (~(MGC_ReadQIRQ32(MGC_O_QIRQ_TEPEIMR)));
				MSG(USB, "Tx endpoint error in QMU mode![0x%x]\n", wRetVal);

				for (i=1; i<=TXQ_NUM; i++){
					if (wRetVal &DQMU_M_TX_EP_ERR(i))
					{
						MSG(USB, "Tx %d Ep Error!\n", i);
						
					}
				}

				MGC_WriteQIRQ32(MGC_O_QIRQ_TEPEIR, wRetVal);
			}

			//RXQ EMPTY
			if (wQmuVal & DQMU_M_RQ_EMPTY)
			{
				wRetVal = MGC_ReadQIRQ32(MGC_O_QIRQ_REPEMPR)
					& (~(MGC_ReadQIRQ32(MGC_O_QIRQ_REPEMPMR)));
				MSG(USB, "Rx Empty in QMU mode![0x%x]\n", wRetVal);

				for (i=1; i<=RXQ_NUM; i++)
				{
					if (wRetVal & DQMU_M_RX_EMPTY(i))
					{
						MSG(USB, "Rx %d Empty!\n", i);
					}
				}

				MGC_WriteQIRQ32(MGC_O_QIRQ_REPEMPR, wRetVal);
			}

			//TXQ EMPTY
			if (wQmuVal & DQMU_M_TQ_EMPTY)
			{
				wRetVal = MGC_ReadQIRQ32(MGC_O_QIRQ_TEPEMPR)
					& (~(MGC_ReadQIRQ32(MGC_O_QIRQ_TEPEMPMR)));
				MSG(USB, "Tx Empty in QMU mode![0x%x]\n", wRetVal);

				for (i=1; i<=TXQ_NUM; i++)
				{
					if (wRetVal & DQMU_M_TX_EMPTY(i))
					{
						MSG(USB, "Tx %d Empty!\n", i);
					}
				}

				MGC_WriteQIRQ32(MGC_O_QIRQ_TEPEMPR, wRetVal);
			}

			if(wQmuVal)
			{	
				//MSG(USB, "wIntrQMUDoneValue :%x\n",wQmuVal);
				//qmu_poll_done = KAL_TRUE;
			}

			if(qmu_poll_done == KAL_TRUE)
			{
				//MSG(USB, "break, wIntrQMUDoneValue :%x, wIntrQMUValue : %x\n",wIntrQMUDoneValue, wIntrQMUValue);
				gUSBAcm_IsQMUDone = KAL_TRUE;
			}
		}
#endif		
		
	if(IntrUSB & USB_INTRUSB_RESET){	
		MSG(USB, "USB_INTRUSB_RESET \n");
		if(g_UsbDrvInfo.reset_hdlr == NULL)
			ASSERT(0);
		/* Set SW_RST bit here, so we do not care about any following ep interrupts */
		g_UsbDrvInfo.reset_hdlr(); 
		IntrTx = 0;
		IntrRx = 0;
	}	

	if ((IntrTx == 0)&& (IntrRx == 0))
		return;
		
	/* Check for endpoint 0 interrupt */
	if(IntrTx&USB_INTRTX_EP0){
		MSG(USB, "USB_INTRTX_EP0\n");
		if(g_UsbDrvInfo.ep0_hdlr == NULL)
			ASSERT(0);

		g_UsbDrvInfo.ep0_hdlr();
		IntrTx =IntrTx & ~USB_INTRTX_EP0;
	}

	/* Check for Bulk TX interrupt */
	/* TX interrupt : 1.TxPktRdy is clear, 2. SENTSTALL is set */
	if (IntrTx != 0){
		for(ep_num = 1; ep_num <= MAX_INTR_EP_NUM; ep_num++){
			if (IntrTx&(0x01 <<ep_num)){
//				MSG(USB, "ep_in_hdlr(%d)\n", ep_num-1);
				DRV_WriteReg8(USB_INDEX_V3, ep_num);
				//TXCSR = DRV_Reg16(USB_TXCSR);
				if(g_UsbDrvInfo.ep_in_hdlr[ep_num-1] != NULL)
					g_UsbDrvInfo.ep_in_hdlr[ep_num -1]();
			}
		}
	}

	/* Check for Bulk RX interrupt */
	/* RX interrupt : 1.RxPktRdy is set, 2. SENTSTALL is set */
	if (IntrRx != 0){
		for(ep_num = 1; ep_num <=MAX_INTR_EP_NUM; ep_num++){
			if(IntrRx&(0x01<<ep_num)){
//				MSG(USB, "ep_out_hdlr(%d)\n", ep_num-1);
				DRV_WriteReg8(USB_INDEX_V3, ep_num);
				RXCSR = DRV_Reg16(USB_RXCSR);
				if((RXCSR&USB_RXCSR_RXPKTRDY) != 0){	
					if(g_UsbDrvInfo.ep_out_hdlr[ep_num-1] != NULL)
						g_UsbDrvInfo.ep_out_hdlr[ep_num -1]();
					else
						ASSERT(0); /* No handler to receive this packet */
				}
			}
		}
	}
#elif	defined(USB_IP_V3)
		kal_uint16  IntrTx;
		kal_uint16  TXCSR;		
		kal_uint16  RXCSR;
		kal_uint16  IntrUSB;
		kal_uint16  IntrRx;
		kal_uint32  ep_num;

		IntrUSB = DRV_Reg8(USB_INTRUSB_V3);
		IntrTx = DRV_Reg16(USB_INTRTX);
		// Write 0 clear
		IntrRx = DRV_Reg16(USB_INTRRX);
		DRV_WriteReg16(USB_INTRRX, ~IntrRx);		
		
		if(IntrUSB & USB_INTRUSB_RESET)
		{	
			if(g_UsbDrvInfo.reset_hdlr == NULL)
				ASSERT(0);
		/* Set SW_RST bit here, so we do not care about any following ep interrupts */
			g_UsbDrvInfo.reset_hdlr(); 
		IntrTx = 0;
		IntrRx = 0;
		}	
			

		if ((IntrTx == 0)&& (IntrRx == 0))
			return;
		
		/* Check for endpoint 0 interrupt */
		if(IntrTx&USB_INTRTX_EP0)
		{
			if(g_UsbDrvInfo.ep0_hdlr == NULL)
				ASSERT(0);

			g_UsbDrvInfo.ep0_hdlr();
			IntrTx =IntrTx & ~USB_INTRTX_EP0;
		}
		/* Check for Bulk TX interrupt */
		/* TX interrupt : 1.TxPktRdy is clear, 2. SENTSTALL is set */
		if (IntrTx != 0)
		{
		for(ep_num = 1; ep_num <= MAX_INTR_EP_NUM; ep_num++)
		{
			if (IntrTx&(0x01 <<ep_num))
			{
				DRV_WriteReg8(USB_INDEX_V3, ep_num);
				TXCSR = DRV_Reg16(USB_TXCSR);					
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
				DRV_WriteReg8(USB_INDEX_V3, ep_num);
				RXCSR = DRV_Reg16(USB_RXCSR);			
				if((RXCSR&USB_RXCSR_RXPKTRDY) != 0)
				{			
					if(g_UsbDrvInfo.ep_out_hdlr[ep_num-1] != NULL)
						g_UsbDrvInfo.ep_out_hdlr[ep_num -1]();
					else
						/* No handler to receive this packet */
						ASSERT(0);
				}				
			}
		}		
		}
#elif	defined(USB_IP_V2)	
			volatile kal_uint8	IntrStatus;
			volatile kal_uint8	FMStatus;
			volatile kal_uint8	ErrorStatus;
			volatile kal_uint8	PhyStatus;
			volatile kal_uint8	direction;
			volatile kal_uint8	even_odd;
			volatile kal_uint8	ep_num;	
			volatile kal_bool	fast_mode_enable;	  
			usb_dma_callback	callback;

			fast_mode_enable = (DRV_Reg8(USB_FM_CTL)&VUSB_FM_CTL_FMENB) ? KAL_TRUE : KAL_FALSE;
			IntrStatus = DRV_Reg8(USB_INT_STAT);
			FMStatus = DRV_Reg8(USB_FM_ERR_STAT);
			ErrorStatus = DRV_Reg8(USB_ERR_STAT);
			PhyStatus = DRV_Reg8(USB_PHY_EXTRA);

			if ( FMStatus  &&  IntrStatus && ErrorStatus && (PhyStatus & ~VUSB_PHY_RESUME_INT_ENB))
						return;

			/* Ignore VUSB_INT_STAT_TOKEN_DONE in fast mode */
			if(((IntrStatus&VUSB_INT_STAT_TOKEN_DONE)&&(fast_mode_enable==KAL_FALSE)) || (FMStatus&VUSB_FM_ERR_STAT_TOKEN_DONE))
			{
						/* Parse FM status first */		
						if(FMStatus&VUSB_FM_ERR_STAT_TOKEN_DONE)
						{
								direction = (DRV_Reg8(USB_FM_ADDITNL_STAT) >> 3) & 0x01;
								even_odd = (DRV_Reg8(USB_FM_ADDITNL_STAT) >> 2) & 0x01;
								ep_num = (DRV_Reg8(USB_FM_ADDITNL_STAT) >> 4) & 0x0f;
						}
						else
						{
								direction = (DRV_Reg8(USB_STAT) >> 3) & 0x01;
								even_odd = (DRV_Reg8(USB_STAT) >> 2) & 0x01;
								ep_num = (DRV_Reg8(USB_STAT) >> 4) & 0x0f;
						}

							/* Note that token done should be cleared here in case FM is set during ep handler, 
							     so that intr will move to fm_intr*/
							/* Ignore token done in fast mode, so no clear its intr status in FM */
							/* First handle VUSB_FM_ERR_STAT_TOKEN_DONE, so do not clear token done intr status */
					if((fast_mode_enable==KAL_TRUE) || (FMStatus&VUSB_FM_ERR_STAT_TOKEN_DONE)) 
					{
								DRV_Reg8(USB_FM_ERR_STAT) = FMStatus;
					}
					else
					{
								DRV_WriteReg8(USB_INT_STAT, (IntrStatus&VUSB_INT_STAT_TOKEN_DONE));
					}
			
					if(ep_num==0)
					{
								g_UsbDrvInfo.ep0_current_dir = direction;
								/* Receive*/
								if(direction==USB_BDT_RX)
								{
									/* This one is received, software should handle this packet*/
										g_UsbDrvInfo.ep0_rx_even_odd = even_odd;

										if((g_UsbDrvInfo.ep0_transaction_type==USB_EP0_TRANS_TX_DATA)
														||(g_UsbDrvInfo.ep0_transaction_type==USB_EP0_TRANS_TX_DATA_END))
										{
														/* Host send status stage before Device expects*/
														g_UsbDrvInfo.ep0_transaction_type = USB_EP0_TRANS_END;
										}	

										if(g_UsbDrvInfo.ep0_transaction_type==USB_EP0_TRANS_TX_STATUS)
										{
														/* Host send status stage and match Device's expection */
													WRITE_EP0_BDT_PID(USB_BDT_RX, g_UsbDrvInfo.ep0_rx_even_odd,(USB_EP0_MAXP << VUSB_BDT_BC_SHIFT) |VUSB_BDT_OWNS_BIT);
													g_UsbDrvInfo.ep0_transaction_type = USB_EP0_TRANS_NONE;
										}
								}
								else
								{
													/* This one is transmitted, software will fill next packet*/
													g_UsbDrvInfo.ep0_tx_even_odd = even_odd^1;	

													if(g_UsbDrvInfo.ep0_transaction_type==USB_EP0_TRANS_RX_DATA)
													{
															g_UsbDrvInfo.ep0_transaction_type = USB_EP0_TRANS_END;
													}	

													if(g_UsbDrvInfo.ep0_transaction_type==USB_EP0_TRANS_TX_DATA_END)
													{
															g_UsbDrvInfo.ep0_transaction_type = USB_EP0_TRANS_TX_STATUS;
													}
					
													if(g_UsbDrvInfo.ep0_transaction_type==USB_EP0_TRANS_RX_STATUS)
													{
															g_UsbDrvInfo.ep0_transaction_type = USB_EP0_TRANS_NONE;
													}
								}

								if(g_UsbDrvInfo.ep0_hdlr == NULL)
										EXT_ASSERT(0, 0, 0, 0);
   							g_UsbDrvInfo.ep0_hdlr();
				}
				else if(direction == USB_BDT_RX)    /* Receive*/
				{
								if(g_UsbDrvInfo.ep_out_hdlr[ep_num-1]!=NULL)
   										g_UsbDrvInfo.ep_out_hdlr[ep_num -1]();

   							/* Not handle else because the fm token done may happen here */	
				}
				else	 	/* Send */
				{
								if(g_UsbDrvInfo.ep_in_hdlr[ep_num-1]!=NULL)
   										g_UsbDrvInfo.ep_in_hdlr[ep_num -1]();
   					//		USB_EP_Poll_Bulk_In_Ready(ep_num);  // make sure owner has change to MCU						
							/* Not handle else because the fm token done may happen here */		

								if(g_UsbDrvInfo.fast_mode_tx_ep == ep_num)
								{
												/* This is Fast mode token done intr */
											g_UsbDrvInfo.fast_mode_tx_ep = 0;	

											if((g_UsbDrvInfo.dma_res_tx_ep==0)&&(g_UsbDrvInfo.dma_tx_ep ==0))
											{
													/* No TX data residue, and DMA callback function has been done */
															callback = g_UsbDrvInfo.dma_callback[ep_num-1];
															g_UsbDrvInfo.dma_callback[ep_num-1] = NULL;
															if(callback!=NULL)
																callback();
											}
								}
								else if(g_UsbDrvInfo.dma_res_tx_ep == ep_num)
								{
												/* This is the last residue packet token done intr*/
													g_UsbDrvInfo.dma_res_tx_ep = 0;
					
													callback = g_UsbDrvInfo.dma_callback[ep_num-1];
													g_UsbDrvInfo.dma_callback[ep_num-1] = NULL;
													if(callback!=NULL)
														callback();
								}
						}	
				} /* End VUSB_INT_STAT_TOKEN_DONE*/
			
				/* Clear interrupt other than token done*/
				DRV_WriteReg8(USB_INT_STAT, (IntrStatus&(~VUSB_INT_STAT_TOKEN_DONE)));
				DRV_WriteReg8(USB_ERR_STAT, ErrorStatus);
#elif	defined(USB_IP_V1)||defined(USB_IP_V1_PLUS)		
		kal_uint8    IntrIn;
		kal_uint8    IntrOut;
		kal_uint32  ep_num;

		IntrIn = DRV_Reg8(USB_INTRIN1);
		IntrOut = DRV_Reg8(USB_INTROUT1);

		if( IntrIn== 0 && IntrOut == 0)
				return;
		
		/* Check for endpoint 0 interrupt */
		if (IntrIn&USB_INTRIN1_EP0)
		{
				if(g_UsbDrvInfo.ep0_hdlr == NULL)
					EXT_ASSERT(0, 0, 0, 0);
				g_UsbDrvInfo.ep0_hdlr();
				IntrIn =IntrIn & ~USB_INTRTX_EP0;
		}

		/* Check for Bulk in interrupt */
		if( IntrIn != 0)
		{
			for(ep_num = 1; ep_num <= MAX_INTR_EP_NUM; ep_num++)	
			{
				if (IntrIn & (0x01 <<ep_num))
   				{
   					if(g_UsbDrvInfo.ep_in_hdlr[ep_num-1] != NULL)
   					{
   						g_UsbDrvInfo.ep_in_hdlr[ep_num -1]();
   					}
   					else
   					{
						USB_CtrlEPStall(ep_num, USB_IN_EP_TYPE, KAL_TRUE, USB_CTRL_STALL_ENTRY_4);
					}
				}
			}
		}

		/* Check for Bulk out interrupt */
		if( IntrOut != 0)
		{
			for(ep_num = 1; ep_num <= MAX_INTR_EP_NUM; ep_num++)
			{
				if (IntrOut & (0x01 <<ep_num))
				{
					if(g_UsbDrvInfo.ep_out_hdlr[ep_num-1]!=NULL)
					{
						g_UsbDrvInfo.ep_out_hdlr[ep_num -1]();
					}
					else
					{
						USB_CtrlEPStall(ep_num, USB_OUT_EP_TYPE, KAL_TRUE, USB_CTRL_STALL_ENTRY_4);
					}
				}
			}
		}
#endif		
}

void USB_SetDPB(void)
{
    if((DRV_Reg8(USB_TXFIFOSZ) & USB_FIFOSZ_DPB) == 0)
    {
        DRV_WriteReg8(USB_TXFIFOSZ, (USB_FIFOSZ_SIZE_512|USB_FIFOSZ_DPB));
        DRV_WriteReg16(USB_TXFIFOADD, (g_FIFOadd/8));
        g_FIFOadd += USB_BULK_FIFO_UNIT_SIZE*2;
    }

    if((DRV_Reg8(USB_RXFIFOSZ) & USB_FIFOSZ_DPB) == 0)
    {
        DRV_WriteReg8(USB_RXFIFOSZ, (USB_FIFOSZ_SIZE_512|USB_FIFOSZ_DPB));
        DRV_WriteReg16(USB_RXFIFOADD, (g_FIFOadd/8));
        g_FIFOadd += USB_BULK_FIFO_UNIT_SIZE*2;
    }
}


