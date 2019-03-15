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
#include "blewifi_common.h"

#include "ftoa_util.h"
#include "sys_os_config.h"
#include "scaler_profile.h"
#include "scaler_data.h"
#include "scaler_sys.h"
#include "mqtt_example.h"
#include "sntp_time.h"

/********************************************
Declaration of Global Variables & Functions
********************************************/
char __product_key[PRODUCT_KEY_LEN + 1];
char __device_name[DEVICE_NAME_LEN + 1];
char __device_secret[DEVICE_SECRET_LEN + 1];

uint32_t SendCount = 0;
unsigned int g_sntp_tm = 0;

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

void mqtt_message_post_user_info(void *pclient)
{
    unsigned char mac_addr[SCALER_MAC_LEN] = {0};		
		iotx_mqtt_topic_info_t topic_msg;
		char param[256];
		char msg_pub[512];
		int i, user_id, msg_len;
		scaler_user_t* pOneUser;
		
    // get mac address info	
    sys_get_mac(mac_addr);
		
		// send MAC address message as an independent topic 
		// obtain vbat percent value from g_ScalerRptData		
    memset(&topic_msg, 0x0, sizeof(iotx_mqtt_topic_info_t));
    topic_msg.qos = IOTX_MQTT_QOS1;
    topic_msg.retain = 0;
    topic_msg.dup = 0;

    memset(param, 0, sizeof(param));
    memset(msg_pub, 0, sizeof(msg_pub)); 

		sprintf(param,  "{\"Mac\":\""MACSTR"\"}",MAC2STR(mac_addr));
		EXAMPLE_TRACE("Param data: %s \n",param);		

	  msg_len = sprintf(msg_pub, ALINK_BODY_FORMAT, ++SendCount, param,ALINK_METHOD_PROP_POST);
    if (msg_len < 0) {
        printf("Error occur! Exit program\r\n");
    }

    topic_msg.payload = (void *)msg_pub;
    topic_msg.payload_len = msg_len;
    IOT_MQTT_Publish(pclient, ALINK_TOPIC_PROP_POST, &topic_msg);	
		
		// send stored user information to cloud 
    if(g_ScalerUser.user_num > 0)
    {
			for (i=0;i<g_ScalerUser.user_num;i++)
			{
				pOneUser = &(g_ScalerUser.reg_user[i]);
				
				memset(&topic_msg, 0x0, sizeof(iotx_mqtt_topic_info_t));
				topic_msg.qos = IOTX_MQTT_QOS1;
				topic_msg.retain = 0;
				topic_msg.dup = 0;

				memset(param, 0, sizeof(param));
				memset(msg_pub, 0, sizeof(msg_pub)); 
        user_id = i+1;
				sprintf(param,  "{\"UserId\":%d,\"Name\":\"%s\",\"Gender\":%d,\"Height\":%d,\"Age\":%d}",
						user_id,pOneUser->user_name,pOneUser->sex,pOneUser->height,pOneUser->age);
				EXAMPLE_TRACE("Param data: %s \n",param);		

				msg_len = sprintf(msg_pub, ALINK_BODY_FORMAT, ++SendCount, param,ALINK_METHOD_PROP_POST);
				if (msg_len < 0) {
								printf("Error occur! Exit program\r\n");
				}

				topic_msg.payload = (void *)msg_pub;
				topic_msg.payload_len = msg_len;
				IOT_MQTT_Publish(pclient, ALINK_TOPIC_PROP_POST, &topic_msg);	
			}
		}
}

