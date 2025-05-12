#ifndef __MT6375_H
#define __MT6375_H

#include <typedefs.h>

extern int mt6375_write_byte(u16 cmd, u8 data);
extern int mt6375_read_byte(u16 cmd, u8 *data);
extern int mt6375_block_read(u16 cmd, u8 *data, int len);
extern int mt6375_block_write(u16 cmd, const u8 *data, int len);
extern int mt6375_update_bits(u16 cmd, u8 mask, u8 data);

static inline int mt6375_set_bits(u32 reg, u32 mask)
{
	return mt6375_update_bits(reg, mask, mask);
}

static inline int mt6375_clr_bits(u32 reg, u32 mask)
{
	return mt6375_update_bits(reg, mask, 0);
}
#endif /* __MT6375_H */
