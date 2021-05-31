void APP_showAppList(){
  ttgo->tft->setTextSize(1);
  for(int i=0; i < APP_count; ++i)
  ttgo->tft->drawString(APP_list[i], 10, 50 + 10*i);
}

void APP_digitalClock(){
    ttgo->tft->setTextSize(2);
    ttgo->tft->drawString(String(ttgo->rtc->formatDateTime()), 70, 3);
}


void APP_drawBattery(){
  ttgo->tft->drawString(String(battery), 3, 3); 
}

void APP_drawText(uint8_t x, uint8_t y, int maxChar){
// ttgo->tft->fontsLoaded() 
  
}
