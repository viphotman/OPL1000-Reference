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
#include "mw_fim_default_group03.h"
#include "mw_fim_default_group03_patch.h"

#include "cmsis_os.h"
#include "sys_os_config.h"
#include "..\mqtt_demo.h"
#include "lwip_helper.h"
#include "sensor_data.h"
#include "sensor_common.h"

blewifi_ota_t *gTheOta = 0;
osThreadId APP_TASK_ID;

extern int mqtt_task(void);

void APP_entry(void *args)
{
    /* Tcpip stack and net interface initialization,  dhcp client process initialization. */
    //lwip_network_init(WIFI_MODE_STA);

    /* Waiting for connection & got IP from DHCP server */
    lwip_net_ready();

	  /*at this point, AP connection shall be OK*/
	  Sensor_Data_StateSet(SENSOR_DATA_STATE_NETWORK_ON);
	
    mqtt_task();

    while (1) {
        osDelay(2000);
    }
}

void BleWifiAppInit(void)
{		
		osThreadDef_t task_def;
    T_MwFim_SysMode tSysMode;
    
	gTheOta = 0;
	
    // get the settings of system mode
	if (MW_FIM_OK != MwFim_FileRead(MW_FIM_IDX_GP03_PATCH_SYS_MODE, 0, MW_FIM_SYS_MODE_SIZE, (uint8_t*)&tSysMode))
    {
        // if fail, get the default value
        memcpy(&tSysMode, &g_tMwFimDefaultSysMode, MW_FIM_SYS_MODE_SIZE);
    }

    // only for the user mode
    if ((tSysMode.ubSysMode == MW_FIM_SYS_MODE_INIT) || (tSysMode.ubSysMode == MW_FIM_SYS_MODE_USER))
    {
        /* Wi-Fi Initialization */
        BleWifi_Wifi_Init();

        /* BLE Stack Initialization */
        BleWifi_Ble_Init();

        /* blewifi "control" task Initialization */
        BleWifi_Ctrl_Init();
			
				/* Sensor Initialization */
				start_sensor();
			
        /* Power saving settings */
        if (tSysMode.ubSysMode == MW_FIM_SYS_MODE_USER)
            ps_smart_sleep(BLEWIFI_COM_POWER_SAVE_EN);
    }

    // update the system mode
    BleWifi_Ctrl_SysModeSet(tSysMode.ubSysMode);
		
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

