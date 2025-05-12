#ifndef __CLDMA_H__
#define __CLDMA_H__

unsigned int CLDMA_Init(void);
unsigned int CLDMA_GetData(unsigned char* buf, unsigned int len, unsigned int timeout);
unsigned int CLDMA_PutData(unsigned char* buf, unsigned int len, unsigned int timeout);

#endif
