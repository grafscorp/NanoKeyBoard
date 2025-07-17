#include "button_handler.hpp"

ButtonHandler::ButtonHandler() {
    //Ставим данные кнопок в положенние Released - не нажаты
    for (auto& buttonPressed : buttonsPressed)
    {
        buttonPressed = false;
    }
     


}


void ButtonHandler::init(const bool initPullUp)
{
    for (auto buttonPin : ButtonsConfig::BUTTON_PINS)
    for (auto buttonPin : HardwareConfig::BUTTON_PINS)
    {
        pinMode(buttonPin, initPullUp?INPUT_PULLUP:INPUT );
    }
    
}

void ButtonHandler::update()
{
    for (uint8_t i = 0; i < ButtonsConfig::BUTTONS_COUNT; i++)
    {
        //Записываем состояние кнопки, так как по умолчанию стоит подтягивающий резистор, значение true с кнопки означает что она не нажата
        buttonsPressed[i] = !(static_cast<bool>(digitalRead(ButtonsConfig::BUTTON_PINS[i])));
    }
    
    
}

uint8_t ButtonHandler::getButtonsStateData() {
    return buttonData;
}
