#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif

#define LILYGO_WATCH_2020_V2
#include <LilyGoWatch.h>
