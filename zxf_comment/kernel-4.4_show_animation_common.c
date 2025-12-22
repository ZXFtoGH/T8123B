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
 * MediaTek Inc. (C) 2010. All rights reserved.
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
 * 这段 C 代码是 MediaTek（联发科）平台上的充电动画显示逻辑，主要用于在设备关机或启动过程中，
 * 根据电池电量和不同的充电场景（普通充、快充、无线充等），从 logo.bin 文件中读取并绘制相应的图形资源到屏幕缓冲区。
 * 
 *  十、关键设计思想总结
资源打包：所有图像打包进 logo.bin，节省空间，便于管理。
按需解压：只解压当前需要的图像，避免内存浪费。
位深自适应：自动识别 16/32 bit 图像，兼容不同屏幕。
动画状态机：用静态变量记录当前帧，实现循环播放。
多场景支持：普通充、快充、无线充分离，UI 风格不同。
坐标抽象化：通过宏定义位置，便于适配不同分辨率。

 */

#define MTK_LOG_ENABLE 1
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "show_animation_common.h"
#include "fast_charging_common.h"
#include "charging_animation.h"

//这些是 动画帧索引计数器，用于循环播放动画（如低电量闪烁、进度条流动等）。
static int charging_low_index = 0;           // 低电量动画索引
static int charging_animation_index = 0;     // 充电动画索引
static int version0_charging_index = 0;      // V0版本充电索引
static int bits = 0;    //bits 记录图像颜色深度（16位 or 32位），避免重复判断

#define CHECK_LOGO_BIN_OK  0
#define CHECK_LOGO_BIN_ERROR  -1

//图像缓存数组（静态 buffer）
//用于临时存放 解压后的图像数据（数字、进度条、百分号等小图块）。
//使用 unsigned short 表示 16-bit 图像（RGB565），但也支持 32-bit（ARGB8888）。
static unsigned short  number_pic_addr[(NUMBER_RIGHT - NUMBER_LEFT)*(NUMBER_BOTTOM - NUMBER_TOP)*4] = {0x0}; //addr
static unsigned short  line_pic_addr[(TOP_ANIMATION_RIGHT - TOP_ANIMATION_LEFT)*4] = {0x0};
static unsigned short  percent_pic_addr[(PERCENT_RIGHT - PERCENT_LEFT)*(PERCENT_BOTTOM - PERCENT_TOP)*4] = {0x0};
static unsigned short  top_animation_addr[(TOP_ANIMATION_RIGHT - TOP_ANIMATION_LEFT)*(TOP_ANIMATION_BOTTOM - TOP_ANIMATION_TOP)*4] = {0x0};



/*
 * Check logo.bin address if valid, and get logo related info
 * pinfo[0] : the number of all pictures in logo.bin
 * pinf0[1] : the whole length of logo.bin
 * pinf0[2] : the index = 0 picture's length
 * pinfo[3+index] - pinfo[2+index] : means index length
 * 
 * ✅ 五、check_logo_index_valid() —— 校验 logo.bin 合法性
功能：
    从 logo_addr 开头读取元数据（pinfo 数组）：
        pinfo[0]：总图片数量
        pinfo[1]：整个 bin 文件大小
        pinfo[2+i]：第 i 张图在文件中的偏移
    判断请求的 index 是否有效
    计算该图的实际长度和内存地址（inaddr）
返回值：
    CHECK_LOGO_BIN_OK (0)：有效
    CHECK_LOGO_BIN_ERROR (-1)：无效（如 index 越界）
💡 logo.bin 是一个打包多个压缩图像的二进制文件，格式为：

[num][total_size][offset0][offset1]...[offsetN][data0][data1]...]
 */
