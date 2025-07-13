#ifndef CONFIG_HPP
#define CONFIG_HPP
#include <Arduino.h>

namespace HardwareConfig{
//PINS CONFIGS
constexpr uint8_t BUTTON_PINS[] = {2,3,4,5};
constexpr uint8_t OLED_SDA = A4;
constexpr uint8_t OLED_SDL = A5;
//OLED DISPLAY CONFIGS
constexpr uint8_t OLED_WIDTH = 128;
constexpr uint8_t OLED_HEIGHT = 64;
constexpr uint8_t OLED_ADDRESS = 0x3C;

//SERIAL PORT
constexpr uint32_t SERIAL_BAUD_RATE = 115200;
}




namespace ButtonsConfig{
//Buttons
constexpr uint8_t BUTTONS_SIZE = 4;

//Buttons actions
enum ButtonDefault : uint8_t{
    TOGGLE_PLAY =1,
    PREV_MEDIA,
    NEXT_MEDIA,
    CUSTOM_ACTION
};
    constexpr uint8_t DebounceThreshold=2;
    constexpr uint8_t DebounceIntervalMs=2;
    constexpr uint8_t LongPressDuration=1000;
}


#endif