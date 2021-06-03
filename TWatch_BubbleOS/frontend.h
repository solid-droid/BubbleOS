void FEND_clock(){
   APP_digitalClock();
}
void FEND_wifi_connected()
{
  uint8_t x= 190, y = 216, w = 21, h= 22;
  ttgo->tft->pushImage(x, y,  w, h, wifi_4);
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
