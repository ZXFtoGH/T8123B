#ifndef __UPM6920_H__
#define __UPM6920_H__

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/* 配置宏定义 */
#define UPM6920_LK_DRV_VERSION "1.0.0_MTK"

/* 函数声明 */
#if (MULTI_EXT_MAIN_CHARGER)
int mtk_charger_sec_init(void);
int mtk_charger_sec_set_aicr(unsigned int aicr);
int mtk_charger_sec_set_mivr(unsigned int mivr);
int mtk_charger_sec_set_ichg(unsigned int ichg);
int mtk_charger_sec_dump_register(void);
int mtk_charger_sec_get_vbus(unsigned int *vbus);
int mtk_charger_sec_enable_charging(bool enable);
int mtk_charger_sec_get_aicr(unsigned int *aicr);
int mtk_charger_sec_get_pg_state(void);
#else
int mtk_charger_init(void);
int mtk_charger_set_aicr(unsigned int aicr);
int mtk_charger_set_mivr(unsigned int mivr);
int mtk_charger_set_ichg(unsigned int ichg);
int mtk_charger_dump_register(void);
int mtk_charger_get_vbus(unsigned int *vbus);
int mtk_charger_enable_charging(bool enable);
int mtk_charger_get_aicr(unsigned int *aicr);
int mtk_charger_get_pg_state(void);
#endif

#ifdef __cplusplus
}
#endif

#endif /* __UPM6920_H__ */