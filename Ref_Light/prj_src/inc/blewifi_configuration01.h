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

#ifndef __BLEWIFI_CONFIGURATION_H__
#define __BLEWIFI_CONFIGURATION_H__

// Common part
/*
Smart sleep
*/
#define BLEWIFI_COM_POWER_SAVE_EN       (0)     // 1: enable    0: disable


// BLE part
/*
BLE Service UUID
*/
#define BLEWIFI_BLE_UUID_SERVICE        0xAAAA
#define BLEWIFI_BLE_UUID_DATA_IN        0xBBB0
#define BLEWIFI_BLE_UUID_DATA_OUT       0xBBB1

/* Device Name
method 1: use prefix + mac address
    The max length of prefix is 18 bytes.
    The length of mac address is 11 bytes.
    
    Ex: OPL_33:44:55:66
    
method 2: full name
    The max length of device name is 29 bytes.
*/
#define BLEWIFI_BLE_DEVICE_NAME_METHOD  1           // 1 or 2
#define BLEWIFI_BLE_DEVICE_NAME_PREFIX  "OPL_"      // for method 1
#define BLEWIFI_BLE_DEVICE_NAME_FULL    "OPL1000"   // for method 2

/* Advertisement Interval Calculation Method:
1000 (ms) / 0.625 (ms) = 1600 = 0x640
*/
#define BLEWIFI_BLE_ADVERTISEMENT_INTERVAL_MIN  0x640  // For 1 Second
#define BLEWIFI_BLE_ADVERTISEMENT_INTERVAL_MAX  0x640  // For 1 Second


// Wifi part
/* Connection Retry times:
When BLE send connect reqest for connecting AP.
If failure will retry times which define below.
*/
#define BLEWIFI_WIFI_REQ_CONNECT_RETRY_TIMES    5


/* Auto Connection Interval:
if the auto connection is fail, the interval will be increased
    Ex: Init 5000, Diff 1000, Max 30000
        1st:  5000 ms
        2nd:  6000 ms
            ...
        26th: 30000 ms
        27th: 30000 ms
            ...
*/
#define BLEWIFI_WIFI_AUTO_CONNECT_INTERVAL_INIT     (5000)      // ms
#define BLEWIFI_WIFI_AUTO_CONNECT_INTERVAL_DIFF     (1000)      // ms
#define BLEWIFI_WIFI_AUTO_CONNECT_INTERVAL_MAX      (30000)     // ms

/* DTIM the times of Interval: ms
*/
#define BLEWIFI_WIFI_DTIM_INTERVAL                  (2000)      // ms

// Sensor part
/* 
Define polling time at once
*/
#define SENSOR_POLLING_DELAY_TIME_DEF               (6000)      // ms

#endif /* __BLEWIFI_CONFIGURATION_H__ */

/*
Define the moving average count for temperature and VBAT
*/
#define SENSOR_MOVING_AVERAGE_COUNT         (5)

/*
Define the critical range of temperature for the moving average of temperature
*/
#define SENSOR_TMPR_CRITICAL_HIGH           (42.0)
//#define SENSOR_TMPR_CRITICAL_LOW            (35.0)
#define SENSOR_TMPR_CRITICAL_LOW            (1.0)
#define SENSOR_TMPR_CRITICAL_BOUNDARY_DIFF  (2.0)

/* 
Define Maximum Voltage & Minimum Voltage
*/
#define MAXIMUM_VOLTAGE_DEF     (3.0f)
#define MINIMUM_VOLTAGE_DEF     (2.0f)

/* LED IO Port
*/
#define LED_IO_PORT             (GPIO_IDX_23)

/* LED time : it is related on blewifi_ctrl.h
    State
    1. No connection
    2. BLE is connected
    3. Wifi is connected
    4. OTA is executed

    unit: ms
*/
#define LED_TIME_NONE_ON_1      (100)
#define LED_TIME_NONE_OFF_1     (200)
#define LED_TIME_NONE_ON_2      (100)
#define LED_TIME_NONE_OFF_2     (5600)
#define LED_TIME_BLE_OFF        (0x7FFFFFFF)        // for a long time (like as always)
#define LED_TIME_WIFI_ON_1      (100)
#define LED_TIME_WIFI_OFF_1     (0x7FFFFFFF)
#define LED_TIME_WIFI_ON_2      (100)               // dummy
#define LED_TIME_WIFI_OFF_2     (5900)              // dummy
#define LED_TIME_OTA_ON         (100)
#define LED_TIME_OTA_OFF        (1900)

/*
#define HTTP_POST_TIMEOUT_DEF               (5000) 
*/

