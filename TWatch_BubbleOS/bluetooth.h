// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

String getString( std::string value){
  String result = "";
  for (int i =0; i< value.length(); ++i){
    result += value[i]; 
  }
  return result;
}

class MyCallbacks: public BLECharacteristicCallbacks
{
  void onWrite(BLECharacteristic *pCharacteristic)
  {
    std::string value = pCharacteristic->getValue();

    if (value.length() > 0)
    {
       if(WifiCred == 0){
          
          WifiCred =1;
          getString(value).toCharArray(ssid, 30);
        }else if(WifiCred == 1){
          
          WifiCred = 2;
          getString(value).toCharArray(password, 30);
        }
      
       
      Serial.println();
    }
  }
};

void stopBluetooth()
{
  BT_STATUS = false;
  BLEDevice::deinit(false);
}

void startBluetooth(char* device = "BubbleOS")
{
  BT_STATUS = true;
  BLEDevice::init(device);
  BLEServer *pServer = BLEDevice::createServer();

  BLEService *pService = pServer->createService(SERVICE_UUID);

  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE
                                       );

  pCharacteristic->setCallbacks(new MyCallbacks());

  pCharacteristic->setValue("Hello World");
  pService->start();

  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->start();
}
