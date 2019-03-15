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
 * @file scaler_polling.c
 * @author SH-SDK
 * @date Feb.25,2019 
 * @brief File creates the major scaler polling behavior.
 *
 */

/***********************
Head Block of The File
***********************/
#include "scaler_sys.h"
#include "scaler_notify.h"
#include "scaler_common.h"
#include "scaler_data_task.h"
#include "ftoa_util.h"

/***********
C Functions
***********/
void polling_loop(void const *argu)
{
    notify_send_msg_to_sd_task(SCALER_DATA_MSG_POST, NULL);
	  EXAMPLE_TRACE("notify_send_msg_to_sd_task=%d \n",SCALER_DATA_MSG_POST);
}
