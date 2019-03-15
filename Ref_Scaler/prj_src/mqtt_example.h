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

#ifndef __MQTT_EXAMPLE_H__
#define __MQTT_EXAMPLE_H__

#include "scaler_profile.h"

/******************************************************
 *                    Macros
 ******************************************************/
/* These are pre-defined topics */
#define TOPIC_UPDATE            "/"PRODUCT_KEY"/"DEVICE_NAME"/update"
#define TOPIC_ERROR             "/"PRODUCT_KEY"/"DEVICE_NAME"/update/error"
#define TOPIC_GET               "/"PRODUCT_KEY"/"DEVICE_NAME"/get"
#define TOPIC_DATA              "/sys/"PRODUCT_KEY"/"DEVICE_NAME"/thing/service/property/set"

/* These are pre-defined topics format*/
#define TOPIC_UPDATE_FMT            "/%s/%s/update"
#define TOPIC_ERROR_FMT             "/%s/%s/update/error"
#define TOPIC_GET_FMT               "/%s/%s/get"
#define TOPIC_DATA_FMT              "/%s/%s/thing/service/property/set"

#define MQTT_MSGLEN             (1024)
#define ALINK_BODY_FORMAT         "{\"id\":\"%d\",\"version\":\"1.0\",\"params\":%s,\"method\":\"%s\"}"  
#define ALINK_METHOD_PROP_POST    "thing.event.property.post"    
#define ALINK_TOPIC_PROP_POST     "/sys/"PRODUCT_KEY"/"DEVICE_NAME"/thing/event/property/post"

#define MQTT_PUBLISH_TIME_INTERVAL   2000  // 2 seconds

/******************************************************
 *               Function Declarations
 ******************************************************/
void mqtt_message_post(void *pclient);
void mqtt_message_post_user_info(void *pclient);
void TakenUserInfoFromCloud(char*taken , char* userName, unsigned char* gender, unsigned char *height, unsigned char* age);
int mqtt_client(void);
int mqtt_task(void);

#endif //__MQTT_EXAMPLE_H__
