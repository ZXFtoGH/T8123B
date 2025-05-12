#include <sys/types.h>
#include <string.h>
#include <debug.h>
#include <error_code.h>
#include "cldma_reg_ops.h"
#include "cldma_err.h"
//#include "dev/gpt_timer/gpt_timer.h"



/*********************************************************************************************
 * Configure region
 *********************************************************************************************/
#define TGPD_N	1
#define RGPD_N	1
#define LOCAL_BUFF_SIZE 2048

/*********************************************************************************************
 * GPD structure operate function
 *********************************************************************************************/
#define GPD_FLAGS_HWO 	 	 		0x01
#define GPD_FLAGS_BDP 	 	 		0x02
#define GPD_FLAGS_BPS 	 	 		0x04
#define GPD_FLAGS_IOC 	 	 		0x80

/* TGPD & RGPD */
typedef struct _GPD {
	unsigned int flag:8;
	unsigned int reserved:8;
	unsigned int allow_len:16;

	struct _GPD *pNext;

	unsigned char *pBuf;

	unsigned int bufLen:16;
	unsigned int dbg_id:16;
} GPD;


#define GPD_IS_FLAGS_HWO(_pd)      	(((volatile GPD *)(_pd))->flag & GPD_FLAGS_HWO)
#define GPD_SET_FLAGS_HWO(_pd)		(((GPD *)(_pd))->flag |= GPD_FLAGS_HWO)
#define GPD_CLR_FLAGS_HWO(_pd)		(((GPD *)(_pd))->flag &= (~GPD_FLAGS_HWO))
#define GPD_IS_FLAGS_BDP(_pd)      	(((GPD *)(_pd))->flag & GPD_FLAGS_BDP)
#define GPD_SET_FLAGS_BDP(_pd)    	(((GPD *)(_pd))->flag |= GPD_FLAGS_BDP)
#define GPD_CLR_FLAGS_BDP(_pd)		(((GPD *)(_pd))->flag &= (~GPD_FLAGS_BDP))
#define GPD_IS_FLAGS_BPS(_pd)      	(((GPD *)(_pd))->flag & GPD_FLAGS_BPS)
#define GPD_SET_FLAGS_BPS(_pd)     	(((GPD *)(_pd))->flag |= GPD_FLAGS_BPS)
#define GPD_CLR_FLAGS_BPS(_pd)		(((GPD *)(_pd))->flag &= (~GPD_FLAGS_BPS))
#define GPD_IS_FLAGS_IOC(_pd)      	(((GPD *)(_pd))->flag & GPD_FLAGS_IOC)
#define GPD_SET_FLAGS_IOC(_pd)		(((GPD *)(_pd))->flag |= GPD_FLAGS_IOC)
#define GPD_CLR_FLAGS_IOC(_pd)		(((GPD *)(_pd))->flag &= (~GPD_FLAGS_IOC))

/* TGPD & RGPD */
#define GPD_SET_FLAG(_pd, _flag)   	((GPD *)(_pd))->flag = _flag
#define GPD_GET_FLAG(_pd)          	((GPD *)(_pd))->flag
#define GPD_SET_NEXT(_pd, _next)   	((GPD *)(_pd))->pNext = (GPD *)_next
#define GPD_GET_NEXT(_pd)          	(GPD *)((GPD *)(_pd))->pNext

#define GPD_SET_DATA(_pd, _data)   	((GPD *)(_pd))->pBuf = (unsigned char *)_data
#define GPD_GET_DATA(_pd)          	(unsigned char*)((GPD *)(_pd))->pBuf
#define GPD_SET_BUF_LEN(_pd, _len) 	((GPD *)(_pd))->bufLen = _len
#define GPD_ADD_BUF_LEN(_pd, _len) 	((GPD *)(_pd))->bufLen += _len
#define GPD_GET_BUF_LEN(_pd)       	((GPD *)(_pd))->bufLen


/* RGPD */
#define GPD_SET_ALLEN(_pd, _len)   	((GPD *)(_pd))->allow_len = _len
#define GPD_GET_ALLEN(_pd)          ((GPD *)(_pd))->allow_len

static GPD g_TGPD[TGPD_N +1];
static GPD g_RGPD[RGPD_N +1];

static unsigned char  s_local_rx_buf[LOCAL_BUFF_SIZE];
static unsigned int s_local_buf_avai;
static unsigned int s_local_buf_curr;
static unsigned int s_cldma_init_done;

#define mb()

