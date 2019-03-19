## Release Version:
Reference Package: v1.1.4.2
Library : 2995
ROM_CODE: 1655 

## Release Date: 2019/3/19

## Function & Feature Update 
1. Add Demo video youtube access link in readme file. 
2. Arduino firmware is updated to v1.1 version 

## Bug Fix List 
1. Fix a bug in Arduino firmware in weighting operation.

## Notes List 
1. Not support user profile define function in APP side yet. 

****************

## Release Version:
Reference Package: v1.1.4.1
Library : 2995
ROM_CODE: 1655 

## Release Date: 2019/3/15

## Function & Feature Update 
1. Documents updating:
    1.1 OPL1000-Reference-scaler-application-guide, R01-v01, Application notes for scaler reference.
    1.2 OPL1000-Reference-scaler-hardware-circuit.pdf, weight sensor and LCD circuit diagram.  
2. Mobile APP updating:
   2.1 Opl_scaler.apk: Android APK program, v1.0. Support weighting and user profile display function;    
3. Support Ali cloud MQTT v3.1 protocol.
4. Two chips solution, external MCU is master device, OPL1000 act as slave device,  complete data transport function. 
5. External MCU function: Get weight value and display it on LCD, pass data to OPL1000 via AT command. 
6. OPL1000 functions: complete BLE configure wifi function, pass weight and user info to cloud. 
7. Support  BLE configure WIFI AP function, shall use common tool "opulinks_iot_app.apk" that located under ..\Tool\BLE_Config_AP directory.
8. Support smart sleep and auto-connection.
9. Support time stamp and battery info publish function. 

## Bug Fix List 

## Notes List 
1. Not support user profile define function in APP side yet. 


