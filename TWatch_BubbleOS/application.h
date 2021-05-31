void APP_showAppList(){
  ttgo->tft->setTextSize(1);
  for(int i=0; i < APP_count; ++i)
  ttgo->tft->drawString(APP_list[i], 10, 50 + 10*i);
}

void APP_digitalClock(){
    ttgo->tft->setTextSize(2);
    ttgo->tft->drawString(String(ttgo->rtc->formatDateTime()), 70, 3);
}

void APP_drawText(String text, uint8_t x, uint8_t y, uint8_t maxChar = -1, 
                  uint8_t fontSize = 2, uint16_t color = TFT_WHITE, uint16_t background = TFT_BLACK){
  uint8_t h = 7*fontSize;
  uint8_t w = 5*fontSize;
  uint8_t charCount = text.length();
  uint8_t rightX = (charCount+1)*w-5;
  uint8_t width = (maxChar-charCount)*w+9 + rightX > 240 ? 240 : (maxChar-charCount)*w+9;   
  if(maxChar > 0) ttgo->tft->fillRect(rightX , y, width , h , background);
  
  ttgo->tft->setTextColor(color, background);
  ttgo->tft->drawString(text, x, y); 
}
