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
 * @file scaler_data_task.c
 * @author SH SDK
 * @date Feb.15, 2019 
 * @brief File creates the task of scaler  data task.
 *
 */

/***********************
Head Block of The File
***********************/
#include <stdlib.h>
#include <string.h>
#include "sys_os_config.h"
#include "scaler_common.h"
#include "scaler_data.h"
#include "scaler_data_task.h"
#include "blewifi_common.h"
#include "ftoa_util.h"
#include "sys_os_config_patch.h"
#include "blewifi_wifi_api.h"
#include "blewifi_ctrl.h"

/***************************************************
Declaration of static Global Variables & Functions
***************************************************/
extern uint8_t g_ubAppCtrlLedStatus;

extern uint8_t BleWifi_Ctrl_OtaStatusGet(void);

osThreadId   ScalerDataTaskId;
osMessageQId ScalerDataQueueId;
osPoolId     ScalerDataMemPoolId;

osPoolDef (ScalerDataMemPoolId, SCALER_DATA_QUEUE_SIZE, xScalerDataMessage_t); // memory pool object

/***********
C Functions
***********/
void scaler_data_task_evt_parse(uint32_t evt_type)
{
    switch (evt_type)
    {
        case SCALER_DATA_MSG_POST:
        {
            // OTA
            if (true == BleWifi_Ctrl_OtaStatusGet())
            {
                // no action
            }
            // Wifi
            else if (true == BleWifi_Ctrl_WifiStatusGet())
            {
                // no action
            }
        
            scaler_data_msg_fsm();
            break;
        }

        default:
            break;
    }
}

void scaler_data_task_evt_handle(uint32_t evt_type)
{
	scaler_data_task_evt_parse(evt_type);
}

void scaler_data_task(void *args)
{
    osEvent rxEvent;
    xScalerDataMessage_t *rxMsg;

    for(;;)
    {
        /* Wait event */
        rxEvent = osMessageGet(ScalerDataQueueId, osWaitForever);
        if(rxEvent.status != osEventMessage)
            continue;

        rxMsg = (xScalerDataMessage_t *) rxEvent.value.p;
        scaler_data_task_evt_handle(rxMsg->event);

        /* Release buffer */
        if(rxMsg->pcMessage != NULL)
            free(rxMsg->pcMessage);

        osPoolFree (ScalerDataMemPoolId, rxMsg);
    }
}

void scaler_data_task_create(void)
{
    osThreadDef_t task_def;
    osMessageQDef_t queue_def;

    /* Create task */
    task_def.name = "scaler  data";
    task_def.stacksize = OS_TASK_STACK_SIZE_SCALER_DATA;
    task_def.tpriority = OS_TASK_PRIORITY_APP;
    task_def.pthread = scaler_data_task;
    ScalerDataTaskId = osThreadCreate(&task_def, (void*)NULL);
    if(ScalerDataTaskId == NULL)
    {
        printf("OPL: scaler  data task create fail \r\n");
    }
    else
    {
        printf("OPL: scaler  data task create successful \r\n");
    }

    /* Create memory pool */
    ScalerDataMemPoolId = osPoolCreate (osPool(ScalerDataMemPoolId));
    if (!ScalerDataMemPoolId)
    {
        msg_print(LOG_HIGH_LEVEL, "OPL: scaler  data task mem pool create fail \r\n");
    }

    /* Create message queue */
    queue_def.item_sz = sizeof(xScalerDataMessage_t);
    queue_def.queue_sz = SCALER_DATA_QUEUE_SIZE;
    ScalerDataQueueId = osMessageCreate(&queue_def, NULL);
    if(ScalerDataQueueId == NULL)
    {
        printf("OPL: scaler  data task create queue fail \r\n");
    }
}

osStatus scaler_data_task_send(xScalerDataMessage_t txMsg)
{
    osStatus ret = osOK;
    xScalerDataMessage_t *pMsg;

    /* Mem pool allocate */
    pMsg = (xScalerDataMessage_t *)osPoolCAlloc(ScalerDataMemPoolId);

    if (pMsg == NULL)
    {
        msg_print(LOG_HIGH_LEVEL, "message allocate fail \r\n");
        return ret;
    }

    pMsg->event = txMsg.event;
    pMsg->length = txMsg.length;

    if(txMsg.length != 0)
    {
        /* Malloc buffer */
        pMsg->pcMessage = (void *)malloc(txMsg.length);

        if(txMsg.pcMessage != NULL)
        {
            memcpy(pMsg->pcMessage, txMsg.pcMessage, txMsg.length);
        }
        else
        {
            ret = osErrorOS;
            msg_print(LOG_HIGH_LEVEL, "OPL: scaler  data task message allocate fail \r\n");
        }
    }

    if(ret == osOK)
    {
        ret= osMessagePut(ScalerDataQueueId, (uint32_t)pMsg, osWaitForever);
        if (ret != osOK)
        {
            msg_print(LOG_HIGH_LEVEL, "scaler_data_task_send queue full\r\n");
        }
    }

    return ret;
}

int scaler_data_msg_on_line(void)
{
    char post_body[256] = {0};
    scaler_hw_attr_t *scaler_attr = attr_get_entity();

    printf("\n\n{\"device\":\""MACSTR"\"}\n", MAC2STR(scaler_attr->mac));
    snprintf(post_body, sizeof(post_body), "{\"device\":\""MACSTR"\"}", MAC2STR(scaler_attr->mac));

		return 1 ;
}

