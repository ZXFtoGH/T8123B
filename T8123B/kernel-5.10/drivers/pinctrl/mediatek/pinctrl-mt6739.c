/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (c) 2019 MediaTek Inc.
*/

#include <linux/module.h>
#include "pinctrl-mtk-mt6739.h"
#include "pinctrl-paris.h"

/* MT6739 have multiple bases to program pin configuration listed as the below:
 * gpio:0x10005000,      io_cfg_lt:0x10002000, io_cfg_lm:0x10002200,
 * io_cfg_lb:0x10002400, io_cfg_bl:0x10002600, io_cfg_bm:0x10002800,
 * io_cfg_rb:0x10002A00, io_cfg_rt:0x10002C00
 * _i_based could be used to indicate what base the pin should be mapped into.
 */

#define PIN_FIELD_BASE(s_pin, e_pin, i_base, s_addr, x_addrs, s_bit, x_bits) \
	PIN_FIELD_CALC(s_pin, e_pin, i_base, s_addr, x_addrs, s_bit, x_bits, \
		       32, 0)

#define PINS_FIELD_BASE(s_pin, e_pin, i_base, s_addr, x_addrs, s_bit, x_bits) \
	PIN_FIELD_CALC(s_pin, e_pin, i_base, s_addr, x_addrs, s_bit, x_bits,  \
		       32, 1)

static const struct mtk_pin_field_calc mt6739_pin_mode_range[] = {
	PIN_FIELD_BASE(0, 7, 0, 0x0300, 0x10, 0, 4),
	PIN_FIELD_BASE(8, 15, 0, 0x0310, 0x10, 0, 4),
	PIN_FIELD_BASE(16, 23, 0, 0x0320, 0x10, 0, 4),
	PIN_FIELD_BASE(24, 31, 0, 0x0330, 0x10, 0, 4),
	PIN_FIELD_BASE(32, 39, 0, 0x0340, 0x10, 0, 4),
	PIN_FIELD_BASE(40, 47, 0, 0x0350, 0x10, 0, 4),
	PIN_FIELD_BASE(48, 55, 0, 0x0360, 0x10, 0, 4),
	PIN_FIELD_BASE(56, 63, 0, 0x0370, 0x10, 0, 4),
	PIN_FIELD_BASE(64, 71, 0, 0x0380, 0x10, 0, 4),
	PIN_FIELD_BASE(72, 79, 0, 0x0390, 0x10, 0, 4),
	PIN_FIELD_BASE(80, 87, 0, 0x03A0, 0x10, 0, 4),
	PIN_FIELD_BASE(88, 95, 0, 0x03B0, 0x10, 0, 4),
	PIN_FIELD_BASE(96, 103, 0, 0x03C0, 0x10, 0, 4),
	PIN_FIELD_BASE(104, 111, 0, 0x03D0, 0x10, 0, 4),
	PIN_FIELD_BASE(112, 119, 0, 0x03E0, 0x10, 0, 4),
	PIN_FIELD_BASE(120, 127, 0, 0x03F0, 0x10, 0, 4),
	PIN_FIELD_BASE(128, 135, 0, 0x0400, 0x10, 0, 4),
	PIN_FIELD_BASE(136, 143, 0, 0x0410, 0x10, 0, 4),
	PIN_FIELD_BASE(144, 151, 0, 0x0420, 0x10, 0, 4),
	PIN_FIELD_BASE(152, 159, 0, 0x0430, 0x10, 0, 4),
	PIN_FIELD_BASE(160, 163, 0, 0x0440, 0x10, 0, 4),
};

static const struct mtk_pin_field_calc mt6739_pin_dir_range[] = {
	PIN_FIELD_BASE(0, 31, 0, 0x0000, 0x10, 0, 1),
	PIN_FIELD_BASE(32, 63, 0, 0x0010, 0x10, 0, 1),
	PIN_FIELD_BASE(64, 95, 0, 0x0020, 0x10, 0, 1),
	PIN_FIELD_BASE(96, 127, 0, 0x0030, 0x10, 0, 1),
	PIN_FIELD_BASE(128, 153, 0, 0x0040, 0x10, 0, 1),
};

static const struct mtk_pin_field_calc mt6739_pin_di_range[] = {
	PIN_FIELD_BASE(0, 31, 0, 0x0200, 0x10, 0, 1),
	PIN_FIELD_BASE(32, 63, 0, 0x0210, 0x10, 0, 1),
	PIN_FIELD_BASE(64, 95, 0, 0x0220, 0x10, 0, 1),
	PIN_FIELD_BASE(96, 127, 0, 0x0230, 0x10, 0, 1),
	PIN_FIELD_BASE(128, 153, 0, 0x0240, 0x10, 0, 1),
};

static const struct mtk_pin_field_calc mt6739_pin_do_range[] = {
	PIN_FIELD_BASE(0, 31, 0, 0x0100, 0x10, 0, 1),
	PIN_FIELD_BASE(32, 63, 0, 0x0110, 0x10, 0, 1),
	PIN_FIELD_BASE(64, 95, 0, 0x0120, 0x10, 0, 1),
	PIN_FIELD_BASE(96, 127, 0, 0x0130, 0x10, 0, 1),
	PIN_FIELD_BASE(128, 153, 0, 0x0140, 0x10, 0, 1),
};

