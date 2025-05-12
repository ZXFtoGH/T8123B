#ifndef __MT6319_SPMI_H_
#define __MT6319_SPMI_H_

#include <pal_log.h>
#include <spmi.h>
#include <spmi_sw.h>

#define MT6319TAG	"[MT6319]"
#define MT6319_ERR(fmt, arg...) pal_log_err(MT6319TAG fmt, ##arg)
#define MT6319_WARN(fmt, arg...) pal_log_warn(MT6319TAG fmt, ##arg)
#define MT6319_INFO(fmt, arg...) pal_log_info(MT6319TAG fmt, ##arg)
#define MT6319_DBG(fmt, arg...) pal_log_debug(MT6319TAG fmt, ##arg)

#define MT6319_SWCID_H_CODE	0x15
#define MT6319_SWCID_L_E1_CODE	0x10
#define MT6319_SWCID_L_E2_CODE	0x20
#define MT6319_SWCID_L_E3_CODE	0x30

/* MT6319 power device index */
enum {
	MT6319_S6,
	MT6319_S7,
	MT6319_MAX,
};

/* for different type of MT6319 initial setting */
enum {
	MT6319_TYPE_A,
	MT6319_TYPE_B,
};

struct mt6319_chip {
	uint8_t power_idx;
	uint8_t master_idx;
	uint8_t slave_addr;
	uint8_t type;
	struct spmi_device *spmi_dev;
};

extern int mt6319_write_field_byte(struct spmi_device *dev, uint32_t addr,
				   uint8_t val, uint16_t mask, uint16_t shift);
extern struct mt6319_chip *mt6319_find_chip(int pidx);
extern void mt6319_all_seq_off(unsigned char en_seq_off);
extern void mt6319_dump_record_reg(struct spmi_device *dev);
extern int mt6319_spmi_probe(void);
extern int mt6319_set_buck_slew_rate(int slvid, uint8_t buck_id, int rrate, int frate);

#endif /* __MT6319_SPMI_H_ */
