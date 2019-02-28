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
 * @file sensor_data_task.h
 * @author SH SDK
 * @date Feb.15, 2019 
 * @brief File containing declaration of sensor data task.
 *
 */

#ifndef __SENSOR_DATA_TASK_H__
#define __SENSOR_DATA_TASK_H__

#include <stdint.h>
#include <stdbool.h>
#include "cmsis_os.h"

#define SENSOR_DATA_BUF_LEN (128)
#define SENSOR_DATA_QUEUE_SIZE (10)

#define SENSOR_DATA_HTTP_POST_COUNT     5

typedef enum sensor_data_msg_type
{
    SENSOR_DATA_MSG_POST = 0,

    SENSOR_DATA_MSG_NUM
} sensor_data_msg_type_e;

typedef struct
{
    uint32_t event;
	uint32_t length;
	uint8_t *pcMessage;
} xSensorDataMessage_t;

void sensor_data_task_evt_parse(uint32_t evt_type);
void sensor_data_task_evt_handle(uint32_t evt_type);
void sensor_data_task(void *args);
void sensor_data_task_create(void);
osStatus sensor_data_task_send(xSensorDataMessage_t txMsg);

int sensor_data_msg_on_line(void);
void sensor_data_msg_off_line(void);
void sensor_data_msg_upload_data(void);
void sensor_data_msg_upload_battery(void);
void sensor_data_msg_post(void);
void sensor_data_msg_fsm(void);

#endif

