#ifndef __SPARSE_FORMAT_
#define __SPARSE_FORMAT_
#include <stdint.h>
typedef struct sparse_header {
   uint32_t  magic;		/* 0xed26ff3a */
   uint16_t	major_version;	/* (0x1) - reject images with higher major versions */
   uint16_t	minor_version;	/* (0x0) - allow images with higer minor versions */
   uint16_t	file_hdr_sz;	/* 28 bytes for first revision of the file format */
   uint16_t	chunk_hdr_sz;	/* 12 bytes for first revision of the file format */
   uint32_t	blk_sz;		/* block size in bytes, must be a multiple of 4 (4096) */
   uint32_t	total_blks;	/* total blocks in the non-sparse output image */
   uint32_t	total_chunks;	/* total chunks in the sparse input image */
   uint32_t	image_checksum; /* CRC32 checksum of the original data, counting "don't care" */
   /* as 0. Standard 802.3 polynomial, use a Public Domain */
   /* table implementation */
} sparse_header_t;

#define SPARSE_HEADER_MAGIC	0xed26ff3a

#define CHUNK_TYPE_RAW		0xCAC1
#define CHUNK_TYPE_FILL		0xCAC2
#define CHUNK_TYPE_DONT_CARE	0xCAC3
#define CHUNK_TYPE_CRC		0xCAC4

#define DATA_SLC_MODE 0
#define DATA_MLC_TLC_MODE 1

typedef struct chunk_header {
   uint16_t	chunk_type;	/* 0xCAC1 -> raw; 0xCAC2 -> fill; 0xCAC3 -> don't care */
   uint16_t	data_mode;
   uint32_t	chunk_sz;	/* in blocks in output image */
   uint32_t	total_sz;	/* in bytes of chunk input file including chunk header and data */
} chunk_header_t;

/* Following a Raw or Fill chunk is data.  For a Raw chunk, it's the data in chunk_sz * blk_sz.
*  For a Fill chunk, it's 4 bytes of the fill data.
*/

#endif
