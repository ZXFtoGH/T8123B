/* SPDX-License-Identifier: GPL-2.0 */
/*  Himax Android Driver Sample Code for QCT platform
 *
 *  Copyright (C) 2019 Himax Corporation.
 *
 *  This software is licensed under the terms of the GNU General Public
 *  License version 2,  as published by the Free Software Foundation,  and
 *  may be copied,  distributed,  and modified under those terms.
 *
 *  This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 */

#include "himax_platform.h"
#include "himax_common.h"

int i2c_error_count;
bool ic_boot_done;
u8 *gp_rw_buf;
/*  Turn on it when there is necessary.
 *	#if defined(HX_RW_FILE)
 *	MODULE_IMPORT_NS(
 *	VFS_internal_I_am_really_a_filesystem_and_am_NOT_a_driver);
 *	MODULE_IMPORT_NS(ANDROID_GKI_VFS_EXPORT_ONLY);
 *
 *	mm_segment_t g_fs;
 *	struct file *g_fn;
 *	loff_t g_pos;
 *
 *	int hx_open_file(char *file_name)
 *	{
 *
 *		int ret = -EFAULT;
 *
 *		g_fn = NULL;
 *		g_pos = 0;
 *
 *
 *		I("Now file name=%s\n", file_name);
 *		g_fn = filp_open(file_name,
 *			O_TRUNC|O_CREAT|O_RDWR, 0660);
 *		if (IS_ERR(g_fn))
 *			E("%s Open file failed!\n", __func__);
 *		else {
 *			ret = NO_ERR;
 *			g_fs = get_fs();
 *			set_fs(KERNEL_DS);
 *		}
 *		return ret;
 *	}
 *	int hx_write_file(char *write_data, uint32_t write_size, loff_t pos)
 *	{
 *		int ret = NO_ERR;
 *
 *		g_pos = pos;
 *		kernel_write(g_fn, write_data,
 *		write_size * sizeof(char), &g_pos);
 *
 *
 *		return ret;
 *	}
 *	int hx_close_file(void)
 *	{
 *		int ret = NO_ERR;
 *
 *		filp_close(g_fn, NULL);
 *		set_fs(g_fs);
 *
 *		return ret;
 *	}
 *	#endif
 */
int himax_dev_set(struct himax_ts_data *ts)
{
	int ret = 0;

	ts->input_dev = input_allocate_device();

	if (ts->input_dev == NULL) {
		ret = -ENOMEM;
		E("%s: Failed to allocate input device-input_dev\n", __func__);
		return ret;
	}

	ts->input_dev->name = "himax-touchscreen";

	if (!ic_data->HX_STYLUS_FUNC)
		goto skip_stylus_operation;

	ts->stylus_dev = input_allocate_device();

	if (ts->stylus_dev == NULL) {
		ret = -ENOMEM;
		E("%s: Failed to allocate input device-stylus_dev\n", __func__);
		input_free_device(ts->input_dev);
		return ret;
	}

	ts->stylus_dev->name = "himax-stylus";
skip_stylus_operation:

	return ret;
}
int himax_input_register_device(struct input_dev *input_dev)
{
	return input_register_device(input_dev);
}

