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

/******************************************************************************
*  Filename:
*  ---------
*  main_patch.c
*
*  Project:
*  --------
*  OPL1000 Project - the main patch implement file
*
*  Description:
*  ------------
*  This implement file is include the main patch function and api.
*
*  Author:
*  -------
*  Jeff Kuo
*
******************************************************************************/
/***********************
Head Block of The File
***********************/
// Sec 0: Comment block of the file


// Sec 1: Include File
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "blewifi_app.h"
#include "hal_pin.h"
#include "hal_pin_def.h"
#include "hal_pin_config_project.h"
#include "hal_system.h"
#include "mw_fim.h"
#include "sys_init.h"
#include "sys_init_patch.h"


// Sec 2: Constant Definitions, Imported Symbols, miscellaneous
// the number of elements in the message queue


/********************************************
Declaration of data structure
********************************************/
// Sec 3: structure, uniou, enum, linked list


/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global variable
extern uint8_t* g_ucaMemPartAddr;
extern uint32_t g_ulMemPartTotalSize;

// Sec 5: declaration of global function prototype
static void Main_PinMuxUpdate(void);


/***************************************************
Declaration of static Global Variables & Functions
***************************************************/
// Sec 6: declaration of static global variable


// Sec 7: declaration of static function prototype
static void __Patch_EntryPoint(void) __attribute__((section(".ARM.__at_0x00420000")));
static void __Patch_EntryPoint(void) __attribute__((used));
static void Main_FlashLayoutUpdate(void);
static void Main_AppInit_patch(void);


/***********
C Functions
***********/
// Sec 8: C Functions

/*************************************************************************
* FUNCTION:
*   __Patch_EntryPoint
*
* DESCRIPTION:
*   the entry point of SW patch
*
* PARAMETERS
*   none
*
* RETURNS
*   none
*
*************************************************************************/
static void __Patch_EntryPoint(void)
{
    // don't remove this code
    SysInit_EntryPoint();

    // update the pin mux
    Hal_SysPinMuxAppInit = Main_PinMuxUpdate;
    
    // update the flash layout
    MwFim_FlashLayoutUpdate = Main_FlashLayoutUpdate;
    
    // modify the heap size, from 0x43C000 to 0x44F000
    g_ucaMemPartAddr = (uint8_t*) 0x43F000;
    g_ulMemPartTotalSize = 0x10000; //0x44F000 - 0x43F000

    
    // application init
    Sys_AppInit = Main_AppInit_patch;
}

/*************************************************************************
* FUNCTION:
*   Main_PinMuxUpdate
*
* DESCRIPTION:
*   update the pinmux setting
*
* PARAMETERS
*   none
*
* RETURNS
*   none
*
*************************************************************************/
static void Main_PinMuxUpdate(void)
{
    Hal_Pin_ConfigSet(0, HAL_PIN_TYPE_IO_0, HAL_PIN_DRIVING_IO_0);
    Hal_Pin_ConfigSet(1, HAL_PIN_TYPE_IO_1, HAL_PIN_DRIVING_IO_1);
    Hal_Pin_ConfigSet(2, HAL_PIN_TYPE_IO_2, HAL_PIN_DRIVING_IO_2);
    Hal_Pin_ConfigSet(3, HAL_PIN_TYPE_IO_3, HAL_PIN_DRIVING_IO_3);
    Hal_Pin_ConfigSet(4, HAL_PIN_TYPE_IO_4, HAL_PIN_DRIVING_IO_4);
    Hal_Pin_ConfigSet(5, HAL_PIN_TYPE_IO_5, HAL_PIN_DRIVING_IO_5);
    Hal_Pin_ConfigSet(6, HAL_PIN_TYPE_IO_6, HAL_PIN_DRIVING_IO_6);
    Hal_Pin_ConfigSet(7, HAL_PIN_TYPE_IO_7, HAL_PIN_DRIVING_IO_7);
    Hal_Pin_ConfigSet(8, HAL_PIN_TYPE_IO_8, HAL_PIN_DRIVING_IO_8);
    Hal_Pin_ConfigSet(9, HAL_PIN_TYPE_IO_9, HAL_PIN_DRIVING_IO_9);
    Hal_Pin_ConfigSet(10, HAL_PIN_TYPE_IO_10, HAL_PIN_DRIVING_IO_10);
    Hal_Pin_ConfigSet(11, HAL_PIN_TYPE_IO_11, HAL_PIN_DRIVING_IO_11);
    Hal_Pin_ConfigSet(12, HAL_PIN_TYPE_IO_12, HAL_PIN_DRIVING_IO_12);
    Hal_Pin_ConfigSet(13, HAL_PIN_TYPE_IO_13, HAL_PIN_DRIVING_IO_13);
    Hal_Pin_ConfigSet(14, HAL_PIN_TYPE_IO_14, HAL_PIN_DRIVING_IO_14);
    Hal_Pin_ConfigSet(15, HAL_PIN_TYPE_IO_15, HAL_PIN_DRIVING_IO_15);
    Hal_Pin_ConfigSet(16, HAL_PIN_TYPE_IO_16, HAL_PIN_DRIVING_IO_16);
    Hal_Pin_ConfigSet(17, HAL_PIN_TYPE_IO_17, HAL_PIN_DRIVING_IO_17);
    Hal_Pin_ConfigSet(18, HAL_PIN_TYPE_IO_18, HAL_PIN_DRIVING_IO_18);
    Hal_Pin_ConfigSet(19, HAL_PIN_TYPE_IO_19, HAL_PIN_DRIVING_IO_19);
    Hal_Pin_ConfigSet(20, HAL_PIN_TYPE_IO_20, HAL_PIN_DRIVING_IO_20);
    Hal_Pin_ConfigSet(21, HAL_PIN_TYPE_IO_21, HAL_PIN_DRIVING_IO_21);
    Hal_Pin_ConfigSet(22, HAL_PIN_TYPE_IO_22, HAL_PIN_DRIVING_IO_22);
    Hal_Pin_ConfigSet(23, HAL_PIN_TYPE_IO_23, HAL_PIN_DRIVING_IO_23);
}

/*************************************************************************
* FUNCTION:
*   Main_FlashLayoutUpdate
*
* DESCRIPTION:
*   update the flash layout
*
* PARAMETERS
*   none
*
* RETURNS
*   none
*
*************************************************************************/
static void Main_FlashLayoutUpdate(void)
{
    // update here
}

/*************************************************************************
* FUNCTION:
*   Main_AppInit_patch
*
* DESCRIPTION:
*   the initial of application
*
* PARAMETERS
*   none
*
* RETURNS
*   none
*
*************************************************************************/
static void Main_AppInit_patch(void)
{
    BleWifiAppInit();
}
