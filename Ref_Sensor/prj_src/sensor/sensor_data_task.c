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
 * @file sensor_data_task.c
 * @author SH SDK
 * @date Feb.15, 2019 
 * @brief File creates the task of sensor data task.
 *
 */

/***********************
Head Block of The File
***********************/
#include <stdlib.h>
#include <string.h>
#include "sys_os_config.h"
#include "sensor_common.h"
#include "sensor_data.h"
#include "sensor_data_task.h"
#include "blewifi_common.h"
#include "ftoa_util.h"
#include "sys_os_config_patch.h"
#include "sensor_auxadc.h"
#include "blewifi_wifi_api.h"
#include "blewifi_ctrl.h"

/***************************************************
Declaration of static Global Variables & Functions
***************************************************/
extern uint8_t g_ubAppCtrlLedStatus;

extern uint8_t BleWifi_Ctrl_OtaStatusGet(void);

osThreadId   SensorDataTaskId;
osMessageQId SensorDataQueueId;
osPoolId     SensorDataMemPoolId;

osPoolDef (SensorDataMemPoolId, SENSOR_DATA_QUEUE_SIZE, xSensorDataMessage_t); // memory pool object

/***********
C Functions
***********/
void sensor_data_task_evt_parse(uint32_t evt_type)
{
    switch (evt_type)
    {
        case SENSOR_DATA_MSG_POST:
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
        
            sensor_data_msg_fsm();
            break;
        }

        default:
            break;
    }
}

void sensor_data_task_evt_handle(uint32_t evt_type)
{
	sensor_data_task_evt_parse(evt_type);
}

void sensor_data_task(void *args)
{
    osEvent rxEvent;
    xSensorDataMessage_t *rxMsg;

    for(;;)
    {
        /* Wait event */
        rxEvent = osMessageGet(SensorDataQueueId, osWaitForever);
        if(rxEvent.status != osEventMessage)
            continue;

        rxMsg = (xSensorDataMessage_t *) rxEvent.value.p;
        sensor_data_task_evt_handle(rxMsg->event);

        /* Release buffer */
        if(rxMsg->pcMessage != NULL)
            free(rxMsg->pcMessage);

        osPoolFree (SensorDataMemPoolId, rxMsg);
    }
}

void sensor_data_task_create(void)
{
    osThreadDef_t task_def;
    osMessageQDef_t queue_def;

    /* Create task */
    task_def.name = "sensor data";
    task_def.stacksize = OS_TASK_STACK_SIZE_SENSOR_DATA;
    task_def.tpriority = OS_TASK_PRIORITY_APP;
    task_def.pthread = sensor_data_task;
    SensorDataTaskId = osThreadCreate(&task_def, (void*)NULL);
    if(SensorDataTaskId == NULL)
    {
        printf("OPL: sensor data task create fail \r\n");
    }
    else
    {
        printf("OPL: sensor data task create successful \r\n");
    }

    /* Create memory pool */
    SensorDataMemPoolId = osPoolCreate (osPool(SensorDataMemPoolId));
    if (!SensorDataMemPoolId)
    {
        msg_print(LOG_HIGH_LEVEL, "OPL: sensor data task mem pool create fail \r\n");
    }

    /* Create message queue */
    queue_def.item_sz = sizeof(xSensorDataMessage_t);
    queue_def.queue_sz = SENSOR_DATA_QUEUE_SIZE;
    SensorDataQueueId = osMessageCreate(&queue_def, NULL);
    if(SensorDataQueueId == NULL)
    {
        printf("OPL: sensor data task create queue fail \r\n");
    }
}

osStatus sensor_data_task_send(xSensorDataMessage_t txMsg)
{
    osStatus ret = osOK;
    xSensorDataMessage_t *pMsg;

    /* Mem pool allocate */
    pMsg = (xSensorDataMessage_t *)osPoolCAlloc(SensorDataMemPoolId);

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
            msg_print(LOG_HIGH_LEVEL, "OPL: sensor data task message allocate fail \r\n");
        }
    }

    if(ret == osOK)
    {
        ret= osMessagePut(SensorDataQueueId, (uint32_t)pMsg, osWaitForever);
        if (ret != osOK)
        {
            msg_print(LOG_HIGH_LEVEL, "sensor_data_task_send queue full\r\n");
        }
    }

    return ret;
}

int sensor_data_msg_on_line(void)
{
    char post_body[256] = {0};
    sensor_data_t *sensor_data = data_get_entity();

    printf("\n\n{\"device\":\""MACSTR"\"}\n", MAC2STR(sensor_data->mac));
    snprintf(post_body, sizeof(post_body), "{\"device\":\""MACSTR"\"}", MAC2STR(sensor_data->mac));

		return 1 ;
}

void sensor_data_msg_off_line(void)
{
    char post_body[256] = {0};
    sensor_data_t *sensor_data = data_get_entity();

    printf("\n\n{\"device\":\""MACSTR"\"}\n", MAC2STR(sensor_data->mac));
    snprintf(post_body, sizeof(post_body), "{\"device\":\""MACSTR"\"}", MAC2STR(sensor_data->mac));
	
}

