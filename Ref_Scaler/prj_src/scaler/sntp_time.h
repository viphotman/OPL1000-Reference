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



#ifndef _SNTP_TIME_H_
#define _SNTP_TIME_H_

/***********************
Head Block of The File
***********************/

// Sec 1: Include File

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "lwip_helper.h"
#include "cmsis_os.h"
#include "arch.h"
#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwip/netdb.h"
#include "lwip/dns.h"
#include "lwip/netif.h"
#include "errno.h"
#include "lwip/inet.h" 
#include <time.h>

/******************************************************
 *                    Macros
 ******************************************************/

#define SNTP_SERVER "1.cn.pool.ntp.org"
#define SNTP_PORT  123
#define SNTP_MAX_SERVERS 3

#define DEFF_1970    (2209017600)  /* 1970 - 1900 in seconds */
#define DEFF_8HOUR   (28800)
#define CONVERT_TIME(s) (s - DEFF_1970 + 2 * DEFF_8HOUR)

#define SNTP_CONNECT_TIME_OUT_MS         60000
#define SNTP_SERVR_TRY_CONNECT_INTERVAL  2000
#define TIME_STAMP_STR_LEN          64 

/******************************************************
 *                 Type Definitions
 ******************************************************/
 
/******************************
Declaration of data structure
******************************/
typedef struct {
    uint8_t li_vn_mode;
    uint8_t stratum;
    uint8_t poll;
    uint8_t precision;
    int32_t rootdelay;
    int32_t rootdispersion;
    int8_t  refid[4];
    int32_t reftimeint;
    int32_t reftimefraction;
    int32_t oritimeint;
    int32_t oritimefraction;
    int32_t recvtimeint;
    int32_t recvtimefraction;
    int32_t trantimeint;
    int32_t trantimefraction;
} sntp_header_t;


/********************************************
Declaration of Global Variables & Functions
********************************************/

int sntp_date_get(unsigned int *pSntpTime);

#endif 
