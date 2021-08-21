void BOOT_power(String type, bool value = false)
{
  if(type == SYS_devices[0]){
    ttgo->power->setPowerOutPut(AXP202_LDO3, value);}
  else if (type == SYS_devices[1]){
    ttgo->power->setPowerOutPut(AXP202_LDO4, value);}
  else if (type == SYS_devices[2]){
    ttgo->power->setPowerOutPut(AXP202_LDO2, value);}
  else if (type == SYS_devices[3]){
    ttgo->power->setPowerOutPut(AXP202_EXTEN, value);
  }
}

void BOOT_setBrightness(int value, bool save = true)
{
  if(save) loadBrightness = value;
  ttgo->setBrightness(value);
}

void BOOT_clearIRQ()
{
     irq = false;
     ttgo->power->clearIRQ();
}

void BOOT_deepSleep()
{
  WiFi.disconnect();
  ttgo->tft->fillScreen(TFT_BLACK);
  ttgo->power->setPowerOutPut(AXP202_DCDC2, false);
  esp_sleep_enable_ext1_wakeup(GPIO_SEL_35, ESP_EXT1_WAKEUP_ALL_LOW);
  esp_deep_sleep_start(); 
}

void BOOT_beginSystem(){
  ttgo->rtc->check();
  ttgo->rtc->syncToSystem();
  ttgo->openBL(); 
}

bool BOOT_deviceStatus(String type)
{
  if ( type == SYS_devices[0]){
    return ttgo->power->isLDO3Enable();
  }
  if ( type == SYS_devices[1]){
    return ttgo->power->isLDO4Enable();
  }
  if ( type == SYS_devices[2]){
    return ttgo->power->isLDO2Enable();
  }
}

void BOOT_powerButton(){
   pinMode(AXP202_INT, INPUT_PULLUP);
   attachInterrupt(AXP202_INT, [] {irq = true;}, FALLING);
   ttgo->power->enableIRQ(AXP202_PEK_SHORTPRESS_IRQ, true);
   ttgo->power->clearIRQ();
   ttgo->power->setPowerOutPut(AXP202_DCDC2, false);
}

bool BOOT_connectWiFi(){
    if (!WiFi.config(local_IP, gateway, subnet)) {
      Serial.println("STA Failed to configure");
    }
    WiFi.disconnect();
    Serial.println(ssid);
    Serial.println(password);
    WiFi.begin(ssid, password);
    uint8_t _count = 0;
    while (WiFi.status() != WL_CONNECTED && _count < 30) {
        delay(500);
        FEND_wifi_connecting();
        Serial.print(".");
        _count++;
    }
    Serial.println(".");
    if(_count>= 30){
      Serial.println("WiFi connection Failed");
    } else {
      Serial.println("WiFi connected");
      Serial.print("IP address: ");
      Serial.println(WiFi.localIP());
      server.begin();
      Serial.println("TCP server started");
    }

    return _count < 30 ? true : false;
}

void BOOT_RTC(){
 pinMode(RTC_INT_PIN, INPUT_PULLUP);
    attachInterrupt(RTC_INT_PIN, [] {
        rtcIrq = 1;
    }, FALLING);

    ttgo->rtc->disableAlarm();
    ttgo->rtc->check();
    ttgo->rtc->syncToSystem();
}

void BOOT_getFileList() {
    File entry =  SD.open("/BubbleOS");
    if (entry) {
       APP_SD =  SD.open("/BubbleOS/Apps");
       SYS_SD =  SD.open("BubbleOS/System");
    }
    entry.close();
}

void BOOT_SD()
{
   if (ttgo->sdcard_begin()) {
    BOOT_getFileList();
   }
}
void BOOT() {
  BOOT_beginSystem();
  BOOT_powerButton();
  BOOT_power(SYS_devices[1], false);
  BOOT_RTC();
  BOOT_SD();
}
