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
 * @file blewifi_app.c
 * @author Vincent Chen
 * @date 12 Feb 2018
 * @brief File creates the wifible app task architecture.
 *
 */
#include "blewifi_configuration.h"
#include "blewifi_app.h"
#include "blewifi_wifi_api.h"
#include "blewifi_ble_api.h"
#include "blewifi_ctrl.h"
#include "sys_common_api.h"
#include "ps_public.h"
#include "cmsis_os.h"
#include "sys_os_config.h"
#include "..\mqtt_demo.h"
#include "lwip_helper.h"
#include "hal_pwm.h"


blewifi_ota_t *gTheOta = 0;
osThreadId APP_TASK_ID;

extern int mqtt_task(void);

void APP_entry(void *args)
{
    /* Tcpip stack and net interface initialization,  dhcp client process initialization. */
    //lwip_network_init(WIFI_MODE_STA);

    /* Waiting for connection & got IP from DHCP server */
    lwip_net_ready();

    mqtt_task();

    while (1) {
        osDelay(2000);
    }
}

void BleWifiAppInit(void)
{
    gTheOta = 0;
    osThreadDef_t task_def;
        /* Wi-Fi Initialization */
    BleWifi_Wifi_Init();

        /* BLE Stack Initialization */
    BleWifi_Ble_Init();

        /* blewifi "control" task Initialization */
    BleWifi_Ctrl_Init();

        /* Power saving settings */
    ps_smart_sleep(BLEWIFI_COM_POWER_SAVE_EN);

    Hal_Pwm_Init();

    Hal_Pwm_ClockSourceSet(HAL_PWM_CLK_22M);

    //Hal_Pwm_ClockSourceSet(HAL_PWM_CLK_32K);

    /* Create task */
    task_def.name = "user_app";
    task_def.stacksize = 512*4;
    task_def.tpriority = OS_TASK_PRIORITY_APP;
    task_def.pthread = APP_entry;
    APP_TASK_ID = osThreadCreate(&task_def, (void*)NULL);
    if(APP_TASK_ID == NULL)
    {
        printf("user_app Task create fail \r\n");
    }
    else
    {
        printf("user_app Task create successful \r\n");
    }    
}
