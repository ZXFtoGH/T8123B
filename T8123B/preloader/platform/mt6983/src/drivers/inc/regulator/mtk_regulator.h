#ifndef __SSPM_MTK_REGULATOR_H
#define __SSPM_MTK_REGULATOR_H

#define DIV_ROUND_UP(n,d) (((n) + (d) - 1) / (d))
#ifndef ARRAY_SIZE
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
#endif

static inline unsigned int gray_to_binary(unsigned int num)
{
	unsigned int mask;

	for (mask = num >> 1; mask != 0; mask = mask >> 1) {
		num = num ^ mask;
	}
	return num;
}

static inline int regulator_ffs(int x)
{
	volatile int r = 1;

	if (!x)
		return r;
	if (!(x & 0xffff)) {
		x >>= 16;
		r += 16;
	}
	if (!(x & 0xff)) {
		x >>= 8;
		r += 8;
	}
	if (!(x & 0xf)) {
		x >>= 4;
		r += 4;
	}
	if (!(x & 3)) {
		x >>= 2;
		r += 2;
	}
	if (!(x & 1)) {
		x >>= 1;
		r += 1;
	}
	return r;
}

struct mtk_regulator {
    const char *name;
    unsigned char id;
    struct regulator_ctrl *reg_ops;
};

enum {
    REGULATOR_MODE_FAST,
    REGULATOR_MODE_NORMAL,
};

extern int mtk_regulator_get(const char *id, struct mtk_regulator *mreg);
extern int mtk_regulator_enable(struct mtk_regulator *mreg, unsigned char enable);
extern int mtk_regulator_is_enabled(struct mtk_regulator *mreg);
extern int mtk_regulator_set_voltage(struct mtk_regulator *mreg, int min_uv, int max_uv);
extern int mtk_regulator_get_voltage(struct mtk_regulator *mreg);
extern int mtk_regulator_set_mode(struct mtk_regulator *mreg, unsigned char mode);
extern int mtk_regulator_get_mode(struct mtk_regulator *mreg);
#ifdef LDO_VOTRIM_SUPPORT
extern int mtk_regulator_set_votrim(struct mtk_regulator *mreg, int trim_uv);
extern int mtk_regulator_get_votrim(struct mtk_regulator *mreg);
#endif

#endif /* __SSPM_MTK_REGULATOR_H */
