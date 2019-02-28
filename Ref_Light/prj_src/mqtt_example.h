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

#ifndef __MQTT_EXAMPL_H__
#define __MQTT_EXAMPL_H__

#define PRODUCT_KEY             "a1lIIDXvQHv"
#define DEVICE_NAME             "uTeMF5vcdG69OuoDxXND"
#define DEVICE_SECRET           "eo6q81pJCpngkJ3ZVQk7XbyT6T4uSVqt"

/* define linght status */
#define NUNBER_OF_LIGHT        5
#define NUNBER_STATUS_OF_LIGHT 10
#define LENGTH_OF_LIGHT_STATUS 32
#define MAX_LIGHT_BRIGHTNESS   95
#define MIN_LIGHT_BRIGHTNESS   0
#define LIGHT_SWITCH_ON        1
#define LIGHT_SWITCH_OFF       0
#define DEAFAULT_PWM_FREQ      500


char __product_key[PRODUCT_KEY_LEN + 1];
char __device_name[DEVICE_NAME_LEN + 1];
char __device_secret[DEVICE_SECRET_LEN + 1];

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

#define EXAMPLE_TRACE(fmt, ...)  \
    do { \
        HAL_Printf("%s|%03d :: ", __func__, __LINE__); \
        HAL_Printf(fmt, ##__VA_ARGS__); \
        HAL_Printf("%s", "\r\n"); \
    } while(0)

#define ALINK_BODY_FORMAT         "{\"id\":\"%d\",\"version\":\"1.0\",\"params\":%s,\"method\":\"%s\"}"  
#define ALINK_METHOD_PROP_POST    "thing.event.property.post"    
#define ALINK_TOPIC_PROP_POST     "/sys/"PRODUCT_KEY"/"DEVICE_NAME"/thing/event/property/post"


typedef struct 
{
    char Topic_of_Light_Switch[32];      // function defined in cloud server ex: RedLightSet
    uint8_t Status_of_Light_Switch;      // 0 : off 1: On
    char Topic_of_Light_Brightness[32];  // function defined in cloud server ex: RedLightAdjustLevel
    uint32_t Level_of_Light_Brightness;  // 0~100
    uint32_t IO_Control_Index;           // ex : GPIO_IDX_23 or   HAL_PWM_IDX_5
} T_Ref_Smart_Light_Profile;

#endif /* __BLEWIFI_CONFIGURATION_H__ */

