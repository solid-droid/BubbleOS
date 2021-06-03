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

//////////////--Tunable Variable--////////////////////////////////////////////////////////////////
int  idleTime0          = 7;             //Maximum allowed idle time (sec) before screen dims (No touch)
int  idleTime1          = 15;            //Maximum allowed idle time (sec) before screen turns off (No touch)
int  idleTime2          = 120;           //Maximum allowed idle time (sec) before shutDown
int  dragThreshold      = 500;           //Threshold millis to treat tap as drag/hold.
int  holdThreshold      = 0;             //Allowed movement range in hold mode.
int  Max_APPS           = 20;            //Max external app count = 20
String APP_list[20];                     //Max external app count = 20
uint8_t  loadBrightness = 100;           //Brightness 
///////////---Task Handlers---//////////////////////////////////
TaskHandle_t task_BEND_idleTimeTracker = NULL;
TaskHandle_t task_BEND_systemMonitoring = NULL;
TaskHandle_t task_BEND_touchDetection = NULL;
TaskHandle_t task_BEND_powerButtonInterrupt = NULL;
TaskHandle_t task_BEND_swipeBrightness = NULL;
/////////////////////////////////////////////////////////////////
#include "boot.h"
#include "system.h"
#include "application.h"
#include "backend.h"
#include "frontend.h"

void BeginTaskManager(){
///////////---Backend Tasks---//////////////////////////////////
xTaskCreate( BEND_idleTimeTracker,                          "Idle Time",
             1000, NULL, 3, &task_BEND_idleTimeTracker );
xTaskCreate( BEND_systemMonitoring,                         "Power Monitor",
             2000, NULL, 3, &task_BEND_systemMonitoring );
xTaskCreate( BEND_touchDetection,                           "Touch Detection",
             1000, NULL, 5, &task_BEND_touchDetection );
xTaskCreate( BEND_powerButtonInterrupt,                     "Power Button",
             1000, NULL, 3, &task_BEND_powerButtonInterrupt);
xTaskCreate( BEND_swipeBrightness,                          "Brightness",
             1000, NULL, 3, &task_BEND_swipeBrightness);  
}


void setup() {
  Serial.begin(115200);
  ttgo = TTGOClass::getWatch();
  ttgo->begin();
  ttgo->tft->setSwapBytes(true);
  BOOT();
  BOOT_setBrightness(loadBrightness);
  ttgo->tft->setTextSize(2);
  previousMillis = millis();
  BeginTaskManager();

/////////////////////////////////////////////////
//BOOT_connectWiFi();
//SYS_getAPPS();
//APP_showAppList();
/////////////////////////////////////////////////

}


void loop() {
  
  FEND_clock();
  FEND_wifi_connected();
  FEND_bluetooth_ON();
  FEND_menu_Icon();
  FEND_battery_Icon();
  FEND_menu_Icon();

//vTaskDelete(<tast>);
//vTaskSuspend(<tast>);
//vTaskResume(<task>);
//xTaskResumeAll();
}
