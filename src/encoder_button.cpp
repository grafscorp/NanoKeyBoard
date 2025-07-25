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

uint8_t EncoderButton::getEncoderModuleSteps()
{
    return encoder.getModuleSteps();
}
int8_t EncoderButton::getEncoderDirection() 
{
    return encoder.getDirection();
}

const uint8_t EncoderButton::getSerialData() const
{
    return data;
}

const uint8_t EncoderButton::getSerialCommand() const
{
    return ProtocolConfig::CMD_VOLUME;
}
void EncoderButton::updateSerialData() {
    data = 0x00;
    //0b01 000000
    if(getButtonIsPressed()) data += 0x40;
    //0b00 01 00000 - rigth
    if(getEncoderDirection()<0) data+= 0x20;
    //0b00 10 0000 - left
    else if (getEncoderDirection()>0) data +=0x30;

    const uint8_t encoderSteps = encoder.getModuleSteps();
    data+= encoderSteps>0b1111?0b1111:encoderSteps;

}
