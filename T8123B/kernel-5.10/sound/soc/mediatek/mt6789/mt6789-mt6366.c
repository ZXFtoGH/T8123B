// SPDX-License-Identifier: GPL-2.0
/*
 *  mt6789-mt6358.c  --  mt6789 mt6358 ALSA SoC machine driver
 *
 *  Copyright (c) 2021 MediaTek Inc.
 *  Author: Yujie Xiao <yujie.xiao@mediatek.com>
 */

#include <linux/module.h>
#include <linux/pm_runtime.h>
#include <sound/pcm_params.h>
#include <sound/soc.h>
#include <linux/gpio.h>
#include <linux/of_gpio.h>

#include "../common/mtk-afe-platform-driver.h"
#include "mt6789-afe-common.h"
#include "mt6789-afe-clk.h"
#include "mt6789-afe-gpio.h"
#include "../../codecs/mt6358.h"

#if IS_ENABLED(CONFIG_SND_SOC_MT6366_ACCDET)
#include "../../codecs/mt6358-accdet.h"
#endif

#include "../common/mtk-sp-spk-amp.h"

/*
 * if need additional control for the ext spk amp that is connected
 * after Lineout Buffer / HP Buffer on the codec, put the control in
 * mt6789_mt6366_spk_amp_event()
 */
#define EXT_SPK_AMP_W_NAME "Ext_Speaker_Amp"

//tom add
#if IS_ENABLED(CONFIG_SND_SOC_AW87XXX)
extern unsigned int g_aw87xxx_dev_cnt;
extern int aw87xxx_set_profile(int dev_index, char *profile);
static char *aw_profile[] = {"Music", "Off"};/*aw87xxx_acf.bin 文件中配置场景*/
enum aw87xxx_dev_index {
	#if IS_ENABLED(CONFIG_SND_SOC_AW87XXX_ONE_PA)
	AW_DEV_0 = 0,	//tom add mod 双功放变更为单功放
	#else
	AW_DEV_0 = 0,
	AW_DEV_1,	
	AW_DEV_2,
	AW_DEV_3,
	#endif
};
#endif
//end

static int ext_amp_gpio;
static u32 spk_amp_mode;
//add
//GPIO_SPK_ONE    --- AF23   GPIO67     J1--U5---right below
//GPIO_SPK_TWO    --- R3     GPIO6      J12--U4--left up
//GPIO_SPK_THREE  --- AE27   GPIO106    J2--U42--left below
//GPIO_SPK_FOUR   --- AF28   GPIO107    J6--U43--right up
static int ext_amp_gpio_one;
static int ext_amp_gpio_three;
static int ext_amp_gpio_four;
static u8 test_spk_amp_mode;
//end

static const char *const mt6789_spk_type_str[] = {MTK_SPK_NOT_SMARTPA_STR,
						  MTK_SPK_RICHTEK_RT5509_STR,
						  MTK_SPK_MEDIATEK_MT6660_STR,
						  MTK_SPK_RICHTEK_RT5512_STR,
						  MTK_SPK_GOODIX_TFA98XX_STR};
static const char *const
	mt6789_spk_i2s_type_str[] = {MTK_SPK_I2S_0_STR,
				     MTK_SPK_I2S_1_STR,
				     MTK_SPK_I2S_2_STR,
				     MTK_SPK_I2S_3_STR,
				     MTK_SPK_I2S_5_STR,
					 };

static const struct soc_enum mt6789_spk_type_enum[] = {
	SOC_ENUM_SINGLE_EXT(ARRAY_SIZE(mt6789_spk_type_str),
			    mt6789_spk_type_str),
	SOC_ENUM_SINGLE_EXT(ARRAY_SIZE(mt6789_spk_i2s_type_str),
			    mt6789_spk_i2s_type_str),
};

static int mt6789_spk_type_get(struct snd_kcontrol *kcontrol,
			       struct snd_ctl_elem_value *ucontrol)
{
	int idx = mtk_spk_get_type();

	pr_debug("%s() = %d\n", __func__, idx);
	ucontrol->value.integer.value[0] = idx;
	return 0;
}

static int mt6789_spk_i2s_out_type_get(struct snd_kcontrol *kcontrol,
				       struct snd_ctl_elem_value *ucontrol)
{
	int idx = mtk_spk_get_i2s_out_type();

	pr_debug("%s() = %d\n", __func__, idx);
	ucontrol->value.integer.value[0] = idx;
	return 0;
}

static int mt6789_spk_i2s_in_type_get(struct snd_kcontrol *kcontrol,
				      struct snd_ctl_elem_value *ucontrol)
{
	int idx = mtk_spk_get_i2s_in_type();

	pr_debug("%s() = %d\n", __func__, idx);
	ucontrol->value.integer.value[0] = idx;
	return 0;
}

static void mt6789_ext_hp_amp_turn_on_or_off(int on)
{
	u32 i;
	if(test_spk_amp_mode != 0){		
		return;
	}
	/* enable spk amp */
	if (ext_amp_gpio >= 0){
		if(on){
			for (i = 0; i < spk_amp_mode; i++) {
				gpio_direction_output(ext_amp_gpio, 0);
				udelay(2);
				gpio_direction_output(ext_amp_gpio, 1);
				udelay(2);
			}
		}else
			gpio_direction_output(ext_amp_gpio, 0);
	}

	/*if (card_data->ext_spk_amp_warmup_time_us > 0)
		usleep_range(card_data->ext_spk_amp_warmup_time_us,
			card_data->ext_spk_amp_warmup_time_us + 1);
	*/
	//add 2025-01-14
	if (ext_amp_gpio_one >= 0){
		if(on){
			for (i = 0; i < spk_amp_mode; i++) {
				gpio_direction_output(ext_amp_gpio_one, 0);
				udelay(2);
				gpio_direction_output(ext_amp_gpio_one, 1);
				udelay(2);
			}
		}else{
			gpio_direction_output(ext_amp_gpio_one, 0);
		}
	}
	//
	if (ext_amp_gpio_three >= 0){
		if(on){
			for (i = 0; i < spk_amp_mode; i++) {
				gpio_direction_output(ext_amp_gpio_three, 0);
				udelay(2);
				gpio_direction_output(ext_amp_gpio_three, 1);
				udelay(2);
			}
		}else{
			gpio_direction_output(ext_amp_gpio_three, 0);
		}
	}	
	//
	if (ext_amp_gpio_four >= 0){
		if(on){
			for (i = 0; i < spk_amp_mode; i++) {
				gpio_direction_output(ext_amp_gpio_four, 0);
				udelay(2);
				gpio_direction_output(ext_amp_gpio_four, 1);
				udelay(2);
			}
		}else{
			gpio_direction_output(ext_amp_gpio_four, 0);
		}
	}	
	//end
}


static int mt6789_mt6366_spk_amp_event(struct snd_soc_dapm_widget *w,
					struct snd_kcontrol *kcontrol,
					int event)
{
	struct snd_soc_dapm_context *dapm = w->dapm;
	struct snd_soc_card *card = dapm->card;

	dev_info(card->dev, "%s(), event %d\n", __func__, event);

	switch (event) {
	case SND_SOC_DAPM_POST_PMU:
		/* spk amp on control */
		/*usleep_range(1 * 1000, 2 * 1000);
		mt6789_ext_hp_amp_turn_on_or_off(1);
		usleep_range(5 * 1000, 10 * 1000);*/		
		//tom add
		#if IS_ENABLED(CONFIG_SND_SOC_AW87XXX)
		/*切换 PA AW_DEV_0 为 Music 场景*/
		    
			aw87xxx_set_profile(AW_DEV_0, aw_profile[0]);
			if(g_aw87xxx_dev_cnt > 1)
			{
				aw87xxx_set_profile(AW_DEV_1, aw_profile[0]);
			}
			if(g_aw87xxx_dev_cnt > 2)
			{
				aw87xxx_set_profile(AW_DEV_2, aw_profile[0]);
			}
			if(g_aw87xxx_dev_cnt > 3)
			{
				aw87xxx_set_profile(AW_DEV_3, aw_profile[0]);
			}
			//printk(" tom add 001001 two pa open 0420\n");			
		#else
			usleep_range(1 * 1000, 2 * 1000);
			mt6789_ext_hp_amp_turn_on_or_off(1);
			usleep_range(5 * 1000, 10 * 1000);
		#endif
		break;
	case SND_SOC_DAPM_PRE_PMD:
		/* spk amp off control */
		//mt6789_ext_hp_amp_turn_on_or_off(0);		
		//tom add
		#if IS_ENABLED(CONFIG_SND_SOC_AW87XXX)
			aw87xxx_set_profile(AW_DEV_0, aw_profile[1]);
			if(g_aw87xxx_dev_cnt > 1)
			{
				aw87xxx_set_profile(AW_DEV_1, aw_profile[1]);
			}
			if(g_aw87xxx_dev_cnt > 2)
			{
				aw87xxx_set_profile(AW_DEV_2, aw_profile[1]);
			}
			if(g_aw87xxx_dev_cnt > 3)
			{
				aw87xxx_set_profile(AW_DEV_3, aw_profile[1]);
			}
			//printk(" tom add 002002 pa off 0420\n");			
		#else
			mt6789_ext_hp_amp_turn_on_or_off(0);
		#endif
		//end
		break;
	default:
		break;
	}

	return 0;
};

