/*
 * Bubble OS for Lilygo T-Watch 2020 v2 
 * Created By Nikhil.M.Jeby
 * Github: 
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
/////////////////////////////////////////////////////////////////////////////////////////////////
TTGOClass *ttgo;

#include "boot.h"
#include "system.h"

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
  Serial.begin(115200);
  ttgo = TTGOClass::getWatch();
  ttgo->begin();
  BOOT();
  ttgo->tft->setTextSize(2);
  Serial.println("loading");
}

void loop() {
///////////////--Touch Control--/////////////////////////////
  int16_t x, y;
  dragStart = false;
  dragEnd   = false;
  if (ttgo->getTouch(x, y)) {
    ttgo->tft->drawString("x: "+String(x)+" y: "+String(y) , 10, 150);
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
   if(tap){
    ttgo->tft->drawString("tap", 10, 20);
   }
   if(drag){
     ttgo->tft->drawString("drag", 10, 20);
   }
   if(hold){
     ttgo->tft->drawString("hold", 10, 20);
   }

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
