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
    buttonChanged = (button.isPressed() || button.isReleased());
     
}
bool EncoderButton::getButtonIsPressed()
{
    return button.isHeld();
}

bool EncoderButton::getButtonStateIsChanged()
{
    return buttonChanged;
}
bool EncoderButton::getEncoderChanged() 
{
    return encoder.changed();
}
int8_t EncoderButton::getEncoderSteps() 
{
    return encoder.getSteps();
}

int8_t EncoderButton::getEncoderDirection() 
{
    return encoder.getDirection();
}
