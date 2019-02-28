## OPL1000-Reference
- OPL1000-Reference is Opulinks official IOT reference design for OPL1000 chip. It contains from embedded device to mobile application complete integrated design. 
- OPL1000-Reference package provides two kind of solutions. All of them connect to Ali cloud. 
  - IOT sensor type application, single chip solution, application runs on OPL1000 M3 MCU. Here temperature sensor is adopted, internal PMU power supply scheme is used.  
  - IOT light application, single chip solution, application runs on OPL1000 M3 MCU. Here external LDO power supply scheme is used.    
- OPL1000-Reference uses BLE config WIFI method to connect Internet (cloud). 
- Three tools are provides in this package, they are: 
  1.  General parameter configure tool, which is used to modify work mode parameters and complete compile, download operation along with. 
  2. BLE configure WIFI AP tool, which runs on Android mobile device.
  3. Firmware download tool, which is used to download firmware into OPL1000 devices.  
- For each reference application guide document, dedicate mobile app and project source code are provided.  

## Use "General Parameter Configure" tool 

"General Parameter Configure" tool ParamCfg.exe is placed under **Tool\ParamCfg** folder. 

Please refer "OPL1000-Parameter-Configure-Tool-guide.pdf" to understand how to use this tool.

## Use "firmware download" tool 

Download tool download_RELEASE.exe is placed under **Tool\Download** folder. 

Please refer "OPL1000-patch-download-tool-user-guide.mht" to understand how  to use this tool. This mht document can also be loaded by click tool "About->Use Manual" button. 

## Use "BLE config AP" tool 

"BLE config AP" tool is an Android APP program, user can use it to configure wifi AP connection . It is placed under **Tool\BLE_Config_AP** folder. 

Please refer "OPL1000-Demo-BLE-setup-network-guide.pdf" to understand how  use this tool.

## Develop application based on OPL1000-Reference

User can follows below steps to develop own application based on OPL1000-Reference.

1. User evaluates which reference is similar to own application. And then based on this reference frame to develop own hardware and software.
2. Read application guide document of selected reference to know this solution functions/features and application scenario. 
3. Evaluate reference function to understand its realization principle. 
4. Develop own application based on this reference, download it into OPL1000 device and test its function.  

## Evaluate OPL1000 reference function 
Generally follows below 3 steps to evaluate OPL1000 reference functions :

1. Run ParamCfg.exe  tool to define work  parameter according to requirement, firmware will be compiled and download into device automatically; 
2. Install and run "BLE config AP" app program to complete WIFI AP connection;
3. Run reference dedicate cloud app to evaluate reference function.

# Development tool 
Please use Keil uVision 5 on Windows system to develop application
