void BEND_swipeBrightness(void *parameters)
{
  for(;;){
    if(drag && touchY < 50){
      if(touchX<10)
      BOOT_setBrightness(7);
      else if (touchX < 200)
      BOOT_setBrightness(touchX);
      else 
      BOOT_setBrightness(255);
    }
  }
}

void BEND_idleTimeTracker(void *parameters){
  for(;;){
     if(previousMillis+1000<=millis())
    {
      previousMillis=millis();
      idleTimeTracker+=1;
    }
  }
}

void BEND_systemMonitoring(void *parameters){
  for(;;){
    SYS_getBatteryLevel();
    SYS_getRemainingTime();
    SYS_savePower();
  }
}

void BEND_maintainWiFi(void *parameters){
  for(;;){
    wifiConnected = WiFi.status() != WL_CONNECTED ? false : true;
    if(wifiConnected){
      APP_drawText("Wifi Connected", 3, 150, 16);
    }else{
      APP_drawText("Wifi Disconnected", 3, 150, 16);
    }
    vTaskDelay(10000/portTICK_PERIOD_MS);
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

void BEND_touchDetection(void *parameters){
  for(;;){
    int16_t x, y;
    dragStart = false;
    dragEnd   = false;
    if (ttgo->getTouch(x, y)) {
       touchX=x;
       touchY=y;
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
  }
}

void BEND_powerButtonInterrupt(void *parameters){
  for(;;){
      if(irq)
    {
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
}
