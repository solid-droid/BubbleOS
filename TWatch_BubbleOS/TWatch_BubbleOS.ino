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
//Touch Screen
bool touch              = false;         //Detected touch
int touchPoint[2]       = {0,0};         //Touch start point
bool tap                = false;         //Detected tap
bool drag               = false;         //Detected drag
bool dragStart          = false;         //Detected drag/hold Start
bool dragEnd            = false;         //Detected drag/hold Start
bool hold               = false;         //Detected hold
int dragThreshold       = 500;           //Threshold millis to treat tap as drag/hold.
int holdThreshold       = 0;             //Allowed movement range in hold mode.
uint32_t touchTime      = 0;             //Touch start time millis
char* SYS_devices[]     = {"display", "gps", "backlight", "touch"};
int Max_APPS            = 20;            // Max external app count = 20
String APP_list[Max_APPS];                     
uint8_t APP_count       = 0;  
File APP_SD;
File SYS_SD;
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
  SYS_getAPPS();
APP_showAppList();
  ttgo->tft->setTextSize(2);
}

void loop() {
///////////////--Touch Control--/////////////////////////////
  int16_t x, y;
  dragStart = false;
  dragEnd   = false;
  if (ttgo->getTouch(x, y)) {
     if(!touch)
     {
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
////////////////////--Home--//////////////////////////////////////////////////////////
    ttgo->tft->setTextSize(2);
    ttgo->tft->drawString("..."+String(ttgo->rtc->formatDateTime())+"...", 10, 90);
////////////////////--Power Button Control--//////////////////////////////////////////
  if(irq)
  {
      if(BOOT_deviceStatus("backlight")){
        SYS_sleep();}
      else {
        SYS_wakeup();}
      delay(3000);
      BOOT_clearIRQ();
  }
}
