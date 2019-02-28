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
 * @file sensor_notify.c
 * @author SH SDK
 * @date Feb.15, 2019 
 * @brief File creates the sensor notify interface.
 *
 */
/***********************
Head Block of The File
***********************/ 
#include "sensor_data_task.h"

/***********
C Functions
***********/
//Send message to "sensor data task"
void notify_send_msg_to_sd_task(uint32_t action, unsigned char * pMsg)
{
    xSensorDataMessage_t txMsg = {0};
    txMsg.event = action;
    txMsg.length = 0;
    txMsg.pcMessage = pMsg;
    sensor_data_task_send(txMsg);
}
