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

    

    for (int i = ButtonsConfig::BUTTONS_COUNT-1; i >=0; --i)
    {
        buttons[i].update();

        if(buttons[i].isPressed() || buttons[i].isReleased()) 
        {
            isChanged = true;
        }
        
    }
    if(isChanged) updateData();
   
}

const uint8_t ButtonHandler::getSerialData() const
{
    return buttonsData;
}

const uint8_t ButtonHandler::getSerialCommand() const
{
    return ProtocolConfig::CMD_BUTTONS;
}

bool ButtonHandler::hasChanged()
{
    return this->isChanged;
}

void ButtonHandler::setChanged(const bool newState) {
    isChanged = newState;
}
void ButtonHandler::updateData() {
    this->buttonsData = 0U;
    for (auto& button : buttons)
    {
        buttonsData <<= 1U;
        buttonsData |= ( static_cast<uint8_t>(button.isHeld()) & 1U);
    }
    
}
