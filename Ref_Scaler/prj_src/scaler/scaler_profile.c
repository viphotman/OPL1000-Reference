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
 * @file scaler_profile.c
 * @author SH SDK
 * @date Mar.07, 2019 
 * @brief File creates functions that process profile related parameters.
 *
 */
 
/***********************
Head Block of The File
***********************/

#include <string.h>
#include "cmsis_os.h"
#include "scaler_data.h"
#include "scaler_profile.h"
#include "scaler_sys.h"
#include "scaler_common.h"
#include "ftoa_util.h"
#include "at_cmd_scaler_ext.h"

/********************************************
Declaration of Global Variables & Functions
********************************************/
scaler_hw_attr_t g_ScalerHwAttr;
scaler_rpt_t g_ScalerRptData;
scaler_setting_t g_ScalerSet;
scaler_register_t g_ScalerUser;

T_Ref_Scaler_User_Profile Scaler_User_Profile = {
	"Name","unknown","Gender",SCALER_GENDER_MALE,"Height",0,"Age",30};

/***********
C Functions
***********/
void scaler_data_init(void)
{
	  scaler_user_t* pUser;
	
	  /*
    osSemaphoreDef_t tSemaphoreDef;

    // the state of scaler  data
    g_ubScalerDataState = SCALER_DATA_STATE_IDLE;
  
    // create the semaphore
    tSemaphoreDef.dummy = 0;    // reserved, it is no used
    g_tScalerDataSemaphoreId = osSemaphoreCreate(&tSemaphoreDef, 1);
    if (g_tScalerDataSemaphoreId == NULL)
    {
        printf("To create the semaphore for Scaler Data Semaphore is fail.\n");
    }
    */
	
		// Set default value of g_ScalerHwAttr
    memset(&g_ScalerHwAttr, 0, sizeof(g_ScalerHwAttr));
		strcpy(g_ScalerHwAttr.triple_prdtKey,PRODUCT_KEY);
		strcpy(g_ScalerHwAttr.triple_devName,DEVICE_NAME);
		strcpy(g_ScalerHwAttr.triple_devScrt,DEVICE_SECRET);
		
		// set g_ScalerHwAttr.mac address 
    if (true != scaler_get_mac())
		{
			// set g_ScalerHwAttr.mac address with default value; change later 
			g_ScalerHwAttr.mac[0] = 0x22;
			g_ScalerHwAttr.mac[1] = 0x33;
			g_ScalerHwAttr.mac[2] = 0x44;
			g_ScalerHwAttr.mac[3] = 0x55;
			g_ScalerHwAttr.mac[4] = 0x66;
			g_ScalerHwAttr.mac[0] = 0x76;
		}
		
		// Set default value of g_ScalerRptData
    memset(&g_ScalerRptData, 0, sizeof(g_ScalerRptData));
		g_ScalerRptData.weight = SCALER_WEIGHT_DEF;
		strcpy(g_ScalerRptData.user,SCALER_USER_DEF);
    g_ScalerRptData.battery = SCALER_BATTERY_DEF;
		strcpy(g_ScalerRptData.time,SCALER_TIME_DEF);

		// Set default value of g_ScalerSet
		memset(&g_ScalerSet, 0, sizeof(g_ScalerSet));
		strcpy(g_ScalerSet.user,SCALER_USER_DEF);
		g_ScalerSet.heartbeat = SCALER_HEARTBEAT_INTERVAL_DEF;	

		// Init scaler registered user stucture g_ScalerUser
		memset(&g_ScalerUser, 0, sizeof(g_ScalerUser));
    g_ScalerUser.user_num = 1;	
    pUser = &(g_ScalerUser.reg_user[0]);
		strcpy(pUser->user_name,SCALER_USER_DEF);
		pUser->sex = SCALER_GENDER_MALE;
		pUser->height = 160; // cm 
		pUser->age = 30;		
}

scaler_rpt_t* data_get_entity(void)
{
    return &g_ScalerRptData;
}

scaler_hw_attr_t* attr_get_entity(void)
{
    return &g_ScalerHwAttr;
}

scaler_setting_t* setting_get_entity(void)
{
    return &g_ScalerSet;
}

uint8_t scaler_Data_TimeGet(char* time_stamp_string)
{
    if (true == sys_get_timeStamp(time_stamp_string))
        return SCALER_DATA_OK;
		
    return SCALER_DATA_FAIL;
}

int scaler_get_mac()
{
    uint8_t mac_addr[6];
    int ret;

    if ((ret = sys_get_mac(mac_addr)) == true)
    {
        memcpy(g_ScalerHwAttr.mac, mac_addr, 6);
    }
    return ret;
}

void scaler_battery_conv(int vbat_percent, char* vbat_percent_str, int *vbat_enum)
{
		ftoa(vbat_percent, vbat_percent_str, 1);
		if (0<=vbat_percent && vbat_percent<=10)
			*vbat_enum = 1;
		else if (10<vbat_percent && vbat_percent<=50)
			*vbat_enum = 2;
		else if (50<vbat_percent && vbat_percent<=90)
			*vbat_enum = 3;
		else if(90<vbat_percent && vbat_percent<=100)
			*vbat_enum = 4;
		else
			*vbat_enum = 3;	
}

int scaler_update_user(char* userName, unsigned char gender, unsigned char height, unsigned char age, scaler_register_t* pScalerUser)
{
		scaler_user_t* pOneUser; 
		
		if(pScalerUser->user_num >= SCALER_MAX_USER_NUM)
		{
			EXAMPLE_TRACE("User number execeed maximum value.\n");
			return SCALER_DATA_FAIL;
		}
		
		// pasering user information from MQTT message 
		pOneUser = 	&(pScalerUser->reg_user[pScalerUser->user_num]);
		if (strlen(userName) >= SCLAER_USER_NAME_LEN)
		{
			EXAMPLE_TRACE("User name (%s) execeeds maximum length.\n",userName);
			return SCALER_DATA_FAIL;			
		}
		strcpy(pOneUser->user_name,userName);
		pOneUser->sex = gender;
		pOneUser->height = height;		
		pOneUser->age =	age;	
		pScalerUser->user_num++;
		return SCALER_DATA_OK;
}
