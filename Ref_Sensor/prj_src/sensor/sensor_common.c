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
 * @file sensor_common.c
 * @author SH SDK
 * @date Feb.15, 2019 
 * @brief File creates the major architecture of sensor common model.
 *
 */
 
/***********************
Head Block of The File
***********************/

#include "sensor_data.h"
#include "sensor_data_task.h"
#include "blewifi_ble_api.h" 
#include "sensor_polling.h" 
#include "sensor_notify.h" 
#include "sensor_common.h"
#include "blewifi_ctrl_http_ota.h"
#include "sensor_auxadc.h"

/********************************************
Declaration of Global Variables & Functions
********************************************/
osTimerId g_tAppPollingTimer = NULL;

/***********
C Functions
***********/

void create_polling_timer_event(void)
{
    osTimerDef_t timer_polling_def;
        
    //sensor data polling timer event
    timer_polling_def.ptimer = polling_loop;
    g_tAppPollingTimer = osTimerCreate(&timer_polling_def, osTimerPeriodic, NULL);
    if (g_tAppPollingTimer == NULL)
    {
        printf("To create the timer for AppSensorDataTimer is fail.\n");
    }
}

void start_sensor(void)
{
    /* Initialization */
    data_init();
    Sensor_Auxadc_Init();

    /* Sensor Tasks Create */
    sensor_data_task_create();
	
	  /* Mask http ota operation 
    blewifi_ctrl_http_ota_task_create();
    */
	
		/* Ignore polling method 
    // Polling Entry 
    create_polling_timer_event();
    osTimerStop(g_tAppPollingTimer);
    osTimerStart(g_tAppPollingTimer, SENSOR_POLLING_DELAY_TIME_DEF);
	  */
}