static const struct mtk_pin_field_calc mt6739_pin_ies_range[] = {
	PIN_FIELD_BASE(0, 5, 1, 0x0030, 0x10, 0, 1),
	PIN_FIELD_BASE(6, 9, 4, 0x0020, 0x10, 9, 1),
	PIN_FIELD_BASE(10, 11, 6, 0x0030, 0x10, 1, 1),
	PIN_FIELD_BASE(12, 13, 5, 0x0030, 0x10, 11, 1),
	PIN_FIELD_BASE(14, 15, 3, 0x0020, 0x10, 0, 1),
	PIN_FIELD_BASE(16, 16, 5, 0x0030, 0x10, 28, 1),
	PIN_FIELD_BASE(17, 17, 5, 0x0030, 0x10, 27, 1),
	PIN_FIELD_BASE(18, 19, 5, 0x0030, 0x10, 29, 1),
	PIN_FIELD_BASE(20, 21, 6, 0x0030, 0x10, 21, 1),
	PIN_FIELD_BASE(22, 23, 4, 0x0020, 0x10, 26, 1),
	PIN_FIELD_BASE(24, 25, 4, 0x0020, 0x10, 14, 1),
	PIN_FIELD_BASE(26, 26, 4, 0x0020, 0x10, 13, 1),
	PIN_FIELD_BASE(27, 30, 5, 0x0030, 0x10, 13, 1),
	PIN_FIELD_BASE(31, 31, 6, 0x0030, 0x10, 17, 1),
	PIN_FIELD_BASE(32, 32, 6, 0x0030, 0x10, 16, 1),
	PIN_FIELD_BASE(33, 34, 6, 0x0030, 0x10, 18, 1),
	PIN_FIELD_BASE(35, 37, 7, 0x0030, 0x10, 0, 1),
	PIN_FIELD_BASE(38, 38, 1, 0x0030, 0x10, 7, 1),
	PIN_FIELD_BASE(39, 39, 1, 0x0030, 0x10, 16, 1),
	PIN_FIELD_BASE(40, 40, 1, 0x0030, 0x10, 6, 1),
	PIN_FIELD_BASE(41, 48, 1, 0x0030, 0x10, 8, 1),
	PIN_FIELD_BASE(49, 49, 1, 0x0030, 0x10, 17, 1),
	PIN_FIELD_BASE(50, 50, 1, 0x0030, 0x10, 20, 1),
	PIN_FIELD_BASE(51, 51, 1, 0x0030, 0x10, 19, 1),
	PIN_FIELD_BASE(52, 52, 1, 0x0030, 0x10, 22, 1),
	PIN_FIELD_BASE(53, 53, 1, 0x0030, 0x10, 18, 1),
	PIN_FIELD_BASE(54, 54, 1, 0x0030, 0x10, 21, 1),
	PIN_FIELD_BASE(55, 55, 2, 0x0010, 0x10, 15, 1),
	PIN_FIELD_BASE(56, 57, 2, 0x0010, 0x10, 13, 1),
	PIN_FIELD_BASE(58, 59, 2, 0x0010, 0x10, 10, 1),
	PIN_FIELD_BASE(60, 61, 2, 0x0010, 0x10, 8, 1),
	PIN_FIELD_BASE(62, 62, 2, 0x0010, 0x10, 7, 1),
	PIN_FIELD_BASE(63, 64, 2, 0x0010, 0x10, 4, 1),
	PIN_FIELD_BASE(65, 65, 2, 0x0010, 0x10, 1, 1),
	PIN_FIELD_BASE(66, 66, 2, 0x0010, 0x10, 6, 1),
	PIN_FIELD_BASE(67, 68, 2, 0x0010, 0x10, 2, 1),
	PIN_FIELD_BASE(69, 69, 2, 0x0010, 0x10, 0, 1),
	PIN_FIELD_BASE(70, 70, 2, 0x0010, 0x10, 12, 1),
	PIN_FIELD_BASE(71, 71, 3, 0x0020, 0x10, 3, 1),
	PIN_FIELD_BASE(72, 72, 3, 0x0020, 0x10, 2, 1),
	PIN_FIELD_BASE(73, 77, 3, 0x0020, 0x10, 4, 1),
	PIN_FIELD_BASE(78, 78, 3, 0x0020, 0x10, 10, 1),
	PIN_FIELD_BASE(79, 79, 3, 0x0020, 0x10, 9, 1),
	PIN_FIELD_BASE(80, 80, 3, 0x0020, 0x10, 11, 1),
	PIN_FIELD_BASE(81, 81, 3, 0x0020, 0x10, 13, 1),
	PIN_FIELD_BASE(82, 82, 3, 0x0020, 0x10, 12, 1),
	PIN_FIELD_BASE(83, 83, 4, 0x0020, 0x10, 16, 1),
	PIN_FIELD_BASE(84, 84, 4, 0x0020, 0x10, 8, 1),
	PIN_FIELD_BASE(85, 85, 4, 0x0020, 0x10, 7, 1),
	PIN_FIELD_BASE(86, 87, 4, 0x0020, 0x10, 24, 1),
	PIN_FIELD_BASE(88, 88, 4, 0x0020, 0x10, 23, 1),
	PIN_FIELD_BASE(89, 89, 4, 0x0020, 0x10, 22, 1),
	PIN_FIELD_BASE(90, 91, 4, 0x0020, 0x10, 5, 1),
	PIN_FIELD_BASE(92, 96, 4, 0x0020, 0x10, 0, 1),
	PIN_FIELD_BASE(97, 97, 4, 0x0020, 0x10, 21, 1),
	PIN_FIELD_BASE(98, 98, 4, 0x0020, 0x10, 17, 1),
	PIN_FIELD_BASE(99, 99, 4, 0x0020, 0x10, 20, 1),
	PIN_FIELD_BASE(100, 100, 4, 0x0020, 0x10, 19, 1),
	PIN_FIELD_BASE(101, 101, 4, 0x0020, 0x10, 18, 1),
	PIN_FIELD_BASE(102, 109, 5, 0x0030, 0x10, 3, 1),
	PIN_FIELD_BASE(110, 112, 5, 0x0030, 0x10, 0, 1),
	PIN_FIELD_BASE(113, 114, 5, 0x0030, 0x10, 25, 1),
	PIN_FIELD_BASE(115, 116, 5, 0x0030, 0x10, 23, 1),
	PIN_FIELD_BASE(117, 117, 5, 0x0030, 0x10, 18, 1),
	PIN_FIELD_BASE(118, 118, 5, 0x0030, 0x10, 20, 1),
	PIN_FIELD_BASE(119, 119, 5, 0x0030, 0x10, 22, 1),
	PIN_FIELD_BASE(120, 120, 5, 0x0030, 0x10, 17, 1),
	PIN_FIELD_BASE(121, 121, 5, 0x0030, 0x10, 19, 1),
	PIN_FIELD_BASE(122, 122, 5, 0x0030, 0x10, 21, 1),
	PIN_FIELD_BASE(123, 124, 7, 0x0030, 0x10, 3, 1),
	PIN_FIELD_BASE(125, 125, 7, 0x0030, 0x10, 6, 1),
	PIN_FIELD_BASE(126, 126, 6, 0x0030, 0x10, 0, 1),
	PIN_FIELD_BASE(127, 127, 7, 0x0030, 0x10, 5, 1),
	PIN_FIELD_BASE(128, 128, 6, 0x0030, 0x10, 4, 1),
	PIN_FIELD_BASE(129, 129, 6, 0x0030, 0x10, 3, 1),
	PIN_FIELD_BASE(130, 130, 6, 0x0030, 0x10, 6, 1),
	PIN_FIELD_BASE(131, 131, 6, 0x0030, 0x10, 5, 1),
	PIN_FIELD_BASE(132, 132, 6, 0x0030, 0x10, 8, 1),
	PIN_FIELD_BASE(133, 133, 6, 0x0030, 0x10, 7, 1),
	PIN_FIELD_BASE(134, 134, 6, 0x0030, 0x10, 10, 1),
	PIN_FIELD_BASE(135, 135, 6, 0x0030, 0x10, 9, 1),
	PIN_FIELD_BASE(136, 136, 6, 0x0030, 0x10, 20, 1),
	PIN_FIELD_BASE(137, 137, 6, 0x0030, 0x10, 15, 1),
	PIN_FIELD_BASE(138, 140, 6, 0x0030, 0x10, 12, 1),
	PIN_FIELD_BASE(141, 141, 6, 0x0030, 0x10, 11, 1),
	PIN_FIELD_BASE(142, 142, 7, 0x0030, 0x10, 8, 1),
	PIN_FIELD_BASE(143, 143, 7, 0x0030, 0x10, 7, 1),
	PIN_FIELD_BASE(144, 147, 7, 0x0030, 0x10, 15, 1),
	PIN_FIELD_BASE(148, 153, 7, 0x0030, 0x10, 9, 1),
};

