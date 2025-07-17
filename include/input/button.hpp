#ifndef BUTTON_HPP
#define BUTTON_HPP
#include <Arduino.h>

class Button
{
public:
    explicit Button(const uint8_t pin,const uint8_t mode= INPUT);
    Button() =default;
    virtual bool isHeld() =0;
    virtual bool isPressed() = 0;
    virtual bool isReleased() = 0;
protected:
    uint8_t pin;
};



#endif