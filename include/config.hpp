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
//Buttons size
constexpr uint8_t BUTTONS_SIZE = sizeof(HardwareConfig::BUTTON_PINS);

//Buttons actions
enum ButtonDefault : uint8_t{
    TOGGLE_PLAY =1, //Button 1: On/off
    PREV_MEDIA,     //Button 2: Preview 
    NEXT_MEDIA,     //Button 3: Next
    CUSTOM_ACTION   //Button 4: Programming
};
    //Buttons debounce

    //Количество стабильных чтений для фиксации
    constexpr uint8_t DebounceThreshold=2;
    // Интервал между чтениями
    constexpr uint8_t DebounceIntervalMs=2;
    // Длительное нажатие (мс)
    constexpr uint8_t LongPressDuration=1000;
}


#endif