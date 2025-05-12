/*
 * Copyright (c) 2021 MediaTek Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files
 * (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <pal_log.h>
#include <regulator/mtk_regulator_errno.h>
#include <pmic_auxadc.h>
#include <timer.h>
#include <string.h>

#ifndef BIT
#define BIT(nr) (1 << (nr))
#endif

#define AUXADC_SLOT_MAX		5
#define AUXADC_RES_MAX		31

struct auxadc {
	const struct auxadc_device *dev;
	u32 support_chans;
};

static struct auxadc auxadc_tbl[AUXADC_SLOT_MAX];

static struct auxadc *auxadc_register(const struct auxadc_device *auxadc_dev)
{
	int i;
	struct auxadc *auxadc = NULL;
	const struct auxadc_chan_spec *chan;

	for (i = 0; i < AUXADC_SLOT_MAX; i++) {
		if (auxadc_tbl[i].dev &&
		    !strcmp(auxadc_tbl[i].dev->name, auxadc_dev->name)) {
			pal_log_err("auxadc %s already exists\n",
				    auxadc_tbl[i].dev->name);
			return NULL;
		} else if (!auxadc && !auxadc_tbl[i].dev)
			auxadc = &auxadc_tbl[i];
	}
	if (auxadc) {
		auxadc->dev = auxadc_dev;
		for (i = 0; i < auxadc_dev->num_chans; i++) {
			chan = &auxadc_dev->chans[i];
			auxadc->support_chans |= BIT(chan->channel);
		}
	}
	return auxadc;
}

static int auxadc_read(const struct auxadc_device *auxadc_dev,
		       const struct auxadc_chan_spec *chan, int *val,
		       enum auxadc_val_type type)
{
	int ret, len = (chan->hw_info.res + 7) / 8;
	u32 regval;
	u32 elapsed = 0;

	if (chan->hw_info.res > AUXADC_RES_MAX)
		return -EINVAL;

	ret = auxadc_dev->write_regs(auxadc_dev, chan->hw_info.enable_reg,
				     (u8 *)&chan->hw_info.enable_mask, 1);
	if (ret < 0) {
		pal_log_err("%s %s chan=%d write enable fail\n", __func__,
			    auxadc_dev->name, chan->channel);
		return ret;
	}
	udelay(chan->hw_info.min_time);
	elapsed += chan->hw_info.min_time;

	while (1) {
		ret = auxadc_dev->read_regs(auxadc_dev, chan->hw_info.ready_reg,
					    (u8 *)&regval, 1);
		if (ret < 0) {
			pal_log_err("%s %s chan=%d read ready fail\n", __func__,
				    auxadc_dev->name, chan->channel);
			goto retry;
		}
		if (regval & chan->hw_info.ready_mask)
			break;
retry:
		if (elapsed > chan->hw_info.max_time) {
			pal_log_err("%s %s chan=%d timeout\n", __func__,
				    auxadc_dev->name, chan->channel);
			return ret < 0 ? ret : -ETIME;
		}
		udelay(chan->hw_info.poll_time);
		elapsed += chan->hw_info.poll_time;
	}

	ret = auxadc_dev->read_regs(auxadc_dev, chan->hw_info.value_reg,
				    (u8 *)&regval, len);
	if (ret < 0) {
		pal_log_err("%s %s chan=%d read value fail\n", __func__,
			    auxadc_dev->name, chan->channel);
		return ret;
	}
	regval &= (BIT(chan->hw_info.res) - 1);
	switch (type) {
		case AUXADC_VAL_RAW:
			*val = regval;
			break;
		case AUXADC_VAL_PROCESSED:
			*val = ((regval * chan->hw_info.ratio[0] *
				chan->hw_info.ref_volt) /
				chan->hw_info.ratio[1]) >> chan->hw_info.res;
			break;
	}
	pal_log_err("%s %s val = %d\n", __func__, auxadc_dev->name, *val);
	return 0;
}

int pmic_auxadc_read_value(const struct auxadc *auxadc, int channel, int *val,
			   enum auxadc_val_type type)
{
	int i;
	const struct auxadc_chan_spec *chan = NULL;

	if (!auxadc || !auxadc->dev)
		return -EINVAL;

	for (i = 0; i < auxadc->dev->num_chans; i++) {
		if (auxadc->dev->chans[i].channel == channel) {
			chan = &auxadc->dev->chans[i];
			break;
		}
	}
	if (!chan)
		return -EINVAL;
	return (auxadc->dev->read ? auxadc->dev->read : auxadc_read)
		(auxadc->dev, chan, val, type);
}

int pmic_auxadc_read_value_ext(int channel, int *val, enum auxadc_val_type type)
{
	int i;
	struct auxadc *auxadc;

	if (channel < 0 || channel >= AUXADC_CHAN_MAX)
		return -EINVAL;

	for (i = 0; i < AUXADC_SLOT_MAX; i++) {
		auxadc = &auxadc_tbl[i];
		if (auxadc->dev && (auxadc->support_chans & BIT(channel)))
			break;
	}
	if (i == AUXADC_SLOT_MAX)
		return -EINVAL;
	return (auxadc->dev->read ? auxadc->dev->read : auxadc_read)
		(auxadc->dev, &auxadc->dev->chans[channel], val, type);
}

struct auxadc *pmic_auxadc_get_by_name(const char *name)
{
	int i;

	if (!name)
		return NULL;

	pal_log_err("%s name = %s\n", __func__, name);
	for (i = 0; i < AUXADC_SLOT_MAX; i++) {
		if (auxadc_tbl[i].dev && !strcmp(auxadc_tbl[i].dev->name, name))
			return &auxadc_tbl[i];
	}
	pal_log_err("%s name = %s fail\n", __func__, name);
	return NULL;
}

int pmic_auxadc_device_register(const struct auxadc_device *auxadc_dev)
{
	int i;
	struct auxadc *auxadc;
	const struct auxadc_chan_spec *chan;

	if (!auxadc_dev) {
		pal_log_err("%s auxadc_dev is null\n", __func__);
		return -EINVAL;
	}

	if (!auxadc_dev->name || !auxadc_dev->chans ||
	    auxadc_dev->num_chans < 0) {
		pal_log_err("%s no name or channel\n", __func__);
		return -EINVAL;
	}

	if ((!auxadc_dev->read_regs || !auxadc_dev->write_regs) &&
	    !auxadc_dev->read) {
		pal_log_err("%s read/write ops incorrect\n", __func__);
		return -EINVAL;
	}

	for (i = 0; i < auxadc_dev->num_chans; i++) {
		chan = &auxadc_dev->chans[i];
		if (chan->channel < 0 || chan->channel >= AUXADC_CHAN_MAX) {
			pal_log_err("%s chan incorrect\n", __func__);
			return -EINVAL;
		}
		if (!auxadc_dev->read &&
		    (chan->hw_info.ratio[1] == 0 ||
		     chan->hw_info.res > AUXADC_RES_MAX)) {
			pal_log_err("%s ratio[1] cannot be 0\n", __func__);
			return -EINVAL;
		}
	}

	auxadc = auxadc_register(auxadc_dev);
	if (!auxadc) {
		pal_log_err("%s failed to register auxadc\n", __func__);
		return -EINVAL;
	}
	pal_log_err("%s %s successfully\n", __func__, auxadc_dev->name);
	return 0;
}

int pmic_auxadc_device_unregister(struct auxadc *auxadc)
{
	if (!auxadc)
		return -EINVAL;

	auxadc->dev = NULL;
	auxadc->support_chans = 0;
	return 0;
}
