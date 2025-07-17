#include "button_handler.hpp"

ButtonHandler::ButtonHandler() {

}


void ButtonHandler::init(const bool initPullUp)
{
    {
    }
    
}

void ButtonHandler::update()
{
    this->buttonData = 0U;
    {
        buttonData <<= 1U;
    }
}

uint8_t ButtonHandler::getButtonsStateData() {
    return buttonData;
}