static void cldma_tx_start(void)
{
	LOGI("CLDMA Tx start \n");
	cldma_tx_start_addr((void*)g_TGPD);
	cldma_start_tx_queue();
}

static void cldma_rx_start(void)
{
	LOGI("CLDMA Rx start \n");
	cldma_rx_start_addr((void*)g_RGPD);
	cldma_start_rx_queue();
}

static void cfg_tx_gpd(unsigned int i, unsigned char *ptr, unsigned int size)
{
	if (ptr && size) {
		GPD_CLR_FLAGS_IOC(&g_TGPD[i]);
		GPD_CLR_FLAGS_BPS(&g_TGPD[i]);
		GPD_SET_DATA(&g_TGPD[i], ptr);
		GPD_SET_BUF_LEN(&g_TGPD[i], (size & 0xFFFF));
		GPD_SET_FLAGS_HWO(&g_TGPD[i]);
	} else {
		GPD_CLR_FLAGS_HWO(&g_TGPD[i]);
	}
}

static void cfg_rx_gpd(unsigned int i, unsigned char *ptr, unsigned int size)
{
	if (ptr && size) {
		GPD_CLR_FLAGS_IOC(&g_RGPD[i]);
		GPD_CLR_FLAGS_BPS(&g_RGPD[i]);
		GPD_SET_DATA(&g_RGPD[i], ptr);
		GPD_SET_BUF_LEN(&g_RGPD[i], 0);
		GPD_SET_ALLEN(&g_RGPD[i], size & 0xFFFF);
		GPD_SET_FLAGS_HWO(&g_RGPD[i]);
	} else {
		GPD_CLR_FLAGS_HWO(&g_RGPD[i]);
	}
}

static void set_gpd_tail(GPD *gpd)
{
	GPD_CLR_FLAGS_HWO(gpd);		/* HWO=0 */
	GPD_CLR_FLAGS_BPS(gpd);		/* BPS=0 */
	GPD_SET_DATA(gpd, NULL);
	GPD_SET_NEXT(gpd, NULL);
}

static unsigned int cldma_tx_wait_idle(unsigned int start_tick, unsigned int timeout_tick)
{
	if (cldma_tx_active_status()) {
		cldma_stop_tx_queue();
		while (cldma_tx_active_status()) {
			if (gpt4_timeout_tick(start_tick, timeout_tick))
				return !0;
		}
	}
	cldma_clear_tx_int_status();
	return 0;
}

static unsigned int cldma_rx_wait_idle(unsigned int start_tick, unsigned int timeout_tick)
{
	if (cldma_rx_active_status()) {
		cldma_stop_rx_queue();
		while (cldma_rx_active_status()) {
			if (gpt4_timeout_tick(start_tick, timeout_tick))
				return !0;
		}
	}
	cldma_clear_rx_int_status();
	return 0;
}

static unsigned int cldma_tx_wait_fsm_err_to_rdy(unsigned int start_tick, unsigned int timeout_tick)
{
	if (!cldma_get_tx_fsm_err())
		return 0;

	cldma_clr_tx_fsm_err();
	while (cldma_get_tx_fsm_err()) {
		if (gpt4_timeout_tick(start_tick, timeout_tick))
			return !0;
	}

	return 0;
}

static unsigned int cldma_rx_wait_fsm_err_to_rdy(unsigned int start_tick, unsigned int timeout_tick)
{
	if (!cldma_get_rx_fsm_err())
		return 0;

	cldma_clr_rx_fsm_err();
	while (cldma_get_rx_fsm_err()) {
		if (gpt4_timeout_tick(start_tick, timeout_tick))
			return !0;
	}

	return 0;
}


static unsigned int check_tx_error(void)
{
	unsigned int err;

	err = cldma_tx_int_status();
	if (!err)
		return 0;

	err &= (~(1 << TX_DONE_INT)); // Clear tx done
	err &= (~(1 << TX_QE_INT)); // Clear tx queue empty
	err &= (~(1 << TX_RX_INDMA_START_CMD)); // Clear write command

	return err;
}


static unsigned int check_rx_error(void)
{
	unsigned int err;

	err = cldma_rx_int_status();
	if (!err)
		return 0;

	err &= (~(1 << RX_DONE_INT)); // Clear rx done
	err &= (~(1 << RX_QE_INT)); // Clear rx queue empty

	return err;
}

#define CLDMA_WAIT_DONE			0
#define CLDMA_WAIT_TIMEOUT		1
#define CLDMA_WAIT_HW_ERR_ASSERT	2

