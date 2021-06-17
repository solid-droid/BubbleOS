void testScreen(){
  screenData = "{\"widgets\":[{\"type\":0,\"text\":\"button1\",\"posY\":1,\"screen\":0,\"event\":\"backgroundRed\"},{\"type\":0,\"text\":\"button2\",\"posY\":2,\"screen\":0,\"event\":\"backgroundBlue\"}],\"events\":{\"backgroundBlue\":[\"Bcolor\",\"053F\"],\"backgroundRed\":[\"Bcolor\",\"F80B\"]}}";
  
}
struct eventhandler{
  String event;
  int posY;
  uint8_t type;
};
///////////////--Widgets--///////////////
void FEND_SB_drawButton(String text, uint8_t posY=0 , uint8_t posX=0 , bool grid= true)
{
 uint8_t y=0, fontSize =2, textSize = text.length()*6*fontSize;
 uint8_t rectX = 102 - textSize/2;
 
 if(grid){
 if     (posY==0) y=20;
 else if(posY==1) y=80;
 else if(posY==2) y=140;
 else if(posY==3) y=190;
 } else {
  y = posY;
  rectX = posX;
 }
 
uint8_t rectY = y-18;
uint8_t rectW = textSize+27;
uint8_t rectH = 7*fontSize+27;

  if(posX!=0 && grid){
    if(posX==1)rectX = 5;
    if(posX==2)rectX = 60;
    if(posX==3)rectX = 130;
    if(posX==4)rectX = 190;
  }
 
 ttgo->tft->setTextColor(TFT_WHITE);
 ttgo->tft->fillRoundRect(rectX , rectY , rectW , rectH , 5 , 0x0394);
 if(posX==0)
 APP_drawTextCenter(text, posX , y-3, fontSize);
 else
 ttgo->tft->drawString(text, rectX+12, y-3); 
 ttgo->tft->drawRoundRect(rectX -2 , rectY-2 , rectW+3 , rectH+3 , 10 , TFT_WHITE);
}
///////////////////--pattern keyboard--////////////////////////
void FEND_SB_drawGrid(uint8_t dim=5, uint8_t gap=35){
  for(uint8_t i =0; i < dim+1 ;++i)
  {
    ttgo->tft->drawFastHLine(55, 45 + i*gap, 175, TFT_DARKGREY);
    ttgo->tft->drawFastVLine(55 + i*gap, 45, 175, TFT_DARKGREY);
  }
}

bool FEND_SB_pixeArray[5][5];
uint8_t FEND_SB_prevPixel[2] = {6,6};
void FEND_SB_beginKeyBoard(){
   uint8_t fontSize =2;
   showNetworkIcon = false;
   showAlarmGPSIcon = false;
   showBatteryIcon  = false;
   
   APP_clearImage(ICON_BATTERY[0],ICON_BATTERY[1], ICON_BATTERY[2],  ICON_BATTERY[3]);
   APP_clearImage(ICON_WIFI[0],ICON_WIFI[1], ICON_WIFI[2],  ICON_WIFI[3]);
   APP_clearImage(ICON_BT[0],ICON_BT[1], ICON_BT[2],  ICON_BT[3]);
   APP_clearImage(ICON_GPS[0],ICON_GPS[1], ICON_GPS[2],  ICON_GPS[3]);
   APP_clearImage(ICON_ALARM[0],ICON_ALARM[1], ICON_ALARM[2],  ICON_ALARM[3]);

   for(uint8_t i=0; i< 5; ++i)
   for(uint8_t j=0; j< 5; ++j)
   FEND_SB_pixeArray[i][j] = false;
   
   ttgo->tft->setTextColor(TFT_DARKGREY);
   ttgo->tft->drawString("Draw A Pattern", 65 , 15);
   ttgo->tft->drawRoundRect(52 , 3 , 186 , 7*fontSize+27 , 10 , TFT_WHITE);

   FEND_SB_drawGrid();
   ttgo->tft->setTextColor(TFT_WHITE);
   FEND_SB_drawButton(">", 0, 1); //y, x, grid
   FEND_SB_drawButton("C", 1, 1);
   FEND_SB_drawButton("N", 2, 1);
}

