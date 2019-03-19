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

/*
 *  Revision History:
 *  1. SH SDK, Mar.14, v1.0 first version 
 *  2. SH SDK, Mar.18, v1.1 refine LCD display by displaying initializing
 *             after done with former loop. 
 *  
*/
#include <LiquidCrystal.h>

#define LED               13
#define OPL_INTERRUPT_PIN 2
#define SERIAL_BAUD_RATE      115200
#define COUNT               3
#define UART_BUFFER_LEN     256

// Define values for sleep mode.
// If there is no weight data available within (SLEEP_INTERVAL * SLEEP_COUNT),
// It would trige OPL1000 into deep sleep mode. take Second as unit.
#define SLEEP_INTERVAL       5
#define SLEEP_COUNT          12

// Define HX711 pin and value.
//SCK pin is used to provide clock for HX711 module communication
//DT pin is used to read AD data from HX711
//GapValue is used to calibrate output weight value, increase this value if output is too high, otherwise reduce its value. 
#define HX711_SCK_PIN       8
#define HX711_DATA_PIN      7
#define HX711_GAP_VALUE     40

// Define LCD setup pins.
#define LCD_RS_PIN     12
#define LCD_EN_PIN     9
#define LCD_D4_PIN     6
#define LCD_D5_PIN     5
#define LCD_D6_PIN     4
#define LCD_D7_PIN     3

// Define AT command.
//#define AT_CMD_INQUERY_AP     "AT+INQAP"
//#define AT_CMD_SEND_DATA      "AT+DATA="
//#define AT_CMD_INQUERY_CLOUD  "AT+INQCLOUD"
//#define AT_CMD_DEEP_SLEEP     "AT+DEEPSLP"
#define AT_CMD_INQUERY_AP     "at+inqap"
#define AT_CMD_SEND_DATA      "at+data="
#define AT_CMD_INQUERY_CLOUD  "at+inqcloud"
#define AT_CMD_DEEP_SLEEP     "at+deepslp"

#define AT_RET_OK             "OK"
#define AT_RET_ERROR          "ERROR"

// Define LCD display message here.
#define LCD_MSG_INIT             "Initializing..."
#define LCD_MSG_AP_CONNECTED     "AP Connected"
#define LCD_MSG_AP_DISCONNECTED  "AP Disconnected"
#define LCD_MSG_WEIGHT           "Weight = "
#define LCD_MSG_UPLOADING        "Uploading"
#define LCD_MSG_UPLOAD_SUCCESS   "Upload Success"
#define LCD_MSG_AP_UPLOAD_FAILED "Upload Failed"
#define LCD_MSG_AP_CLOSE_DEVICE  "Close Device"

// define firmware version: 
String fw_version = "v1.1";

// ====== HW Definition HERE AP

//LiquidCrystal lcd(12, 11, 7, 8, 6, 5);
//create LiquidCrystal class ,use IO:13,8,5,4,3,2 
LiquidCrystal lcd(LCD_RS_PIN,LCD_EN_PIN,LCD_D4_PIN,LCD_D5_PIN,LCD_D6_PIN,LCD_D7_PIN);

// ====== Global parameters HERE =======
long    Weight = 0;            // define weight value, in g unit 
uint8_t led_stat = 0;        //ARDUINO LED, light means initialization is completed. 
uint8_t Sleep_Counter = 0;
uint8_t opl1000_alive = 0;   //opl1000 can be response to at cmd.
uint8_t opl1000_connected_ap    = 0;   //opl1000 connected to AP and got ip.
uint8_t opl1000_connected_cloud = 0;   //opl1000 connected to cloud.
uint8_t opl1000_upload_data     = 0;   //opl1000 upload data to cloud.
char    uart_rx[UART_BUFFER_LEN];

// global parameters define for HX711.
int HX711_SCK = 0;
int HX711_DT = 0;
float ValueGap;
long HX711_Buffer;
long Weight_Rough;
long Weight_Real;
int CurrentAlarm;

void setup()
{
  String outMsg; 
  // Serial Port
  Serial1.begin(SERIAL_BAUD_RATE);  // define serial port baudrate 
  Serial.begin(SERIAL_BAUD_RATE);   // use same baudrate 
  
  HX711_Initial(HX711_SCK_PIN, HX711_DATA_PIN, HX711_GAP_VALUE);   //SCK,DT,GapValue
 
  // GPIO Init
  pinMode(LED, OUTPUT);           // define LED pin as output 
  // pinMode(interruptPin, INPUT);   // Interrupt Pin
  digitalWrite(LED, LOW);         // LED turn off 

  lcd.begin(16,2);      // init LCD1602 
  lcd.clear(); 
  delay(100);  
  lcd.setCursor(0,0);
  lcd.print(LCD_MSG_INIT); // display Welcome to use！  
  delay(100);          //delay 1000ms  
  
  // Setup Interrupt Pin
  pinMode(OPL_INTERRUPT_PIN, OUTPUT);    // define interrupt pin as output 
  digitalWrite(OPL_INTERRUPT_PIN, LOW);  // Set pin to low.
  
  //attachInterrupt(digitalPinToInterrupt(interruptPin), pushBtn, FALLING);    // LOW, CHANGE, RISING, FALLING, HIGH

  opl1000_connected_ap    = 0;   //opl1000 connected to AP and got ip.
  opl1000_connected_cloud = 0;   //opl1000 connected to cloud.
  opl1000_upload_data     = 0;   //opl1000 upload data to cloud.

  // Parameters
  Sleep_Counter = 0;

  outMsg = "OPL1000 Sacler reference firmware " + fw_version;
  Serial.println(outMsg);
  outMsg =  String("Scaler ") + fw_version;
  LCD_display(outMsg,0);
  // Before into main
  
}

