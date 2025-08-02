#ifndef DISPLAY_MANAGER_HPP
#define DISPLAY_MANAGER_HPP

#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include "config.hpp"
#include "bitmap/BitMaps.hpp"

class DisplayManager final
{
public:
    explicit DisplayManager(const uint8_t width=128,const uint8_t heigth=64,TwoWire * wire = &Wire ,const int8_t rstPin = -1 );


    enum DisplayState{
        IDLE,
        SYSTEM,
        MEDIA
    };
    bool init(const uint8_t addr = 0x3C);
    void update();
    void drawIDLE();
private:
    Adafruit_SSD1306 display;
    bool isInit = false;

    DisplayState currentState = IDLE;
    uint8_t width;
    uint8_t heigth;
    uint8_t addresDisplay;

    
};



#endif

