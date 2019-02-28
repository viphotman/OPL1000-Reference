/*
 * Copyright (c) 2014-2016 Alibaba Group. All rights reserved.
 * License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

/***********************
Head Block of The File
***********************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "cmsis_os.h"
#include "iot_import.h"
#include "iot_export.h"
#include "hal_vic.h"
#include "hal_pwm.h"
#include "ftoa_util.h"
#include "sys_os_config.h"
#include "sensor_common.h"
#include "sensor_data.h"
#include "sensor_data_task.h"
#include "sensor_auxadc.h"
#include "mqtt_example.h"
    
/********************************************
Declaration of Global Variables & Functions
********************************************/
char __product_key[PRODUCT_KEY_LEN + 1];
char __device_name[DEVICE_NAME_LEN + 1];
char __device_secret[DEVICE_SECRET_LEN + 1];

uint32_t SendCount = 0;

/***********
C Functions
***********/
void event_handle(void *pcontext, void *pclient, iotx_mqtt_event_msg_pt msg)
{
    uintptr_t packet_id = (uintptr_t)msg->msg;
    iotx_mqtt_topic_info_pt topic_info = (iotx_mqtt_topic_info_pt)msg->msg;

    switch (msg->event_type) {
        case IOTX_MQTT_EVENT_UNDEF:
            EXAMPLE_TRACE("undefined event occur.");
            break;

        case IOTX_MQTT_EVENT_DISCONNECT:
            EXAMPLE_TRACE("MQTT disconnect.");
            break;

        case IOTX_MQTT_EVENT_RECONNECT:
            EXAMPLE_TRACE("MQTT reconnect.");
            break;

        case IOTX_MQTT_EVENT_SUBCRIBE_SUCCESS:
            EXAMPLE_TRACE("subscribe success, packet-id=%u", (unsigned int)packet_id);
            break;

        case IOTX_MQTT_EVENT_SUBCRIBE_TIMEOUT:
            EXAMPLE_TRACE("subscribe wait ack timeout, packet-id=%u", (unsigned int)packet_id);
            break;

        case IOTX_MQTT_EVENT_SUBCRIBE_NACK:
            EXAMPLE_TRACE("subscribe nack, packet-id=%u", (unsigned int)packet_id);
            break;

        case IOTX_MQTT_EVENT_UNSUBCRIBE_SUCCESS:
            EXAMPLE_TRACE("unsubscribe success, packet-id=%u", (unsigned int)packet_id);
            break;

        case IOTX_MQTT_EVENT_UNSUBCRIBE_TIMEOUT:
            EXAMPLE_TRACE("unsubscribe timeout, packet-id=%u", (unsigned int)packet_id);
            break;

        case IOTX_MQTT_EVENT_UNSUBCRIBE_NACK:
            EXAMPLE_TRACE("unsubscribe nack, packet-id=%u", (unsigned int)packet_id);
            break;

        case IOTX_MQTT_EVENT_PUBLISH_SUCCESS:
            EXAMPLE_TRACE("publish success, packet-id=%u", (unsigned int)packet_id);
            break;

        case IOTX_MQTT_EVENT_PUBLISH_TIMEOUT:
            EXAMPLE_TRACE("publish timeout, packet-id=%u", (unsigned int)packet_id);
            break;

        case IOTX_MQTT_EVENT_PUBLISH_NACK:
            EXAMPLE_TRACE("publish nack, packet-id=%u", (unsigned int)packet_id);
            break;

        case IOTX_MQTT_EVENT_PUBLISH_RECEIVED:
            EXAMPLE_TRACE("topic message arrived but without any related handle: topic=%.*s, topic_msg=%.*s",
                          topic_info->topic_len,
                          topic_info->ptopic,
                          topic_info->payload_len,
                          topic_info->payload);
            break;

        case IOTX_MQTT_EVENT_BUFFER_OVERFLOW:
            EXAMPLE_TRACE("buffer overflow, %s", msg->msg);
            break;

        default:
            EXAMPLE_TRACE("Should NOT arrive here.");
            break;
    }
}

static void mqtt_message_post(void *pclient)
{
		float fTemp,fBodyTemperature;

		uint8_t ret = SENSOR_DATA_FAIL;
		char ftoa_string[16] = {0};
		iotx_mqtt_topic_info_t topic_msg;
		char param[256];
		char msg_pub[512];
		int msg_len;
	
    //read sensor data
		Sensor_Auxadc_Tmpr_Get(&fTemp);
		ret = SENSOR_DATA_OK;

		if (SENSOR_DATA_OK == ret)
			 fBodyTemperature = fTemp;  
		else
			 fBodyTemperature = DEFAULT_BODY_TEMP; 
		
		ftoa(fBodyTemperature, ftoa_string, 1);
		EXAMPLE_TRACE("Get temperature valus: %s \n",ftoa_string);
		
    memset(&topic_msg, 0x0, sizeof(iotx_mqtt_topic_info_t));
    topic_msg.qos = IOTX_MQTT_QOS1;
    topic_msg.retain = 0;
    topic_msg.dup = 0;
    
    memset(param, 0, sizeof(param));
    memset(msg_pub, 0, sizeof(msg_pub));
		sprintf(param,  "{\"BodyTemperature\":%s}",ftoa_string);
		EXAMPLE_TRACE("Param data: %s \n",param);
	  msg_len = sprintf(msg_pub, ALINK_BODY_FORMAT, ++SendCount, param,ALINK_METHOD_PROP_POST);
    if (msg_len < 0) {
            printf("Error occur! Exit program\r\n");
    }

    topic_msg.payload = (void *)msg_pub;
    topic_msg.payload_len = msg_len;
    IOT_MQTT_Publish(pclient, ALINK_TOPIC_PROP_POST, &topic_msg);		
}

