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
 * @file scaler_data_task.h
 * @author SH SDK
 * @date Mar.05, 2019 
 * @brief File containing declaration of scaler  data task.
 *
 */

#ifndef __SCALER_DATA_TASK_H__
#define __SCALER_DATA_TASK_H__

#include <stdint.h>
#include <stdbool.h>
#include "cmsis_os.h"

#define SCALER_DATA_BUF_LEN (128)
#define SCALER_DATA_QUEUE_SIZE (10)

#define SCALER_DATA_HTTP_POST_COUNT      5
#define OS_TASK_STACK_SIZE_SCALER_DATA   512 

typedef enum scaler_data_msg_type
{
    SCALER_DATA_MSG_POST = 0,

    SCALER_DATA_MSG_NUM
} scaler_data_msg_type_e;

typedef struct
{
    uint32_t event;
	uint32_t length;
	uint8_t *pcMessage;
} xScalerDataMessage_t;

void scaler_data_task_evt_parse(uint32_t evt_type);
void scaler_data_task_evt_handle(uint32_t evt_type);
void scaler_data_task(void *args);
void scaler_data_task_create(void);
osStatus scaler_data_task_send(xScalerDataMessage_t txMsg);

int scaler_data_msg_on_line(void);
void scaler_data_msg_off_line(void);
void scaler_data_msg_upload_data(void);
void scaler_data_msg_upload_battery(void);
void scaler_data_msg_post(void);
void scaler_data_msg_fsm(void);

#endif

