/******************************************************************************
*  Copyright 2017 - 2018, Opulinks Technology Ltd.
*  ---------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Opulinks Technology Ltd. (C) 2018
******************************************************************************/

#ifndef _AT_CMD_SCALER_EXT_H_
#define _AT_CMD_SCALER_EXT_H_

#include "at_cmd.h"
#include <stdio.h>
#include <stdint.h>

#define SCALER_WEIGHT_COUNT  (128)  // buffer size that recorded in device internal 

#define MQTT_PUBLISH_SUCCESS  1 
#define MQTT_PUBLISH_FAILED   0
#define MQTT_PUBLISH_UNKNOWN  -1

#define AP_CONNECTION_SUCCESS  1 
#define AP_CONNECTION_BROKEN   0
#define AP_CONNECTION_READ     -1

/**
 * structure Scaler_Data_Weight - Sensor Data Block
 *
 * This data structure is used to store scaler data, include weight, cloud updating flag and measurement time stamp.
 */
typedef struct
{
    int iWeight[SCALER_WEIGHT_COUNT];           /* Weight in g unit */
    int iFlag[SCALER_WEIGHT_COUNT];             /* Cloud uploading flag, 0 failed, 1 success */    
    int ulReadIdx;                              /* Indicate reading pointer */
    int ulWriteIdx;                             /* Pointer indicate which unit is blank to store new data */
} Scaler_Data_t;

/**
 * structure Scaler_AP_Inquire - AP connection inquire result block
 *
 * This data structure is used to store AP connection status inquire result.
 */
typedef struct
{
    char cIutIpAddr[16];        /* OPL1000 device IP address that obtained from AP, like "192.168.1.104" */
    int iFlag;                  /* AP connection flag, 0 disconnected, 1 connected, -1 means it has been read by AT command */    
} AP_Connect_t;

extern Scaler_Data_t *g_pScalerData;
extern AP_Connect_t *g_pScalerApConn;

extern int at_cmd_scal_inq_ap(char *buf, int len, int mode);
extern int at_cmd_scal_send_dat(char *buf, int len, int mode);
extern int at_cmd_scal_inq_cld(char *buf, int len, int mode);
extern int at_cmd_scal_dp_slp(char *buf, int len, int mode);

#endif /* _AT_CMD_SCALER_EXT_H_ */
