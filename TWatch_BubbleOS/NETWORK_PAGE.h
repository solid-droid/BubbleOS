void NETWORK_BT_SSID(){
      BT_STATUS = true;
      FEND_loadIcons();
      startBluetooth();
      APP_drawText("Use bluetooth" , 10, 60);
      APP_drawText("Enter WiFi name:", 10, 90);      
}

void NETWORK_BT_password(){
      APP_drawText(ssid, 10, 110);
      APP_drawText("Enter WiFi pass:", 10, 130);
}

void NETWORK_BT_Connect(){
  APP_drawText(password, 10, 150);
  stopBluetooth();
  BT_STATUS = false;
  FEND_BT_ICO();
  BEND_storeWifiCred();
  APP_drawText("Connecting to Wifi", 10, 170 , 19);
  if(BOOT_connectWiFi()){
        WIFI_STATUS = 2;
        FEND_WIFI_ICO();
        APP_drawText("Connected", 10, 170 , 19);
   } else {
        WIFI_STATUS = 0;
        FEND_WIFI_ICO();
        APP_drawText("Not Connected", 10, 170 , 19); 
   }
}

bool NP_BT = false;
void NETWORK_PAGE_CRED() {
  if(Button_reset){
   Button_reset = false;
   NP_BT = 0;
   APP_drawTextCenter("WiFi Cred", 0, 60, 3);
   BEND_readWifiCred();
   APP_drawTextCenter(String(ssid), 0, 90);
   APP_drawTextCenter(String(password), 0, 110);
   APP_drawButton(" RESET ", 70 , 160 , TFT_RED);
  }
  if(!NP_BT && APP_tapEvent(70, 110, 160)){
      NP_BT = true;
      WifiCred = 0;
      savePower= false;
      ttgo->tft->fillScreen(TFT_BLACK);
      NETWORK_BT_SSID();
  }
  if(NP_BT && WifiCred == 1){
    NETWORK_BT_password();
  }
  if(NP_BT && WifiCred == 2){
    NETWORK_BT_Connect();
    delay(1000);
    NP_BT = false;
    Button_reset = true;
    ttgo->tft->fillScreen(TFT_BLACK);
    FEND_loadIcons();
    savePower= true;
  }
}

void NETWORK_PAGE_WIFI_ON(){
      ttgo->tft->fillRect(0 , 100, 237 , 60 , TFT_BLACK);
      APP_drawButton(" OFF ", 10 , 130 , TFT_RED);
      APP_drawText(" ON", 150, 127 , 4);
      APP_drawTextCenter("            ", 0 , 170);
      savePower= true;
      Button_off = false;
      Button_on = true;
}

void NETWORK_PAGE_WIFI_OFF(){
      ttgo->tft->fillRect(0 , 100, 237 , 60 , TFT_BLACK);
      APP_drawButton(" ON ", 138 , 130 ,TFT_GREEN);
      APP_drawText(" OFF", 22 , 127 , 5);
      APP_drawTextCenter("192.168.1.11", 0 , 170);
      savePower= false;
      Button_on = false;
      Button_off = true;
}

void NETWORK_PAGE_WIFI(){
  APP_drawTextCenter("WebServer", 0, 60, 3);
  if(WIFI_STATUS == 0){
    if(Button_off){
     NETWORK_PAGE_WIFI_ON();
    }
    if(APP_tapEvent(120, 90, 110)){
      ttgo->tft->fillRect(0 , 100, 237 , 60 , TFT_BLACK);
        APP_drawTextCenter("Connecting...", 0 , 120);
        BEND_readWifiCred();
        if(BOOT_connectWiFi()){
          WIFI_STATUS = 2;
        }else {
          APP_drawTextCenter("Check WiFi Cred.", 0 , 120);
          delay(1000);
          Button_off = true;
          NETWORK_PAGE_WIFI_ON();     
        }
      FEND_loadIcons();
    }
  } else if(WIFI_STATUS == 2) {
    if(Button_on){
     NETWORK_PAGE_WIFI_OFF();
    }
    if(APP_tapEvent(0, 90 , 110)){
      WiFi.disconnect();
      WIFI_STATUS = 0;
      FEND_loadIcons();
    }
    
  }
  
}

void NETWORK_WIFI_AUTO_ON(){
      ttgo->tft->fillRect(0 , 100, 237 , 60 , TFT_BLACK);
      APP_drawButton(" OFF ", 10 , 130 , TFT_RED);
      APP_drawText(" ON", 150, 127 , 4);
      Button_off = false;
      Button_on = true;
}

void NETWORK_WIFI_AUTO_OFF(){
      ttgo->tft->fillRect(0 , 100, 237 , 60 , TFT_BLACK);
      APP_drawButton(" ON ", 138 , 130 ,TFT_GREEN);
      APP_drawText(" OFF", 22 , 127 , 5);
      Button_on = false;
      Button_off = true;
}

void NETWORK_PAGE_WIFI_AUTO(){
  APP_drawTextCenter("Auto-Connect", 0, 70, 3);
   if(EEPROM.read(1)==1){
        if(Button_off){
         NETWORK_WIFI_AUTO_ON();
        }
        if(APP_tapEvent(120, 90, 110)){
          EEPROM.write(1,0);
          EEPROM.commit();
        }
  } else if(EEPROM.read(1)==0) {
        if(Button_on){
         NETWORK_WIFI_AUTO_OFF();
        }
        if(APP_tapEvent(0, 90 , 110)){
          EEPROM.write(1,1);
          EEPROM.commit();
        }
  }
}
