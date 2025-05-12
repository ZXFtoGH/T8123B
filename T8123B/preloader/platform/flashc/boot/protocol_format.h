
#pragma once

#include "stdint.h"

#define MAGIC_PROTOCOL 0xFEEEEEEF

typedef enum
{
   DT_NULL = 0,
   DT_PROTOCOL_FLOW = 1,
   DT_MESSAGE = 2,
}data_type_t;

typedef struct
{  
   uint32_t guard_magic;     
   uint32_t type;  //protocol or massage.   
}protocol_header_t;

typedef struct
{  
   protocol_header_t phdr;
   uint32_t length;  //length of data
}protocol_header_wrapper_t;

typedef struct
{  
   protocol_header_t phdr;
   uint32_t length; //length of data
   uint32_t priority;
}message_header_wrapper_t;

typedef struct
{  
   protocol_header_wrapper_t hdr;
   uint8_t  status[512]; 
}protocol_data_status_t;


//just for format depiction.
typedef struct
{  
   protocol_header_wrapper_t hdr;
   uint8_t  data[1]; 
}protocol_data_t;

typedef struct
{  
   message_header_wrapper_t hdr; 
   uint8_t  data[1]; 
}message_data_t;


