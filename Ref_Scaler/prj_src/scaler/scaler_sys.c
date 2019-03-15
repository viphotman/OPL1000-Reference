/******************************************************************************
*  Copyright 2017 - 2018, Opulinks Technology Ltd.
*  ----------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Opulinks Technology Ltd. (C) 2018
******************************************************************************/
/**
 * @file scaler_sys.c
 * @author SH-SDK
 * @date Mar.05,2019 
 * @brief File creates the scaler reference wrapper for low level driver & os mapping.
 *
 */
 
/***********************
Head Block of The File
***********************/ 
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "opl1000.h"
#include "cmsis_os.h"
#include "wifi_api.h"
#include "scaler_common.h"
#include "scaler_data.h"
#include "scaler_sys.h"
#include "time.h"
#include "ftoa_util.h"

/********************************************
Declaration of Global Variables & Functions
********************************************/
extern char c_tm[64];  // defined in sntp_time.c 
extern unsigned int g_sntp_tm;  // defined in mqtt_example.c

/***********
C Functions
***********/
void sys_osdelay(unsigned int ms)
{
    osDelay(ms);
}

unsigned int sys_get_tick(void)
{
    unsigned int tickCount = 0;
    tickCount = xTaskGetTickCount();
    return tickCount;
}

int sys_get_mac(unsigned char *mac)
{
    /* Use system driver api to get mac */
    wifi_config_get_mac_address(WIFI_MODE_STA, mac);

    return true;
}

int sys_get_timeStamp(char* cTimeStampBuffer)	
{	
	unsigned int c_time;
	
  // Obtain sntp time stamp from SNTP server 	
	if(g_sntp_tm == 0)
	{
		EXAMPLE_TRACE(" Get SNTP time error!");
		return false;
	}		
	
	c_time = g_sntp_tm + xTaskGetTickCount()/1000;
	struct tm *t = localtime(&c_time);
	//strftime(c_tm, sizeof(c_tm), "%Y/%m/%d %H:%M:%S", t);
	strftime(c_tm, sizeof(c_tm), "%Y/%m/%d %H:%M", t);
	memset(cTimeStampBuffer, 0, sizeof(c_tm));
	sprintf(cTimeStampBuffer,"%s", c_tm);
	return true;
}