int check_logo_index_valid(int index, void * logo_addr, LOGO_PARA_T *logo_info)
{
    unsigned int *pinfo = (unsigned int*)logo_addr;
    logo_info->logonum = pinfo[0];

    if (MTK_LOG_ENABLE == 1) {
        SLOGD("[show_animation_common: %s %d]logonum =%d, index =%d\n", __FUNCTION__,__LINE__,logo_info->logonum, index);
    }
    if (index >= logo_info->logonum)
    {
        if (MTK_LOG_ENABLE == 1) {
            SLOGE("[show_animation_common: %s %d]unsupported logo, index =%d\n", __FUNCTION__,__LINE__, index);
        }
        return CHECK_LOGO_BIN_ERROR;
    }

    if(index < logo_info->logonum - 1) {
        logo_info->logolen = pinfo[3+index] - pinfo[2+index];
        if (MTK_LOG_ENABLE == 1) {
            SLOGD("show_animation_common, pinfo[1]=%d, pinfo[3+index] - pinfo[2+index]= %d\n",
                   pinfo[1], pinfo[3+index] - pinfo[2+index]);
        }
    }
    else {
        logo_info->logolen = pinfo[1] - pinfo[2+index];
        if (MTK_LOG_ENABLE == 1) {
            SLOGD("show_animation_common, pinfo[1]=%d, pinfo[1] - pinfo[2+index] =%d \n",
                   pinfo[1], pinfo[1] - pinfo[2+index]);
        }
    }

    logo_info->inaddr = (unsigned int)logo_addr + pinfo[2+index];
    if (MTK_LOG_ENABLE == 1) {
        SLOGD("show_animation_common, logo_addr=0x%08x , in_addr=0x%08x,  logolen=%d\n",
                    (unsigned int)logo_addr, logo_info->inaddr,  logo_info->logolen);
    }
    return CHECK_LOGO_BIN_OK;
}


/*
 * Fill a screen size buffer with logo content
 *
 * fill_animation_logo() —— 绘制全屏背景图
        调用 decompress_logo() 解压整张图（如背景）
        自动检测图像位深（16/32 bit）
        调用 fill_rect_with_content() 填充整个屏幕
 */
void fill_animation_logo(int index, void *fill_addr, void * dec_logo_addr, void * logo_addr, LCM_SCREEN_T phical_screen)
{
    LOGO_PARA_T logo_info;
    int logo_width;
    int logo_height;
    int raw_data_size;
    if(check_logo_index_valid(index, logo_addr, &logo_info) != CHECK_LOGO_BIN_OK)
        return;

    raw_data_size = decompress_logo((void*)logo_info.inaddr, dec_logo_addr, logo_info.logolen, phical_screen.fb_size);
    if (MTK_LOG_ENABLE == 1) {
        SLOGD("[show_animation_common: %s %d]raw_data_size = %d\n",__FUNCTION__,__LINE__, raw_data_size);
    }
    //RECT_REGION_T rect = {0, 0, phical_screen.width, phical_screen.height};
    logo_width =phical_screen.width;
    logo_height = phical_screen.height;
    if (phical_screen.rotation == 270 || phical_screen.rotation == 90) {
        logo_width = phical_screen.height;
        logo_height = phical_screen.width;
    }
    if (0 == bits) {
        if (raw_data_size == logo_width*logo_height*4) {
            bits = 32;
        } else if (raw_data_size == logo_width*logo_height*2) {
            bits = 16;
        } else {
            if (MTK_LOG_ENABLE == 1) {
                SLOGE("[show_animation_common: %s %d]Logo data error\n",__FUNCTION__,__LINE__);
            }
            return;
        }
        if (MTK_LOG_ENABLE == 1) {
            SLOGD("[show_animation_common: %s %d]bits = %d\n",__FUNCTION__,__LINE__, bits);
        }
    }

    RECT_REGION_T rect = {0, 0, logo_width, logo_height};

    fill_rect_with_content(fill_addr, rect, dec_logo_addr, phical_screen, bits);
}

/*
 * Fill a rectangle size address with special color
 *
 * fill_animation_prog_bar() —— 绘制老式进度条（分段式）
        将进度条分为 ANIM_V0_REGIONS 段
        每段之间有空隙 ANIM_V0_SPACE_AFTER_REGION
        分别绘制“已充”部分（绿色）和“未充”部分（灰色）
 */
