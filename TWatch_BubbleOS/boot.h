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

void BOOT_init_loadingScreen(){
  byte xpos = 40; // Stating position for the display
  byte ypos = 90;
  ttgo->tft->fillScreen(TFT_BLACK);
  ttgo->tft->setSwapBytes(true);
  ttgo->tft->setTextSize(2);
  ttgo->tft->setTextColor(0x6E2B, TFT_BLACK);
  ttgo->tft->drawString("Loading...", xpos, ypos);
}

void BOOT_powerButton(){
   pinMode(AXP202_INT, INPUT_PULLUP);
   attachInterrupt(AXP202_INT, [] {irq = true;}, FALLING);
   ttgo->power->enableIRQ(AXP202_PEK_SHORTPRESS_IRQ, true);
   ttgo->power->clearIRQ();
   ttgo->power->setPowerOutPut(AXP202_DCDC2, false);
}

void BOOT_connectWiFi(){
    WiFi.begin(ssid, password);
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
//  BOOT_init_loadingScreen();
  BOOT_beginSystem();
  BOOT_powerButton();
  BOOT_power(SYS_devices[1], false);
  BOOT_RTC();
  BOOT_SD();
}
