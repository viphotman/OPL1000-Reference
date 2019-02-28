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
*  sensor_auxadc.c
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
// Sec 0: Comment block of the file


// Sec 1: Include File
#include <stdlib.h>
#include <string.h>
#include "sensor_auxadc.h"
#include "hal_pin_def.h"
#include "hal_pin.h"
#include "hal_auxadc.h"
#include "hal_auxadc_internal.h"
#include "hal_vic.h"    
#include "hal_tick.h"
#include "mw_fim\mw_fim_default_group03.h"
#include "boot_sequence.h"
#include "blewifi_configuration01.h"

// Sec 2: Constant Definitions, Imported Symbols, miscellaneous


/********************************************
Declaration of data structure
********************************************/
// Sec 3: structure, uniou, enum, linked list

/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global variable
T_HalTmprCalData g_tAPPTmpr_CalData;
uint32_t g_ulSensorTmprMovingAverageCount;
uint32_t g_ulSensorVbatMovingAverageCount;
float g_fSensorTmprCurrentValue;
float g_fSensorVbatCurrentValue;


// Sec 5: declaration of global function prototype


/***************************************************
Declaration of static Global Variables & Functions
***************************************************/
// Sec 6: declaration of static global variable


// Sec 7: declaration of static function prototype


/***********
C Functions
***********/
// Sec 8: C Functions



/*************************************************************************
* FUNCTION:
*   Hal_Tmpr_CompareResistor
*
* DESCRIPTION:
*   compare the resistor to get the temperature
*
* PARAMETERS
*   1. fResistor     : [In] the resistor
*   2. pfTemperature : [Out] the temperature
*
* RETURNS
*   1. SENSOR_AUX_OK   : success
*   2. SENSOR_AUX_FAIL : fail
*
*************************************************************************/
uint8_t Sensor_Auxadc_Tmpr_CompareResistor(float fResistor, float *pfTemperature)
{
    float fInterpolation;
    uint8_t i;
    //printf("Sensor_Auxadc_Tmpr_CompareResistor \n");
    // max resistor : min temperature
    if (fResistor >= g_tAPPTmpr_CalData.faThermistor[0])
    {
        *pfTemperature = g_tAPPTmpr_CalData.fBaseTemperature;
    }
    // min resistor : max temperature
    else if (fResistor < g_tAPPTmpr_CalData.faThermistor[HAL_TMPR_STEP_MAX - 1])
    {
        *pfTemperature = g_tAPPTmpr_CalData.fBaseTemperature + (HAL_TMPR_STEP_MAX - 1);
    }
    // others
    else
    {
        for (i=0; i<(HAL_TMPR_STEP_MAX-1); i++)
        {
            if ((g_tAPPTmpr_CalData.faThermistor[i] > fResistor) && (fResistor >= g_tAPPTmpr_CalData.faThermistor[i+1]))
            {
                fInterpolation = 1 - ((fResistor - g_tAPPTmpr_CalData.faThermistor[i+1])
                                      / (g_tAPPTmpr_CalData.faThermistor[i] - g_tAPPTmpr_CalData.faThermistor[i+1]));
                *pfTemperature = g_tAPPTmpr_CalData.fBaseTemperature + i + fInterpolation;

                break;
            }
        }
    }

    return HAL_TMPR_OK;
}

/*************************************************************************
* FUNCTION:
*   Sensor_Auxadc_Tmpr_Get
*
* DESCRIPTION:
*   Convert resistor to temperature
*
* PARAMETERS
*   1. pfTemperature : [Out] the temperature
*
* RETURNS
*   1. SENSOR_AUX_OK   : success
*   2. SENSOR_AUX_FAIL : fail
*
*************************************************************************/
uint8_t Sensor_Auxadc_Tmpr_Get(float *pfTemperature)
{
    uint8_t ubRet = SENSOR_AUX_FAIL;
    float   ulValue = 0;
    float   fTemperature;
    float   fAverageTemperature;

    /* The resistor of thermistor */
    if (SENSOR_AUX_OK != Sensor_Auxadc_Resistor_Calculation(&ulValue))
        goto done;

    /* Through resistor of thermistor convert to temperature */    
    if (SENSOR_AUX_OK != Sensor_Auxadc_Tmpr_CompareResistor(ulValue, &fTemperature))
        goto done;

    // error handle: if the average count = 0
    if (SENSOR_MOVING_AVERAGE_COUNT == 0)
    {
        fAverageTemperature = fTemperature;
    }
    // compute the moving average value
    else
    {
        // if the current is in critical range, compute the moving average value
        if ((SENSOR_TMPR_CRITICAL_LOW <= g_fSensorTmprCurrentValue)
            && (g_fSensorTmprCurrentValue <= SENSOR_TMPR_CRITICAL_HIGH))
        {
            // if the new value is out of boundary, use it directly
            if ((fTemperature < (SENSOR_TMPR_CRITICAL_LOW - SENSOR_TMPR_CRITICAL_BOUNDARY_DIFF))
                || (fTemperature > (SENSOR_TMPR_CRITICAL_HIGH + SENSOR_TMPR_CRITICAL_BOUNDARY_DIFF)))
            {
                fAverageTemperature = fTemperature;
            }
            else
            {
                // compute the new moving average value
                fAverageTemperature = g_fSensorTmprCurrentValue * (SENSOR_MOVING_AVERAGE_COUNT - 1);
                fAverageTemperature = (fAverageTemperature + fTemperature) / SENSOR_MOVING_AVERAGE_COUNT;
            }
        }
        else
        {
            fAverageTemperature = fTemperature;
        }
    }

    g_fSensorTmprCurrentValue = fAverageTemperature;
    ubRet = SENSOR_AUX_OK;

done:
    *pfTemperature = g_fSensorTmprCurrentValue;
    return ubRet;
}

