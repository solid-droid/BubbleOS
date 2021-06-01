void SYS_devicePower(bool value)
{
  for(int i=0; i< sizeof(SYS_devices); ++i)
  {
    BOOT_power(SYS_devices[i], value);
  }
}
void SYS_sleep()
{
 watchInSleep= true;
 BOOT_power(SYS_devices[2], false);
}

void SYS_wakeup()
{
  watchInSleep= false;
  BOOT_power(SYS_devices[2], true);
}

void SYS_powerOFF()
{
   watchInSleep= true;
   SYS_devicePower(false);
   BOOT_deepSleep();
}

void SYS_getAPPS(){
   int i=0;
   if(APP_SD){
     while(i<Max_APPS)
      {
        File entry =  APP_SD.openNextFile();
        if (! entry) break;
        APP_count=i+1;
        APP_list[i] = entry.name();
        entry.close();
        i++;
      }
   }
}

void SYS_getBatteryLevel(){
    battery = ttgo->power->getBattPercentage();
}

float SYS_getDischargeTime(){
  float totalTime = ttgo->power->getBattDischargeCurrent();
  if(totalTime > 0)
  {
    totalTime = 380.0/ totalTime;
  }else
  {
    totalTime = 1;
  }
  return totalTime;
  
}

void SYS_savePower(){
    if(idleTimeTracker>idleTime0 && idleTimeTracker<idleTime1)      BOOT_setBrightness(7, false);
    else if(idleTimeTracker>idleTime1 && idleTimeTracker<idleTime2) SYS_sleep();
    else if(idleTimeTracker>idleTime2)                              BOOT_deepSleep();
}

float SYS_timeMean=0;
uint16_t SYS_timeMeanArrCounter=0;
void SYS_getRemainingTime()
{
  float dischargeRate = SYS_getDischargeTime();
  if(dischargeRate>5){
    SYS_timeMean += dischargeRate*battery/100;
    if(SYS_timeMeanArrCounter>500){
      batteryTime = SYS_timeMean/SYS_timeMeanArrCounter;
      SYS_timeMean = 0;
      SYS_timeMeanArrCounter = 0;
    } 
    SYS_timeMeanArrCounter+=1;
  }
}

bool SYS_FirmwareVersionCheck(void) {
  String payload;
  int httpCode;
  String fwurl = "";
  fwurl += version_url;
  fwurl += "?";
  fwurl += String(rand());
  Serial.println(fwurl);
  WiFiClientSecure * client = new WiFiClientSecure;

  if (client) 
  {
    client -> setCACert(rootCACertificate);

    // Add a scoping block for HTTPClient https to make sure it is destroyed before WiFiClientSecure *client is 
    HTTPClient https;

    if (https.begin( * client, fwurl)) 
    { // HTTPS      
      Serial.print("[HTTPS] GET...\n");
      // start connection and send HTTP header
      delay(100);
      httpCode = https.GET();
      delay(100);
      if (httpCode == HTTP_CODE_OK) // if version received
      {
        payload = https.getString(); // save received version
      } else {
        Serial.print("error in downloading version file:");
        Serial.println(httpCode);
      }
      https.end();
    }
    delete client;
  }
      
  if (httpCode == HTTP_CODE_OK) // if version received
  {
    payload.trim();
    if (payload.equals(FirmwareVer)) {
      Serial.printf("\nDevice already on latest firmware version:%s\n", FirmwareVer);
      return false;
    } 
    else 
    {
      NewFirmwareVer = payload;
      Serial.println(payload);
      Serial.println("New firmware detected");
      return true;
    }
  } 
  return false;  
}
