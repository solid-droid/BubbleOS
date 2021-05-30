void SYS_devicePower(bool value)
{
  for(int i=0; i< sizeof(SYS_devices); ++i)
  {
    BOOT_power(SYS_devices[i], value);
  }
}
void SYS_sleep()
{
 BOOT_power(SYS_devices[2], false);
}

void SYS_wakeup()
{
  BOOT_power(SYS_devices[2], true);
}

void SYS_powerOFF()
{
   SYS_sleep();
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