static const struct snd_soc_dapm_widget mt6789_mt6366_widgets[] = {
	SND_SOC_DAPM_SPK(EXT_SPK_AMP_W_NAME, mt6789_mt6366_spk_amp_event),
};

static const struct snd_soc_dapm_route mt6789_mt6366_routes[] = {
	{EXT_SPK_AMP_W_NAME, NULL, "LINEOUT L"},
	{EXT_SPK_AMP_W_NAME, NULL, "Headphone L Ext Spk Amp"},
	{EXT_SPK_AMP_W_NAME, NULL, "Headphone R Ext Spk Amp"},
};

static const struct snd_kcontrol_new mt6789_mt6366_controls[] = {
	SOC_DAPM_PIN_SWITCH(EXT_SPK_AMP_W_NAME),
	SOC_ENUM_EXT("MTK_SPK_TYPE_GET", mt6789_spk_type_enum[0],
		     mt6789_spk_type_get, NULL),
	SOC_ENUM_EXT("MTK_SPK_I2S_OUT_TYPE_GET", mt6789_spk_type_enum[1],
		     mt6789_spk_i2s_out_type_get, NULL),
	SOC_ENUM_EXT("MTK_SPK_I2S_IN_TYPE_GET", mt6789_spk_type_enum[1],
		     mt6789_spk_i2s_in_type_get, NULL),
};

/*
 * define mtk_spk_i2s_mck node in dts when need mclk,
 * BE i2s need assign snd_soc_ops = mt6789_mt6366_i2s_ops
 */
static int mt6789_mt6366_i2s_hw_params(struct snd_pcm_substream *substream,
					struct snd_pcm_hw_params *params)
{
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	unsigned int rate = params_rate(params);
	unsigned int mclk_fs_ratio = 128;
	unsigned int mclk_fs = rate * mclk_fs_ratio;
	struct snd_soc_dai *cpu_dai = asoc_rtd_to_cpu(rtd, 0);

	return snd_soc_dai_set_sysclk(cpu_dai,
				      0, mclk_fs, SND_SOC_CLOCK_OUT);
}

static const struct snd_soc_ops mt6789_mt6366_i2s_ops = {
	.hw_params = mt6789_mt6366_i2s_hw_params,
};

static int mt6789_mt6366_mtkaif_calibration(struct snd_soc_pcm_runtime *rtd)
{
#if !defined(CONFIG_FPGA_EARLY_PORTING) && !defined(SKIP_SB)
	struct snd_soc_component *component =
		snd_soc_rtdcom_lookup(rtd, AFE_PCM_NAME);
	struct mtk_base_afe *afe = snd_soc_component_get_drvdata(component);
	struct mt6789_afe_private *afe_priv = afe->platform_priv;
	struct snd_soc_component *codec_component =
		snd_soc_rtdcom_lookup(rtd, CODEC_MT6358_NAME);
	int phase;
	unsigned int monitor = 0;
	int test_done_1, test_done_2;
	int miso0_need_calib, miso1_need_calib;
	int cycle_1, cycle_2;
	int prev_cycle_1, prev_cycle_2;
	int counter;
	int mtkaif_calib_ok;

	dev_info(afe->dev, "%s(), start\n", __func__);

	pm_runtime_get_sync(afe->dev);

	miso0_need_calib = mt6789_afe_gpio_is_prepared(MT6789_AFE_GPIO_DAT_MISO0_ON);
	miso1_need_calib = mt6789_afe_gpio_is_prepared(MT6789_AFE_GPIO_DAT_MISO1_ON);

	mt6789_afe_gpio_request(afe, true, MT6789_DAI_ADDA, 1);
	mt6789_afe_gpio_request(afe, true, MT6789_DAI_ADDA, 0);

	mt6358_mtkaif_calibration_enable(codec_component);

	/* set clock protocol 2 */
	regmap_update_bits(afe->regmap, AFE_AUD_PAD_TOP, 0xff, 0x38);
	regmap_update_bits(afe->regmap, AFE_AUD_PAD_TOP, 0xff, 0x39);

	/* set test type to synchronizer pulse */
	regmap_update_bits(afe_priv->topckgen,
			   CKSYS_AUD_TOP_CFG, 0xffff, 0x4);

	mtkaif_calib_ok = true;
	afe_priv->mtkaif_calibration_num_phase = 42;	/* mt6358: 0 ~ 42 */
	afe_priv->mtkaif_chosen_phase[0] = -1;
	afe_priv->mtkaif_chosen_phase[1] = -1;

	for (phase = 0;
	     phase <= afe_priv->mtkaif_calibration_num_phase &&
	     mtkaif_calib_ok;
	     phase++) {
		mt6358_set_mtkaif_calibration_phase(codec_component,
						    phase, phase);

		regmap_update_bits(afe_priv->topckgen,
				   CKSYS_AUD_TOP_CFG, 0x1, 0x1);

		test_done_1 = miso0_need_calib ? 0 : -1;
		test_done_2 = miso1_need_calib ? 0 : -1;
		cycle_1 = -1;
		cycle_2 = -1;
		counter = 0;
		while (test_done_1 == 0 || test_done_2 == 0) {
			regmap_read(afe_priv->topckgen,
				    CKSYS_AUD_TOP_MON, &monitor);

			/* get test status */
			if (test_done_1 == 0)
				test_done_1 = (monitor >> 28) & 0x1;
			if (test_done_2 == 0)
				test_done_2 = (monitor >> 29) & 0x1;

			/* get delay cycle */
			if (test_done_1 == 1)
				cycle_1 = monitor & 0xf;
			if (test_done_2 == 1)
				cycle_2 = (monitor >> 4) & 0xf;

			/* handle if never test done */
			if (++counter > 10000) {
				dev_err(afe->dev, "%s(), test fail, cycle_1 %d, cycle_2 %d, monitor 0x%x\n",
					__func__,
					cycle_1, cycle_2, monitor);
				mtkaif_calib_ok = false;
				break;
			}
		}

		if (phase == 0) {
			prev_cycle_1 = cycle_1;
			prev_cycle_2 = cycle_2;
		}

		if (miso0_need_calib &&
		    cycle_1 != prev_cycle_1 &&
		    afe_priv->mtkaif_chosen_phase[0] < 0) {
			afe_priv->mtkaif_chosen_phase[0] = phase - 1;
			afe_priv->mtkaif_phase_cycle[0] = prev_cycle_1;
		}

		if (miso1_need_calib &&
		    cycle_2 != prev_cycle_2 &&
		    afe_priv->mtkaif_chosen_phase[1] < 0) {
			afe_priv->mtkaif_chosen_phase[1] = phase - 1;
			afe_priv->mtkaif_phase_cycle[1] = prev_cycle_2;
		}

		regmap_update_bits(afe_priv->topckgen,
				   CKSYS_AUD_TOP_CFG, 0x1, 0x0);
	}

	mt6358_set_mtkaif_calibration_phase(codec_component,
		(afe_priv->mtkaif_chosen_phase[0] < 0) ?
		0 : afe_priv->mtkaif_chosen_phase[0],
		(afe_priv->mtkaif_chosen_phase[1] < 0) ?
		0 : afe_priv->mtkaif_chosen_phase[1]);

	/* disable rx fifo */
	regmap_update_bits(afe->regmap, AFE_AUD_PAD_TOP, 0xff, 0x38);

	mt6358_mtkaif_calibration_disable(codec_component);

	mt6789_afe_gpio_request(afe, false, MT6789_DAI_ADDA, 1);
	mt6789_afe_gpio_request(afe, false, MT6789_DAI_ADDA, 0);

	/* disable syncword if miso pin not prepared */
	if (!miso0_need_calib)
		regmap_update_bits(afe->regmap, AFE_ADDA_MTKAIF_SYNCWORD_CFG,
				   RG_ADDA_MTKAIF_RX_SYNC_WORD1_DISABLE_MASK_SFT,
				   0x1 << RG_ADDA_MTKAIF_RX_SYNC_WORD1_DISABLE_SFT);
	if (!miso1_need_calib)
		regmap_update_bits(afe->regmap, AFE_ADDA_MTKAIF_SYNCWORD_CFG,
				   RG_ADDA_MTKAIF_RX_SYNC_WORD2_DISABLE_MASK_SFT,
				   0x1 << RG_ADDA_MTKAIF_RX_SYNC_WORD2_DISABLE_SFT);

	pm_runtime_put(afe->dev);

	dev_info(afe->dev, "%s(), mtkaif_chosen_phase[0/1]:%d/%d, miso_need_calib[%d/%d]\n",
		 __func__,
		 afe_priv->mtkaif_chosen_phase[0],
		 afe_priv->mtkaif_chosen_phase[1],
		 miso0_need_calib, miso1_need_calib);
#endif
	return 0;
}