static const struct mtk_pin_field_calc mt6739_pin_smt_range[] = {
	PINS_FIELD_BASE(0, 5, 1, 0x0090, 0x10, 0, 1),
	PINS_FIELD_BASE(6, 9, 4, 0x0060, 0x10, 4, 1),
	PINS_FIELD_BASE(10, 11, 6, 0x0080, 0x10, 1, 1),
	PINS_FIELD_BASE(12, 13, 5, 0x00b0, 0x10, 3, 1),
	PINS_FIELD_BASE(14, 15, 3, 0x0090, 0x10, 0, 1),
	PIN_FIELD_BASE(16, 17, 5, 0x00b0, 0x10, 12, 1),
	PINS_FIELD_BASE(18, 19, 5, 0x00b0, 0x10, 12, 1),
	PINS_FIELD_BASE(20, 21, 6, 0x0080, 0x10, 11, 1),
	PINS_FIELD_BASE(22, 23, 4, 0x0060, 0x10, 13, 1),
	PIN_FIELD_BASE(24, 25, 4, 0x0060, 0x10, 6, 1),
	PIN_FIELD_BASE(26, 26, 4, 0x0060, 0x10, 5, 1),
	PIN_FIELD_BASE(27, 30, 5, 0x00b0, 0x10, 4, 1),
	PIN_FIELD_BASE(31, 32, 6, 0x0080, 0x10, 8, 1),
	PINS_FIELD_BASE(33, 34, 6, 0x0080, 0x10, 8, 1),
	PINS_FIELD_BASE(35, 36, 7, 0x0080, 0x10, 0, 1),
	PIN_FIELD_BASE(37, 37, 7, 0x0080, 0x10, 1, 1),
	PIN_FIELD_BASE(38, 38, 1, 0x0090, 0x10, 2, 1),
	PIN_FIELD_BASE(39, 39, 1, 0x0090, 0x10, 4, 1),
	PIN_FIELD_BASE(40, 40, 1, 0x0090, 0x10, 1, 1),
	PINS_FIELD_BASE(41, 48, 1, 0x0090, 0x10, 3, 1),
	PIN_FIELD_BASE(49, 49, 1, 0x0090, 0x10, 5, 1),
	PINS_FIELD_BASE(50, 54, 1, 0x0090, 0x10, 6, 1),
	PIN_FIELD_BASE(55, 55, 2, 0x0050, 0x10, 10, 1),
	PINS_FIELD_BASE(56, 57, 2, 0x0050, 0x10, 9, 1),
	PINS_FIELD_BASE(58, 59, 2, 0x0050, 0x10, 6, 1),
	PIN_FIELD_BASE(60, 60, 2, 0x0050, 0x10, 7, 1),
	PIN_FIELD_BASE(61, 61, 2, 0x0050, 0x10, 6, 1),
	PIN_FIELD_BASE(62, 62, 2, 0x0050, 0x10, 5, 1),
	PINS_FIELD_BASE(63, 64, 2, 0x0050, 0x10, 3, 1),
	PIN_FIELD_BASE(65, 65, 2, 0x0050, 0x10, 1, 1),
	PIN_FIELD_BASE(66, 66, 2, 0x0050, 0x10, 4, 1),
	PINS_FIELD_BASE(67, 68, 2, 0x0050, 0x10, 2, 1),
	PIN_FIELD_BASE(69, 69, 2, 0x0050, 0x10, 0, 1),
	PIN_FIELD_BASE(70, 70, 2, 0x0050, 0x10, 8, 1),
	PIN_FIELD_BASE(71, 71, 3, 0x0090, 0x10, 2, 1),
	PIN_FIELD_BASE(72, 72, 3, 0x0090, 0x10, 1, 1),
	PINS_FIELD_BASE(73, 76, 3, 0x0090, 0x10, 3, 1),
	PIN_FIELD_BASE(77, 77, 3, 0x0090, 0x10, 5, 1),
	PINS_FIELD_BASE(78, 79, 3, 0x0090, 0x10, 4, 1),
	PIN_FIELD_BASE(80, 80, 3, 0x0090, 0x10, 7, 1),
	PINS_FIELD_BASE(81, 82, 3, 0x0090, 0x10, 6, 1),
	PIN_FIELD_BASE(83, 83, 4, 0x0060, 0x10, 8, 1),
	PIN_FIELD_BASE(84, 84, 4, 0x0060, 0x10, 3, 1),
	PIN_FIELD_BASE(85, 85, 4, 0x0060, 0x10, 2, 1),
	PINS_FIELD_BASE(86, 87, 4, 0x0060, 0x10, 12, 1),
	PINS_FIELD_BASE(88, 89, 4, 0x0060, 0x10, 11, 1),
	PINS_FIELD_BASE(90, 93, 4, 0x0060, 0x10, 0, 1),
	PINS_FIELD_BASE(94, 96, 4, 0x0060, 0x10, 1, 1),
	PIN_FIELD_BASE(97, 97, 4, 0x0060, 0x10, 10, 1),
	PIN_FIELD_BASE(98, 98, 4, 0x0060, 0x10, 9, 1),
	PINS_FIELD_BASE(99, 101, 4, 0x0060, 0x10, 10, 1),
	PINS_FIELD_BASE(102, 105, 5, 0x00b0, 0x10, 1, 1),
	PINS_FIELD_BASE(106, 109, 5, 0x00b0, 0x10, 2, 1),
	PINS_FIELD_BASE(110, 112, 5, 0x00b0, 0x10, 0, 1),
	PINS_FIELD_BASE(113, 114, 5, 0x00b0, 0x10, 11, 1),
	PINS_FIELD_BASE(115, 116, 5, 0x00b0, 0x10, 10, 1),
	PINS_FIELD_BASE(117, 119, 5, 0x00b0, 0x10, 9, 1),
	PINS_FIELD_BASE(120, 122, 5, 0x00b0, 0x10, 8, 1),
	PINS_FIELD_BASE(123, 124, 7, 0x0080, 0x10, 2, 1),
	PIN_FIELD_BASE(125, 125, 7, 0x0080, 0x10, 4, 1),
	PIN_FIELD_BASE(126, 126, 6, 0x0080, 0x10, 0, 1),
	PIN_FIELD_BASE(127, 127, 7, 0x0080, 0x10, 3, 1),
	PINS_FIELD_BASE(128, 129, 6, 0x0080, 0x10, 2, 1),
	PINS_FIELD_BASE(130, 131, 6, 0x0080, 0x10, 3, 1),
	PINS_FIELD_BASE(132, 133, 6, 0x0080, 0x10, 4, 1),
	PINS_FIELD_BASE(134, 135, 6, 0x0080, 0x10, 5, 1),
	PIN_FIELD_BASE(136, 136, 6, 0x0080, 0x10, 10, 1),
	PIN_FIELD_BASE(137, 137, 6, 0x0080, 0x10, 7, 1),
	PIN_FIELD_BASE(138, 138, 6, 0x0080, 0x10, 6, 1),
	PINS_FIELD_BASE(139, 141, 6, 0x0080, 0x10, 7, 1),
	PINS_FIELD_BASE(142, 143, 7, 0x0080, 0x10, 5, 1),
	PIN_FIELD_BASE(144, 144, 7, 0x0080, 0x10, 8, 1),
	PIN_FIELD_BASE(145, 145, 7, 0x0080, 0x10, 7, 1),
	PINS_FIELD_BASE(146, 147, 7, 0x0080, 0x10, 8, 1),
	PINS_FIELD_BASE(148, 153, 7, 0x0080, 0x10, 6, 1),
};

