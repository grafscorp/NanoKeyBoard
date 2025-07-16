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
    constexpr uint16_t SERIAL_TIMEOUT_MS = 100;

    //Potenciometer
    constexpr uint8_t VolumePotPin = A0;         // Аналоговый пин
    constexpr uint16_t VolumeReadInterval = 50;  // Интервал чтения (мс)
    constexpr uint8_t VolumeChangeThreshold = 3; // Порог изменения (%)

}


namespace ButtonsConfig{
    constexpr uint8_t BUTTON_PINS[] = {2,3,4,5};
    constexpr uint8_t BUTTONS_COUNT = sizeof(BUTTON_PINS);
}



#endif