/*************************************************************************
* FUNCTION:
*   Sensor_Auxadc_Resistor_Calculation
*
* DESCRIPTION:
*   Calculate the resistor of thermistor
*
* PARAMETERS
*   1. fResistorOfThermistor            : [Out] The value of resistor of thermistor
*
* RETURNS
*   1. SENSOR_AUX_OK   : success
*   2. SENSOR_AUX_FAIL : fail
*
*************************************************************************/
uint8_t Sensor_Auxadc_Resistor_Calculation(float *fResistorOfThermistor)
{
    uint8_t ubRet = SENSOR_AUX_FAIL;
    uint32_t ulDcValue = 0;
    uint32_t ulAdcConditionValue1 = 0;
    uint32_t ulAdcConditionValue2 = 0;
    float    fValue1 = 0;
    float    fValue2 = 0;

    if (SENSOR_AUX_OK == Sensor_Auxadc_Get_Value(&ulDcValue, &ulAdcConditionValue1, &ulAdcConditionValue2))
    {    
        fValue1 = (ulAdcConditionValue1 - ulDcValue) * (30 * 2);
        fValue2 = (ulAdcConditionValue2 - ulDcValue);
        
        *fResistorOfThermistor = ((fValue1/fValue2) - 30);     

        ubRet = SENSOR_AUX_OK;
    }
    
    return ubRet;

}

/*************************************************************************
* FUNCTION:
*   Sensor_Auxadc_Get_Value
*
* DESCRIPTION:
*   Get the value of Sensor AUXADC
*
* PARAMETERS
*   1. ulDcValue            : [Out] The value of DC
*   2. ulAdcConditionValue1 : [Out] The value of condition 1
*   3. ulAdcConditionValue2 : [Out] The value of condition 2
*
* RETURNS
*   1. SENSOR_AUX_OK   : success
*   2. SENSOR_AUX_FAIL : fail

*
*************************************************************************/
uint8_t Sensor_Auxadc_Get_Value(uint32_t *ulDcValue, uint32_t *ulAdcConditionValue1, uint32_t *ulAdcConditionValue2)
{
    uint8_t ubRet = SENSOR_AUX_FAIL;

    /* Get DC value, note IO2/3/4 are fixed in code that corresponding to certain board */
    Hal_Pin_ConfigSet(2, PIN_TYPE_AUX_2, PIN_DRIVING_FLOAT);
    Hal_Pin_ConfigSet(3, PIN_TYPE_NONE, PIN_DRIVING_FLOAT);
    Hal_Pin_ConfigSet(4, PIN_TYPE_GPIO_OUTPUT_LOW, PIN_DRIVING_FLOAT);
    if (SENSOR_AUX_OK != Hal_Aux_SourceSelect(HAL_AUX_SRC_GPIO, 2))
        goto done;

    if (SENSOR_AUX_OK != Hal_Aux_AdcValueGet(ulDcValue))
        goto done;

    /*  Condition 2  
        VBAT GPIO 4 set to HIGH
        Thermistor connect to GPIO 3, set GPIO 3 no use
    */
    Hal_Pin_ConfigSet(2, PIN_TYPE_AUX_2,PIN_DRIVING_FLOAT);
    Hal_Pin_ConfigSet(3, PIN_TYPE_NONE,PIN_DRIVING_FLOAT);
    Hal_Pin_ConfigSet(4, PIN_TYPE_GPIO_OUTPUT_HIGH,PIN_DRIVING_FLOAT);

                
    if (SENSOR_AUX_OK != Hal_Aux_SourceSelect(HAL_AUX_SRC_GPIO, 2))
        goto done;

    if (SENSOR_AUX_OK != Hal_Aux_AdcValueGet(ulAdcConditionValue1))
        goto done;

    /*  Condition 3  
     VBAT GPIO 4 set to no use
     Thermistor connect to GPIO 3, GPIO 3 set to low
    */
    Hal_Pin_ConfigSet(2, PIN_TYPE_AUX_2, PIN_DRIVING_FLOAT);
    Hal_Pin_ConfigSet(3, PIN_TYPE_GPIO_OUTPUT_HIGH, PIN_DRIVING_FLOAT);
    Hal_Pin_ConfigSet(4, PIN_TYPE_NONE, PIN_DRIVING_FLOAT);
    if (SENSOR_AUX_OK != Hal_Aux_SourceSelect(HAL_AUX_SRC_GPIO, 2))
     goto done;

    if (SENSOR_AUX_OK != Hal_Aux_AdcValueGet(ulAdcConditionValue2))
     goto done;

    ubRet = SENSOR_AUX_OK;

    done:
        return ubRet;
}