static unsigned int wait_tx_gpd_done(unsigned int num, unsigned int start_tick, unsigned int tiemout_tick)
{
	unsigned int i;

	for (i  = 0; i  < num; i ++) {
		while (1) {
			if (!(GPD_IS_FLAGS_HWO(&g_TGPD[i])))
				break;
			if (check_tx_error())
				return CLDMA_WAIT_HW_ERR_ASSERT;
			if (gpt4_timeout_tick(start_tick, tiemout_tick))
				return CLDMA_WAIT_TIMEOUT;
		}
	}
	return CLDMA_WAIT_DONE;
}

static unsigned int wait_rx_gpd_done(unsigned int num, unsigned int start_tick, unsigned int timeout_tick)
{
	unsigned int i;

	for (i  = 0; i  < num; i ++) {
		while (1) {
			if (!(GPD_IS_FLAGS_HWO(&g_RGPD[i]))) {
				LOGE("HWO bit !=0, receving data \n");
				break;
			}
			if (check_rx_error())
				return CLDMA_WAIT_HW_ERR_ASSERT;
			if (gpt4_timeout_tick(start_tick, timeout_tick))
				return CLDMA_WAIT_TIMEOUT;
		}
	}
	return CLDMA_WAIT_DONE;
}


unsigned int CLDMA_PutData(unsigned char* buf, unsigned int len, unsigned int timeout)
{
	unsigned int sent_num = 0, send_num_curr = 0;
	unsigned int timeout_tick;
	unsigned int start_tick;
	unsigned int ret = 0;
	unsigned int wait_err_code = 0;

	if (buf == NULL) {
		LOGE("CLDMA_PutData buf NULL\n");
		ret = CLDMA_ERR_TX_BUF_NULL;
		goto exit;
	}

	if (len == 0) {
		LOGE("CLDMA_PutData buf size 0\n");
		ret = CLDMA_ERR_TX_BUF_ZERO;
		goto exit;
	}

	if (timeout < 100)
		timeout = 100;

	timeout_tick = gpt4_time2tick_ms(timeout); //ms -> tick
	start_tick = gpt4_get_current_tick();

	if (cldma_tx_wait_idle(start_tick, timeout_tick) != 0) {
		LOGE("CLDMA_PutData IDLE TO\n");
		ret = CLDMA_ERR_TX_WAIT_IDLE_TIMEOUT;
		goto exit;
	}

	if (cldma_tx_wait_fsm_err_to_rdy(start_tick, timeout_tick) != 0) {
		LOGE("CLDMA_PutData FSM FAIL\n");
		ret = CLDMA_ERR_TX_WAIT_FSM_TIMEOUT;
		goto exit;
	}

	while (len) {
		if (len > LOCAL_BUFF_SIZE) {
			cfg_tx_gpd(0, &buf[sent_num], LOCAL_BUFF_SIZE);
			send_num_curr = LOCAL_BUFF_SIZE;
		} else {
			/* last or only 1 packet need to send */
			cfg_tx_gpd(0, &buf[sent_num], len);
			send_num_curr = len;
		}
		mb();
		cldma_tx_start();
		wait_err_code = wait_tx_gpd_done(1, start_tick, timeout_tick);

		if (wait_err_code == CLDMA_WAIT_HW_ERR_ASSERT) {
			LOGE("CLDMA_PutData HW ASSERT\n");
			ret = CLDMA_ERR_TX_HW_ERR_ASSERT;
			goto err;
		} else if (wait_err_code == CLDMA_WAIT_TIMEOUT) {
			LOGE("CLDMA_PutData WAIT TO\n");
			ret = CLDMA_ERR_TX_ERR_WRITE_TIMEOUT;
			goto err;
		}

		sent_num += send_num_curr;
		len -= send_num_curr;
	}

err:
	cldma_stop_tx_queue();

exit:
	if (len != 0) {
		LOGI("cldma put data fail! %u:%u:%d\n", sent_num, len, ret);
		return STATUS_ERR;
	}

	return STATUS_OK;
}