static const struct mtk_pin_field_calc mt6739_pin_pu_range[] = {
	PIN_FIELD_BASE(6, 9, 4, 0x0040, 0x10, 9, 1),
	PIN_FIELD_BASE(10, 11, 6, 0x0050, 0x10, 1, 1),
	PIN_FIELD_BASE(12, 13, 5, 0x0060, 0x10, 11, 1),
	PIN_FIELD_BASE(16, 16, 5, 0x0060, 0x10, 22, 1),
	PIN_FIELD_BASE(17, 17, 5, 0x0060, 0x10, 21, 1),
	PIN_FIELD_BASE(18, 19, 5, 0x0060, 0x10, 23, 1),
	PIN_FIELD_BASE(20, 21, 6, 0x0050, 0x10, 22, 1),
	PIN_FIELD_BASE(22, 23, 4, 0x0040, 0x10, 26, 1),
	PIN_FIELD_BASE(24, 25, 4, 0x0040, 0x10, 14, 1),
	PIN_FIELD_BASE(26, 26, 4, 0x0040, 0x10, 13, 1),
	PIN_FIELD_BASE(27, 30, 5, 0x0060, 0x10, 13, 1),
	PIN_FIELD_BASE(31, 31, 6, 0x0050, 0x10, 17, 1),
	PIN_FIELD_BASE(32, 32, 6, 0x0050, 0x10, 16, 1),
	PIN_FIELD_BASE(33, 34, 6, 0x0050, 0x10, 18, 1),
	PIN_FIELD_BASE(35, 37, 7, 0x0050, 0x10, 0, 1),
	PIN_FIELD_BASE(55, 55, 2, 0x0030, 0x10, 16, 1),
	PIN_FIELD_BASE(56, 57, 2, 0x0030, 0x10, 13, 1),
	PIN_FIELD_BASE(58, 59, 2, 0x0030, 0x10, 10, 1),
	PIN_FIELD_BASE(60, 61, 2, 0x0030, 0x10, 8, 1),
	PIN_FIELD_BASE(62, 62, 2, 0x0030, 0x10, 7, 1),
	PIN_FIELD_BASE(63, 64, 2, 0x0030, 0x10, 4, 1),
	PIN_FIELD_BASE(65, 65, 2, 0x0030, 0x10, 1, 1),
	PIN_FIELD_BASE(66, 66, 2, 0x0030, 0x10, 6, 1),
	PIN_FIELD_BASE(67, 68, 2, 0x0030, 0x10, 2, 1),
	PIN_FIELD_BASE(69, 69, 2, 0x0030, 0x10, 0, 1),
	PIN_FIELD_BASE(70, 70, 2, 0x0030, 0x10, 12, 1),
	PIN_FIELD_BASE(83, 83, 4, 0x0040, 0x10, 16, 1),
	PIN_FIELD_BASE(84, 84, 4, 0x0040, 0x10, 8, 1),
	PIN_FIELD_BASE(85, 85, 4, 0x0040, 0x10, 7, 1),
	PIN_FIELD_BASE(86, 87, 4, 0x0040, 0x10, 24, 1),
	PIN_FIELD_BASE(88, 88, 4, 0x0040, 0x10, 23, 1),
	PIN_FIELD_BASE(89, 89, 4, 0x0040, 0x10, 22, 1),
	PIN_FIELD_BASE(90, 91, 4, 0x0040, 0x10, 5, 1),
	PIN_FIELD_BASE(92, 96, 4, 0x0040, 0x10, 0, 1),
	PIN_FIELD_BASE(97, 97, 4, 0x0040, 0x10, 21, 1),
	PIN_FIELD_BASE(98, 98, 4, 0x0040, 0x10, 17, 1),
	PIN_FIELD_BASE(99, 99, 4, 0x0040, 0x10, 20, 1),
	PIN_FIELD_BASE(100, 100, 4, 0x0040, 0x10, 19, 1),
	PIN_FIELD_BASE(101, 101, 4, 0x0040, 0x10, 18, 1),
	PIN_FIELD_BASE(102, 109, 5, 0x0060, 0x10, 3, 1),
	PIN_FIELD_BASE(110, 112, 5, 0x0060, 0x10, 0, 1),
	PIN_FIELD_BASE(113, 114, 5, 0x0060, 0x10, 19, 1),
	PIN_FIELD_BASE(115, 116, 5, 0x0060, 0x10, 17, 1),
	PIN_FIELD_BASE(123, 124, 7, 0x0050, 0x10, 3, 1),
	PIN_FIELD_BASE(125, 125, 7, 0x0050, 0x10, 6, 1),
	PIN_FIELD_BASE(126, 126, 6, 0x0050, 0x10, 0, 1),
	PIN_FIELD_BASE(127, 127, 7, 0x0050, 0x10, 5, 1),
	PIN_FIELD_BASE(128, 128, 6, 0x0050, 0x10, 4, 1),
	PIN_FIELD_BASE(129, 129, 6, 0x0050, 0x10, 3, 1),
	PIN_FIELD_BASE(130, 130, 6, 0x0050, 0x10, 6, 1),
	PIN_FIELD_BASE(131, 131, 6, 0x0050, 0x10, 5, 1),
	PIN_FIELD_BASE(132, 132, 6, 0x0050, 0x10, 8, 1),
	PIN_FIELD_BASE(133, 133, 6, 0x0050, 0x10, 7, 1),
	PIN_FIELD_BASE(134, 134, 6, 0x0050, 0x10, 10, 1),
	PIN_FIELD_BASE(135, 135, 6, 0x0050, 0x10, 9, 1),
	PIN_FIELD_BASE(136, 136, 6, 0x0050, 0x10, 20, 1),
	PIN_FIELD_BASE(137, 137, 6, 0x0050, 0x10, 15, 1),
	PIN_FIELD_BASE(138, 140, 6, 0x0050, 0x10, 12, 1),
	PIN_FIELD_BASE(141, 141, 6, 0x0050, 0x10, 11, 1),
	PIN_FIELD_BASE(142, 142, 7, 0x0050, 0x10, 8, 1),
	PIN_FIELD_BASE(143, 143, 7, 0x0050, 0x10, 7, 1),
	PIN_FIELD_BASE(144, 147, 7, 0x0050, 0x10, 15, 1),
	PIN_FIELD_BASE(148, 153, 7, 0x0050, 0x10, 9, 1),
};

