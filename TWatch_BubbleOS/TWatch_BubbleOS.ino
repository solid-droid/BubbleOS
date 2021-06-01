/*
 * Bubble OS for Lilygo T-Watch 2020 v2 
 * Created By Nikhil M Jeby
 * Github: https://github.com/solid-droid/Bubble-OS
*/
#include "config.h"
#include <soc/rtc.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include "cert.h"


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
//-----------------------------------------------------//////////
TaskHandle_t task_FEND_home = NULL;
//----------------------------------------------------///////////
TaskHandle_t task_TaskManager = NULL;
/////////////////////////////////////////////////////////////////

TTGOClass *ttgo;
#include "boot.h"
#include "system.h"
#include "application.h"
#include "backend.h"
#include "frontend.h"

void TaskManager(void *parameters)
{
    /////////--Begin Background Tasks--///////
  vTaskResume(task_BEND_idleTimeTracker);
  vTaskResume(task_BEND_systemMonitoring);
  vTaskResume(task_BEND_touchDetection);
  vTaskResume(task_BEND_powerButtonInterrupt);
  vTaskResume(task_BEND_swipeBrightness);
   /////////--Starting Screen--/////////////
   vTaskResume(task_FEND_home);
  for(;;){
    ////////--Screen Change Management--//////////   
  }
}

void BeginTaskManager(){
///////////---Backend Tasks---//////////////////////////////////
xTaskCreate( BEND_idleTimeTracker,                          "Idle Time Tracker",
             1000, NULL, 3, &task_BEND_idleTimeTracker );
xTaskCreate( BEND_systemMonitoring,                         "System Monitoring",
             1000, NULL, 3, &task_BEND_systemMonitoring );
xTaskCreate( BEND_touchDetection,                           "Touch detection",
             1000, NULL, 3, &task_BEND_touchDetection );
xTaskCreate( BEND_powerButtonInterrupt,                     "Power Button Interrupt",
             1000, NULL, 3, &task_BEND_powerButtonInterrupt);
xTaskCreate( BEND_swipeBrightness,                          "Brightness Control",
             1000, NULL, 3, &task_BEND_swipeBrightness);
//////////---Frontend Tasks---/////////////////////////////////

xTaskCreate( FEND_home,                                     "Clock + Menus",
             1000, NULL, 3, &task_FEND_home );

//////////---Task Manager---//////////////////////////////////
xTaskCreate( TaskManager,                                   "TaskManager",
             1000, NULL, 3, &task_TaskManager);
}


void setup() {
  Serial.begin(115200);
  ttgo = TTGOClass::getWatch();
  ttgo->begin();
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
  if(task_TaskManager!=NULL)
  {
      delay(1000);
      vTaskResume(task_TaskManager);
      vTaskSuspendAll();
  }
  
//vTaskDelete(<tast>);
//vTaskSuspend(<tast>);
//vTaskResume(<task>);
//xTaskResumeAll();
}
