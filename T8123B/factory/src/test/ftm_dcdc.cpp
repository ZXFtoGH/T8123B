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
#define TAG        "[DCDC] "                                                                              
#define MAX_MODEM_INDEX 4
int g_dcdc_nr_lines; 
bool g_dcdc_state = false;
                                                                                             
extern sp_ata_data return_data;
extern char test_data[128];                                                                                  
                                                                                                          
extern int textview_key_handler(int key, void *priv);                                                        
extern int write_test_report(item_t *items, FILE *fp);                                                       
extern char SP_ATA_PASS[16];    
extern char SP_ATA_FAIL[16];                                                                             
                                                                                              
#define MAX_RETRY_COUNT 20
                                                         
void print_dcdc_state(char *info, int *len, char *tag, char *msg)                                               
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
		g_dcdc_nr_lines++;                                                                                            
	}                                                                                                          
                                                                                                             
	while(msg_len>0)                                                                                           
    {                                                                                                        
		buf_len = max_len - 4;                                                                                   
		buf_len = (msg_len > buf_len ? buf_len : msg_len);                                                       
		strncpy(buf, msg, 256);                                                                              
		buf[buf_len] = 0;                                                                                        
                                                                                                             
		_len += sprintf(info + _len, "%s", buf);                                                                 
		_len += sprintf(info + _len, "\n");                                                                      
		g_dcdc_nr_lines++;                                                                                            
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
	//LOGD(TAG "In factory mode: g_dcdc_nr_lines = %d\n", g_dcdc_nr_lines);                                              
}                                                                                                                                                                                                                   
 
                                                                                                           
static int get_dcdc_value(char *info, int *len)                                                           
{                                                                                                                                                                                             
    char val[128] = {0};                                                                                     
    char dcdc_var[32] = "unknown";                                                                        
                                                                 
    int dcdc_fd = -1;                                                                                  
                                                                                
    char buffer[1024] = {0};                                                                                 
    char *ptr= NULL, *pstr = NULL;                                                                           
    int i = 0;  

    g_dcdc_state = false;	
                                                                                                             
    dcdc_fd = open("/sys/bus/i2c/devices/2-0060/name",O_RDONLY);                                                          
    if(dcdc_fd!=-1)                                                                                    
    {                                                                                                        
        read(dcdc_fd, dcdc_var, 32);                                                                
        close(dcdc_fd);                                                                                
    }                                                                                                        
                                                                                    
    if(dcdc_var[strlen(dcdc_var)-1]=='\n') dcdc_var[strlen(dcdc_var)-1]=0;                           
                                                                                                             
                                                   
    print_dcdc_state(info, len,  "DCDC     ", dcdc_var);       

    if ( 0 == strcmp(dcdc_var, "unknown") ) {
		g_dcdc_state = false;
	} else {
		g_dcdc_state = true;
    }		
                                                                                                             
    return *len;                                                                                              
}                                                                                                            

                                                                                                             
int get_dcdc_state(char *info, int size)                                                              
{                                                                                                            
                                                                                                             
    int len = 0;                                                                                             
    g_dcdc_nr_lines = 0;                                                                                            
                                                                                                             
    get_dcdc_value(info, &len);                                                                                                                                               
                                                                                                             
    return 0;                                                                                                
}                                                                                                            
 
                                                                                                            
char ** trans_dcdc(const char *str, int *line)                                                            
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
    pstrs = (char**)malloc(g_dcdc_nr_lines * sizeof(char*));                                                      
                                                                                                             
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
                                                                                                             
void dcdc_tear_down(char **pstr, int row)                                                                         
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
char* display_dcdc_state_ata(int index, char* result)                                                           
{                                                                                                            
    if(result == NULL)                                                                                       
    {                                                                                                        
        return NULL;                                                                                         
    }                                                                                                        
    return display_dcdc_state(index, result, true);	                                                           
}                                                                                                            

                                                                                                          
char* display_dcdc_state(int index, char* result, bool autoreturn)                                              
{                                                                                                            
	char *buf = NULL;                                                                                          
	struct textview vi;	 /* version info */                                                                    
	text_t vi_title;                                                                                           
	int nr_line;                                                                                               
	text_t info;                                                                                               
	int avail_lines = 0;                                                                                       
	text_t rbtn;                                                                                               
                                                                                                             
	buf = (char *)malloc(BUFSZ);
                                                                                                             
	init_text(&vi_title, uistr_ntc, COLOR_YELLOW);                                                         
	init_text(&info, buf, COLOR_YELLOW);
                                                                                                             
	avail_lines = get_avail_textline();                                                                        
	init_text(&rbtn, uistr_key_back, COLOR_YELLOW);                                                            
	ui_init_textview(&vi, textview_key_handler, &vi);                                                          
	vi.set_btn(&vi, NULL, NULL, &rbtn);                                                                        
 	get_dcdc_state(buf, BUFSZ);                                                                                
	LOGD(TAG "after get_ntc");                                                                          
	vi.set_title(&vi, &vi_title);                                                                              
	vi.set_text(&vi, &info);                                                                                   
	vi.m_pstr = trans_dcdc(info.string, &nr_line);                                                          
	vi.m_nr_lines = g_dcdc_nr_lines;                                                                                
	LOGD(TAG "g_dcdc_nr_lines is %d, avail_lines is %d\n", g_dcdc_nr_lines, avail_lines);                                
	vi.m_start = 0;                                                                                            
	vi.m_end = (nr_line < avail_lines ? nr_line : avail_lines);                                                
	LOGD(TAG "vi.m_end is %d, ntc: %d\n", vi.m_end, return_data.ntc.ntc);                                                                    
                                                                                                             
    if(autoreturn)                                                                                           
    {                                                                                                        
    	vi.redraw(&vi);           
	    strcpy(result, g_dcdc_state ? SP_ATA_PASS : SP_ATA_FAIL);      
    }                                                                                                        
    else                                                                                                     
    {                                                                                                        
        vi.run(&vi);                                                                                         
    }                                                                                                        
                                                                                                             
	LOGD(TAG "Before dcdc_tear_down\n");                                                                            
	dcdc_tear_down(vi.m_pstr, nr_line);                                                                             
	if (buf)                                                                                                   
    {                                                                                                        
		free(buf);                                                                                               
        buf = NULL;                                                                                          
    }                                                                                                        
    LOGD(TAG "End of %s\n", __FUNCTION__);                                                                   
                                                                                                             
    return SP_ATA_PASS;                                                                                      
}                                                                                                            
                                                                                                             

