void FEND_clock(){
//    APP_digitalClock();
    RTC_Date currentTime = ttgo->rtc->getDateTime();
    APP_drawHourNeedle(currentTime.hour);
    APP_drawMinutesNeedle(currentTime.minute);
    APP_drawSecondsNeedle(currentTime.second);
    APP_drawClockCenter();
    
}

void FEND_wifi_connected()
{
  uint8_t x= 190, y = 216, w = 21, h= 22;
  ttgo->tft->pushImage(x, y,  w, h, wifi_4);
}

uint8_t FEND_wifi_counter = 0;
void FEND_wifi_connecting()
{
  uint8_t x= 190, y = 216, w = 21, h= 22;
    if(FEND_wifi_counter == 1) ttgo->tft->pushImage(x, y,  w, h, wifi_0);
    else if(FEND_wifi_counter == 2) ttgo->tft->pushImage(x, y,  w, h, wifi_1);
    else if(FEND_wifi_counter == 3) ttgo->tft->pushImage(x, y,  w, h, wifi_2);
    else if(FEND_wifi_counter == 4) ttgo->tft->pushImage(x, y,  w, h, wifi_3);
    else if(FEND_wifi_counter == 5) ttgo->tft->pushImage(x, y,  w, h, wifi_4);
    else FEND_wifi_counter = 0;
    FEND_wifi_counter++;;
}

void FEND_bluetooth_ON(){
  uint8_t x= 213, y = 178, w = 22, h= 28;
  ttgo->tft->pushImage(x, y,  w, h, bluetooth_ON);
}

void FEND_bluetooth_OFF(){
  uint8_t x= 213, y = 178, w = 22, h= 28;
  ttgo->tft->pushImage(x, y,  w, h, bluetooth_OFF);
}
void FEND_battery_Icon(){
  uint8_t x=0, y =0, w = 43, h= 42;
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
  uint8_t x=190, y =5, w = 22, h= 23;
  ttgo->tft->pushImage(x, y,  w, h, gps_ON);
}

void FEND_GPS_OFF(){
  uint8_t x=190, y =5, w = 22, h= 23;
  ttgo->tft->pushImage(x, y,  w, h, gps_OFF);
}

void FEND_alarm_ON(){
  uint8_t x=203, y =24, w = 35, h= 34;
  ttgo->tft->pushImage(x, y,  w, h, alarm_ON);
}

void FEND_alarm_OFF(){
  uint8_t x=203, y =24, w = 35, h= 34;
  ttgo->tft->pushImage(x, y,  w, h, alarm_OFF);
}

void FEND_network(){
 if(screenLoad)
 {
    APP_button("Storage", 0 , 50);
    APP_button("WiFi", 0 , 100);
    APP_button("Bluetooth", 0 , 150);
 }
}

void FEND_power(){
 if(screenLoad)
 {
    APP_button("Modes", 0 , 50);
    APP_button("Status", 0 , 100);
    APP_button("Sleep", 0 , 150);
 }
}

void FEND_nav_alarm(){
 if(screenLoad)
 {
    APP_button("GPS", 0 , 50);
    APP_button("Time", 0 , 100);
    APP_button("Alarm", 0 , 150);
 }
}

void FEND_bubbleMenu(){
//  if(BEND_delay(1000,3))APP_digitalClock();
   if(screenLoad)
 {
    ttgo->tft->drawCircle(120,120,110, TFT_WHITE);
 }
}

void FEND_setTimeMenu(){

}

void FEND_loadIcons(){
  FEND_bluetooth_OFF();
  FEND_wifi_connected();
  FEND_menu_Icon();
  FEND_battery_Icon();
  FEND_GPS_OFF();
  FEND_alarm_OFF();
}