static const struct mtk_pin_field_calc mt6739_pin_pd_range[] = {
	PIN_FIELD_BASE(6, 9, 4, 0x0030, 0x10, 9, 1),
	PIN_FIELD_BASE(10, 11, 6, 0x0040, 0x10, 1, 1),
	PIN_FIELD_BASE(12, 13, 5, 0x0040, 0x10, 11, 1),
	PIN_FIELD_BASE(16, 16, 5, 0x0040, 0x10, 22, 1),
	PIN_FIELD_BASE(17, 17, 5, 0x0040, 0x10, 21, 1),
	PIN_FIELD_BASE(18, 19, 5, 0x0040, 0x10, 23, 1),
	PIN_FIELD_BASE(20, 21, 6, 0x0040, 0x10, 22, 1),
	PIN_FIELD_BASE(22, 23, 4, 0x0030, 0x10, 26, 1),
	PIN_FIELD_BASE(24, 25, 4, 0x0030, 0x10, 14, 1),
	PIN_FIELD_BASE(26, 26, 4, 0x0030, 0x10, 13, 1),
	PIN_FIELD_BASE(27, 30, 5, 0x0040, 0x10, 13, 1),
	PIN_FIELD_BASE(31, 31, 6, 0x0040, 0x10, 17, 1),
	PIN_FIELD_BASE(32, 32, 6, 0x0040, 0x10, 16, 1),
	PIN_FIELD_BASE(33, 34, 6, 0x0040, 0x10, 18, 1),
	PIN_FIELD_BASE(35, 37, 7, 0x0040, 0x10, 0, 1),
	PIN_FIELD_BASE(55, 55, 2, 0x0020, 0x10, 16, 1),
	PIN_FIELD_BASE(56, 57, 2, 0x0020, 0x10, 13, 1),
	PIN_FIELD_BASE(58, 59, 2, 0x0020, 0x10, 10, 1),
	PIN_FIELD_BASE(60, 61, 2, 0x0020, 0x10, 8, 1),
	PIN_FIELD_BASE(62, 62, 2, 0x0020, 0x10, 7, 1),
	PIN_FIELD_BASE(63, 64, 2, 0x0020, 0x10, 4, 1),
	PIN_FIELD_BASE(65, 65, 2, 0x0020, 0x10, 1, 1),
	PIN_FIELD_BASE(66, 66, 2, 0x0020, 0x10, 6, 1),
	PIN_FIELD_BASE(67, 68, 2, 0x0020, 0x10, 2, 1),
	PIN_FIELD_BASE(69, 69, 2, 0x0020, 0x10, 0, 1),
	PIN_FIELD_BASE(70, 70, 2, 0x0020, 0x10, 12, 1),
	PIN_FIELD_BASE(83, 83, 4, 0x0030, 0x10, 16, 1),
	PIN_FIELD_BASE(84, 84, 4, 0x0030, 0x10, 8, 1),
	PIN_FIELD_BASE(85, 85, 4, 0x0030, 0x10, 7, 1),
	PIN_FIELD_BASE(86, 87, 4, 0x0030, 0x10, 24, 1),
	PIN_FIELD_BASE(88, 88, 4, 0x0030, 0x10, 23, 1),
	PIN_FIELD_BASE(89, 89, 4, 0x0030, 0x10, 22, 1),
	PIN_FIELD_BASE(90, 91, 4, 0x0030, 0x10, 5, 1),
	PIN_FIELD_BASE(92, 96, 4, 0x0030, 0x10, 0, 1),
	PIN_FIELD_BASE(97, 97, 4, 0x0030, 0x10, 21, 1),
	PIN_FIELD_BASE(98, 98, 4, 0x0030, 0x10, 17, 1),
	PIN_FIELD_BASE(99, 99, 4, 0x0030, 0x10, 20, 1),
	PIN_FIELD_BASE(100, 100, 4, 0x0030, 0x10, 19, 1),
	PIN_FIELD_BASE(101, 101, 4, 0x0030, 0x10, 18, 1),
	PIN_FIELD_BASE(102, 109, 5, 0x0040, 0x10, 3, 1),
	PIN_FIELD_BASE(110, 112, 5, 0x0040, 0x10, 0, 1),
	PIN_FIELD_BASE(113, 114, 5, 0x0040, 0x10, 19, 1),
	PIN_FIELD_BASE(115, 116, 5, 0x0040, 0x10, 17, 1),
	PIN_FIELD_BASE(123, 124, 7, 0x0040, 0x10, 3, 1),
	PIN_FIELD_BASE(125, 125, 7, 0x0040, 0x10, 6, 1),
	PIN_FIELD_BASE(126, 126, 6, 0x0040, 0x10, 0, 1),
	PIN_FIELD_BASE(127, 127, 7, 0x0040, 0x10, 5, 1),
	PIN_FIELD_BASE(128, 128, 6, 0x0040, 0x10, 4, 1),
	PIN_FIELD_BASE(129, 129, 6, 0x0040, 0x10, 3, 1),
	PIN_FIELD_BASE(130, 130, 6, 0x0040, 0x10, 6, 1),
	PIN_FIELD_BASE(131, 131, 6, 0x0040, 0x10, 5, 1),
	PIN_FIELD_BASE(132, 132, 6, 0x0040, 0x10, 8, 1),
	PIN_FIELD_BASE(133, 133, 6, 0x0040, 0x10, 7, 1),
	PIN_FIELD_BASE(134, 134, 6, 0x0040, 0x10, 10, 1),
	PIN_FIELD_BASE(135, 135, 6, 0x0040, 0x10, 9, 1),
	PIN_FIELD_BASE(136, 136, 6, 0x0040, 0x10, 20, 1),
	PIN_FIELD_BASE(137, 137, 6, 0x0040, 0x10, 15, 1),
	PIN_FIELD_BASE(138, 140, 6, 0x0040, 0x10, 12, 1),
	PIN_FIELD_BASE(141, 141, 6, 0x0040, 0x10, 11, 1),
	PIN_FIELD_BASE(142, 142, 7, 0x0040, 0x10, 8, 1),
	PIN_FIELD_BASE(143, 143, 7, 0x0040, 0x10, 7, 1),
	PIN_FIELD_BASE(144, 147, 7, 0x0040, 0x10, 15, 1),
	PIN_FIELD_BASE(148, 153, 7, 0x0040, 0x10, 9, 1),
};