void fill_animation_prog_bar(RECT_REGION_T rect_bar,
                       unsigned int fgColor,
                       unsigned int start_div, unsigned int occupied_div,
                       void *fill_addr, LCM_SCREEN_T phical_screen)
{
    unsigned int div_size  = (rect_bar.bottom - rect_bar.top) / (ANIM_V0_REGIONS);
    unsigned int draw_size = div_size - (ANIM_V0_SPACE_AFTER_REGION);

    unsigned int i;

    for (i = start_div; i < start_div + occupied_div; ++ i)
    {
        unsigned int draw_bottom = rect_bar.bottom - div_size * i - (ANIM_V0_SPACE_AFTER_REGION);
        unsigned int draw_top    = draw_bottom - draw_size;

        RECT_REGION_T rect = {rect_bar.left, draw_top, rect_bar.right, draw_bottom};

        fill_rect_with_color(fill_addr, rect, fgColor, phical_screen);

    }
}


/*
 * Fill a rectangle with logo content
 *
 * fill_animation_dynamic() —— 绘制任意矩形区域的图像
        用于绘制数字、百分号、动画元素等
        指定 RECT_REGION_T rect 区域
        同样自动检测位深
 */
void fill_animation_dynamic(int index, RECT_REGION_T rect, void *fill_addr, void * dec_logo_addr, void * logo_addr, LCM_SCREEN_T phical_screen)
{
    LOGO_PARA_T logo_info;
    int raw_data_size;
    if(check_logo_index_valid(index, logo_addr, &logo_info) != CHECK_LOGO_BIN_OK)
        return;

    raw_data_size = decompress_logo((void*)logo_info.inaddr, (void*)dec_logo_addr, logo_info.logolen, (rect.right-rect.left)*(rect.bottom-rect.top)*4);
    if (MTK_LOG_ENABLE == 1) {
        SLOGD("[show_animation_common: %s %d]raw_data_size = %d\n",__FUNCTION__,__LINE__, raw_data_size);
    }
    if (0 == bits) {
        if (raw_data_size == (rect.right-rect.left)*(rect.bottom-rect.top)*4) {
            bits = 32;
        } else if (raw_data_size == (rect.right-rect.left)*(rect.bottom-rect.top)*2) {
            bits = 16;
        } else {
       	    if (MTK_LOG_ENABLE == 1) {
                SLOGE("[show_animation_common: %s %d]Logo data error\n",__FUNCTION__,__LINE__);
            }
            return;
        }
        if (MTK_LOG_ENABLE == 1) {
            SLOGD("[show_animation_common: %s %d]bits = %d\n",__FUNCTION__,__LINE__, bits);
        }
    }
    if (MTK_LOG_ENABLE == 1) {
        SLOGD("[show_animation_common: %s %d]bits = %d\n",__FUNCTION__,__LINE__, bits);
    }
    fill_rect_with_content(fill_addr, rect, dec_logo_addr, phical_screen, bits);
}


/*
 * Fill a rectangle  with number logo content
 *
 * number_position: 0~1st number, 1~2nd number
 * 
 * fill_animation_number() —— 绘制两位数的电量（十位 & 个位）
        number_position = 0 → 十位
        number_position = 1 → 个位
        使用预定义坐标宏（如 NUMBER_LEFT, NUMBER_TOP）
 */
