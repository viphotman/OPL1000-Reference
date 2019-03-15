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
 * @file blewifi_ctrl.h
 * @author Vincent Chen, Michael Liao
 * @date 20 Feb 2018
 * @brief File includes the function declaration of blewifi ctrl task.
 *
 */

#ifndef __BLEWIFI_CTRL_H__
#define __BLEWIFI_CTRL_H__

#include <stdint.h>
#include <stdbool.h>
#include "blewifi_configuration.h"

#define BLEWIFI_CTRL_QUEUE_SIZE         (20)

typedef enum blewifi_ctrl_msg_type
{
    /* BLE Trigger */
    BLEWIFI_CTRL_MSG_BLE_INIT_COMPLETE = 0,     //BLE report status
    BLEWIFI_CTRL_MSG_BLE_ADVERTISING_CFM,       //BLE report status
    BLEWIFI_CTRL_MSG_BLE_ADVERTISING_EXIT_CFM,  //BLE report status
    BLEWIFI_CTRL_MSG_BLE_CONNECTION_COMPLETE,   //BLE report status
    BLEWIFI_CTRL_MSG_BLE_CONNECTION_FAIL,       //BLE report status
    BLEWIFI_CTRL_MSG_BLE_DISCONNECT,            //BLE report status
    BLEWIFI_CTRL_MSG_BLE_DATA_IND,              //BLE receive the data from peer to device

    BLEWIFI_CTRL_MSG_BLE_NUM,

    /* Wi-Fi Trigger */
    BLEWIFI_CTRL_MSG_WIFI_INIT_COMPLETE = 0x80, //Wi-Fi report status
    BLEWIFI_CTRL_MSG_WIFI_SCAN_DONE_IND,        //Wi-Fi report status
    BLEWIFI_CTRL_MSG_WIFI_CONNECTION_IND,       //Wi-Fi report status
    BLEWIFI_CTRL_MSG_WIFI_DISCONNECTION_IND,    //Wi-Fi report status
    BLEWIFI_CTRL_MSG_WIFI_GOT_IP_IND,           //Wi-Fi report status
    BLEWIFI_CTRL_MSG_WIFI_AUTO_CONNECT_IND,     //Wi-Fi the auto connection is triggered by timer

    BLEWIFI_CTRL_MSG_WIFI_NUM,

    /* Others Event */
    BLEWIFI_CTRL_MSG_OTHER_OTA_ON = 0x100,      //OTA
    BLEWIFI_CTRL_MSG_OTHER_OTA_OFF,             //OTA success
    BLEWIFI_CTRL_MSG_OTHER_OTA_OFF_FAIL,        //OTA fail
    
    BLEWIFI_CTRL_MSG_OTHER__NUM
} blewifi_ctrl_msg_type_e;

typedef enum blewifi_ctrl_auto_conn_state
{
    BLEWIFI_CTRL_AUTO_CONN_STATE_IDLE = (BLEWIFI_WIFI_REQ_CONNECT_RETRY_TIMES + 1),
    BLEWIFI_CTRL_AUTO_CONN_STATE_SCAN,

    BLEWIFI_CTRL_AUTO_CONN_STATE_NUM
} blewifi_ctrl_auto_conn_state_e;

typedef struct
{
    uint32_t event;
	uint32_t length;
	uint8_t *pcMessage;
} xBleWifiCtrlMessage_t;

void    BleWifi_Ctrl_SysModeSet(uint8_t mode);
uint8_t BleWifi_Ctrl_SysModeGet(void);
void    BleWifi_Ctrl_BleStatusSet(uint8_t status);
uint8_t BleWifi_Ctrl_BleStatusGet(void);
void    BleWifi_Ctrl_WifiStatusSet(uint8_t status);
uint8_t BleWifi_Ctrl_WifiStatusGet(void);
int     BleWifi_Ctrl_MsgSend(int msg_type, uint8_t *data, int data_len);
void    BleWifi_Ctrl_Init(void);

#endif /* __BLEWIFI_CTRL_H__ */

