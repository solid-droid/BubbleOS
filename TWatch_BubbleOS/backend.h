void BEND_swipeBrightness()
{
    if(drag && touchY < 50 && touchPoint[1] < 50){
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
      if(idleTimeTracker>idleTime2+1){
        idleTimeTracker = 0;
      }
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
       WIFI_STATUS = 2;
    }else{
      WIFI_STATUS = 0;
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
uint8_t network[4]={0,50,0,50}, menu[4]={0,50,200,240} , battery[4]={190, 240 ,190, 240}, gps[4]={190, 240, 0 ,  60};
if(tap){
  savePower = true;
  uint8_t prev = currentScreen;
  bool refresh = false;
    if(BEND_checkTap(menu)){
      refresh = true;
      currentScreen     = 4;
    }
    if(showNetworkIcon && BEND_checkTap(network)){
      refresh = true;
      currentScreen     = 1;
    }
    if(showBatteryIcon && BEND_checkTap(battery)){
      refresh = true;
      currentScreen     = 2;
    }
    if(showAlarmGPSIcon && BEND_checkTap(gps)){
      refresh = true;
      currentScreen     = 3;
    }
    if(prev == currentScreen && refresh) currentScreen  = 0;
    return refresh;
   
}
return false;
}

bool BEND_srollMenuSelection() {
  if(tap && touchY > 85 && touchY < 145) return true;
  else return false;
  
}

bool BEND_resetOS(){
   WifiCred = 2;
  if(EEPROM.read(0)!= clearEEPROM){
    WifiCred = 0;
    reset = true;
    //set cleared
      EEPROM.write(0, clearEEPROM);
      EEPROM.commit();
      delay(2);
    //reset wifi auto connect
      EEPROM.write(1, 0);
      EEPROM.commit();
      delay(2);
    return true;
  }
  return false;
}

void BEND_storeToEEPROM(char* dat, uint8_t s){
 for(uint8_t i = 0; i < strlen(dat) ; ++i){
  EEPROM.write(s+i, dat[i]);
  EEPROM.commit();
 } 
}


void BEND_storeWifiCred(){
  EEPROM.write(2, strlen(ssid));
  EEPROM.commit();
  BEND_storeToEEPROM(ssid, 10);
  EEPROM.write(3, strlen(password));
  EEPROM.commit();
  BEND_storeToEEPROM(password, 40);
}

void BEND_readWifiCred(){
  String _ssid = "" , _pass = "";
  char ch;
  int ssidlen = EEPROM.read(2);
  int passLen = EEPROM.read(3);
  for(uint8_t i = 0; i < ssidlen ; ++i){
    ch = EEPROM.read(10+i);
   _ssid += ch;
  }
  for(uint8_t i = 0; i < passLen ; ++i){
    ch = EEPROM.read(40+i);
   _pass += ch;
  }
  
  _ssid.toCharArray(ssid, 30);
  _pass.toCharArray(password, 30);
}


void BEND_server(){
  WiFiClient client = server.available();
  if(client){

    Serial.println("");
    Serial.println("New client");
    
    if(!client.connected() || client.available()){

          String req = client.readStringUntil('\r');
          int addr_start = req.indexOf(' ');
          int addr_end = req.indexOf(' ', addr_start + 1);
          if (addr_start == -1 || addr_end == -1) {
              Serial.print("Invalid request: ");
              Serial.println(req);
              return;
          }
          req = req.substring(addr_start + 1, addr_end);
          Serial.print("Request: ");
          Serial.println(req);
      
          String s;
          if (req == "/")
          {
              s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>";
              s += "Hello from BubbleOS";
              s += "</html>\r\n\r\n";
              Serial.println("Sending 200");
          }
          else
          {
              s = "HTTP/1.1 404 Not Found\r\n\r\n";
              Serial.println("Sending 404");
          }
          client.print(s);
      
          client.stop();
          Serial.println("Done with client");
      }
    }
}
