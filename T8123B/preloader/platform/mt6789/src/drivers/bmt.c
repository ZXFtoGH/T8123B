/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein is
 * confidential and proprietary to MediaTek Inc. and/or its licensors. Without
 * the prior written permission of MediaTek inc. and/or its licensors, any
 * reproduction, modification, use or disclosure of MediaTek Software, and
 * information contained herein, in whole or in part, shall be strictly
 * prohibited.
 * 
 * MediaTek Inc. (C) 2012. All rights reserved.
 * 
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER
 * ON AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL
 * WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR
 * NONINFRINGEMENT. NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH
 * RESPECT TO THE SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY,
 * INCORPORATED IN, OR SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES
 * TO LOOK ONLY TO SUCH THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO.
 * RECEIVER EXPRESSLY ACKNOWLEDGES THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO
 * OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES CONTAINED IN MEDIATEK
 * SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE
 * RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S
 * ENTIRE AND CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE
 * RELEASED HEREUNDER WILL BE, AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE
 * MEDIATEK SOFTWARE AT ISSUE, OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE
 * CHARGE PAID BY RECEIVER TO MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 * The following software/firmware and/or related documentation ("MediaTek
 * Software") have been modified by MediaTek Inc. All revisions are subject to
 * any receiver's applicable license agreements with MediaTek Inc.
 */

#include "bmt.h"
#include "mt6789.h"
#include "platform.h"
#include "dram_buffer.h"

#define DBG_PRELOADER

typedef struct {
    char signature[3];
    u8 version;
    u8 bad_count;           // bad block count in pool
    u8 mapped_count;        // mapped block count in pool
    u8 checksum;
    u8 reseverd[13];
} phys_bmt_header;

typedef struct {
    phys_bmt_header header;
    bmt_entry table[MAX_BMT_SIZE];
} phys_bmt_struct;

typedef struct {
    char signature[3];
} bmt_oob_data;

static char MAIN_SIGNATURE[] = "BMT";
static char OOB_SIGNATURE[] = "bmt";
#define SIGNATURE_SIZE      (3)

#define MAX_DAT_SIZE        0x1000
#define MAX_OOB_SIZE        0x80

#if defined(__PRELOADER_NAND__)

static struct nand_chip *nand_chip_bmt;
#define BLOCK_SIZE_BMT          (nand_chip_bmt->erasesize)
#define PAGE_SIZE_BMT           (nand_chip_bmt->page_size)

#elif defined(__KERNEL_NAND__)

static struct mtd_info *mtd_bmt;
static struct nand_chip *nand_chip_bmt;
#define BLOCK_SIZE_BMT          (1 << nand_chip_bmt->phys_erase_shift)
#define PAGE_SIZE_BMT           (1 << nand_chip_bmt->page_shift)

#endif


#define OFFSET(block)       ((block) * BLOCK_SIZE_BMT)           //((block) << (mtd->erasesize_shift) + (page) << (mtd->writesize_shift))
#define PAGE_ADDR(block)    ((block) * BLOCK_SIZE_BMT / PAGE_SIZE_BMT)

/*********************************************************************
* Flash is splited into 2 parts, system part is for normal system    *
* system usage, size is system_block_count, another is replace pool  *
*    +-------------------------------------------------+             *
*    |     system_block_count     |   bmt_block_count  |             *
*    +-------------------------------------------------+             *
*********************************************************************/
static u32 total_block_count;       // block number in flash
static u32 system_block_count;
static int bmt_block_count;         // bmt table size
static int page_per_block;          // page per count

static u32 bmt_block_index;         // bmt block index
static bmt_struct bmt;              // dynamic created global bmt table

#define BMT_BUFFER_SIZE     0x10000
/*DRAM BUFFER to be modified*/
//static u8 __DRAM__ bmt_buf[BMT_BUFFER_SIZE];
//u8 __DRAM__ bmt_dat_buf[BMT_DAT_BUFFER_SIZE];
#define bmt_buf g_dram_buf->bmt_buf
#define bmt_dat_buf g_dram_buf->bmt_dat_buf


//static u8 *dat_buf = bmt_buf;		//(0x80000);
//static u8 *oob_buf = bmt_buf + 4096;		//(0x80000 + 4096);
static u8 *dat_buf ;
static u8 *oob_buf ;

static bool pool_erased;




/***************************************************************
*                                                              
* Interface adaptor for preloader/uboot/kernel                 
*    These interfaces operate on physical address, read/write
*       physical data.
*                                                              
***************************************************************/
#if defined(__PRELOADER_NAND__)
int nand_read_page_bmt(u32 page, u8 *dat, u8 *oob)
{
    return mtk_nand_read_page_hw(page, dat, oob);
}

bool nand_block_bad_bmt(u32 offset)
{
    return nand_block_bad_hw(offset);
}

bool nand_erase_bmt(u32 offset)
{
    return mtk_nand_erase_hw(offset);
}

int mark_block_bad_bmt(u32 offset)
{
    return mark_block_bad_hw(offset);
}

bool nand_write_page_bmt(u32 page, u8 *dat, u8 *oob)
{
    return mtk_nand_write_page_hw(page, dat, oob);
}

#elif defined(__KERNEL_NAND__)

int nand_read_page_bmt(u32 page, u8 *dat, u8 *oob)
{
    return mtk_nand_exec_read_page(mtd_bmt, page, PAGE_SIZE_BMT, dat, oob);
}

bool nand_block_bad_bmt(u32 offset)
{
    return mtk_nand_block_bad_hw(mtd_bmt, offset);
}

bool nand_erase_bmt(u32 offset)
{
    int status;
    if (offset < 0x20000)
    {
        MSG(INIT, "erase offset: 0x%x\n", offset);
    }
    
    status = mtk_nand_erase_hw(mtd_bmt, offset / PAGE_SIZE_BMT);      // as nand_chip structure doesn't have a erase function defined
    if (status & NAND_STATUS_FAIL)
        return false;
    else
        return true;
}

int mark_block_bad_bmt(u32 offset)
{
    return mtk_nand_block_markbad_hw(mtd_bmt, offset);             //mark_block_bad_hw(offset);
}

bool nand_write_page_bmt(u32 page, u8 *dat, u8 *oob)
{
    if (mtk_nand_exec_write_page(mtd_bmt, page, PAGE_SIZE_BMT, dat, oob))
        return false;
    else
        return true;
}

#endif



/***************************************************************
*                                                              *
* static internal function                                     *
*                                                              *
***************************************************************/
static void dump_bmt_info(bmt_struct *bmt)
{
    int i;
    
    MSG(INIT, "BMT v%d. total %d mapping:\n", bmt->version, bmt->mapped_count);
    for (i = 0; i < bmt->mapped_count; i++)
    {
        MSG(INIT, "\t0x%x -> 0x%x\n", bmt->table[i].bad_index, bmt->table[i].mapped_index);
    }
}

static bool match_bmt_signature(u8 *dat, u8 *oob)
{
    if (memcmp(dat + MAIN_SIGNATURE_OFFSET, MAIN_SIGNATURE, SIGNATURE_SIZE))
    {
MSG(INIT, "[%s]0x%x,0x%x,0x%x,0x%x \n",__FUNCTION__, *((UINT32 *)dat),*(((UINT32 *)dat)+1),*(((UINT32 *)dat)+2),*(((UINT32 *)dat)+3));
        return false;
    }
    
    if (memcmp(oob + OOB_SIGNATURE_OFFSET, OOB_SIGNATURE, SIGNATURE_SIZE))
    {
        MSG(INIT, "main signature match, oob signature doesn't match, but ignore\n");
    }
    return true;
}

static u8 cal_bmt_checksum(phys_bmt_struct *phys_table, int bmt_size)
{
    int i;
    u8 checksum = 0;
    u8 *dat = (u8 *)phys_table;

    checksum += phys_table->header.version;
    // checksum += phys_table.header.bad_count;
    checksum += phys_table->header.mapped_count;

    dat += sizeof(phys_bmt_header);
    for (i = 0; i < bmt_size * sizeof(bmt_entry); i++)
    {
        checksum += dat[i];
    }

    return checksum;
}

static int is_block_mapped(int index)
{
    int i;
    for (i = 0; i < bmt.mapped_count; i++)
    {
        if (index == bmt.table[i].mapped_index)
            return i;
    }
    return -1;
}

static bool is_page_used(u8 *dat, u8 *oob)
{
    return ( (oob[OOB_INDEX_OFFSET] != 0xFF)  || (oob[OOB_INDEX_OFFSET + 1] != 0xFF) );
}

static bool valid_bmt_data(phys_bmt_struct *phys_table)
{
    int i;
    u8 checksum = cal_bmt_checksum(phys_table, bmt_block_count);
    
    // checksum correct?
    if ( phys_table->header.checksum != checksum)
    {
        MSG(INIT, "BMT Data checksum error: %x %x\n", phys_table->header.checksum, checksum);
        return false;
    }
#ifdef DBG_PRELOADER
    MSG(INIT, "BMT Checksum is: 0x%x\n", phys_table->header.checksum);
#endif    
    // block index correct?
    for (i = 0; i < phys_table->header.mapped_count; i++)
    {
        if (phys_table->table[i].bad_index >= total_block_count ||
            phys_table->table[i].mapped_index >= total_block_count ||
            phys_table->table[i].mapped_index < system_block_count)
        {
            MSG(INIT, "index error: bad_index: %d, mapped_index: %d\n", 
                phys_table->table[i].bad_index, phys_table->table[i].mapped_index);
            return false;
        }
    }
#ifdef DBG_PRELOADER
    // pass check, valid bmt.
    MSG(INIT, "Valid BMT, version v%d\n", phys_table->header.version);
#endif
    return true;
}


static void fill_nand_bmt_buffer(bmt_struct *bmt, u8 *dat, u8 *oob)
{
    phys_bmt_struct phys_bmt;

    dump_bmt_info(bmt);

    // fill phys_bmt_struct structure with bmt_struct
    memset(&phys_bmt, 0xFF, sizeof(phys_bmt));
    
    memcpy(phys_bmt.header.signature, MAIN_SIGNATURE, SIGNATURE_SIZE);
    phys_bmt.header.version = BMT_VERSION;
    // phys_bmt.header.bad_count = bmt->bad_count;
    phys_bmt.header.mapped_count = bmt->mapped_count;
    memcpy(phys_bmt.table, bmt->table, sizeof(bmt_entry) * bmt_block_count);

    phys_bmt.header.checksum = cal_bmt_checksum(&phys_bmt, bmt_block_count);

    memcpy(dat + MAIN_SIGNATURE_OFFSET, &phys_bmt, sizeof(phys_bmt));
    memcpy(oob + OOB_SIGNATURE_OFFSET, OOB_SIGNATURE, SIGNATURE_SIZE);
}

// return valid index if found BMT, else return 0

static int load_bmt_data(int start, int pool_size)
{
    int bmt_index = start + pool_size - 1;        // find from the end
    phys_bmt_struct phys_table;
    int i;


#ifdef DBG_PRELOADER 
    MSG(INIT,  "[%s]: begin to search BMT from block 0x%x\n", __FUNCTION__, bmt_index);
#endif
	MSG(INIT,  "[%s]: bmt_index=0x%x, start=0x%x, pool_size=0x%x,  \n", __FUNCTION__, bmt_index,start,pool_size);	

    for (bmt_index = start + pool_size - 1; bmt_index >= start; bmt_index--)
    {
MSG(INIT,  "[%s]: bmt_index=0x%x-- ", __FUNCTION__, bmt_index);
        if (nand_block_bad_bmt(OFFSET(bmt_index)))
        {
            MSG(INIT, "Skip bad block: %d\n", bmt_index);
            continue;
        }
 memset(bmt_buf, 0xAA, 4096);
 memset(oob_buf, 0xAA, 128);
        if (!nand_read_page_bmt(PAGE_ADDR(bmt_index), bmt_buf, oob_buf))
        //if (!nand_read_page_bmt(0x3FFC0, bmt_buf, oob_buf))
        {
            MSG(INIT, "Error found when read block %d\n", bmt_index);
            continue;
        }

        if (!match_bmt_signature(bmt_buf, oob_buf))
        {
MSG(INIT,  "[%s]: match_bmt_signature  out! \n", __FUNCTION__); 
            continue;
        }
#ifdef DBG_PRELOADER
        MSG(INIT, "Match bmt signature @ block: 0x%x\n", bmt_index);
#endif        
        memcpy(&phys_table, bmt_buf + MAIN_SIGNATURE_OFFSET, sizeof(phys_table));

        if (!valid_bmt_data(&phys_table))
        {
            MSG(INIT, "BMT data is not correct %d\n", bmt_index);
            continue;
        }
        else
        {
            bmt.mapped_count = phys_table.header.mapped_count;
            bmt.version = phys_table.header.version;
            // bmt.bad_count = phys_table.header.bad_count;
            memcpy(bmt.table, phys_table.table, bmt.mapped_count * sizeof(bmt_entry));
#ifdef DBG_PRELOADER
            MSG(INIT, "bmt found at block: %d, mapped block: %d\n", bmt_index, bmt.mapped_count);
#endif
            for (i = 0; i < bmt.mapped_count; i++)
            {
                if (!nand_block_bad_bmt(OFFSET(bmt.table[i].bad_index)))
                {
                    MSG(INIT, "block 0x%x is not mark bad, should be power lost last time\n", bmt.table[i].bad_index);
                    mark_block_bad_bmt(OFFSET(bmt.table[i].bad_index));
                }
            }
            
            return bmt_index;
        }
    }    
#ifdef DBG_PRELOADER
    MSG(INIT, "bmt block not found!\n");
#endif
    return 0;
}


/*************************************************************************
* Find an available block and erase.                                     *
* start_from_end: if true, find available block from end of flash.       *
*                 else, find from the beginning of the pool              *
* need_erase: if true, all unmapped blocks in the pool will be erased    *
*************************************************************************/
static int find_available_block(bool start_from_end)
{
    int i;      // , j;
    int block = system_block_count;
    int direction;
    // int avail_index = 0;
    MSG(INIT, "Try to find_available_block, pool_erase: %d\n", pool_erased);

    // erase all un-mapped blocks in pool when finding avaliable block
    if (!pool_erased)
    {
        MSG(INIT, "Erase all un-mapped blocks in pool\n");
        for (i = 0; i < bmt_block_count; i++)
        {
	        if (block == bmt_block_index)
	        {
	            MSG(INIT, "Skip bmt block 0x%x\n", block);
	            continue;
	        }            
	        
	        if (nand_block_bad_bmt(OFFSET(block + i)))
            {
                MSG(INIT, "Skip bad block 0x%x\n", block + i);
                continue;
            }

            if (is_block_mapped(block + i) >= 0)
            {
                MSG(INIT, "Skip mapped block 0x%x\n", block + i);
                continue;
            }

            if (!nand_erase_bmt(OFFSET(block + i)))
            {
                MSG(INIT, "Erase block 0x%x failed\n", block + i);
                mark_block_bad_bmt(OFFSET(block + i));
            }
        }

        pool_erased = 1;
    }

    if (start_from_end)
    {
        block = total_block_count - 1;
        direction = -1;
    }
    else
    {
        block = system_block_count;
        direction = 1;
    }

    for (i = 0; i < bmt_block_count; i++, block += direction)
    {
        if (block == bmt_block_index)
        {
            MSG(INIT, "Skip bmt block 0x%x\n", block);
            continue;
        }

        if (nand_block_bad_bmt(OFFSET(block)))
        {
            MSG(INIT, "Skip bad block 0x%x\n", block);
            continue;
        }

        if (is_block_mapped(block) >= 0)
        {
            MSG(INIT, "Skip mapped block 0x%x\n", block);
            continue;
        }

        MSG(INIT, "Find block 0x%x available\n", block);
        return block;
    }

    return 0;
}


static unsigned short get_bad_index_from_oob(u8 *oob_buf)
{
    unsigned short index;
    memcpy(&index, oob_buf + OOB_INDEX_OFFSET, OOB_INDEX_SIZE);

    return index;
}

void set_bad_index_to_oob(u8 *oob, u16 index)
{
    memcpy(oob + OOB_INDEX_OFFSET, &index, sizeof(index));
}

static int migrate_from_bad(int offset, u8 *write_dat, u8 *write_oob)
{
    int page;
    int error_block = offset / BLOCK_SIZE_BMT;
    int error_page = (offset / PAGE_SIZE_BMT) % page_per_block;
    int to_index;

    memcpy(oob_buf, write_oob, MAX_OOB_SIZE);
    
    to_index = find_available_block(false);

    if (!to_index)
    {
        MSG(INIT, "Cannot find an available block for BMT\n");
        return 0;
    }

    {       // migrate error page first
        MSG(INIT, "Write error page: 0x%x\n", error_page);
        if (!write_dat)
        {
            nand_read_page_bmt(PAGE_ADDR(error_block) + error_page, dat_buf, NULL);
            write_dat = dat_buf;
        }

        // memcpy(oob_buf, write_oob, MAX_OOB_SIZE);
        
        if (error_block < system_block_count)
            set_bad_index_to_oob(oob_buf, error_block);       // if error_block is already a mapped block, original mapping index is in OOB.
        
        if (!nand_write_page_bmt(PAGE_ADDR(to_index) + error_page, write_dat, oob_buf))
        {
            MSG(INIT, "Write to page 0x%x fail\n", PAGE_ADDR(to_index) + error_page);
            mark_block_bad_bmt(to_index);
            return migrate_from_bad(offset, write_dat, write_oob);
        }
    }



    for (page = 0; page < page_per_block; page++)
    {
        if (page != error_page)
        {
            nand_read_page_bmt(PAGE_ADDR(error_block) + page, dat_buf, oob_buf);
            if (is_page_used(dat_buf, oob_buf))
            {
                if (error_block < system_block_count)
                {
                    set_bad_index_to_oob(oob_buf, error_block);
                }
                MSG(INIT, "\tmigrate page 0x%x to page 0x%x\n", 
                    PAGE_ADDR(error_block) + page, PAGE_ADDR(to_index) + page);
                if (!nand_write_page_bmt(PAGE_ADDR(to_index) + page, dat_buf, oob_buf))
                {
                    MSG(INIT, "Write to page 0x%x fail\n", PAGE_ADDR(to_index) + page);
                    mark_block_bad_bmt(to_index);
                    return migrate_from_bad(offset, write_dat, write_oob);
                }
            }
        }
    }

    MSG(INIT, "Migrate from 0x%x to 0x%x done!\n", error_block, to_index);

    return to_index;
}

static bool write_bmt_to_flash(u8 *dat, u8 *oob)
{
    bool need_erase = true;
    MSG(INIT, "Try to write BMT\n");

	MSG(INIT, "bmt_block_index = 0x%x\n", bmt_block_index);
    if (bmt_block_index == 0)
    {
        // if we don't have index, we don't need to erase found block as it has been erased in find_available_block()
        need_erase = false;
		MSG(INIT, "set need_erase = 0x%x\n", need_erase);
        if ( !(bmt_block_index = find_available_block(true)) )
        {
            MSG(INIT, "Cannot find an available block for BMT\n");
            return false;
        }
    }

    MSG(INIT, "Find BMT block: 0x%x\n", bmt_block_index);
    MSG(INIT, "need_erase = 0x%x\n", need_erase);
    // write bmt to flash
    if (need_erase)
    {
        if (!nand_erase_bmt(OFFSET(bmt_block_index)))
        {
            MSG(INIT, "BMT block erase fail, mark bad: 0x%x\n", bmt_block_index);
            mark_block_bad_bmt(OFFSET(bmt_block_index));
            // bmt.bad_count++;

            bmt_block_index = 0;
            return write_bmt_to_flash(dat, oob);        // recursive call 
        }
    }

    if ( !nand_write_page_bmt(PAGE_ADDR(bmt_block_index), dat, oob) )
    {
        MSG(INIT, "Write BMT data fail, need to write again\n");
        mark_block_bad_bmt(OFFSET(bmt_block_index));
        // bmt.bad_count++;
        
        bmt_block_index = 0;
        return write_bmt_to_flash(dat, oob);        // recursive call 
    }

    MSG(INIT, "Write BMT data to block 0x%x success\n", bmt_block_index);
    return true;
}

/*******************************************************************
* Reconstruct bmt, called when found bmt info doesn't match bad 
* block info in flash.
* 
* Return NULL for failure
*******************************************************************/
bmt_struct *reconstruct_bmt(bmt_struct * bmt)
{
    int i;
    int index = system_block_count;
    unsigned short bad_index;
    int mapped;

    // init everything in BMT struct 
    bmt->version = BMT_VERSION;
    bmt->bad_count = 0;
    bmt->mapped_count = 0;
    
    memset(bmt->table, 0, bmt_block_count * sizeof(bmt_entry));

    for (i = 0; i < bmt_block_count; i++, index++)
    {
        if (nand_block_bad_bmt(OFFSET(index)))
        {
            MSG(INIT,  "Skip bad block: 0x%x\n", index);
            // bmt->bad_count++;
            continue;
        }

        MSG(INIT,  "read page: 0x%x\n", PAGE_ADDR(index));
        nand_read_page_bmt(PAGE_ADDR(index), dat_buf, oob_buf);

        if ((bad_index = get_bad_index_from_oob(oob_buf)) >= system_block_count)
        {
            MSG(INIT, "get bad index: 0x%x\n", bad_index);
            if (bad_index != 0xFFFF)
                MSG(INIT,  "Invalid bad index found in block 0x%x, bad index 0x%x\n", index, bad_index);
            continue;
        }

        MSG(INIT,  "Block 0x%x is mapped to bad block: 0x%x\n", index, bad_index);

        if (!nand_block_bad_bmt(OFFSET(bad_index)))
        {
            MSG(INIT,  "\tbut block 0x%x is not marked as bad, invalid mapping\n", bad_index);
            continue;       // no need to erase here, it will be erased later when trying to write BMT
        }


        if ( (mapped = is_block_mapped(bad_index)) >= 0)
        {
            MSG(INIT, "bad block 0x%x is mapped to 0x%x, should be caused by power lost, replace with one\n", 
                bmt->table[mapped].bad_index, bmt->table[mapped].mapped_index);
            bmt->table[mapped].mapped_index = index;    // use new one instead.
        }
        else
        {
            // add mapping to BMT
            bmt->table[bmt->mapped_count].bad_index = bad_index;
            bmt->table[bmt->mapped_count].mapped_index = index;
            bmt->mapped_count++;
        }

        MSG(INIT,  "Add mapping: 0x%x -> 0x%x to BMT\n", bad_index, index);

    }

    MSG(INIT,  "Scan replace pool done, mapped block: %d\n", bmt->mapped_count);

    memset(oob_buf, 0xFF, 128);
    fill_nand_bmt_buffer(bmt, dat_buf, oob_buf);

    // write BMT back
    if (!write_bmt_to_flash(dat_buf, oob_buf))
    {
        MSG(INIT,  "TRAGEDY: cannot find a place to write BMT!!!!\n");
    }

    return bmt;
}

/*******************************************************************
* [BMT Interface]
*
* Description:
*   Init bmt from nand. Reconstruct if not found or data error
*
* Parameter:
*   size: size of bmt and replace pool
* 
* Return: 
*   NULL for failure, and a bmt struct for success
*******************************************************************/
bmt_struct *init_bmt(struct nand_chip *chip, int size)
{

#if defined(__KERNEL_NAND__)
    struct mtk_nand_host *host;
#endif

    dat_buf = bmt_buf;		
    oob_buf = bmt_buf + 4096;		
	//return NULL;

    if (size > 0 && size < MAX_BMT_SIZE)
    {
        MSG(INIT, "Init bmt table, size: %d\n", size);
        bmt_block_count = size;
    }
    else
    {
        MSG(INIT, "Invalid bmt table size: %d\n", size);
        return NULL;
    }

#if defined(__PRELOADER_NAND__)

    nand_chip_bmt = chip;    
    system_block_count = chip->chipsize / chip->erasesize;
    total_block_count = bmt_block_count + system_block_count;
    page_per_block = chip->erasesize / chip->page_size;

#elif defined(__KERNEL_NAND__)

    nand_chip_bmt = chip;
    system_block_count = chip->chipsize >> chip->phys_erase_shift;
    total_block_count = bmt_block_count + system_block_count;
    page_per_block = BLOCK_SIZE_BMT / PAGE_SIZE_BMT;
    host = (struct mtk_nand_host *)chip->priv;
    mtd_bmt = &host->mtd;
#endif

#ifdef DBG_PRELOADER
    MSG(INIT, "bmt count: %d, system count: %d\n", bmt_block_count, system_block_count);
#endif
    // set this flag, and unmapped block in pool will be erased.
    pool_erased = 0;

    // alloc size for bmt.
    memset(bmt.table, 0, size * sizeof(bmt_entry));

    // load bmt if exist
    if ((bmt_block_index = load_bmt_data(system_block_count, size)))
    {
#ifdef DBG_PRELOADER
        MSG(INIT, "Load bmt data success @ block 0x%x\n", bmt_block_index);
        dump_bmt_info(&bmt);
#endif
        return &bmt;
    }
    else
    {
        MSG(INIT,  "Load bmt data fail, need re-construct!\n");
#ifndef __UBOOT_NAND__            // BMT is not re-constructed in UBOOT.
        if (reconstruct_bmt(&bmt))
            return &bmt;
        else
#endif
            return NULL;
    }
}


/*******************************************************************
* [BMT Interface]
*
* Description:
*   Update BMT.
*
* Parameter:
*   offset: update block/page offset.
*   reason: update reason, see update_reason_t for reason.
*   dat/oob: data and oob buffer for write fail.
* 
* Return: 
*   Return true for success, and false for failure.
*******************************************************************/
bool update_bmt(u32 offset, update_reason_t reason, u8 *dat, u8 *oob)
{
    int map_index;
    int orig_bad_block = -1;
    // int bmt_update_index;
    int i;
    int bad_index = offset / BLOCK_SIZE_BMT;
//return false;
    if (reason == UPDATE_WRITE_FAIL)
    {
        MSG(INIT, "Write fail, need to migrate\n");
        if ( !(map_index = migrate_from_bad(offset, dat, oob)) )
        {
            MSG(INIT, "migrate fail\n");
            return false;
        }
    }
    else
    {
        if ( !(map_index = find_available_block(false)) )
        {
            MSG(INIT,  "Cannot find block in pool\n");
            return false;
        }
    }

    // now let's update BMT
    if (bad_index >= system_block_count)     // mapped block become bad, find original bad block
    {
        for (i = 0; i < bmt_block_count; i++)
        {
            if (bmt.table[i].mapped_index == bad_index)
            {
                orig_bad_block = bmt.table[i].bad_index;
                break;
            }
        }
        // bmt.bad_count++;
        MSG(INIT, "Mapped block becomes bad, orig bad block is 0x%x\n", orig_bad_block);

        bmt.table[i].mapped_index = map_index;
    }
    else
    {
        bmt.table[bmt.mapped_count].mapped_index = map_index;
        bmt.table[bmt.mapped_count].bad_index = bad_index;
        bmt.mapped_count++;
    }

    memset(oob_buf, 0xFF, 128);
    fill_nand_bmt_buffer(&bmt, dat_buf, oob_buf);
    if (!write_bmt_to_flash(dat_buf, oob_buf))
        return false;

    mark_block_bad_bmt(offset);

    return true;
}

/*******************************************************************
* [BMT Interface]
*
* Description:
*   Given an block index, return mapped index if it's mapped, else 
*   return given index.
*
* Parameter:
*   index: given an block index. This value cannot exceed 
*   system_block_count.
*
* Return NULL for failure
*******************************************************************/
u16 get_mapping_block_index(int index)
{
    int i;
//return index;
    if (index > system_block_count)
    {
        MSG(INIT, "Given index exceed: 0x%x > 0x%x\n", index, system_block_count);
        return index;
    }

    for (i = 0; i < bmt.mapped_count; i++)
    {
        if (bmt.table[i].bad_index == index)
        {
            MSG(INIT, "Redirect 0x%x to 0x%x\n", index, bmt.table[i].mapped_index);
            return bmt.table[i].mapped_index;
        }
    }

    return index;
}

