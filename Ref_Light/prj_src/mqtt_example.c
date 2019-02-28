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


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "iot_import.h"
#include "iot_export.h"
#include "hal_vic.h"
#include "hal_pwm.h"
#include "mqtt_example.h"


T_Ref_Smart_Light_Profile Smart_Light_Profile_def[NUNBER_OF_LIGHT] =
{
    {"RedLightSet"   , LIGHT_SWITCH_OFF, "RedLightAdjustLevel"   , MIN_LIGHT_BRIGHTNESS, HAL_PWM_IDX_5},
    {"GreenLightSet" , LIGHT_SWITCH_OFF, "GreenLightAdjustLevel" , MIN_LIGHT_BRIGHTNESS, HAL_PWM_IDX_4},
    {"BlueLightSet"  , LIGHT_SWITCH_OFF, "BlueLightAdjustLevel"  , MIN_LIGHT_BRIGHTNESS, HAL_PWM_IDX_3},
    {"WhiteLightSet" , LIGHT_SWITCH_OFF, "WhiteLightAdjustLevel" , MIN_LIGHT_BRIGHTNESS, HAL_PWM_IDX_2},
    {"YellowLightSet", LIGHT_SWITCH_OFF, "YellowLightAdjustLevel", MIN_LIGHT_BRIGHTNESS, HAL_PWM_IDX_1},
};
    
int gIndex=0;
uint32_t SendCount = 0;

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


void TakenLightSettingFromCloud(char*taken , char* keyword, int* value, int* pwmindex)
{
    for( gIndex = 0; gIndex < NUNBER_OF_LIGHT; gIndex++)
    {
        char *ptr = NULL ;
        int nTakenLen = 0;
        ptr = strstr(taken, Smart_Light_Profile_def[gIndex].Topic_of_Light_Switch);
        if(ptr != NULL)
        {
            nTakenLen = strlen(Smart_Light_Profile_def[gIndex].Topic_of_Light_Switch);
            ptr = (char *)&ptr[nTakenLen+2];
		    int len = strcspn(ptr,"}");
		    ptr[len]='\0';
            Smart_Light_Profile_def[gIndex].Status_of_Light_Switch = atoi(ptr);
            *value = Smart_Light_Profile_def[gIndex].Status_of_Light_Switch;
            strcpy(keyword,Smart_Light_Profile_def[gIndex].Topic_of_Light_Switch);
            *pwmindex = Smart_Light_Profile_def[gIndex].IO_Control_Index;
            printf(" %d [ %s --> Smart_Light_Profile_def[%d] = %d --->%d]  \r\n",__LINE__, keyword, gIndex, *value,*pwmindex);
            break;
        }
        else
        {
            ptr = strstr(taken, Smart_Light_Profile_def[gIndex].Topic_of_Light_Brightness);
            if(ptr != NULL)
            {
                nTakenLen = strlen(Smart_Light_Profile_def[gIndex].Topic_of_Light_Brightness);
                ptr = (char *)&ptr[nTakenLen+2];
		        int len = strcspn(ptr,"}");
		        ptr[len]='\0';
                Smart_Light_Profile_def[gIndex].Level_of_Light_Brightness = atoi(ptr);
                *value = Smart_Light_Profile_def[gIndex].Level_of_Light_Brightness;
                strcpy(keyword,Smart_Light_Profile_def[gIndex].Topic_of_Light_Brightness);
                *pwmindex = Smart_Light_Profile_def[gIndex].IO_Control_Index;
                printf(" %d [ %s --> Smart_Light_Profile_def[%d] = %d --->%d]  \r\n",__LINE__, keyword, gIndex, *value,*pwmindex);
                break;
            }
        }    
        *value = -1;
    }
}