void loop()
{
  uint8_t err_type = 0;
  uint8_t i = 0;
  String outMsg;
  String weight_str = String("");
  
  Serial.println("Enter loop...");  // For debug.
  
  if (doMeasureWeight() > 0) // Get valid weight case.
  {
    Sleep_Counter = 0;    // re-initialize the sleep counter value.

	if (opl1000_connected_ap == 0)
	{
      OPL1000_wakeup();     // TBD
	  delay(200);
	}
	
	if (Weight < 100)
	{
	  goto End;
	}
	
	weight_str = String("Weight = ") + Weight + " g";
	//Serial2.println(weight_str);
	Serial.println(weight_str);
	LCD_display(weight_str,0);
	delay(2000);

	for (i = 0;i < COUNT;i++)
	{
	  OPL1000_write_command(AT_CMD_INQUERY_AP); // query OPL1000 AP connection status
      if (OPL1000_get_response(2,AT_RET_OK))
	  {
		opl1000_connected_ap = 1;
		LCD_display(String(LCD_MSG_AP_CONNECTED),0);
		break;
	  }
	  else
	  {
		opl1000_connected_ap = 0;
		continue;
	  }
	}
	
	if (opl1000_connected_ap != 1)  // end this loop if AP not connected.
	{
		LCD_display(String(LCD_MSG_AP_DISCONNECTED),0);
		delay(500);
		goto End;
	}
	
	OPL1000_send_data();    // Send weight data to opl1000.
	LCD_display_upload_status(String(LCD_MSG_UPLOADING));
	Serial.println(LCD_MSG_UPLOADING);
	delay(1000);
	
	for (i = 0;i < COUNT;i++)
	{
	  OPL1000_write_command(AT_CMD_INQUERY_CLOUD); // query OPL1000 cloud connection status
      if (OPL1000_get_response(2,AT_RET_OK))
	  {
		opl1000_connected_cloud = 1;
		LCD_display_upload_status(String(LCD_MSG_UPLOAD_SUCCESS));
		Serial.println(LCD_MSG_UPLOAD_SUCCESS);
		Weight = 0;
		break;
	  }
	  else
	  {
		opl1000_connected_cloud = 0;
		continue;
	  }
	}

	if (opl1000_connected_cloud != 1)  // end this loop if AP not connected.
	{
		LCD_display_upload_status(String(LCD_MSG_AP_UPLOAD_FAILED));
		Serial.println(LCD_MSG_AP_UPLOAD_FAILED);
		Weight = 0;
		delay(1000);
	}
  }
  else
  {
	Sleep_Counter++;
    outMsg =  String("Scaler ") + fw_version;
    LCD_display(outMsg,0);
  }
  
  End:
  Weight = 0;
  if (Sleep_Counter >= SLEEP_COUNT)
  {
    goto_sleep_mode();
  } 
  delay(100);
}

//****************************************************
// LCD related code.
//****************************************************
/* LCD display message. */
void LCD_display(String str,int line)
{
  if(line == 0)
  {
	lcd.clear();
	delay(100);
	lcd.setCursor(0,0);
	lcd.print(str);
	delay(100);
  }
  else if(line == 1)
  {
    lcd.setCursor(0,1);
    lcd.print(str);
	delay(100);
  }
  else
  {
	Serial.println("Invalid msg");
	lcd.clear();
	delay(100);
  }	  
  delay(100);          //delay 100ms  
}

/* LCD display update data status. */
void LCD_display_upload_status(String str)
{
  String str_first_line = String("") + LCD_MSG_WEIGHT + Weight + " g";
  LCD_display(str_first_line,0);
  delay(100);
  LCD_display(str,1);
  delay(100);
}

//****************************************************
// OPL1000 related code.
//****************************************************
// build "AT+DATA,data" at command with weight data.
String OPL1000_build_data(long weight_data) {
  String str = String("") + AT_CMD_SEND_DATA + weight_data + "\r\n";
  Serial.println("write command");  // for debug.
  return str;
}