static int mt6789_mt6366_init(struct snd_soc_pcm_runtime *rtd)
{
	struct snd_soc_component *component =
		snd_soc_rtdcom_lookup(rtd, AFE_PCM_NAME);
	struct mtk_base_afe *afe = snd_soc_component_get_drvdata(component);
	struct mt6789_afe_private *afe_priv = afe->platform_priv;
	struct snd_soc_component *codec_component =
		snd_soc_rtdcom_lookup(rtd, CODEC_MT6358_NAME);
	struct snd_soc_dapm_context *dapm = &rtd->card->dapm;
	struct mt6358_codec_ops ops;

	/* set dc component callback function for codec */
	ops.enable_dc_compensation = mt6789_enable_dc_compensation;
	ops.set_lch_dc_compensation = mt6789_set_lch_dc_compensation;
	ops.set_rch_dc_compensation = mt6789_set_rch_dc_compensation;
	ops.adda_dl_gain_control = mt6789_adda_dl_gain_control;
	mt6358_set_codec_ops(codec_component, &ops);

	/* set mtkaif protocol */
	mt6358_set_mtkaif_protocol(codec_component,
				   MT6358_MTKAIF_PROTOCOL_1);
	afe_priv->mtkaif_protocol = MT6358_MTKAIF_PROTOCOL_1;

	/* mtkaif calibration */
	if (afe_priv->mtkaif_protocol == MTKAIF_PROTOCOL_2_CLK_P2)
		mt6789_mt6366_mtkaif_calibration(rtd);

	/* disable ext amp connection */
	snd_soc_dapm_disable_pin(dapm, EXT_SPK_AMP_W_NAME);
#if IS_ENABLED(CONFIG_SND_SOC_MT6366_ACCDET)
	mt6358_accdet_init(codec_component, rtd->card);
#endif
	return 0;
}

static int mt6789_i2s_hw_params_fixup(struct snd_soc_pcm_runtime *rtd,
				      struct snd_pcm_hw_params *params)
{
	dev_info(rtd->dev, "%s(), fix format to 32bit\n", __func__);

	/* fix BE i2s format to 32bit, clean param mask first */
	snd_mask_reset_range(hw_param_mask(params, SNDRV_PCM_HW_PARAM_FORMAT),
			     0, SNDRV_PCM_FORMAT_LAST);

	params_set_format(params, SNDRV_PCM_FORMAT_S32_LE);
	return 0;
}

#if IS_ENABLED(CONFIG_MTK_VOW_SUPPORT) && !defined(CONFIG_FPGA_EARLY_PORTING)
#if !defined(SKIP_SB_VOW)
static const struct snd_pcm_hardware mt6789_mt6366_vow_hardware = {
	.info = (SNDRV_PCM_INFO_MMAP | SNDRV_PCM_INFO_INTERLEAVED |
		 SNDRV_PCM_INFO_MMAP_VALID),
	.period_bytes_min = 256,
	.period_bytes_max = 2 * 1024,
	.periods_min = 2,
	.periods_max = 4,
	.buffer_bytes_max = 2 * 2 * 1024,
};

static int mt6789_mt6366_vow_startup(struct snd_pcm_substream *substream)
{
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_component *component =
			snd_soc_rtdcom_lookup(rtd, AFE_PCM_NAME);
	struct mtk_base_afe *afe = snd_soc_component_get_drvdata(component);
	int i;

	dev_info(afe->dev, "%s(), start\n", __func__);
	snd_soc_set_runtime_hwparams(substream, &mt6789_mt6366_vow_hardware);

	mt6789_afe_gpio_request(afe, true, MT6789_DAI_VOW, 0);

	/* ASoC will call pm_runtime_get, but vow don't need */
	for_each_rtd_components(rtd, i, component) {
		pm_runtime_put_autosuspend(component->dev);
	}

	return 0;
}

static void mt6789_mt6366_vow_shutdown(struct snd_pcm_substream *substream)
{
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_component *component =
			snd_soc_rtdcom_lookup(rtd, AFE_PCM_NAME);
	struct mtk_base_afe *afe = snd_soc_component_get_drvdata(component);
	int i;

	dev_info(afe->dev, "%s(), end\n", __func__);
	mt6789_afe_gpio_request(afe, false, MT6789_DAI_VOW, 0);

	/* restore to fool ASoC */
	for_each_rtd_components(rtd, i, component) {
		pm_runtime_get_sync(component->dev);
	}
}

static const struct snd_soc_ops mt6789_mt6366_vow_ops = {
	.startup = mt6789_mt6366_vow_startup,
	.shutdown = mt6789_mt6366_vow_shutdown,
};
#endif  // #if IS_ENABLED(CONFIG_MTK_VOW_SUPPORT)
#endif

/* FE */
SND_SOC_DAILINK_DEFS(playback1,
	DAILINK_COMP_ARRAY(COMP_CPU("DL1")),
	DAILINK_COMP_ARRAY(COMP_DUMMY()),
	DAILINK_COMP_ARRAY(COMP_EMPTY()));
SND_SOC_DAILINK_DEFS(playback12,
	DAILINK_COMP_ARRAY(COMP_CPU("DL12")),
	DAILINK_COMP_ARRAY(COMP_DUMMY()),
	DAILINK_COMP_ARRAY(COMP_EMPTY()));
SND_SOC_DAILINK_DEFS(playback2,
	DAILINK_COMP_ARRAY(COMP_CPU("DL2")),
	DAILINK_COMP_ARRAY(COMP_DUMMY()),
	DAILINK_COMP_ARRAY(COMP_EMPTY()));
SND_SOC_DAILINK_DEFS(playback3,
	DAILINK_COMP_ARRAY(COMP_CPU("DL3")),
	DAILINK_COMP_ARRAY(COMP_DUMMY()),
	DAILINK_COMP_ARRAY(COMP_EMPTY()));
SND_SOC_DAILINK_DEFS(playback4,
	DAILINK_COMP_ARRAY(COMP_CPU("DL4")),
	DAILINK_COMP_ARRAY(COMP_DUMMY()),
	DAILINK_COMP_ARRAY(COMP_EMPTY()));
SND_SOC_DAILINK_DEFS(playback5,
	DAILINK_COMP_ARRAY(COMP_CPU("DL5")),
	DAILINK_COMP_ARRAY(COMP_DUMMY()),
	DAILINK_COMP_ARRAY(COMP_EMPTY()));
SND_SOC_DAILINK_DEFS(playback6,
	DAILINK_COMP_ARRAY(COMP_CPU("DL6")),
	DAILINK_COMP_ARRAY(COMP_DUMMY()),
	DAILINK_COMP_ARRAY(COMP_EMPTY()));
SND_SOC_DAILINK_DEFS(playback7,
	DAILINK_COMP_ARRAY(COMP_CPU("DL7")),
	DAILINK_COMP_ARRAY(COMP_DUMMY()),
	DAILINK_COMP_ARRAY(COMP_EMPTY()));
SND_SOC_DAILINK_DEFS(playback8,
	DAILINK_COMP_ARRAY(COMP_CPU("DL8")),
	DAILINK_COMP_ARRAY(COMP_DUMMY()),
	DAILINK_COMP_ARRAY(COMP_EMPTY()));
SND_SOC_DAILINK_DEFS(capture1,
	DAILINK_COMP_ARRAY(COMP_CPU("UL1")),
	DAILINK_COMP_ARRAY(COMP_DUMMY()),
	DAILINK_COMP_ARRAY(COMP_EMPTY()));
SND_SOC_DAILINK_DEFS(capture2,
	DAILINK_COMP_ARRAY(COMP_CPU("UL2")),
	DAILINK_COMP_ARRAY(COMP_DUMMY()),
	DAILINK_COMP_ARRAY(COMP_EMPTY()));
