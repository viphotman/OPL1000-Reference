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
 * @file sensor_data.h
 * @author SH SDK
 * @date Feb.15, 2019 
 * @brief File containing declaration of sensor application & definition of structure for reference.
 *
 */
#ifndef __SENSOR_DATA_H__
#define __SENSOR_DATA_H__
#include <stdint.h>

/******************************************************
 *                    Macros
 ******************************************************/
#define SENSOR_DATA_OK      1
#define SENSOR_DATA_FAIL    0

#define SENSOR_MAC_LEN (6)
#define SENSOR_DB_TIME_LEN (16)
#define SENSOR_TEMP_NAME "temp"
#define SENSOR_TEMP_DEF (37.5)
#define SENSOR_TIME_NAME "time"
#define SENSOR_TIME_DEF "1519885635234"
#define SENSOR_DEVICE_NAME "device"
#define SENSOR_DEVICE_DEF "11:22:33:44:55:76"
#define SENSOR_BATTERY_NAME "battery"
#define SENSOR_BATTERY_DEF (50)

#define SENSOR_TEMP_COUNT  (128)  // buffer size that recorded in device internal 

/******************************************************
 *                 Type Definitions
 ******************************************************/
 

/******************************************************
 *                   Enumerations
 ******************************************************/
// the state of sensor data
typedef enum
{
    SENSOR_DATA_STATE_IDLE = 0,
    SENSOR_DATA_STATE_NETWORK_ON,
    SENSOR_DATA_STATE_DEVICE_ON,

    SENSOR_DATA_STATE_MAX
} E_SensorDataState;

/******************************************************
 *                    Structures
 ******************************************************/ 
/**
 * struct sensor_data - Sensor properity Block
 *
 * This data structure is used to provide storage for sensor device properities.
 */
typedef struct sensor_data_tag {
    unsigned char mac[SENSOR_MAC_LEN];      /* Device ID (Mac Addr) */
    int battery;                            /* Remaing Battery (%)  (Not support yet) */
}sensor_data_t;

/**
 * struct Sensor_Data_Temperature - Sensor Data Block
 *
 * This data structure is used to store sensor data, include temperature and measurement time stamp.
 */
typedef struct
{
    float faTemperature[SENSOR_TEMP_COUNT];     /* Temperature */
    uint32_t ulaSysTick[SENSOR_TEMP_COUNT];     /* Time */
    uint32_t ulReadIdx;                         /* Indicate reading pointer */
    uint32_t ulWriteIdx;                        /* Pointer indicate which unit is blank to store new data */
} Sensor_Data_Temperature_t;

/******************************************************
 *               Global Variables
 ******************************************************/
extern sensor_data_t g_SensorData;


/******************************************************
 *               Function Declarations
 ******************************************************/
void data_init(void);

/**
 * @brief This function gets time stamp when push sensor data (temperature) into g_tSensorDataTemperature.
 * @param[in] pulSysTick system time tick value.
 *
 * @return SENSOR_DATA_OK or SENSOR_DATA_FAIL.
 */
uint8_t Sensor_Data_TimeGet(uint32_t *pulSysTick);

int data_get_mac(void);
sensor_data_t* data_get_entity(void);

/**
 * @brief This function push sensor data (temperature and time stamp) into g_tSensorDataTemperature.
 * @param[in] fTemp temperature value, float type.
 * @param[in] ulSysTick system time tick.
 *
 * @return SENSOR_DATA_OK or SENSOR_DATA_FAIL.
 */
uint8_t Sensor_Data_TemperaturePush(float fTemp, uint32_t ulSysTick);

/**
 * @brief This function pop sensor data from g_tSensorDataTemperature.
 * @param[out] pfTemp temperature value, float type.
 * @param[out] pulSysTick system time tick.
 *
 * @return SENSOR_DATA_OK or SENSOR_DATA_FAIL.
 */
uint8_t Sensor_Data_TemperaturePop(float *pfTemp, uint32_t *pulSysTick);

/**
 * @brief This function updates read index value (corresponds to g_tSensorDataTemperature.ulReadIdx).
 *
 * @return SENSOR_DATA_OK or SENSOR_DATA_FAIL.
 */
uint8_t Sensor_Data_TemperatureReadIdxUpdate(void);

/**
 * @brief This function updates sensor operation state machine value
 *
 * @return None
 */
void Sensor_Data_StateSet(uint8_t ubState);

/**
 * @brief This function return g_ubSensorDataState which indicate sensor operation state machine value
 *
 * @return  sensor operation state machine value
 */
uint8_t Sensor_Data_StateGet(void);

#endif

