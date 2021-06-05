void BEND_swipeBrightness()
{
    if(drag && touchY < 50){
      if(touchX<10)
      BOOT_setBrightness(7);
      else if (touchX < 200)
      BOOT_setBrightness(touchX);
      else 
      BOOT_setBrightness(255);
    }

}

void BEND_idleTimeTracker(){

     if(previousMillis+1000<=millis())
    {
      previousMillis=millis();
      idleTimeTracker+=1;
    }

}

void BEND_systemMonitoring(){

    SYS_getBatteryLevel();
    SYS_getRemainingTime();
    SYS_savePower();

}

void BEND_maintainWiFi(){

    wifiConnected = WiFi.status() != WL_CONNECTED ? false : true;
    if(wifiConnected){
      APP_drawText("Wifi Connected", 3, 150, 16);
    }else{
      APP_drawText("Wifi Disconnected", 3, 150, 16);
    }
}
bool BEND_touchPointChange(){
  
  uint8_t deltaX = abs(touchPoint[0] - touchX);
  uint8_t deltaY = abs(touchPoint[1] - touchY);
  if( deltaX > holdThreshold || deltaY > holdThreshold)
  {
    return true;
  } else {
    return false;
  }
}

void BEND_touchDetection(){
    int16_t x, y;
    dragStart = false;
    dragEnd   = false;
    if (ttgo->getTouch(x, y)) {
       touchX=x;
       touchY=y;
       idleTimeTracker=0;
       if(watchInSleep){
        SYS_wakeup();
        TouchWakeUp = true;
        BOOT_setBrightness(loadBrightness);
       }
       else {
        if(!TouchWakeUp){
         if(!touch)
           {
            BOOT_setBrightness(loadBrightness);
            touchTime = millis();
            touchPoint[0] = touchX;
            touchPoint[1] = touchY; 
            touch = true;
           } 
         if(millis()-touchTime>dragThreshold){
          dragStart = true;
          if(!BEND_touchPointChange() && !drag)
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
     }
    } else {
      tap   = false;
      TouchWakeUp = false;
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
}

void BEND_powerButtonInterrupt(){
      if(irq)
    {
        TouchWakeUp = false;
        idleTimeTracker=0;
        if(BOOT_deviceStatus("backlight")){
          SYS_sleep();}
        else {
          BOOT_setBrightness(loadBrightness);
          SYS_wakeup();}
        vTaskDelay(700/portTICK_PERIOD_MS);
        BOOT_clearIRQ();
    }
}


bool BEND_delay(int value ,int index){
 if(!BEND_delay_register[index]){
    BEND_delay_register[index] = millis();
    return false;
  }else{
    if(BEND_delay_register[index]+value>=millis()) {
      return false;
    } else {
      BEND_delay_register[index]=0;
      return true;
    }
  }  
}

bool BEND_checkTap(uint8_t *pos){
  if(touchX>=pos[0] && touchX<=pos[1] && touchY>=pos[2] && touchY<=pos[3])
  return true;
  else return false;
}

bool BEND_updateScreen(){
uint8_t battery[4]={0,50,0,50}, menu[4]={0,50,200,240} , network[4]={190, 240 ,190, 240}, gps[4]={200, 240, 0 ,  50};
if(tap){
  uint8_t prev = currentScreen;
  bool refresh = false;
    if(BEND_checkTap(menu)){
      refresh = true;
      currentScreen     = 4;
    }
    if(BEND_checkTap(network)){
      refresh = true;
      currentScreen     = 1;
    }
    if(BEND_checkTap(battery)){
      refresh = true;
      currentScreen     = 2;
    }
    if(BEND_checkTap(gps)){
      refresh = true;
      currentScreen     = 3;
    }
    if(prev == currentScreen && refresh) currentScreen  = 0;
    return refresh;
   
}
return false;
}