SND_SOC_DAILINK_DEFS(capture3,
	DAILINK_COMP_ARRAY(COMP_CPU("UL3")),
	DAILINK_COMP_ARRAY(COMP_DUMMY()),
	DAILINK_COMP_ARRAY(COMP_EMPTY()));
SND_SOC_DAILINK_DEFS(capture4,
	DAILINK_COMP_ARRAY(COMP_CPU("UL4")),
	DAILINK_COMP_ARRAY(COMP_DUMMY()),
	DAILINK_COMP_ARRAY(COMP_EMPTY()));
SND_SOC_DAILINK_DEFS(capture5,
	DAILINK_COMP_ARRAY(COMP_CPU("UL5")),
	DAILINK_COMP_ARRAY(COMP_DUMMY()),
	DAILINK_COMP_ARRAY(COMP_EMPTY()));
SND_SOC_DAILINK_DEFS(capture6,
	DAILINK_COMP_ARRAY(COMP_CPU("UL6")),
	DAILINK_COMP_ARRAY(COMP_DUMMY()),
	DAILINK_COMP_ARRAY(COMP_EMPTY()));
SND_SOC_DAILINK_DEFS(capture7,
	DAILINK_COMP_ARRAY(COMP_CPU("UL7")),
	DAILINK_COMP_ARRAY(COMP_DUMMY()),
	DAILINK_COMP_ARRAY(COMP_EMPTY()));
SND_SOC_DAILINK_DEFS(capture8,
	DAILINK_COMP_ARRAY(COMP_CPU("UL8")),
	DAILINK_COMP_ARRAY(COMP_DUMMY()),
	DAILINK_COMP_ARRAY(COMP_EMPTY()));
SND_SOC_DAILINK_DEFS(capture_mono_1,
	DAILINK_COMP_ARRAY(COMP_CPU("UL_MONO_1")),
	DAILINK_COMP_ARRAY(COMP_DUMMY()),
	DAILINK_COMP_ARRAY(COMP_EMPTY()));
SND_SOC_DAILINK_DEFS(capture_mono_2,
	DAILINK_COMP_ARRAY(COMP_CPU("UL_MONO_2")),
	DAILINK_COMP_ARRAY(COMP_DUMMY()),
	DAILINK_COMP_ARRAY(COMP_EMPTY()));
SND_SOC_DAILINK_DEFS(capture_mono_3,
	DAILINK_COMP_ARRAY(COMP_CPU("UL_MONO_3")),
	DAILINK_COMP_ARRAY(COMP_DUMMY()),
	DAILINK_COMP_ARRAY(COMP_EMPTY()));

/* hostless */
SND_SOC_DAILINK_DEFS(hostless_lpbk,
	DAILINK_COMP_ARRAY(COMP_CPU("Hostless LPBK DAI")),
	DAILINK_COMP_ARRAY(COMP_DUMMY()),
	DAILINK_COMP_ARRAY(COMP_EMPTY()));
SND_SOC_DAILINK_DEFS(hostless_fm,
	DAILINK_COMP_ARRAY(COMP_CPU("Hostless FM DAI")),
	DAILINK_COMP_ARRAY(COMP_DUMMY()),
	DAILINK_COMP_ARRAY(COMP_EMPTY()));
SND_SOC_DAILINK_DEFS(hostless_speech,
	DAILINK_COMP_ARRAY(COMP_CPU("Hostless Speech DAI")),
	DAILINK_COMP_ARRAY(COMP_DUMMY()),
	DAILINK_COMP_ARRAY(COMP_EMPTY()));
SND_SOC_DAILINK_DEFS(hostless_sph_echo_ref,
	DAILINK_COMP_ARRAY(COMP_CPU("Hostless_Sph_Echo_Ref_DAI")),
	DAILINK_COMP_ARRAY(COMP_DUMMY()),
	DAILINK_COMP_ARRAY(COMP_EMPTY()));
SND_SOC_DAILINK_DEFS(hostless_spk_init,
	DAILINK_COMP_ARRAY(COMP_CPU("Hostless_Spk_Init_DAI")),
	DAILINK_COMP_ARRAY(COMP_DUMMY()),
	DAILINK_COMP_ARRAY(COMP_EMPTY()));
SND_SOC_DAILINK_DEFS(hostless_adda_dl_i2s_out,
	DAILINK_COMP_ARRAY(COMP_CPU("Hostless_ADDA_DL_I2S_OUT DAI")),
	DAILINK_COMP_ARRAY(COMP_DUMMY()),
	DAILINK_COMP_ARRAY(COMP_EMPTY()));
SND_SOC_DAILINK_DEFS(hostless_src1,
	DAILINK_COMP_ARRAY(COMP_CPU("Hostless_SRC_1_DAI")),
	DAILINK_COMP_ARRAY(COMP_DUMMY()),
	DAILINK_COMP_ARRAY(COMP_EMPTY()));
SND_SOC_DAILINK_DEFS(hostless_src_bargein,
	DAILINK_COMP_ARRAY(COMP_CPU("Hostless_SRC_Bargein_DAI")),
	DAILINK_COMP_ARRAY(COMP_DUMMY()),
	DAILINK_COMP_ARRAY(COMP_EMPTY()));

/* BE */
SND_SOC_DAILINK_DEFS(adda,
	DAILINK_COMP_ARRAY(COMP_CPU("ADDA")),
	DAILINK_COMP_ARRAY(COMP_CODEC("mt6358-sound",
				      "mt6358-snd-codec-aif1")),
	DAILINK_COMP_ARRAY(COMP_EMPTY()));
SND_SOC_DAILINK_DEFS(ap_dmic,
	DAILINK_COMP_ARRAY(COMP_CPU("AP_DMIC")),
	DAILINK_COMP_ARRAY(COMP_DUMMY()),
	DAILINK_COMP_ARRAY(COMP_EMPTY()));
SND_SOC_DAILINK_DEFS(i2s0,
	DAILINK_COMP_ARRAY(COMP_CPU("I2S0")),
	DAILINK_COMP_ARRAY(COMP_DUMMY()),
	DAILINK_COMP_ARRAY(COMP_EMPTY()));
SND_SOC_DAILINK_DEFS(i2s1,
	DAILINK_COMP_ARRAY(COMP_CPU("I2S1")),
	DAILINK_COMP_ARRAY(COMP_DUMMY()),
	DAILINK_COMP_ARRAY(COMP_EMPTY()));
SND_SOC_DAILINK_DEFS(i2s2,
	DAILINK_COMP_ARRAY(COMP_CPU("I2S2")),
	DAILINK_COMP_ARRAY(COMP_DUMMY()),
	DAILINK_COMP_ARRAY(COMP_EMPTY()));
SND_SOC_DAILINK_DEFS(i2s3,
	DAILINK_COMP_ARRAY(COMP_CPU("I2S3")),
	DAILINK_COMP_ARRAY(COMP_DUMMY()),
	DAILINK_COMP_ARRAY(COMP_EMPTY()));
SND_SOC_DAILINK_DEFS(hw_gain1,
	DAILINK_COMP_ARRAY(COMP_CPU("HW Gain 1")),
	DAILINK_COMP_ARRAY(COMP_DUMMY()),
	DAILINK_COMP_ARRAY(COMP_EMPTY()));
SND_SOC_DAILINK_DEFS(hw_gain2,
	DAILINK_COMP_ARRAY(COMP_CPU("HW Gain 2")),
	DAILINK_COMP_ARRAY(COMP_DUMMY()),
	DAILINK_COMP_ARRAY(COMP_EMPTY()));
SND_SOC_DAILINK_DEFS(hw_src1,
	DAILINK_COMP_ARRAY(COMP_CPU("HW_SRC_1")),
	DAILINK_COMP_ARRAY(COMP_DUMMY()),
	DAILINK_COMP_ARRAY(COMP_EMPTY()));
SND_SOC_DAILINK_DEFS(hw_src2,
	DAILINK_COMP_ARRAY(COMP_CPU("HW_SRC_2")),
	DAILINK_COMP_ARRAY(COMP_DUMMY()),
	DAILINK_COMP_ARRAY(COMP_EMPTY()));
SND_SOC_DAILINK_DEFS(connsys_i2s,
	DAILINK_COMP_ARRAY(COMP_CPU("CONNSYS_I2S")),
	DAILINK_COMP_ARRAY(COMP_DUMMY()),
	DAILINK_COMP_ARRAY(COMP_EMPTY()));
SND_SOC_DAILINK_DEFS(pcm2,
	DAILINK_COMP_ARRAY(COMP_CPU("PCM 2")),
	DAILINK_COMP_ARRAY(COMP_DUMMY()),
	DAILINK_COMP_ARRAY(COMP_EMPTY()));

