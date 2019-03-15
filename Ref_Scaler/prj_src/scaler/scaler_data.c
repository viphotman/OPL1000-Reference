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
 * @file scaler_data.c
 * @author SH SDK
 * @date Feb.15, 2019 
 * @brief File creates the major architecture of scaler data model.
 *
 */
 
/***********************
Head Block of The File
***********************/

#include <string.h>
#include "cmsis_os.h"
#include "httpclient.h"
#include "scaler_data.h"
#include "scaler_sys.h"
#include "scaler_common.h"
#include "ftoa_util.h"
#include "at_cmd_scaler_ext.h"

/********************************************
Declaration of Global Variables & Functions
********************************************/
uint8_t g_ubScalerDataState;
osSemaphoreId g_tScalerDataSemaphoreId;

Scaler_Data_t g_tScalerData;
AP_Connect_t g_tScalerApConnect;

Scaler_Data_t *g_pScalerData;
AP_Connect_t *g_pScalerApConn;

unsigned char g_IutIpAddrHex[4];

/***********
C Functions
***********/
void comm_data_init(void)
{
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

    // get ring buffer and ap connection result global structer pointer 
	  memset(&g_tScalerData, 0, sizeof(g_tScalerData));
	  memset(&g_tScalerApConnect, 0, sizeof(g_tScalerApConnect));
	  g_pScalerData = &g_tScalerData;
	  g_pScalerApConn = &g_tScalerApConnect;			
    g_pScalerData->ulReadIdx = 0;
    g_pScalerData->ulWriteIdx = 0;
}

uint8_t scaler_update_ap_conn(int iApConnFlag, unsigned char *pIutIpAddrHex)
{
	  char IutIpAddr[16] = {0};
		
	  if (g_pScalerData == NULL)
		   return SCALER_DATA_FAIL;
		
		// Harris. 03-11. comment it out for debug.
		//if (g_pScalerApConn->iFlag != AP_CONNECTION_READ)  
		//{
			// last time this AP connection status has not been read 
      //return SCALER_DATA_FAIL;			
		//}
	  g_pScalerApConn->iFlag = iApConnFlag;
		// convert hex format to [C0 A8 1 66] -> 192.168.1.102
    util_ip_conv_str(pIutIpAddrHex,IutIpAddr);	
	  strcpy(g_pScalerApConn->cIutIpAddr,IutIpAddr);
	  return SCALER_DATA_OK;
}

uint8_t scaler_RingBufData_Push(int iCloudConnFlag)
{
	  // Because g_pScalerData->ulWriteIdx indicates next unit need to store weight info 
    // hence it is blnak. Previous one stores weight info that need to publish to cloud   
    uint32_t ulWritePrevious;
		
	  if (g_pScalerData == NULL)
		   return SCALER_DATA_FAIL;
		
    // ulWriteNext always point to next blank unit 
	  if(g_pScalerData->ulWriteIdx > 0)
		    ulWritePrevious = g_pScalerData->ulWriteIdx-1;
		else
			  ulWritePrevious = SCALER_WEIGHT_COUNT-1; 

    // update the weight data in g_tScalerData.iWeight
    g_tScalerData.iFlag[ulWritePrevious] = iCloudConnFlag;

    return SCALER_DATA_OK;
}

uint8_t scaler_RingBufData_Pop(int *piWeight)
{
	  char ftoa_string[16] = {0};

    // empty, ulWriteIdx == ulReadIdx; for normal case ulReadIdx = ulWriteIdx - 1 
    if (g_tScalerData.ulWriteIdx == g_tScalerData.ulReadIdx)
		{
			  EXAMPLE_TRACE(" Ring buffer is empty , write index = %d ",g_tScalerData.ulWriteIdx);
        return SCALER_DATA_FAIL;
    }
	  
    *piWeight = g_tScalerData.iWeight[g_tScalerData.ulReadIdx];

   	ftoa(*piWeight , ftoa_string, 1);
    EXAMPLE_TRACE("Pop g_tScalerData weight = %s g ",ftoa_string);
    scaler_Data_WeightReadIdxUpdate();
		
    return SCALER_DATA_OK;
}

uint8_t scaler_Data_WeightReadIdxUpdate(void)
{
    g_tScalerData.ulReadIdx = (g_tScalerData.ulReadIdx + 1) % SCALER_WEIGHT_COUNT;

    return SCALER_DATA_OK;

}

void scaler_Data_StateSet(uint8_t ubState)
{
    // error handle
    if (ubState >= SCALER_DATA_STATE_MAX)
        return;

    osSemaphoreWait(g_tScalerDataSemaphoreId, osWaitForever);

    switch (g_ubScalerDataState)
    {
        // Idle -> Idle
        // Idle -> Network on
        case SCALER_DATA_STATE_IDLE:
            // Idle -> Network on
            if (ubState == SCALER_DATA_STATE_NETWORK_ON)
                g_ubScalerDataState = SCALER_DATA_STATE_NETWORK_ON;
            break;

        // Network on -> Idle
        // Network on -> Network on
        // Network on -> Device on
        case SCALER_DATA_STATE_NETWORK_ON:
            g_ubScalerDataState = ubState;
            break;

        // Device on -> Idle
        // Device on -> Device on
        case SCALER_DATA_STATE_DEVICE_ON:
            // Device on -> Idle
            if (ubState == SCALER_DATA_STATE_IDLE)
                g_ubScalerDataState = SCALER_DATA_STATE_IDLE;
            break;
    }

    osSemaphoreRelease(g_tScalerDataSemaphoreId);
}

uint8_t scaler_Data_StateGet(void)
{
    return g_ubScalerDataState;
}