void mqtt_message_post(void *pclient)
{
		float fWeight;
	//,fBatPercent;
		char vbat_percent_str[8] = {0};
		char weight_str[8] = {0};	
    char timeStamp_buffer[TIME_STAMP_STR_LEN] = {0};		
		iotx_mqtt_topic_info_t topic_msg;
		char param[256];
		char msg_pub[512];
		int msg_len;
	  int vbat_percent,vbat_enum,iWeight;
		uint8_t ret;
		unsigned char IutIpAddrHex[4] = {192,168,1,105};
		
    //read weight data from g_ScalerRptData, g_ScalerRptData has been updated by AT cmd process function
		ret = scaler_RingBufData_Pop(&iWeight);
		if (ret != SCALER_DATA_OK)  // read weight information failed. 
		{
			EXAMPLE_TRACE("There is no new weight information! \n");
			return;
		}
		
		fWeight = (float)iWeight / 1000.0;  // convert unit from g to kg 		
		ftoa(fWeight, weight_str, 1);
		EXAMPLE_TRACE("Get weight valus: %s kg\n",weight_str);
		
		// obtain vbat percent value from g_ScalerRptData, 
		// calling AUX battery function API, TBD
		vbat_percent = g_ScalerRptData.battery;
		//fBatPercent = (float)vbat_percent/1.0;
		scaler_battery_conv(vbat_percent, vbat_percent_str, &vbat_enum);
	  EXAMPLE_TRACE("Battery percent: %s \n",vbat_percent_str);
    
		// get time stamp
    if(true != sys_get_timeStamp(timeStamp_buffer))
		{
			EXAMPLE_TRACE(" Get SNTP time stamp error!");
		}
		
		scaler_update_ap_conn(AP_CONNECTION_SUCCESS, IutIpAddrHex);
		
		// obtain vbat percent value from g_ScalerRptData		
    memset(&topic_msg, 0x0, sizeof(iotx_mqtt_topic_info_t));
    topic_msg.qos = IOTX_MQTT_QOS1;
    topic_msg.retain = 0;
    topic_msg.dup = 0;
	
    memset(param, 0, sizeof(param));
    memset(msg_pub, 0, sizeof(msg_pub)); 

		sprintf(param,  "{\"Name\":\"%s\",\"Weight\":%s,\"BatteryPercentage\":%s,\"BatteryStatus\":%d,\"TimeStamp\":\"%s\"}",
				g_ScalerRptData.user,weight_str,vbat_percent_str,vbat_enum,timeStamp_buffer);
		EXAMPLE_TRACE("Param data: %s \n",param);		

	  msg_len = sprintf(msg_pub, ALINK_BODY_FORMAT, ++SendCount, param,ALINK_METHOD_PROP_POST);
    if (msg_len < 0) {
        printf("Error occur! Exit program\r\n");
    }

    topic_msg.payload = (void *)msg_pub;
    topic_msg.payload_len = msg_len;
    IOT_MQTT_Publish(pclient, ALINK_TOPIC_PROP_POST, &topic_msg);	

		// May be changed later ! 
		scaler_RingBufData_Push(MQTT_PUBLISH_SUCCESS);
}

