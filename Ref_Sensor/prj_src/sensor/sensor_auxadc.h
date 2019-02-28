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

/******************************************************************************
*  Filename:
*  ---------
*  sensor_auxadc.h
*
*  Project:
*  --------
*  OPL1000 Project - the AUXADC Sensor implement file
*
*  Description:
*  ------------
*  This implement file is include the AUXADC Sensor function and api.
*
*  Author:
*  -------
*  Luke Fang
*
******************************************************************************/
/***********************
Head Block of The File
***********************/
#ifndef _SENSOR_AUXADC_H_
#define _SENSOR_AUXADC_H_

#ifdef __cplusplus
extern "C" {
#endif

// Sec 0: Comment block of the file

// Sec 1: Include File
#include <stdio.h>
#include <stdint.h>


// Sec 2: Constant Definitions, Imported Symbols, miscellaneous
#define SENSOR_AUX_OK                  1
#define SENSOR_AUX_FAIL                0


/******************************
Declaration of data structure
******************************/
// Sec 3: structure, uniou, enum, linked list
// the calibration data of Temperature Sensor


/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global variable


// Sec 5: declaration of global function prototype
uint8_t Sensor_Auxadc_Tmpr_Get(float *pfTemperature);
uint8_t Sensor_Auxadc_Resistor_Calculation(float *fResistorOfThermistor);
uint8_t Sensor_Auxadc_Get_Value(uint32_t *ulDcValue, uint32_t *ulAdcConditionValue1, uint32_t *ulAdcConditionValue2);
uint8_t Sensor_Auxadc_Tmpr_Calibration(float fTmpr);
void Sensor_Auxadc_Init(void);

float Sensor_Auxadc_VBat_Convert_to_Percentage(void);

/***************************************************
Declaration of static Global Variables & Functions
***************************************************/
// Sec 6: declaration of static global variable


// Sec 7: declaration of static function prototype


#ifdef __cplusplus
}
#endif

#endif // _SENSOR_AUXADC_H_