int mqtt_client(void)
{
    int rc = 0;
    void *pclient;
    iotx_conn_info_pt pconn_info;
    iotx_mqtt_param_t mqtt_params;

    char *msg_buf = NULL, *msg_readbuf = NULL;

    if (NULL == (msg_buf = (char *)HAL_Malloc(MQTT_MSGLEN))) {
        EXAMPLE_TRACE("not enough memory");
        rc = -1;
        goto do_exit;
    }

    if (NULL == (msg_readbuf = (char *)HAL_Malloc(MQTT_MSGLEN))) {
        EXAMPLE_TRACE("not enough memory");
        rc = -1;
        goto do_exit;
    }

    HAL_GetProductKey(__product_key);
    HAL_GetDeviceName(__device_name);
    HAL_GetDeviceSecret(__device_secret);

    /* Device AUTH */
    if (0 != IOT_SetupConnInfo(__product_key, __device_name, __device_secret, (void **)&pconn_info)) {
        EXAMPLE_TRACE("AUTH request failed!");
        rc = -1;
        goto do_exit;
    }
    else {
			  EXAMPLE_TRACE("AUTH request success");
		}
    /* Initialize MQTT parameter */
    memset(&mqtt_params, 0x0, sizeof(mqtt_params));

    mqtt_params.port = pconn_info->port;
    mqtt_params.host = pconn_info->host_name;
    mqtt_params.client_id = pconn_info->client_id;
    mqtt_params.username = pconn_info->username;
    mqtt_params.password = pconn_info->password;
    mqtt_params.pub_key = pconn_info->pub_key;

    mqtt_params.request_timeout_ms = 2000;
    mqtt_params.clean_session = 0;
    mqtt_params.keepalive_interval_ms = 60000;
    mqtt_params.pread_buf = msg_readbuf;
    mqtt_params.read_buf_size = MQTT_MSGLEN;
    mqtt_params.pwrite_buf = msg_buf;
    mqtt_params.write_buf_size = MQTT_MSGLEN;

    mqtt_params.handle_event.h_fp = event_handle;
    mqtt_params.handle_event.pcontext = NULL;


    /* Construct a MQTT client with specify parameter */
    pclient = IOT_MQTT_Construct(&mqtt_params);
    if (NULL == pclient) {
        EXAMPLE_TRACE("MQTT construct failed");
        rc = -1;
        goto do_exit;
    }
    else {
				HAL_SleepMs(100);
			  EXAMPLE_TRACE("MQTT construct success");
		}
			
		// here we implement publish operation 
			mqtt_message_post(pclient);
		

    do {
				// check if conection keep
				IOT_MQTT_Yield(pclient, 200);
				HAL_SleepMs(6000);
				mqtt_message_post(pclient);
			
				} while (1);
  
				
do_exit:
    if (NULL != msg_buf) {
        HAL_Free(msg_buf);
    }

    if (NULL != msg_readbuf) {
        HAL_Free(msg_readbuf);
    }

    IOT_MQTT_Destroy(&pclient);
    return rc;
	}

int mqtt_task(void)
{
    IOT_OpenLog("mqtt");
    IOT_SetLogLevel(IOT_LOG_DEBUG);

    HAL_SetProductKey(PRODUCT_KEY);
    HAL_SetDeviceName(DEVICE_NAME);
    HAL_SetDeviceSecret(DEVICE_SECRET);
#if defined(SUPPORT_ITLS)
    HAL_SetProductSecret(PRODUCT_SECRET);
#endif
    /* Choose Login Server */
    int domain_type = IOTX_CLOUD_DOMAIN_SH;
    IOT_Ioctl(IOTX_IOCTL_SET_DOMAIN, (void *)&domain_type);

    /* Choose Login  Method */
    int dynamic_register = 0;
    IOT_Ioctl(IOTX_IOCTL_SET_DYNAMIC_REGISTER, (void *)&dynamic_register);

    mqtt_client();
    IOT_DumpMemoryStats(IOT_LOG_DEBUG);
    IOT_CloseLog();

    EXAMPLE_TRACE("out of sample!");

    return 0;
}
