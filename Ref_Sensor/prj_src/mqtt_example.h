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

#define DEFAULT_BODY_TEMP       36.5
#define PRODUCT_KEY             "a1qqoiDBbFT"
#define DEVICE_NAME             "sh_sensor02"
#define DEVICE_SECRET           "QfnuQNldq0nrFr8NUy9zCI9dil55MIsU"

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

#endif //__MQTT_EXAMPLE_H__