static const struct mtk_pin_field_calc mt6739_pin_pupd_range[] = {
	PIN_FIELD_BASE(0, 5, 1, 0x0040, 0x10, 0, 1),
	PIN_FIELD_BASE(14, 15, 3, 0x0030, 0x10, 0, 1),
	PIN_FIELD_BASE(38, 38, 1, 0x0040, 0x10, 7, 1),
	PIN_FIELD_BASE(39, 39, 1, 0x0040, 0x10, 16, 1),
	PIN_FIELD_BASE(40, 40, 1, 0x0040, 0x10, 6, 1),
	PIN_FIELD_BASE(41, 48, 1, 0x0040, 0x10, 8, 1),
	PIN_FIELD_BASE(49, 49, 1, 0x0040, 0x10, 17, 1),
	PIN_FIELD_BASE(50, 50, 1, 0x0040, 0x10, 20, 1),
	PIN_FIELD_BASE(51, 51, 1, 0x0040, 0x10, 19, 1),
	PIN_FIELD_BASE(52, 52, 1, 0x0040, 0x10, 22, 1),
	PIN_FIELD_BASE(53, 53, 1, 0x0040, 0x10, 18, 1),
	PIN_FIELD_BASE(54, 54, 1, 0x0040, 0x10, 21, 1),
	PIN_FIELD_BASE(71, 71, 3, 0x0030, 0x10, 3, 1),
	PIN_FIELD_BASE(72, 72, 3, 0x0030, 0x10, 2, 1),
	PIN_FIELD_BASE(73, 77, 3, 0x0030, 0x10, 4, 1),
	PIN_FIELD_BASE(78, 78, 3, 0x0030, 0x10, 10, 1),
	PIN_FIELD_BASE(79, 79, 3, 0x0030, 0x10, 9, 1),
	PIN_FIELD_BASE(80, 80, 3, 0x0030, 0x10, 11, 1),
	PIN_FIELD_BASE(81, 81, 3, 0x0030, 0x10, 13, 1),
	PIN_FIELD_BASE(82, 82, 3, 0x0030, 0x10, 12, 1),
	PIN_FIELD_BASE(117, 117, 5, 0x0050, 0x10, 1, 1),
	PIN_FIELD_BASE(118, 118, 5, 0x0050, 0x10, 3, 1),
	PIN_FIELD_BASE(119, 119, 5, 0x0050, 0x10, 5, 1),
	PIN_FIELD_BASE(120, 120, 5, 0x0050, 0x10, 0, 1),
	PIN_FIELD_BASE(121, 121, 5, 0x0050, 0x10, 2, 1),
	PIN_FIELD_BASE(122, 122, 5, 0x0050, 0x10, 4, 1),
};

static const struct mtk_pin_field_calc mt6739_pin_r0_range[] = {
	PIN_FIELD_BASE(0, 5, 1, 0x0050, 0x10, 0, 1),
	PIN_FIELD_BASE(14, 15, 3, 0x0050, 0x10, 0, 1),
	PIN_FIELD_BASE(38, 38, 1, 0x0050, 0x10, 7, 1),
	PIN_FIELD_BASE(39, 39, 1, 0x0050, 0x10, 16, 1),
	PIN_FIELD_BASE(40, 40, 1, 0x0050, 0x10, 6, 1),
	PIN_FIELD_BASE(41, 48, 1, 0x0050, 0x10, 8, 1),
	PIN_FIELD_BASE(49, 49, 1, 0x0050, 0x10, 17, 1),
	PIN_FIELD_BASE(50, 50, 1, 0x0050, 0x10, 20, 1),
	PIN_FIELD_BASE(51, 51, 1, 0x0050, 0x10, 19, 1),
	PIN_FIELD_BASE(52, 52, 1, 0x0050, 0x10, 22, 1),
	PIN_FIELD_BASE(53, 53, 1, 0x0050, 0x10, 18, 1),
	PIN_FIELD_BASE(54, 54, 1, 0x0050, 0x10, 21, 1),
	PIN_FIELD_BASE(71, 71, 3, 0x0050, 0x10, 3, 1),
	PIN_FIELD_BASE(72, 72, 3, 0x0050, 0x10, 2, 1),
	PIN_FIELD_BASE(73, 77, 3, 0x0050, 0x10, 4, 1),
	PIN_FIELD_BASE(78, 78, 3, 0x0050, 0x10, 10, 1),
	PIN_FIELD_BASE(79, 79, 3, 0x0050, 0x10, 9, 1),
	PIN_FIELD_BASE(80, 80, 3, 0x0050, 0x10, 11, 1),
	PIN_FIELD_BASE(81, 81, 3, 0x0050, 0x10, 13, 1),
	PIN_FIELD_BASE(82, 82, 3, 0x0050, 0x10, 12, 1),
	PIN_FIELD_BASE(117, 117, 5, 0x0070, 0x10, 1, 1),
	PIN_FIELD_BASE(118, 118, 5, 0x0070, 0x10, 3, 1),
	PIN_FIELD_BASE(119, 119, 5, 0x0070, 0x10, 5, 1),
	PIN_FIELD_BASE(120, 120, 5, 0x0070, 0x10, 0, 1),
	PIN_FIELD_BASE(121, 121, 5, 0x0070, 0x10, 2, 1),
	PIN_FIELD_BASE(122, 122, 5, 0x0070, 0x10, 4, 1),
};