void fill_animation_number(int index, unsigned int number_position, void *fill_addr,  void * logo_addr, LCM_SCREEN_T phical_screen)
{
    if (MTK_LOG_ENABLE == 1) {
        SLOGD("[show_animation_common: %s %d]index= %d, number_position = %d\n",__FUNCTION__,__LINE__, index, number_position);
    }

    LOGO_PARA_T logo_info;
    int raw_data_size;
    if(check_logo_index_valid(index, logo_addr, &logo_info) != CHECK_LOGO_BIN_OK)
        return;

    // draw default number rect,
    raw_data_size = decompress_logo((void*)logo_info.inaddr, (void*)number_pic_addr, logo_info.logolen, number_pic_size);
    if (MTK_LOG_ENABLE == 1) {
        SLOGD("[show_animation_common: %s %d]raw_data_size = %d\n",__FUNCTION__,__LINE__, raw_data_size);
    }
    //static RECT_REGION_T number_location_rect = {NUMBER_LEFT,NUMBER_TOP,NUMBER_RIGHT,NUMBER_BOTTOM};
    RECT_REGION_T battery_number_rect = {NUMBER_LEFT + (NUMBER_RIGHT - NUMBER_LEFT)*number_position,
                            NUMBER_TOP,
                            NUMBER_RIGHT + (NUMBER_RIGHT - NUMBER_LEFT)*number_position,
                            NUMBER_BOTTOM};

    if (0 == bits) {
        if (raw_data_size == (NUMBER_RIGHT - NUMBER_LEFT)*(NUMBER_BOTTOM - NUMBER_TOP)*4) {
            bits = 32;
        } else if (raw_data_size == (NUMBER_RIGHT - NUMBER_LEFT)*(NUMBER_BOTTOM - NUMBER_TOP)*2) {
            bits = 16;
        } else {
            if (MTK_LOG_ENABLE == 1) {
                SLOGE("[show_animation_common: %s %d]Logo data error\n",__FUNCTION__,__LINE__);
            }
            return;
        }
        if (MTK_LOG_ENABLE == 1) {
            SLOGD("[show_animation_common: %s %d]bits = %d\n",__FUNCTION__,__LINE__, bits);
        }
    }
    if (MTK_LOG_ENABLE == 1) {
        SLOGD("[show_animation_common: %s %d]bits = %d\n",__FUNCTION__,__LINE__, bits);
    }
    fill_rect_with_content(fill_addr, battery_number_rect, number_pic_addr,phical_screen, bits);
}

/*
 * Fill a line with special color
 *
 * fill_animation_line() —— 绘制电池容量柱状图（竖线）
        实际是 从下往上填充像素行
        capacity_grids 决定填充到哪一行
        每行用 line_pic_addr（可能是一条渐变线）绘制
 */
void fill_animation_line(int index, unsigned int capacity_grids, void *fill_addr,  void * logo_addr, LCM_SCREEN_T phical_screen)
{
    LOGO_PARA_T logo_info;
    int raw_data_size;
    if(check_logo_index_valid(index, logo_addr, &logo_info) != CHECK_LOGO_BIN_OK)
        return;

    raw_data_size = decompress_logo((void*)logo_info.inaddr, (void*)line_pic_addr, logo_info.logolen, line_pic_size);
    if (MTK_LOG_ENABLE == 1) {
        SLOGD("[show_animation_common: %s %d]raw_data_size = %d\n",__FUNCTION__,__LINE__, raw_data_size);
    }
    if (0 == bits) {
        if (raw_data_size == (TOP_ANIMATION_RIGHT - TOP_ANIMATION_LEFT)*4) {
            bits = 32;
        } else if (raw_data_size == (TOP_ANIMATION_RIGHT - TOP_ANIMATION_LEFT)*2) {
            bits = 16;
        } else {
            if (MTK_LOG_ENABLE == 1) {
                SLOGE("[show_animation_common: %s %d]Logo data error\n",__FUNCTION__,__LINE__);
            }
            return;
        }
        if (MTK_LOG_ENABLE == 1) {
            SLOGD("[show_animation_common: %s %d]bits = %d\n",__FUNCTION__,__LINE__, bits);
        }
    }
    RECT_REGION_T rect = {CAPACITY_LEFT, CAPACITY_TOP, CAPACITY_RIGHT, CAPACITY_BOTTOM};
    int i = capacity_grids;
    for(; i < CAPACITY_BOTTOM; i++)
    {
        rect.top = i;
        rect.bottom = i+1;
        fill_rect_with_content(fill_addr, rect, line_pic_addr, phical_screen, bits);
    }
}



