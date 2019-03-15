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
 * @file scaler_sys.h
 * @author SH-SDK
 * @date Mar.05,2019 
 * @brief File containing declaration of scaler reference application & definition of structure for reference.
 *
 */
#ifndef __SCALER_SYS_H__
#define __SCALER_SYS_H__

/******************************************************
*               Global Variables
******************************************************/
 
/******************************************************
 *               Function Declarations
 ******************************************************/
void sys_osdelay(unsigned int ms);

unsigned int sys_get_tick(void);
int sys_get_mac(unsigned char *mac);
int sys_get_timeStamp(char* cTimeStampBuffer);

#endif