unsigned int CLDMA_GetData(unsigned char* buf, unsigned int len, unsigned int timeout)
{
	unsigned int ret = 0, recv_num = 0;
	unsigned int wait_read_len = 0;
	unsigned int timeout_tick;
	unsigned int start_tick;
	unsigned char *tmp_ptr;
	unsigned int need_cpy = 0, get_len, cpy_size;

	unsigned int wait_err_code = 0;

	if (buf == NULL) {
		LOGE("CLDMA_GetData buf NULL\n");
		ret = CLDMA_ERR_RX_BUF_NULL;
		goto exit;
	}

	if (len == 0) {
		LOGE("CLDMA_GetData buf size 0\n");
		ret = CLDMA_ERR_RX_BUF_ZERO;
		goto exit;
	}

	if (timeout < 100)
		timeout = 100;

	timeout_tick = gpt4_time2tick_ms(timeout);
	start_tick = gpt4_get_current_tick();

	if (s_local_buf_avai) {
		if (s_local_buf_avai >= len) {
			memcpy(buf, &s_local_rx_buf[s_local_buf_curr], len);
			s_local_buf_curr += len;
			s_local_buf_avai -= len;
			return STATUS_OK;
		}

		memcpy(buf, &s_local_rx_buf[s_local_buf_curr], s_local_buf_avai);
		buf += s_local_buf_avai;
		len -= s_local_buf_avai;
		s_local_buf_curr = 0;
		s_local_buf_avai = 0;
	}

	wait_read_len = len;
	tmp_ptr = buf;

	while (wait_read_len) {
		if (cldma_rx_wait_idle(start_tick, timeout_tick) != 0) {
			ret = CLDMA_ERR_RX_WAIT_IDLE_TIMEOUT;
			LOGE("CLDMA_GetData IDLE TO\n");
			goto err;
		}

		if (cldma_rx_wait_fsm_err_to_rdy(start_tick, timeout_tick) != 0) {
			ret = CLDMA_ERR_RX_WAIT_FSM_TIMEOUT;
			LOGE("CLDMA_GetData FSM TO\n");
			goto err;
		}

		if (wait_read_len >= LOCAL_BUFF_SIZE) {
			cfg_rx_gpd(0, tmp_ptr, LOCAL_BUFF_SIZE);
			need_cpy = 0;
		} else {
			/* Configure rx GPD */
			cfg_rx_gpd(0, s_local_rx_buf, LOCAL_BUFF_SIZE);
			need_cpy = 1;
		}
		mb();

		cldma_rx_start();

		wait_err_code = wait_rx_gpd_done(1, start_tick, timeout_tick);
		if (wait_err_code == CLDMA_WAIT_HW_ERR_ASSERT) {
			LOGE("CLDMA_GetData HW ERR\n");
			ret = CLDMA_ERR_RX_HW_ERR_ASSERT;
			goto err;
		} else if (wait_err_code == CLDMA_WAIT_TIMEOUT) {
			LOGE("CLDMA_GetData WAIT TO\n");
			ret = CLDMA_ERR_RX_ERR_READ_TIMEOUT;
			goto err;
		}

		get_len = GPD_GET_BUF_LEN(&g_RGPD[0]);

		if (need_cpy) {
			if (wait_read_len >= get_len)
				cpy_size = get_len;
			else {
				cpy_size = wait_read_len;
				s_local_buf_curr = cpy_size;
				s_local_buf_avai = get_len - cpy_size;
				get_len = wait_read_len; /* Note here */
			}
			memcpy(tmp_ptr, s_local_rx_buf, cpy_size);
		}

		wait_read_len -= get_len;
		tmp_ptr += get_len;
		recv_num += get_len;

		cldma_stop_rx_queue();
	}

err:
	cldma_clear_rx_done_status();
	cldma_stop_rx_queue();

exit:
	if (recv_num != len) {
		LOGI("cldma get data fail! %u:%u:%d\n", recv_num, len, ret);
		return STATUS_ERR;
	}

	return STATUS_OK;
}


static void cldma_buffer_init(void)
{
	int i = 0;

	memset(g_TGPD, 0, sizeof(GPD) * (TGPD_N + 1));
	memset(g_RGPD, 0, sizeof(GPD) * (RGPD_N + 1));

	for (i = 0; i < TGPD_N; i++)
		GPD_SET_NEXT(&g_TGPD[i], &g_TGPD[i + 1]);


	for (i = 0; i < RGPD_N; i++)
		GPD_SET_NEXT(&g_RGPD[i], &g_RGPD[i + 1]);

}

unsigned int CLDMA_Init(void)
{
	LOGI("CLDMA_DA_init \n");

	if (s_cldma_init_done)
		return 0;

	s_cldma_init_done = 1;

	/* set domain ID */
	cldma_reg_set_domain_id();

	/* buffer init */
	cldma_buffer_init();

	/* stop all queue */
	cldma_stop_all_queues();

	/* clear interrupt status */
	cldma_clear_interrupt_status();

	return 0;
}


