#include "input/button.hpp"

Button::Button(const uint8_t pin, const uint8_t mode ):pin(pin) {
    pinMode(this->pin, mode);
}

