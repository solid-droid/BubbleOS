void FEND_clock(){
    RTC_Date currentTime = ttgo->rtc->getDateTime();
    //////////////////////
    APP_clearHourNeedle(currentTime.hour);
    APP_clearMinutesNeedle(currentTime.minute);
    APP_clearSecondsNeedle(currentTime.second);
    /////////////////////
    APP_drawHourNeedle(currentTime.hour);
    APP_drawMinutesNeedle(currentTime.minute);
    APP_drawSecondsNeedle(currentTime.second);
    APP_drawClockCenter();
    
}

void FEND_wifi_ON()
{
  uint8_t x= ICON_WIFI[0], y = ICON_WIFI[1], w = ICON_WIFI[2], h= ICON_WIFI[3];
  ttgo->tft->pushImage(x, y,  w, h, wifi_4);
}

void FEND_wifi_connecting()
{
  uint8_t x= ICON_WIFI[0], y = ICON_WIFI[1], w = ICON_WIFI[2], h= ICON_WIFI[3];
    if(FEND_wifi_counter == 1) ttgo->tft->pushImage(x, y,  w, h, wifi_0);
    else if(FEND_wifi_counter == 2) ttgo->tft->pushImage(x, y,  w, h, wifi_1);
    else if(FEND_wifi_counter == 3) ttgo->tft->pushImage(x, y,  w, h, wifi_2);
    else if(FEND_wifi_counter == 4) ttgo->tft->pushImage(x, y,  w, h, wifi_3);
    else if(FEND_wifi_counter == 5) ttgo->tft->pushImage(x, y,  w, h, wifi_4);
    else FEND_wifi_counter = 0;
    FEND_wifi_counter++;
}

void FEND_wifi_OFF(){
 uint8_t x= ICON_WIFI[0], y = ICON_WIFI[1], w = ICON_WIFI[2], h= ICON_WIFI[3];
  ttgo->tft->pushImage(x, y,  w, h, wifi_0);
}

void FEND_bluetooth_ON(){
 uint8_t x= ICON_BT[0], y = ICON_BT[1], w = ICON_BT[2], h= ICON_BT[3];
  ttgo->tft->pushImage(x, y,  w, h, bluetooth_ON);
}

void FEND_bluetooth_OFF(){
 uint8_t x= ICON_BT[0], y = ICON_BT[1], w = ICON_BT[2], h= ICON_BT[3];
  ttgo->tft->pushImage(x, y,  w, h, bluetooth_OFF);
}
void FEND_battery_Icon(){
 uint8_t x= ICON_BATTERY[0], y = ICON_BATTERY[1], w = ICON_BATTERY[2], h= ICON_BATTERY[3];
    if(battery<10)ttgo->tft->pushImage(x, y,  w, h, battery_0);
    else if(battery<30)ttgo->tft->pushImage(x, y,  w, h, battery_1);
    else if(battery<50)ttgo->tft->pushImage(x, y,  w, h, battery_2);
    else if(battery<70)ttgo->tft->pushImage(x, y,  w, h, battery_3);
    else ttgo->tft->pushImage(x, y,  w, h, battery_4);
}


void FEND_menu_Icon(){
  uint8_t x=0, y =186, w = 63, h= 51;
  ttgo->tft->pushImage(x, y,  w, h, menu_icon);
}

void FEND_GPS_ON(){
 uint8_t x= ICON_GPS[0], y = ICON_GPS[1], w = ICON_GPS[2], h= ICON_GPS[3];
  ttgo->tft->pushImage(x, y,  w, h, gps_ON);
}

void FEND_GPS_OFF(){
 uint8_t x= ICON_GPS[0], y = ICON_GPS[1], w = ICON_GPS[2], h= ICON_GPS[3];
  ttgo->tft->pushImage(x, y,  w, h, gps_OFF);
}

void FEND_alarm_ON(){
 uint8_t x= ICON_ALARM[0], y = ICON_ALARM[1], w = ICON_ALARM[2], h= ICON_ALARM[3];
  ttgo->tft->pushImage(x, y,  w, h, alarm_ON);
}

void FEND_alarm_OFF(){
uint8_t x= ICON_ALARM[0], y = ICON_ALARM[1], w = ICON_ALARM[2], h= ICON_ALARM[3];
  ttgo->tft->pushImage(x, y,  w, h, alarm_OFF);
}

void FEND_network(){
 if(screenLoad)
 {
    currentMenuIndex =1;
    scrollMenuList[0] = "WiFi Cred";
    scrollMenuList[1] = "WiFi Web";
    scrollMenuList[2] = "WiFi Auto";
    scrollMenuList[3] = "";
    APP_scrollMenu();
 }
  if(touch){
   if(BEND_delay(500,3)) APP_updateScrollMenu(); 
 }
  if(BEND_srollMenuSelection()){
    clearScreen = true;
    FEND_screenBuilder();
 }
}

void FEND_power(){
 if(screenLoad)
 {
    currentMenuIndex =1;
    scrollMenuList[0] = "Reset";
    scrollMenuList[1] = "Sleep";
    scrollMenuList[2] = "Status";
    scrollMenuList[3] = "";
    APP_scrollMenu();
 }
  if(touch){
   if(BEND_delay(500,3)) APP_updateScrollMenu(); 
 }
  if(BEND_srollMenuSelection()){
    clearScreen = true;
    FEND_screenBuilder();
 }
}

void FEND_nav_alarm(){
 if(screenLoad)
 {
    currentMenuIndex =1;
    scrollMenuList[0] = "GPS";
    scrollMenuList[1] = "ESP-NOW";
    scrollMenuList[2] = "Watch";
    scrollMenuList[3] = "";
    APP_scrollMenu();
 }

 if(touch){
     if(BEND_delay(500,3)) APP_updateScrollMenu(); 
 }
 if(BEND_srollMenuSelection()){
    clearScreen = true;
    FEND_screenBuilder();
 }
}

bool FEND_cursorON = true;
void FEND_bubbleMenu(){
  
  if(screenLoad)
 {
    currentMenuIndex =1;
    scrollMenuList[0] = "About";
    scrollMenuList[1] = "App1";
    scrollMenuList[2] = "App2";
    scrollMenuList[3] = "";
    APP_scrollMenu();
 }
  if(touch){
   if(BEND_delay(500,3)) APP_updateScrollMenu(); 
 }
  if(BEND_srollMenuSelection()){
    clearScreen = true;
    FEND_screenBuilder();
 }

}

void FEND_WIFI_ICO(){
  switch(WIFI_STATUS){
    case 0 :  FEND_wifi_OFF();break;
    case 2 :  FEND_wifi_ON();break;
  }
}


void FEND_BT_ICO(){
  if(BT_STATUS){
    FEND_bluetooth_ON();
  } else {
    FEND_bluetooth_OFF();
  }
}


void FEND_GPS_ICO(){
   if(GPS_STATUS){
    FEND_GPS_ON();
  } else {
    FEND_GPS_OFF();
  }
}

void FEND_loadIcons(){
  FEND_WIFI_ICO();
  FEND_BT_ICO();
  FEND_GPS_ICO();
  FEND_menu_Icon();
  FEND_battery_Icon();
  FEND_alarm_OFF();
}