/*************************************************************************
* FUNCTION:
*   Sensor_Auxadc_Tmpr_Calibration
*
* DESCRIPTION:
*   do the calibration of the temperature
*
* PARAMETERS
*   1. fTmpr : [In] the temperature
*
* RETURNS
*   1. SENSOR_AUX_OK   : success
*   2. SENSOR_AUX_FAIL : fail
*
*************************************************************************/
uint8_t Sensor_Auxadc_Tmpr_Calibration(float fTmpr)
{
    float fTemperature;
    uint8_t ubRet = SENSOR_AUX_FAIL;

    /* Calculate the Temperature */
    Sensor_Auxadc_Tmpr_Get(&fTemperature);

    // offset = real - compute
    // offset = new base - old base
    // new base = old base + (real - compute)
    g_tAPPTmpr_CalData.fBaseTemperature = g_tAPPTmpr_CalData.fBaseTemperature + (fTmpr - fTemperature);

    if (MW_FIM_OK != MwFim_FileWrite(MW_FIM_IDX_GP03_CAL_TEMPERATURE, 0, MW_FIM_CAL_TEMPERATURE_SIZE, (uint8_t*)&g_tAPPTmpr_CalData))
        goto done;

    ubRet = SENSOR_AUX_OK;

done:
    return ubRet;
}

/*************************************************************************
* FUNCTION:
*   Sensor_Auxadc_Init
*
* DESCRIPTION:
*   Sensor Auxadc init
*
* PARAMETERS
*   none
*
* RETURNS
*   none
*
*************************************************************************/
void Sensor_Auxadc_Init(void)
{
    // cold boot
    if (0 == Boot_CheckWarmBoot())
    {
        if (MW_FIM_OK != MwFim_FileRead(MW_FIM_IDX_GP03_CAL_TEMPERATURE, 0, MW_FIM_CAL_TEMPERATURE_SIZE, (uint8_t*)&g_tAPPTmpr_CalData))
        {
            // if fail, get the default value
            memcpy(&g_tAPPTmpr_CalData, &g_tMwFimDefaultCalTmpr, MW_FIM_CAL_TEMPERATURE_SIZE);
        }

        g_ulSensorTmprMovingAverageCount = 0;
        g_ulSensorVbatMovingAverageCount = 0;
        g_fSensorTmprCurrentValue = 0.0;
        g_fSensorVbatCurrentValue = MAXIMUM_VOLTAGE_DEF;    // give the max voltage
    }
}

/*************************************************************************
* FUNCTION:
*   Sensor_Auxadc_VBat_Convert_to_Percentage
*
* DESCRIPTION:
*   Convert Battery value to percentage
*
* PARAMETERS
*   N/A
*
* RETURNS
*   The percentage of VBat
*
*************************************************************************/
float Sensor_Auxadc_VBat_Convert_to_Percentage(void)
{
    float fVBat;
    float fAverageVBat;

    Hal_Aux_VbatGet(&fVBat);
    if (fVBat > MAXIMUM_VOLTAGE_DEF)
        fVBat = MAXIMUM_VOLTAGE_DEF;
    if (fVBat < MINIMUM_VOLTAGE_DEF)
        fVBat = MINIMUM_VOLTAGE_DEF;

    // error handle: if the average count = 0
    if (SENSOR_MOVING_AVERAGE_COUNT == 0)
    {
        fAverageVBat = fVBat;
    }
    // compute the moving average value
    else
    {
        // update the new average count
        if (g_ulSensorVbatMovingAverageCount < SENSOR_MOVING_AVERAGE_COUNT)
            g_ulSensorVbatMovingAverageCount++;
            
        // compute the new moving average value
        fAverageVBat = g_fSensorVbatCurrentValue * (g_ulSensorVbatMovingAverageCount - 1);
        fAverageVBat = (fAverageVBat + fVBat) / g_ulSensorVbatMovingAverageCount;
    }

    // the value is updated when the new value is the lower one.
    if (fAverageVBat < g_fSensorVbatCurrentValue)
        g_fSensorVbatCurrentValue = fAverageVBat;

    return ((g_fSensorVbatCurrentValue - MINIMUM_VOLTAGE_DEF)/(MAXIMUM_VOLTAGE_DEF - MINIMUM_VOLTAGE_DEF))*100;
}


