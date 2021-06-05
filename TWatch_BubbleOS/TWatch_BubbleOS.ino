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

#include ".\Assets\bluetooth\bluetooth_OFF.h"
#include ".\Assets\bluetooth\bluetooth_ON.h"

#include ".\Assets\battery\battery_0.h"
#include ".\Assets\battery\battery_1.h"
#include ".\Assets\battery\battery_2.h"
#include ".\Assets\battery\battery_3.h"
#include ".\Assets\battery\battery_4.h"

#include "config.h"
#include <soc/rtc.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include "cert.h"
TTGOClass *ttgo;

TFT_eSPI tft = TFT_eSPI();    

const char * ssid = "SSID1-2.4G";
const char * password = "12345678";

String NewFirmwareVer;
String FirmwareVer = {"0.0"};
static const char *version_url = "https://raw.githubusercontent.com/solid-droid/BubbleOS/main/Releases/LatestVersion.txt";

///////////////---System Variables---//////////////////////////////////////////////////////////
char buf[128];
#define DEG2RAD 0.0174532925
bool wifiConnected      = false;
bool irq                = false;
bool rtcIrq             = false;
char* SYS_devices[]     = {"display", "gps", "backlight", "touch"};
uint8_t APP_count       = 0;  
int  battery            = 100;          //Stores current battery percentage.                         
float batteryTime       = 0;            //Stores battery time avaible before next charge.                 
File APP_SD;
File SYS_SD;
int  idleTimeTracker    = 0;
uint32_t previousMillis;
bool touch              = false;         //Detected touch
int  touchPoint[2]      = {0,0};         //Touch start point
uint16_t touchX          = 0;            //Current touch x
uint16_t touchY          = 0;            //Current touch y
bool tap                = false;         //Detected tap
bool drag               = false;         //Detected drag
bool dragStart          = false;         //Detected drag/hold Start
bool dragEnd            = false;         //Detected drag/hold Start
bool hold               = false;         //Detected hold
uint32_t touchTime      = 0;             //Touch start time millis
bool watchInSleep       = false; 
bool TouchWakeUp        = false;  
uint8_t FEND_hour =0, FEND_minutes=0;

//////////////--Tunable Variable--////////////////////////////////////////////////////////////////
int  idleTime0          = 7;             //Maximum allowed idle time (sec) before screen dims (No touch)
int  idleTime1          = 15;            //Maximum allowed idle time (sec) before screen turns off (No touch)
int  idleTime2          = 120;           //Maximum allowed idle time (sec) before shutDown
int  dragThreshold      = 500;           //Threshold millis to treat tap as drag/hold.
int  holdThreshold      = 0;             //Allowed movement range in hold mode.
int  Max_APPS           = 20;            //Max external app count
String APP_list[20];                     //Max external app count
uint32_t  BEND_delay_register[20]={};    //Max number of async delay calls
uint8_t  loadBrightness = 100;           //Brightness 

#include "boot.h"
#include "system.h"
#include "application.h"
#include "backend.h"
#include "frontend.h"


void setup() {
  Serial.begin(115200);
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
  APP_drawClockCenter();
  FEND_clock();
  FEND_bluetooth_OFF();
  FEND_menu_Icon();
  FEND_battery_Icon();
/////////////////////////////////////////////////
//BOOT_connectWiFi();
//SYS_getAPPS();
//APP_showAppList();
/////////////////////////////////////////////////

}

int16_t angle =0;
void loop() {
  
  BEND_idleTimeTracker();
  BEND_systemMonitoring();
  BEND_touchDetection();
  BEND_powerButtonInterrupt();
  BEND_swipeBrightness();
  //////////////////////////////////
  if(BEND_delay(1000,0)) FEND_clock();
  if(BEND_delay(300,1))  FEND_wifi_connecting();
  if(BEND_delay(5000,2)) FEND_battery_Icon();

}