static void _demo_message_arrive(void *pcontext, void *pclient, iotx_mqtt_event_msg_pt msg)
{
    iotx_mqtt_topic_info_pt ptopic_info = (iotx_mqtt_topic_info_pt) msg->msg;

    printf("--- %s --- %d --- %s ptopic_info->payload=%s  \r\n",__FUNCTION__,__LINE__,__TIME__,ptopic_info->payload);
    char keyword[LENGTH_OF_LIGHT_STATUS]={0};
    int value,PWMIndex = -1;

    iotx_mqtt_topic_info_t topic_msg;
    char param[256];
    char param2[256];
    char msg_pub[512];

    memset(param, 0, sizeof(param));
    memset(param2, 0, sizeof(param2));
    
    TakenLightSettingFromCloud((char*)ptopic_info->payload, keyword, &value, &PWMIndex);

    printf("-- %s -- %d  value=%d  PWMIndex is %d \r\n",__FUNCTION__,__LINE__,value,PWMIndex);

    if(value > 0)
    {
        if(value == 1)
        {
            Hal_Pwm_SimpleConfigSet(PWMIndex,MAX_LIGHT_BRIGHTNESS,DEAFAULT_PWM_FREQ); // default bringtness level
            sprintf(param2,  "{\"%s\":%d}",Smart_Light_Profile_def[gIndex].Topic_of_Light_Brightness,MAX_LIGHT_BRIGHTNESS);
        }
        else if (value > 1)
        {
            Hal_Pwm_SimpleConfigSet(PWMIndex,value,DEAFAULT_PWM_FREQ);
            sprintf(param2,  "{\"%s\":%d}",Smart_Light_Profile_def[gIndex].Topic_of_Light_Switch,LIGHT_SWITCH_ON);
        }
            
        Hal_Pwm_Enable(PWMIndex);
        
        //sprintf(param,  "{\"LightSwitch\":%d}",g_LightSwitch);
        sprintf(param,  "{\"%s\":%d}",keyword,value);
    }
    else if(value == 0)
    {
        Hal_Pwm_SimpleConfigSet(PWMIndex,MIN_LIGHT_BRIGHTNESS,DEAFAULT_PWM_FREQ);
        Hal_Pwm_Enable(PWMIndex);
        Hal_Pwm_Disable(PWMIndex);
        sprintf(param , "{\"%s\":%d}",Smart_Light_Profile_def[gIndex].Topic_of_Light_Switch,LIGHT_SWITCH_OFF);
        sprintf(param2 , "{\"%s\":%d}",Smart_Light_Profile_def[gIndex].Topic_of_Light_Brightness,MIN_LIGHT_BRIGHTNESS);
    }
    else
    {
        printf("----------- parser topic error ------------- \r\n");
        printf("----------- parser topic error ------------- \r\n");
        printf("----------- parser topic error ------------- \r\n");
        printf("----------- parser topic error ------------- \r\n");
        printf("----------- parser topic error ------------- \r\n");
    }

    printf("-- %s -- %d  param = %s   \r\n",__FUNCTION__,__LINE__,param);
    printf("-- %s -- %d  param2 = %s  \r\n",__FUNCTION__,__LINE__,param2);
    

    memset(&topic_msg, 0x0, sizeof(iotx_mqtt_topic_info_t));
    topic_msg.qos = IOTX_MQTT_QOS1;
    topic_msg.retain = 0;
    topic_msg.dup = 0;

    //read sensor data

    memset(msg_pub, 0, sizeof(msg_pub));
    int msg_len = sprintf(msg_pub, ALINK_BODY_FORMAT, ++SendCount, param, ALINK_METHOD_PROP_POST);
    if (msg_len < 0) {
            printf("Error occur! Exit program\r\n");
    }

    topic_msg.payload = (void *)msg_pub;
    topic_msg.payload_len = msg_len;
    IOT_MQTT_Publish(pclient, ALINK_TOPIC_PROP_POST, &topic_msg);


    memset(msg_pub, 0, sizeof(msg_pub));
	msg_len = sprintf(msg_pub, ALINK_BODY_FORMAT, ++SendCount, param2, ALINK_METHOD_PROP_POST);
    if (msg_len < 0) {
            printf("Error occur! Exit program\r\n");
    }

    topic_msg.payload = (void *)msg_pub;
    topic_msg.payload_len = msg_len;
    IOT_MQTT_Publish(pclient, ALINK_TOPIC_PROP_POST, &topic_msg);

    
    /* print topic name and topic message */
    EXAMPLE_TRACE("----");
    EXAMPLE_TRACE("packetId: %d", ptopic_info->packet_id);
    EXAMPLE_TRACE("Topic: '%.*s' (Length: %d)",
                  ptopic_info->topic_len,
                  ptopic_info->ptopic,
                  ptopic_info->topic_len);
    EXAMPLE_TRACE("Payload: '%.*s' (Length: %d)",
                  ptopic_info->payload_len,
                  ptopic_info->payload,
                  ptopic_info->payload_len);
    EXAMPLE_TRACE("----");
}

int mqtt_client(void)
{
    int rc = 0;
    void *pclient;
    iotx_conn_info_pt pconn_info;
    iotx_mqtt_param_t mqtt_params;
    iotx_mqtt_topic_info_t topic_msg;
    char msg_pub[128];
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

    /* Initialize topic information */
    memset(&topic_msg, 0x0, sizeof(iotx_mqtt_topic_info_t));
    strcpy(msg_pub, "update: hello! start!");

    topic_msg.qos = IOTX_MQTT_QOS1;
    topic_msg.retain = 0;
    topic_msg.dup = 0;
    topic_msg.payload = (void *)msg_pub;
    topic_msg.payload_len = strlen(msg_pub);

    /* Subscribe the specific topic */
    rc = IOT_MQTT_Subscribe(pclient, TOPIC_DATA, IOTX_MQTT_QOS1, _demo_message_arrive, NULL);
    if (rc < 0) {
        IOT_MQTT_Destroy(&pclient);
        EXAMPLE_TRACE("IOT_MQTT_Subscribe() failed, rc = %d", rc);
        rc = -1;
        goto do_exit;
    }

    //IOT_MQTT_Yield(pclient, 200);

    //HAL_SleepMs(2000);

    /* Initialize topic information */
    memset(msg_pub, 0x0, 128);
    strcpy(msg_pub, "data: hello! start!");
    memset(&topic_msg, 0x0, sizeof(iotx_mqtt_topic_info_t));
    topic_msg.qos = IOTX_MQTT_QOS1;
    topic_msg.retain = 0;
    topic_msg.dup = 0;
    topic_msg.payload = (void *)msg_pub;
    topic_msg.payload_len = strlen(msg_pub);

    //IOT_MQTT_Yield(pclient, 200);

    do {
       
        IOT_MQTT_Yield(pclient, 200);
        } while (1);
   

do_exit:
    if (NULL != msg_buf) {
        HAL_Free(msg_buf);
    }

    if (NULL != msg_readbuf) {
        HAL_Free(msg_readbuf);
    }

    //IOT_MQTT_Yield(pclient, 200);

    IOT_MQTT_Unsubscribe(pclient, TOPIC_DATA);

    //IOT_MQTT_Yield(pclient, 200);

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
