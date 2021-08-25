/*
 * Bubble OS for Lilygo T-Watch 2020 v2 
 * Created By Nikhil M Jeby
 * Github: https://github.com/solid-droid/Bubble-OS
*/
#include ".\Assets\Wifi\wifi_0.h"
#include ".\Assets\Wifi\wifi_1.h"
#include ".\Assets\Wifi\wifi_2.h"
#include ".\Assets\Wifi\wifi_3.h"
#include ".\Assets\Wifi\wifi_4.h"

#include ".\Assets\WatchFace\AnalogBasic\center.h"
#include ".\Assets\WatchFace\AnalogBasic\needle_hour.h"
#include ".\Assets\WatchFace\AnalogBasic\needle_minutes.h"
#include ".\Assets\WatchFace\AnalogBasic\needle_seconds.h"

#include ".\Assets\bubbleMenu\menu_icon.h"

#include ".\Assets\alarm\alarm_OFF.h"
#include ".\Assets\alarm\alarm_ON.h"

#include ".\Assets\gps\gps_OFF.h"
#include ".\Assets\gps\gps_ON.h"

#include ".\Assets\bluetooth\bluetooth_OFF.h"
#include ".\Assets\bluetooth\bluetooth_ON.h"

#include ".\Assets\battery\battery_0.h"
#include ".\Assets\battery\battery_1.h"
#include ".\Assets\battery\battery_2.h"
#include ".\Assets\battery\battery_3.h"
#include ".\Assets\battery\battery_4.h"

#include ".\Assets\apps\IOT.h"

#include "config.h"
/////RTC
#include <soc/rtc.h>
/////EEPROM
#include <EEPROM.h>
#define EEPROM_SIZE 100
//10-39 WIFI SSID
//40-70 WIFI password
//3     WIFI password length
//2     WIFI SSID length
//1     WIFI autoConnect -> 1= dont connect, 0=connect.
//0    reset
uint8_t clearEEPROM = 1;
/////wifi
#include <WiFi.h>
#include <WiFiClient.h>
/////bluetooth
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
////ttgo
TTGOClass *ttgo;

TFT_eSPI tft = TFT_eSPI();    

char ssid[30];
char password[30];
WiFiServer server(80);
IPAddress local_IP(192, 168, 1, 11);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 0, 0);

String FirmwareVer = "0.0";

///////////////---System Variables---//////////////////////////////////////////////////////////
char buf[128];
#define DEG2RAD 0.0174532925
bool wifiConnected      = false;
bool irq                = false;
bool rtcIrq             = false;
char* SYS_devices[]     = {"display", "gps", "backlight", "touch"};
uint8_t APP_count       = 0;  
uint8_t  battery        = 100;          //Stores current battery percentage.                         
float batteryTime       = 0;            //Stores battery time avaible before next charge.
float batteryVolt       = 0;            //Volts       
File APP_SD;
File SYS_SD;
int  idleTimeTracker    = 0;
uint32_t previousMillis;
bool touch              = false;         //Detected touch
uint8_t touchPoint[2]   = {0,0};         //Touch start point
uint8_t touchX          = 0;             //Current touch x
uint8_t touchY          = 0;             //Current touch y
bool tap                = false;         //Detected tap
bool drag               = false;         //Detected drag
bool dragStart          = false;         //Detected drag/hold Start
bool dragEnd            = false;         //Detected drag/hold Start
bool hold               = false;         //Detected hold
uint32_t touchTime      = 0;             //Touch start time millis
bool watchInSleep       = false;         //Watch Screen OFF
bool TouchWakeUp        = false;         //Wakeup by touch.
uint8_t FEND_hour =0, FEND_minutes=0;    //Previous Hour and Minutes
uint8_t currentScreen   = 0;             //Tracks screen
bool    screenLoad      = false;         //For initial loading of screens
uint8_t currentMenuIndex=0;              //For Scroll Menu
String  scrollMenuList[10];              //Menu List
bool clearScreen        = false;         //Enable to clear screen.
uint8_t WifiCred        = 0;             //0- no data , 1 -client , 2 - client + pass

byte WIFI_STATUS     = 0;                 //0-not connected 1-connecting 2-connected
bool BT_STATUS       = false;            //false-not connected true-connected
bool GPS_STATUS      = false;            //false-not connected true-connected
bool reset           = false;

bool savePower       = true;            //enable auto sleep after timeout
 
////---ICON configs---////////////
bool showAlarmGPSIcon = true;
uint8_t ICON_GPS[4] = {190,5,22,23};
uint8_t ICON_ALARM[4] = {203,24,35,34};