int himax_parse_dt(struct himax_ts_data *ts, struct himax_platform_data *pdata)
{
	int rc, coords_size = 0;
	uint32_t coords[4] = {0};
	struct property *prop;
	struct device_node *dt = private_ts->client->dev.of_node;
	u32 data = 0;
	int ret = 0;

	prop = of_find_property(dt, "himax,panel-coords", NULL);

	if (prop) {
		coords_size = prop->length / sizeof(u32);

		if (coords_size != 4)
			D(" %s:Invalid panel coords size %d\n",
				__func__, coords_size);
	}

	ret = of_property_read_u32_array(dt, "himax,panel-coords",
			coords, coords_size);
	if (ret == 0) {
		pdata->abs_x_min = coords[0];
		pdata->abs_x_max = (coords[1] - 1);
		pdata->abs_y_min = coords[2];
		pdata->abs_y_max = (coords[3] - 1);
		I(" DT-%s:panel-coords = %d, %d, %d, %d\n", __func__,
				pdata->abs_x_min,
				pdata->abs_x_max,
				pdata->abs_y_min,
				pdata->abs_y_max);
	}

	prop = of_find_property(dt, "himax,display-coords", NULL);

	if (prop) {
		coords_size = prop->length / sizeof(u32);

		if (coords_size != 4)
			D(" %s:Invalid display coords size %d\n",
				__func__, coords_size);
	}

	rc = of_property_read_u32_array(dt, "himax,display-coords",
			coords, coords_size);

	if (rc && (rc != -EINVAL)) {
		D(" %s:Fail to read display-coords %d\n", __func__, rc);
		return rc;
	}

	pdata->screenWidth  = coords[1];
	pdata->screenHeight = coords[3];
	I(" DT-%s:display-coords = (%d, %d)\n", __func__,
			pdata->screenWidth,
			pdata->screenHeight);
	pdata->gpio_irq = of_get_named_gpio(dt, "himax,irq-gpio", 0);

	if (!gpio_is_valid(pdata->gpio_irq))
		I(" DT:gpio_irq value is not valid\n");

	pdata->gpio_reset = of_get_named_gpio(dt, "himax,rst-gpio", 0);

	if (!gpio_is_valid(pdata->gpio_reset))
		I(" DT:gpio_rst value is not valid\n");

#if defined(HX_PON_PIN_SUPPORT)
	pdata->gpio_pon = of_get_named_gpio(dt, "himax,pon-gpio", 0);

	if (!gpio_is_valid(pdata->gpio_pon))
		I(" DT:gpio_pon value is not valid\n");

	pdata->lcm_rst = of_get_named_gpio(dt, "himax,lcm-rst", 0);

	if (!gpio_is_valid(pdata->lcm_rst))
		I(" DT:tp-rst value is not valid\n");

	I(" DT:pdata->gpio_pon=%d, pdata->lcm_rst=%d\n",
		pdata->gpio_pon, pdata->lcm_rst);
#endif

	pdata->gpio_3v3_en = of_get_named_gpio(dt, "himax,3v3-gpio", 0);

	if (!gpio_is_valid(pdata->gpio_3v3_en))
		I(" DT:gpio_3v3_en value is not valid\n");

	I(" DT:gpio_irq=%d, gpio_rst=%d, gpio_3v3_en=%d\n",
			pdata->gpio_irq,
			pdata->gpio_reset,
			pdata->gpio_3v3_en);

	if (of_property_read_u32(dt, "report_type", &data) == 0) {
		pdata->protocol_type = data;
		I(" DT:protocol_type=%d\n", pdata->protocol_type);
	}

#if defined(HX_FIRMWARE_HEADER)
	mapping_panel_id_from_dt(dt);
#endif
	return 0;
}
EXPORT_SYMBOL(himax_parse_dt);

#if defined(HX_PARSE_FROM_DT)
static void hx_generate_ic_info_from_dt(
	uint32_t proj_id, char *buff, char *main_str, char *item_str)
{
	if (proj_id == 0xffff)
		snprintf(buff, 128, "%s,%s", main_str, item_str);
	else
		snprintf(buff, 128, "%s_%04X,%s", main_str, proj_id, item_str);

}
static void hx_generate_fw_name_from_dt(
	uint32_t proj_id, char *buff, char *main_str, char *item_str)
{
	if (proj_id == 0xffff)
		snprintf(buff, 128, "%s_%s", main_str, item_str);
	else
		snprintf(buff, 128, "%s_%04X_%s", main_str, proj_id, item_str);

}

void himax_parse_dt_ic_info(struct himax_ts_data *ts,
	struct himax_platform_data *pdata)
{
	struct device_node *dt = ts->dev->of_node;
	u32 data = 0;
	char *str_rx_num = "rx-num";
	char *str_tx_num = "tx-num";
	char *str_bt_num = "bt-num";
	char *str_max_pt = "max-pt";
	char *str_int_edge = "int-edge";
	char *str_stylus_func = "stylus-func";
	char *str_firmware_name_tail = "firmware.bin";
	char *str_mp_firmware_name_tail = "mpfw.bin";
	char buff[128] = {0};

	hx_generate_ic_info_from_dt(g_proj_id, buff, ts->chip_name, str_rx_num);
	if (of_property_read_u32(dt, buff, &data) == 0) {
		ic_data->HX_RX_NUM = data;
		I("%s,Now parse:%s=%d\n", __func__, buff, ic_data->HX_RX_NUM);
	} else
		I("%s, No definition: %s!\n", __func__, buff);

	hx_generate_ic_info_from_dt(g_proj_id, buff, ts->chip_name, str_tx_num);
	if (of_property_read_u32(dt, buff, &data) == 0) {
		ic_data->HX_TX_NUM = data;
		I("%s,Now parse:%s=%d\n", __func__, buff, ic_data->HX_TX_NUM);
	} else
		I("%s, No definition: %s!\n", __func__, buff);

	hx_generate_ic_info_from_dt(g_proj_id, buff, ts->chip_name, str_bt_num);
	if (of_property_read_u32(dt, buff, &data) == 0) {
		ic_data->HX_BT_NUM = data;
		I("%s,Now parse:%s=%d\n", __func__, buff, ic_data->HX_BT_NUM);
	} else
		I("%s, No definition: %s!\n", __func__, buff);

	hx_generate_ic_info_from_dt(g_proj_id, buff, ts->chip_name, str_max_pt);
	if (of_property_read_u32(dt, buff, &data) == 0) {
		ic_data->HX_MAX_PT = data;
		I("%s,Now parse:%s=%d\n", __func__, buff, ic_data->HX_MAX_PT);
	} else
		I("%s, No definition: %s!\n", __func__, buff);

