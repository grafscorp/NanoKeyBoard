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

namespace DisplayConfig{
    // Система сообщений
    constexpr uint8_t MaxMessageQueueSize = 5;    // Размер очереди сообщений
    constexpr uint16_t DefaultMessageDuration = 2000; // Время показа сообщения (мс)
    constexpr uint16_t ScrollAnimationDelay = 30;  // Задержка анимации (мс)
    
    // Энергосбережение
    constexpr uint16_t ScreenTimeout = 30000;      // Таймаут отключения экрана (мс)
    constexpr uint8_t DimmingBrightness = 16;      // Яркость в режиме энергосбережения
    
    // Шрифты
    constexpr uint8_t TitleFontSize = 2;
    constexpr uint8_t BodyFontSize = 1;
    
    // Стандартные экраны
    enum class DefaultScreen {
        MainStatus,
        ButtonAssignment,
        SystemInfo
    };
}
namespace ProtocolConfig {
    // Структура пакета
    constexpr uint8_t PacketStartMarker = 0xAA;
    constexpr uint8_t PacketEndMarker = 0x55;
    constexpr uint8_t MaxPacketSize = 64;
    constexpr uint8_t HeaderSize = 4; // [Start][Command][Length][Flags]
    
    // Команды
    enum class Command : uint8_t {
        // Входящие команды (PC -> Arduino)
        SetButtonAction = 0x10,   // Установить действие кнопки
        SetDisplayText = 0x11,    // Установить текст дисплея
        SaveConfig = 0x12,         // Сохранить конфигурацию
        ResetDevice = 0x13,        // Сбросить устройство
        GetStatus = 0x14,          // Запрос статуса
        
        // Исходящие команды (Arduino -> PC)
        ButtonEvent = 0x80,        // Событие кнопки
        DisplayState = 0x81,       // Состояние дисплея
        ConfigAck = 0x82,          // Подтверждение конфигурации
        ErrorReport = 0x83         // Отчет об ошибке
    };
    
}

#endif