static const struct mtk_pin_field_calc mt6739_pin_r1_range[] = {
	PIN_FIELD_BASE(0, 5, 1, 0x0060, 0x10, 0, 1),
	PIN_FIELD_BASE(14, 15, 3, 0x0060, 0x10, 0, 1),
	PIN_FIELD_BASE(38, 38, 1, 0x0060, 0x10, 7, 1),
	PIN_FIELD_BASE(39, 39, 1, 0x0060, 0x10, 16, 1),
	PIN_FIELD_BASE(40, 40, 1, 0x0060, 0x10, 6, 1),
	PIN_FIELD_BASE(41, 48, 1, 0x0060, 0x10, 8, 1),
	PIN_FIELD_BASE(49, 49, 1, 0x0060, 0x10, 17, 1),
	PIN_FIELD_BASE(50, 50, 1, 0x0060, 0x10, 20, 1),
	PIN_FIELD_BASE(51, 51, 1, 0x0060, 0x10, 19, 1),
	PIN_FIELD_BASE(52, 52, 1, 0x0060, 0x10, 22, 1),
	PIN_FIELD_BASE(53, 53, 1, 0x0060, 0x10, 18, 1),
	PIN_FIELD_BASE(54, 54, 1, 0x0060, 0x10, 21, 1),
	PIN_FIELD_BASE(71, 71, 3, 0x0060, 0x10, 3, 1),
	PIN_FIELD_BASE(72, 72, 3, 0x0060, 0x10, 2, 1),
	PIN_FIELD_BASE(73, 77, 3, 0x0060, 0x10, 4, 1),
	PIN_FIELD_BASE(78, 78, 3, 0x0060, 0x10, 10, 1),
	PIN_FIELD_BASE(79, 79, 3, 0x0060, 0x10, 9, 1),
	PIN_FIELD_BASE(80, 80, 3, 0x0060, 0x10, 11, 1),
	PIN_FIELD_BASE(81, 81, 3, 0x0060, 0x10, 13, 1),
	PIN_FIELD_BASE(82, 82, 3, 0x0060, 0x10, 12, 1),
	PIN_FIELD_BASE(117, 117, 5, 0x0080, 0x10, 1, 1),
	PIN_FIELD_BASE(118, 118, 5, 0x0080, 0x10, 3, 1),
	PIN_FIELD_BASE(119, 119, 5, 0x0080, 0x10, 5, 1),
	PIN_FIELD_BASE(120, 120, 5, 0x0080, 0x10, 0, 1),
	PIN_FIELD_BASE(121, 121, 5, 0x0080, 0x10, 2, 1),
	PIN_FIELD_BASE(122, 122, 5, 0x0080, 0x10, 4, 1),
};

static const struct mtk_pin_field_calc mt6739_pin_drv_range[] = {
	PIN_FIELD_BASE(0, 5, 1, 0x0000, 0x10, 0, 3),
	PIN_FIELD_BASE(6, 9, 4, 0x0000, 0x10, 8, 2),
	PINS_FIELD_BASE(10, 11, 6, 0x0000, 0x10, 2, 2),
	PIN_FIELD_BASE(12, 13, 5, 0x0000, 0x10, 6, 5),
	PINS_FIELD_BASE(14, 15, 3, 0x0000, 0x10, 0, 3),
	PIN_FIELD_BASE(16, 17, 5, 0x0010, 0x10, 8, 2),
	PINS_FIELD_BASE(18, 19, 5, 0x0010, 0x10, 8, 2),
	PINS_FIELD_BASE(20, 21, 6, 0x0010, 0x10, 25, 2),
	PIN_FIELD_BASE(22, 23, 4, 0x0010, 0x10, 2, 2),
	PIN_FIELD_BASE(24, 25, 4, 0x0000, 0x10, 18, 2),
	PIN_FIELD_BASE(26, 26, 4, 0x0000, 0x10, 16, 2),
	PIN_FIELD_BASE(27, 30, 5, 0x0000, 0x10, 16, 2),
	PIN_FIELD_BASE(31, 32, 6, 0x0010, 0x10, 19, 2),
	PINS_FIELD_BASE(33, 34, 6, 0x0010, 0x10, 19, 2),
	PIN_FIELD_BASE(35, 36, 7, 0x0000, 0x10, 0, 5),
	PIN_FIELD_BASE(37, 37, 7, 0x0000, 0x10, 10, 2),
	PIN_FIELD_BASE(38, 38, 1, 0x0000, 0x10, 21, 3),
	PIN_FIELD_BASE(39, 39, 1, 0x0010, 0x10, 18, 3),
	PIN_FIELD_BASE(40, 40, 1, 0x0000, 0x10, 18, 3),
	PIN_FIELD_BASE(41, 42, 1, 0x0000, 0x10, 24, 3),
	PIN_FIELD_BASE(43, 48, 1, 0x0010, 0x10, 0, 3),
	PIN_FIELD_BASE(49, 49, 1, 0x0010, 0x10, 21, 3),
	PIN_FIELD_BASE(50, 50, 1, 0x0020, 0x10, 0, 3),
	PIN_FIELD_BASE(51, 51, 1, 0x0010, 0x10, 27, 3),
	PIN_FIELD_BASE(52, 52, 1, 0x0020, 0x10, 6, 3),
	PIN_FIELD_BASE(53, 53, 1, 0x0010, 0x10, 24, 3),
	PIN_FIELD_BASE(54, 54, 1, 0x0020, 0x10, 3, 3),
	PIN_FIELD_BASE(55, 55, 2, 0x0000, 0x10, 24, 2),
	PINS_FIELD_BASE(56, 57, 2, 0x0000, 0x10, 22, 2),
	PINS_FIELD_BASE(58, 59, 2, 0x0000, 0x10, 16, 2),
	PIN_FIELD_BASE(60, 60, 2, 0x0000, 0x10, 18, 2),
	PIN_FIELD_BASE(61, 61, 2, 0x0000, 0x10, 16, 2),
	PIN_FIELD_BASE(62, 62, 2, 0x0000, 0x10, 14, 2),
	PIN_FIELD_BASE(63, 64, 2, 0x0000, 0x10, 8, 2),
	PIN_FIELD_BASE(65, 65, 2, 0x0000, 0x10, 2, 2),
	PIN_FIELD_BASE(66, 66, 2, 0x0000, 0x10, 12, 2),
	PIN_FIELD_BASE(67, 68, 2, 0x0000, 0x10, 4, 2),
	PIN_FIELD_BASE(69, 69, 2, 0x0000, 0x10, 0, 2),
	PIN_FIELD_BASE(70, 70, 2, 0x0000, 0x10, 20, 2),
	PIN_FIELD_BASE(71, 71, 3, 0x0000, 0x10, 6, 3),
	PIN_FIELD_BASE(72, 72, 3, 0x0000, 0x10, 3, 3),
	PINS_FIELD_BASE(73, 76, 3, 0x0000, 0x10, 9, 3),
	PIN_FIELD_BASE(77, 77, 3, 0x0000, 0x10, 14, 2),
	PINS_FIELD_BASE(78, 79, 3, 0x0000, 0x10, 12, 2),
	PIN_FIELD_BASE(80, 80, 3, 0x0000, 0x10, 18, 2),
	PINS_FIELD_BASE(81, 82, 3, 0x0000, 0x10, 16, 2),
	PIN_FIELD_BASE(83, 83, 4, 0x0000, 0x10, 22, 2),
	PIN_FIELD_BASE(84, 84, 4, 0x0000, 0x10, 6, 2),
	PIN_FIELD_BASE(85, 85, 4, 0x0000, 0x10, 4, 2),
	PINS_FIELD_BASE(86, 87, 4, 0x0010, 0x10, 0, 2),
	PINS_FIELD_BASE(88, 89, 4, 0x0000, 0x10, 30, 2),
	PINS_FIELD_BASE(90, 93, 4, 0x0000, 0x10, 0, 2),
	PINS_FIELD_BASE(94, 96, 4, 0x0000, 0x10, 2, 2),
	PIN_FIELD_BASE(97, 97, 4, 0x0000, 0x10, 28, 2),
	PIN_FIELD_BASE(98, 98, 4, 0x0000, 0x10, 24, 2),
	PINS_FIELD_BASE(99, 101, 4, 0x0000, 0x10, 26, 2),
	PINS_FIELD_BASE(102, 105, 5, 0x0000, 0x10, 2, 2),
	PINS_FIELD_BASE(106, 109, 5, 0x0000, 0x10, 4, 2),
	PINS_FIELD_BASE(110, 112, 5, 0x0000, 0x10, 0, 2),
	PINS_FIELD_BASE(113, 114, 5, 0x0010, 0x10, 6, 2),
	PINS_FIELD_BASE(115, 116, 5, 0x0010, 0x10, 4, 2),
	PIN_FIELD_BASE(117, 117, 5, 0x0000, 0x10, 26, 2),
	PIN_FIELD_BASE(118, 118, 5, 0x0000, 0x10, 30, 2),
	PIN_FIELD_BASE(119, 119, 5, 0x0010, 0x10, 2, 2),
	PIN_FIELD_BASE(120, 120, 5, 0x0000, 0x10, 24, 2),
	PIN_FIELD_BASE(121, 121, 5, 0x0000, 0x10, 28, 2),
	PIN_FIELD_BASE(122, 122, 5, 0x0010, 0x10, 0, 2),
	PIN_FIELD_BASE(123, 124, 7, 0x0000, 0x10, 12, 5),
	PIN_FIELD_BASE(125, 125, 7, 0x0000, 0x10, 24, 2),
	PIN_FIELD_BASE(126, 126, 6, 0x0000, 0x10, 0, 2),
	PIN_FIELD_BASE(127, 127, 7, 0x0000, 0x10, 22, 2),
	PIN_FIELD_BASE(128, 128, 6, 0x0000, 0x10, 9, 5),
	PIN_FIELD_BASE(129, 129, 6, 0x0000, 0x10, 4, 5),
	PIN_FIELD_BASE(130, 130, 6, 0x0000, 0x10, 19, 5),
	PIN_FIELD_BASE(131, 131, 6, 0x0000, 0x10, 14, 5),
	PIN_FIELD_BASE(132, 132, 6, 0x0010, 0x10, 0, 5),
	PIN_FIELD_BASE(133, 133, 6, 0x0000, 0x10, 24, 5),
	PIN_FIELD_BASE(134, 134, 6, 0x0010, 0x10, 10, 5),
	PIN_FIELD_BASE(135, 135, 6, 0x0010, 0x10, 5, 5),
	PIN_FIELD_BASE(136, 136, 6, 0x0010, 0x10, 23, 2),
	PIN_FIELD_BASE(137, 137, 6, 0x0010, 0x10, 17, 2),
	PIN_FIELD_BASE(138, 138, 6, 0x0010, 0x10, 15, 2),
	PINS_FIELD_BASE(139, 141, 6, 0x0010, 0x10, 17, 2),
	PINS_FIELD_BASE(142, 143, 7, 0x0000, 0x10, 26, 2),
	PIN_FIELD_BASE(144, 144, 7, 0x0010, 0x10, 10, 2),
	PIN_FIELD_BASE(145, 145, 7, 0x0010, 0x10, 8, 2),
	PINS_FIELD_BASE(146, 147, 7, 0x0010, 0x10, 10, 2),
	PIN_FIELD_BASE(148, 149, 7, 0x0000, 0x10, 28, 2),
	PIN_FIELD_BASE(150, 153, 7, 0x0010, 0x10, 0, 2),
};

