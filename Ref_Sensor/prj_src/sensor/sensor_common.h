/******************************************************************************
*  Copyright 2018, Netlink Communication Corp.
*  ---------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Netlnik Communication Corp. (C) 2018
******************************************************************************/
/**
 * @file sensor_common.h
 * @author Michael Liao
 * @date 15 Mar 2018
 * @brief File containing declaration of sensor common & definition of structure for reference.
 *
 */
#ifndef __SENSOR_COMMON_H__
#define __SENSOR_COMMON_H__
#include "msg.h"
#include "blewifi_configuration.h"
#include "iot_import.h"

#define EXAMPLE_TRACE(fmt, ...)  \
    do { \
        HAL_Printf("%s|%03d :: ", __func__, __LINE__); \
        HAL_Printf(fmt, ##__VA_ARGS__); \
        HAL_Printf("%s", "\r\n"); \
    } while(0)

void start_sensor(void);

#endif