/* hostless */
SND_SOC_DAILINK_DEFS(hostless_ul1,
	DAILINK_COMP_ARRAY(COMP_CPU("Hostless_UL1 DAI")),
	DAILINK_COMP_ARRAY(COMP_DUMMY()),
	DAILINK_COMP_ARRAY(COMP_EMPTY()));
SND_SOC_DAILINK_DEFS(hostless_ul2,
	DAILINK_COMP_ARRAY(COMP_CPU("Hostless_UL2 DAI")),
	DAILINK_COMP_ARRAY(COMP_DUMMY()),
	DAILINK_COMP_ARRAY(COMP_EMPTY()));
SND_SOC_DAILINK_DEFS(hostless_ul3,
	DAILINK_COMP_ARRAY(COMP_CPU("Hostless_UL3 DAI")),
	DAILINK_COMP_ARRAY(COMP_DUMMY()),
	DAILINK_COMP_ARRAY(COMP_EMPTY()));
SND_SOC_DAILINK_DEFS(hostless_ul6,
	DAILINK_COMP_ARRAY(COMP_CPU("Hostless_UL6 DAI")),
	DAILINK_COMP_ARRAY(COMP_DUMMY()),
	DAILINK_COMP_ARRAY(COMP_EMPTY()));
SND_SOC_DAILINK_DEFS(hostless_hw_gain_aaudio,
	DAILINK_COMP_ARRAY(COMP_CPU("Hostless HW Gain AAudio DAI")),
	DAILINK_COMP_ARRAY(COMP_DUMMY()),
	DAILINK_COMP_ARRAY(COMP_EMPTY()));
SND_SOC_DAILINK_DEFS(hostless_src_aaudio,
	DAILINK_COMP_ARRAY(COMP_CPU("Hostless SRC AAudio DAI")),
	DAILINK_COMP_ARRAY(COMP_DUMMY()),
	DAILINK_COMP_ARRAY(COMP_EMPTY()));
#if IS_ENABLED(CONFIG_SND_SOC_MTK_BTCVSD) && !defined(CONFIG_FPGA_EARLY_PORTING)
SND_SOC_DAILINK_DEFS(btcvsd,
	DAILINK_COMP_ARRAY(COMP_DUMMY()),
	DAILINK_COMP_ARRAY(COMP_DUMMY()),
	DAILINK_COMP_ARRAY(COMP_PLATFORM("18050000.mtk-btcvsd-snd")));
#endif
#if IS_ENABLED(CONFIG_MTK_VOW_SUPPORT)  && !defined(CONFIG_FPGA_EARLY_PORTING)
#if !defined(SKIP_SB_VOW)
SND_SOC_DAILINK_DEFS(vow,
	DAILINK_COMP_ARRAY(COMP_DUMMY()),
	DAILINK_COMP_ARRAY(COMP_CODEC(DEVICE_MT6358_NAME,
						"mt6358-snd-codec-vow")),
	DAILINK_COMP_ARRAY(COMP_EMPTY()));
#endif
#endif
#if IS_ENABLED(CONFIG_MTK_ULTRASND_PROXIMITY) && !defined(CONFIG_FPGA_EARLY_PORTING)
SND_SOC_DAILINK_DEFS(ultra,
		DAILINK_COMP_ARRAY(COMP_DUMMY()),
		DAILINK_COMP_ARRAY(COMP_DUMMY()),
		DAILINK_COMP_ARRAY(COMP_PLATFORM("snd_scp_ultra")));
#endif

#if IS_ENABLED(CONFIG_MTK_SCP_AUDIO)
SND_SOC_DAILINK_DEFS(scpspkprocess,
	DAILINK_COMP_ARRAY(COMP_DUMMY()),
	DAILINK_COMP_ARRAY(COMP_DUMMY()),
	DAILINK_COMP_ARRAY(COMP_DUMMY()));
#endif

