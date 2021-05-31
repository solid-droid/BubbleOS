/*
 * Bubble OS for Lilygo T-Watch 2020 v2 
 * Created By Nikhil M Jeby
 * Github: https://github.com/solid-droid/Bubble-OS
*/
#include "config.h"
#include <soc/rtc.h>

///////////////---System Variables---//////////////////////////////////////////////////////////
char buf[128];
bool irq                = false;
bool rtcIrq             = false;
char* SYS_devices[]     = {"display", "gps", "backlight", "touch"};
uint8_t APP_count       = 0;  
int  battery            = 100;          //Stores current battery percentage.                         
float batteryTime       = 0;            //Stores battery time avaible before next charge.                 
bool FEND_task[20];                     //Frontend taskManager
bool BEND_task[20];                     //Backend  taskManager
File APP_SD;
File SYS_SD;
int  idleTimeTracker    = 0;
uint32_t previousMillis;
bool touch              = false;         //Detected touch
int  touchPoint[2]      = {0,0};         //Touch start point
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
/////////////////////////////////////////////////////////////////////////////////////////////////
TTGOClass *ttgo;

#include "boot.h"
#include "system.h"
#include "application.h"
#include "backend.h"
#include "frontend.h"

bool touchPointChange(int x, int y){
  
  int deltaX = abs(touchPoint[0] - x);
  int deltaY = abs(touchPoint[1] - y);
  if( deltaX > holdThreshold || deltaY > holdThreshold)
  {
    return true;
  } else {
    return false;
  }
}
void setup() {
  ttgo = TTGOClass::getWatch();
  ttgo->begin();
  BOOT();
  BOOT_setBrightness(loadBrightness);
  SYS_getAPPS();
//APP_showAppList();
  ttgo->tft->setTextSize(2);
  ////////////////////////////////////////////////////////////////
  previousMillis = millis();
}

void loop() {
 if(previousMillis+1000<=millis())
  {
    previousMillis=millis();
    idleTimeTracker+=1;
  }
/////////////--Power  Saving--//////////////////////////////
  SYS_getBatteryLevel();
  SYS_getRemainingTime();
  SYS_savePower();
///////////////--Touch Control--/////////////////////////////
  int16_t x, y;
  dragStart = false;
  dragEnd   = false;
  if (ttgo->getTouch(x, y)) {
     idleTimeTracker=0;
     if(watchInSleep){
      SYS_wakeup();
      BOOT_setBrightness(loadBrightness);
     }
     else {
      if(!touch)
       {
        BOOT_setBrightness(loadBrightness);
        touchTime = millis();
        touchPoint[0] = x;
        touchPoint[1] = y; 
        touch = true;
       } 
      if(millis()-touchTime>dragThreshold){
      dragStart = true;
      if(!touchPointChange(x,y) && !drag)
      { 
        hold = true;
      }
      else
      {
        drag = true;
        hold = false;
      }
     }
    }
  } else {
    tap   = false;
    if(drag || hold)
    {
      dragEnd = true;
    }
    if(!drag && !hold && touch){
      tap = true;
    }
    touchTime = 0; 
    touch = false;
    hold  = false;
    drag  = false;
 }
/////////////////////--Application--//////////////////////////////////////////
   BEND_begin(x,y);
   FEND_begin(x,y);
////////////////////--Power Button Control--//////////////////////////////////////////
  if(irq)
  {
      idleTimeTracker=0;
      if(BOOT_deviceStatus("backlight")){
        SYS_sleep();}
      else {
        BOOT_setBrightness(loadBrightness);
        SYS_wakeup();}
      delay(700);
      BOOT_clearIRQ();
  }
}
