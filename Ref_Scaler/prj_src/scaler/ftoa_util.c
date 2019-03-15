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
 *  file         : ftoa.c
 *  description  : convert double to string
 *
 */
/***********************
Head Block of The File
***********************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_PRECISION   (10)
#if 0
static const double rounders[MAX_PRECISION + 1] =
{
    0.5,                // 0
    0.05,               // 1
    0.005,              // 2
    0.0005,             // 3
    0.00005,            // 4
    0.000005,           // 5
    0.0000005,          // 6
    0.00000005,         // 7
    0.000000005,        // 8
    0.0000000005,       // 9
    0.00000000005       // 10
};
#endif

/***********
C Functions
***********/

char * ftoa(double f, char * buf, int precision)
{
    char * ptr = buf;
    char * p = ptr;
    char * p1;
    char c;
    long intPart;

    // check precision bounds
    if (precision > MAX_PRECISION)
        precision = MAX_PRECISION;

    // sign stuff
    if (f < 0)
    {
        f = -f;
        *ptr++ = '-';
    }

    if (precision < 0)  // negative precision == automatic precision guess
    {
        if (f < 1.0) precision = 6;
        else if (f < 10.0) precision = 5;
        else if (f < 100.0) precision = 4;
        else if (f < 1000.0) precision = 3;
        else if (f < 10000.0) precision = 2;
        else if (f < 100000.0) precision = 1;
        else precision = 0;
    }

    // round value according the precision
    //if (precision)
    //    f += rounders[precision];

    // integer part...
    intPart = f;
    f -= intPart;

    if (!intPart)
        *ptr++ = '0';
    else
    {
        // save start pointer
        p = ptr;

        // convert (reverse order)
        while (intPart)
        {
            *p++ = '0' + intPart % 10;
            intPart /= 10;
        }

        // save end pos
        p1 = p;

        // reverse result
        while (p > ptr)
        {
            c = *--p;
            *p = *ptr;
            *ptr++ = c;
        }

        // restore end pos
        ptr = p1;
    }

    // decimal part
    if (precision)
    {
        // place decimal point
        *ptr++ = '.';

        // convert
        while (precision--)
        {
            f *= 10.0;
            c = f;
            *ptr++ = '0' + c;
            f -= c;
        }
    }

    // terminating zero
    *ptr = 0;

    return buf;
}

int util_conv_hex(unsigned char hexByte, char* hexStr)
{
		int k=0;
		char oneChar;	
		oneChar = hexByte/100 + '0';
		if(oneChar > '0' & oneChar <= '9'){		
			hexStr[k++]= oneChar;
			// calculate second char 
			oneChar = (hexByte%100)/10 + '0';
			hexStr[k++]= oneChar;	
			// calculate third char
			oneChar = hexByte %10 + '0';
			hexStr[k++]= oneChar;
		}
		else{
			oneChar = hexByte/10 + '0';
			if(oneChar > '0' & oneChar <= '9'){		
				hexStr[k++]= oneChar;
				// calculate second char
				oneChar = hexByte %10 + '0';
				hexStr[k++]= oneChar;
			}
			else  
				oneChar = hexByte %10 + '0';
				hexStr[k++]= oneChar;				
		}
		return k;	
}

int util_ip_conv_str(unsigned char *pHexIpAddr, char* pIpStr)
{
	int i,n,k=0;
	char charArray[3] = {0};
	for (i=0;i<3;i++)
	{
		memset(charArray,0,3);
		n = util_conv_hex(pHexIpAddr[i],&pIpStr[k]);
		k = k + n;
    pIpStr[k++] = '.';
	}
	memset(charArray,0,3);
	n = util_conv_hex(pHexIpAddr[3],&pIpStr[k]);
	return (k+n);
}
