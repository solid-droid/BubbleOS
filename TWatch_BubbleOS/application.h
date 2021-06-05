void APP_rotateImage(uint8_t x,uint8_t y, uint8_t w , uint8_t h, const unsigned short *image , uint16_t angle , uint8_t offset=0) {
float  cosa = cos(angle * DEG2RAD), sina = sin(angle * DEG2RAD);
uint8_t newx, newy;
uint8_t Woffset = w/2;
for(int i=0, index=0; i< h; ++ i)
  for(int j=0; j< w; ++ j)
   {
    newx = x + (i+offset)*cosa  - (j-Woffset)*sina;
    newy = y + (j-Woffset)*cosa  +  (i+offset)*sina;
    ttgo->tft->drawPixel(newx, newy, image[index++]);
   }
}

void APP_clearRotatedImage(uint8_t x,uint8_t y, uint8_t w , uint8_t h, uint16_t angle , uint8_t offset=0, uint16_t background = TFT_BLACK ) {
float  cosa = cos(angle * DEG2RAD), sina = sin(angle * DEG2RAD);
uint16_t newx, newy;
uint8_t Woffset = w/2;
for(int i=0; i< h; ++ i)
  for(int j=0; j< w; ++ j)
   {
    newx = x + (i+offset)*cosa  - (j-Woffset)*sina;
    newy = y + (j-Woffset)*cosa  +  (i+offset)*sina;
    ttgo->tft->drawPixel(newx, newy, background);
   }
}
//////////////////////////////////////////////////////////////////////////////
void APP_drawClockCenter(){
  ttgo->tft->fillCircle(120, 120, 20, TFT_ORANGE );
}

void APP_showAppList(){
  ttgo->tft->setTextSize(1);
  for(int i=0; i < APP_count; ++i)
  ttgo->tft->drawString(APP_list[i], 10, 50 + 10*i);
}

void APP_digitalClock(){
    ttgo->tft->setTextSize(2);
    ttgo->tft->drawString(String(ttgo->rtc->formatDateTime()), 70, 3);
}

uint16_t prevSecond =0;
void APP_drawSecondsNeedle(uint8_t seconds){
  APP_clearRotatedImage(120,120,7,73, 6* (prevSecond-15<0?prevSecond+45:prevSecond-15),25);
  APP_rotateImage(120,120,7,73,needle_seconds, 6*(seconds-15<0?seconds+45:seconds-15),25);
  prevSecond = seconds;
}

void APP_drawHourNeedle(uint8_t hour){
    hour = hour>12?hour-12:hour;
   if(hour > FEND_hour || hour == 0 ) {
      APP_clearRotatedImage(120,120,20,55, 30*(FEND_hour-3<0? FEND_hour+9: FEND_hour-3),25);
      FEND_hour = hour;
    }
    APP_rotateImage(120,120,20,55,needle_hour, 30*(hour-3<0? hour+9: hour-3), 25);
}

void APP_drawMinutesNeedle(uint8_t minutes){
    if(minutes > FEND_minutes || minutes==0){
      APP_clearRotatedImage(120,120,20,76,6*(FEND_minutes-15<0?FEND_minutes+45:FEND_minutes-15),25);
      FEND_minutes = minutes;  
    }
    APP_rotateImage(120,120,20,76,needle_minutes,6*(minutes-15<0?minutes+45:minutes-15),25);
    
}

uint8_t APP_getWidth(uint8_t len, uint8_t fontSize=2){
  return  len*6*fontSize; 
}

void APP_drawText(String text, uint8_t x, uint8_t y, uint8_t maxChar = -1, 
                  uint8_t fontSize = 2, uint16_t color = TFT_WHITE, uint16_t background = TFT_BLACK){
  uint8_t h = 7*fontSize;
  uint8_t w = 6*fontSize;
  uint8_t charCount = text.length();
  uint8_t rightX = x + charCount*w;
  uint8_t width = (maxChar-charCount)*w; 
  if(maxChar > 0) ttgo->tft->fillRect(rightX , y, width , h , background);
  
  ttgo->tft->setTextColor(color, background);
  ttgo->tft->drawString(text, x, y); 
}
