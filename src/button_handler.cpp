#include "input/button_handler.hpp"


ButtonHandler::ButtonHandler() {

}


void ButtonHandler::init(const bool initPullUp)
{
    // for (auto buttonPin : HardwareConfig::BUTTON_PINS)
    // {
    //     pinMode(buttonPin, initPullUp?INPUT_PULLUP:INPUT );
    // }
    
    for (uint8_t i =0; i < ButtonsConfig::BUTTONS_COUNT; ++i)
    {
        buttons[i].init(HardwareConfig::BUTTON_PINS[i], ButtonsConfig::DebounceDelay, initPullUp);
    }
    
}

void ButtonHandler::update()
{

    this->buttonData = 0U;

    for (int i = ButtonsConfig::BUTTONS_COUNT-1; i >=0; --i)
    {
        buttons[i].update();

        buttonData <<= 1U;
        buttonData |= ( static_cast<uint8_t>(buttons[i].isHeld()) & 1U);
    }
   
}

uint8_t ButtonHandler::getSerialData()
{
    return buttonData;
}
