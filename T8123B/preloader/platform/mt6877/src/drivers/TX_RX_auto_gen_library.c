#if FOR_DV_SIMULATION_USED
#include <stdio.h>
#include <string.h>
#endif

//#include "typedefs.h"
#include "dramc_dv_init.h"

U32 get_max_val_form_2dim(unsigned int init_data[2][2])
{
  unsigned int max_val;
  unsigned int rank_cnt;
  unsigned int byte_cnt;
  max_val = init_data[0][0];
  for (rank_cnt=0; rank_cnt<2; rank_cnt++)
  {
    for(byte_cnt=0; byte_cnt<2; byte_cnt++)
    {
      if (max_val < init_data[rank_cnt][byte_cnt])
	max_val = init_data[rank_cnt][byte_cnt];
    }
  }
  return max_val;
}
U32 get_min_val_form_2dim(unsigned int init_data[2][2])
{
  unsigned int min_val;
  unsigned int rank_cnt;
  unsigned int byte_cnt;
  min_val = init_data[0][0];
  for (rank_cnt=0; rank_cnt<2; rank_cnt++)
  {
    for(byte_cnt=0; byte_cnt<2; byte_cnt++)
    {
      if (min_val > init_data[rank_cnt][byte_cnt])
	min_val = init_data[rank_cnt][byte_cnt];
    }
  }
  return min_val;
}

///TODO: check ser_latency floating value
U8 get_wck_p2s_latency(U8 p2s_ratio, U8 frate)
{
  U8 ser_latency;
  //U8    real_latency;
  if (((p2s_ratio == 2) & (frate == 0)) | (frate > 1))
  {
    mcSHOW_DBG_MSG(("Error: when p2s_ratio is 2, frate should be 1. But p2s_ratio is %0d, frate is %0d \n", p2s_ratio, frate));
  }
  p2s_ratio = p2s_ratio * (1 + frate);
  switch (p2s_ratio)
  {
    case 16: {(DUT_p.DEF_DIGPHY==1) ? (ser_latency = 9   ) : (ser_latency = 9   ); break;}
    case 8 : {(DUT_p.DEF_DIGPHY==1) ? (ser_latency = 7   ) : (ser_latency = 5   ); break;}
    case 4 : {(DUT_p.DEF_DIGPHY==1) ? (ser_latency = 3   ) : (ser_latency = 3   ); break;}
    //case 2 : {(DUT_p.DEF_DIGPHY==1) ? (ser_latency = 1.5 ) : (ser_latency = 1.5 ); break;}
    case 2 : {(DUT_p.DEF_DIGPHY==1) ? (ser_latency = 1 ) : (ser_latency = 1 ); break;}
    default : { mcSHOW_ERR_MSG(("Error: %0d is unexpected p2s ratio \n", p2s_ratio)); ASSERT(0); }
  }
  //real_latency = (U8)(ser_latency * (1 + frate));
  return ser_latency;
}

///TODO: check ser_latency floating value
U8 get_dq_ca_p2s_latency(U8 p2s_ratio, U8 frate)
{
  U8 ser_latency;
  //U8    real_latency;
  if (((p2s_ratio == 2) & (frate == 0)) | (frate > 1))
  {
    mcSHOW_DBG_MSG(("Error: when p2s_ratio is 2, frate should be 1. But p2s_ratio is %0d, frate is %0d \n", p2s_ratio, frate));
  }
  p2s_ratio = p2s_ratio * (1 + frate);

  switch (p2s_ratio)
  {
    case 16: {(DUT_p.DEF_DIGPHY==1) ? (ser_latency = 9   ) : (ser_latency = 9   ); break;}
    case 8 : {(DUT_p.DEF_DIGPHY==1) ? (ser_latency = 6   ) : (ser_latency = 5   ); break;}
    case 4 : {(DUT_p.DEF_DIGPHY==1) ? (ser_latency = 3   ) : (ser_latency = 3   ); break;}
    //case 2 : {(DUT_p.DEF_DIGPHY==1) ? (ser_latency = 1.5 ) : (ser_latency = 1.5 ); break;}
    case 2 : {(DUT_p.DEF_DIGPHY==1) ? (ser_latency = 1 ) : (ser_latency = 1 ); break;}
    default : { mcSHOW_ERR_MSG(("Error: %0d is unexpected p2s ratio \n", p2s_ratio)); ASSERT(0); }
  }
  //real_latency = (U8)(ser_latency * (1 + frate));
  return ser_latency;
}

U8 get_dqsien_p2s_latency(U8 p2s_ratio)
{
  U8 ser_latency;
  switch (p2s_ratio)
  {
    case 16: {(DUT_p.DEF_DIGPHY==1) ? (ser_latency = 18 ) : (ser_latency = 18 ); break;}
    case 8 : {(DUT_p.DEF_DIGPHY==1) ? (ser_latency = 7  ) : (ser_latency = 8  ); break;}
    case 4 : {(DUT_p.DEF_DIGPHY==1) ? (ser_latency = 4  ) : (ser_latency = 4  ); break;}
    case 2 : {(DUT_p.DEF_DIGPHY==1) ? (ser_latency = 2  ) : (ser_latency = 2  ); break;}
    default : { mcSHOW_ERR_MSG(("Error: %0d is unexpected p2s ratio \n", p2s_ratio)); ASSERT(0); }
  }
  return ser_latency;
}

U8 get_oe_p2s_latency(U8 p2s_ratio)
{
  U8 ser_latency;
  switch (p2s_ratio)
  {
    case 16: {ser_latency = 8 ; break;}
    case 8 : {ser_latency = 4  ; break;}
    case 4 : {ser_latency = 2  ; break;}
    case 2 : {ser_latency = 1  ; break;}
    default : { mcSHOW_ERR_MSG(("Error: %0d is unexpected p2s ratio \n", p2s_ratio)); ASSERT(0); }
  }
  return ser_latency;
}
U32 A_div_B_RU (U32 A, U32 B)
{
#if 0
  U32 result;
  if (A == ( (A/B) * B ))
    result = A/B;
  else 
    result = ((float)((float)(A) / (float)(B)) > (int)((float)(A) / (float)(B)) ) ? ( (int)((float)(A) / (float)(B)) +1 ) : ((int)((float)(A) / (float)(B)) ) ;
  return result;
#endif
    U32 result = 0;
    if(B!=0)
    {
        result = (A+B-1)/B;
    }
    return result;
};