static struct snd_soc_dai_link mt6789_mt6366_dai_links[] = {
	/* Front End DAI links */
	{
		.name = "Playback_1",
		.stream_name = "Playback_1",
		.trigger = {SND_SOC_DPCM_TRIGGER_PRE,
			    SND_SOC_DPCM_TRIGGER_PRE},
		.dynamic = 1,
		.dpcm_playback = 1,
		SND_SOC_DAILINK_REG(playback1),
	},
	{
		.name = "Playback_12",
		.stream_name = "Playback_12",
		.trigger = {SND_SOC_DPCM_TRIGGER_PRE,
			    SND_SOC_DPCM_TRIGGER_PRE},
		.dynamic = 1,
		.dpcm_playback = 1,
		SND_SOC_DAILINK_REG(playback12),
	},
	{
		.name = "Playback_2",
		.stream_name = "Playback_2",
		.trigger = {SND_SOC_DPCM_TRIGGER_PRE,
			    SND_SOC_DPCM_TRIGGER_PRE},
		.dynamic = 1,
		.dpcm_playback = 1,
		SND_SOC_DAILINK_REG(playback2),
	},
	{
		.name = "Playback_3",
		.stream_name = "Playback_3",
		.trigger = {SND_SOC_DPCM_TRIGGER_PRE,
			    SND_SOC_DPCM_TRIGGER_PRE},
		.dynamic = 1,
		.dpcm_playback = 1,
		SND_SOC_DAILINK_REG(playback3),
	},
	{
		.name = "Playback_4",
		.stream_name = "Playback_4",
		.trigger = {SND_SOC_DPCM_TRIGGER_PRE,
			    SND_SOC_DPCM_TRIGGER_PRE},
		.dynamic = 1,
		.dpcm_playback = 1,
		SND_SOC_DAILINK_REG(playback4),
	},
	{
		.name = "Playback_5",
		.stream_name = "Playback_5",
		.trigger = {SND_SOC_DPCM_TRIGGER_PRE,
			    SND_SOC_DPCM_TRIGGER_PRE},
		.dynamic = 1,
		.dpcm_playback = 1,
		SND_SOC_DAILINK_REG(playback5),
	},
	{
		.name = "Playback_6",
		.stream_name = "Playback_6",
		.trigger = {SND_SOC_DPCM_TRIGGER_PRE,
			    SND_SOC_DPCM_TRIGGER_PRE},
		.dynamic = 1,
		.dpcm_playback = 1,
		SND_SOC_DAILINK_REG(playback6),
	},
	{
		.name = "Playback_7",
		.stream_name = "Playback_7",
		.trigger = {SND_SOC_DPCM_TRIGGER_PRE,
			    SND_SOC_DPCM_TRIGGER_PRE},
		.dynamic = 1,
		.dpcm_playback = 1,
		SND_SOC_DAILINK_REG(playback7),
	},
	{
		.name = "Playback_8",
		.stream_name = "Playback_8",
		.trigger = {SND_SOC_DPCM_TRIGGER_PRE,
			    SND_SOC_DPCM_TRIGGER_PRE},
		.dynamic = 1,
		.dpcm_playback = 1,
		SND_SOC_DAILINK_REG(playback8),
	},
	{
		.name = "Capture_1",
		.stream_name = "Capture_1",
		.trigger = {SND_SOC_DPCM_TRIGGER_PRE,
			    SND_SOC_DPCM_TRIGGER_PRE},
		.dynamic = 1,
		.dpcm_capture = 1,
		SND_SOC_DAILINK_REG(capture1),
	},
	{
		.name = "Capture_2",
		.stream_name = "Capture_2",
		.trigger = {SND_SOC_DPCM_TRIGGER_PRE,
			    SND_SOC_DPCM_TRIGGER_PRE},
		.dynamic = 1,
		.dpcm_capture = 1,
		SND_SOC_DAILINK_REG(capture2),
	},
	{
		.name = "Capture_3",
		.stream_name = "Capture_3",
		.trigger = {SND_SOC_DPCM_TRIGGER_PRE,
			    SND_SOC_DPCM_TRIGGER_PRE},
		.dynamic = 1,
		.dpcm_capture = 1,
		SND_SOC_DAILINK_REG(capture3),
	},
	{
		.name = "Capture_4",
		.stream_name = "Capture_4",
		.trigger = {SND_SOC_DPCM_TRIGGER_PRE,
			    SND_SOC_DPCM_TRIGGER_PRE},
		.dynamic = 1,
		.dpcm_capture = 1,
		SND_SOC_DAILINK_REG(capture4),
	},
	{
		.name = "Capture_5",
		.stream_name = "Capture_5",
		.trigger = {SND_SOC_DPCM_TRIGGER_PRE,
			    SND_SOC_DPCM_TRIGGER_PRE},
		.dynamic = 1,
		.dpcm_capture = 1,
		SND_SOC_DAILINK_REG(capture5),
	},
	{
		.name = "Capture_6",
		.stream_name = "Capture_6",
		.trigger = {SND_SOC_DPCM_TRIGGER_PRE,
			    SND_SOC_DPCM_TRIGGER_PRE},
		.dynamic = 1,
		.dpcm_capture = 1,
		SND_SOC_DAILINK_REG(capture6),
	},
	{
		.name = "Capture_7",
		.stream_name = "Capture_7",
		.trigger = {SND_SOC_DPCM_TRIGGER_PRE,
			    SND_SOC_DPCM_TRIGGER_PRE},
		.dynamic = 1,
		.dpcm_capture = 1,
		SND_SOC_DAILINK_REG(capture7),
	},
	{
		.name = "Capture_8",
		.stream_name = "Capture_8",
		.trigger = {SND_SOC_DPCM_TRIGGER_PRE,
			    SND_SOC_DPCM_TRIGGER_PRE},
		.dynamic = 1,
		.dpcm_capture = 1,
		SND_SOC_DAILINK_REG(capture8),
	},
	{
		.name = "Capture_Mono_1",
		.stream_name = "Capture_Mono_1",
		.trigger = {SND_SOC_DPCM_TRIGGER_PRE,
			    SND_SOC_DPCM_TRIGGER_PRE},
		.dynamic = 1,
		.dpcm_capture = 1,
		SND_SOC_DAILINK_REG(capture_mono_1),
	},
	{
		.name = "Capture_Mono_2",
		.stream_name = "Capture_Mono_2",
		.trigger = {SND_SOC_DPCM_TRIGGER_PRE,
			    SND_SOC_DPCM_TRIGGER_PRE},
		.dynamic = 1,
		.dpcm_capture = 1,
		SND_SOC_DAILINK_REG(capture_mono_2),
	},
	{
		.name = "Capture_Mono_3",
		.stream_name = "Capture_Mono_3",
		.trigger = {SND_SOC_DPCM_TRIGGER_PRE,
			    SND_SOC_DPCM_TRIGGER_PRE},
		.dynamic = 1,
		.dpcm_capture = 1,
		SND_SOC_DAILINK_REG(capture_mono_3),
	},
	{
		.name = "Hostless_LPBK",
		.stream_name = "Hostless_LPBK",
		.trigger = {SND_SOC_DPCM_TRIGGER_PRE,
			    SND_SOC_DPCM_TRIGGER_PRE},
		.dynamic = 1,
		.dpcm_playback = 1,
		.dpcm_capture = 1,
		.ignore_suspend = 1,
		SND_SOC_DAILINK_REG(hostless_lpbk),
	},
	{
		.name = "Hostless_FM",
		.stream_name = "Hostless_FM",
		.trigger = {SND_SOC_DPCM_TRIGGER_PRE,
			    SND_SOC_DPCM_TRIGGER_PRE},
		.dynamic = 1,
		.dpcm_playback = 1,
		.dpcm_capture = 1,
		.ignore_suspend = 1,
		SND_SOC_DAILINK_REG(hostless_fm),
	},
	{
		.name = "Hostless_Speech",
		.stream_name = "Hostless_Speech",
		.trigger = {SND_SOC_DPCM_TRIGGER_PRE,
			    SND_SOC_DPCM_TRIGGER_PRE},
		.dynamic = 1,
		.dpcm_playback = 1,
		.dpcm_capture = 1,
		.ignore_suspend = 1,
		SND_SOC_DAILINK_REG(hostless_speech),
	},
	{
		.name = "Hostless_Sph_Echo_Ref",
		.stream_name = "Hostless_Sph_Echo_Ref",
		.trigger = {SND_SOC_DPCM_TRIGGER_PRE,
			    SND_SOC_DPCM_TRIGGER_PRE},
		.dynamic = 1,
		.dpcm_playback = 1,
		.dpcm_capture = 1,
		.ignore_suspend = 1,
		SND_SOC_DAILINK_REG(hostless_sph_echo_ref),
	},
	{
		.name = "Hostless_Spk_Init",
		.stream_name = "Hostless_Spk_Init",
		.trigger = {SND_SOC_DPCM_TRIGGER_PRE,
			    SND_SOC_DPCM_TRIGGER_PRE},
		.dynamic = 1,
		.dpcm_playback = 1,
		.dpcm_capture = 1,
		.ignore_suspend = 1,
		SND_SOC_DAILINK_REG(hostless_spk_init),
	},
	{
		.name = "Hostless_ADDA_DL_I2S_OUT",
		.stream_name = "Hostless_ADDA_DL_I2S_OUT",
		.trigger = {SND_SOC_DPCM_TRIGGER_PRE,
			    SND_SOC_DPCM_TRIGGER_PRE},
		.dynamic = 1,
		.dpcm_playback = 1,
		.ignore_suspend = 1,
		SND_SOC_DAILINK_REG(hostless_adda_dl_i2s_out),
	},
	{
		.name = "Hostless_SRC_1",
		.stream_name = "Hostless_SRC_1",
		.trigger = {SND_SOC_DPCM_TRIGGER_PRE,
			    SND_SOC_DPCM_TRIGGER_PRE},
		.dynamic = 1,
		.dpcm_playback = 1,
		.dpcm_capture = 1,
		.ignore_suspend = 1,
		SND_SOC_DAILINK_REG(hostless_src1),
	},
	{
		.name = "Hostless_SRC_Bargein",
		.stream_name = "Hostless_SRC_Bargein",
		.trigger = {SND_SOC_DPCM_TRIGGER_PRE,
			    SND_SOC_DPCM_TRIGGER_PRE},
		.dynamic = 1,
		.dpcm_playback = 1,
		.dpcm_capture = 1,
		.ignore_suspend = 1,
		SND_SOC_DAILINK_REG(hostless_src_bargein),
	},
	/* Back End DAI links */
	{
		.name = "Primary Codec",
		.no_pcm = 1,
		.dpcm_playback = 1,
		.dpcm_capture = 1,
		.ignore_suspend = 1,
		.init = mt6789_mt6366_init,
		SND_SOC_DAILINK_REG(adda),
	},
	{
		.name = "AP_DMIC",
		.no_pcm = 1,
		.dpcm_capture = 1,
		.ignore_suspend = 1,
		SND_SOC_DAILINK_REG(ap_dmic),
	},
	{
		.name = "I2S0",
		.dai_fmt = SND_SOC_DAIFMT_I2S | SND_SOC_DAIFMT_CBS_CFS
			| SND_SOC_DAIFMT_GATED,
		.ops = &mt6789_mt6366_i2s_ops,
		.no_pcm = 1,
		.dpcm_capture = 1,
		.ignore_suspend = 1,
		.ignore_pmdown_time = 1,
		.be_hw_params_fixup = mt6789_i2s_hw_params_fixup,
		SND_SOC_DAILINK_REG(i2s0),
	},
	{
		.name = "I2S1",
		.dai_fmt = SND_SOC_DAIFMT_I2S | SND_SOC_DAIFMT_CBS_CFS
			| SND_SOC_DAIFMT_GATED,
		.ops = &mt6789_mt6366_i2s_ops,
		.no_pcm = 1,
		.dpcm_playback = 1,
		.ignore_suspend = 1,
		.be_hw_params_fixup = mt6789_i2s_hw_params_fixup,
		SND_SOC_DAILINK_REG(i2s1),
	},
	{
		.name = "I2S2",
		.dai_fmt = SND_SOC_DAIFMT_I2S | SND_SOC_DAIFMT_CBS_CFS
			| SND_SOC_DAIFMT_GATED,
		.ops = &mt6789_mt6366_i2s_ops,
		.no_pcm = 1,
		.dpcm_capture = 1,
		.ignore_suspend = 1,
		.be_hw_params_fixup = mt6789_i2s_hw_params_fixup,
		SND_SOC_DAILINK_REG(i2s2),
	},
	{
		.name = "I2S3",
		.dai_fmt = SND_SOC_DAIFMT_I2S | SND_SOC_DAIFMT_CBS_CFS
			| SND_SOC_DAIFMT_GATED,
		.ops = &mt6789_mt6366_i2s_ops,
		.no_pcm = 1,
		.dpcm_playback = 1,
		.ignore_suspend = 1,
		.ignore_pmdown_time = 1,
		.be_hw_params_fixup = mt6789_i2s_hw_params_fixup,
		SND_SOC_DAILINK_REG(i2s3),
	},
	{
		.name = "HW Gain 1",
		.no_pcm = 1,
		.dpcm_playback = 1,
		.dpcm_capture = 1,
		.ignore_suspend = 1,
		SND_SOC_DAILINK_REG(hw_gain1),
	},
	{
		.name = "HW Gain 2",
		.no_pcm = 1,
		.dpcm_playback = 1,
		.dpcm_capture = 1,
		.ignore_suspend = 1,
		SND_SOC_DAILINK_REG(hw_gain2),
	},
	{
		.name = "HW_SRC_1",
		.no_pcm = 1,
		.dpcm_playback = 1,
		.dpcm_capture = 1,
		.ignore_suspend = 1,
		SND_SOC_DAILINK_REG(hw_src1),
	},
	{
		.name = "HW_SRC_2",
		.no_pcm = 1,
		.dpcm_playback = 1,
		.dpcm_capture = 1,
		.ignore_suspend = 1,
		SND_SOC_DAILINK_REG(hw_src2),
	},
	{
		.name = "CONNSYS_I2S",
		.no_pcm = 1,
		.dpcm_capture = 1,
		.ignore_suspend = 1,
		SND_SOC_DAILINK_REG(connsys_i2s),
	},
	{
		.name = "PCM 2",
		.no_pcm = 1,
		.dpcm_playback = 1,
		.dpcm_capture = 1,
		.ignore_suspend = 1,
		SND_SOC_DAILINK_REG(pcm2),
	},
	/* dummy BE for ul memif to record from dl memif */
	{
		.name = "Hostless_UL1",
		.no_pcm = 1,
		.dpcm_capture = 1,
		.ignore_suspend = 1,
		SND_SOC_DAILINK_REG(hostless_ul1),
	},
	{
		.name = "Hostless_UL2",
		.no_pcm = 1,
		.dpcm_capture = 1,
		.ignore_suspend = 1,
		SND_SOC_DAILINK_REG(hostless_ul2),
	},
	{
		.name = "Hostless_UL3",
		.no_pcm = 1,
		.dpcm_capture = 1,
		.ignore_suspend = 1,
		SND_SOC_DAILINK_REG(hostless_ul3),
	},
	{
		.name = "Hostless_UL6",
		.no_pcm = 1,
		.dpcm_capture = 1,
		.ignore_suspend = 1,
		SND_SOC_DAILINK_REG(hostless_ul6),
	},
	{
		.name = "Hostless_HW_Gain_AAudio",
		.stream_name = "Hostless_HW_Gain_AAudio",
		.trigger = {SND_SOC_DPCM_TRIGGER_PRE,
			    SND_SOC_DPCM_TRIGGER_PRE},
		.dynamic = 1,
		.dpcm_capture = 1,
		.ignore_suspend = 1,
		SND_SOC_DAILINK_REG(hostless_hw_gain_aaudio),
	},
	{
		.name = "Hostless_SRC_AAudio",
		.stream_name = "Hostless_SRC_AAudio",
		.trigger = {SND_SOC_DPCM_TRIGGER_PRE,
			    SND_SOC_DPCM_TRIGGER_PRE},
		.dynamic = 1,
		.dpcm_playback = 1,
		.dpcm_capture = 1,
		.ignore_suspend = 1,
		SND_SOC_DAILINK_REG(hostless_src_aaudio),
	},
	/* BTCVSD */
#if IS_ENABLED(CONFIG_SND_SOC_MTK_BTCVSD) && !defined(CONFIG_FPGA_EARLY_PORTING)
	{
		.name = "BTCVSD",
		.stream_name = "BTCVSD",
		SND_SOC_DAILINK_REG(btcvsd),
	},
#endif
	/* VoW */
#if IS_ENABLED(CONFIG_MTK_VOW_SUPPORT) && !defined(CONFIG_FPGA_EARLY_PORTING)
#if !defined(SKIP_SB_VOW)
	{
		.name = "VOW_Capture",
		.stream_name = "VOW_Capture",
		.ignore_suspend = 1,
		.ops = &mt6789_mt6366_vow_ops,
		SND_SOC_DAILINK_REG(vow),
	},
#endif
#endif
#if IS_ENABLED(CONFIG_MTK_ULTRASND_PROXIMITY) && !defined(CONFIG_FPGA_EARLY_PORTING)
	{
		.name = "SCP_ULTRA_Playback",
		.stream_name = "SCP_ULTRA_Playback",
		SND_SOC_DAILINK_REG(ultra),
	},
#endif
#if IS_ENABLED(CONFIG_MTK_SCP_AUDIO)
	{
		.name = "SCP_SPK_Process",
		.stream_name = "SCP_SPK_Process",
		SND_SOC_DAILINK_REG(scpspkprocess),
	},
#endif
};