/*
 * Show old charging animation
 *
 * fill_animation_battery_old() —— 旧版动画（VERION_OLD_ANIMATION）
        类似 Android 早期风格：分段式电池图标 + 进度条
        使用 version0_charging_index 控制动画帧（缓慢增长）
        背景图 + 两层进度条（occupied + empty）
 */
void fill_animation_battery_old(int capacity,  void *fill_addr, void * dec_logo_addr, void * logo_addr,
                       LCM_SCREEN_T phical_screen)
{
    int capacity_grids = 0;
    if (capacity > 100) capacity = 100;
    capacity_grids = (capacity * (ANIM_V0_REGIONS)) / 100;

    if (version0_charging_index < capacity_grids * 2)
        version0_charging_index = capacity_grids * 2;

    if (capacity < 100){
        version0_charging_index > 7? version0_charging_index = capacity_grids * 2 : version0_charging_index++;
    } else {
        version0_charging_index = ANIM_V0_REGIONS * 2;
    }

    fill_animation_logo(ANIM_V0_BACKGROUND_INDEX, fill_addr, dec_logo_addr, logo_addr,phical_screen);

    RECT_REGION_T rect_bar = {bar_rect.left + 1, bar_rect.top + 1, bar_rect.right, bar_rect.bottom};

    fill_animation_prog_bar(rect_bar,
                       (unsigned int)(BAR_OCCUPIED_COLOR),
                       0,  version0_charging_index/2,
                       fill_addr, phical_screen);

    fill_animation_prog_bar(rect_bar,
                      (unsigned int)(BAR_EMPTY_COLOR),
                      version0_charging_index/2, ANIM_V0_REGIONS - version0_charging_index/2,
                      fill_addr, phical_screen);
}

/*
 * Show new charging animation
 *
 * fill_animation_battery_new() —— 新版动画（VERION_NEW_ANIMATION）
        更现代：数字电量 + 动态波浪/光效
        分三种情况：
            ≥100%：显示满电图标
            <10%：播放低电量动画（9帧循环），只显示个位数
            10%~99%：
                显示两位数字（十位+个位）
                绘制底部容量柱（fill_animation_line）
                在容量线上方叠加动态光效（top_animation_rect，随电量上升）
                显示“%”符号
 */
void fill_animation_battery_new(int capacity, void *fill_addr, void * dec_logo_addr, void * logo_addr, LCM_SCREEN_T phical_screen)
{
    if (MTK_LOG_ENABLE == 1) {
        SLOGD("[show_animation_common: %s %d]capacity : %d\n",__FUNCTION__,__LINE__, capacity);
    }

    if (capacity >= 100) {
        //show_logo(37); // battery 100
        fill_animation_logo(FULL_BATTERY_INDEX, fill_addr, dec_logo_addr, logo_addr,phical_screen);

    } else if (capacity < 10) {
        if (MTK_LOG_ENABLE == 1) {
            SLOGD("[show_animation_common: %s %d]charging_low_index = %d\n",__FUNCTION__,__LINE__, charging_low_index);
        }
        charging_low_index ++ ;

        fill_animation_logo(LOW_BAT_ANIM_START_0 + charging_low_index, fill_addr, dec_logo_addr, logo_addr,phical_screen);
        fill_animation_number(NUMBER_PIC_START_0 + capacity, 1, fill_addr, logo_addr, phical_screen);
        fill_animation_dynamic(NUMBER_PIC_PERCENT, percent_location_rect, fill_addr, percent_pic_addr, logo_addr, phical_screen);

        if (charging_low_index >= 9) charging_low_index = 0;

    } else {

        unsigned int capacity_grids = 0;
        //static RECT_REGION_T battery_rect = {CAPACITY_LEFT,CAPACITY_TOP,CAPACITY_RIGHT,CAPACITY_BOTTOM};
        capacity_grids = CAPACITY_BOTTOM - (CAPACITY_BOTTOM - CAPACITY_TOP) * (capacity - 10) / 90;
        if (MTK_LOG_ENABLE == 1) {
            SLOGD("[show_animation_common: %s %d]capacity_grids : %d,charging_animation_index = %d\n"
                     ,__FUNCTION__,__LINE__, capacity_grids,charging_animation_index);
        }

        //background
        fill_animation_logo(ANIM_V1_BACKGROUND_INDEX, fill_addr, dec_logo_addr, logo_addr,phical_screen);

        fill_animation_line(ANIM_LINE_INDEX, capacity_grids, fill_addr,  logo_addr, phical_screen);
        fill_animation_number(NUMBER_PIC_START_0 + (capacity/10), 0, fill_addr, logo_addr, phical_screen);
        fill_animation_number(NUMBER_PIC_START_0 + (capacity%10), 1, fill_addr, logo_addr, phical_screen);
        fill_animation_dynamic(NUMBER_PIC_PERCENT, percent_location_rect, fill_addr, percent_pic_addr, logo_addr, phical_screen);


         if (capacity <= 90)
         {
            RECT_REGION_T top_animation_rect = {TOP_ANIMATION_LEFT, capacity_grids - (TOP_ANIMATION_BOTTOM - TOP_ANIMATION_TOP), TOP_ANIMATION_RIGHT, capacity_grids};
            //top_animation_rect.bottom = capacity_grids;
            //top_animation_rect.top = capacity_grids - top_animation_height;
            charging_animation_index++;
            //show_animation_dynamic(15 + charging_animation_index, top_animation_rect, top_animation_addr);
            fill_animation_dynamic(BAT_ANIM_START_0 + charging_animation_index, top_animation_rect, fill_addr,
                            top_animation_addr, logo_addr, phical_screen);

            if (charging_animation_index >= 9) charging_animation_index = 0;
         }
    }

}