	hx_generate_ic_info_from_dt(
			g_proj_id, buff, ts->chip_name, str_int_edge);
	if (of_property_read_u32(dt, buff, &data) == 0) {
		ic_data->HX_INT_IS_EDGE = data;
		I("%s,Now parse:%s=%d\n",
			__func__, buff, ic_data->HX_INT_IS_EDGE);
	} else
		I("%s, No definition: %s!\n", __func__, buff);

	hx_generate_ic_info_from_dt(g_proj_id, buff,
		ts->chip_name, str_stylus_func);
	if (of_property_read_u32(dt, buff, &data) == 0) {
		ic_data->HX_STYLUS_FUNC = data;
		I("%s,Now parse:%s=%d\n",
			__func__, buff, ic_data->HX_STYLUS_FUNC);
	} else
		I("%s, No definition: %s!\n", __func__, buff);

	hx_generate_fw_name_from_dt(g_proj_id, buff, ts->chip_name,
		str_firmware_name_tail);
	I("%s,buff=%s!\n", __func__, buff);
#if defined(HX_BOOT_UPGRADE)
	g_fw_boot_upgrade_name = kzalloc(sizeof(buff), GFP_KERNEL);
	memcpy(&g_fw_boot_upgrade_name[0], &buff[0], sizeof(buff));
	I("%s,g_fw_boot_upgrade_name=%s!\n",
		__func__, g_fw_boot_upgrade_name);
#endif

	hx_generate_fw_name_from_dt(g_proj_id, buff, ts->chip_name,
		str_mp_firmware_name_tail);
	I("%s,buff=%s!\n", __func__, buff);
#if defined(HX_ZERO_FLASH)
	g_fw_mp_upgrade_name = kzalloc(sizeof(buff), GFP_KERNEL);
	memcpy(&g_fw_mp_upgrade_name[0], &buff[0], sizeof(buff));
	I("%s,g_fw_mp_upgrade_name=%s!\n", __func__, g_fw_mp_upgrade_name);
#endif

	I(" DT:rx, tx, bt, pt, int, stylus\n");
	I(" DT:%d, %d, %d, %d, %d, %d\n", ic_data->HX_RX_NUM,
		ic_data->HX_TX_NUM, ic_data->HX_BT_NUM, ic_data->HX_MAX_PT,
		ic_data->HX_INT_IS_EDGE, ic_data->HX_STYLUS_FUNC);
}
EXPORT_SYMBOL(himax_parse_dt_ic_info);
#endif

int himax_bus_read(uint8_t cmd, uint8_t *buf, uint32_t len)
{
	int retry;
	struct i2c_client *client = private_ts->client;
	uint8_t hw_addr = private_ts->select_slave_reg == true ?
			private_ts->slave_read_reg : client->addr;
	struct i2c_msg msg[] = {
		{
			.addr = hw_addr,
			.flags = 0,
			.len = 1,
			.buf = &cmd,
		},
		{
			.addr = hw_addr,
			.flags = I2C_M_RD,
			.len = len,
			.buf = gp_rw_buf,
		}
	};

	if (len > BUS_R_DLEN) {
		E("%s: len[%d] is over %d\n", __func__, len, BUS_R_DLEN);
		return -EFAULT;
	}

	mutex_lock(&private_ts->rw_lock);

	for (retry = 0; retry < HIMAX_BUS_RETRY_TIMES; retry++) {
		if (i2c_transfer(client->adapter, msg, 2) == 2) {
			memcpy(buf, gp_rw_buf, len);
			break;
		}
		/*msleep(20);*/
	}

	if (retry == HIMAX_BUS_RETRY_TIMES) {
		E("%s: i2c_read_block retry over %d\n",
		  __func__, HIMAX_BUS_RETRY_TIMES);
		i2c_error_count = HIMAX_BUS_RETRY_TIMES;
		mutex_unlock(&private_ts->rw_lock);
		return -EIO;
	}

	mutex_unlock(&private_ts->rw_lock);
	return 0;
}
EXPORT_SYMBOL(himax_bus_read);

