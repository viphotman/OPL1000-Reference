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
 * @file sensor_sys.c
 * @author SH-SDK
 * @date Feb.25,2019 
 * @brief File creates the sensor system wrapper for low level driver & os mapping.
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
#include "sensor_common.h"
#include "sensor_data.h"

/***********
C Functions
***********/
void sys_osdelay(unsigned int ms)
{
    osDelay(ms);
}

int sys_get_time(uint32_t *tick)
{
    uint32_t dwTick = 0;
    int32_t dwOverflow = 0;
    /* Use system driver api to get timestamp */

    osKernelSysTickEx(&dwTick, &dwOverflow);
    *tick = dwTick;

    /* Update timestamp to g_SensorData */

    return true;
}

int sys_get_mac(uint8_t *mac)
{
    /* Use system driver api to get mac */
    wifi_config_get_mac_address(WIFI_MODE_STA, mac);

    /* Update mac to g_SensorData */
    return true;
}
