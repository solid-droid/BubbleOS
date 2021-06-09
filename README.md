# Bubble OS  
Smart Watch OS for lilygo T-Watch 2020 v2

> 05-06-2021 BubbleOS UI  

![ezgif-7-8682f3b8bb51](https://user-images.githubusercontent.com/32586986/120884801-f0f8bf00-c602-11eb-8271-17e863c66293.gif)

> Short Term Goals  
1. Pattern Recognition based keyboard
2. Widgets -> Button, toggle Button, Slider. Input box , Scroll Menu
3. Add custom apps in run-time
4. IOT related apps

> File Structure  

![file_struct_watch (2)](https://user-images.githubusercontent.com/32586986/120196580-a3183b80-c23d-11eb-99d6-1febd3c8f595.png)

## Logs:  
> 08-06-2021

1. Added JSON program extracter for external apps

> 06-06-2021

1. Added basic Menu Navigation
2. Added Veritical + Horizonal align center logic for text.
3. Added Basic Button Widget UI
4. Added Scrollable Menu

> 05-06-2021

1. Added Analog Clock
2. Image support for minute, second and hour needle. 

> 04-06-2021

1. Added image rotation.

> 03-06-2021

1. Removed FreeRTOS as its getting complicated with more functions.
2. Added custom interval scheduler (replacing FreeRTOS)
3. Added Battery, Wifi animated, bluetooth and menu icons

> 02-06-2021

1. Added FreeRTOS Multitasking

> 01-06-2021 

1. Added Battery remaining ON time calculation based on load current.
2. Clear and Draw text (to minimize flickering)

> 31-05-2021

1. Added Brightness Control on Top section swipe
2. Added autoDim , autoSleep, autoDeepSleep, touch to wakeup.

> 30-05-2021  

Just started development
1. Added RTC, power and touch controls
2. Added Tap, Drag, Hold, DragStart, DragEnd detection 
