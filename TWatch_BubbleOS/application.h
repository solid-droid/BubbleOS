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
    ttgo->tft->setTextColor(TFT_DARKGREY , TFT_BLACK);
    ttgo->tft->drawString(String(ttgo->rtc->formatDateTime()), 70, 3);
}

uint16_t prevSecond =0;
//////////////////////////////////////////////////////////
void APP_clearSecondsNeedle(uint8_t seconds){
  APP_clearRotatedImage(120,120,7,73, 6* (prevSecond-15<0?prevSecond+45:prevSecond-15),25);
}

bool prev_min_temp = false;
bool prev_hour_temp = false;
void APP_clearHourNeedle(uint8_t hour){
    hour = hour>12?hour-12:hour;
   if(hour > FEND_hour || (hour == 0 && !prev_hour_temp) ) {
      if(hour == 0){
        prev_hour_temp = true;
      } else{
         prev_hour_temp = false;
      }
      APP_clearRotatedImage(120,120,20,55, 30*(FEND_hour-3<0? FEND_hour+9: FEND_hour-3),25);
    }
}

void APP_clearMinutesNeedle(uint8_t minutes){
    if(minutes > FEND_minutes || (minutes==0 && !prev_min_temp)){
       if(minutes == 0){
        prev_min_temp = true;
      }else{
        prev_min_temp = false;
      }
      APP_clearRotatedImage(120,120,20,76,6*(FEND_minutes-15<0?FEND_minutes+45:FEND_minutes-15),25);
    }    
}
//////////////////////////////////////////////////////////
void APP_drawSecondsNeedle(uint8_t seconds){
  APP_rotateImage(120,120,7,73,needle_seconds, 6*(seconds-15<0?seconds+45:seconds-15),25);
  prevSecond = seconds;
}

void APP_drawHourNeedle(uint8_t hour){
   if(hour > FEND_hour || hour == 0 ) {
      FEND_hour = hour;
    }
    APP_rotateImage(120,120,20,55,needle_hour, 30*(hour-3<0? hour+9: hour-3), 25);
}

void APP_drawMinutesNeedle(uint8_t minutes){
    if(minutes > FEND_minutes || minutes==0){
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
  maxChar = maxChar <= 0 ? (charCount > 0 ? charCount : 1 ): maxChar;
  uint8_t rightX = x + charCount*w;
  uint8_t width = (maxChar-charCount)*w; 
  ttgo->tft->fillRect(rightX , y, width , h , background);
  ttgo->tft->setTextSize(fontSize);
  ttgo->tft->setTextColor(color, background);
  ttgo->tft->drawString(text, x, y); 
}

void APP_drawTextCenter(String text, uint8_t x=0, uint8_t y=100, uint8_t fontSize = 2, uint8_t w=240){

   uint8_t newX = x + w/2 - (text.length()*6*fontSize)/2;
   ttgo->tft->setTextSize(fontSize);
   ttgo->tft->drawString(text, newX, y); 
}


void APP_remainingTime(){
// APP_drawText(String(batteryVolt, 1)+"V",140 ,220,4 , 2, TFT_DARKGREY);
 APP_drawText(String(battery < 100 ? battery : 100)+"%",140 ,220,4 , 2, TFT_DARKGREY);
  
}

void APP_scrollMenu(){

  ttgo->tft->fillRect(50 , 40, 160 , 20 , TFT_BLACK);
  ttgo->tft->fillRect(0 , 100,  240 , 40 , TFT_BLACK);
  ttgo->tft->fillRect(50 , 180, 160 , 20 , TFT_BLACK);
 ////////////////////
 if(scrollMenuList[currentMenuIndex]=="") currentMenuIndex -=1;
////////////////////
 if(currentMenuIndex!=0){ 
    ttgo->tft->setTextColor(TFT_DARKGREY , TFT_BLACK);
    APP_drawTextCenter(scrollMenuList[currentMenuIndex-1], 0 , 40, 2);
 }
    ttgo->tft->setTextColor(TFT_WHITE, TFT_BLACK);
    APP_drawTextCenter(scrollMenuList[currentMenuIndex], 0 , 100, 4);
    ttgo->tft->drawRoundRect(5, 85 , 225 , 60, 20 , TFT_WHITE);
///////////////////
    ttgo->tft->setTextColor(TFT_DARKGREY , TFT_BLACK);
    APP_drawTextCenter(scrollMenuList[currentMenuIndex+1], 0 , 180, 2);
}
///////////////////////////////////////////////////////////////////////
void APP_scrollMenu_UP(){
    currentMenuIndex+=1;
    APP_scrollMenu();
 }
void APP_scrollMenu_DOWN(){
    if(currentMenuIndex!=0){
      currentMenuIndex-=1;
      APP_scrollMenu();
   }
 }
void APP_updateScrollMenu (){
     if(touchPoint[1]+20<touchY){
        APP_scrollMenu_DOWN();
      }else if(touchPoint[1]-20>touchY){
        APP_scrollMenu_UP();
      }
}

void APP_drawButton(String text, uint8_t rectX=10 , uint8_t posY=100 , uint16_t color = TFT_WHITE)
{
   uint8_t textSize = text.length()*12;  
   uint8_t rectY = posY-18;
   uint8_t rectW = textSize+27;
   uint8_t rectH = 14+27;
   ttgo->tft->setTextSize(2);
   ttgo->tft->setTextColor(TFT_WHITE);
   ttgo->tft->fillRoundRect(rectX , rectY , rectW , rectH , 5 , 0x0394);
   if(rectX==0)
   APP_drawTextCenter(text, rectX , posY-3, 2);
   else
   ttgo->tft->drawString(text, rectX+12, posY-3); 
   ttgo->tft->drawRoundRect(rectX -2 , rectY-2 , rectW+3 , rectH+3 , 10 , color);
}

bool APP_tapEvent(uint8_t x, uint8_t y, uint8_t w = 235, uint8_t h = 50){
  if(tap && touchX > x && touchX < (x+w) && touchY > y && touchY < (y+h)){
    return true;
  } 
  return false;
}


void APP_drawImage(const unsigned short *img, uint8_t x, uint8_t y, uint8_t w,  uint8_t h){
  ttgo->tft->pushImage(x, y,  w, h, img);
}
void APP_clearImage(uint8_t x, uint8_t y, uint8_t w,  uint8_t h){
  ttgo->tft->fillRect(x, y,  w, h, TFT_BLACK);
}
