/* Copyright Statement:
*                                                                                                            
* This software/firmware and related documentation ("MediaTek Software") are                                 
* protected under relevant copyright laws. The information contained herein                                  
* is confidential and proprietary to MediaTek Inc. and/or its licensors.                                     
* Without the prior written permission of MediaTek inc. and/or its licensors,                                
* any reproduction, modification, use or disclosure of MediaTek Software,                                    
* and information contained herein, in whole or in part, shall be strictly prohibited.                       
*/     
#include <ctype.h>                                                                                           
#include <errno.h>                                                                                           
#include <fcntl.h>                                                                                           
#include <getopt.h>                                                                                          
#include <limits.h>                                                                                          
#include <linux/input.h>                                                                                     
#include <stdio.h>                                                                                           
#include <stdlib.h>                                                                                          
#include <string.h>                                                                                          
#include <sys/reboot.h>                                                                                      
#include <sys/types.h>                                                                                       
#include <time.h>                                                                                            
#include <unistd.h>                                                                                                                                                                                                     
#include <cutils/properties.h>
#include "me_connection.h"
#include <pthread.h>
#include "common.h"
#include "ftm.h"
#include "miniui.h"
#include "utils.h"
#include "item.h"
#if defined(MTK_FTM_C2K_SUPPORT) && !defined(EVDO_FTM_DT_VIA_SUPPORT)
#include <c2kutils.h>
#endif
#include "at_command.h"
#define TAG        "[GMS] "                                                                              
#define MAX_MODEM_INDEX 4
int g_gms_nr_lines; 
                                                                                             
static char s_gms_info[1024] = {'\0'};                                                                             
                                                                                                          
extern int textview_key_handler(int key, void *priv);                                                        
extern int write_test_report(item_t *items, FILE *fp);                                                       
extern char SP_ATA_PASS[16];                                                                                 
//extern char SP_ATA_FAIL[16] = "fail\r\n";                                                                                                
#define MAX_RETRY_COUNT 20
                                                         