bool showNetworkIcon = true;
uint8_t ICON_WIFI[4] = {22,0,21,22}; //x , y, w, h 
uint8_t ICON_BT[4]   = {2,26,22,28};

bool showBatteryIcon = true;
uint8_t ICON_BATTERY[4] = {197,198,43,42};

//////////////--Tunable Variable--////////////////////////////////////////////////////////////////
int  idleTime0          = 7;             //Maximum allowed idle time (sec) before screen dims (No touch)
int  idleTime1          = 15;            //Maximum allowed idle time (sec) before screen turns off (No touch)
int  idleTime2          = 60;           //Maximum allowed idle time (sec) before shutDown
int  dragThreshold      = 500;           //Threshold millis to treat tap as drag/hold.
int  holdThreshold      = 0;             //Allowed movement range in hold mode.
uint32_t  BEND_delay_register[20]={};    //Max number of async delay calls
uint8_t  loadBrightness = 100;           //Brightness 
int  Max_APPS           = 20;            //Max external app count
String APP_list[20];                     //Max external app count

/////////////--SUB-SCREEN-variables--///////////////////////////////////////////////////////////////
bool Button_on  = true;
bool Button_off = true;
bool Button_reset = true;

/////////////////--OS files--///////////////////////////////////////////////////////////////////////
#include "bluetooth.h"
uint8_t FEND_wifi_counter = 0;
void FEND_wifi_connecting();
#include "boot.h"
#include "system.h"
#include "application.h"
#include "backend.h"
/////////--SHARED SERVICES--/////////
void FEND_wifi_connecting();
void FEND_WIFI_ICO();
void FEND_BT_ICO();
void FEND_GPS_ICO();
void FEND_loadIcons();
////////--SUB-SCREANS--//////////////
#include "APP_PAGE.h"
#include "NETWORK_PAGE.h"
#include "POWER_PAGE.h"
#include "GPS_ALARM_PAGE.h"

//////////////////////////////////////
#include "screenBuilder.h"
#include "frontend.h"


void setup() {
  Serial.begin(115200);
  EEPROM.begin(EEPROM_SIZE);
  ttgo = TTGOClass::getWatch();
  ttgo->begin();
  ttgo->tft->setSwapBytes(true);
  BOOT();
  BOOT_setBrightness(loadBrightness);
  ttgo->tft->setTextSize(2);
  previousMillis = millis();
  RTC_Date currentTime = ttgo->rtc->getDateTime();
  FEND_hour = currentTime.hour;
  FEND_minutes = currentTime.minute;
///////////--first time UI--//////////////////////////
  if(BEND_resetOS()){
    Serial.println("Reset Success");
  } else {
    Serial.println("Welcome back");
  }
  if(reset){
      FEND_SB_welcome();
      delay(2000);
  }
  reset = false;
///////////--WiFi-autoConnect--//////////////////////
  FEND_SB_connectWiFi();
///////////--regular UI--////////////////////////////  
  FEND_loadIcons();
  APP_drawClockCenter();
  FEND_clock();
////////////////////////////////////////////////////
//SYS_getAPPS();
//APP_showAppList();
///////////////////////////////////////////////////

}

int16_t angle =0;
void loop() {
  BEND_idleTimeTracker();
  BEND_systemMonitoring();
  BEND_touchDetection();
  BEND_powerButtonInterrupt();
  BEND_swipeBrightness();
  BEND_server();
  BEND_maintainWiFi();
  //////////////////////////////////
  APP_digitalClock();
  APP_remainingTime();
  //////////////////////////////////
 if(WIFI_STATUS == 1 && BEND_delay(300,0)) FEND_wifi_connecting();
 if(showBatteryIcon && BEND_delay(5000,1)) FEND_battery_Icon();
 if(BEND_updateScreen() || clearScreen){
  if(BT_STATUS){
    stopBluetooth();
  }
  ttgo->tft->fillScreen(TFT_BLACK);
  FEND_loadIcons();
  screenLoad = true;
  clearScreen = false;
 }
 else screenLoad = false;
//////---Application screens --///////////
 switch(currentScreen)
  {
    case 0 : //Clock screen - home
      if(BEND_delay(1000,2)) FEND_clock(); 
      showAlarmGPSIcon = true;
      showNetworkIcon = true;
      showBatteryIcon = true;
      break;
    case 1 : //Network Menu 
      FEND_network();
      break;
    case 2 : //battery Menu 
      FEND_power();
      break;
    case 3 : //GPS / Alarm Menu
      FEND_nav_alarm();
      break;
    case 4 : //App Menu - bubble menu
      FEND_bubbleMenu();
      break;
    case 5 : 
      FEND_screenBuilder();
      break;
  }
}
