#ifndef DEBOUNCED_BUTTON_HPP
#define DEBOUNCED_BUTTON_HPP

#include <Arduino.h>
#include <input/button.hpp>

class DebouncedButton : public Button
{
public:
    explicit DebouncedButton(const uint8_t pin, const unsigned long debounceDelay = 50);
    DebouncedButton() =default;
    void init(const bool = true);
    void init(const uint8_t pin, const unsigned long debounceDelay = 50,const bool = true );
    void update();

    bool isPressed() override;
    bool isReleased()override;
    bool isHeld()override;

protected:
    unsigned long lastDebounceTime = 0;
    unsigned long debounceDelay=50;
    bool lastState = HIGH;
    bool stableState = HIGH;
    bool changed = false;
};



#endif