static const struct mtk_pin_reg_calc mt6739_reg_cals[PINCTRL_PIN_REG_MAX] = {
	[PINCTRL_PIN_REG_MODE] = MTK_RANGE(mt6739_pin_mode_range),
	[PINCTRL_PIN_REG_DIR] = MTK_RANGE(mt6739_pin_dir_range),
	[PINCTRL_PIN_REG_DI] = MTK_RANGE(mt6739_pin_di_range),
	[PINCTRL_PIN_REG_DO] = MTK_RANGE(mt6739_pin_do_range),
	[PINCTRL_PIN_REG_SR] = MTK_RANGE(mt6739_pin_dir_range),
	[PINCTRL_PIN_REG_SMT] = MTK_RANGE(mt6739_pin_smt_range),
	[PINCTRL_PIN_REG_IES] = MTK_RANGE(mt6739_pin_ies_range),
	[PINCTRL_PIN_REG_PU] = MTK_RANGE(mt6739_pin_pu_range),
	[PINCTRL_PIN_REG_PD] = MTK_RANGE(mt6739_pin_pd_range),
	[PINCTRL_PIN_REG_DRV] = MTK_RANGE(mt6739_pin_drv_range),
	[PINCTRL_PIN_REG_PUPD] = MTK_RANGE(mt6739_pin_pupd_range),
	[PINCTRL_PIN_REG_R0] = MTK_RANGE(mt6739_pin_r0_range),
	[PINCTRL_PIN_REG_R1] = MTK_RANGE(mt6739_pin_r1_range),
};

static const struct mtk_pin_soc mt6739_data = {
	.reg_cal = mt6739_reg_cals,
	.pins = mtk_pins_mt6739,
	.npins = ARRAY_SIZE(mtk_pins_mt6739),
	.ngrps = ARRAY_SIZE(mtk_pins_mt6739),
	.nfuncs = 8,
	.gpio_m = 0,
	.capability_flags = FLAG_RACE_FREE_ACCESS
				| FLAG_DRIVE_SET_RAW,
	.bias_set_combo = mtk_pinconf_bias_set_combo,
	.bias_get_combo = mtk_pinconf_bias_get_combo,
	.adv_drive_set = mtk_pinconf_adv_drive_set,
	.adv_drive_get = mtk_pinconf_adv_drive_get,
};

static const struct of_device_id mt6739_pinctrl_of_match[] = {
	{ .compatible = "mediatek,mt6739-pinctrl", },
	{ }
};

static int mt6739_pinctrl_probe(struct platform_device *pdev)
{
	return mtk_paris_pinctrl_probe(pdev, &mt6739_data);
}

static struct platform_driver mt6739_pinctrl_driver = {
	.driver = {
		.name = "mt6739-pinctrl",
		.of_match_table = mt6739_pinctrl_of_match,
		.pm = &mtk_paris_pinctrl_pm_ops,
	},
	.probe = mt6739_pinctrl_probe,
};

static int __init mt6739_pinctrl_init(void)
{
	return platform_driver_register(&mt6739_pinctrl_driver);
}
arch_initcall(mt6739_pinctrl_init);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("MediaTek MT6739 Pinctrl Driver");