int himax_bus_write(uint8_t cmd, uint8_t *addr, uint8_t *data, uint32_t len)
{
	int retry/*, loop_i*/;
	uint8_t offset = 0;
	uint32_t tmp_len = len;
	struct i2c_client *client = private_ts->client;
	uint8_t hw_addr = private_ts->select_slave_reg == true ?
			private_ts->slave_write_reg : client->addr;
	struct i2c_msg msg[] = {
		{
			.addr = hw_addr,
			.flags = 0,
			.len = len+BUS_W_HLEN,
			.buf = gp_rw_buf,
		}
	};

	if (len > BUS_W_DLEN) {
		E("%s: len[%d] is over %d\n", __func__, len, BUS_W_DLEN);
		return -EFAULT;
	}

	mutex_lock(&private_ts->rw_lock);
	gp_rw_buf[0] = cmd;
	offset = BUS_W_HLEN;

	if (addr != NULL) {
		memcpy(gp_rw_buf+offset, addr, 4);
		offset += 4;
		tmp_len -= 4;
	}

	if (data != NULL)
		memcpy(gp_rw_buf+offset, data, tmp_len);

	for (retry = 0; retry < HIMAX_BUS_RETRY_TIMES; retry++) {
		if (i2c_transfer(client->adapter, msg, 1) == 1)
			break;

		/*msleep(20);*/
	}

	if (retry == HIMAX_BUS_RETRY_TIMES) {
		E("%s: i2c_write_block retry over %d\n",
		  __func__, HIMAX_BUS_RETRY_TIMES);
		i2c_error_count = HIMAX_BUS_RETRY_TIMES;
		mutex_unlock(&private_ts->rw_lock);
		return -EIO;
	}

	mutex_unlock(&private_ts->rw_lock);
	return 0;
}
EXPORT_SYMBOL(himax_bus_write);


void himax_int_enable(int enable)
{
	struct himax_ts_data *ts = private_ts;
	unsigned long irqflags = 0;
	int irqnum = ts->client->irq;

	spin_lock_irqsave(&ts->irq_lock, irqflags);
	I("%s: Entering!\n", __func__);
	if (enable == 1 && atomic_read(&ts->irq_state) == 0) {
		atomic_set(&ts->irq_state, 1);
		enable_irq(irqnum);
		private_ts->irq_enabled = 1;
	} else if (enable == 0 && atomic_read(&ts->irq_state) == 1) {
		atomic_set(&ts->irq_state, 0);
		disable_irq_nosync(irqnum);
		private_ts->irq_enabled = 0;
	}

	I("enable = %d\n", enable);
	spin_unlock_irqrestore(&ts->irq_lock, irqflags);
}
EXPORT_SYMBOL(himax_int_enable);

#if defined(HX_RST_PIN_FUNC)
void himax_rst_gpio_set(int pinnum, uint8_t value)
{
	gpio_direction_output(pinnum, value);
}
EXPORT_SYMBOL(himax_rst_gpio_set);
#endif

uint8_t himax_int_gpio_read(int pinnum)
{
	return gpio_get_value(pinnum);
}

#if defined(CONFIG_HMX_DB)
static int himax_regulator_configure(struct himax_platform_data *pdata)
{
	int retval;
	struct i2c_client *client = private_ts->client;

	pdata->vcc_dig = regulator_get(&client->dev, "vdd");

	if (IS_ERR(pdata->vcc_dig)) {
		E("%s: Failed to get regulator vdd\n",
		  __func__);
		retval = PTR_ERR(pdata->vcc_dig);
		return retval;
	}

	pdata->vcc_ana = regulator_get(&client->dev, "avdd");

	if (IS_ERR(pdata->vcc_ana)) {
		E("%s: Failed to get regulator avdd\n",
		  __func__);
		retval = PTR_ERR(pdata->vcc_ana);
		regulator_put(pdata->vcc_dig);
		return retval;
	}

	return 0;
};

static void himax_regulator_deinit(struct himax_platform_data *pdata)
{
	I("%s: entered.\n", __func__);

	if (!IS_ERR(pdata->vcc_ana))
		regulator_put(pdata->vcc_ana);

	if (!IS_ERR(pdata->vcc_dig))
		regulator_put(pdata->vcc_dig);

	I("%s: regulator put, completed.\n", __func__);
};

static int himax_power_on(struct himax_platform_data *pdata, bool on)
{
	int retval;

	if (on) {
		retval = regulator_enable(pdata->vcc_dig);

		if (retval) {
			E("%s: Failed to enable regulator vdd\n",
			  __func__);
			return retval;
		}

		/*msleep(100);*/
		usleep_range(1000, 1001);
		retval = regulator_enable(pdata->vcc_ana);

		if (retval) {
			E("%s: Failed to enable regulator avdd\n",
			  __func__);
			regulator_disable(pdata->vcc_dig);
			return retval;
		}
	} else {
		regulator_disable(pdata->vcc_dig);
		regulator_disable(pdata->vcc_ana);
	}

	return 0;
}

