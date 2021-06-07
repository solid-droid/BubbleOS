void testScreen(){
  screenData = "{\"widgets\":[{\"type\":0,\"text\":\"button0\",\"posY\":0,\"screen\":0,\"event\":\"backgroundBlue\"},{\"type\":0,\"text\":\"button1\",\"posY\":1,\"screen\":0,\"event\":\"backgroundRed\"},{\"type\":0,\"text\":\"button2\",\"posY\":2,\"screen\":0,\"event\":\"backgroundRed\"}],\"events\":{\"backgroundBlue\":[\"Bcolor\",\"blue\"],\"backgroundRed\":[\"Bcolor\",\"red\"]}}";

}
struct eventhandler{
  String event;
  int posY;
  uint8_t type;
};
///////////////--Widgets--///////////////
void FEND_SB_drawButton(String text, uint8_t posY)
{
 uint8_t x=0, y=0, fontSize =2, textSize = text.length()*6*fontSize;
 
 if     (posY==0) y=40;
 else if(posY==1) y=100;
 else if(posY==2) y=160;
 else if(posY==3) y=210;
 
 ttgo->tft->setTextColor(TFT_WHITE);
 ttgo->tft->fillRoundRect(103 - textSize/2, y-17, textSize+25 , 7*fontSize+25 , 5 , 0x0394);
 APP_drawTextCenter(text, 0 , y-3, fontSize);
 ttgo->tft->drawRoundRect(100 - textSize/2, y-20 , textSize+30 , 7*fontSize+30 , 10 , TFT_WHITE);
}

///////////////////////////////
struct eventhandler FEND_SB_eventList[20];
uint8_t FEND_SB_eventCounter = 0;

void FEND_SB_beginEvent(String event){
  
}

void FEND_SB_eventHandlers(){
if(tap){
  //button, toggleButton
  uint8_t posY;
  if(touchY>40 && touchY<90)posY = 0;
  if(touchY>95 && touchY<145)posY = 1;
  if(touchY>150 && touchY<200)posY = 2;
  if(touchY>150 && touchY<200)posY = 3;
  
 for(int i=0; i<FEND_SB_eventCounter; ++i)
 {
    if(posY == FEND_SB_eventList[i].posY)
    FEND_SB_beginEvent(FEND_SB_eventList[i].event);
 }
}

//if(drag){
//  //slider
//}

//if(BEND_srollMenuSelection()){
// //For Scroll Menu
//}

}

void FEND_SB_drawWidgets(uint8_t screen = 0){
  ttgo->tft->fillScreen(TFT_BLACK);
  FEND_loadIcons();
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

void FEND_screenBuilder(){
  currentScreen = 5;
 if(screenLoad && screenData!= "")
 {
   DeserializationError error = deserializeJson(screenJSON, screenData);
  if (!error) {
   FEND_SB_drawWidgets();   
  }else
  {
     ttgo->tft->drawString("Error: "+String(error.f_str()), 10, 100);
  }
 }
  FEND_SB_eventHandlers();
}