/*
 * Show wireless charging animation
 * total 29 logo:from 39 ~ 68
 * less(0<10): 50-53 , low(<30):54-57 ,middle(<60):58-61 , high():62-75 , o:66, full:67,num (0-9):39-48, %:49
 *
 * fill_animation_battery_wireless_charging() —— 无线充电动画
        使用独立的图像资源（索引 39~68）
        根据电量区间选择不同背景（0-10, 10-40, 40-80, 80-100）
        每个区间有 4 帧动画（charging_low_index % 4）
        数字和“%”绘制在屏幕中央偏上位置（动态计算坐标）
 */

 void fill_animation_battery_wireless_charging(int capacity, void *fill_addr, void * dec_logo_addr, void * logo_addr, LCM_SCREEN_T phical_screen)
{
    if (MTK_LOG_ENABLE == 1) {
        SLOGD("[show_animation_common: %s %d]capacity : %d\n",__FUNCTION__,__LINE__, capacity);
    }
//    RECT_REGION_T wireless_bgd_rect = {0, 0, phical_screen.width, phical_screen.height};

    charging_low_index >= 3? charging_low_index = 0:charging_low_index++;
    if (MTK_LOG_ENABLE == 1) {
        SLOGD("[show_animation_common: %s %d]charging_low_index = %d\n",__FUNCTION__,__LINE__, charging_low_index);
    }

    if (capacity >= 100) {
         // battery 100
        fill_animation_logo(V2_BAT_100_INDEX, fill_addr, dec_logo_addr, logo_addr,phical_screen);
    } else if (capacity <= 0) {
        fill_animation_logo(V2_BAT_0_INDEX, fill_addr, dec_logo_addr, logo_addr,phical_screen);
    } else {
        int bg_index = V2_BAT_0_10_START_INDEX; //capacity > 0 && capacity < 10
        if (capacity >= 10 && capacity < 40) {
            bg_index = V2_BAT_10_40_START_INDEX;
        } else if (capacity >= 40 && capacity < 80) {
            bg_index = V2_BAT_40_80_START_INDEX;
        } else if (capacity >= 80 && capacity < 100) {
            bg_index = V2_BAT_80_100_START_NDEX;
        }
        fill_animation_logo(bg_index + charging_low_index, fill_addr, dec_logo_addr, logo_addr,phical_screen);
        RECT_REGION_T tmp_rect = {(int)phical_screen.width * 4/10,
                        (int) phical_screen.height * 1/6,
                        (int)phical_screen.width* 5/10,
                        (int)phical_screen.height*16/60};
        unsigned short tmp_num_addr[(int)phical_screen.width * phical_screen.height/100]; //addr

        if (capacity >= 10) {
            if (MTK_LOG_ENABLE == 1) {
                SLOGD("[show_animation_common: %s %d]tmp_rect left = %d, right = %d,top = %d,bottom = %d,\n",__FUNCTION__,__LINE__,
                            tmp_rect.left,tmp_rect.right,tmp_rect.top,tmp_rect.bottom);
            }

            fill_animation_dynamic(V2_NUM_START_0_INDEX + (capacity/10), tmp_rect, fill_addr, tmp_num_addr, logo_addr, phical_screen);
            tmp_rect.left += (int)phical_screen.width /10;
            tmp_rect.right += (int)phical_screen.width /10;
        }

        if (MTK_LOG_ENABLE == 1) {
            SLOGD("[show_animation_common: %s %d]tmp_rect left = %d, right = %d,top = %d,bottom = %d,\n",__FUNCTION__,__LINE__,
                    tmp_rect.left,tmp_rect.right,tmp_rect.top,tmp_rect.bottom);
        }

        fill_animation_dynamic(V2_NUM_START_0_INDEX + (capacity%10), tmp_rect, fill_addr, tmp_num_addr, logo_addr, phical_screen);

        tmp_rect.left += (int)phical_screen.width /10;
        tmp_rect.right += (int)phical_screen.width /10;
        if (MTK_LOG_ENABLE == 1) {
            SLOGD("[show_animation_common: %s %d]tmp_rect left = %d, right = %d,top = %d,bottom = %d,\n",__FUNCTION__,__LINE__,
                            tmp_rect.left,tmp_rect.right,tmp_rect.top,tmp_rect.bottom);
        }

        fill_animation_dynamic(V2_NUM_PERCENT_INDEX, tmp_rect, fill_addr, tmp_num_addr, logo_addr, phical_screen);
    }
}

