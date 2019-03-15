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
 * @date Mar.06,2019 
 * @brief File containing scaler reference profile definition.
 *
 */
#ifndef __SCALER_PROFILE_H__
#define __SCALER_PROFILE_H__

/******************************************************
 *                    Macros
 ******************************************************/
#define SCALER_DATA_OK      1
#define SCALER_DATA_FAIL    0

#define SCALER_MAC_LEN 					(6)
#define IOT_TRIPLE_STR_LEN 			(64)   // Define IOT TRIPLE string length 
#define SCLAER_USER_NAME_LEN    (16)
#define SCLAER_TIME_STAMP_LEN   (20)

#define SCALER_USER_NAME 				"Name"
#define SCALER_USER_DEF         "OPL_User"   // default user name  

#define SCALER_WEIGHT_NAME 			"Weight"
#define SCALER_WEIGHT_DEF 			(2.35)  		 // unit kg, default value  

#define SCALER_BATTERY_NAME 		"battery"
#define SCALER_BATTERY_DEF 			(50)

#define SCALER_TIME_NAME 				"time"
#define SCALER_TIME_DEF 				"2019/03/05 14:20:45"

#define SCALER_GENDER_MALE      0
#define SCALER_GENDER_FEMALE    1

#define SCALER_WIFI_MAC_NAME    "mac"
#define SCALER_WIFI_MAC_DEF     "22:33:44:55:66:76" 

#define SCALER_BLE_MAC_DEF      "11:22:33:44:55:76"
#define SCALER_DEVICE_NAME      "device"

#define SCALER_HEARTBEAT_INTERVAL_DEF    300  // in second, 5 minutes 

#define SCALER_MAX_USER_NUM     3   // now support maximum 3 users to register one device
#define MAX_PROFILE_TOPIC_LEN   8   // maximum 8 characters 

#define PRODUCT_KEY             "a17kCRksX7t"
#define DEVICE_NAME             "Opl1000_scaler01"
#define DEVICE_SECRET           "fbFOfUeBxTKkZKv2tV022ChFB1KcipiQ"

/******************************************************
 *                 Type Definitions
 ******************************************************/
 

/******************************************************
 *                   Enumerations
 ******************************************************/
// the state of scaler operation
typedef enum
{
    SCALER_DATA_STATE_IDLE = 0,
    SCALER_DATA_STATE_NETWORK_ON,
    SCALER_DATA_STATE_DEVICE_ON,
    SCALER_DATA_STATE_MAX
} E_ScalerDataState;

/******************************************************
 *                    Structures
 ******************************************************/ 
/**
 * This data structure defines scaler HW related properities definition.
 */
typedef struct scaler_hw_attr_tag {
		char triple_prdtKey[IOT_TRIPLE_STR_LEN];		/* IOT triple item: PRODUCT_KEY */
		char triple_devName[IOT_TRIPLE_STR_LEN];		/* IOT triple item: DEVICE_NAME */	
		char triple_devScrt[IOT_TRIPLE_STR_LEN];		/* IOT triple item: DEVICE_SECRET */		
    unsigned char mac[SCALER_MAC_LEN];          /* Device ID (Mac Addr) */
}scaler_hw_attr_t;

/**
 * This data structure defines scaler report data, include weight,time,user,mac,bettery etc.
 */
typedef struct scaler_rpt_tag {
	  float weight; 												 /* weight information, in kg unit*/
    char user[SCLAER_USER_NAME_LEN];			 /* user name, shall post to cloud*/
	  char time[SCLAER_TIME_STAMP_LEN];      /* time stamp, format: 2019/03/05 14:20:45*/   
    int battery;                           /* Remain Battery (%), for example 65 */
}scaler_rpt_t;

/**
 * This data structure defines setting related parameters, include user name, heartbeat interval etc.
   these parameters are published from mobile APP side 
 */
typedef struct scaler_setting_tag {
    char user[SCLAER_USER_NAME_LEN];			 /* user name, shall post to cloud*/	
    int heartbeat;                         /* define heart beat monitor time interval */
}scaler_setting_t;

/**
 * This data structure defines user information when implement weighting operation 
*/
typedef struct scaler_user_tag {
	  char user_name[SCLAER_USER_NAME_LEN];  /* user name, support english firstly*/
	  unsigned char sex;   							 		/* user gender,  male = 0, or female = 1  */
    unsigned char height;   							 /* user height, in cm unit*/
	  unsigned char age;      							 /* user age group */   
}scaler_user_t;

/**
 * This data defines how many users are registered to this device 
*/
typedef struct scaler_register_tag {
	  scaler_user_t reg_user[SCALER_MAX_USER_NUM];  /* user name, support english firstly*/
	  int user_num;                                 /* user gender*/  
}scaler_register_t;

typedef struct 
{
    char Topic_of_User_Name[MAX_PROFILE_TOPIC_LEN];      // function defined in cloud server ex: Name
    char user_name[SCLAER_USER_NAME_LEN];  // define user's name 
    char Topic_of_Gender[MAX_PROFILE_TOPIC_LEN];  // function defined in cloud server ex: Gender
    unsigned char gender;                        // male = 0, or female = 1 
    char Topic_of_Height[MAX_PROFILE_TOPIC_LEN];  // function defined in cloud server ex: Height
	  unsigned char height;  // 0 ~ 300 cm 
    char Topic_of_Age[MAX_PROFILE_TOPIC_LEN];  // function defined in cloud server ex: Age
	  unsigned char age;  // refer unsigned char
} T_Ref_Scaler_User_Profile;


 /******************************************************
 *               Global Variables
 ******************************************************/
extern scaler_hw_attr_t g_ScalerHwAttr;
extern scaler_rpt_t g_ScalerRptData;
extern scaler_setting_t g_ScalerSet;
extern scaler_register_t g_ScalerUser;
extern T_Ref_Scaler_User_Profile Scaler_User_Profile;

/******************************************************
 *               Function Declarations
 ******************************************************/
/**
 * @brief This function initialize scaler profile related global structure.
 * @param[in] none.
 *
 * @return none.
 */
void scaler_data_init(void); 

/**
 * @brief This function returns pointer of g_ScalerRptData.
 * @param[in] none.
 *
 * @return none.
 */
scaler_rpt_t* data_get_entity(void);

/**
 * @brief This function returns pointer of g_ScalerHwAttr.
 * @param[in] none.
 *
 * @return none.
 */
scaler_hw_attr_t* attr_get_entity(void);

/**
 * @brief This function returns pointer of g_ScalerSet.
 * @param[in] none.
 *
 * @return none.
 */
scaler_setting_t* setting_get_entity(void);

/**
 * @brief This function get wifi mac address.
 * @param[in] none.
 *
 * @return true or false.
 */
int scaler_get_mac(void);

/**
 * @brief This function convert scaler battery percent information to 
 *        string 
 * @param[in] vbat_percent.
 *
 * @return char* vbat_percent_str: battery remainder percent string 
 *         int *vbat_enum: [1~4) mapping to (0~10%,10%~50%,50%~90%,90%~100%)
 */
void scaler_battery_conv(int vbat_percent, char* vbat_percent_str, int *vbat_enum);

/**
 * @brief This function get user register information from MQTT message and fill into 
 *        global parameter 
 * @param[in] mqtt_user_msg.
 *
 * @return pScalerUser: pointer to global parameter that stores registered user info 
 *
 */
int scaler_update_user(char* userName, unsigned char gender, unsigned char height, unsigned char age, scaler_register_t* pScalerUser); 

#endif