static struct snd_soc_card mt6789_mt6366_soc_card = {
	.name = "mt6789-mt6366",
	.owner = THIS_MODULE,
	.dai_link = mt6789_mt6366_dai_links,
	.num_links = ARRAY_SIZE(mt6789_mt6366_dai_links),

	.controls = mt6789_mt6366_controls,
	.num_controls = ARRAY_SIZE(mt6789_mt6366_controls),
	.dapm_widgets = mt6789_mt6366_widgets,
	.num_dapm_widgets = ARRAY_SIZE(mt6789_mt6366_widgets),
	.dapm_routes = mt6789_mt6366_routes,
	.num_dapm_routes = ARRAY_SIZE(mt6789_mt6366_routes),
};

#if IS_ENABLED(CONFIG_MTK_SCP_AUDIO)
int mtk_update_scp_audio_info(struct snd_soc_card *card,
			struct platform_device *pdev)
{
	struct snd_soc_dai_link *dai_link;
	int i = 0;

	/* find dai link of SCP_SPK_Process */
	for_each_card_prelinks(card, i, dai_link) {
		if (strcmp(dai_link->name, "SCP_SPK_Process") == 0) {
			dai_link->cpus->name = NULL;
			dai_link->cpus->dai_name = "audio_task_spk_process";
			dai_link->platforms->name = "snd_scp_audio";
			dai_link->platforms->dai_name = NULL;
			dev_info(&pdev->dev, "scp audio updated\n");
		}
	}

	return 0;
}
#endif

//add 2025-01-14
void get_spk_ext_amp_gpio(struct platform_device *pdev){
	int ret=0;
	ext_amp_gpio_one = of_get_named_gpio(pdev->dev.of_node, "ext-amp-gpio-one", 0);
	if (!gpio_is_valid(ext_amp_gpio_one)){
		dev_err(&pdev->dev, "%s get invalid ext-amp-one %d\n",__func__, ext_amp_gpio_one);
	}else{
		ret = gpio_request(ext_amp_gpio_one, "ext_amp_en");
		if (ret < 0) {
			dev_err(&pdev->dev, "%s: request ext one pin failed\n", __func__);
		}else{
			ret = gpio_direction_output(ext_amp_gpio_one, 0);
			if (ret) {
				dev_err(&pdev->dev, "unable to set direction for one gpio [%d]\n", ext_amp_gpio_one);
			}
		}
	}
	//
	ext_amp_gpio_three = of_get_named_gpio(pdev->dev.of_node, "ext-amp-gpio-three", 0);
	if (!gpio_is_valid(ext_amp_gpio_three)){
		dev_err(&pdev->dev, "%s get invalid ext-amp-three %d\n",__func__, ext_amp_gpio_three);
	}else{
		ret = gpio_request(ext_amp_gpio_three, "ext_amp_en");
		if (ret < 0) {
			dev_err(&pdev->dev, "%s: request ext three pin failed\n", __func__);
		}else{
			ret = gpio_direction_output(ext_amp_gpio_three, 0);
			if (ret) {
				dev_err(&pdev->dev, "unable to set direction for three gpio [%d]\n", ext_amp_gpio_three);
			}
		}
	}
	//
	ext_amp_gpio_four = of_get_named_gpio(pdev->dev.of_node, "ext-amp-gpio-four", 0);
	if (!gpio_is_valid(ext_amp_gpio_four)){
		dev_err(&pdev->dev, "%s get invalid ext-amp-four %d\n",__func__, ext_amp_gpio_four);
	}else{
		ret = gpio_request(ext_amp_gpio_four, "ext_amp_en");
		if (ret < 0) {
			dev_err(&pdev->dev, "%s: request ext four pin failed\n", __func__);
		}else{
			ret = gpio_direction_output(ext_amp_gpio_four, 0);
			if (ret) {
				dev_err(&pdev->dev, "unable to set direction for four gpio [%d]\n", ext_amp_gpio_four);
			}
		}
	}
	//end
}


