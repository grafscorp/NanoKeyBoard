#include "input/encoder_button.hpp"



EncoderButton::EncoderButton(const uint8_t pinButton, const uint8_t pinA, const uint8_t pinB, const unsigned long debounceButtonDelay):encoder(pinA, pinB), button(pinButton, debounceButtonDelay)
{
 
}

void EncoderButton::init()
{
    button.init();
    encoder.init();
}

void EncoderButton::update()
{
    button.update();
}
bool EncoderButton::getButtonChanged()
{
    return(button.isPressed() || button.isReleased());
}