/*
 * Pump charging aniamtion
 * index 39: 100%, 40~45 : animation logo, 46~55: number logo, 56: % logo
 *
 * 类似无线充，但使用另一套资源（FAST_CHARGING_*_INDEX）
        支持屏幕旋转（90°/270°）
        数字居中显示，带顶部 margin
        动画帧数为 6（charging_low_index % 6）
 */
void fill_animation_battery_fast_charging(int capacity, void *fill_addr, void * dec_logo_addr, void * logo_addr, LCM_SCREEN_T phical_screen, int draw_anim_mode)
{
    int display_width = phical_screen.width;
    int display_height = phical_screen.height;
    int curr_left = 0;
    int num_width = 0;
    int num_height = 0;
    int top_margin_height = 0;
    if(draw_anim_mode == DRAW_ANIM_MODE_FB){
	if (0 == strncmp(MTK_LCM_PHYSICAL_ROTATION, "90", 2) ||
	     0 == strncmp(MTK_LCM_PHYSICAL_ROTATION, "270", 3)) {
	     display_width = phical_screen.height;
	     display_height = phical_screen.width;
	    }
	}
    num_width = LOGO_NUM_WIDTH(display_width);
    num_height = LOGO_NUM_HEIGHT(display_height);
    top_margin_height = LOGO_TOP_MARGIN(display_height);
    if (MTK_LOG_ENABLE == 1) {
        SLOGD("[show_animation_common: %s %d]capacity : %d, num_width:%d, num_height:%d, top_margin_height:%d\n", __FUNCTION__,__LINE__, capacity, num_width, num_height, top_margin_height);
    }

    charging_low_index >= 5? charging_low_index = 0:charging_low_index++;
    if (MTK_LOG_ENABLE == 1) {
        SLOGD("[show_animation_common: %s %d]charging_low_index = %d\n",__FUNCTION__,__LINE__, charging_low_index);

        SLOGD("[show_animation_common: %s %d]capacity : %d\n",__FUNCTION__,__LINE__, capacity);
    }

    if (capacity >= 100) {
        fill_animation_logo(FAST_CHARGING_BAT_100_INDEX, fill_addr, dec_logo_addr, logo_addr, phical_screen);
    } else {
        fill_animation_logo(FAST_CHARGING_BAT_START_0_INDEX + charging_low_index, fill_addr, dec_logo_addr, logo_addr, phical_screen);

        curr_left = (display_width - num_width * 2) >> 1;
        unsigned short tmp_num_addr[(int)phical_screen.width * phical_screen.height/20];

        if (capacity >= 10) {
            curr_left = (display_width - num_width * 3) >> 1;
            RECT_REGION_T tmp_rect_1 = {curr_left, top_margin_height, curr_left + num_width, top_margin_height + num_height};
            if (MTK_LOG_ENABLE == 1) {
                SLOGD("[show_animation_common: %s %d]capacity = %d, show 1nd num : %d\n",__FUNCTION__,__LINE__, capacity, capacity/10);
            }
            fill_animation_dynamic(FAST_CHARGING_NUM_START_0_INDEX + (capacity/10), tmp_rect_1, fill_addr, tmp_num_addr, logo_addr, phical_screen);
            curr_left += num_width;
        }
        RECT_REGION_T tmp_rect_2 = {curr_left, top_margin_height, curr_left + num_width, top_margin_height + num_height};
        if (MTK_LOG_ENABLE == 1) {
            SLOGD("[show_animation_common: %s %d]capacity = %d, show 2st num : %d\n",__FUNCTION__,__LINE__, capacity, capacity%10);
        }
        fill_animation_dynamic(FAST_CHARGING_NUM_START_0_INDEX + (capacity%10), tmp_rect_2, fill_addr, tmp_num_addr, logo_addr, phical_screen);

        curr_left += num_width;
        RECT_REGION_T tmp_rect_3 = {curr_left, top_margin_height, curr_left + num_width, top_margin_height + num_height};
        if (MTK_LOG_ENABLE == 1) {
            SLOGD("[show_animation_common: %s %d]show percent \n",__FUNCTION__,__LINE__);
        }
        fill_animation_dynamic(FAST_CHARGING_NUM_PERCENT_INDEX, tmp_rect_3, fill_addr, tmp_num_addr, logo_addr, phical_screen);
    }
}

