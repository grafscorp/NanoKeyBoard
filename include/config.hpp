#ifndef CONFIG_HPP
#define CONFIG_HPP
#include <Arduino.h>

namespace HardwareConfig{
    //PINS CONFIGS
    constexpr uint8_t BUTTON_PINS[] = {2,3,4,5};
    constexpr uint8_t OLED_SDA = A4;
    constexpr uint8_t OLED_SDL = A5;

    //Potenciometer
    constexpr uint8_t VolumePotPin = A0;         // Аналоговый пин
    constexpr uint16_t VolumeReadInterval = 50;  // Интервал чтения (мс)
    constexpr uint8_t VolumeChangeThreshold = 3; // Порог изменения (%)

}


namespace ButtonsConfig{
    //constexpr uint8_t BUTTON_PINS[] = {2,3,4,5};
    constexpr uint8_t BUTTONS_COUNT = sizeof(HardwareConfig::BUTTON_PINS);

}



namespace SerialConfig{
    //SERIAL PORT
    constexpr uint32_t SERIAL_BAUD_RATE = 115200;
    constexpr uint16_t SERIAL_TIMEOUT_MS = 100;

    
}

namespace ProtocolConfig{

    constexpr uint8_t START_BYTE = 0xff;
    constexpr uint8_t END_BYTE = 0xaa;
    constexpr uint8_t MAX_PAYLOAD = 64;
    
    enum COMMANDS : uint8_t{
        CMD_SYSTEM = 0x01,
        CMD_BUTTONS = 0x02,
        CMD_VOLUME = 0x03, 
        CMD_DISPLAY = 0x04
    };

    struct Packet
    {
        uint8_t length;
        COMMANDS cmds;
        uint8_t data;
    };
    
}
namespace DisplayConfig{
    //OLED DISPLAY CONFIGS
    constexpr uint8_t OLED_WIDTH = 128;
    constexpr uint8_t OLED_HEIGHT = 64;
    constexpr uint8_t OLED_ADDRESS = 0x3C;
}
//TEST SERIAL PROTOCOL
//8 byte 
/*
0xff -      start byte
0x00 -      length
0x01-0x40 - command {system, buttons, volume, display}
0x00-0xff -      payload{type logs, num button, canhed?, ...}
0x -        CRC
0xaa -      end byte


BUTTONS EXAMPLE

0xff - start
0x02     length
0x01 - buttons
0x0000_0001 - 0 button
0xcrc  (0x10, 0x01)
0xAA -end

*/ 
#endif