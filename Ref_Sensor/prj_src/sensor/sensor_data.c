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
 * @file sensor_data.c
 * @author SH SDK
 * @date Feb.15, 2019 
 * @brief File creates the major architecture of sensor data model.
 *
 */
 
/***********************
Head Block of The File
***********************/

#include <string.h>
#include "cmsis_os.h"
#include "httpclient.h"
#include "sensor_data.h"
#include "sensor_sys.h"
#include "sensor_common.h"
#include "ftoa_util.h"
/********************************************
Declaration of Global Variables & Functions
********************************************/
sensor_data_t g_SensorData;
Sensor_Data_Temperature_t g_tSensorDataTemperature;

uint8_t g_ubSensorDataState;
osSemaphoreId g_tSensorDataSemaphoreId;

/***********
C Functions
***********/
void data_init(void)
{
    osSemaphoreDef_t tSemaphoreDef;

    // the state of sensor data
    g_ubSensorDataState = SENSOR_DATA_STATE_IDLE;

    // create the semaphore
    tSemaphoreDef.dummy = 0;    // reserved, it is no used
    g_tSensorDataSemaphoreId = osSemaphoreCreate(&tSemaphoreDef, 1);
    if (g_tSensorDataSemaphoreId == NULL)
    {
        printf("To create the semaphore for Sensor Data Semaphore is fail.\n");
    }
    
		// Define default wifi mac address 
    memset(&g_SensorData, 0, sizeof(g_SensorData));
    g_SensorData.mac[0] = 0x11;
    g_SensorData.mac[1] = 0x22;
    g_SensorData.mac[2] = 0x33;
    g_SensorData.mac[3] = 0x44;
    g_SensorData.mac[4] = 0x55;
    g_SensorData.mac[5] = 0x76;

    data_get_mac();

    g_SensorData.battery = SENSOR_BATTERY_DEF;

		// g_tSensorDataTemperature.faTemperature[] and g_tSensorDataTemperature.ulaSysTick[]
		// are ring memory to store temperature and time stamp data
    g_tSensorDataTemperature.ulReadIdx = 0;
    g_tSensorDataTemperature.ulWriteIdx = 0;
}

sensor_data_t* data_get_entity(void)
{
    return &g_SensorData;
}

uint8_t Sensor_Data_TimeGet(uint32_t *pulSysTick)
{
    if (true == sys_get_time(pulSysTick))
        return SENSOR_DATA_OK;
		
    //return SENSOR_DATA_FAIL;
		return SENSOR_DATA_OK;
}

int data_get_mac()
{
    uint8_t mac_addr[6];
    int ret;

    if ((ret = sys_get_mac(mac_addr)) == true)
    {
        memcpy(g_SensorData.mac, mac_addr, 6);
    }
    return ret;
}

uint8_t Sensor_Data_TemperaturePush(float fTemp, uint32_t ulSysTick)
{
    uint32_t ulWriteNext;
    char ftoa_string[16] = {0};
		
    // ulWriteNext always point to next blank unit 
    ulWriteNext = (g_tSensorDataTemperature.ulWriteIdx + 1) % SENSOR_TEMP_COUNT;
	  // ulWriteIdx + 1 == ulReadIdx means ring is full, ulReadIdx need to increase 1 
    if (ulWriteNext == g_tSensorDataTemperature.ulReadIdx)
    {
        // dicard the oldest data
        g_tSensorDataTemperature.ulReadIdx = (g_tSensorDataTemperature.ulReadIdx + 1) % SENSOR_TEMP_COUNT;
    }
    ftoa(fTemp, ftoa_string, 1);

    // update the temperature data in g_tSensorDataTemperature.faTemperature
    g_tSensorDataTemperature.faTemperature[g_tSensorDataTemperature.ulWriteIdx] = fTemp;
		// update time stamp in g_tSensorDataTemperature.ulaSysTick
    g_tSensorDataTemperature.ulaSysTick[g_tSensorDataTemperature.ulWriteIdx] = ulSysTick;
    g_tSensorDataTemperature.ulWriteIdx = ulWriteNext;

    return SENSOR_DATA_OK;
}

uint8_t Sensor_Data_TemperaturePop(float *pfTemp, uint32_t *pulSysTick)
{
    uint8_t bRet = SENSOR_DATA_FAIL;
	  char ftoa_string[16] = {0};

    // empty, ulWriteIdx == ulReadIdx; for normal case ulReadIdx = ulWriteIdx - 1 
    if (g_tSensorDataTemperature.ulWriteIdx == g_tSensorDataTemperature.ulReadIdx)
		{
			  EXAMPLE_TRACE(" Ring buffer is empty , write index = %d ",g_tSensorDataTemperature.ulWriteIdx);
        goto done;
    }
	  
    *pfTemp = g_tSensorDataTemperature.faTemperature[g_tSensorDataTemperature.ulReadIdx];
    *pulSysTick = g_tSensorDataTemperature.ulaSysTick[g_tSensorDataTemperature.ulReadIdx];
	  ftoa(*pfTemp , ftoa_string, 1);
    EXAMPLE_TRACE("Pop g_tSensorDataTemperature temperature = %s ",ftoa_string);

    bRet = SENSOR_DATA_OK;

done:
    return bRet;
}

uint8_t Sensor_Data_TemperatureReadIdxUpdate(void)
{
    g_tSensorDataTemperature.ulReadIdx = (g_tSensorDataTemperature.ulReadIdx + 1) % SENSOR_TEMP_COUNT;

    return SENSOR_DATA_OK;

}

void Sensor_Data_StateSet(uint8_t ubState)
{
    // error handle
    if (ubState >= SENSOR_DATA_STATE_MAX)
        return;

    osSemaphoreWait(g_tSensorDataSemaphoreId, osWaitForever);

    switch (g_ubSensorDataState)
    {
        // Idle -> Idle
        // Idle -> Network on
        case SENSOR_DATA_STATE_IDLE:
            // Idle -> Network on
            if (ubState == SENSOR_DATA_STATE_NETWORK_ON)
                g_ubSensorDataState = SENSOR_DATA_STATE_NETWORK_ON;
            break;

        // Network on -> Idle
        // Network on -> Network on
        // Network on -> Device on
        case SENSOR_DATA_STATE_NETWORK_ON:
            g_ubSensorDataState = ubState;
            break;

        // Device on -> Idle
        // Device on -> Device on
        case SENSOR_DATA_STATE_DEVICE_ON:
            // Device on -> Idle
            if (ubState == SENSOR_DATA_STATE_IDLE)
                g_ubSensorDataState = SENSOR_DATA_STATE_IDLE;
            break;
    }

    osSemaphoreRelease(g_tSensorDataSemaphoreId);
}

uint8_t Sensor_Data_StateGet(void)
{
    return g_ubSensorDataState;
}