void scaler_data_msg_off_line(void)
{
    char post_body[256] = {0};
    scaler_hw_attr_t *scaler_attr = attr_get_entity();

    printf("\n\n{\"device\":\""MACSTR"\"}\n", MAC2STR(scaler_attr->mac));
    snprintf(post_body, sizeof(post_body), "{\"device\":\""MACSTR"\"}", MAC2STR(scaler_attr->mac));
	
}
/*
void scaler_data_msg_upload_data(void)
{
    char ftoa_string[16] = {0};
    char post_body[256] = {0};
    scaler_rpt_t *scaler_data = data_get_entity();

    float fWeight;
    uint32_t ulSysTick;

    uint32_t i;

    for (i=0; i<SCALER_DATA_HTTP_POST_COUNT; i++)
    {
        if (SCALER_DATA_OK == scaler_RingBufData_Pop(&fWeight, &ulSysTick))
        {
            printf("\n\n{\"device\":\""MACSTR"\",\"dataJSON\":[{\"temp\":%s, \"time\":%d}]}  \n ", MAC2STR(scaler_data->mac), ftoa(fWeight, ftoa_string, 1), ulSysTick);
            snprintf(post_body, sizeof(post_body), "{\"device\":\""MACSTR"\",\"dataJSON\":[{\"temp\":%s, \"time\":%d}]}",
                     MAC2STR(scaler_data->mac), ftoa(fWeight, ftoa_string, 1), ulSysTick);
            if (true == 1)
                scaler_Data_WeightReadIdxUpdate();
            else
                break;
        }
        else
            break;
    }
}

void scaler_data_msg_upload_battery(void)
{
    char  ftoa_string[16] = {0};
    char  post_body[256]  = {0};
    float fVBatPercentage = 0;

    // here we need porting battery reading function into scaler_data.c 
    //fVBatPercentage = scaler_Auxadc_VBat_Convert_to_Percentage();

    scaler_rpt_t *scaler_data = data_get_entity();

    printf("\n\n{\"device\":\""MACSTR"\",\"battery\":%s} \n",MAC2STR(scaler_data->mac),  ftoa(fVBatPercentage, ftoa_string, 1));
    snprintf(post_body, sizeof(post_body), "{\"device\":\""MACSTR"\",\"battery\":%s}",
            MAC2STR(scaler_data->mac), ftoa(fVBatPercentage, ftoa_string, 1));   
}

void scaler_data_msg_upload_all_data(void)
{
    char  ftoa_string_battery[16] = {0};
    char post_body[256] = {0};
    char ftoa_string[16] = {0};
    float fWeight;
    float fVBatPercentage = 0;
    int8_t rssi = 0;
    int RssiResult;
    uint32_t ulSysTick;
    uint32_t i;
    scaler_rpt_t *scaler_data = data_get_entity();

    // here we need porting battery reading function into scaler_data.c 
    // fVBatPercentage = Scaler_Auxadc_VBat_Convert_to_Percentage();
    // RssiResult = BleWifi_Wifi_Rssi(&rssi);
    
    for (i = 0; i<SCALER_DATA_HTTP_POST_COUNT; i++)
    {
        if (SCALER_DATA_OK == scaler_RingBufData_Pop(&fWeight, &ulSysTick))
        {
            if (0 == RssiResult)
            {
                printf("\n\n{\"device\":\""MACSTR"\",\"dataJSON\":[{\"temp\":%s, \"time\":%d ", MAC2STR(scaler_data->mac), ftoa(fWeight, ftoa_string, 1), ulSysTick);
                printf(",\"battery\":%s, \"rssi\":%d}]}  \n\n", ftoa(fVBatPercentage, ftoa_string_battery, 1), rssi);            
                snprintf(post_body, sizeof(post_body), "{\"device\":\""MACSTR"\",\"dataJSON\":[{\"temp\":%s, \"time\":%d, \"battery\":%s, \"rssi\":%d}]}",
                         MAC2STR(scaler_data->mac), ftoa(fWeight, ftoa_string, 1), ulSysTick, ftoa(fVBatPercentage, ftoa_string_battery, 1), rssi);
            }
            else
            {
                printf("\n\n{\"device\":\""MACSTR"\",\"dataJSON\":[{\"temp\":%s, \"time\":%d ", MAC2STR(scaler_data->mac), ftoa(fWeight, ftoa_string, 1), ulSysTick);
                printf(",\"battery\":%s}]}  \n\n", ftoa(fVBatPercentage, ftoa_string_battery, 1));            
                snprintf(post_body, sizeof(post_body), "{\"device\":\""MACSTR"\",\"dataJSON\":[{\"temp\":%s, \"time\":%d, \"battery\":%s}]}",
                         MAC2STR(scaler_data->mac), ftoa(fWeight, ftoa_string, 1), ulSysTick, ftoa(fVBatPercentage, ftoa_string_battery, 1));
            } 
            if (true == 1)
                scaler_Data_WeightReadIdxUpdate();
            else
                break;
        }
        else
            break;
    }
}
*/
void scaler_data_msg_post(void)
{    
    BleWifi_Wifi_SetDTIM(0);
    BleWifi_Wifi_SetDTIM(BLEWIFI_WIFI_DTIM_INTERVAL);
}


void scaler_data_msg_fsm(void)
{
    uint8_t ubState;
    ubState = scaler_Data_StateGet();
        
    if (ubState == SCALER_DATA_STATE_NETWORK_ON)
    {
       BleWifi_Wifi_SetDTIM(0); 
       if(true == scaler_data_msg_on_line())
       {
            scaler_Data_StateSet(SCALER_DATA_STATE_DEVICE_ON);
       }
       BleWifi_Wifi_SetDTIM(BLEWIFI_WIFI_DTIM_INTERVAL);
    }
    else if (ubState == SCALER_DATA_STATE_DEVICE_ON)
    {
        scaler_data_msg_post();
    }
}