void TakenUserInfoFromCloud(char*taken , char* userName, unsigned char* gender, unsigned char *height, unsigned char* age)
{
		char *ptr = NULL ;
		int len, nTakenLen = 0;
	
		ptr = strstr(taken, Scaler_User_Profile.Topic_of_User_Name);
		if(ptr != NULL)
		{
				nTakenLen = strlen(Scaler_User_Profile.Topic_of_User_Name);
				ptr = (char *)&ptr[nTakenLen+2];
		    len = strcspn(ptr,"}");
		    ptr[len]='\0';
			  strcpy(Scaler_User_Profile.user_name,ptr);	
        strcpy(userName,Scaler_User_Profile.user_name);
				printf(" %d [ %s --> Scaler_User_Profile[%s] = %s]  \r\n",__LINE__, Scaler_User_Profile.Topic_of_User_Name, userName);
		}
		else
		    strcpy(userName,"unknown");
	
		ptr = strstr(taken, Scaler_User_Profile.Topic_of_Gender);
		if(ptr != NULL)
		{
				nTakenLen = strlen(Scaler_User_Profile.Topic_of_Gender);
				ptr = (char *)&ptr[nTakenLen+2];
		    len = strcspn(ptr,"}");
		    ptr[len]='\0';
			  //strcpy(Scaler_User_Profile.gender,ptr);
			  Scaler_User_Profile.gender = atoi(ptr);
			  *gender = Scaler_User_Profile.gender;
				printf(" %d [ %s --> Scaler_User_Profile[%s] = %d]  \r\n",__LINE__, Scaler_User_Profile.Topic_of_Gender, *gender);
		}
		else
			  gender = SCALER_GENDER_MALE;

		ptr = strstr(taken, Scaler_User_Profile.Topic_of_Height);
		if(ptr != NULL)
		{
				nTakenLen = strlen(Scaler_User_Profile.Topic_of_Height);
				ptr = (char *)&ptr[nTakenLen+2];
		    len = strcspn(ptr,"}");
		    ptr[len]='\0';
			  Scaler_User_Profile.height = atoi(ptr);
			  *height = Scaler_User_Profile.height;
				printf(" %d [ %s --> Scaler_User_Profile[%s] = %d cm]  \r\n",__LINE__, Scaler_User_Profile.Topic_of_Height, *height);
		}
		else
		    *height = 0;
		
		ptr = strstr(taken, Scaler_User_Profile.Topic_of_Age);
		if(ptr != NULL)
		{
				nTakenLen = strlen(Scaler_User_Profile.Topic_of_Age);
				ptr = (char *)&ptr[nTakenLen+2];
		    len = strcspn(ptr,"}");
		    ptr[len]='\0';
			  Scaler_User_Profile.age = atoi(ptr);
			  *age = Scaler_User_Profile.age;
				printf(" %d [ %s --> Scaler_User_Profile[%s] = %d]  \r\n",__LINE__, Scaler_User_Profile.Topic_of_Age, *age);
		}
		else
			 *age = 30;
}

/*
static void _demo_message_arrive(void *pcontext, void *pclient, iotx_mqtt_event_msg_pt msg)
{
    iotx_mqtt_topic_info_pt ptopic_info = (iotx_mqtt_topic_info_pt) msg->msg;
    //iotx_mqtt_topic_info_t topic_msg;

    char msg_pub[512];
    char userName[SCLAER_USER_NAME_LEN];
	  unsigned char gender;
	  unsigned char height;
	  unsigned char age;
	
    printf("--- %s --- %d --- %s ptopic_info->payload=%s  \r\n",__FUNCTION__,__LINE__,__TIME__,ptopic_info->payload);

	  // get user profile info from cloud publish message 
    TakenUserInfoFromCloud((char*)ptopic_info->payload,userName,&gender,&height,&age);
	  // fill user info into g_ScalerUser
    scaler_update_user(userName, gender, height, age, &g_ScalerUser);
    
    // print topic name and topic message 
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
*/
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

    /* Here we assume internet connection is normal, inquire time info from SNTP server */
    if( true != sntp_date_get(&g_sntp_tm))
    {
			EXAMPLE_TRACE("Inquire time info from SNTP server failed.");
		}	
		
		/* pulish MAC address and user group information that stored in device to cloud */
		mqtt_message_post_user_info(pclient);
		
    /* Subscribe the specific topic 
    rc = IOT_MQTT_Subscribe(pclient, TOPIC_DATA, IOTX_MQTT_QOS1, _demo_message_arrive, NULL);
    if (rc < 0) {
        IOT_MQTT_Destroy(&pclient);
        EXAMPLE_TRACE("IOT_MQTT_Subscribe() failed, rc = %d", rc);
        rc = -1;
        goto do_exit;
    }
		*/
		
		// here publish weight, time, battery etc. info 
		mqtt_message_post(pclient);

    do {
					// check if conection keep
					IOT_MQTT_Yield(pclient, 200);
					
					// Temperary processing method: check reported weight inforperiodically 
			    // if there is no new weight info, publish action will be skipped 
					HAL_SleepMs(MQTT_PUBLISH_TIME_INTERVAL);
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
