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
 *  file         : ftoa.c
 *  description  : convert double to string
 *
 */

#ifndef __FTOA_H__
#define __FTOA_H__

#ifdef __cplusplus
extern "C" {
#endif

	
#define EXAMPLE_TRACE(fmt, ...)  \
    do { \
        HAL_Printf("%s|%03d :: ", __func__, __LINE__); \
        HAL_Printf(fmt, ##__VA_ARGS__); \
        HAL_Printf("%s", "\r\n"); \
    } while(0)

		
char * ftoa(double f, char * buf, int precision);
int util_ip_conv_str(unsigned char *pHexIpAddr, char* pIpStr);
		
#ifdef __cplusplus
}
#endif

#endif // __FTOA_H__