// Send at command.
void OPL1000_write_command(String cmd) {
  // uint8_t wrLen = cmd.length()+1;

  // Waiting for RX is empty
  while (Serial1.available()) {
    char cc = Serial1.read();
  }

  Serial1.println(cmd);
  Serial.println("write command");
}

// Send weight dataat command.
void OPL1000_send_data() {
  String data = OPL1000_build_data(Weight);

  OPL1000_write_command(data);
  Serial.println("send data command");
  Serial.println(Weight);
}

// Get result of at command.
uint8_t OPL1000_get_response(uint8_t sec, String _search)
{  
  //get uart responce without "Serial.find()".
  unsigned long lastTick = millis();
  unsigned long  timeout;
  uint8_t  ch = 0;
  char     cc;

  if (sec == 0 || sec > 30) {
    timeout = 30000;
  } else {
    timeout = sec * 1000;
  }

  memset(uart_rx, 0, sizeof(uart_rx));

  while ( (millis() - lastTick) < timeout ) {
    while (Serial1.available()) {
      char cc = Serial1.read();
      if ( (cc != '\n') && (cc != '\r') ) {
        uart_rx[ch++] = cc;
      } else {
        // compare string here
        String response(uart_rx);
        if (response.indexOf(_search) != -1) {
          return 1;
        }
      }
    }
  }
  return 0;
}

void OPL1000_wakeup()
{
  digitalWrite(OPL_INTERRUPT_PIN, HIGH);  // Set pin to high.
}  

void goto_sleep_mode()
{
  Sleep_Counter = 0;
  OPL1000_write_command(AT_CMD_DEEP_SLEEP); // Clears the display
}
//****************************************************
// HX711 related code.
//****************************************************
/* HX711 Setup. */
void HX711_begin()
{
  pinMode(HX711_SCK, OUTPUT); 
  pinMode(HX711_DT, INPUT);

  Get_Rough_Value();
}

/* HX711 Initialization. */
void HX711_Initial(int SCK_PIN,int DT_PIN,float GapValueIn)
{
  HX711_SCK = SCK_PIN;
  HX711_DT  = DT_PIN;
  ValueGap  = GapValueIn;
}

/* HX711 pressed. It means starting to weight something. */
int HX711_Pressed(int AlarmValue)
{
  if(HX711_Get_Weight() >= AlarmValue)
  {
    return 1;
  }
  return 0;
}

//****************************************************
//Obtain 
//****************************************************
void Get_Rough_Value()
{
  Weight_Rough = HX711_Read();    
} 

//****************************************************
// Get Weight 
//****************************************************
long HX711_Get_Weight()
{
  HX711_Buffer = HX711_Read();

  Weight_Real = HX711_Buffer;
  Weight_Real = Weight_Real - Weight_Rough;  // Get object AD sample value 
  
  Weight_Real = (long)((float)(Weight_Real/ValueGap)+0.05);   

  if(Weight_Real < 0) {
    Weight_Real = 0 - Weight_Real;
  }
    
  return Weight_Real;
}

//****************************************************
// Read HX711 value 
//****************************************************
unsigned long HX711_Read() // Gain 128
{
  unsigned long count; 
  unsigned char i;
  bool Flag = 0;

  digitalWrite(HX711_DT, HIGH);
  delayMicroseconds(1);

  digitalWrite(HX711_SCK, LOW);
  delayMicroseconds(1);

    count=0; 
    while(digitalRead(HX711_DT)); 
    for(i=0;i<24;i++)
  { 
      digitalWrite(HX711_SCK, HIGH); 
    delayMicroseconds(1);
      count=count<<1; 
    digitalWrite(HX711_SCK, LOW); 
    delayMicroseconds(1);
      if(digitalRead(HX711_DT))
      count++; 
  } 
  digitalWrite(HX711_SCK, HIGH); 
  delayMicroseconds(1);
  digitalWrite(HX711_SCK, LOW); 
  delayMicroseconds(1);
  count ^= 0x800000;
  return(count);
}

// Return 0 if timeout
// Retrun >0, success
int doMeasureWeight()
{
  int timeout = SLEEP_INTERVAL;
  int Weight_Done = 0;
  led_stat = HIGH;
  
  digitalWrite(LED, led_stat);    // lighten LED indicate init is done 
  Weight = 0;
  // HX711 weight calabration
  HX711_begin();          // REad weight value from sensor 

  while (!HX711_Pressed(200) & (timeout-- > 0)) {
    lcd.noDisplay();
    delay(500);          // If no any weight, wait here
    lcd.display();
    delay(500);          // If no any weight, wait here
    // TBD: you can add a timer here for idle
  }
  if (timeout > 0) {
    while (!Weight_Done) {
      Weight = HX711_Get_Weight();    // current sensor weight value 
      delay(40);
      //if (Weight == HX711_Get_Weight()) {
	  if (abs(HX711_Get_Weight() - Weight) <= 20) {
        Weight_Done = 1;
      }
    }
  }

  digitalWrite(LED, LOW);
  return timeout;
}