void sensor_data_msg_upload_data(void)
{
    char ftoa_string[16] = {0};
    char post_body[256] = {0};
    sensor_data_t *sensor_data = data_get_entity();

    float fTemp;
    uint32_t ulSysTick;

    uint32_t i;

    for (i=0; i<SENSOR_DATA_HTTP_POST_COUNT; i++)
    {
        if (SENSOR_DATA_OK == Sensor_Data_TemperaturePop(&fTemp, &ulSysTick))
        {
            printf("\n\n{\"device\":\""MACSTR"\",\"dataJSON\":[{\"temp\":%s, \"time\":%d}]}  \n ", MAC2STR(sensor_data->mac), ftoa(fTemp, ftoa_string, 1), ulSysTick);
            snprintf(post_body, sizeof(post_body), "{\"device\":\""MACSTR"\",\"dataJSON\":[{\"temp\":%s, \"time\":%d}]}",
                     MAC2STR(sensor_data->mac), ftoa(fTemp, ftoa_string, 1), ulSysTick);
            if (true == 1)
                Sensor_Data_TemperatureReadIdxUpdate();
            else
                break;
        }
        else
            break;
    }
}

void sensor_data_msg_upload_battery(void)
{
    char  ftoa_string[16] = {0};
    char  post_body[256]  = {0};
    float fVBatPercentage;

    fVBatPercentage = Sensor_Auxadc_VBat_Convert_to_Percentage();

    sensor_data_t *sensor_data = data_get_entity();

    printf("\n\n{\"device\":\""MACSTR"\",\"battery\":%s} \n",MAC2STR(sensor_data->mac),  ftoa(fVBatPercentage, ftoa_string, 1));
    snprintf(post_body, sizeof(post_body), "{\"device\":\""MACSTR"\",\"battery\":%s}",
            MAC2STR(sensor_data->mac), ftoa(fVBatPercentage, ftoa_string, 1));   
}

void sensor_data_msg_upload_all_data(void)
{
    char  ftoa_string_battery[16] = {0};
    char post_body[256] = {0};
    char ftoa_string[16] = {0};
    float fTemp;
    float fVBatPercentage;
    int8_t rssi;
    int RssiResult;
    uint32_t ulSysTick;
    uint32_t i;
    sensor_data_t *sensor_data = data_get_entity();

    fVBatPercentage = Sensor_Auxadc_VBat_Convert_to_Percentage();
    RssiResult = BleWifi_Wifi_Rssi(&rssi);
    
    for (i = 0; i<SENSOR_DATA_HTTP_POST_COUNT; i++)
    {
        if (SENSOR_DATA_OK == Sensor_Data_TemperaturePop(&fTemp, &ulSysTick))
        {
            if (0 == RssiResult)
            {
                printf("\n\n{\"device\":\""MACSTR"\",\"dataJSON\":[{\"temp\":%s, \"time\":%d ", MAC2STR(sensor_data->mac), ftoa(fTemp, ftoa_string, 1), ulSysTick);
                printf(",\"battery\":%s, \"rssi\":%d}]}  \n\n", ftoa(fVBatPercentage, ftoa_string_battery, 1), rssi);            
                snprintf(post_body, sizeof(post_body), "{\"device\":\""MACSTR"\",\"dataJSON\":[{\"temp\":%s, \"time\":%d, \"battery\":%s, \"rssi\":%d}]}",
                         MAC2STR(sensor_data->mac), ftoa(fTemp, ftoa_string, 1), ulSysTick, ftoa(fVBatPercentage, ftoa_string_battery, 1), rssi);
            }
            else
            {
                printf("\n\n{\"device\":\""MACSTR"\",\"dataJSON\":[{\"temp\":%s, \"time\":%d ", MAC2STR(sensor_data->mac), ftoa(fTemp, ftoa_string, 1), ulSysTick);
                printf(",\"battery\":%s}]}  \n\n", ftoa(fVBatPercentage, ftoa_string_battery, 1));            
                snprintf(post_body, sizeof(post_body), "{\"device\":\""MACSTR"\",\"dataJSON\":[{\"temp\":%s, \"time\":%d, \"battery\":%s}]}",
                         MAC2STR(sensor_data->mac), ftoa(fTemp, ftoa_string, 1), ulSysTick, ftoa(fVBatPercentage, ftoa_string_battery, 1));
            } 
            if (true == 1)
                Sensor_Data_TemperatureReadIdxUpdate();
            else
                break;
        }
        else
            break;
    }
}

void sensor_data_msg_post(void)
{
    uint32_t ulSysTick;
    float fTemperature;

    /* Get */
    if (SENSOR_DATA_OK != Sensor_Data_TimeGet(&ulSysTick))
        return;

    /* Calculate the Temperature */
    Sensor_Auxadc_Tmpr_Get(&fTemperature);
    BleWifi_Wifi_SetDTIM(0);
    BleWifi_Wifi_SetDTIM(BLEWIFI_WIFI_DTIM_INTERVAL);

}


void sensor_data_msg_fsm(void)
{
    uint8_t ubState;
    ubState = Sensor_Data_StateGet();
        
    if (ubState == SENSOR_DATA_STATE_NETWORK_ON)
    {
       BleWifi_Wifi_SetDTIM(0); 
       if(true == sensor_data_msg_on_line())
       {
            Sensor_Data_StateSet(SENSOR_DATA_STATE_DEVICE_ON);
       }
       BleWifi_Wifi_SetDTIM(BLEWIFI_WIFI_DTIM_INTERVAL);
    }
    else if (ubState == SENSOR_DATA_STATE_DEVICE_ON)
    {
        sensor_data_msg_post();
    }
}