void print_gms(char *info, int *len, char *tag, char *msg)                                               
{                                                                                                            
	char buf[256] = {0};                                                                                       
	int _len = 0;                                                                                              
	int tag_len = 0;                                                                                           
	int max_len=0;
    if((info == NULL) || (len == NULL) || (tag == NULL) || (msg == NULL))                                    
    {                                                                                                        
        return;                                                                                              
    }                                                                                                        
                                                                                                             
    _len = *len;                                                                                             
    tag_len = strlen(tag);                                                                                   
	//int max_len = gr_fb_width() / CHAR_WIDTH *2;
	#if defined SUPPORT_GB2312
	    max_len = gr_fb_width() / CHAR_WIDTH*2;
	#else
	    max_len = gr_fb_width() / CHAR_WIDTH;
	#endif

	int msg_len = strlen(msg);                                                                                 
                                                                                                             
	int buf_len = gr_fb_width() / CHAR_WIDTH;                                                                  
                                                                                                             
	_len += sprintf(info + _len, "%s", tag);                                                                   
	_len += sprintf(info + _len, ": ");                                                                        
                                                                                                             
	if(msg_len>max_len-tag_len-2)                                                                              
    {                                                                                                        
		_len += sprintf(info+_len,"\n    ");                                                                     
		g_gms_nr_lines++;                                                                                            
	}                                                                                                          
                                                                                                             
	while(msg_len>0)                                                                                           
    {                                                                                                        
		buf_len = max_len - 4;                                                                                   
		buf_len = (msg_len > buf_len ? buf_len : msg_len);                                                       
		strncpy(buf, msg, 256);                                                                              
		buf[buf_len] = 0;                                                                                        
                                                                                                             
		_len += sprintf(info + _len, "%s", buf);                                                                 
		_len += sprintf(info + _len, "\n");                                                                      
		g_gms_nr_lines++;                                                                                            
		msg_len-=buf_len;                                                                                        
		msg = &(msg[buf_len]);                                                                                   
		while(msg_len>0 && msg[0]==' ')                                                                          
        {                                                                                                    
			msg_len--;                                                                                             
			msg = &(msg[1]);                                                                                       
		}                                                                                                        
                                                                                                             
		if(msg_len>0)                                                                                            
        {                                                                                                    
			for(buf_len=0; buf_len < 4; buf_len++) buf[buf_len]=' ';                                               
			buf[buf_len]=0;                                                                                        
			//_len += sprintf(info+_len, buf);                                                                     
			// Fix Anroid 2.3 build error                                                                          
			_len += sprintf(info + _len, "%s", buf);                                                               
		}                                                                                                        
                                                                                                             
	}                                                                                                          
	*len = _len;                                                                                               
	//LOGD(TAG "In factory mode: g_gms_nr_lines = %d\n", g_gms_nr_lines);                                              
}                                                                                                                                                                                                                   
 
                                                                                                           
static int get_gms_value(char *info, int *len)                                                           
{                                                                                                            
                             
    char info_product[32], info_vendor_product[32] = {"\0"};
    char info_fingerprint[92], info_vendor_fingerprint[92] = {"\0"};
    char info_manufacturer[32], info_vendor_manufacturer[32] = {"\0"};
    char info_brand[32], info_vendor_brand[32] = {"\0"};
    char info_device[32], info_vendor_device[32] = {"\0"};
    char info_model[32], info_vendor_model[32] = {"\0"};
    char info_inc[32];
    char info_kernel_v[32];
    char info_clientid[32];
    char info_osversion[32];
    char propbuf[PROPERTY_VALUE_MAX];   
    int isdk = 0;
    bool bsucess = true;
                                                                                 
    property_get("ro.product.name", propbuf, "null");
    strcpy(info_product, propbuf);
    property_get("ro.product.model", propbuf, "null");
    strcpy(info_model, propbuf);
    property_get("ro.product.brand", propbuf, "null");
    strcpy(info_brand, propbuf);
    property_get("ro.product.device", propbuf, "null"); 
    strcpy(info_device, propbuf);
    property_get("ro.build.version.incremental", propbuf, "null");
    strcpy(info_inc, propbuf);
    property_get("ro.build.fingerprint", propbuf, "null");
    strcpy(info_fingerprint, propbuf);
    property_get("ro.product.manufacturer", propbuf, "null");
    strcpy(info_manufacturer, propbuf);
    property_get("ro.vendor.build_clientidbase", propbuf, "null");
    strcpy(info_clientid, propbuf);
    property_get("ro.build.version.sdk", propbuf, "null");
    strcpy(info_osversion, propbuf);

    isdk = atoi(info_osversion);

    if ( isdk >=26 ) {
        property_get("ro.product.vendor.name", propbuf, "null");
	 strcpy(info_vendor_product, propbuf);
        property_get("ro.product.vendor.model", propbuf, "null");
	 strcpy(info_vendor_model, propbuf);
        property_get("ro.product.vendor.brand", propbuf, "null");
	 strcpy(info_vendor_brand, propbuf);
        property_get("ro.product.vendor.device", propbuf, "null");
	 strcpy(info_vendor_device, propbuf);
        property_get("ro.vendor.build.fingerprint", propbuf, "null");
	 strcpy(info_vendor_fingerprint, propbuf);
        property_get("ro.product.vendor.manufacturer", propbuf, "null");
	 strcpy(info_vendor_manufacturer, propbuf);

	 bsucess = ((0 == strcmp(info_vendor_device, info_device))
		 && (0 == strcmp(info_vendor_brand, info_brand))
		 && (0 == strcmp(info_vendor_fingerprint, info_fingerprint) )
		 && (0 == strcmp(info_vendor_manufacturer, info_manufacturer))
		 && (0 == strcmp(info_vendor_model, info_model))
		 && (0 == strcmp(info_vendor_product, info_product)));

        if ( !bsucess ) {
		char einfo[1024];
		LOGE("system properties not same!!\n");
		if ( 0 != strcmp(info_vendor_device, info_device) ) {
			sprintf(einfo, "ro.product.vendor.device (%s) not equal ro.product.device (%s)\n", info_vendor_device, info_device);
			print_gms(info, len,  "GMS     ", einfo);
		}
		if ( 0 != strcmp(info_vendor_model, info_model) ) {
			sprintf(einfo, "ro.product.vendor.model (%s) not equal ro.product.model (%s)\n", info_vendor_model, info_model);
			print_gms(info, len,  "GMS     ", einfo);
		}
		if ( 0 != strcmp(info_vendor_brand, info_brand) ) {
			sprintf(einfo, "ro.product.vendor.brand (%s) not equal ro.product.brand (%s)\n", info_vendor_brand, info_brand);
			print_gms(info, len,  "GMS     ", einfo);
		}
		if ( 0 != strcmp(info_vendor_product, info_product) ) {
			sprintf(einfo, "ro.product.vendor.name (%s) not equal ro.product.name (%s)\n", info_vendor_product, info_product);
			print_gms(info, len,  "GMS     ", einfo);
		}
		if ( 0 != strcmp(info_vendor_fingerprint, info_fingerprint) ) {
			sprintf(einfo, "ro.vendor.build.fingerprint (%s) not equal ro.build.fingerprint (%s)\n", info_vendor_fingerprint, info_fingerprint);
			print_gms(info, len,  "GMS     ", einfo);
		}
		if ( 0 != strcmp(info_vendor_manufacturer, info_manufacturer) ) {
			sprintf(einfo, "ro.product.vendor.manufacturer (%s) not equal ro.product.manufacturer (%s)\n", info_vendor_manufacturer, info_manufacturer);
			print_gms(info, len,  "GMS     ", einfo);
		}
		
        }
    } 
    

    //[ok|ng];[$product];[$model];[$device];[$manufacturer];[$brand];[$fingerprint]

    sprintf(s_gms_info, "%s<|>%s<|>%s<|>%s<|>%s<|>%s<|>%s<|>%s<|>%s", 
             bsucess ? "ok" : "ng", info_product, info_model, info_device, info_manufacturer, info_brand, info_fingerprint, info_inc, info_clientid);
    if ( bsucess ) {
        print_gms(info, len,  "GMS     ", s_gms_info);                                                          
    }
                                                                                                                                          
                                                                                                         
    return *len;                                                                                              
}                                                                                                            

                                                                                                             
int get_gms(char *info, int size)                                                              
{                                                                                                            
                                                                                                             
    int len = 0;                                                                                             
    g_gms_nr_lines = 0;                                                                                            
                                                                                                             
    get_gms_value(info, &len);                                                                                                                                               
                                                                                                             
    return 0;                                                                                                
}                                                                                                            
 
                                                                                                            
char ** trans_gms(const char *str, int *line)                                                            
{                                                                                                            
	char **pstrs = NULL;                                                                                       
	int  len     = 0;                                                                                          
	int  row     = 0;                                                                                          
	const char *start;                                                                                         
	const char *end;                                                                                           
                                                                                                             
    if((str == NULL) || (line == NULL))                                                                      
    {                                                                                                        
        return NULL;                                                                                         
    }                                                                                                        
                                                                                                             
    len = strlen(str) + 1;                                                                                   
    start  = str;                                                                                            
    end    = str;                                                                                            
    pstrs = (char**)malloc(g_gms_nr_lines * sizeof(char*));                                                      
                                                                                                             
	if (!pstrs)                                                                                                
    {                                                                                                        
		LOGE("In factory mode: malloc failed\n");                                                                
		return NULL;                                                                                             
	}                                                                                                          
                                                                                                             
	while (len--)                                                                                              
    {                                                                                                        
		if ('\n' == *end)                                                                                        
        {                                                                                                    
			pstrs[row] = (char*)malloc((end - start + 1) * sizeof(char));                                          
                                                                                                             
			if (!pstrs[row])                                                                                       
            {                                                                                                
				LOGE("In factory mode: malloc failed\n");                                                            
				return NULL;                                                                                         
			}                                                                                                      
                                                                                                             
			strncpy(pstrs[row], start, end - start);                                                               
			pstrs[row][end - start] = '\0';                                                                        
			start = end + 1;                                                                                       
			row++;                                                                                                 
		}                                                                                                        
		end++;                                                                                                   
	}                                                                                                          
                                                                                                             
	*line = row;                                                                                               
	return pstrs;                                                                                              
}                                                                                                            
                                                                                                             
