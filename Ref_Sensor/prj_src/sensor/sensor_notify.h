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
 * @file sensor_notify.h
 * @author Michael Liao
 * @date 15 Mar 2018
 * @brief File containing declaration of sensor application & definition of structure for reference.
 *
 */
#ifndef __SENSOR_NOTIFY_H__
#define __SENSOR_NOTIFY_H__

void notify_send_msg_to_sd_task(uint32_t action, unsigned char * pMsg);

#endif

