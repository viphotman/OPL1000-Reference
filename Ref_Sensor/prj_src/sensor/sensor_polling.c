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
 * @file sensor_polling.c
 * @author SH-SDK
 * @date Feb.25,2019 
 * @brief File creates the major sensor polling behavior.
 *
 */

/***********************
Head Block of The File
***********************/
#include "sensor_sys.h"
#include "sensor_notify.h"
#include "sensor_common.h"
#include "sensor_data_task.h"

/***********
C Functions
***********/
void polling_loop(void const *argu)
{
    notify_send_msg_to_sd_task(SENSOR_DATA_MSG_POST, NULL);
	  EXAMPLE_TRACE("notify_send_msg_to_sd_task=%d \n",SENSOR_DATA_MSG_POST);
}
