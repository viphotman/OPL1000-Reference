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

#include "sntp_time.h"
#include "scaler_sys.h"

/********************************************
Declaration of Global Variables & Functions
********************************************/
static sntp_header_t sntp_h;
static struct sockaddr_in server;
char c_tm[TIME_STAMP_STR_LEN];

/***********
C Functions
***********/

int sntp_date_get(unsigned int *pSntpTime)
{
	struct addrinfo hints = {
					.ai_family = AF_INET,
					.ai_socktype = SOCK_DGRAM,
					};

	struct addrinfo *res = NULL;
	struct in_addr *addr = NULL;
  uint32_t start_tick, curr_tick;
					
	start_tick = sys_get_tick();
	do {
		  curr_tick = sys_get_tick(); 
			/* get sntp server ip */
			if (res == NULL) {
				int err = getaddrinfo(SNTP_SERVER, "123", &hints, &res);
				if (err != 0 || res == NULL) 
				{
						osDelay(1000);
						continue;
					}

					if (err != 0 || res == NULL) {
					osDelay(1000);
					continue;
					}
					addr = &((struct sockaddr_in *)res->ai_addr)->sin_addr;
					printf("... DNS lookup succeeded. IP=%s \r\n", inet_ntoa(*addr));
					osDelay(1000);
		  }
			
			server.sin_family = AF_INET;
			server.sin_port = htons(123);
			server.sin_addr.s_addr = addr->s_addr;
			
			int sockfd = socket(AF_INET,SOCK_DGRAM,0);
			if (sockfd < 0) {
					printf("create socket failed.\n");
					continue;
			}
			printf ("... socket.\n");
			
			/* init sntp header */
			sntp_h.li_vn_mode = 0x1b; /* li = 0, no warning, vn = 3, ntp version, mode = 3, client */

		 if (sendto(sockfd, &sntp_h, sizeof(sntp_h), 0, (struct sockaddr *)&server, sizeof(server)) < 0) {
				 printf("sendto failed.\n");
				 continue;
		 } 
		 else {
				 printf("... sendto.\n");

				 struct timeval receiving_timeout;
				 receiving_timeout.tv_sec = 5;
				 receiving_timeout.tv_usec = 0;
				 if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &receiving_timeout,
						 sizeof(receiving_timeout)) < 0) {
						 printf("... failed to set socket receiving timeout \r\n");
						 close(sockfd);
						 osDelay(3000);
						 continue;
				 }
				 //printf("... setsockopt.\n");

				 socklen_t addrlen = sizeof(server);
				 if (recvfrom(sockfd, &sntp_h, sizeof(sntp_h), 0, (struct sockaddr *)&server, &addrlen) < 0) {
						 printf("... recvfrom failed.\n");
						 osDelay(SNTP_SERVR_TRY_CONNECT_INTERVAL);
				 } 
				 else {
						 //printf("... recvfrom.\n");

						 time_t tt = CONVERT_TIME(ntohl(sntp_h.trantimeint));
						 struct tm *t = localtime(&tt);
						 printf("... Current time (BeiJing): %d, %d, %d, %d, %d, %d, \n", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
						 //memset(c_tm, 0, sizeof(c_tm));
							
						 sprintf(c_tm,"%d/%d/%d   %d:%d:%d", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
		
					   *pSntpTime = (unsigned int)tt;
					   close(sockfd);
						 return true;
				 }
		 }
		 osDelay(SNTP_SERVR_TRY_CONNECT_INTERVAL);
	}while ((curr_tick - start_tick) < SNTP_CONNECT_TIME_OUT_MS);
	
  if((curr_tick - start_tick) >= SNTP_CONNECT_TIME_OUT_MS)
	{
		printf(" Time out, current tick = %d, begin tick = %d  \n",curr_tick,start_tick);
		return false;
	}
	else
		return true;	
}
