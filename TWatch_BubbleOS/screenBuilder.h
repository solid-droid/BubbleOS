

uint8_t SB_Main;
uint8_t SB_Sub;

void FEND_SB_apps(){
  
}
void FEND_SB_network(){
  switch(SB_Sub){
    case 0: NETWORK_PAGE_CRED();break;
    case 1: NETWORK_PAGE_WIFI();break;
    case 2: NETWORK_PAGE_WIFI_AUTO();break;
  }
}
void FEND_SB_power(){
  
}
void FEND_SB_alarmGPS(){
  
}
void FEND_SB_PageType(){

  switch(SB_Main){
    case 1 : FEND_SB_network(); break;
    case 2 : FEND_SB_power(); break; 
    case 3 : FEND_SB_alarmGPS(); break; 
    case 4 : FEND_SB_apps(); break;  
    
  }
  
}


void FEND_screenBuilder() {
 if(currentScreen!=5){
  //pre Load
    SB_Sub  = currentMenuIndex;
    SB_Main = currentScreen;
 }
 if(screenLoad)
 {
  //1st load
  ttgo->tft->fillScreen(TFT_BLACK);
  FEND_loadIcons();
  //reset button redraw escape Variables <==avoids flickering
  Button_on  = true;
  Button_off = true;
  Button_reset = true;
 
 } else if (currentScreen == 5){
  //loop Load
  FEND_SB_PageType();
 }

 
   currentScreen = 5;
}

void FEND_SB_connectWiFi(bool force = false){
  if(EEPROM.read(1)!=1 || force){
    if(reset){
      EEPROM.write(1,1);
      EEPROM.commit();
    }
    ttgo->tft->fillScreen(TFT_BLACK);
    FEND_WIFI_ICO();
    FEND_BT_ICO();
    FEND_GPS_ICO();
    WIFI_STATUS = 1;
    if(WifiCred == 0){
      BT_STATUS = true;
      FEND_BT_ICO();
      startBluetooth();
      APP_drawText("Use bluetooth" , 10, 10);
      APP_drawText("Enter WiFi name:", 10, 50);
      while(WifiCred == 0){
           FEND_wifi_connecting();
          delay(300);
      }
      
      APP_drawText(ssid, 10, 70);
      APP_drawText("Enter WiFi password: ", 10, 100);
      while(WifiCred == 1){
          FEND_wifi_connecting();
          delay(500);
      }
      
      APP_drawText(password, 10, 120);
      stopBluetooth();
      BT_STATUS = false;
      FEND_BT_ICO();
      BEND_storeWifiCred();
      APP_drawText("Connecting to Wifi", 10, 140 , 19);
      if(BOOT_connectWiFi()){
       WIFI_STATUS = 2;
       FEND_WIFI_ICO();
      APP_drawText("Connected", 10, 140 , 19);
      } else {
        WIFI_STATUS = 0;
        FEND_WIFI_ICO();
        APP_drawText("Not Connected", 10, 140 , 19); 
      }
   } else {
      BEND_readWifiCred();
      APP_drawTextCenter("Welcome Back  ", 10, 80, 3);
      APP_drawText("Connecting to Wifi", 10, 140 , 19);
      if(BOOT_connectWiFi()){
        WIFI_STATUS = 2;
        FEND_WIFI_ICO();
        APP_drawText("Connected", 10, 140 , 19);
      } else {
        WIFI_STATUS = 0;
        FEND_WIFI_ICO();
         APP_drawText("Not Connected", 10, 140 , 19); 
      }
   }
    delay(2000);
  }
  ttgo->tft->fillScreen(TFT_BLACK);
}

void FEND_SB_welcome(){
  ttgo->tft->fillScreen(TFT_BLACK);
  FEND_WIFI_ICO();
  FEND_BT_ICO();
  FEND_GPS_ICO();
  APP_drawTextCenter("Bubble OS  ", 10, 80, 3);
  APP_drawTextCenter("Loading..  ", 10, 130);
}
