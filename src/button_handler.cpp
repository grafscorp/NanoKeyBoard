#include "button_handler.hpp"

ButtonHandler::ButtonHandler() {}


void ButtonHandler::init()
{
    for (auto buttonPin : ButtonsConfig::BUTTON_PINS)
    {
        pinMode(buttonPin, INPUT_PULLUP);
    }
    
}

void ButtonHandler::update()
{

}