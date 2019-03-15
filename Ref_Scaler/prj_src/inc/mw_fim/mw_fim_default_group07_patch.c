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

/***********************
Head Block of The File
***********************/
// Sec 0: Comment block of the file


// Sec 1: Include File
#include "mw_fim_default_group07.h"
#include "mw_fim_default_group07_patch.h"


// Sec 2: Constant Definitions, Imported Symbols, miscellaneous


/********************************************
Declaration of data structure
********************************************/
// Sec 3: structure, uniou, enum, linked list


/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global variable

extern uint32_t gLeHostNvmStore0[MW_FIM_LE_STORE0_NUM];
extern uint32_t gLeHostNvmStore1[MW_FIM_LE_STORE1_NUM];
extern uint32_t gLeHostNvmStore2[MW_FIM_LE_STORE2_NUM];
extern uint32_t gLeHostNvmStore3[MW_FIM_LE_STORE3_NUM];

// the information table of group 07
const T_MwFimFileInfo g_taMwFimGroupTable07_patch[] =
{
    {MW_FIM_IDX_LE_STORE0, MW_FIM_LE_STORE0_NUM, MW_FIM_LE_STORE0_SIZE, NULL, gLeHostNvmStore0},
    {MW_FIM_IDX_LE_STORE1, MW_FIM_LE_STORE1_NUM, MW_FIM_LE_STORE1_SIZE, NULL, gLeHostNvmStore1},
    {MW_FIM_IDX_LE_STORE2, MW_FIM_LE_STORE2_NUM, MW_FIM_LE_STORE2_SIZE, NULL, gLeHostNvmStore2},
    {MW_FIM_IDX_LE_STORE3, MW_FIM_LE_STORE3_NUM, MW_FIM_LE_STORE3_SIZE, NULL, gLeHostNvmStore3},
    
    // the end, don't modify and remove it
    {0xFFFFFFFF,            0x00,              0x00,               NULL,                            NULL}
};


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
