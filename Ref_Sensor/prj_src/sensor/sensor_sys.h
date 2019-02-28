/******************************************************************************
*  Copyright 2018, Netlink Communication Corp.
*  ---------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Netlnik Communication Corp. (C) 2018
******************************************************************************/
/**
 * @file sensor_sys.h
 * @author Michael Liao
 * @date 15 Mar 2018
 * @brief File containing declaration of sensor system application & definition of structure for reference.
 *
 */
#ifndef __SENSOR_SYS_H__
#define __SENSOR_SYS_H__

void sys_osdelay(unsigned int ms);

int sys_get_time(uint32_t *tick);
int sys_get_mac(uint8_t *mac);

#endif

