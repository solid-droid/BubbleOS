void NETWORK_BT_SSID(){
      BT_STATUS = true;
      FEND_BT_ICO();
      startBluetooth();
      APP_drawText("Use bluetooth" , 10, 60);
      APP_drawText("Enter WiFi name:", 10, 90, 1);      
}

void NETWORK_BT_password(){
      APP_drawText(ssid, 10, 110, 1);
      APP_drawText("Enter WiFi password: ", 10, 130, 1);
}

void NETWORK_BT_Connect(){
  APP_drawText(password, 10, 150, 1);
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
  if(NP_wifi_cred){
   NP_wifi_cred = false;
   NP_BT = 0;
   APP_drawTextCenter("WiFi Cred", 0, 60, 3);
   BEND_readWifiCred();
   APP_drawTextCenter(String(ssid), 0, 90);
   APP_drawTextCenter(String(password), 0, 110);
   APP_drawButton(" RESET ", 50 , 150 , TFT_RED);
  }
  if(APP_tapEvent(50, 100, 160)){
      NP_BT = true;
      WifiCred = 0;
      NETWORK_BT_SSID();
  }
  if(NP_BT && WifiCred == 1){
    NETWORK_BT_password();
  }
  if(NP_BT && WifiCred == 2){
    NETWORK_BT_Connect();
    delay(1000);
    NP_BT = false;
    NP_wifi_cred = true;
  }
}

void NETWORK_PAGE_WIFI_ON(){
      ttgo->tft->fillRect(0 , 100, 237 , 60 , TFT_BLACK);
      APP_drawButton(" OFF ", 10 , 130 , TFT_RED);
      APP_drawText(" ON", 150, 127 , 4);
      APP_drawTextCenter("            ", 0 , 170);
      NP_wifi_off = false;
      NP_wifi_on = true;
}

void NETWORK_PAGE_WIFI_OFF(){
      ttgo->tft->fillRect(0 , 100, 237 , 60 , TFT_BLACK);
      APP_drawButton(" ON ", 138 , 130 ,TFT_GREEN);
      APP_drawText(" OFF", 22 , 127 , 5);
      APP_drawTextCenter("192.168.1.11", 0 , 170);
      NP_wifi_on = false;
      NP_wifi_off = true;
}

void NETWORK_PAGE_WIFI(){
  APP_drawTextCenter("WebServer", 0, 60, 3);
  if(WIFI_STATUS == 0){
    if(NP_wifi_off){
     NETWORK_PAGE_WIFI_ON();
    }
    if(APP_tapEvent(120, 90, 110)){
      ttgo->tft->fillRect(0 , 100, 237 , 60 , TFT_BLACK);
      if(WifiCred == 2){
        APP_drawTextCenter("Connecting...", 0 , 120);
        BEND_readWifiCred();
        if(BOOT_connectWiFi()){
          WIFI_STATUS = 2;
        }else {
          NETWORK_PAGE_WIFI_ON();     
        }
      } else {
        APP_drawTextCenter("No WiFi Cred.", 0 , 120);
        delay(2000);
        NP_wifi_off = true;
      }
      FEND_loadIcons();
    }
  
  } else if(WIFI_STATUS == 2) {
    if(NP_wifi_on){
     NETWORK_PAGE_WIFI_OFF();
    }
    if(APP_tapEvent(0, 90 , 110)){
      WiFi.disconnect();
      WIFI_STATUS = 0;
      FEND_loadIcons();
    }
    
  }
  
}