/*
 * Show charging animation by version
 *统一入口：fill_animation_battery_by_ver()
 * 根据 version 参数选择动画类型：
        VERION_OLD_ANIMATION → 老式
        VERION_NEW_ANIMATION → 新式
        VERION_WIRELESS_CHARGING_ANIMATION → 无线充
        默认回退到老式
        ⚠️ 注意：VERION 拼写错误，应为 VERSION，但可能是历史遗留。
 */
void fill_animation_battery_by_ver(int capacity, void *fill_addr, void * dec_logo_addr, void * logo_addr,
                        LCM_SCREEN_T phical_screen, int version)
{
    if (MTK_LOG_ENABLE == 1) {
        SLOGD("[show_animation_common: %s %d]version : %d\n",__FUNCTION__,__LINE__, version);
    }
    switch (version)
    {
        case VERION_OLD_ANIMATION:
            fill_animation_battery_old(capacity, fill_addr, dec_logo_addr, logo_addr, phical_screen);

            break;
        case VERION_NEW_ANIMATION:
            fill_animation_battery_new(capacity, fill_addr, dec_logo_addr, logo_addr, phical_screen);

            break;
        case VERION_WIRELESS_CHARGING_ANIMATION:
            fill_animation_battery_wireless_charging(capacity, fill_addr, dec_logo_addr, logo_addr, phical_screen);

            break;
        default:
            fill_animation_battery_old(capacity, fill_addr, dec_logo_addr, logo_addr, phical_screen);

            break;
    }
}