static ssize_t set_ext_spk_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t count)
{
	u8 val=0;
	u32 i=0;
	if (kstrtou8(buf, 0, &val))
		return -EINVAL;	
	mt6789_ext_hp_amp_turn_on_or_off(0);
	//printk("tom add val=%d, %s !\n", val, __func__);
	if(val==0){
		test_spk_amp_mode = 0;
		mt6789_ext_hp_amp_turn_on_or_off(0);
	}
	//GPIO_SPK_TWO--R3--GPIO6  J12--U4--left up	
	if(val==1){
		test_spk_amp_mode = 1;
		if (ext_amp_gpio >= 0){		
			for (i = 0; i < spk_amp_mode; i++) {
				gpio_direction_output(ext_amp_gpio, 0);
				udelay(2);
				gpio_direction_output(ext_amp_gpio, 1);
				udelay(2);
			}
		}
	}
	//GPIO_SPK_THREE--AE27--GPIO106 J2--U42--left below
	if(val==2){
		test_spk_amp_mode = 2;
		if (ext_amp_gpio_three >= 0){		
			for (i = 0; i < spk_amp_mode; i++) {
				gpio_direction_output(ext_amp_gpio_three, 0);
				udelay(2);
				gpio_direction_output(ext_amp_gpio_three, 1);
				udelay(2);
			}
		}
	}
	//GPIO_SPK_FOUR--AF28--GPIO107 J6--U43--right up
	if(val==3){
		test_spk_amp_mode = 3;
		if (ext_amp_gpio_four >= 0){		
			for (i = 0; i < spk_amp_mode; i++) {
				gpio_direction_output(ext_amp_gpio_four, 0);
				udelay(2);
				gpio_direction_output(ext_amp_gpio_four, 1);
				udelay(2);
			}
		}
	}
	//GPIO_SPK_ONE--AF23--GPIO67 J1--U5--right below
	if(val==4){
		test_spk_amp_mode = 4;
		if (ext_amp_gpio_one >= 0){		
			for (i = 0; i < spk_amp_mode; i++) {
				gpio_direction_output(ext_amp_gpio_one, 0);
				udelay(2);
				gpio_direction_output(ext_amp_gpio_one, 1);
				udelay(2);
			}
		}
	}	
	return count;
}
static DEVICE_ATTR_WO(set_ext_spk);

static struct attribute *spk_class_attrs[] = {
	&dev_attr_set_ext_spk.attr,
	NULL,
};

static struct attribute_group ext_spk_groups = {
	.name = NULL,
	.attrs = spk_class_attrs,
};
//end


static int mt6789_mt6366_dev_probe(struct platform_device *pdev)
{
	struct snd_soc_card *card = &mt6789_mt6366_soc_card;
	struct device_node *platform_node, *spk_node;
	int ret, i;
	struct snd_soc_dai_link *dai_link;
#if IS_ENABLED(CONFIG_MTK_SCP_AUDIO)
	struct device_node *scp_audio_node;
	int spkProcessEnable = 0;
#endif
	test_spk_amp_mode = 0;
	dev_info(&pdev->dev, "%s()\n", __func__);

	/* update speaker type */
	ret = mtk_spk_update_info(card, pdev);
	if (ret) {
		dev_err(&pdev->dev, "%s(), mtk_spk_update_info error\n",
			__func__);
		return -EINVAL;
	}

	/* get platform node */
	platform_node = of_parse_phandle(pdev->dev.of_node,
					 "mediatek,platform", 0);
	if (!platform_node) {
		dev_err(&pdev->dev, "Property 'platform' missing or invalid\n");
		return -EINVAL;
	}

	/* get speaker codec node */
	spk_node = of_get_child_by_name(pdev->dev.of_node,
					"mediatek,speaker-codec");
	if (!spk_node) {
		dev_err(&pdev->dev,
			"spk_node of_get_child_by_name fail\n");
		return -EINVAL;
	}

	for_each_card_prelinks(card, i, dai_link) {
		if (!dai_link->platforms->name)
			dai_link->platforms->of_node = platform_node;

		if (!strcmp(dai_link->name, "Speaker Codec")) {
			ret = snd_soc_of_get_dai_link_codecs(
						&pdev->dev, spk_node, dai_link);
			if (ret < 0) {
				dev_err(&pdev->dev,
					"Speaker Codec get_dai_link fail: %d\n", ret);
				return -EINVAL;
			}
		} else if (!strcmp(dai_link->name, "Speaker Codec Ref")) {
			ret = snd_soc_of_get_dai_link_codecs(
						&pdev->dev, spk_node, dai_link);
			if (ret < 0) {
				dev_err(&pdev->dev,
					"Speaker Codec Ref get_dai_link fail: %d\n", ret);
				return -EINVAL;
			}
		}
	}
	//add 2025-01-14
	get_spk_ext_amp_gpio(pdev);
	//end
	ext_amp_gpio = of_get_named_gpio(pdev->dev.of_node, "ext-amp-gpio", 0);
	if (!gpio_is_valid(ext_amp_gpio))
		dev_err(&pdev->dev, "%s get invalid ext-amp-gpio %d\n",
				__func__, ext_amp_gpio);
	else {
		ret = gpio_request(ext_amp_gpio, "ext_amp_en");
		if (ret < 0) {
			dev_err(&pdev->dev, "%s: request ext amp pin failed\n", __func__);
		}else{
			ret = gpio_direction_output(ext_amp_gpio, 0);
			if (ret) {
				dev_err(&pdev->dev, "unable to set direction for gpio [%d]\n", ext_amp_gpio);
			}
		}

		ret = of_property_read_u32(pdev->dev.of_node, "pa_mode", &spk_amp_mode);			//add by zkc
		if (ret < 0) {
			dev_err(&pdev->dev, "%s: get ext amp mode failed\n", __func__);
			spk_amp_mode = 3;	/* default mode is 3 */
		}else{
			if(spk_amp_mode > 4 || spk_amp_mode < 1)	/* set mode only allow 1/2/3/4. */
				spk_amp_mode = 3;
		}
		//pr_info("zkc spk_amp_mode: %d\n", spk_amp_mode);
	}

#if IS_ENABLED(CONFIG_MTK_SCP_AUDIO)
	/* get scp audio node */
	scp_audio_node = of_parse_phandle(pdev->dev.of_node,
					 "mediatek,scp-audio", 0);
	if (scp_audio_node) {
		dev_err(&pdev->dev, "got scp audio node\n");

		ret = of_property_read_u32(scp_audio_node,
					   "scp_spk_process_enable",
					   &spkProcessEnable);
		if (ret != 0) {
			pr_info("%s cannot get spkProcessEnable\n", __func__);
			spkProcessEnable = 0;
		}
		if (spkProcessEnable)
			mtk_update_scp_audio_info(card, pdev);
		pr_info("%s spkProcessEnable %d\n", __func__, spkProcessEnable);
	} else {
		dev_err(&pdev->dev, "can't find scp audio node\n");
	}
#endif

	card->dev = &pdev->dev;

	ret = devm_snd_soc_register_card(&pdev->dev, card);
	if (ret)
		dev_err(&pdev->dev, "%s snd_soc_register_card fail %d\n",
			__func__, ret);
			
	//add 2025-01-14	
	ret = sysfs_create_group(&pdev->dev.kobj, &ext_spk_groups);
	if (ret < 0) {
		dev_err(&pdev->dev,"spk: Cannot create sysfs group ret:%d\n", ret);
			
		return ret;
	}
	ret = sysfs_create_link(NULL, &pdev->dev.kobj, "ext_spk");
	if (ret < 0) {
		dev_err(&pdev->dev, "spk: Failed to create link!");
		return ret;
	}
	//end
	
	return ret;
}


#if IS_ENABLED(CONFIG_OF)
static const struct of_device_id mt6789_mt6366_dt_match[] = {
	{.compatible = "mediatek,mt6789-mt6366-sound",},
	{}
};
#endif

static const struct dev_pm_ops mt6789_mt6366_pm_ops = {
	.poweroff = snd_soc_poweroff,
	.restore = snd_soc_resume,
};

static struct platform_driver mt6789_mt6366_driver = {
	.driver = {
		.name = "mt6789-mt6366",
#if IS_ENABLED(CONFIG_OF)
		.of_match_table = mt6789_mt6366_dt_match,
#endif
		.pm = &mt6789_mt6366_pm_ops,
	},
	.probe = mt6789_mt6366_dev_probe,
};

module_platform_driver(mt6789_mt6366_driver);

/* Module information */
MODULE_DESCRIPTION("MT6789 MT6366 ALSA SoC machine driver");
MODULE_AUTHOR("Yujie Xiao <yujie.xiao@mediatek.com>");
MODULE_LICENSE("GPL v2");
MODULE_ALIAS("mt6789 mt6366 soc card");
