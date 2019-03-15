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
 * @file scaler_data.h
 * @author SH SDK
 * @date Mar.05, 2019 
 * @brief File containing declaration of scaler  application & definition of structure for reference.
 *
 */
#ifndef __SCALER_DATA_H__
#define __SCALER_DATA_H__
#include <stdint.h>
#include "cmsis_os.h"
#include "scaler_profile.h"
#include "at_cmd_scaler_ext.h"
/******************************************************
 *                    Macros
 ******************************************************/

/******************************************************
 *                 Type Definitions
 ******************************************************/
 

/******************************************************
 *                   Enumerations
 ******************************************************/


/******************************************************
 *                    Structures
 ******************************************************/ 


/******************************************************
 *               Global Variables
 ******************************************************/
extern uint8_t g_ubScalerDataState;
extern osSemaphoreId g_tScalerDataSemaphoreId;
extern Scaler_Data_t g_tScalerData;
extern AP_Connect_t g_tScalerApConnect;

/******************************************************
 *               Function Declarations
 ******************************************************/
 /**
 * @brief This function initialize AT communication 
 *        related global structure, such as ring buffer.
 * @param[in] none.
 *
 * @return none.
 */
void comm_data_init(void);

/**
 * @brief This function push "AP connection" flag into g_tScalerApConn.
 * @param[in] iApConnFlag: AP connection flag.
 * @param[in] IutIpAddr: OPL1000 IUT IP address that obtained from AP  
 *
 * @return SCALER_DATA_OK or SCALER_DATA_FAIL.
 */
uint8_t scaler_update_ap_conn(int iApConnFlag, unsigned char *pIutIpAddrHex);

/**
 * @brief This function push "publish data to cloud result" flag into ring buffer.
 * @param[out] none.
 *
 * @return SCALER_DATA_OK or SCALER_DATA_FAIL.
 */
uint8_t scaler_RingBufData_Push(int iCloudConnFlag); 

/**
 * @brief This function pop scaler data from g_tScalerData.
 * @param[out] pfWeight weight value, float type.
 * @param[out] pulSysTick system time tick.
 *
 * @return SCALER_DATA_OK or SCALER_DATA_FAIL.
 */
uint8_t scaler_RingBufData_Pop(int *piWeight);

/**
 * @brief This function updates read index value (corresponds to g_tScalerDataWeight.ulReadIdx).
 *
 * @return SCALER_DATA_OK or SCALER_DATA_FAIL.
 */
uint8_t scaler_Data_WeightReadIdxUpdate(void);

/**
 * @brief This function updates scaler operation state machine value
 *
 * @return None
 */
void scaler_Data_StateSet(uint8_t ubState);

/**
 * @brief This function return g_ubScalerDataState which indicate scaler  operation state machine value
 *
 * @return  scaler  operation state machine value
 */
uint8_t scaler_Data_StateGet(void);

#endif

