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
 * @file scaler_common.c
 * @author SH SDK
 * @date Mar.05, 2019 
 * @brief File creates the major architecture of scaler  common model.
 *
 */
 
/***********************
Head Block of The File
***********************/
#include "scaler_profile.h"
#include "scaler_data.h"
#include "scaler_data_task.h"
#include "blewifi_ble_api.h" 
#include "scaler_polling.h" 
#include "scaler_notify.h" 
#include "scaler_common.h"
#include "blewifi_ctrl_http_ota.h"

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
        
    //scaler  data polling timer event
    timer_polling_def.ptimer = polling_loop;
    g_tAppPollingTimer = osTimerCreate(&timer_polling_def, osTimerPeriodic, NULL);
    if (g_tAppPollingTimer == NULL)
    {
        printf("To create the timer for AppScalerDataTimer is fail.\n");
    }
}

void start_scaler (void)
{
    /* Initialization */
    scaler_data_init();	  
    comm_data_init();

    /* Sensor Tasks Create */
    //scaler_rpt_task_create();
	
	/* Ignore polling method 
    // Polling Entry 
    create_polling_timer_event();
    osTimerStop(g_tAppPollingTimer);
    osTimerStart(g_tAppPollingTimer, SCALER_POLLING_DELAY_TIME_DEF);
  */
}