int himax_gpio_power_config(struct himax_platform_data *pdata)
{
	int error;
	struct i2c_client *client = private_ts->client;

	error = himax_regulator_configure(pdata);

	if (error) {
		E("Failed to intialize hardware\n");
		goto err_regulator_not_on;
	}

#if defined(HX_RST_PIN_FUNC)

	if (gpio_is_valid(pdata->gpio_reset)) {
		/* configure touchscreen reset out gpio */
		error = gpio_request(pdata->gpio_reset, "hmx_reset_gpio");

		if (error) {
			E("unable to request gpio [%d]\n", pdata->gpio_reset);
			goto err_gpio_reset_req;
		}

		error = gpio_direction_output(pdata->gpio_reset, 0);

		if (error) {
			E("unable to set direction for gpio [%d]\n",
			  pdata->gpio_reset);
			goto err_gpio_reset_dir;
		}
	}

#endif
#if defined(HX_PON_PIN_SUPPORT)
	if (pdata->lcm_rst >= 0) {
		error = gpio_direction_output(pdata->lcm_rst, 0);

		if (error) {
			E("unable to set direction for lcm_rst [%d]\n",
				pdata->lcm_rst);
		}
	}
#endif
	error = himax_power_on(pdata, true);

	if (error) {
		E("Failed to power on hardware\n");
		goto err_power_on;
	}

	if (gpio_is_valid(pdata->gpio_irq)) {
		/* configure touchscreen irq gpio */
		error = gpio_request(pdata->gpio_irq, "hmx_gpio_irq");

		if (error) {
			E("unable to request gpio [%d]\n",
			  pdata->gpio_irq);
			goto err_req_irq_gpio;
		}

		error = gpio_direction_input(pdata->gpio_irq);

		if (error) {
			E("unable to set direction for gpio [%d]\n",
			  pdata->gpio_irq);
			goto err_set_gpio_irq;
		}

		client->irq = gpio_to_irq(pdata->gpio_irq);
		private_ts->hx_irq = client->irq;
	} else {
		E("irq gpio not provided\n");
		goto err_req_irq_gpio;
	}
#if defined(HX_PON_PIN_SUPPORT)
	if (pdata->lcm_rst >= 0) {
		error = gpio_direction_output(pdata->lcm_rst, 1);

		if (error) {
			E("lcm_rst unable to set direction for gpio [%d]\n",
				pdata->lcm_rst);
		}
	}
#endif
	/*msleep(20);*/
	usleep_range(2000, 2001);
#if defined(HX_RST_PIN_FUNC)

	if (gpio_is_valid(pdata->gpio_reset)) {
		error = gpio_direction_output(pdata->gpio_reset, 1);

		if (error) {
			E("unable to set direction for gpio [%d]\n",
			  pdata->gpio_reset);
			goto err_gpio_reset_set_high;
		}
	}

#endif
	return 0;
#if defined(HX_RST_PIN_FUNC)
err_gpio_reset_set_high:
#endif
err_set_gpio_irq:
	if (gpio_is_valid(pdata->gpio_irq))
		gpio_free(pdata->gpio_irq);

err_req_irq_gpio:
	himax_power_on(pdata, false);
err_power_on:
#if defined(HX_RST_PIN_FUNC)
err_gpio_reset_dir:
	if (gpio_is_valid(pdata->gpio_reset))
		gpio_free(pdata->gpio_reset);

err_gpio_reset_req:
#endif
	himax_regulator_deinit(pdata);
err_regulator_not_on:
	return error;
}

