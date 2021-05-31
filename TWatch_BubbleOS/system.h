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

void SYS_savePower(){
    if(idleTimeTracker>idleTime0 && idleTimeTracker<idleTime1)      BOOT_setBrightness(7, false);
    else if(idleTimeTracker>idleTime1 && idleTimeTracker<idleTime2) SYS_sleep();
    else if(idleTimeTracker>idleTime2)                              BOOT_deepSleep();
}
