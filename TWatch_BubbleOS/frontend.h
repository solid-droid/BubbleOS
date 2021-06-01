////////////////////--Tasks--////////////////////////////////////
bool FEND_home(){
 APP_digitalClock();
 APP_drawText(String(battery)+'%', 3, 3, 4); 
 return false;
}

bool showFirmwareVerOnce = false;
bool dontDetect = false;
bool FEND_showFirmwareVer(uint8_t x, uint8_t y){
  if(!showFirmwareVerOnce){
    showFirmwareVerOnce = true;
    OTA_currentVersion(3,30);
  }
  if(hold && !dontDetect){
    APP_drawText("hold "+String(x)+" "+String(y), 3, 70, 23);
    if(x>3 && x<APP_getWidth(13) && y<50 && y >14)
    {
        APP_drawText("detecting Version...", 3, 70, 23);
       if(OTA_FirmwareVersionCheck())
       {
        dontDetect = true;
        APP_drawText("Ver "+NewFirmwareVer+" available", 3, 70, 23);
       }else
       {
         APP_drawText("Firmware upto date.", 3, 70, 23);
       }
    }
  }
  if(dontDetect && tap){
    OTA_updateFirmware();
  }
  return false;
}
bool FEND_batteryIcon()
{
  return true;
}

bool FEND_menuIcon()
{

  return true;
}

///////////////////--Main Thread--//////////////////////////////
void FEND_begin(int x, int y)
{

 if(!FEND_task[0])
 FEND_task[0] = FEND_home();

 if(!FEND_task[1])
 FEND_task[1] = FEND_batteryIcon();

 if(!FEND_task[2])
 FEND_task[2] = FEND_menuIcon();

 if(!FEND_task[3])
 FEND_task[2] = FEND_showFirmwareVer(x,y);

}