void gms_tear_down(char **pstr, int row)                                                                         
{                                                                                                            
    int i;                                                                                                   
    if(pstr == NULL)                                                                                         
    {                                                                                                        
        return;                                                                                              
    }                                                                                                        
    for (i = 0; i < row; i++)                                                                                
    {                                                                                                        
        if (pstr[i])                                                                                         
        {                                                                                                    
            free(pstr[i]);                                                                                   
            pstr[i] = NULL;                                                                                  
        }                                                                                                    
    }                                                                                                        
	                                                                                                           
    if (pstr)                                                                                                
    {                                                                                                        
        free(pstr);                                                                                          
        pstr = NULL;                                                                                         
    }                                                                                                        
}                                                                                                            
                                                                                                             
                                                                                                             
/*                                                                                                           
    autoreturn:  if the function called by ata, then true;                                                   
    if called by main, then false;                                                                           
*/                                                                                                           
char* display_gms_ata(int index, char* result)                                                           
{                                                                                                            
    if(result == NULL)                                                                                       
    {                                                                                                        
        return NULL;                                                                                         
    }                                                                                                        
    return display_gms(index, result, true);	                                                           
}                                                                                                            


                                                                                                          
char* display_gms(int index, char* result, bool autoreturn)                                              
{                                                                                                            
	char *buf = NULL;                                                                                          
	struct textview vi;	 /* version info */                                                                    
	text_t vi_title;                                                                                           
	int nr_line;                                                                                               
	text_t info;                                                                                               
	int avail_lines = 0;                                                                                       
	text_t rbtn;                                                                                               
                                                                                                             
	buf = (char *)malloc(BUFSZ);
                                                                                                             
	init_text(&vi_title, uistr_gms, COLOR_YELLOW);                                                         
	init_text(&info, buf, COLOR_YELLOW);
                                                                                                             
	avail_lines = get_avail_textline();                                                                        
	init_text(&rbtn, uistr_key_back, COLOR_YELLOW);                                                            
	ui_init_textview(&vi, textview_key_handler, &vi);                                                          
	vi.set_btn(&vi, NULL, NULL, &rbtn);                                                                        
 	get_gms(buf, BUFSZ);                                                                                
	LOGD(TAG "after get_gms");                                                                          
	vi.set_title(&vi, &vi_title);                                                                              
	vi.set_text(&vi, &info);                                                                                   
	vi.m_pstr = trans_gms(info.string, &nr_line);                                                          
	vi.m_nr_lines = g_gms_nr_lines;                                                                                
	LOGD(TAG "g_gms_nr_lines is %d, avail_lines is %d\n", g_gms_nr_lines, avail_lines);                                
	vi.m_start = 0;                                                                                            
	vi.m_end = (nr_line < avail_lines ? nr_line : avail_lines);                                                
	LOGD(TAG "vi.m_end is %d, info: %s\n", vi.m_end, s_gms_info);                                                                    
                                                                                                             
    if(autoreturn)                                                                                           
    {                                                                                                        
    	vi.redraw(&vi);      
	sprintf(result, "%s\r\n", s_gms_info);      
	//strcpy(result, SP_ATA_PASS);      
    }                                                                                                        
    else                                                                                                     
    {                                                                                                        
        vi.run(&vi);                                                                                         
    }                                                                                                        
                                                                                                             
	LOGD(TAG "Before gms_tear_down\n");                                                                            
	gms_tear_down(vi.m_pstr, nr_line);                                                                             
	if (buf)                                                                                                   
    {                                                                                                        
		free(buf);                                                                                               
        buf = NULL;                                                                                          
    }                                                                                                        
    LOGD(TAG "End of %s\n", __FUNCTION__);                                                                   
                                                                                                             
    return SP_ATA_PASS;                                                                                      
}                                                                                                            
                                                                                                             

