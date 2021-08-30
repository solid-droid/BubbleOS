# Bubble OS  
> Smart Watch OS for lilygo T-Watch 2020 v2  
 05-06-2021 BubbleOS UI  

![ezgif-7-8682f3b8bb51](https://user-images.githubusercontent.com/32586986/120884801-f0f8bf00-c602-11eb-8271-17e863c66293.gif)

# How To Install
1. Get lilygo T-Watch 2020 v2 ( any version is fine ).
2. Install Arduino IDE
3. ESP32 boards in Arduino IDE using (https://dl.espressif.com/dl/package_esp32_index.json), 
4. Install [ttygo library](https://github.com/Xinyuan-LilyGO/TTGO_TWatch_Library)
5. compile and upload the code ( watch will use your computer time as its current time)
6. use BLE Scanner app (android) to connect and set wifi SSID and password


# Short Term Goals  
1. Webserver -> set time, date , watch face
2. Webserver - upload to EEPROM -> watch face, apps
3. Webserver -> read IOT data, read GPS data, read usage data
4. ESP-NOW -> p2p IOT protocol for ESP32 / ESP8266
5. APP UI -> Senser data UI, buttons, slider, joystick
6. IOT over the internet

