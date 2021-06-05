void FEND_clock(){
    APP_digitalClock();
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