#else
int himax_gpio_power_config(struct himax_platform_data *pdata)
{
	int error = 0;
	struct i2c_client *client = private_ts->client;
#if defined(HX_RST_PIN_FUNC)

	if (pdata->gpio_reset >= 0) {
		error = gpio_request(pdata->gpio_reset, "himax-reset");

		if (error < 0) {
			E("%s: request reset pin failed\n", __func__);
			goto err_gpio_reset_req;
		}

		error = gpio_direction_output(pdata->gpio_reset, 0);

		if (error) {
			E("unable to set direction for gpio [%d]\n",
			  pdata->gpio_reset);
			goto err_gpio_reset_dir;
		}
	}

#endif

#if defined(HX_PON_PIN_SUPPORT)
	if (pdata->lcm_rst >= 0) {
		error = gpio_direction_output(pdata->lcm_rst, 0);
		if (error) {
			E("unable to set direction for lcm_rst [%d]\n",
				pdata->lcm_rst);
		}
	}

	if (gpio_is_valid(pdata->gpio_pon)) {
		error = gpio_request(pdata->gpio_pon, "hmx_pon_gpio");

		if (error) {
			E("unable to request pon gpio [%d]\n", pdata->gpio_pon);
			goto err_gpio_pon_req;
		}

		error = gpio_direction_output(pdata->gpio_pon, 0);

		I("gpio_pon LOW [%d]\n", pdata->gpio_pon);

		if (error) {
			E("unable to set direction for pon gpio [%d]\n",
				pdata->gpio_pon);
			goto err_gpio_pon_dir;
		}
	}
#endif


	if (pdata->gpio_3v3_en >= 0) {
		error = gpio_request(pdata->gpio_3v3_en, "himax-3v3_en");

		if (error < 0) {
			E("%s: request 3v3_en pin failed\n", __func__);
			goto err_gpio_3v3_req;
		}

		gpio_direction_output(pdata->gpio_3v3_en, 1);
		I("3v3_en set 1 get pin = %d\n",
			gpio_get_value(pdata->gpio_3v3_en));
	}

	if (gpio_is_valid(pdata->gpio_irq)) {
		/* configure touchscreen irq gpio */
		error = gpio_request(pdata->gpio_irq, "himax_gpio_irq");

		if (error) {
			E("unable to request gpio [%d]\n", pdata->gpio_irq);
			goto err_gpio_irq_req;
		}

		error = gpio_direction_input(pdata->gpio_irq);

		if (error) {
			E("unable to set direction for gpio [%d]\n",
				pdata->gpio_irq);
			goto err_gpio_irq_set_input;
		}

		client->irq = gpio_to_irq(pdata->gpio_irq);
		private_ts->hx_irq = client->irq;
	} else {
		E("irq gpio not provided\n");
		goto err_gpio_irq_req;
	}

	usleep_range(2000, 2001);

#if defined(HX_PON_PIN_SUPPORT)
	msleep(20);

	if (pdata->lcm_rst >= 0) {
		error = gpio_direction_output(pdata->lcm_rst, 1);

		if (error) {
			E("lcm_rst unable to set direction for gpio [%d]\n",
				pdata->lcm_rst);
		}
	}
	msleep(20);
#endif

#if defined(HX_RST_PIN_FUNC)

	if (pdata->gpio_reset >= 0) {
		error = gpio_direction_output(pdata->gpio_reset, 1);

		if (error) {
			E("unable to set direction for gpio [%d]\n",
			  pdata->gpio_reset);
			goto err_gpio_reset_set_high;
		}
	}
#endif

#if defined(HX_PON_PIN_SUPPORT)
	msleep(800);

	if (gpio_is_valid(pdata->gpio_pon)) {

		error = gpio_direction_output(pdata->gpio_pon, 1);

		I("gpio_pon HIGH [%d]\n", pdata->gpio_pon);

		if (error) {
			E("gpio_pon unable to set direction for gpio [%d]\n",
					pdata->gpio_pon);
			goto err_gpio_pon_set_high;
		}
	}
#endif
	return error;

#if defined(HX_PON_PIN_SUPPORT)
err_gpio_pon_set_high:
#endif
#if defined(HX_RST_PIN_FUNC)
err_gpio_reset_set_high:
#endif
err_gpio_irq_set_input:
	if (gpio_is_valid(pdata->gpio_irq))
		gpio_free(pdata->gpio_irq);
err_gpio_irq_req:
	if (pdata->gpio_3v3_en >= 0)
		gpio_free(pdata->gpio_3v3_en);
err_gpio_3v3_req:
#if defined(HX_PON_PIN_SUPPORT)
err_gpio_pon_dir:
	if (gpio_is_valid(pdata->gpio_pon))
		gpio_free(pdata->gpio_pon);
err_gpio_pon_req:
#endif
#if defined(HX_RST_PIN_FUNC)
err_gpio_reset_dir:
	if (pdata->gpio_reset >= 0)
		gpio_free(pdata->gpio_reset);
err_gpio_reset_req:
#endif
	return error;
}

#endif

void himax_gpio_power_deconfig(struct himax_platform_data *pdata)
{
	if (gpio_is_valid(pdata->gpio_irq))
		gpio_free(pdata->gpio_irq);

#if defined(HX_RST_PIN_FUNC)
	if (gpio_is_valid(pdata->gpio_reset))
		gpio_free(pdata->gpio_reset);
#endif

#if defined(CONFIG_HMX_DB)
	himax_power_on(pdata, false);
	himax_regulator_deinit(pdata);
#else
	if (pdata->gpio_3v3_en >= 0)
		gpio_free(pdata->gpio_3v3_en);

#if defined(HX_PON_PIN_SUPPORT)
	if (gpio_is_valid(pdata->gpio_pon))
		gpio_free(pdata->gpio_pon);
#endif

#endif
}

static void himax_ts_isr_func(struct himax_ts_data *ts)
{
	himax_ts_work(ts);
}

irqreturn_t himax_ts_thread(int irq, void *ptr)
{
	himax_ts_isr_func((struct himax_ts_data *)ptr);

	return IRQ_HANDLED;
}

static void himax_ts_work_func(struct work_struct *work)
{
	struct himax_ts_data *ts = container_of(work,
		struct himax_ts_data, work);


	himax_ts_work(ts);
}

int himax_int_register_trigger(void)
{
	int ret = 0;
	struct himax_ts_data *ts = private_ts;
	struct i2c_client *client = private_ts->client;

	if (ic_data->HX_INT_IS_EDGE) {
		I("%s edge triiger falling\n ", __func__);
		ret = request_threaded_irq(client->irq, NULL, himax_ts_thread,
			IRQF_TRIGGER_FALLING | IRQF_ONESHOT,
			client->name, ts);
	}

	else {
		I("%s level trigger low\n ", __func__);
		ret = request_threaded_irq(client->irq, NULL, himax_ts_thread,
			IRQF_TRIGGER_LOW | IRQF_ONESHOT, client->name, ts);
	}

	return ret;
}

int himax_int_en_set(void)
{
	int ret = NO_ERR;


	ret = himax_int_register_trigger();
	return ret;
}

