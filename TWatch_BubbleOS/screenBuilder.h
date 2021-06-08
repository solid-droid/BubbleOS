void testScreen(){
  screenData = "{\"widgets\":[{\"type\":0,\"text\":\"button1\",\"posY\":1,\"screen\":0,\"event\":\"backgroundRed\"},{\"type\":0,\"text\":\"button2\",\"posY\":2,\"screen\":0,\"event\":\"backgroundBlue\"}],\"events\":{\"backgroundBlue\":[\"Bcolor\",\"053F\"],\"backgroundRed\":[\"Bcolor\",\"F80B\"]}}";
  
}
struct eventhandler{
  String event;
  int posY;
  uint8_t type;
};
///////////////--Widgets--///////////////
void FEND_SB_drawButton(String text, uint8_t posY=0 , uint8_t posX=0)
{
 uint8_t x=0, y=0, fontSize =2, textSize = text.length()*6*fontSize;
 
 if     (posY==0) y=40;
 else if(posY==1) y=100;
 else if(posY==2) y=160;
 else if(posY==3) y=210;

uint8_t rectX = 102 - textSize/2;
uint8_t rectY = y-18;
uint8_t rectW = textSize+27;
uint8_t rectH = 7*fontSize+27;

  if(posX!=0){
    if(posX==1)rectX = 0;
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
void FEND_SB_beginKeyBoard(){
   uint8_t fontSize =2;
   ttgo->tft->setTextColor(TFT_DARKGREY);
   APP_drawTextCenter("Draw A Character", 0 , 97, fontSize);
   ttgo->tft->drawRoundRect(2 , 83 , 236 , 7*fontSize+27 , 10 , TFT_WHITE);
   ttgo->tft->setTextColor(TFT_WHITE);
   FEND_SB_drawButton("X", 0, 2);
   FEND_SB_drawButton(">", 0, 3);
}

void FEND_SB_updateKeyBoard(){
  if(keyboardCharIndex<50000){
  keyboardChar[keyboardCharIndex][0]=touchX;
  keyboardChar[keyboardCharIndex++][1]=touchY;
  ttgo->tft->drawPixel(touchX, touchY, TFT_WHITE);
  }
 }

 void FEND_SB_keyboard_clearChar(){
  for(int i=0; i<keyboardCharIndex;++i)
  ttgo->tft->drawPixel(keyboardChar[i][0], keyboardChar[i][1], TFT_BLACK);
  FEND_SB_beginKeyBoard();
  keyboardCharIndex=0;
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
   if(touchY>40 && touchY<90)posY = 0;
   if(touchY>95 && touchY<145)posY = 1;
   if(touchY>150 && touchY<200)posY = 2;
   if(touchY>205)posY = 3;
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
