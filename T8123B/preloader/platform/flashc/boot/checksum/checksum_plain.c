#include "checksum.h"
#include "boot/error_message.h"


uint32_t checksum_plain_impl(uint8_t* data, uint32_t length)
{ 
#if 1
   register uint32_t chksum = 0;
   uint8_t* ptr = data;
   uint32_t i = 0;

   for(i=0;i<(length&(~3));i +=sizeof(int))
   {
      chksum += (*(uint32_t*)(data+i));
   }
   if(i < length)//can not aligned by 4
   {
      ptr += i;
      for(i = 0; i<(length&3); i++)/// remain
      {
         chksum += ptr[i];
      }
   }
   return chksum;

  
#else
  uint8_t *p_base = (uint8_t *)data;
  uint32_t i, length_aligned;
  int check_sum = 0;
  uint8x16_t vec_line_u8x16;
  uint16x8_t vec_sum_u16x8;
  uint16_t vec_result_u16[17] = {0};

  length_aligned = length & ~15;
  vec_sum_u16x8 = vdupq_n_u16(0);
  vec_line_u8x16 = vdupq_n_u8(0);

  for (i = 0; i < length_aligned; i += 16) {
    vec_line_u8x16 = vld1q_u8(p_base + i);
    vec_sum_u16x8 = vpadalq_u8(vec_sum_u16x8, vec_line_u8x16);
  }

  if (i < length) {
    for (; i < length; i++)
      check_sum += *(p_base + i);
    // workaround for align with old architure padding mechanism, if length is
    // odd, padding with 0xff.
    //lxo. remove this padding. consider useless.
    //if ((length & 0x1) != 0) {
    //  check_sum += 0xFF;
    //}
  }

  vst1q_u16(vec_result_u16, vec_sum_u16x8);
  for (i = 0; i < 8; i++) {
    check_sum += vec_result_u16[i];
  }

    return check_sum;
#endif
}