bool preHold = false;
void FEND_SB_updateKeyBoard(){
  uint8_t newPixel[2] = {6,6};
  bool valid = false;
  bool value = false;
  if(touchX>55 && touchY>45)
    for(uint8_t i=1; i<=5; ++i)//finding row
      if(touchY < 45 + i*35) {
        for(uint8_t j=1; j<=5; ++j)//finding column
          if(touchX < 55 + j*35)
           {
            if((newPixel[0]!=FEND_SB_prevPixel[0] && newPixel[1]!=FEND_SB_prevPixel[1]) || FEND_SB_prevPixel[0]==6){
              valid = true;
              Serial.println(hold);
              value = !FEND_SB_pixeArray[j-1][i-1];
              FEND_SB_pixeArray[j-1][i-1] = preHold ? !value :  true;
              if(preHold && !hold){
                preHold = false;
              }
              newPixel[0]=j-1;
              newPixel[1]=i-1;
            }
            break;
           }
         if(valid) break;
      }
 if(valid && !preHold){
  if(hold){
    preHold = true;
    ttgo->tft->fillRect(55 + newPixel[0]*35 +1 , 45 + newPixel[1]*35 + 1 , 34, 34, value ? TFT_WHITE : TFT_BLACK);  
    FEND_SB_pixeArray[ newPixel[0]][ newPixel[1]] = value;
  } else{
    ttgo->tft->fillRect(55 + newPixel[0]*35 +1 , 45 + newPixel[1]*35 +1, 34, 34,TFT_WHITE);  
  }
     
 }
  FEND_SB_prevPixel[0] = newPixel[0];
  FEND_SB_prevPixel[1] = newPixel[1];
 }

 void FEND_SB_keyboard_clearChar(){
//  for(int i=0; i<keyboardCharIndex;++i)
//  ttgo->tft->drawPixel(keyboardChar[i][0], keyboardChar[i][1], TFT_BLACK);
//  FEND_SB_beginKeyBoard();
 }

///////////////////////////////
struct eventhandler FEND_SB_eventList[20];
uint8_t FEND_SB_eventCounter = 0;

void FEND_SB_drawWidgets(uint8_t screen = 0){
  FEND_SB_eventCounter = 0;
  for(uint8_t i=0; i<= screenJSON["widgets"].size(); i++)
  {
    if(screenJSON["widgets"][i]["screen"] == screen){
      switch(screenJSON["widgets"][i]["type"].as<int>())
      {
        case 0: //Button Widget
            FEND_SB_drawButton(screenJSON["widgets"][i]["text"],screenJSON["widgets"][i]["posY"]);
            FEND_SB_eventList[FEND_SB_eventCounter].event =screenJSON["widgets"][i]["event"].as<String>();
            FEND_SB_eventList[FEND_SB_eventCounter].posY = screenJSON["widgets"][i]["posY"].as<int>();
            FEND_SB_eventList[FEND_SB_eventCounter].type = screenJSON["widgets"][i]["type"].as<int>();
            FEND_SB_eventCounter++;
        break;
        case 1: //Toggle Button
        break;
        case 2: //Scroll Menu
        break;
        case 3: //Slider
        break;
        case 4: //animation event based or delay based.
        break;
      }
    }
  }
}

void FEND_SB_beginEvent(String event, uint8_t index){
  for(int i=0; i<screenJSON["events"][event].size();i++)
  {
    String command = screenJSON["events"][event][i];
    String value = screenJSON["events"][event][++i];
    if(command == "Bcolor")
    {
      char color[5];
      uint8_t k=0;
      color[k]=value[k++];
      color[k]=value[k++];
      color[k]=value[k++];
      color[k]=value[k];
      ttgo->tft->fillCircle(120, 120, 110,strtol(color, NULL, 16));
      FEND_SB_drawWidgets(0);
    }
  }

}

void FEND_SB_eventHandlers(){
if(tap){
  //button, toggleButton
  uint8_t posY;
 if(touchX>30 && touchX<230){ 
   if(touchY>10 && touchY<60)posY = 0;
   if(touchY>70 && touchY<120)posY = 1;
   if(touchY>130 && touchY<160)posY = 2;
   if(touchY>170)posY = 3;
   for(int i=0; i<FEND_SB_eventCounter; ++i)
   {
      if(posY == FEND_SB_eventList[i].posY)
      FEND_SB_beginEvent(FEND_SB_eventList[i].event, i);
   }
 }
} 
//if(drag){
//  //slider
//}

//if(BEND_srollMenuSelection()){
// //For Scroll Menu
//}

}

void FEND_screenBuilder(){
  currentScreen = 5;
  FEND_SB_eventHandlers();
  
 if(screenLoad && screenData!= "")
 {
  ttgo->tft->fillScreen(TFT_BLACK);
  FEND_loadIcons();
   DeserializationError error = deserializeJson(screenJSON, screenData);
  if (!error) {
   FEND_SB_drawWidgets();   
  }else
  {
     ttgo->tft->drawString("Error: "+String(error.f_str()), 10, 100);
  }
 }
}