int himax_ts_register_interrupt(void)
{
	struct himax_ts_data *ts = private_ts;
	struct i2c_client *client = private_ts->client;
	int ret = 0;


	ts->irq_enabled = 0;

	/* Work functon */
	if (client->irq && private_ts->hx_irq) {/*INT mode*/
		ts->use_irq = 1;
		ret = himax_int_register_trigger();

		if (ret == 0) {
			ts->irq_enabled = 1;
			atomic_set(&ts->irq_state, 1);
			I("%s: irq enabled at gpio: %d\n", __func__,
				client->irq);
#if defined(HX_SMART_WAKEUP)
			irq_set_irq_wake(client->irq, 1);
#endif
		} else {
			ts->use_irq = 0;
			E("%s: request_irq failed\n", __func__);
		}
	} else {
		I("%s: client->irq is empty, use polling mode.\n", __func__);
	}

	/*if use polling mode need to disable HX_ESD_RECOVERY function*/
	if (!ts->use_irq) {
		ts->himax_wq = create_singlethread_workqueue("himax_touch");
		INIT_WORK(&ts->work, himax_ts_work_func);
		hrtimer_init(&ts->timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
		ts->timer.function = himax_ts_timer_func;
		hrtimer_start(&ts->timer, ktime_set(1, 0), HRTIMER_MODE_REL);
		I("%s: polling mode enabled\n", __func__);
	}

	return ret;
}

int himax_ts_unregister_interrupt(void)
{
	struct himax_ts_data *ts = private_ts;
	int ret = 0;


	I("%s: entered.\n", __func__);

	/* Work functon */
	if (private_ts->hx_irq && ts->use_irq) {/*INT mode*/
#if defined(HX_SMART_WAKEUP)
		irq_set_irq_wake(ts->hx_irq, 0);
#endif
		free_irq(ts->hx_irq, ts);
		I("%s: irq disabled at qpio: %d\n", __func__,
			private_ts->hx_irq);
	}

	/*if use polling mode need to disable HX_ESD_RECOVERY function*/
	if (!ts->use_irq) {
		hrtimer_cancel(&ts->timer);
		cancel_work_sync(&ts->work);
		if (ts->himax_wq != NULL)
			destroy_workqueue(ts->himax_wq);
		I("%s: polling mode destroyed", __func__);
	}

	return ret;
}

static int himax_common_suspend(struct device *dev)
{
	struct himax_ts_data *ts = dev_get_drvdata(dev);

	I("%s: enter\n", __func__);
	if (!ts->initialized) {
		E("%s: init not ready, skip!\n", __func__);
		return -ECANCELED;
	}
	himax_chip_common_suspend(ts);
	return 0;
}

static int himax_common_resume(struct device *dev)
{
	struct himax_ts_data *ts = dev_get_drvdata(dev);

	I("%s: enter %s\n", __func__,ts->initialized);
	/*
	 *	wait until device resume for TDDI
	 *	TDDI: Touch and display Driver IC
	 */
	if (!ts->initialized){
#if defined(HX_CONFIG_DRM) && !defined(HX_CONFIG_FB)
		if (himax_chip_common_init())
			return -ECANCELED;
#else
		E("%s: init not ready, skip!\n", __func__);
		return -ECANCELED;
#endif
	}
	himax_chip_common_resume(ts);
	return 0;
}


#if defined(HX_CONFIG_DRM_MTK)
/**
 * himax_ts_disp_notifier_callback - mtk display notifier callback
 * Called by kernel during framebuffer blanck/unblank phrase
 */
int himax_disp_notifier_callback(struct notifier_block *nb,
	unsigned long value, void *v)
{
	struct himax_ts_data *ts =
		container_of(nb, struct himax_ts_data, fb_notif);
	int *data = (int *)v;

	if (ts && v) {
		if (value == MTK_DISP_EARLY_EVENT_BLANK) {
			if (*data == MTK_DISP_BLANK_POWERDOWN)
				himax_common_suspend(ts->dev);
		} else if (value == MTK_DISP_EVENT_BLANK) {
			if (*data == MTK_DISP_BLANK_UNBLANK)
				himax_common_resume(ts->dev);
		}
	} else {
		I("HIMAX touch IC can not suspend or resume");
		return -ECANCELED;
	}
	return 0;
}

static void himax_fb_register(struct work_struct *work)
{
	int ret = 0;

	struct himax_ts_data *ts = container_of(work, struct himax_ts_data,
			work_att.work);

	I("%s in\n", __func__);
#if defined(HX_CONFIG_DRM_MTK)
	ts->fb_notif.notifier_call = himax_disp_notifier_callback;
	ret = mtk_disp_notifier_register("HIMAX Touch", &ts->fb_notif);
	if (ret)
		E("Failed to register disp notifier client:%d\n", ret);
#endif
	if (ret)
		E("Unable to register fb_notifier: %d\n", ret);
}
#endif

int himax_chip_common_probe(struct i2c_client *client,
		const struct i2c_device_id *id)
{
	int ret = 0;
	struct himax_ts_data *ts;

	I("%s:[i2c!~]Enter\n", __func__);

	gp_rw_buf = kcalloc(BUS_RW_MAX_LEN, sizeof(uint8_t), GFP_KERNEL);
	if (!gp_rw_buf) {
		E("Allocate I2C RW Buffer failed\n");
		ret = -ENODEV;
		goto err_alloc_rw_buf_failed;
	}

	/* Check I2C functionality */
	if (!i2c_check_functionality(client->adapter, I2C_FUNC_I2C)) {
		E("%s: i2c check functionality error\n", __func__);
		return -ENODEV;
	}

	ts = kzalloc(sizeof(struct himax_ts_data), GFP_KERNEL);
	if (ts == NULL) {
		E("%s: allocate himax_ts_data failed\n", __func__);
		ret = -ENOMEM;
		goto err_alloc_data_failed;
	}

	i2c_set_clientdata(client, ts);
	ts->client = client;
	ts->dev = &client->dev;
	mutex_init(&ts->rw_lock);
	mutex_init(&ts->reg_lock);
	private_ts = ts;

	ts->probe_finish = false;
	ts->initialized = false;
	ret = himax_chip_common_init();
	if (ret < 0)
		goto err_common_init_failed;

#if defined(HX_CONFIG_DRM_MTK)
	ts->himax_att_wq = create_singlethread_workqueue("HMX_ATT_request");
	if (!ts->himax_att_wq) {
		E(" allocate himax_att_wq failed\n");
		ret = -ENOMEM;
		goto err_get_intr_bit_failed;
	}

	INIT_DELAYED_WORK(&ts->work_att, himax_fb_register);
	queue_delayed_work(ts->himax_att_wq, &ts->work_att,
			msecs_to_jiffies(0));
#endif

	ts->probe_finish = true;

	return ret;

#if defined(HX_CONFIG_DRM_MTK)
	cancel_delayed_work_sync(&ts->work_att);
	destroy_workqueue(ts->himax_att_wq);
err_get_intr_bit_failed:
#endif
err_common_init_failed:
	kfree(ts);
err_alloc_data_failed:
	kfree(gp_rw_buf);
err_alloc_rw_buf_failed:

	return ret;
}

int himax_chip_common_remove(struct i2c_client *client)
{
	struct himax_ts_data *ts = private_ts;

	if (ts->probe_finish) {
#if defined(HX_CONFIG_DRM_MTK)
		if (mtk_disp_notifier_unregister(&ts->fb_notif))
			E("Error occurred while unregistering notifier.\n");
#endif
		himax_chip_common_deinit();
	}
	kfree(gp_rw_buf);

	return 0;
}

static const struct i2c_device_id himax_common_ts_id[] = {
	{HIMAX_common_NAME, 0 },
	{}
};
MODULE_DEVICE_TABLE(i2c, himax_common_ts_id);

static const struct dev_pm_ops himax_common_pm_ops = {
#if (!defined(HX_CONFIG_FB)) && (!defined(HX_CONFIG_DRM))
	.suspend = himax_common_suspend,
	.resume  = himax_common_resume,
#endif
};

#if defined(CONFIG_OF)
static const struct of_device_id himax_match_table[] = {
	{.compatible = "himax,hxcommon" },
	{},
};
MODULE_DEVICE_TABLE(of, himax_match_table);
#else
#define himax_match_table NULL
#endif

#ifdef CONFIG_ACPI
static const struct acpi_device_id acpi_matches[] = {
	{.id = "PNPxxx"},
	{},
};
MODULE_DEVICE_TABLE(acpi, acpi_matches);
#endif

static struct i2c_driver himax_common_driver = {
	.id_table	= himax_common_ts_id,
	.probe		= himax_chip_common_probe,
	.remove		= himax_chip_common_remove,
	.driver		= {
		.name = HIMAX_common_NAME,
		.owner = THIS_MODULE,
		.of_match_table = of_match_ptr(himax_match_table),
#if defined(CONFIG_PM)
		.pm				= &himax_common_pm_ops,
#endif
#ifdef CONFIG_ACPI
		.acpi_match_table = acpi_matches,
#endif
	},
};

static int __init himax_common_init(void)
{
	I("Himax common touch panel driver init\n");
	D("Himax check double loading\n");
	if (g_mmi_refcnt++ > 0) {

		I("Himax driver has been loaded! ignoring....\n");

		return 0;
	}

	return i2c_add_driver(&himax_common_driver);
}

static void __exit himax_common_exit(void)
{
	i2c_del_driver(&himax_common_driver);
}

#if defined(__HIMAX_MOD__)
module_init(himax_common_init);
#else
late_initcall(himax_common_init);
#endif
module_exit(himax_common_exit);

MODULE_DESCRIPTION("Himax_common driver");
MODULE_LICENSE("GPL");
