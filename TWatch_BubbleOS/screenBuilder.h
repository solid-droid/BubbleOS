void FEND_screenBuilder() {
 if(screenLoad)
 {
  ttgo->tft->fillScreen(TFT_BLACK);
  FEND_loadIcons();
  Serial.println(currentMenuIndex);
  Serial.println(currentScreen);
 }
 
   currentScreen = 5;
}

void FEND_SB_connectWiFi(){
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
    void FEND_BT_ICO();
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
